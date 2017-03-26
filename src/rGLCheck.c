/*
	rGLCheck.c

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

void rDoGLCheck(const char *file, const char *func, unsigned int ln)
{
	GLenum errCode = glGetError();

	if(errCode == GL_NO_ERROR) return;

	switch(errCode)
	{
		case GL_INVALID_ENUM:
			rLogError("GL Error: GL_INVALID_ENUM\n '%s' %s():%u",
				file, func, ln);
			break;

		case GL_INVALID_VALUE:
			rLogError("GL Error: GL_INVALID_VALUE\n '%s' %s():%u",
				file, func, ln);
			break;

		case GL_INVALID_OPERATION:
			rLogError("GL Error: GL_INVALID_OPERATION\n '%s' %s():%u",
				file, func, ln);
			break;

		case GL_OUT_OF_MEMORY:
			rLogError("GL Error: GL_OUT_OF_MEMORY\n '%s' %s():%u",
				file, func, ln);
			break;

		case GL_INVALID_FRAMEBUFFER_OPERATION_EXT:
			rLogError("GL Error: GL_INVALID_FRAMEBUFFER_OPERATION_EXT\n"
				" '%s' %s():%u", file, func, ln);
			break;

		default:
			rLogError("GL Error: Unknown (%u)\n '%s' %s():%u",
				errCode, file, func, ln);
			break;
	}
}
