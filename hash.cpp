#include <cmath>
#include <iostream>

#include <openssl/sha.h>

#include "include/hash.h"
#include "include/bitstream.h"
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

    sfEuimidVec.clear();
    sfEuimidVec.emplace_back(0xA0);
    sfEuimidVec.emplace_back(0x00);
    sfEuimidVec.emplace_back(0x00);
    for (int i = 1; i < 5; i++) {
        sfEuimidVec.emplace_back(hash[SHA_DIGEST_LENGTH - i]);
    }

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

static const std::string KEY_CHARS = "%0123456789ABCDEFGHJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

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
        int index = (hash[i] ^ hash[y]) % KEY_CHARS.length();
        vec.emplace_back(KEY_CHARS[index]);
    }

    key = bin2hex(vec);

    return true;
}

bool generateHRPDChapSS(const std::string& a12Chap, std::string &chapSS)
{
    std::vector<unsigned char> a12ChapData;
    hex2bin(a12Chap, a12ChapData);

    std::vector<uint8_t> bitData;
    BitStream bitStream(bitData);
    bitStream.write(a12ChapData.size(), 5);
    for (unsigned char d: a12ChapData) {
        bitStream.write(d, 8);
    }

    std::vector<unsigned char> data;
    data.emplace_back(static_cast<uint8_t>(bitData.size()));
    data.insert(data.end(), bitData.begin(), bitData.end());

    chapSS = bin2hex(data);
    return true;
}

bool generateUserSS(const std::string& mnHA, const std::string& mnAAA, std::string &userSS)
{
    std::vector<unsigned char> mnHAData, mnAAAData;
    hex2bin(mnHA, mnHAData);
    hex2bin(mnAAA, mnAAAData);

    std::vector<uint8_t> bitData;
    BitStream bitStream(bitData);
    bitStream.write(1, 4);
    bitStream.write(0, 4);

    bitStream.write(mnAAAData.size(), 5);
    for (unsigned char d: mnAAAData) {
        bitStream.write(d, 8);
    }

    bitStream.write(mnHAData.size(), 5);
    for (unsigned char d: mnHAData) {
        bitStream.write(d, 8);
    }

    std::vector<unsigned char> data;
    data.emplace_back(static_cast<uint8_t>(bitData.size()));
    data.insert(data.end(), bitData.begin(), bitData.end());

    userSS = bin2hex(data);
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