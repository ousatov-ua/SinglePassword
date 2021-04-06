#ifndef UTIL_H
#define UTIL_H

#include <string>
#include "core/Encryptor.h"
#include "encryptservice.h"

class Util
{
public:

    static void getEncKeys(EncKeys *encKeys, const std::string &masterPass);
    static void toToken(std::string plainToken, Token &outToken);
    static void toPlainToken(const Token &token, DecryptedData &outDecryptedData);

private:
    Util();

};

#endif // UTIL_H
