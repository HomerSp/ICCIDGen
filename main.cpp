#include <algorithm>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstring>

#include "encrypt.h"
#include "hash.h"
#include "utils.h"

void usage(char* prog) {
    std::cerr << "Usage: " << prog << " <input csv> [output csv]\n";
    std::cerr << "\t<input csv> is a comma-separated file with the format:\n";
    std::cerr << "\t\tICCID;KI Key;Transport Key\n";
    std::cerr << "\t\tone per line.\n";
    std::cerr << "\t\tKI Key and Transport Key are optional\n";
    std::cerr << "\t[output csv] is optional and specifies where to write\n";
    std::cerr << "\t\tthe output, in the format: ICCID;SF_EUIMID;EUIMID\n";
    std::cerr << "\t\tone per line.\n";
    std::cerr << "\t\tIf this is not specified it will write to the console.\n";
}

bool processLine(std::string line, std::ostream& output)
{
    std::vector<std::string> split = splitString(line, ';');
    if (split.size() > 0) {
        std::string sfEuimid, euimid;
        iccidToEuimid(split.at(0), sfEuimid, euimid);

        output << split.at(0) << ";" << sfEuimid << ";" << euimid;
    }

    if (split.size() >= 3) {
        std::string key = split.at(2);
        std::string encrypted;
        bool ret = encrypt(split.at(1), key, encrypted, (key.length() == 16) ? TYPE_DES : (key.length() == 48) ? TYPE_DES3 : TYPE_AES128);
        if (ret) {
            output << ";" << encrypted;
        }
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