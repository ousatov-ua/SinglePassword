#ifndef UTIL_H
#define UTIL_H

#include <string>
#include "core/Encryptor.h"

class Util
{
public:

    static void getEncKeys(EncKeys *encKeys, const std::string &masterPass);

private:
    Util();

};

#endif // UTIL_H
