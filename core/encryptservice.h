#ifndef ENCRYPTSERVICE_H
#define ENCRYPTSERVICE_H

#include <memory>
#include "Database.h"
#include "Encryptor.h"

class EncryptService
{
public:
    EncryptService(EncryptService &other) = delete;
    void operator=(const EncryptService &) = delete;
    static EncryptService *GetInstance(const std::string* value = nullptr);
    void initializeDb(const std::string &pass);
    bool initialTokenExists();
    bool validatePass(const std::string &pass);
    std::vector<Token> getTokens() const;
    bool encryptToken(Token &outToken, const DecryptedData &data);
    DecryptedData decryptToken(const Token &token);
    SaveResult saveTokenData(const Token &token, const DecryptedData &data);
    DecryptedData decryptData(const Token &token);
    SaveResult removeToken(const Token &token);
    bool tokenExists(const Token &token);
private:
    EncryptService(const std::string db);
    std::unique_ptr<Database> database;
    std::unique_ptr<Encryptor> encryptor;
    bool encrypt(EncryptedData &outEncryptedData, const DecryptedData &decryptedData);
    void decryptData(const EncryptedData& encryptedData, DecryptedData &outDecryptedData);
};

#endif // ENCRYPTSERVICE_H
