#include "encryptservice.h"
#include "util.h"

EncryptService::EncryptService()
{
    this->encryptor = std::make_unique<Encryptor>();
    this->database = std::make_unique<Database>("db.spdb");
}

void EncryptService::initializeDb(const std::string &pass){

    std::string initValue = INITIAL_VALUE;
    const Token initialToken{.data = INITIAL_TOKEN};
    Util::getEncKeys(encryptor->getEncKeys(), pass);
    EncryptedData encryptedData{};
    DecryptedData decryptedData{};
    encryptor->createDecryptedData(initValue, &decryptedData);
    encryptor->encrypt(&decryptedData, &encryptedData);
    database->addToken(initialToken, encryptedData);
}

bool EncryptService::initialTokenExists(){
    const Token initialToken{.data = INITIAL_TOKEN};
    std::string initValue = INITIAL_VALUE;
    return database->tokenExists(initialToken);
}

bool EncryptService::validatePass(const std::string &pass){
    try
    {
        DecryptedData decryptedData{};
        const Token initialToken{.data = INITIAL_TOKEN};
        std::string initValue = INITIAL_VALUE;
        EncryptedData encryptedData = database->getEncryptedData(initialToken);
        EncKeys encKeys{};
        Util::getEncKeys(encryptor->getEncKeys(), pass);
        encryptor->decrypt(&encryptedData, &decryptedData);
        return initValue == decryptedData;
    }catch(const DecryptException &e){
        return false;;
    }
}
