#include <openssl/sha.h>

#include "include/hash.h"
#include "include/utils.h"

bool iccidToEuimid(const std::string& iccid, std::string& sfEuimid, std::string& euimid)
{
    if (iccid.length() < 18) {
        return false;
    }
    
    std::vector<unsigned char> iccidVec;
    hex2bin(iccid.substr(iccid.length() - 14, 14), iccidVec);

    std::vector<unsigned char> sfEuimidVec;
    for (int i = 0; i < 7; i++) {
        sfEuimidVec.emplace_back(iccidVec[6 - i]);
    }

    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(sfEuimidVec.data(), sfEuimidVec.size(), hash);

    std::vector<unsigned char> euimidVec;
    euimidVec.emplace_back(hash[SHA_DIGEST_LENGTH - 1]);
    euimidVec.emplace_back(hash[SHA_DIGEST_LENGTH - 2]);
    euimidVec.emplace_back(hash[SHA_DIGEST_LENGTH - 3]);
    euimidVec.emplace_back(0x80);

    sfEuimid = bin2hex(sfEuimidVec);
    euimid = bin2hex(euimidVec);

    return true;
}