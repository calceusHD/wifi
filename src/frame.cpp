#include "frame.h"
#include "byte_composer.h"
#include "byte_decomposer.h"


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


frame& frame::set_payload(std::vector<uint8_t>::const_iterator begin, std::vector<uint8_t>::const_iterator end) {
    m_payload.assign(begin, end);
    return *this;
}

uint16_t frame::get_duration() {
    return m_duration;
}

mac_address frame::get_mac_1() {
    return m_mac_1;
}

mac_address frame::get_mac_2() {
    return m_mac_2;
}

mac_address frame::get_mac_3() {
    return m_mac_3;
}

uint16_t frame::get_sequence() {
    return m_sequence;
}

std::vector<uint8_t> frame::get_payload() {
    return m_payload;
}

const std::vector<uint8_t> frame::assemble() {
    size_t header_bytes = 0;

    header_bytes += sizeof(mac_address) * 4;
    header_bytes += sizeof(uint16_t) * 3;
    byte_composer bc(header_bytes + m_payload.size());
    bc.add_uint16_t(0x0008).add_uint16_t(m_duration);
    bc.add_array(m_mac_1).add_array(m_mac_2).add_array(m_mac_3);
    bc.add_uint16_t(m_sequence);
    bc.add_vector(m_payload);
    //printf("assembling %zd bytes\n", bc.get_contents().size());
    return bc.get_contents();
}

void frame::disassemble(const std::vector<uint8_t> data) {
    byte_decomposer bd;
    bd.set_data(data);
    bd.get_uint16_t();
    m_duration = bd.get_uint16_t();
    m_mac_1 = bd.get_array<6>();
    m_mac_2 = bd.get_array<6>();
    m_mac_3 = bd.get_array<6>();
    m_sequence = bd.get_uint16_t();
    m_payload = bd.get_vector();
}
