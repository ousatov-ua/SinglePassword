//
// Created by alus on 18.03.2021.
//

#ifndef MY_ENCRYPTOR_ENCRYPTOR_H
#define MY_ENCRYPTOR_ENCRYPTOR_H

#include <string>
#include <array>
#include <boost/serialization/access.hpp>
#include <openssl/evp.h>

#define BUFFER_SIZE 1024
#define PASS_MIN_LENGTH 8

struct EncKeys {
    unsigned char key [EVP_MAX_KEY_LENGTH];
    unsigned char iv [EVP_MAX_IV_LENGTH];
};

struct DecryptedData {
    unsigned char result[BUFFER_SIZE];
    int length = BUFFER_SIZE;

    friend std::ostream &operator<<(std::ostream &ostream, const DecryptedData &decryptedData) {
        std::string_view v = std::string_view((char *) decryptedData.result);
        return ostream << v.substr(0, decryptedData.length);
    }

    friend bool operator==(const std::string &string, const DecryptedData &decryptedData) {
        std::string_view v = std::string_view((char *) decryptedData.result);
        return string == v.substr(0, decryptedData.length);
    }

    friend bool operator!=(const std::string &string, const DecryptedData &decryptedData) {
        return !(string == decryptedData);
    }


};

//std::string toStdString(const DecryptedData &decryptedData);



struct EncryptedData {
    std::string rawData;
    int length;
private:
    friend class boost::serialization::access;

    template<typename Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & rawData;
        ar & length;
    }
};


struct EncryptException : public std::exception {
    const std::string message;

    explicit EncryptException(const char *message) : message(message) {
    }

    const char *what() const noexcept override {
        return message.c_str();
    }
};

struct DecryptException : public std::exception {
    std::string message;

    explicit DecryptException(const char *message) : message(message) {

    }

    const char *what() const noexcept override {
        return message.c_str();
    }
};

class Encryptor {
public:
    Encryptor();

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
