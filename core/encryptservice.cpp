#include "encryptservice.h"
#include "util.h"
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

namespace fs = boost::filesystem;

EncryptService::EncryptService(const std::string db)
{
    this->encryptor = std::make_unique<Encryptor>();
    this->database = std::make_unique<Database>(db);
}

static EncryptService *instance_ = nullptr;

EncryptService *EncryptService::GetInstance(const std::string* value)
{
    if(instance_==nullptr){
        if(value == nullptr){
            const char *homedir;

            if ((homedir = getenv("HOME")) == NULL) {
                homedir = getpwuid(getuid())->pw_dir;
            }
            fs::path path_s(std::string(homedir) + "/.singlePass");
            if(!fs::exists(path_s)){
                fs::create_directory(path_s);
            }
            std::string db = path_s.string() +  "/db.spdb";
            fs::path path_db = db;
            instance_ = new EncryptService(db);
        }else{
            instance_ = new EncryptService(*value);
        }

    }
    return instance_;
}

void EncryptService::initializeDb(const std::string &pass){

    Util::getEncKeys(encryptor->getEncKeys(), pass);
    Token initialToken{};
    Util::toPlainToken(INITIAL_TOKEN, initialToken);

    EncryptedData encryptedData{};
    DecryptedData decryptedData = Util::createDecryptedData(Util::randomString());
    encryptor->encrypt(&decryptedData, &encryptedData);
    database->addToken(initialToken, encryptedData);
}

bool EncryptService::initialTokenExists(){
    Token initialToken{};
    Util::toPlainToken(INITIAL_TOKEN, initialToken);
    return database->tokenExists(initialToken);
}

bool EncryptService::validatePass(const std::string &pass){
    try
    {
        Util::getEncKeys(encryptor->getEncKeys(), pass);
        Token initialToken{};
        Util::toPlainToken(INITIAL_TOKEN, initialToken);
        const EncryptedData encryptedData = database->getEncryptedData(initialToken);
        DecryptedData decryptedData{};
        encryptor->decrypt(&encryptedData, &decryptedData);
        return Util::isUUIDString(decryptedData);
    }catch(const DecryptException &e){
        return false;
    }
}

std::vector<Token> EncryptService::getTokens() const {
    return database->getTokens();
}


bool EncryptService::encrypt(EncryptedData &outEncryptedData, const DecryptedData &decryptedData){
    try{
        encryptor->encrypt(&decryptedData, &outEncryptedData);
        DecryptedData outDecryptedData{};
        encryptor->decrypt(&outEncryptedData, &outDecryptedData);
        if(toStdString(decryptedData)!= toStdString(outDecryptedData)){
            return false;
        }
        return true;
    } catch(const std::exception &e){
        return false;
    }
}

bool EncryptService::encryptToken(Token &outToken, const DecryptedData &decryptedData){
    try{
        return encrypt(outToken.data, decryptedData);
    }catch(const std::exception &e){
        return SAVE_FAILED;
    }
}

SaveResult EncryptService::encrypt(const Token &token, const DecryptedData &decryptedData){
    EncryptedData encryptedData{};
    try{
        if(!encrypt(encryptedData, decryptedData)){
            return SAVE_FAILED;
        }
        return database->addToken(token, encryptedData);
    } catch(const std::exception &e){
        return SAVE_FAILED;
    }
}

SaveResult EncryptService::removeToken(const Token &token){
    return database->removeToken(token);
}

bool EncryptService::containsToken(const Token &token){
    return database->containsToken(token);
}

void EncryptService::decryptValue(const Token &token, DecryptedData &outDecryptedData){
    const EncryptedData data = database->getEncryptedData(token);
    decrypt(data, outDecryptedData);
}

void EncryptService::decrypt(const EncryptedData &encryptedData, DecryptedData &outDecryptedData){
    encryptor->decrypt(&encryptedData, &outDecryptedData);
}


std::string EncryptService::toStdString(const DecryptedData &decryptedData){
    std::string v = std::string((char*) decryptedData.data);
    return v;
}
