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
            instance_ = new EncryptService(db);
        }else{
            instance_ = new EncryptService(*value);
        }

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
    EncryptedData encryptedData{};

    try{
        encryptor->encrypt(&decryptedData, &encryptedData);
        DecryptedData outDecryptedData{};
        encryptor->decrypt(&encryptedData, &outDecryptedData);
        if(toStdString(decryptedData)!= toStdString(outDecryptedData)){
            //BOOST_LOG_TRIVIAL(error) << "Cannot encrypt data";
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

void EncryptService::createDecryptedData(const std::string &value, DecryptedData *outDecryptedData) {
    memcpy(outDecryptedData->data, value.c_str(), value.size());
    //int length = value.size() +1;
    //outDecryptedData->data[length] = '\0';
    outDecryptedData->length =value.size();
}

bool EncryptService::containsToken(const Token &token){
    return database->containsToken(token);
}

void EncryptService::decrypt(const Token &token, DecryptedData &outDecryptedData){
    const EncryptedData data = database->getEncryptedData(token);
    encryptor->decrypt(&data, &outDecryptedData);
}


std::string EncryptService::toStdString(const DecryptedData &decryptedData){
    std::string v = std::string((char*) decryptedData.data);
    return v.substr(0, decryptedData.length);
}
