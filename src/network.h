#ifndef _NETWORK_H
#define _NETWORK_H

#include <cstdint>
#ifdef __WIN32
#include <Winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#endif
#include <string>
#include <queue>
#include <vector>
#include <array>

#ifndef __WIN32
typedef int SOCKET;
#endif

struct rx_info;
class tx_info;

class network {
    public:
    enum {
        NET_RC = 1,
        NET_GET_CHAN,
        NET_SET_CHAN,
        NET_WRITE,
        NET_PACKET,		/* 5 */
        NET_GET_MAC,
        NET_MAC,
        NET_GET_MONITOR,
        NET_GET_RATE,
        NET_SET_RATE,
    };
    
    private:
    std::queue<std::vector<uint8_t>> m_queue;
    SOCKET m_s;





    public:
    network(std::string iface);
    ~network();

    int read(std::vector<uint8_t>& h80211, rx_info *ri);
    int write(const std::vector<uint8_t>& h80211, tx_info *ti);
    int set_channel(int channel);
    int get_channel();
    int set_rate(int rate);
    int get_rate();
    SOCKET fd();
    int get_mac(std::array<uint8_t, 6> mac);
    int get_monitor();

    private:
    int send(int command, const std::vector<uint8_t>& arg);
    int read_exact(std::vector<uint8_t> &arg, int len);
    int get(std::vector<uint8_t> &arg);
    int get_nopacket(std::vector<uint8_t> &arg);
    int cmd(int cmd, std::vector<uint8_t> &arg);






};
#pragma pack(1)
struct net_hdr {
    uint8_t type;
    uint32_t len;
};

struct rx_info {
    uint64_t mactime;
    int32_t power;
    int32_t noise;
    uint32_t channel;
    uint32_t freq;
    uint32_t rate;
    uint32_t antenna;
};

class tx_info {
    public:
    unsigned int ti_rate;
};
#pragma pack()

#endif
