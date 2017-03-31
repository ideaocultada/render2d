/*
	rexLogger.h

	Copyright 2017 (c) Michael Cabral.

	This software is free to use under the terms of the BSD-3 license.

	Please refer to the LICENSE file included with this source code. You can
	also find details here: https://opensource.org/licenses/BSD-3-Clause
*/

#pragma once

// Maximum length of log a log string.
#define REX_LOG_BUFF_LEN 4086

// Log types.
enum
{
	REX_LOG_INFO,
	REX_LOG_WARNING,
	REX_LOG_ERROR
};

// Open a log file to keep a record of the engine behavior.
void rexLogOpen(const char *path);

// The base print function. You should probably use the macros below.
void rexLogPrint(int logType, const char *fmt, ...);

// Just a raw print function. It will print whatever you input to the logs.
void rexLogRaw(const char *fmt, ...);

// Close the log file.
void rexLogClose();

// Some macro wrappers to make things simple.
#define rexLogInfo(fmt, ...) \
	rexLogPrint(REX_LOG_INFO, fmt, ##__VA_ARGS__);
#define rexLogWarning(fmt, ...) \
	rexLogPrint(REX_LOG_WARNING, fmt, ##__VA_ARGS__);
#define rexLogError(fmt, ...) \
	rexLogPrint(REX_LOG_ERROR, fmt, ##__VA_ARGS__);
