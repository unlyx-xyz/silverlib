/* 
Silver Library project - Made by: unlyx.xyz
04-25-2026
Version: 1.1.0
*/

#ifndef SILVER_LIB_H
#define SILVER_LIB_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#define TINT_LIB_IMPLEMENTATION
#include "deps/tintlib/tintlib.h"

/*
======================
==###DECLARATIONS###==
======================
*/

// LOGGING ------------------------------

typedef enum {
    LOGL_TRACE = 0,
    LOGL_DEBUG = 1, 
    LOGL_INFO = 2,
    LOGL_WARN = 3,
    LOGL_ERROR = 4,
    LOGL_FATAL = 5,
    LOGL_DISABLED = 6,
} LOG_LEVEL;

extern TL_Sequence _log_color[];

// NETWORKING -> SOCKETS ----------------

typedef struct {
    int enable;
    int idle;
    int interval;
    int probes;
} keepalive_settings_t;

typedef struct {
    int reuseaddr;
    int reuseport;
    keepalive_settings_t keepalive; 
} sock_opt_t;

void SLInitLib(void);
int SLCreateUDPIPv4Socket(sock_opt_t *opts);
int SLCreateTCPIPv4Socket(sock_opt_t *opts);
int SLCloseSocket(int fd);

/*
========================
==###IMPLEMENTATION###==
========================
*/

#ifdef SILVER_LIB_IMPLEMENTATION

TL_Sequence _log_color[] = {
    (TL_Sequence){0},
    (TL_Sequence){0},
    (TL_Sequence){0},
    (TL_Sequence){0},
    (TL_Sequence){0},
};

void SLInitLib(void) {

    TL_Sequence graphics_settings = {0};
    graphics_settings.da.bold = true;
    graphics_settings.da.underline = true;

    uint8_t log_color_elements = sizeof(_log_color) / sizeof(TL_Sequence);
    for (uint8_t i = 0; i < log_color_elements; i++) {
        TL_CopyGraphics(graphics_settings, &_log_color[i]);
        _log_color[i].c8bit.background = -1;
    }

    _log_color[LOGL_TRACE].c8bit.foreground = 255;
    _log_color[LOGL_DEBUG].c8bit.foreground = 226;
    _log_color[LOGL_INFO].c8bit.foreground = 45;
    _log_color[LOGL_WARN].c8bit.foreground = 208;
    _log_color[LOGL_ERROR].c8bit.foreground = 196;
    _log_color[LOGL_FATAL].c8bit.foreground = 232;

}

#ifndef SILVER_LIB_LOGL
#define SILVER_LIB_LOGL LOGL_DISABLED
#endif

static inline void _sl_log(LOG_LEVEL logl, const char* file, 
             int line, const char* fnc, 
             const char* fmt, ...)
{
    if (SILVER_LIB_LOGL == LOGL_DISABLED || logl < SILVER_LIB_LOGL) return;
    static const char* logl_presentations[] = {"TRACE", "DEBUG", "INFO ", "WARN ", "ERROR", "FATAL"};
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    fprintf(stderr, "| %d:%d:%d.%03ld | ", tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec, ts.tv_nsec / 1000000);
    TL_fprintfc8bit(stderr, _log_color[logl], "[%s] %s:%d:%s:", logl_presentations[logl], file, line, fnc);
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");
}

#define _sl_log_trace(...) _sl_log(LOGL_TRACE, __FILE__, __LINE__, __func__, ##__VA_ARGS__);
#define _sl_log_debug(...) _sl_log(LOGL_DEBUG, __FILE__, __LINE__, __func__, ##__VA_ARGS__);
#define _sl_log_info(...)  _sl_log(LOGL_INFO,  __FILE__, __LINE__, __func__, ##__VA_ARGS__);
#define _sl_log_warn(...)  _sl_log(LOGL_WARN,  __FILE__, __LINE__, __func__, ##__VA_ARGS__);
#define _sl_log_error(...) _sl_log(LOGL_ERROR, __FILE__, __LINE__, __func__, ##__VA_ARGS__);
#define _sl_log_fatal(...) _sl_log(LOGL_FATAL, __FILE__, __LINE__, __func__, ##__VA_ARGS__);

