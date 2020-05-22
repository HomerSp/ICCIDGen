#include <cstdint>
#include <vector>

class BitStream {
public:
    BitStream(std::vector<uint8_t>& data);

    uint64_t offset() const { return mOffset; }

    uint32_t read(unsigned count);
    void write(uint32_t value, unsigned count);

protected:
    uint32_t ReadData(const uint8_t *src, unsigned offset, unsigned count, uint32_t accum = 0);
    void WriteData(uint8_t *dst, unsigned offset, unsigned count, uint32_t value);

private:
    std::vector<uint8_t>& mData;
    uint64_t mOffset;
};