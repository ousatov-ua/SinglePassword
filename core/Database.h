//
// Created by alus on 21.03.2021.
//

#ifndef MY_ENCRYPTOR_DATABASE_H
#define MY_ENCRYPTOR_DATABASE_H

#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <boost/serialization/access.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/map.hpp>
#include "Encryptor.h"

#define SAVE_FAILED 0
#define SAVE_SUCCESS 1

#define INITIAL_TOKEN std::string("??$$#initial#$$??")
#define INITIAL_VALUE std::string("initial database")

using SaveResult = int;


struct Token {
    std::string data;

    std::string toString() const {
        return "Token(data = " + data + ")";
    }

    bool operator<(const Token &ob) const {
        return strcmp(data.c_str(), ob.data.c_str()) < 0;
    }

private:
    friend class boost::serialization::access;

    template<typename Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & data;
    }
};

static std::ostream &operator<<(std::ostream &os, Token &arg) {
    os << "title = " << arg.data;
    return os;
}

class Database {
public:
    explicit Database(const char *file);

    bool tokenExists(const Token &token) const;

    std::vector<Token> getTokens() const;

    SaveResult addToken(const Token &token, const EncryptedData &encryptedData);

    SaveResult removeToken(const Token &token);

    const EncryptedData &getEncryptedData(const Token &token) const;

    bool containsToken(const Token &token) const;

private:
    const char *path_;
    std::map<Token, EncryptedData> data_;

    SaveResult save();
};


#endif //MY_ENCRYPTOR_DATABASE_H