#define SILVER_LIB_IMPLEMENTATION
#define SILVER_LIB_LOGL LOGL_TRACE
#include "../silverlib.h"

#include <sys/socket.h>
#include <assert.h>

int main() {

    SocketContext srvsockctx = {
        .reuseport = 1,
        .reuseaddr = 1,
        .keepalive = {
            .enable = 1,
            .idle = 60,
            .interval = 10,
            .probes = 6,
        },
    };
    int srvsock = SLCreateTCPIPv4Socket(&srvsockctx);
    assert(srvsock > 0);

    ListeningContext srvlisctx = {
        .sock = srvsock,
        .port = 8080,
        .incoming = 1,
    };
    
    int cnnsock = SLListenIPv4Socket(&srvlisctx);
    assert(cnnsock > 0);

    char msg[100] = {0};
    assert(SLRecvIPv4Socket(cnnsock, msg, sizeof(msg), 0) > 0);

    printf("%s\n", msg);

    assert(SLCloseSocket(srvsock) != -1);
    assert(SLCloseSocket(cnnsock) != -1);

}

