#ifndef _PACKET_H
#define _PACKET_H

#include <vector>
#include <array>
#include "network_wifi.h"
#include "common.h"


class packet_writer {
    private:
        std::vector<uint8_t> m_current_data;

        unsigned int m_frame_size, m_resend_count;

    public:
        packet_writer();

        void set_data(const std::vector<uint8_t>& src);
        void write_to_network(network_wifi &tgt);




};


#endif
