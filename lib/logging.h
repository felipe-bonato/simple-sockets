/**
 * WARNING: This require non-standard GCC extensions.
 *          Needs flag `-Wno-variadic-macros` to work
 */

/**
 * TODO: Add log levels 
 */

#ifndef __LOGGING_H__
#define __LOGGING_H__

#define LOG_ENABLE 1
#define LOG_LOCATION_INFO 0
#define LOG_DEBUG 1
#define LOG_LEVEL 3

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#if LOG_ENABLE
	#if LOG_LOCATION_INFO
		#define loginfo(msg, ...) fprintf(stdout, "I [%s: %d][%s: %d] " msg ".\n", __func__, getpid(), __FILE__, __LINE__, msg, ##__VA_ARGS__);
		#define logerr(msg, ...) fprintf(stderr, "E [%s: %d][%s: %d] " msg ". %s.\n", __func__, getpid(), __FILE__, __LINE__, msg, strerror(errno), ##__VA_ARGS__);
		#if LOG_DEBUG
			#define logdebug(msg, ...) fprintf(stdout, "D [%s: %d][%s: %d] " msg ".\n", __func__, getpid(), __FILE__, __LINE__, msg, ##__VA_ARGS__);
		#else
			#define logdebug(msg, ...) // Do nothing
		#endif
	#else
		#define loginfo(msg, ...) fprintf(stdout, "I [%s: %d] " msg ".\n", __func__, getpid(), ##__VA_ARGS__);
		#define logerr(msg, ...) fprintf(stderr, "E [%s: %d] " msg ". %s.\n", __func__, getpid(), strerror(errno), ##__VA_ARGS__);
		#if LOG_DEBUG
			#define logdebug(msg, ...) fprintf(stdout, "D [%s: %d] " msg ".\n", __func__, getpid(), ##__VA_ARGS__);
		#else
			#define logdebug(msg, ...) // Do nothing
		#endif
	#endif
#else
	#define loginfo(msg) // Do nothing
	#define logerr(msg) // Do nothing
	#define logdebug(msg) // Do nothing
#endif

#endif //__LOGGING_H__