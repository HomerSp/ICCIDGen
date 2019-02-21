#ifndef ICCIDGEN_UTILS_H
#define ICCIDGEN_UTILS_H

#include <string>
#include <vector>

std::string bin2hex(const std::vector<unsigned char>&vec);
bool hex2bin(const std::string &str, std::vector<unsigned char>& out);

std::vector<std::string> splitString(const std::string& input, char delim);

#endif // ICCIDGEN_UTILS_H
