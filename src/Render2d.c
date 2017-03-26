/*
	Render2d.c

	Copyright 2017 (c) Michael Cabral.

	This software is free to use under the terms of the BSD-3 license.

	Please refer to the LICENSE file included with this source code. You can
	also find details here: https://opensource.org/licenses/BSD-3-Clause
*/

// Standard C boilerplate.
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

// Our system should work on anything that supports OpenGL/ES 2.1+
#if defined(__APPLE__) && defined(__MACH__)
	// include opengl
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
	#include <OpenGL/glext.h>
// Both windows and linux should build with the same headers.
#elif defined(_WIN32) || defined(__linux__)
	// Include opengl
	#include <GL/glew.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#else
	#error unsupported platform!
#endif

// Our little logging utility.
#include "rLogger.h"
#include "rGLCheck.h"

// This is where the party is at.
#include "Render2d.h"

// There are 2 triangles per quad, that's 3 verts per triangle, 4 vertex
//	elements per vert. 2 * 3 * 4 = 24.
#define NUM_BATCH_BUFFER_VERTS (R_BATCH_BUFFER_SIZE * 24)

// Vertex attrib ids used for our shaders.
enum
{
	ATTRIB_VERTEX,
	ATTRIB_TEXTURE
};

// Name constants for our shader vertex attributes.
#define ATTRIB_VERTEX_NAME "vertex"
#define ATTRIB_TEX_COORD_NAME "tex_coord"

// Name constonts for shader uniforms.
#define TEX0_UNIFORM_NAME "tex0"

// Amount of vbos we cycle through.
#define NUM_VBOS 6

// Little struct to store shader text.
struct ShaderText
{
	const char *vert, *frag;
};

// The standard shader program our little engine will use for now.
static struct ShaderText StdShaderText =
{
	.vert =
	"#version 120\n"
	"attribute vec4 vertex;"
	"attribute vec2 tex_coord;"
	"varying vec2 v_tex_coord;"
	"void main()"
	"{"
		"v_tex_coord = tex_coord;"
		"gl_Position = vertex;"
	"}",

	.frag =
	"#version 120\n"
	"uniform sampler2D tex0;"
	"varying vec2 v_tex_coord;"
	"void main()"
	"{"
		"vec4 tex_color = texture2D(tex0, v_tex_coord);"
		"if(tex_color.a == 0.0)"
			"discard;"
		"gl_FragColor = tex_color;"
	"}"
};

// OpenGL data for the standard shader used in the engine.
static struct StdShader
{
	GLuint shaderId;
	GLint tex0Loc;
}
StdShader =
{
	0, 0
};

// Materials represent the current visual settings of the game.
struct Material
{
	unsigned int texId;
};

// The renderer batches quads into little segments of geometry called a
//	"DrawCall". This data structure represents one of those segments.
struct DrawCall
{
	unsigned int startIndex;
	unsigned int numElements;
	struct Material material;
};

// The current material state of the renderer.
struct Material CurMaterialState;

// The draw call buffer.
static struct DrawCall DrawCalls[R_MAX_DRAW_CALLS];

// This flag is set if we need to generate a new draw call.
static bool NeedsANewDrawCall = false;

// This keeps track of how many draw calles have been buffered so far.
static unsigned int NumDrawCalls = 0;

// This keeps track of the last vertex in the vbo that was changed.
static unsigned int LastChangeIndex = 0;

// Mapped pointer to the vbo data. If it's NULL, we aren't batching.
static GLfloat *VertexBufferMappedPtr = NULL;

static bool IsBatching = false;
// static GLfloat *VertexBufferData = NULL;

// Current vbo we are using.
static unsigned int CurVertexBufferIndex = 0;

// Id for the vertex array buffer. We cycle through mutliple vbos for perf.
//	reasons.
static GLuint VertexBufferIds[NUM_VBOS] = { 0, 0, 0, 0, 0, 0 };

