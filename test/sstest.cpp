#include <cstdlib>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <vector>

#include "../include/bitstream.h"
#include "../include/utils.h"

struct NaiEntry {
    uint8_t entryIndex;
    std::vector<uint8_t> mnAAA;
    std::vector<uint8_t> mnHA;
};

void decodeData(std::vector<uint8_t>& data, std::vector<NaiEntry>& nais)
{
    BitStream bitStream(data);
    uint64_t bitStart = 0;
    uint32_t numNai = bitStream.read(4);
    std::cout << "numNai " << numNai << "\n";

    for (uint32_t i = 0, index = 0; i < numNai; i++) {
        NaiEntry nai;
        nai.entryIndex = bitStream.read(4);

        int aaaLen = bitStream.read(5);
        for (uint32_t x = 0; x < aaaLen; x++) {
            nai.mnAAA.emplace_back(static_cast<uint8_t>(bitStream.read(8)));
        }

        int haLen = bitStream.read(5);
        for (uint32_t x = 0; x < haLen; x++) {
            nai.mnHA.emplace_back(static_cast<uint8_t>(bitStream.read(8)));
        }

        nais.push_back(nai);
    }
}

void encodeData(std::vector<uint8_t>& data, const std::vector<NaiEntry>& nais)
{
    BitStream bitStream(data);
    bitStream.write(nais.size(), 4);

    for (uint32_t i = 0; i < nais.size(); i++) {
        bitStream.write(i, 4);

        NaiEntry nai = nais[i];

        bitStream.write(nai.mnAAA.size(), 5);
        for (uint8_t d: nai.mnAAA) {
            bitStream.write(d, 8);
        }

        bitStream.write(nai.mnHA.size(), 5);
        for (uint8_t d: nai.mnHA) {
            bitStream.write(d, 8);
        }
    }
}

int main (int argc, char** argv)
{
    if (argc < 2) {
        return -1;
    }

    std::vector<uint8_t> vec;
    hex2bin(argv[1], vec);

    vec.erase(vec.begin());

    std::vector<NaiEntry> nais;
    decodeData(vec, nais);

    for (const NaiEntry& nai: nais) {
        std::cout << "entry " << (int) nai.entryIndex << "\n";

        std::cout << " mnAAA: ";
        for (uint8_t d: nai.mnAAA) {
            printf("%02X", d);
        }

        std::cout << "\n";
        std::cout << " mnHA: ";
        for (uint8_t d: nai.mnHA) {
            std::cout << (char) d;
        }

        std::cout << "\n";
    }

    std::vector<uint8_t> out;
    encodeData(out, nais);

    std::cout << "out: ";
    for (uint8_t d: out) {
        printf("%02X", d);
    }

    std::cout << "\n";

    return 0;
}