#include "util.h"

Util::Util()
{

}

void Util::getEncKeys(EncKeys *encKeys, const std::string &masterPass) {
    unsigned char key[KEY_LENGTH];
    int length = masterPass.size();
    strcpy(reinterpret_cast<char *>(key), masterPass.c_str());
    if (length < KEY_LENGTH) {
            while (length < KEY_LENGTH) {
                key[length] = KEY_SPACE;
                length++;
            }
        }


        unsigned char iv[VI_LENGTH];
        memcpy(iv, key, VI_LENGTH);
        encKeys->key = std::string((char *) key);
        encKeys->iv = std::string((char *) iv);
}
