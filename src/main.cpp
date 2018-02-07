#include <stdio.h>
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

int main(int argc, char **argv) {
    
    if (argc <= 1) {
        printf("please specify an input file\n");
    }
    char *tmep = new char[2<<16];
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(0, &rfds);

    /*while (true) {
        select(1, &rfds, nullptr, nullptr, nullptr);
        size_t rd = read(0, tmep, 2<<16);
        printf("read some %zd\n", rd);
    }*/

    network_wifi net(argv[1]);

    bool client = argv[2][0] == 'c';
    net.set_channel(40);
    packet_writer p;
    std::vector<uint8_t> min_header = {
        0x08, 0x0,
        0x0, 0x0,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xcF, 0x25, 0x6, 0x69, 0x3, 0xf,
        0xc0, 0x25, 0x6, 0x69, 0x3, 0xf,
        0x00, 0x00,
    };


    std::vector<uint8_t> test;

    for (uint32_t i = 0; i < 2000; ++i) {
        test.push_back((i % 52) + 'A');
    }
    p.set_data(test);

    rx_info rxi;
    net.set_rate(54'000'000);

    //std::cout << "rate is " << net.get_rate() << std::endl;
    if (!client) {
        //for (unsigned int i = 0; i < 1; ++i) {
        while (1) {
            select(1, &rfds, nullptr, nullptr, nullptr);
            size_t rd = read(0, tmep, 2<<16);
            test.resize(rd);
            memcpy(test.data(), tmep, rd);
            p.set_data(test);
            p.write_to_network(net);
            //net.write(test, nullptr);
        }
    }
    frame fra;
    packet_reader pr;
    while (1) {
        net.read(test, &rxi);
        fra.disassemble(test);
        if (pr.process_frame(fra)) {
            //printf("read packet of: %zd bytes\n", pr.get_data().size());
            std::cout.write((char*)pr.get_data().data(), pr.get_data().size());
        }


        //std::cout << "channel " << rxi.channel << " rate " << rxi.rate << std::endl;
        /*for (unsigned int j = 0; j < test.size(); ++j) {
            std::cout << ", 0x" << std::hex << (int)test.at(j);
        }*/
        //std::cout << std::endl;
    }


}
