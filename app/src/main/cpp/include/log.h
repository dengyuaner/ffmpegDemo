//
// Created by Jonesx on 2015/10/23.
//

#ifndef _LOG_H
#define _LOG_H

#define LOGN (void) 0

#ifndef WIN32
#include <android/log.h>

#define LOG_VERBOSE		1
#define LOG_DEBUG		2
#define LOG_INFO		3
#define LOG_WARNING		4
#define LOG_ERROR		5
#define LOG_FATAL		6
#define LOG_SILENT		7

#ifndef LOG_TAG
#define LOG_TAG __FILE__
#endif

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_VERBOSE
#endif

#define LOGP(level, fmt, ...) \
        __android_log_print(level, LOG_TAG, "%s:" fmt, \
            __PRETTY_FUNCTION__, ##__VA_ARGS__)

#if LOG_VERBOSE >= LOG_LEVEL
#define LOGV(fmt, ...) \
        LOGP(ANDROID_LOG_VERBOSE, fmt, ##__VA_ARGS__)
#else
#define LOGV(...) LOGN
#endif

#if LOG_DEBUG >= LOG_LEVEL
#define LOGD(fmt, ...) \
        LOGP(ANDROID_LOG_DEBUG, fmt, ##__VA_ARGS__)
#else
#define LOGD(...) LOGN
#endif

#if LOG_INFO >= LOG_LEVEL
#define LOGI(fmt, ...) \
        LOGP(ANDROID_LOG_INFO, fmt, ##__VA_ARGS__)
#else
#define LOGI(...) LOGN
#endif

#if LOG_WARNING >= LOG_LEVEL
#define LOGW(fmt, ...) \
        LOGP(ANDROID_LOG_WARN, fmt, ##__VA_ARGS__)
#else
#define LOGW(...) LOGN
#endif

#if LOG_ERROR >= LOG_LEVEL
#define LOGE(fmt, ...) \
        LOGP(ANDROID_LOG_ERROR, fmt, ##__VA_ARGS__)
#else
#define LOGE(...) LOGN
#endif

#if LOG_FATAL >= LOG_LEVEL
#define LOGF(fmt, ...) \
        LOGP(ANDROID_LOG_FATAL, fmt, ##__VA_ARGS__)
#else
#define LOGF(...) LOGN
#endif

#if LOG_FATAL >= LOG_LEVEL
#define LOGA(condition, fmt, ...) \
    if (!(condition)) \
    { \
        __android_log_assert(condition, LOG_TAG, "(%s:%u) %s: error:%s " fmt, \
            __FILE__, __LINE__, __PRETTY_FUNCTION__, condition, ##__VA_ARGS__); \
    }
#else
#define LOGA(...) LOGN
#endif

#else
#include <stdio.h>

#define LOGP(fmt, ...) printf("%s line:%d " fmt, __FILE__, __LINE__, ##__VA_ARGS__)

#define LOGV(fmt, ...) LOGP(fmt, ##__VA_ARGS__)

#define LOGD(fmt, ...) LOGP(fmt, ##__VA_ARGS__)

#define LOGI(fmt, ...) LOGP(fmt, ##__VA_ARGS__)

#define LOGW(fmt, ...) LOGP(fmt, ##__VA_ARGS__)

#define LOGE(fmt, ...) LOGP(fmt, ##__VA_ARGS__)

#define LOGF(fmt, ...) LOGP(fmt, ##__VA_ARGS__)

#define LOGA(...) LOGN

#endif // ANDROID_PROJECT

#endif // _LOG_H