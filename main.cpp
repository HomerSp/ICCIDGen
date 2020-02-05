#include <algorithm>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstring>

#include "include/encrypt.h"
#include "include/hash.h"
#include "include/utils.h"

static int sVersion = 2;
static int sVersionMinor = 5;

void usage(char* prog) {
    std::cerr << "iccidgen v" << sVersion << "." << sVersionMinor << "\n"
        << "Usage: " << prog << " [-1] <input csv> [output csv]\n"
        << "\t-1\n"
        << "\t\tUse format version 1 (defaults to 2)\n"
        << "\n"
        << "\t<input csv>\n"
        << "\t\tA comma-separated file, see Formats below for details\n"
        << "\t\tone per line.\n"
        << "\t\tKI Key and Transport Key are optional\n"
        << "\n"
        << "\t[output csv]\n"
        << "\t\tOutput file, where to write the data\n"
        << "\t\toptional and defaults to the console\n"
        << "\n"
        << "Formats\n"
        << "\tVersion 1:\n"
        << "\t\tInput\n"
        << "\t\tICCID;KI Key;Transport Key\n"
        << "\t\tOutput\n"
        << "\t\tICCID;SF_EUIMID;pUIMID;Encrypted KI Key\n"
        << "\tVersion 2:\n"
        << "\t\tInput\n"
        << "\t\tICCID;MN HA HEX\n"
        << "\t\tOutput\n"
        << "\t\tICCID;SF_EUIMID;pUIMID;A12 CHAP;MN HA;MN AAA;HRPDCHAPSS;MIPPS\n";
}

bool processLine(std::string line, std::ostream& output)
{
    std::vector<std::string> split = splitString(line, ';');
    if (split.size() > 0) {
        std::string iccid = split.at(0);
        if (iccid.length() % 2 == 1) {
            iccid += '0' + calculateCD(iccid);
        }

        std::string sfEuimid, puimid;
        switch (sVersion) {
        case 1:
            iccidToEuimid(iccid, sfEuimid, puimid);
            output << iccid << ";" << sfEuimid << ";" << puimid;

            if (split.size() >= 3) {
                std::string key = split.at(2);
                std::string encrypted;
                bool ret = encrypt(split.at(1), key, encrypted, (key.length() == 16) ? TYPE_DES : (key.length() == 48) ? TYPE_DES3 : TYPE_AES128);
                if (ret) {
                    output << ";" << encrypted;
                }
            }

            break;
        case 2:
            std::string a12chap, mnaaa;
            iccidToEuimidMeid(iccid, sfEuimid, puimid);
            generateKey(iccid, a12chap);
            generateKey(sfEuimid, mnaaa);
            output << iccid << ";" << sfEuimid << ";" << puimid << ";" << a12chap;

            if (split.size() > 1) {
                std::string mnha = split.at(1);
                output << ";" << mnha << ";" << mnaaa;

                std::string chapSS = "";
                generateChapSS(a12chap, chapSS);
                output << ";" << chapSS;

                std::string userSS = "";
                generateUserSS(mnha, mnaaa, userSS);
                output << ";" << userSS;
            }

            break;
        }       
    }

    output << "\n";

    return true;
}

int main(int argc, char **argv) {
    srand(time(0));

    if (argc < 2 || argc > 4) {
        usage(argv[0]);
        return -1;
    }

    int arg = 1;
    if (std::string(argv[arg]) == "-1") {
        sVersion = 1;
        arg++;
    }

    std::ifstream stream(argv[arg], std::ifstream::in);
    if (!stream.good()) {
        std::cerr << "Could not open file " << argv[arg] << "\n";
        return -1;
    }

    std::ostream *output = &std::cout;
    std::ofstream outFile;
    if (argc == arg + 2) {
        outFile.open(argv[arg + 1], std::ofstream::out);
        if (!outFile.good()) {
            std::cerr << "Could not open output file " << argv[arg + 1] << "\n";
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