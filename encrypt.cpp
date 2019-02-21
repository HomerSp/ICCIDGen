#include <iostream>

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/sha.h>
#include <openssl/applink.c> 

#include "include/encrypt.h"
#include "include/utils.h"

static const std::vector<unsigned char> IV_16_NULL = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

bool encrypt(const std::string& input, const std::string& key, std::string& output, ENC_TYPE type)
{
    std::vector<unsigned char> outputVec;
    if (encrypt(input, key, outputVec, type)) {
        output = bin2hex(outputVec);
        return true;
    }

    return false;
}

bool encrypt(const std::string& input, const std::string& key, std::vector<unsigned char>& output, ENC_TYPE type)
{
    std::vector<unsigned char> inputVec, keyVec;
    if (!hex2bin(input, inputVec) || !hex2bin(key, keyVec)) {
        return false;
    }

    return encrypt(inputVec, keyVec, output, type);
}

bool encrypt(const std::string& input, const std::vector<unsigned char>& key, std::vector<unsigned char>& output, ENC_TYPE type)
{
    std::vector<unsigned char> inputVec;
    if (!hex2bin(input, inputVec)) {
        return false;
    }

    return encrypt(inputVec, key, output, type);
}

bool encrypt(const std::vector<unsigned char>& input, const std::vector<unsigned char>& key, std::vector<unsigned char>& output, ENC_TYPE type)
{
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        ERR_print_errors_fp(stderr);
        return false;
    }

    const EVP_CIPHER *cipherType = nullptr;
    switch (type) {
        case TYPE_AES128:
            cipherType = EVP_aes_128_cbc();
            break;
        case TYPE_DES:
            cipherType = EVP_des_cbc();
            break;
        case TYPE_DES2:
            cipherType = EVP_des_ede_cbc();
            break;
        case TYPE_DES3:
            cipherType = EVP_des_ede3_cbc();
            break;
        default:
            std::cerr << "Unknown encryption type\n";
            break;
    }

    if (EVP_CIPHER_key_length(cipherType) != key.size()) {
        std::cerr << "Invalid key length " << key.size() << ", should be " << EVP_CIPHER_key_length(cipherType) << "\n";
        return false;
    }

    std::vector<unsigned char> iv(EVP_CIPHER_iv_length(cipherType), 0x0);
    if (EVP_EncryptInit_ex(ctx, cipherType, NULL, key.data(), IV_16_NULL.data()) != 1) {
        ERR_print_errors_fp(stderr);
        return false;
    }

    EVP_CIPHER_CTX_set_padding(ctx, 0);

    output.resize(input.size() * 2);

    int lenUpdate, lenFinal;
    if (EVP_EncryptUpdate(ctx, output.data(), &lenUpdate, input.data(), input.size()) != 1) {
        ERR_print_errors_fp(stderr);
        return false;
    }

    if (EVP_EncryptFinal_ex(ctx, output.data() + lenUpdate, &lenFinal) != 1) {
        ERR_print_errors_fp(stderr);
        return false;
    }

    output.resize(lenUpdate + lenFinal);

    EVP_CIPHER_CTX_free(ctx);

    return true;
}
