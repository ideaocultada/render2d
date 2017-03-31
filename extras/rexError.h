/*
	rexError.h

	Copyright 2017 (c) Michael Cabral.

	This software is free to use under the terms of the BSD-3 license.

	Please refer to the LICENSE file included with this source code. You can
	also find details here: https://opensource.org/licenses/BSD-3-Clause
*/

#pragma once

#define REX_ERR_BUFF_LEN 2048

// Various macros
#ifndef REX_NO_ASSERT
#define rexAssert(COND, ...) \
	if(!(COND)) REX_FATAL_ERROR("Assertion '" #COND "' failed! " \
			__VA_ARGS__);
#else
	#define rexAssert(COND, ...);
#endif

#define rexMemCheck(COND) \
	rexAssert(COND, "Memory allocation failed.");

#define rexNullCheck(COND) \
	rexAssert(COND, "Unexpected NULL pointer.");

#define rexFatalError(MSG) \
	rexHandleFatalErrorCallback(MSG, __FILE__, __PRETTY_FUNCTION__, __LINE__);

#define rexUnreachable() \
	rexFatalError("Unreachable code reached!");

#define rexUnimplemented() \
	rexFatalError("unimplemented code reached!");

// Our error handling function
void rexHandleFatalErrorCallback(const char *msg, const char *file,
	const char *func, unsigned int ln);

void rexSetFatalErrorCallback(void (*cback)(int err, const char *msg));