#define opthandler(enabled, fd, lvl, opt, val, size)\
        do {\
            _sl_log_trace("Entering opthandler macro with values: %d, %d, %d, %d, %d, %d", enabled, fd, lvl, opt, val, size);
            if (enabled) {\
                if (setsockopt(fd, lvl, opt, val, size) < 0) {\
                    _sl_log_error("%s", strerror(errno));\
                    return -1;\
                }\
                _sl_log_info(#opt " applied successfully");\
            }\
        } while (0)\

int SLCreateUDPIPv4Socket(sock_opt_t *opts) {
    _sl_log_trace("Entering function with values: opts:(%d, %d, keepalive: %d, %d, %d, %d)", opts->reuseaddr, opts->reuseport, opts->keepalive.enable, opts->keepalive.idle, opts->keepalive.interval, opts->keepalive.probes);
    int sock;
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        _sl_log_error("Failed creating socket | %s", strerror(errno));
        return -1;
    }
    _sl_log_info("Socket created successfully");
    opthandler(opts->reuseaddr, sock, SOL_SOCKET, SO_REUSEADDR, &opts->reuseaddr, sizeof(opts->reuseaddr));
    opthandler(opts->reuseport, sock, SOL_SOCKET, SO_REUSEPORT, &opts->reuseport, sizeof(opts->reuseport));
    _sl_log_trace("Leaving function with values: opts:(%d, %d, keepalive: %d, %d, %d, %d)", opts->reuseaddr, opts->reuseport, opts->keepalive.enable, opts->keepalive.idle, opts->keepalive.interval, opts->keepalive.probes);
    return sock;
};
int SLCreateTCPIPv4Socket(sock_opt_t *opts) {
    _sl_log_trace("Entering function with values: opts:(%d, %d, keepalive: %d, %d, %d, %d)", opts->reuseaddr, opts->reuseport, opts->keepalive.enable, opts->keepalive.idle, opts->keepalive.interval, opts->keepalive.probes);
    int sock;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        _sl_log_error("Failed creating socket | %s", strerror(errno));
        return -1;
    }
    _sl_log_info("Socket created successfully");
    opthandler(opts->reuseaddr,         sock, SOL_SOCKET,   SO_REUSEADDR,   &opts->reuseaddr,           sizeof(opts->reuseaddr));
    opthandler(opts->reuseport,         sock, SOL_SOCKET,   SO_REUSEPORT,   &opts->reuseport,           sizeof(opts->reuseport));
    opthandler(opts->keepalive.enable,  sock, SOL_SOCKET,   SO_KEEPALIVE,   &opts->keepalive.enable,    sizeof(opts->keepalive.enable));
    opthandler(opts->keepalive.enable,  sock, IPPROTO_TCP,  TCP_KEEPIDLE,   &opts->keepalive.idle,      sizeof(opts->keepalive.interval));
    opthandler(opts->keepalive.enable,  sock, IPPROTO_TCP,  TCP_KEEPINTVL,  &opts->keepalive.interval,  sizeof(opts->keepalive.interval)); 
    opthandler(opts->keepalive.enable,  sock, IPPROTO_TCP,  TCP_KEEPCNT,    &opts->keepalive.probes,    sizeof(opts->keepalive.probes));
    _sl_log_trace("Leaving function with values: opts:(%d, %d, keepalive: %d, %d, %d, %d)", opts->reuseaddr, opts->reuseport, opts->keepalive.enable, opts->keepalive.idle, opts->keepalive.interval, opts->keepalive.probes);
    return sock;
}
int SLCloseSocket(int fd) {
    _sl_log_trace("Entering function with values: fd:%d", fd);
    if (close(fd) < 0) {
        _sl_log_error("Failed closing file descriptor: %d | %s", fd, strerror(errno));
        return -1;
    }
    _sl_log_info("File descriptor: %d closed successfully", fd);
    _sl_log_trace("Leaving function with values: fd:%d", fd);
    return 0;
};
#endif

#endif
