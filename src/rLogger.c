/*
	rLogger.c

	Copyright 2017 (c) Michael Cabral.

	This software is free to use under the terms of the BSD-3 license.

	Please refer to the LICENSE file included with this source code. You can
	also find details here: https://opensource.org/licenses/BSD-3-Clause
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "rLogger.h"

static FILE *LogFilePtr = NULL;

static char LogBuffer[R_LOG_BUFF_LEN] = "\0";

void rLogOpen(const char *path)
{
	LogFilePtr = fopen(path, "w");
	if(!LogFilePtr)
	{
		rLogError("Could not open LogFilePtr: %s", path);
	}
}

void rLogRaw(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vsnprintf(LogBuffer, R_LOG_BUFF_LEN, fmt, args);
	va_end(args);
	if(LogFilePtr) fprintf(LogFilePtr, "%s", LogBuffer);
	fprintf(stdout, "%s", LogBuffer);
	if(LogFilePtr) fflush(LogFilePtr);
}

void rLogPrint(int logType, const char *fmt, ...)
{
	switch(logType)
	{
		case R_LOG_INFO:
			if(LogFilePtr) fprintf(LogFilePtr, "[rInfo]: ");
			fprintf(stdout, "[rInfo]: ");
			break;
		case R_LOG_WARNING:
			if(LogFilePtr) fprintf(LogFilePtr, "[rWarning]: ");
			fprintf(stdout, "[rWarning]: ");
			break;
		case R_LOG_ERROR:
			if(LogFilePtr) fprintf(LogFilePtr, "[rError]: ");
			fprintf(stderr, "[rError]: ");
			break;
		default:
			break;
	}
	va_list args;
	va_start(args, fmt);
	vsnprintf(LogBuffer, R_LOG_BUFF_LEN, fmt, args);
	va_end(args);
	if(LogFilePtr)
	{
		fprintf(LogFilePtr, "%s\n", LogBuffer);
	}
	if(logType == R_LOG_ERROR)
	{
		fprintf(stderr, "%s\n", LogBuffer);
	}
	else
	{
		fprintf(stdout, "%s\n", LogBuffer);
	}
	if(LogFilePtr)
	{
		fflush(LogFilePtr);
	}
}

void rLogClose()
{
	if(LogFilePtr) fclose(LogFilePtr);
	LogFilePtr= NULL;
}