// Value retrived on init.
static float MaxAniostrophyLevel = 0.0f;

// These values are used to 'project' points into the viewport as they come in
//	to the draw function.
static float HalfViewportW = 0.0f;
static float HalfViewportH = 0.0f;
static float ViewPortScaleW = 0.0f;
static float ViewPortScaleH = 0.0f;

// This function initializes materials to their default values.
static void InitMaterial(struct Material *m)
{
	m->texId = 0;
}

static void CompileShaderText (
	GLuint programId, GLuint shaderId, const char *str
)
{
	glShaderSource(shaderId, 1, &str, NULL);
	glCompileShader(shaderId);
	glAttachShader(programId, shaderId);
}

void rInit()
{
// Init glew if we are on windows or linux.
#if defined(_WIN32) || defined(__linux__)
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if(err != GLEW_OK)
	{
		rLogError("%s\n", glewGetErrorString(err));
		exit(-1);
	}
	// Make sure we have a compatible version of OpenGL available
	if (GLEW_VERSION_2_1)
	{
		rLogInfo("OpenGL 2.1 is supported.");
	}
	else
	{
		rLogError("This program requires at least OpenGL 2.1 compatible"
			"hardware.");
		exit(-1);
	}
#endif

	// Set up OpenGL to its default values.
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_MULTISAMPLE);
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &MaxAniostrophyLevel);

// Enable the apple flush buffer extenstion if available:
// https://developer.apple.com/library/content/documentation/GraphicsImaging/
//	Conceptual/OpenGL-MacProgGuide/opengl_vertexdata/opengl_vertexdata.html
/*#if defined(__APPLE__) && defined(__MACH__)
	glBufferParameteriAPPLE (
		GL_ARRAY_BUFFER, GL_BUFFER_FLUSHING_UNMAP_APPLE, GL_FALSE
	);
#endif*/

	VertexBufferMappedPtr = malloc(sizeof(GLfloat) * NUM_BATCH_BUFFER_VERTS);
	if(!VertexBufferMappedPtr)
	{
		rLogError("Failed to allocate memory for VertexBufferMappedPtr!");
		exit(-1);
	}

	for(int i = 0; i < NUM_VBOS; i++)
	{
		// Create the vertex buffer.
		glGenBuffers(1, &VertexBufferIds[i]);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferIds[i]);
		/*glBufferData (
			GL_ARRAY_BUFFER,
			sizeof(GLfloat) * NUM_BATCH_BUFFER_VERTS,
			NULL,
			GL_STREAM_DRAW
		);*/
	}

	// Create the standard shader.
	StdShader.shaderId = glCreateProgram();
	GLuint vertShaderId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	CompileShaderText(StdShader.shaderId, vertShaderId, StdShaderText.vert);
	CompileShaderText(StdShader.shaderId, fragShaderId, StdShaderText.frag);
	glDeleteShader(vertShaderId);
	glDeleteShader(fragShaderId);
	glBindAttribLocation (
		StdShader.shaderId,
		ATTRIB_VERTEX,
		ATTRIB_VERTEX_NAME
	);
	glBindAttribLocation (
		StdShader.shaderId,
		ATTRIB_TEXTURE,
		ATTRIB_TEX_COORD_NAME
	);
	glLinkProgram(StdShader.shaderId);
	StdShader.tex0Loc = glGetUniformLocation (
		StdShader.shaderId, TEX0_UNIFORM_NAME
	);

	// Log friendly init message.
	rLogInfo("Render2d initialized.")
}

void rQuit()
{
	for(int i = 0; i < NUM_VBOS; i++)
	{
		glDeleteBuffers(1, &VertexBufferIds[i]);
	}
	glDeleteProgram(StdShader.shaderId);
}

void rSetViewport(unsigned w, unsigned h)
{
	HalfViewportW = (float)w * 0.5f;
	HalfViewportH = (float)h * 0.5f;
	ViewPortScaleW = 2.0f/(float)w;
	ViewPortScaleH = 2.0f/(float)h;
	glViewport(0, 0, w, h);
}

