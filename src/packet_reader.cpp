#include "packet_reader.h"

#include <cstring>

packet_reader::packet_reader() {
    m_frame_size = 1500;
    m_packet_number = 0;
}


int packet_reader::process_frame(frame fr) {

    uint64_t packet_number;
    mac_address adr_pack;
    adr_pack = fr.get_mac_1();
    memcpy(&packet_number, &adr_pack, sizeof(mac_address));
    if (packet_number > m_packet_number) {
        m_packet_number = packet_number;
        mac_address adr_bytes = fr.get_mac_2();
        unsigned int byte_count = *((uint32_t*)&adr_bytes) ;
        m_frame_count = (byte_count - 1) / m_frame_size + 1; 
        m_valid_frames.clear();
        m_valid_frames.resize(m_frame_count);
        m_current_packet.resize(byte_count);
        m_packet_done = false;
    } else if (packet_number < m_packet_number) {
        return 0;
    }
    unsigned int sequence = fr.get_sequence();
    unsigned int frame_start = sequence * m_frame_size;
    unsigned int frame_end = (sequence + 1) * m_frame_size - 1;
    if (frame_end > m_current_packet.size()) {
        frame_end = m_current_packet.size();
    }
    m_valid_frames.at(sequence) = true;
    memcpy(m_current_packet.data() + frame_start, fr.get_payload().data(), frame_end - frame_start);
    bool done = true;
    for (bool valid : m_valid_frames) {
        if (!valid) {
            done = false;
        }
    }
    bool rv = !m_packet_done && done;
    m_packet_done = done;

    return rv ? 1 : 0;
}

std::vector<uint8_t> packet_reader::get_data() {
    return m_current_packet;
}
