#ifndef UTIL_H
#define UTIL_H

#include <string>
#include "core/Encryptor.h"

class Util
{
public:
    Util();

    static void getEncKeys(EncKeys *encKeys, const std::string &masterPass);
};

#endif // UTIL_H
