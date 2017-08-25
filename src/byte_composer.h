#ifndef _BYTE_COMPOSER_H
#define _BYTE_COMPOSER_H

#include <vector>
#include <cstdint>
#include <array>



class byte_composer {
    private:
        std::vector<uint8_t> m_bytes;
        
        template<class T>
        inline byte_composer& add(T in) {
            m_bytes.resize(m_bytes.size() + sizeof(T));
            memcpy(&*(m_bytes.end() - sizeof(T)), &in, sizeof(T));
        }

    public:
        byte_composer(size_t reserve) {
            m_bytes.reserve(reserve);
        }

        inline const std::vector<uint8_t>& get_contents() {
            return m_bytes;
        }

        
        inline byte_composer& add_uint8_t(uint8_t in) {
            return add(in);
        }
        
        inline byte_composer& add_uint16_t(uint16_t in) {
            return add(in);
        }

        inline byte_composer& add_uint32_t(uint32_t in) {
            return add(in);
        }
        
        inline byte_composer& add_vector(const std::vector<uint8_t>& in) {
            m_bytes.insert(m_bytes.end(), in.begin(), in.end());
            return *this;
        }

        inline byte_composer& add_vector(std::vector<uint8_t>::const_iterator begin, std::vector<uint8_t>::const_iterator end) {
            m_bytes.insert(m_bytes.end(), begin, end);
            return *this;
        }

        template<size_t N>
        inline byte_composer& add_array(const std::array<uint8_t, N>& in) {
            m_bytes.insert(m_bytes.end(), in.begin(), in.end());
            return *this;
        }





};


#endif
