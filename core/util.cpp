#include "util.h"
#include <openssl/evp.h>
#include <QByteArray>


Util::Util()
{

}
static const std::string bucket = "abcdefghijklmnopqrstuvwxyz";

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
    nlohmann::ordered_json res;
    res["type"] = "notes";
    res["value"]=value;
    const std::string data = res.dump();
    DecryptedData decryptedData{};
    memcpy(decryptedData.data, data.c_str(), data.size());
    decryptedData.data[data.size()] = '\0';
    decryptedData.length = data.size();
    return decryptedData;
}

const std::string Util::randomString(){
    std::string uuid;
    for (int i=0; i< 16; i++) {
        uuid += bucket[rand() % bucket.size()];
    }
    return uuid;
}

bool Util::isUUIDString(const std::string &value){
    for(size_t i=0; i< value.size(); i++){
        if(bucket.find(value[i]) == std::string::npos){
            return false;
        }
    }
    return true;
}

const std::string Util::toStdString(const DecryptedData &decryptedData){
    std::string v = std::string((char*) decryptedData.data);
    return v;
}

