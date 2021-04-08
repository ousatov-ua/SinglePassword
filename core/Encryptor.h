//
// Created by alus on 18.03.2021.
//

#ifndef MY_ENCRYPTOR_ENCRYPTOR_H
#define MY_ENCRYPTOR_ENCRYPTOR_H

#include <string>
#include <array>
#include <boost/serialization/access.hpp>
#include <openssl/evp.h>
#include <nlohmann/json.hpp>
#include "entities.h"


class Encryptor {
public:
    Encryptor(const std::string &log);

    ~Encryptor();

    void logException() const;

    void encrypt(const DecryptedData *decryptedData, EncryptedData *encryptedData) const;

    void decrypt(const EncryptedData *encryptedData, DecryptedData *decryptedData) const;

    EncKeys* getEncKeys();

private:
    FILE *log_;
    std::unique_ptr<EncKeys> encKeys_;
};


#endif //MY_ENCRYPTOR_ENCRYPTOR_H
