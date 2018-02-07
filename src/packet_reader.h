#ifndef _PACKET_READER_H
#define _PACKET_READER_H

#include <vector>
#include <cstdint>
#include <frame.h>

class packet_reader {
    private:
        std::vector<uint8_t> m_current_packet;
        std::vector<bool> m_valid_frames;
        bool m_packet_done;
        uint64_t m_packet_number;
        unsigned int m_frame_count, m_frame_size;
    public:
        packet_reader();


        int process_frame(frame fr);
        std::vector<uint8_t> get_data();


};

#endif
