#ifndef ICCIDGEN_HASH_H
#define ICCIDGEN_HASH_H

#include <string>
#include <vector>

bool iccidToEuimid(const std::string& iccid, std::string& sfEuimid, std::string& puimid);
bool iccidToEuimidMeid(const std::string& iccid, std::string& sfEuimid, std::string& puimid);

bool generateKey(const std::string& iccid, std::string& key);

uint8_t calculateCD(const std::string& iccid);

#endif // ICCIDGEN_HASH_H