void rSetClearColor(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

void rClear()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void rBegin()
{
#ifndef R_NO_ERROR_CHECKING
	// Abort begin if already batching.
	if(IsBatching)
	{
		rLogWarning("Aborting rBegin. Batch already started!");
		return;
	}
#endif

	NeedsANewDrawCall = true;
	glEnableVertexAttribArray(ATTRIB_VERTEX);
	glEnableVertexAttribArray(ATTRIB_TEXTURE);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferIds[CurVertexBufferIndex]);
	if(++CurVertexBufferIndex >= NUM_VBOS) CurVertexBufferIndex = 0;
	/*VertexBufferMappedPtr =
		(GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);*/
	glVertexAttribPointer (
		ATTRIB_VERTEX,
		2,
		GL_FLOAT,
		0,
		sizeof(GLfloat) * 4,
		0
	);
	glVertexAttribPointer (
		ATTRIB_TEXTURE,
		2,
		GL_FLOAT,
		0,
		sizeof(GLfloat) * 4,
		(void*)(sizeof(GLfloat) * 2)
	);
	DrawCalls[0].startIndex = 0;
	DrawCalls[0].numElements = 0;
	InitMaterial(&DrawCalls[0].material);
	IsBatching = true;
}

unsigned int rCreateTexture (
	unsigned int w, unsigned int h, unsigned int n, unsigned char *pixels
)
{
#ifndef R_NO_ERROR_CHECKING
	// Prevent the creation of textures during a batch.
	if(IsBatching)
	{
		rLogWarning("Aborting rCreateTexture. Cannot create textures while "
			"batching.");
		return 0;
	}
#endif

	GLuint texId = 0;
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	switch(n)
	{
		case 4:
			rLogInfo("Created an RGBA texture: %u", texId);
			glTexImage2D(GL_TEXTURE_2D, 0, n, w, h, 0, GL_RGBA,
				GL_UNSIGNED_BYTE, pixels);
			break;
		case 3:
			rLogInfo("Created an RGB texture: %u", texId);
			glTexImage2D(GL_TEXTURE_2D, 0, n, w, h, 0, GL_RGB,
				GL_UNSIGNED_BYTE, pixels);
			break;
		default:
			rLogWarning("Unknown value for texure elements: %u\n", n);
			glDeleteTextures(1, &texId);
			return 0;
			break;
	}
	return (unsigned int)texId;
}

void rDestroyTexture(unsigned int texId)
{
#ifndef R_NO_ERROR_CHECKING
	// Prevent the destruction of textures during a batch.
	if(IsBatching)
	{
		rLogWarning("Aborting rDestroyTexture. Cannot destroy textures while "
			"batching.");
		return;
	}
#endif
	glDeleteTextures(1, &texId);
}

unsigned int PendingTextureId = 0;

void rSetTexture(unsigned int texId)
{
#ifndef R_NO_ERROR_CHECKING
	// Prevent the setting a texture outside of a batch.
	if(!IsBatching)
	{
		rLogWarning("Aborting rDestroyTexture. Cannot destroy textures while "
			"batching.");
		return;
	}

	// Error out if we pass in a zero texture id.
	if(!texId)
	{
		rLogWarning("Aborting rDestroyTexture. texId is zero!");
		return;
	}
#endif
	PendingTextureId = texId;
	NeedsANewDrawCall = true;
}

