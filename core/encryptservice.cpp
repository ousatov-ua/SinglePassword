#include "encryptservice.h"
#include "util.h"
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <nlohmann/json.hpp>

namespace fs = boost::filesystem;

EncryptService::EncryptService(const std::string db, const std::string log)
{
    this->encryptor = std::make_unique<Encryptor>(log);
    this->database = std::make_unique<Database>(db);
}

static EncryptService *instance_ = nullptr;

EncryptService *EncryptService::GetInstance(const std::string* value)
{
    if(instance_==nullptr){
        const char *homedir;

        if ((homedir = getenv("HOME")) == NULL) {
            homedir = getpwuid(getuid())->pw_dir;
        }
        fs::path path_s(std::string(homedir) + "/.singlePass");
        if(!fs::exists(path_s)){
            fs::create_directory(path_s);
        }
        std::string log = path_s.string() + "/enc.log";

        if(value == nullptr){
            std::string db = path_s.string() +  "/db.spdb";
            fs::path path_db = db;
            instance_ = new EncryptService(db, log);
        }else{
            instance_ = new EncryptService(*value, log);
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
        DecryptedData data = decryptData(initialToken);
        return Util::isUUIDString(data.getValue());
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
        if(Util::toStdString(decryptedData)!= Util::toStdString(outDecryptedData)){
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

DecryptedData EncryptService::decryptToken(const Token &token){
    DecryptedData decryptedData{};
    decryptData(token.data, decryptedData);
    nlohmann::ordered_json json = nlohmann::ordered_json::parse(decryptedData.data);
    decryptedData.json = json;
    return decryptedData;
}

SaveResult EncryptService::saveTokenData(const Token &token, const DecryptedData &decryptedData){
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

bool EncryptService::tokenExists(const Token &token){
    return database->containsToken(token);
}

DecryptedData EncryptService::decryptData(const Token &token){
    DecryptedData decryptedData{};
    const EncryptedData data = database->getEncryptedData(token);
    decryptData(data, decryptedData);
    nlohmann::ordered_json res = nlohmann::ordered_json::parse(decryptedData.data);
    decryptedData.json = res;
    return decryptedData;
}

void EncryptService::decryptData(const EncryptedData &encryptedData, DecryptedData &outDecryptedData){
    encryptor->decrypt(&encryptedData, &outDecryptedData);
}


