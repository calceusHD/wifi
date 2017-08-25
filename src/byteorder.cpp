#ifdef __WIN32
#include <Winsock2.h>
#else
#include <arpa/inet.h>
#endif
#include <byteorder.h>

union net_untion {
    int64_t i64;
    int32_t i32[2];
};

int64_t htonll(int64_t host) {
    net_untion u;
    u.i64 = host;
    int32_t temp = u.i32[0];
    u.i32[0] = htonl(u.i32[1]);
    u.i32[1] = htonl(temp);
    return u.i64;
}

int64_t ntohll(int64_t network) {
    net_untion u;
    u.i64 = network;
    int32_t temp = u.i32[0];
    u.i32[0] = ntohl(u.i32[1]);
    u.i32[1] = ntohl(temp);
    return u.i64;
}
    

