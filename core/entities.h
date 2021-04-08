#ifndef ENTITIES_H
#define ENTITIES_H

#define BUFFER_SIZE 4096
#define PASS_MIN_LENGTH 8

#include <openssl/evp.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <boost/serialization/access.hpp>


struct EncKeys {
    unsigned char key [EVP_MAX_KEY_LENGTH];
    unsigned char iv [EVP_MAX_IV_LENGTH];
};


struct DecryptedData {
    unsigned char data[BUFFER_SIZE];
    size_t length;
    nlohmann::ordered_json json;

    const std::string getValue() const{
        return json["value"];
    }

    friend std::ostream &operator<<(std::ostream &ostream, const DecryptedData &decryptedData) {
        std::string_view v = std::string_view((char *) decryptedData.data);
        return ostream << v.substr(0, decryptedData.length);
    }

    friend bool operator==(const std::string &string, const DecryptedData &decryptedData) {
        std::string_view v = std::string_view((char *) decryptedData.data);
        return string == v.substr(0, decryptedData.length);
    }

    friend bool operator!=(const std::string &string, const DecryptedData &decryptedData) {
        return !(string == decryptedData);
    }
};

struct EncryptedData {
    unsigned char data[BUFFER_SIZE];
    size_t length;

    friend bool operator==(const std::string &string, const EncryptedData &encryptedData) {
        std::string_view v = std::string_view((char *) encryptedData.data);
        return string == v.substr(0, encryptedData.length);
    }

private:
    friend class boost::serialization::access;

    template<typename Archive>
    void serialize(Archive &ar, const unsigned int /*version */) {
        ar & data;
        ar & length;
    }
};

struct Token {
    EncryptedData data;
    bool plain = false;

    bool operator<(const Token &ob) const {
        return strcmp((char*)data.data, (char*)ob.data.data) < 0;
    }


private:
    friend class boost::serialization::access;

    template<typename Archive>
    void serialize(Archive &ar, const unsigned int /* version */) {
        ar & data;
        ar & plain;
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

#endif // ENTITIES_H
