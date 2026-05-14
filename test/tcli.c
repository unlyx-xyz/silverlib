#define SILVER_LIB_IMPLEMENTATION
#define SILVER_LIB_LOGL LOGL_TRACE
#include "../silverlib.h"

#include <sys/socket.h>
#include <assert.h>

int main() {

    SLInitLib();

    SocketContext clisockctx = {
        .reuseport = 1,
        .reuseaddr = 1,
        .keepalive = {
            .enable = 1,
            .idle = 60,
            .interval = 10,
            .probes = 6,
        },
    };
    int clisock = SLCreateTCPIPv4Socket(&clisockctx);
    assert(clisock > 0);

    ConnectionContext clicnnctx = {
        .origsock = clisock,
        .destaddr = {
            .sin_family = AF_INET,
            .sin_port = htons(8080),
        },
    };
    inet_pton(AF_INET, "127.0.0.1", &clicnnctx.destaddr.sin_addr);
    assert(SLConnectIPv4Socket(&clicnnctx) >= 0);

    char msg_sent[100] = "Hello from silverlib!";
    assert(SLSendIPv4Socket(clisock, &msg_sent, sizeof(msg_sent), 0) == strlen(msg_sent) + 1);

    SLCloseSocket(clisock);

}

