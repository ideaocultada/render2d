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

	AddOption (
		"--debug-build",
		dest = "debug-build",
		action = "store_true",
		default = False
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
