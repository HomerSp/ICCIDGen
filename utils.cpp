#include <sstream>
#include <iomanip>

#include "include/utils.h"

char atoc(char a) {
    if (a >= '0' && a <= '9') {
        return a - '0';
    }
    if (a >= 'A' && a <= 'F') {
        return 10 + a - 'A';
    }
    if (a >= 'a' && a <= 'f') {
        return 10 + a - 'a';
    }
    return 0;
}

std::string bin2hex(const std::vector<unsigned char>&vec)
{
    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::uppercase;
    for (auto c: vec)
        ss << std::setw(2) << +c;

    return ss.str();
}

bool hex2bin(const std::string &str, std::vector<unsigned char>& out)
{
    if (str.length() % 2 != 0) {
        return false;
    }

    out.resize(str.length() / 2);
    for(int i = 0; i < out.size(); i++) {
        char c1 = atoc(str[i * 2]), c2 = atoc(str[i * 2 + 1]);
        out[i] = (c1 << 4) | (c2 & 0x0f);
    }

    return true;
} 

std::vector<std::string> splitString(const std::string& input, char delim)
{
    std::vector<std::string> ret;

    int start = 0, end;
    while ((end = input.find(delim, start)) != std::string::npos) {
        ret.emplace_back(input.substr(start, end - start));
        start = end + 1;
    }

    if (start < input.length()) {
        ret.emplace_back(input.substr(start));
    }

    return ret;
}
