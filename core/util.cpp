#include "util.h"
#include <openssl/evp.h>
#include <QByteArray>


Util::Util()
{

}
static const std::string bucket = "abcdefghijklmnopqrstuvwxyz";

void Util::getEncKeys(EncKeys *encKeys, const std::string &masterPass) {
    const EVP_CIPHER *cipher;
    std::string salt_s = std::string(masterPass);
    std::reverse(salt_s.begin(), salt_s.end());
    const unsigned char *salt = (unsigned char*) salt_s.c_str();
    cipher = EVP_get_cipherbyname("aes-256-cbc");
    EVP_BytesToKey(cipher, EVP_get_digestbyname("md5"), salt,
               (unsigned char *) masterPass.c_str(),
               masterPass.size(), 1, encKeys->key,  encKeys->iv);
}


void Util::toToken(std::string plainToken, Token &outToken){
    DecryptedData decryptedToken = createDecryptedData(plainToken);
    EncryptService::GetInstance()->encryptToken(outToken, decryptedToken);
}

void Util::toPlainToken(const std::string &plainToken, Token &outToken){
    memcpy((char*)outToken.data.data, plainToken.c_str(), plainToken.size());
    outToken.data.data[plainToken.size()] = '\0';
    outToken.data.length = plainToken.size();
    outToken.plain = true;
}

DecryptedData Util::createDecryptedData(const std::string &value){
    DecryptedData decryptedData{};
    memcpy(decryptedData.data, value.c_str(), value.size());
    decryptedData.data[value.size()] = '\0';
    decryptedData.length =value.size();
    return decryptedData;
}

const std::string Util::randomString(){
    std::string uuid;
    for (int i=0; i< 16; i++) {
        uuid += bucket[rand() % bucket.size()];
    }
    return uuid;
}

bool Util::isUUIDString(const DecryptedData &decryptedData){
    for(size_t i=0; i< decryptedData.length; i++){
        if(bucket.find(decryptedData.data[i]) == std::string::npos){
            return false;
        }
    }
    return true;
}

const std::string Util::toStdString(const DecryptedData &decryptedData){
    std::string v = std::string((char*) decryptedData.data);
    return v;
}

