/* 
Silver Library project - Made by: unlyx.xyz
04-25-2026
Version: 1.2.3
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

void SLInitLib(void);

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
} KeepaliveContext;

typedef struct {
    int reuseaddr;
    int reuseport;
    KeepaliveContext keepalive; 
} SocketContext;

int SLCreateUDPIPv4Socket(SocketContext *ctx);
int SLCreateTCPIPv4Socket(SocketContext *ctx);
int SLCloseSocket(int fd);

// NETWORKING -> CONNECTION --------------

typedef struct {
    int origsock;
    struct sockaddr_in destaddr;
} ConnectionContext;

int SLConnectIPv4Socket(ConnectionContext *ctx);

typedef struct {
    int sock;
    uint16_t port;
    uint32_t incoming;
} ListeningContext;

int SLListenIPv4Socket(ListeningContext *ctx);

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
        TL_CopySequenceDa(graphics_settings, &_log_color[i]);
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
            _sl_log_trace("Entering opthandler macro with values: %d, %d, %d, %d, %d, %d", enabled, fd, lvl, opt, val, size);\
            if (enabled) {\
                if (setsockopt(fd, lvl, opt, val, size) < 0) {\
                    _sl_log_error("%s", strerror(errno));\
                    return -1;\
                }\
                _sl_log_info(#opt " applied successfully");\
            }\
        } while (0)\

int SLCreateUDPIPv4Socket(SocketContext *ctx) {
    _sl_log_trace("Entering function with values: ctx:(%d, %d, keepalive: %d, %d, %d, %d)", ctx->reuseaddr, ctx->reuseport, ctx->keepalive.enable, ctx->keepalive.idle, ctx->keepalive.interval, ctx->keepalive.probes);
    int sock;
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        _sl_log_error("Failed creating socket | %s", strerror(errno));
        return -1;
    }
    _sl_log_info("Socket created successfully");
    opthandler(ctx->reuseaddr, sock, SOL_SOCKET, SO_REUSEADDR, &ctx->reuseaddr, sizeof(ctx->reuseaddr));
    opthandler(ctx->reuseport, sock, SOL_SOCKET, SO_REUSEPORT, &ctx->reuseport, sizeof(ctx->reuseport));
    _sl_log_trace("Leaving function with values: ctx:(%d, %d, keepalive: %d, %d, %d, %d)", ctx->reuseaddr, ctx->reuseport, ctx->keepalive.enable, ctx->keepalive.idle, ctx->keepalive.interval, ctx->keepalive.probes);
    return sock;
}
int SLCreateTCPIPv4Socket(SocketContext *ctx) {
    _sl_log_trace("Entering function with values: ctx:(%d, %d, keepalive: %d, %d, %d, %d)", ctx->reuseaddr, ctx->reuseport, ctx->keepalive.enable, ctx->keepalive.idle, ctx->keepalive.interval, ctx->keepalive.probes);
    int sock;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        _sl_log_error("Failed creating socket | %s", strerror(errno));
        return -1;
    }
    _sl_log_info("Socket created successfully");
    opthandler(ctx->reuseaddr,         sock, SOL_SOCKET,   SO_REUSEADDR,   &ctx->reuseaddr,           sizeof(ctx->reuseaddr));
    opthandler(ctx->reuseport,         sock, SOL_SOCKET,   SO_REUSEPORT,   &ctx->reuseport,           sizeof(ctx->reuseport));
    opthandler(ctx->keepalive.enable,  sock, SOL_SOCKET,   SO_KEEPALIVE,   &ctx->keepalive.enable,    sizeof(ctx->keepalive.enable));
    opthandler(ctx->keepalive.enable,  sock, IPPROTO_TCP,  TCP_KEEPIDLE,   &ctx->keepalive.idle,      sizeof(ctx->keepalive.interval));
    opthandler(ctx->keepalive.enable,  sock, IPPROTO_TCP,  TCP_KEEPINTVL,  &ctx->keepalive.interval,  sizeof(ctx->keepalive.interval)); 
    opthandler(ctx->keepalive.enable,  sock, IPPROTO_TCP,  TCP_KEEPCNT,    &ctx->keepalive.probes,    sizeof(ctx->keepalive.probes));
    _sl_log_trace("Leaving function with values: ctx:(%d, %d, keepalive: %d, %d, %d, %d)", ctx->reuseaddr, ctx->reuseport, ctx->keepalive.enable, ctx->keepalive.idle, ctx->keepalive.interval, ctx->keepalive.probes);
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
}
int SLConnectIPv4Socket(ConnectionContext *ctx) {
    _sl_log_trace("Entering function with values: ctx:(%d, destaddr: %d, %d)", ctx->origsock, ctx->destaddr.sin_port, ctx->destaddr.sin_addr.s_addr);
    char presentation_dest_ip[INET_ADDRSTRLEN] = {0};
    uint16_t presentation_dest_port = ntohs(ctx->destaddr.sin_port);
    inet_ntop(AF_INET, &ctx->destaddr.sin_addr.s_addr, presentation_dest_ip, sizeof(presentation_dest_ip));
    socklen_t destaddrlen = sizeof(ctx->destaddr);
    _sl_log_info("Connecting socket with file descriptor: %d to: %s:%d", ctx->origsock, presentation_dest_ip, presentation_dest_port);
    if(connect(ctx->origsock, (struct sockaddr *)&ctx->destaddr, destaddrlen) < 0) {
        _sl_log_error("Failed connecting socket with file descriptor: %d to: %s:%d | %s", ctx->origsock, presentation_dest_ip, presentation_dest_port, strerror(errno));
        return -1;
    }
    _sl_log_info("Socket with file descriptor: %d connected to: %s:%d", ctx->origsock, presentation_dest_ip, presentation_dest_port);
    _sl_log_trace("Leaving function with values: ctx:(%d, destaddr: %d, )", ctx->origsock, ctx->destaddr.sin_port, ctx->destaddr.sin_addr.s_addr);
    return 0;
}
int SLListenIPv4Socket(ListeningContext *ctx) {
    _sl_log_trace("Entering function with values: ctx:(%d, %d, %d)", ctx->sock, ctx->port, ctx->incoming);
    _sl_log_info("Binding socket with file descriptor: %d to poert: %d", ctx->sock, ctx->port);
    struct sockaddr_in srvaddr;
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_addr.s_addr = INADDR_ANY;
    srvaddr.sin_port = htons(ctx->port);
    socklen_t srvaddrlen = sizeof(srvaddr);
    if(bind(ctx->sock, (struct sockaddr *)&srvaddr, srvaddrlen) < 0) {
        _sl_log_error("Failed binding socket with file descriptor: %d | %s", ctx->sock, strerror(errno));
        return -1;
    }
    _sl_log_info("Socket with file descriptor: %d successfully binded to port: %d", ctx->sock, ctx->port);
    _sl_log_info("Setting socket with file descriptor: %d to listening mode", ctx->sock);
    if (listen(ctx->sock, ctx->incoming) < 0) {
        _sl_log_error("Failed setting listening mode to socket: %d", ctx->sock);
        return -1;
    }
    _sl_log_info("Socket with file descriptor: %d successfully set to listening mode", ctx->sock);
    _sl_log_trace("Leaving function with values: ctx:(%d, %d, %d)", ctx->sock, ctx->port, ctx->incoming);
    return 0;
} 
#endif

#endif