// This is the function that actually draws the draw calls.
static inline void Flush()
{
// Use the apple flush buffer extenstion first if available:
// https://developer.apple.com/library/content/documentation/GraphicsImaging/
//	Conceptual/OpenGL-MacProgGuide/opengl_vertexdata/opengl_vertexdata.html
/*#if defined(__APPLE__) && defined(__MACH__)
	glFlushMappedBufferRangeAPPLE (
		GL_ARRAY_BUFFER,
		0,
		LastChangeIndex * sizeof(GLfloat)
	);
#endif*/

	// Unmap the array vuffer since we are no longer updating it.
	// glUnmapBuffer(GL_ARRAY_BUFFER);

	glBufferData (
		GL_ARRAY_BUFFER,
		sizeof(GLfloat) * LastChangeIndex,
		NULL,
		GL_STREAM_DRAW
	);
	glBufferData (
		GL_ARRAY_BUFFER,
		sizeof(GLfloat) * LastChangeIndex,
		VertexBufferMappedPtr,
		GL_STREAM_DRAW
	);
	// glBufferSubData(GL_ARRAY_BUFFER, 0, LastChangeIndex * sizeof(GLfloat), VertexBufferMappedPtr);

	// Set it to NULL.
	// VertexBufferMappedPtr = NULL;

	// Loop through all our draw calls.
	for(unsigned int i = 0; i < NumDrawCalls; i++)
	{
		// Activate the standard shader.
		glUseProgram(StdShader.shaderId);

		// Activate main tex channel.
		glActiveTexture(GL_TEXTURE0);

		// Enable texturing.
		glEnable(GL_TEXTURE_2D);

		// Bind our texture.
		glBindTexture(GL_TEXTURE_2D, DrawCalls[i].material.texId);

		// Set the texture params.
		glTexParameterf (
			GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, MaxAniostrophyLevel
		);
		glTexParameteri (
			GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST
		);
		glTexParameteri (
			GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST
		);

		// Set the tex0 uniform to 0.
		glUniform1i(StdShader.tex0Loc, 0);

		// Draw a segment of our geometry.
		glDrawArrays (
			GL_TRIANGLES, DrawCalls[i].startIndex, DrawCalls[i].numElements
		);
	}
}

