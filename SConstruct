"""
	Copyright 2017 (c) Michael Cabral.

	This software is free to use under the terms of the BSD-3 license.

	Please refer to the LICENSE file included with this source code. You can
	also find details here: https://opensource.org/licenses/BSD-3-Clause
"""

import sys
import glob
import os
import shutil

platform = sys.platform

if platform == "darwin":
	libs = [
	]

	frameworks = [
		"SDL2",
		"OpenGL"
	]

	defines = []

	env = Environment (
		ENV = {
			"PATH" : os.environ["PATH"]
		}
	)

	env.VariantDir("build", ".", 0)

	sources = Glob("build/src/*.c")
	sources += Glob("build/examples/Simple/*.c")

	env.Program (
		tools = ["default", "Xcode"],
		target = "examples/bin/Simple",
		source = sources,
		LIBS = libs,
		LIBPATH = [
			"/usr/local/lib"
		],
		CPPPATH = [
			"/usr/local/include"
		],
		CPPDEFINES = defines,
		CFLAGS = ["-O1", "-std=c11"],
		CXXFLAGS = ["-g", "-std=c++11"],
		FRAMEWORKPATH = [
			"/Library/Frameworks"
		],
		FRAMEWORKS = frameworks
	)


if platform == "win32":
	libs = [
		"mingw32",
		"SDL2main",
		"SDL2",
		"opengl32",
		"glu32"
	]

	defines = [
		"GLEW_STATIC"
	]

	env = Environment (
		tools = ["mingw"],
		ENV = {
			"PATH" : os.environ["PATH"]
		}
	)

	env.VariantDir("build", ".", 0)

	sources = Glob("build/glew/*.c")
	sources += Glob("build/src/*.c")
	sources += Glob("build/examples/Simple/*.c")

	env.Program (
		target = "examples/bin/Simple",
		source = sources,
		LIBS = libs,
		CPPPATH = [
			"glew"
		],
		CPPDEFINES = defines,
		CFLAGS = ["-O1", "-std=c11"],
		CXXFLAGS = ["-O1", "-std=c++11"]
	)
