/*
	rGLCheck.h

	Copyright 2017 (c) Michael Cabral.

	This software is free to use under the terms of the BSD-3 license.

	Please refer to the LICENSE file included with this source code. You can
	also find details here: https://opensource.org/licenses/BSD-3-Clause
*/

#pragma once

void rDoGLCheck(const char *file, const char *func, unsigned int ln);

#ifndef R_NO_GLCHECK
	#define rGLCheck() \
		rDoGLCheck(__FILE__, __PRETTY_FUNCTION__, __LINE__)
	#else
		#define rGLCheck()
#endif
