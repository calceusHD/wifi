#include <stdio.h>
#include <stdint.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <vector>
#include "network.h"
#include "crc32.h"
#include "packet.h"

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

    network net(argv[1]);

    bool client = argv[2][0] == 'c';
    net.set_channel(38);
    packet p;
    std::vector<uint8_t> min_header = {
        0x08, 0x0,
        0x0, 0x0,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xcF, 0x25, 0x6, 0x69, 0x3, 0xf,
        0xc0, 0x25, 0x6, 0x69, 0x3, 0xf,
        0x00, 0x00,
        

    std::vector<uint8_t> test(min_header);

    for (uint32_t i = 0; i < 2170; ++i) {
        test.push_back(i);
    }


    
    //crc32::append_crc32(test);
    
    //std::vector<uint8_t> test = {0xFF, 0xFF, 0x00,};
    /*for (unsigned int i = 0; i < 256; ++i) {
        test.push_back(i);
    }*/
    rx_info rxi;
    tx_info txi;
    txi.ti_rate = 54'000'000;
    net.set_rate(54'000'000);

    std::cout << "rate is " << net.get_rate() << std::endl;
    if (!client) {
        for (unsigned int i = 0; i < 4; ++i) {
            net.write(test, &txi);
        }
    }
    for (unsigned int i = 0; i < 8; ++i) {
        net.read(test, &rxi);
        std::cout << "channel " << rxi.channel << " rate " << rxi.rate << std::endl;
        /*for (unsigned int j = 0; j < test.size(); ++j) {
            std::cout << ", 0x" << std::hex << (int)test.at(j);
        }*/
        std::cout << std::endl;
    }


}
