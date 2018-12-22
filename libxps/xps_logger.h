//
//  xps_logger.h
//  XPS
//
//  Created by WizJin on 2018/12/22.
//

#ifndef __XPS_LOGGER_H__
#define __XPS_LOGGER_H__

#include "xps_config.h"
#include <time.h>
#include <stdio.h>
#include <errno.h>

#define LOG_LEVEL_NONE                  0x00
#define LOG_LEVL_ALL                    0xff
#define LOG_LEVEL_ERROR                 0x01
#define LOG_LEVEL_WARN                  0x02
#define LOG_LEVEL_INFO                  0x03
#define LOG_LEVEL_DEBUG                 0x04
#define LOG_LEVEL_TRACE                 0x05

#ifndef LOG_LEVEL
#   ifdef DEBUG
#       define LOG_LEVEL                LOG_LEVEL_INFO
#   else
#       define LOG_LEVEL                LOG_LEVEL_NONE
#   endif
#endif

#define LOG_FMT(_tag, _format)          "%s | " _tag " | " _format
#define log_none(_format, ...)          ((void)0)
#define log_output(_format, _args...)   \
    do {                                \
        char nowstr[32];                \
        struct tm ft;                   \
        time_t now = time(NULL);        \
        strftime(nowstr, sizeof(nowstr), "%Y-%m-%d %H:%M:%S", localtime_r(&now, &ft));  \
        fprintf(stderr, _format "\n", nowstr, ## _args);  \
    } while(0)

#if LOG_LEVEL >= LOG_LEVEL_ERROR
#   define log_error(_format, _args...) log_output(LOG_FMT("ERROR", _format), ## _args)
#else
#   define log_error                    log_none
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARN
#   define log_warn(_format, _args...)  log_output(LOG_FMT("WARN", _format), ## _args)
#else
#   define log_warn                     log_none
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#   define log_info(_format, _args...)  log_output(LOG_FMT("INFO", _format), ## _args)
#else
#   define log_info                     log_none
#endif

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#   define log_debug(_format, _args...) log_output(LOG_FMT("DEBUG", _format), ## _args)
#else
#   define log_debug                    log_none
#endif

#if LOG_LEVEL >= LOG_LEVEL_TRACE
#   define log_trace(_format, _args...) log_output(LOG_FMT("TRACE", _format), ## _args)
#else
#   define log_trace                    log_none
#endif



#endif /* __XPS_LOGGER_H__ */
