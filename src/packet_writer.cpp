#include "packet_writer.h"
#include "frame.h"
#include <cstring>

packet_writer::packet_writer() {
    m_frame_size = 1500;
    m_resend_count = 2;
    m_packet_number = 1;
}

void packet_writer::set_data(const std::vector<uint8_t>& in) {
    m_current_data = in;
}


void packet_writer::write_to_network(network_wifi &tgt) {
    unsigned int byte_count = m_current_data.size();
    unsigned int frame_count = (byte_count - 1) / m_frame_size + 1; // elegantly round up
    
    //printf("frame_count: %d\n", frame_count);
    //printf("data_bytes: %d\n", byte_count);
    //printf("packet_number start: %ld\n", m_packet_number);

    for (unsigned int resend = 0; resend < m_resend_count; ++resend) {
        for (unsigned int current_frame = 0; current_frame < frame_count; ++current_frame) {
            unsigned int frame_start = current_frame * m_frame_size;
            unsigned int frame_end = (current_frame + 1) * m_frame_size - 1;

            if (frame_end >= byte_count) {
                frame_end = byte_count;
            }

            //printf("creating frame: %d to %d\n", frame_start, frame_end);
            frame f;
            mac_address *adr1 = (mac_address*)&m_packet_number;
            mac_address adr2 = {0, 0, 0, 0, 0, 0};
            memcpy(&adr2, &byte_count, sizeof(byte_count));
            f.set_mac_1(*adr1)
                .set_mac_2(adr2)
                .set_mac_3({0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF})
                .set_sequnce(current_frame)
                .set_duration(0).
                set_payload(m_current_data.begin() + frame_start, m_current_data.begin() + frame_end);
            //printf("wtf?? %zd", f.assemble().size());
            tgt.write(f.assemble(), nullptr);
        }
    }
    ++m_packet_number;

}

