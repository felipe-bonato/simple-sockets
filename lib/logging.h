/**
 * TODO: Add log levels 
 */

#ifndef __LOGGING_H__
#define __LOGGING_H__

#define LOG_ENABLE true
#define LOG_LOCATION_INFO false
#define LOG_DEBUG true
#define LOG_LEVEL 3

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#if LOG_ENABLE
	#if LOG_LOCATION_INFO
		#define loginfo(msg) printf("[%s: %d][%s: %d] %s.\n", __func__, getpid(), __FILE__, __LINE__, msg);
		#define logerr(msg) printf("[%s: %d][%s: %d][ERROR] %s. %s.\n", __func__, getpid(), __FILE__, __LINE__, msg, strerror(errno));
		#if LOG_DEBUG
			#define logdebug(msg) printf("[%s: %d][%s: %d] %s.\n", __func__, getpid(), __FILE__, __LINE__, msg);
		#else
			#define logdebug(msg) // Do nothing
		#endif
	#else
		#define loginfo(msg) printf("[%s: %d] %s.\n", __func__, getpid(), msg);
		#define logerr(msg) printf("[%s: %d][ERROR] %s. %s.\n", __func__, getpid(), msg, strerror(errno));
		#if LOG_DEBUG
			#define logdebug(msg) printf("[%s: %d] %s.\n", __func__, getpid(), msg);
		#else
			#define logdebug(msg) // Do nothing
		#endif
	#endif
#endif

#endif //__LOGGING_H__