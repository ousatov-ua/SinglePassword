#include "encryptservice.h"
#include "util.h"

EncryptService::EncryptService(const std::string &db)
{
    this->encryptor = std::make_unique<Encryptor>();
    this->database = std::make_unique<Database>(db.c_str());
}

static EncryptService *instance_ = nullptr;

EncryptService *EncryptService::GetInstance(const std::string& value)
{
    if(instance_==nullptr){
        instance_ = new EncryptService(value);
    }
    return instance_;
}

void EncryptService::initializeDb(const std::string &pass){

    std::string initValue = INITIAL_VALUE;
    const Token initialToken{.data = INITIAL_TOKEN};
    Util::getEncKeys(encryptor->getEncKeys(), pass);
    EncryptedData encryptedData{};
    DecryptedData decryptedData{};
    createDecryptedData(initValue, &decryptedData);
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
        Util::getEncKeys(encryptor->getEncKeys(), pass);
        encryptor->decrypt(&encryptedData, &decryptedData);
        return initValue == decryptedData;
    }catch(const DecryptException &e){
        return false;;
    }
}

std::vector<Token> EncryptService::getTokens() const {
    return database->getTokens();
}

SaveResult EncryptService::encrypt(const Token &token, const DecryptedData &decryptedData){
    EncryptedData encryptedData;
    encryptor->encrypt(&decryptedData, &encryptedData);
    DecryptedData outDecryptedData{};
    encryptor->decrypt(&encryptedData, &outDecryptedData);
    if(toStdString(decryptedData)!= toStdString(outDecryptedData)){
        throw DecryptException("Cannot verify decryption!");
    }
    return database->addToken(token, encryptedData);
}

SaveResult EncryptService::removeToken(const Token &token){
    return database->removeToken(token);
}

void EncryptService::createDecryptedData(const std::string &value, DecryptedData *outDecryptedData) {
    memcpy(outDecryptedData->result, value.c_str(), value.size());
    outDecryptedData->length = value.size();
}

bool EncryptService::containsToken(const Token &token){
    return database->containsToken(token);
}

void EncryptService::decrypt(const Token &token, DecryptedData &outDecryptedData){
    const EncryptedData data = database->getEncryptedData(token);
    encryptor->decrypt(&data, &outDecryptedData);
}


std::string EncryptService::toStdString(const DecryptedData &decryptedData){
    std::string v = std::string((char*) decryptedData.result);
    return v.substr(0, decryptedData.length);
}
