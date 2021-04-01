//
// Created by alus on 18.03.2021.
//

#ifndef MY_ENCRYPTOR_ENCRYPTOR_H
#define MY_ENCRYPTOR_ENCRYPTOR_H

#include <string>
#include <array>
#include <boost/serialization/access.hpp>

#define BUFFER_SIZE 4096
#define KEY_LENGTH 32
#define VI_LENGTH 16
#define KEY_MIN_LENGTH 8
#define KEY_SPACE '$'

struct EncKeys {
    std::string key;
    std::string iv;
};

struct DecryptedData {
    unsigned char result[BUFFER_SIZE];
    int length;

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

    void encrypt(DecryptedData *decryptedData, EncryptedData *encryptedData) const;

    void decrypt(EncryptedData *encryptedData, DecryptedData *decryptedData) const;

    EncKeys* getEncKeys();

    void createDecryptedData(const std::string &value, DecryptedData *decryptedData);

private:
    FILE *log_;
    std::unique_ptr<EncKeys> encKeys_;
};


#endif //MY_ENCRYPTOR_ENCRYPTOR_H
