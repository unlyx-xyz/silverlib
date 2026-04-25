/* 
Silver Library project - Made by: unlyx.xyz
04-25-2026
Version: 1.0.0
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

/*
======================
==###DECLARATIONS###==
======================
*/

typedef enum {
    LOGL_TRACE = 0,
    LOGL_DEBUG = 1, 
    LOGL_INFO = 2,
    LOGL_WARN = 3,
    LOGL_ERROR = 4,
    LOGL_FATAL = 5,
    LOGL_DISABLED = 6,
} LOG_LEVEL;

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

int SLCreateUDPIPv4Socket(sock_opt_t *opts);
int SLCreateTCPIPv4Socket(sock_opt_t *opts);
int SLCloseSocket(int fd);

/*
========================
==###IMPLEMENTATION###==
========================
*/

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
    fprintf(stderr, "[%s] %s:%d:%s - ", logl_presentations[logl], file, line, fnc);
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");
}

#define SL_LogTrace(...) _sl_log(LOGL_TRACE, __FILE__, __LINE__, __func__, ##__VA_ARGS__);
#define SL_LogDebug(...) _sl_log(LOGL_DEBUG, __FILE__, __LINE__, __func__, ##__VA_ARGS__);
#define SL_LogInfo(...)  _sl_log(LOGL_INFO,  __FILE__, __LINE__, __func__, ##__VA_ARGS__);
#define SL_LogWarn(...)  _sl_log(LOGL_WARN,  __FILE__, __LINE__, __func__, ##__VA_ARGS__);
#define SL_LogError(...) _sl_log(LOGL_ERROR, __FILE__, __LINE__, __func__, ##__VA_ARGS__);
#define SL_LogFatal(...) _sl_log(LOGL_FATAL, __FILE__, __LINE__, __func__, ##__VA_ARGS__);

#ifdef SILVER_LIB_IMPLEMENTATION

#define opthandler(enabled, fd, lvl, opt, val, size)\
        do {\
            if (enabled) {\
                if (setsockopt(fd, lvl, opt, val, size) < 0) {\
                    SL_LogError("%s", strerror(errno));\
                    return -1;\
                }\
                SL_LogInfo(#opt " applied successfully");\
            }\
        } while (0)\

int SLCreateUDPIPv4Socket(sock_opt_t *opts) {
    int sock;
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        SL_LogError("Failed creating socket | %s", strerror(errno));
        return -1;
    }
    SL_LogInfo("Socket created successfully");
    opthandler(opts->reuseaddr, sock, SOL_SOCKET, SO_REUSEADDR, &opts->reuseaddr, sizeof(opts->reuseaddr));
    opthandler(opts->reuseport, sock, SOL_SOCKET, SO_REUSEPORT, &opts->reuseport, sizeof(opts->reuseport));
    return sock;
};
int SLCreateTCPIPv4Socket(sock_opt_t *opts) {
    int sock;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        SL_LogError("Failed creating socket | %s", strerror(errno));
        return -1;
    }
    SL_LogInfo("Socket created successfully");
    opthandler(opts->reuseaddr,         sock, SOL_SOCKET,   SO_REUSEADDR,   &opts->reuseaddr,           sizeof(opts->reuseaddr));
    opthandler(opts->reuseport,         sock, SOL_SOCKET,   SO_REUSEPORT,   &opts->reuseport,           sizeof(opts->reuseport));
    opthandler(opts->keepalive.enable,  sock, SOL_SOCKET,   SO_KEEPALIVE,   &opts->keepalive.enable,    sizeof(opts->keepalive.enable));
    opthandler(opts->keepalive.enable,  sock, IPPROTO_TCP,  TCP_KEEPIDLE,   &opts->keepalive.idle,      sizeof(opts->keepalive.interval));
    opthandler(opts->keepalive.enable,  sock, IPPROTO_TCP,  TCP_KEEPINTVL,  &opts->keepalive.interval,  sizeof(opts->keepalive.interval)); 
    opthandler(opts->keepalive.enable,  sock, IPPROTO_TCP,  TCP_KEEPCNT,    &opts->keepalive.probes,    sizeof(opts->keepalive.probes)); 
    return sock;
}
int SLCloseSocket(int fd) {
    if (close(fd) < 0) {
        SL_LogError("Failed closing file descriptor: %d | %s", fd, strerror(errno));
        return -1;
    }
    SL_LogInfo("File descriptor: %d closed successfully", fd);
    return 0;
};
#endif

#endif
