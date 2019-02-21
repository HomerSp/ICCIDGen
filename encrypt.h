#ifndef ICCIDGEN_ENCRYPT_H
#define ICCIDGEN_ENCRYPT_H

#include <string>
#include <vector>

enum ENC_TYPE {
    TYPE_UNKNOWN = 0,
    TYPE_AES128,
    TYPE_DES,
    TYPE_DES2,
    TYPE_DES3
};

bool encrypt(const std::string& input, const std::string& key, std::string& output, ENC_TYPE type);
bool encrypt(const std::string& input, const std::string& key, std::vector<unsigned char>& output, ENC_TYPE type);
bool encrypt(const std::string& input, const std::vector<unsigned char>& key, std::vector<unsigned char>& output, ENC_TYPE type);
bool encrypt(const std::vector<unsigned char>& input, const std::vector<unsigned char>& key, std::vector<unsigned char>& output, ENC_TYPE type);

#endif // ICCIDGEN_ENCRYPT_H
