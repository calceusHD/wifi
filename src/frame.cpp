#include "frame.h"
#include "byte_composer.h"



frame::frame() {
}

frame& frame::set_duration(uint16_t duration) {
    m_duration = duration;
    return *this;
}

frame& frame::set_mac_1(mac_address mac) {
    m_mac_1 = mac;
    return *this;
}

frame& frame::set_mac_2(mac_address mac) {
    m_mac_2 = mac;
    return *this;
}

frame& frame::set_mac_3(mac_address mac) {
    m_mac_3 = mac;
    return *this;
}

frame& frame::set_sequnce(uint16_t sequence) {
    m_sequence = sequence;
    return *this;
}

frame& frame::set_mac_4(mac_address mac) {
    m_mac_4 = mac;
    return *this;
}

frame& frame::set_payload(std::vector<uint8_t>::const_iterator begin, std::vector<uint8_t>::const_iterator end) {
    m_begin = begin;
    m_end = end;
    return *this;
}

const std::vector<uint8_t>& frame::assemble() {
    size_t header_bytes = 0;
    std::vector<uint8_t> out;

    header_bytes += sizeof(mac_address) * 4;
    header_bytes += sizeof(uint16_t) * 3;
    byte_composer bc(header_bytes + std::distance(m_begin, m_end));
    bc.add_uint16_t(0x0800).add_uint16_t(m_duration);
    bc.add_array(m_mac_1).add_array(m_mac_2).add_array(m_mac_3);
    bc.add_uint16_t(m_sequence);
    bc.add_vector(m_begin, m_end);
    return bc.get_contents();
}
