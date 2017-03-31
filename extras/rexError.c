/*
	rexError.c

	Copyright 2017 (c) Michael Cabral.

	This software is free to use under the terms of the BSD-3 license.

	Please refer to the LICENSE file included with this source code. You can
	also find details here: https://opensource.org/licenses/BSD-3-Clause
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "rexLogger.h"
#include "rexError.h"

static char ErrBuff[REX_ERR_BUFF_LEN] = "\0";

static void (*FatalErrorCallbackPtr)(int err, const char *msg) = NULL;

void rexSetFatalErrorCallback(void (*cback)(int err, const char *msg))
{
	FatalErrorCallbackPtr = cback;
}

void rexHandleFatalErrorCallback(const char *msg, const char *file,
	const char *func, unsigned int ln)
{
	snprintf
	(
		ErrBuff,
		REX_ERR_BUFF_LEN,
		"Fatal Error:\n"
		"    Message: %s\n"
		"    File: %s\n"
		"    Func: %s\n"
		"    Line: %u\n",
		msg,
		file,
		func,
		ln
	);
	rexLogError(ErrBuff);
	if(FatalErrorCallbackPtr)
	{
		FatalErrorCallbackPtr(-1, ErrBuff);
	}
	else
	{
		exit(-1);
	}
}
