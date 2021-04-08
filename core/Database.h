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
#include "entities.h"

#define SAVE_FAILED 0
#define SAVE_SUCCESS 1

#define INITIAL_TOKEN std::string("??$$#initial#$$??")

using SaveResult = int;

class Database {

public:

    explicit Database(const std::string file);

    bool tokenExists(const Token &token) const;

    std::vector<Token> getTokens() const;

    SaveResult addToken(const Token &token, const EncryptedData &encryptedData);

    SaveResult removeToken(const Token &token);

    const EncryptedData &getEncryptedData(const Token &token) const;

    bool containsToken(const Token &token) const;

    SaveResult save();

private:
    const std::string path_;
    std::map<Token, EncryptedData> data_;

};


#endif //MY_ENCRYPTOR_DATABASE_H
