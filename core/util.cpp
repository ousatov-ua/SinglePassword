#include "util.h"
#include <openssl/evp.h>
#include <QByteArray>
#include "encryptservice.h"

Util::Util()
{

}

void Util::getEncKeys(EncKeys *encKeys, const std::string &masterPass) {
    const EVP_CIPHER *cipher;
    const unsigned char *salt = NULL;
    cipher = EVP_get_cipherbyname("aes-256-cbc");
    EVP_BytesToKey(cipher, EVP_get_digestbyname("md5"), salt,
               (unsigned char *) masterPass.c_str(),
               masterPass.size(), 1, encKeys->key,  encKeys->iv);
}
