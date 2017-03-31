/*
	rexLogger.c

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

static FILE *LogFilePtr = NULL;

static char LogBuffer[REX_LOG_BUFF_LEN] = "\0";

void rexLogOpen(const char *path)
{
	LogFilePtr = fopen(path, "w");
	if(!LogFilePtr)
	{
		rexLogError("Could not open LogFilePtr: %s", path);
	}
}

void rexLogRaw(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vsnprintf(LogBuffer, REX_LOG_BUFF_LEN, fmt, args);
	va_end(args);
	if(LogFilePtr) fprintf(LogFilePtr, "%s", LogBuffer);
	fprintf(stdout, "%s", LogBuffer);
	if(LogFilePtr) fflush(LogFilePtr);
}

void rexLogPrint(int logType, const char *fmt, ...)
{
	switch(logType)
	{
		case REX_LOG_INFO:
			if(LogFilePtr) fprintf(LogFilePtr, "[rInfo]: ");
			fprintf(stdout, "[rInfo]: ");
			break;
		case REX_LOG_WARNING:
			if(LogFilePtr) fprintf(LogFilePtr, "[rWarning]: ");
			fprintf(stdout, "[rWarning]: ");
			break;
		case REX_LOG_ERROR:
			if(LogFilePtr) fprintf(LogFilePtr, "[rError]: ");
			fprintf(stderr, "[rError]: ");
			break;
		default:
			break;
	}
	va_list args;
	va_start(args, fmt);
	vsnprintf(LogBuffer, REX_LOG_BUFF_LEN, fmt, args);
	va_end(args);
	if(LogFilePtr)
	{
		fprintf(LogFilePtr, "%s\n", LogBuffer);
	}
	if(logType == REX_LOG_ERROR)
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

void rexLogClose()
{
	if(LogFilePtr) fclose(LogFilePtr);
	LogFilePtr= NULL;
}
