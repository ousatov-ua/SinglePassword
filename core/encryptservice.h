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
    SaveResult encrypt(const Token &token, const DecryptedData &data);
    void decryptValue(const Token &token, DecryptedData &outDecryptedData);
    void decrypt(const EncryptedData& encryptedData, DecryptedData &outDecryptedData);
    SaveResult removeToken(const Token &token);
    void createDecryptedData(const std::string &value, DecryptedData *outDecryptedData);
    bool containsToken(const Token &token);
    std::string toStdString(const DecryptedData &decryptedData);
private:
    EncryptService(const std::string db);
    std::unique_ptr<Database> database;
    std::unique_ptr<Encryptor> encryptor;
    bool encrypt(EncryptedData &outEncryptedData, const DecryptedData &decryptedData);
    bool databaseExists;
};

#endif // ENCRYPTSERVICE_H
