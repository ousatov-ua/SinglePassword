#ifndef UTIL_H
#define UTIL_H

#include <string>
#include "core/Encryptor.h"
#include "encryptservice.h"
#include <nlohmann/json.hpp>

class Util
{
public:

    static void toToken(std::string plainToken, Token &outToken);
    static void toPlainToken(const std::string &plainToken, Token &outToken);
    static DecryptedData createDecryptedData(const std::string &value);
    static const std::string randomString();
    static bool isUUIDString(const std::string &decryptedData);
    static const std::string toStdString(const DecryptedData &decryptedData);

private:
    Util();

};

#endif // UTIL_H
