#ifndef _PACKET_WRITER_H
#define _PACKET_WRITER_H

#include <vector>
#include <array>
#include "network_wifi.h"
#include "common.h"


class packet_writer {
    private:
        std::vector<uint8_t> m_current_data;

        unsigned int m_frame_size, m_resend_count;
        uint64_t m_packet_number;
    public:
        packet_writer();

        void set_data(const std::vector<uint8_t>& src);
        void write_to_network(network_wifi &tgt);




};


#endif
