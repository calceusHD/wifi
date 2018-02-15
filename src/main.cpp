#include <stdio.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <vector>
#include "network_wifi.h"
#include "crc32.h"
#include "packet_writer.h"
#include <sys/select.h>
#include "frame.h"
#include "packet_reader.h"
#include <cstring>

int main(int argc, char **argv) {
    
    if (argc <= 1) {
        printf("please specify an input file\n");
    }

    char *tmep = new char[2<<16];
    fd_set rfds;
    FD_ZERO(&rfds);
    //FD_SET(0, &rfds);

    network_wifi net(argv[1]);

    bool client = argv[2][0] == 'c';
    std::string gst_addr(argv[3]);
    net.set_channel(40);
    packet_writer p;

    std::vector<uint8_t> test;

    rx_info rxi;
    net.set_rate(54'000'000);

    addrinfo *res = nullptr, hints;

    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
    int rv;

    rv = getaddrinfo(gst_addr.substr(0, gst_addr.find(":", 0)).c_str(), gst_addr.substr(gst_addr.find(":", 0) + 1, gst_addr.size()).c_str(), &hints, &res);

    SOCKET gst_sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    FD_SET(gst_sock, &rfds);

    sockaddr* sock_addr = res->ai_addr;
    
    if (!client) {
        rv = bind(gst_sock, sock_addr, res->ai_addrlen);
    }

    if (rv)
        printf("bind failed %d\n", rv);

    if (!client) {
        while (1) {
            select(gst_sock+1, &rfds, nullptr, nullptr, nullptr);
            size_t rd = recv(gst_sock, tmep, 2<<16, MSG_DONTWAIT);
            test.resize(rd);
            memcpy(test.data(), tmep, rd);
            p.set_data(test);
            p.write_to_network(net);
        }
    }
    
    frame fra;
    packet_reader pr;
    if (client) {
        while (1) {
            net.read(test, &rxi);
            fra.disassemble(test);
            if (pr.process_frame(fra)) {
                rv = sendto(gst_sock, (char*)pr.get_data().data(), pr.get_data().size(), 0, res->ai_addr, res->ai_addrlen);
                if (rv < 1) {
                    printf("snedto error %d\n", rv);
                }
                //std::cout.write((char*)pr.get_data().data(), pr.get_data().size());
            }
        }
    }
}
