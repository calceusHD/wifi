#ifndef _BYTE_DECOMPOSER_H
#define _BYTE_DECOMPOSER_H

#include <vector>
#include <cstdint>
#include <cstring>
#include <array>

class byte_decomposer {
    private:
        std::vector<uint8_t> m_bytes;
        size_t m_position;
        
        template<class T>
        inline T get() {
            T rv;
            memcpy(&rv, &*(m_bytes.begin() + m_position), sizeof(T));
            m_position += sizeof(T);
            return rv;
        }

    public:

        inline void set_data(const std::vector<uint8_t> data) {
            m_bytes.assign(data.begin(), data.end());
            m_position = 0;
        }
        
        inline uint8_t get_uint8_t() {
            return get<uint8_t>();
        }

        inline uint16_t get_uint16_t() {
            return get<uint16_t>();
        }

        inline uint32_t get_uint32_t() {
            return get<uint32_t>();
        }

        inline std::vector<uint8_t> get_vector(size_t num) {
            m_position += num;
            return std::vector<uint8_t>(m_bytes.begin() + m_position - num, m_bytes.begin() + m_position);
        }

        inline std::vector<uint8_t> get_vector() {
            return std::vector<uint8_t>(m_bytes.begin() + m_position, m_bytes.end());
        }
        
        template<size_t N>
        inline std::array<uint8_t, N> get_array() {
            std::array<uint8_t, N> rv;
            memcpy(rv.data(), &*(m_bytes.begin() + m_position), N);
            m_position += N;
            return rv;
        }



};




#endif
