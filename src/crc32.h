#include <vector>
#include <cstdint>

class crc32 {
    private:
        crc32();
    public:
        static uint32_t calc_crc32(std::vector<uint8_t> in);
        static void append_crc32(std::vector<uint8_t> &data);

};