void rDraw (
	float x, float y, float w, float h, float u, float v, float s, float t
)
{
#ifndef R_NO_ERROR_CHECKING
	// Abort draw if not batching.
	if(!IsBatching)
	{
		rLogWarning("Aborting rDraw. No batch not started!");
		return;
	}
#endif

	// If we need a new draw call, increment the counter and set the flag to
	//	false.
	if(NeedsANewDrawCall)
	{
#ifndef R_NO_ERROR_CHECKING
		// If don't have a texture, abort the draw call and log a warning.
		if(!PendingTextureId)
		{
			rLogWarning("Aborting rDraw. Material has no texture assigned!");
			return;
		}
#endif

		DrawCalls[NumDrawCalls].material.texId = PendingTextureId;

		// The start index should be the index of the last vertex we added.
		DrawCalls[NumDrawCalls].startIndex = (LastChangeIndex / 24) * 6;
		DrawCalls[NumDrawCalls].numElements = 0;
		NumDrawCalls++;
		NeedsANewDrawCall = false;
	}

	// If we are full, flush.
	if (
		LastChangeIndex >= NUM_BATCH_BUFFER_VERTS ||
		NumDrawCalls > R_MAX_DRAW_CALLS
	)
	{
		// Draw stuff.
		Flush();

		// Set the first draw call material to the last draw call material.
		DrawCalls[0].material = DrawCalls[NumDrawCalls-1].material;

		// Re-init the other fields in the draw call.
		DrawCalls[0].startIndex = 0;
		DrawCalls[0].numElements = 0;

		// Change index back to zero.
		LastChangeIndex = 0;

		// draw calls back to one since we are about to modify data.
		NumDrawCalls = 1;

		// Bind a new vertex buffer.
		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferIds[CurVertexBufferIndex]);
		if(++CurVertexBufferIndex >= NUM_VBOS) CurVertexBufferIndex = 0;

		// remap our buffer.
		/*VertexBufferMappedPtr =
			(GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);*/
	}

	// Scale the values into the viewport.
	float minx = (x - HalfViewportW) * ViewPortScaleW;
	float miny = (y - HalfViewportH) * ViewPortScaleH;
	float maxx = ((x + w) - HalfViewportW) * ViewPortScaleW;
	float maxy = ((y + h) - HalfViewportH) * ViewPortScaleH;

	/*

	This is the layout of verts:

	(0,1)----------(1,1)
	|                  |
	|                  |
	|                  |
	|                  |
	|                  |
	|                  |
	(0,0)----------(1,0)


	This is the way UVs are layed out:

	(u,v)----------(s,v)
	|                  |
	|                  |
	|                  |
	|                  |
	|                  |
	|                  |
	(u,t)----------(s,t)

	*/

	// Triangle 1:
	// Vert 1
	VertexBufferMappedPtr[LastChangeIndex++] = minx;
	VertexBufferMappedPtr[LastChangeIndex++] = miny;
	VertexBufferMappedPtr[LastChangeIndex++] = u;
	VertexBufferMappedPtr[LastChangeIndex++] = t;

	// Vert 2
	VertexBufferMappedPtr[LastChangeIndex++] = maxx;
	VertexBufferMappedPtr[LastChangeIndex++] = miny;
	VertexBufferMappedPtr[LastChangeIndex++] = s;
	VertexBufferMappedPtr[LastChangeIndex++] = t;

	// Vert 3
	VertexBufferMappedPtr[LastChangeIndex++] = maxx;
	VertexBufferMappedPtr[LastChangeIndex++] = maxy;
	VertexBufferMappedPtr[LastChangeIndex++] = s;
	VertexBufferMappedPtr[LastChangeIndex++] = v;

	// Triangle 2:
	// Vert 1
	VertexBufferMappedPtr[LastChangeIndex++] = maxx;
	VertexBufferMappedPtr[LastChangeIndex++] = maxy;
	VertexBufferMappedPtr[LastChangeIndex++] = s;
	VertexBufferMappedPtr[LastChangeIndex++] = v;

	// Vert 2
	VertexBufferMappedPtr[LastChangeIndex++] = minx;
	VertexBufferMappedPtr[LastChangeIndex++] = maxy;
	VertexBufferMappedPtr[LastChangeIndex++] = u;
	VertexBufferMappedPtr[LastChangeIndex++] = v;

	// Vert 3
	VertexBufferMappedPtr[LastChangeIndex++] = minx;
	VertexBufferMappedPtr[LastChangeIndex++] = miny;
	VertexBufferMappedPtr[LastChangeIndex++] = u;
	VertexBufferMappedPtr[LastChangeIndex++] = t;

	// Increment the current draw call by 6 verts.
	DrawCalls[NumDrawCalls-1].numElements += 6;
}

void rEnd()
{
#ifndef R_NO_ERROR_CHECKING
	// Abort draw if not batching.
	if(!IsBatching)
	{
		rLogWarning("Aborting rEnd. No batch not started!");
		return;
	}
#endif

	// If the array has been modified, flush it.
	if(LastChangeIndex)
	{
		Flush();
	}
	else
	{
		glBufferData (
			GL_ARRAY_BUFFER,
			sizeof(GLfloat) * LastChangeIndex,
			NULL,
			GL_STREAM_DRAW
		);
		glBufferData (
			GL_ARRAY_BUFFER,
			sizeof(GLfloat) * LastChangeIndex,
			VertexBufferMappedPtr,
			GL_STREAM_DRAW
		);
		//glBufferSubData(GL_ARRAY_BUFFER, 0, LastChangeIndex * sizeof(GLfloat), VertexBufferMappedPtr);
		// glUnmapBuffer(GL_ARRAY_BUFFER);
	}
	glDisableVertexAttribArray(ATTRIB_VERTEX);
	glDisableVertexAttribArray(ATTRIB_TEXTURE);

	// Reset all our state variables.
	NumDrawCalls = 0;
	LastChangeIndex = 0;
	PendingTextureId = 0;
	// VertexBufferMappedPtr = NULL;
	NeedsANewDrawCall = false;
	IsBatching = false;
}
