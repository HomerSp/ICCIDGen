#include <cmath>
#include <iostream>

#include <openssl/sha.h>

#include "include/hash.h"
#include "include/utils.h"

bool iccidToEuimid(const std::string& iccid, std::string& sfEuimid, std::string& puimid)
{
    if (iccid.length() < 18) {
        return false;
    }

    std::vector<unsigned char> iccidVec;
    hex2bin(iccid.substr(iccid.length() - 15, 14), iccidVec);

    std::vector<unsigned char> sfEuimidVec;
    for (int i = 0; i < 7; i++) {
        sfEuimidVec.emplace_back(iccidVec[6 - i]);
    }

    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(sfEuimidVec.data(), sfEuimidVec.size(), hash);

    std::vector<unsigned char> puimidVec;
    puimidVec.emplace_back(hash[SHA_DIGEST_LENGTH - 1]);
    puimidVec.emplace_back(hash[SHA_DIGEST_LENGTH - 2]);
    puimidVec.emplace_back(hash[SHA_DIGEST_LENGTH - 3]);
    puimidVec.emplace_back(0x80);

    sfEuimid = bin2hex(sfEuimidVec);
    puimid = bin2hex(puimidVec);

    return true;
}

bool iccidToEuimidMeid(const std::string& iccid, std::string& sfEuimid, std::string& puimid)
{
    if (iccid.length() < 18) {
        return false;
    }

    std::vector<unsigned char> iccidVec;
    hex2bin(iccid.substr(iccid.length() - 15, 14), iccidVec);

    std::vector<unsigned char> sfEuimidVec;
    for (int i = 0; i < 7; i++) {
        sfEuimidVec.emplace_back(iccidVec[6 - i]);
    }

    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(sfEuimidVec.data(), sfEuimidVec.size(), hash);

    sfEuimidVec[0] = 0xA0;
    sfEuimidVec[3] = hash[SHA_DIGEST_LENGTH - 1];
    sfEuimidVec[4] = hash[SHA_DIGEST_LENGTH - 2];
    sfEuimidVec[5] = hash[SHA_DIGEST_LENGTH - 3];
    sfEuimidVec[6] = hash[SHA_DIGEST_LENGTH - 4];

    SHA1(sfEuimidVec.data(), sfEuimidVec.size(), hash);

    std::vector<unsigned char> puimidVec;
    puimidVec.emplace_back(0x80);
    puimidVec.emplace_back(hash[SHA_DIGEST_LENGTH - 3]);
    puimidVec.emplace_back(hash[SHA_DIGEST_LENGTH - 2]);
    puimidVec.emplace_back(hash[SHA_DIGEST_LENGTH - 1]);   
    
    sfEuimid = bin2hex(sfEuimidVec);
    puimid = bin2hex(puimidVec);

    return true;
}

bool generateKey(const std::string& iccid, std::string& key)
{
    std::vector<unsigned char> tohash;
    for (int i = 0; i < iccid.size(); i++) {
        tohash.emplace_back(iccid[i]);
    }

    for (int i = 0; i < 16; i++) {
        tohash.emplace_back(rand() % 256);
    }

    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(tohash.data(), tohash.size(), hash);

    std::vector<unsigned char> vec;
    for (int i = 0, y = SHA_DIGEST_LENGTH - 1; i < 16; i++, y--) {
        vec.emplace_back((hash[i] ^ hash[y]) % 0x7F);
    }

    key = bin2hex(vec);

    return true;
}

uint8_t calculateCD(const std::string& iccid)
{
    if (iccid.length() < 18) {
        return 0;
    }

    uint32_t sum = 0;
    for (int i = 0; i < iccid.length(); i++) {
        uint8_t val = (iccid[iccid.length() - i - 1] - '0');
        if (i % 2 == 0) {
            val *= 2;
            sum += std::floor(val / 10.0f);
            sum += val % 10;
        } else {
            sum += val;
        }
    }

    return (sum * 9) % 10;
}