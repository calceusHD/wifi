#ifndef _PACKET_H
#define _PACKET_H

#include <vector>
#include <array>
#include "network.h"
#include "common.h"


class packet {
    private:
        std::vector<uint8_t> m_current_data;

        unsigned int m_frame_size, m_resend_count;

    public:

        void set_data(const std::vector<uint8_t>& src);
        void write_to_network(network &tgt);




};


#endif
