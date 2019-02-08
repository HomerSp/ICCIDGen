#include <algorithm>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstring>

#include <openssl/sha.h>

void usage(char* prog) {
    std::cerr << "Usage: " << prog << " <iccid file> [output csv]\n";
    std::cerr << "\t<iccid list> is a text file with one iccid per line\n";
    std::cerr << "\t[output csv] is optional and specifies where to write\n";
    std::cerr << "\t\tthe output, in the format: ICCID;SF_EUIMID;EUIMID\n";
    std::cerr << "\t\tone per line.\n";
    std::cerr << "\t\tIf this is not specified it will write to stdout.\n";
}

void hex2bin(const std::string &str, uint8_t* out, int len) {
    for(int i = 0; i < len; i++) {
        char c1 = str[i * 2] - '0', c2 = str[i * 2 + 1] - '0';
        out[i] = (c1 << 4) | (c2 & 0x0f);
    }
}

bool processLine(std::string line, std::ostream& output) {
    uint8_t iccid[9];
    if (line.length() == 19) {
        hex2bin(line.substr(1), iccid, 9);
    } else if (line.length() == 18) {
        hex2bin(line, iccid, 9);
    } else {
        std::cerr << "Invalid iccid " << line << "\n";
        return false;
    }

    uint8_t sf_euimid[7];
    for (int i = 0; i < 7; i++) {
        sf_euimid[i] = iccid[8 - i];
    }

    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(sf_euimid, 7, hash);

    uint8_t euimid[4];
    euimid[0] = hash[SHA_DIGEST_LENGTH - 1];
    euimid[1] = hash[SHA_DIGEST_LENGTH - 2];
    euimid[2] = hash[SHA_DIGEST_LENGTH - 3];
    euimid[3] = 0x80;

    output << line << ";";
    for (int i = 0; i < 7; i++) {
        output << std::hex << std::setw(2) << std::setfill('0') << std::uppercase << +sf_euimid[i];
    }

    output << ";";

    for (int i = 0; i < 4; i++) {
        output << std::hex << std::setw(2) << std::setfill('0') << std::uppercase << +euimid[i];
    }

    output << "\n";

    return true;

}

int main(int argc, char **argv) {
    if (argc < 2 || argc > 3) {
        usage(argv[0]);
        return -1;
    }

    std::ifstream stream(argv[1], std::ifstream::in);
    if (!stream.good()) {
        std::cerr << "Could not open file " << argv[1] << "\n";
        return -1;
    }

    std::ostream *output = &std::cout;
    std::ofstream outFile;
    if (argc == 3) {
        outFile.open(argv[2], std::ofstream::out);
        if (!outFile.good()) {
            std::cerr << "Could not open output file " << argv[2] << "\n";
            return -1;
        }

        output = &outFile;
    }

    std::string line;
    while (stream.good()) {
        std::getline(stream, line);

        line.erase(
            std::remove_if(line.begin(), line.end(),
                      [](unsigned char x){return std::isspace(x);}),
            line.end()
        );

        if (line.length() == 0) {
            continue;
        }

        if (!processLine(line, *output)) {
            break;
        }
    }
    
    return 0;
}