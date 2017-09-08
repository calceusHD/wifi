#include <network_wifi.h>
#include <cassert>
#include <iostream>
#include <byteorder.h>
#include <cstdio>
#include <cstring>


network_wifi::network_wifi(std::string iface) {
    addrinfo *res = nullptr, hints;

    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    int rv;
#ifdef __WIN32
    WSADATA wsaData;
    rv = WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

    rv = getaddrinfo(iface.substr(0, iface.find(":", 0)).c_str(), iface.substr(iface.find(":", 0) + 1, iface.size()).c_str(), &hints, &res);

    m_s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    rv = connect(m_s, res->ai_addr, (int)res->ai_addrlen);
}

network_wifi::~network_wifi() {
#ifdef __WIN32
    closesocket(m_s);
    WSACleanup();
#else
    close(m_s);
#endif
}

int network_wifi::read(std::vector<uint8_t> &h80211, rx_info *ri) {
    std::vector<uint8_t> buf;

    int cmd;

    if (!m_queue.empty()) {
        buf = m_queue.front();
        m_queue.pop();
    } else {
        cmd = get(buf);

        if (cmd == -1) {
            return -1;
        }
        if (cmd == NET_RC) {
            uint32_t *tmp = (uint32_t*)buf.data();
            return ntohl(*tmp);
        }
        assert(cmd == NET_PACKET);
    }

    if (ri) {
        assert(buf.size() >= sizeof(*ri));
        uint32_t *buf32 = (uint32_t*)buf.data();
        ri->mactime = ntohll(((uint64_t)buf32[0] << 32 || buf32[1]));
        ri->power = ntohl(buf32[2]);
        ri->noise = ntohl(buf32[3]);
        ri->channel = ntohl(buf32[4]);
        ri->freq = ntohl(buf32[5]);
        ri->rate = ntohl(buf32[6]);
        ri->antenna = ntohl(buf32[7]);
    }
    h80211.assign(buf.begin()+sizeof(*ri), buf.end());
    return h80211.size();
}

int network_wifi::write(const std::vector<uint8_t>& h80211, tx_info *ti) {
    std::vector<uint8_t> tmp;

    tmp.resize(sizeof(*ti));
    if (ti) {
        memcpy(tmp.data(), ti, sizeof(*ti));
    } else {
        memset(tmp.data(), 0, sizeof(*ti));
    }
    tmp.insert(tmp.end(), h80211.begin(), h80211.end());
    std::cout << "sending" << tmp.size() << "bytes" << std::endl;
    return cmd(NET_WRITE, tmp);
}

int network_wifi::set_channel(int channel) {
    std::vector<uint8_t> tmp;
    tmp.resize(sizeof(uint32_t));
    *(uint32_t*)tmp.data() = htonl(channel);
    return cmd(NET_SET_CHAN, tmp);
}

int network_wifi::get_channel() {
    std::vector<uint8_t> tmp;
    return cmd(NET_GET_CHAN, tmp);
}

int network_wifi::set_rate(int rate) {
    std::vector<uint8_t> tmp;
    tmp.resize(sizeof(uint32_t));
    *(uint32_t*)tmp.data() = htonl(rate);
    return cmd(NET_SET_RATE, tmp);
}

int network_wifi::get_rate() {
    std::vector<uint8_t> tmp;
    return cmd(NET_GET_RATE, tmp);
}


int network_wifi::send(int command, const std::vector<uint8_t>& arg) {
    std::vector<uint8_t> tmp(sizeof(net_hdr));
    net_hdr *hdr = (net_hdr*)tmp.data();

    hdr->type = command;
    hdr->len = htonl(arg.size());

    std::cout << "setting a size of " << arg.size() << std::endl;

    std::cout << *(uint32_t*)(((uint8_t*)hdr)+1) << std::endl;
    std::cout << sizeof(net_hdr) << std::endl;
    tmp.insert(tmp.end(), arg.begin(), arg.end());

    ::send(m_s, (const char*)tmp.data(), tmp.size(), 0);

    return 0;
}

int network_wifi::read_exact(std::vector<uint8_t> &arg, int len) {
    size_t rc;
    int rlen = 0;
    arg.resize(len);
    char* buf = (char*)arg.data();
    while (rlen < len) {
        rc = recv(this->m_s, buf, (len - rlen), 0);

        if (rc < 1) {
            return -1;
        }
        buf += rc;
        rlen += rc;
    }
    return 0;
}


int network_wifi::get(std::vector<uint8_t> &arg) {
    std::vector<uint8_t> nh;
    if (read_exact(nh, sizeof(net_hdr)) == -1) {
        return -1;
    }
    net_hdr *nh2 = (net_hdr*)nh.data();
    uint32_t len = ntohl(nh2->len);
    std::cout << "packetlen is " << len << std::endl;
    if (len && read_exact(arg, len) == -1) {
        return -1;
    }
    std::cout << "arg is " << arg.size() << std::endl;
    return nh2->type;
}

int network_wifi::get_nopacket(std::vector<uint8_t> &arg) {
    int c;
    while (true) {
        c = get(arg);

        if (c < 0) {
            arg.clear();
            return c;
        } else if (c == NET_PACKET) {
            m_queue.push(arg);
        } else {
            return c;
        }
    }
}

int network_wifi::cmd(int cmd, std::vector<uint8_t> &arg) {
    if (send(cmd, arg) == -1) {
        return -1;
    }
    uint32_t rc;
    std::vector<uint8_t> tmp;
    cmd = get_nopacket(tmp);
    if (cmd == -1) {
        return -1;
    }
    assert(cmd == NET_RC);
    std::cout << "got " << tmp.size() << " expected " << sizeof(rc) << std::endl;
    assert(tmp.size() == sizeof(rc));
    return ntohl(*(uint32_t*)tmp.data());
}
