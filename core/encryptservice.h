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
private:
    std::unique_ptr<Database> database;
    std::unique_ptr<Encryptor> encryptor;
};

#endif // ENCRYPTSERVICE_H
