#include <cmath>
#include "include/bitstream.h"

BitStream::BitStream(std::vector<uint8_t>& data)
    : mData(data)
    , mOffset(0)
{

}

uint32_t BitStream::read(unsigned count)
{
    uint32_t ret = ReadData(mData.data(), mOffset, count);
    mOffset += count;
    return ret;
}

void BitStream::write(uint32_t value, unsigned count)
{
    int needSize = ((mOffset + count) / 8) + 1;
    if (mData.size() < needSize) {
        mData.resize(needSize);
    }

    WriteData(mData.data(), mOffset, count, value);
    mOffset += count;
}

uint32_t BitStream::ReadData(const uint8_t *src, unsigned offset, unsigned count, uint32_t accum)
{
    if (count == 0) {
        return accum;
    }

    const uint8_t *curSrc = src + offset / 8;
    unsigned bitsLeft = 8 - offset % 8;
    const uint8_t cur_data = *curSrc << (8 - bitsLeft);
    unsigned bits_to_use = bitsLeft < count ? bitsLeft : count;

    accum <<= bits_to_use;
    uint8_t mask = (1 << bits_to_use) - 1;
    unsigned off = 8 - bits_to_use;
    accum |= (cur_data & (mask << off)) >> off;

    return ReadData(src, offset + bits_to_use, count - bits_to_use, accum);
}

void BitStream::WriteData(uint8_t *dst, unsigned offset, unsigned count, uint32_t value)
{
    if (count == 0) {
        return;
    }

    uint8_t *cur_dst = dst + offset / 8;
    unsigned bitsLeft = 8 - offset % 8;

    unsigned bits_to_use = bitsLeft < count ? bitsLeft : count;
    const uint8_t mask = (1 << bits_to_use) - 1;

    unsigned value_off = count - bits_to_use;
    const uint8_t value_bits = (value >> value_off) & mask;

    uint8_t cur_data = *cur_dst;
    unsigned off = bitsLeft - bits_to_use;
    cur_data &= ~(mask << off);
    cur_data |= value_bits << off;
    *cur_dst = cur_data;

    return WriteData(dst, offset + bits_to_use, count - bits_to_use, value);
}