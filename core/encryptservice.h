#ifndef ENCRYPTSERVICE_H
#define ENCRYPTSERVICE_H

#include <memory>
#include "Database.h"
#include "Encryptor.h"

class EncryptService
{
public:
    EncryptService();
    void initializeDb(const std::string &pass);
    bool initialTokenExists();
    bool validatePass(const std::string &pass);
    std::vector<Token> getTokens() const;
    SaveResult encrypt(const Token &token, const DecryptedData &data);
    void decrypt(const Token &token, DecryptedData &outDecryptedData);
    SaveResult removeToken(const Token &token);
    void createDecryptedData(const std::string &value, DecryptedData *outDecryptedData);
    bool containsToken(const Token &token);
private:
    std::unique_ptr<Database> database;
    std::unique_ptr<Encryptor> encryptor;
};

#endif // ENCRYPTSERVICE_H