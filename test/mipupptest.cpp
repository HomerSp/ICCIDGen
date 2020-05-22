#include <cstdlib>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <vector>

#include "../include/bitstream.h"
#include "../include/utils.h"

int main (int argc, char** argv)
{
    if (argc < 2) {
        return -1;
    }

    std::vector<uint8_t> vec;
    hex2bin(argv[1], vec);

    BitStream bs(vec);

    std::cout << "Total length: " << (int) bs.read(8) << "\n";

    bool retryInfo = (bs.read(1) == 1);
    std::cout << "Retry info: " << retryInfo << "\n";
    if (retryInfo) {
        std::cout << "Max num retry: " << (int) bs.read(2) << "\n"
            << "First retry timeout: " << (int) bs.read(3) << "\n"
            << "Rereg threshold: " << (int) bs.read(6) << "\n";
    }

    int numNai = (int) bs.read(4);
    std::cout << "Num nai: " << numNai << "\n";
    for (int i = 0; i < numNai; i++) {
        std::cout << "Nai index: " << (int) bs.read(4) << "\n";

        int naiLength = (int) bs.read(8);
        std::cout << "Nai length: " << naiLength << "\n";

        std::string nai = "";
        for (int x = 0; x < naiLength; x++) {
            nai += (char) bs.read(8);
        }

        std::cout << "Nai: " << nai << "\n";

        std::cout << "T Bit: " << (int) bs.read(1) << "\n";
        std::cout << "Home address: " << (unsigned long) bs.read(32) << "\n";
        std::cout << "Primary home agent: " << (unsigned long) bs.read(32) << "\n";
        std::cout << "Secondary home agent: " << (unsigned long) bs.read(32) << "\n";

        std::cout << "MN-AAA Auth algo: " << (int) bs.read(4) << "\n";

        bool spiInd = bs.read(1) == 1;
        std::cout << "MN-AAA SPI Indicator: " << spiInd << "\n";
        if (spiInd) {
            std::cout << "MN-AAA SPI: " << (unsigned long) bs.read(32) << "\n";
        }

        std::cout << "MN-HA Auth algo: " << (int) bs.read(4) << "\n";

        spiInd = bs.read(1) == 1;
        std::cout << "MN-HA SPI Indicator: " << spiInd << "\n";
        if (spiInd) {
            std::cout << "MN-HA SPI: " << (unsigned long) bs.read(32) << "\n";
        }
    }

    return 0;
}