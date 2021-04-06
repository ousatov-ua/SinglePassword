//
// Created by alus on 21.03.2021.
//

#include "Database.h"
#include <fstream>
#include <map>

using namespace boost::archive;

Database::Database(const std::string path):path_(path) {
    std::ifstream file(path_, std::ios::binary);
    if (!file.is_open()) {
        data_ = std::map<Token, EncryptedData>();
        return;
    }
    binary_iarchive ia{file};
    std::string data_s;
    ia >> data_;
}

SaveResult Database::save() {
    std::ofstream f(path_, std::ios::binary);
    if (f.fail()) {
        return SAVE_FAILED;
    }
    binary_oarchive oa(f);
    oa << data_;
    return SAVE_SUCCESS;
}

bool Database::tokenExists(const Token &token) const {
    return data_.count(token);
}

std::vector<Token> Database::getTokens() const {
    std::vector<Token> tokens;
    for (auto const &imap: data_) {
        tokens.push_back(imap.first);
    }
    return tokens;
}

SaveResult Database::addToken(const Token &token, const EncryptedData &encryptedData) {
    data_[token] = encryptedData;
    return save();
}

SaveResult Database::removeToken(const Token &token){
    data_.erase(token);
    return save();
}

const EncryptedData &Database::getEncryptedData(const Token &token) const {
    return data_.at(token);
}

bool Database::containsToken(const Token &token) const{
    return data_.count(token) > 0;
}
