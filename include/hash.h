#ifndef ICCIDGEN_HASH_H
#define ICCIDGEN_HASH_H

#include <string>
#include <vector>

bool iccidToEuimid(const std::string& iccid, std::string& sfEuimid, std::string& puimid);
bool iccidToEuimidMeid(const std::string& iccid, std::string& sfEuimid, std::string& puimid);

bool generateKey(const std::string& iccid, std::string& key);

uint8_t calculateCD(const std::string& iccid);

bool generateHRPDChapSS(const std::string& a12Chap, std::string &chapSS);
bool generateUserSS(const std::string& mnHA, const std::string& mnAAA, std::string &userSS);

std::string generateMDN(const std::string& iccid);
std::string generateHRPDUPP(const std::string& user);
std::string generateMIPUPP(const std::string& user);

#endif // ICCIDGEN_HASH_H
