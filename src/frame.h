#ifndef _FRAME_H
#define _FRAME_H

#include <cstdint>
#include <vector>
#include "common.h"



class frame {
    private:
        uint16_t m_duration, m_sequence;
        std::vector<uint8_t>::const_iterator m_begin, m_end;
        mac_address m_mac_1, m_mac_2, m_mac_3, m_mac_4;

    public:

        frame();
        
        frame& set_duration(uint16_t duration);
        frame& set_mac_1(mac_address mac);
        frame& set_mac_2(mac_address mac);
        frame& set_mac_3(mac_address mac);
        frame& set_sequnce(uint16_t sequence);
        frame& set_mac_4(mac_address mac);
        frame& set_payload(std::vector<uint8_t>::const_iterator begin, std::vector<uint8_t>::const_iterator end);


        const std::vector<uint8_t>& assemble();



};


#endif
