//
// Created by alus on 18.03.2021.
//

#include "Encryptor.h"
#include <openssl/ossl_typ.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#include <utility>


Encryptor::Encryptor() {
    encKeys_ = std::make_unique<EncKeys>();
    log_ = fopen("enc.log", "a");
}

Encryptor::~Encryptor() {
    fclose(log_);
}

void Encryptor::encrypt(DecryptedData *decryptedData,
                        EncryptedData *encryptedData) const {
    try {
        EVP_CIPHER_CTX *ctx;

        int len;

        unsigned char result[BUFFER_SIZE];
        int ciphertext_len;

        if (!(ctx = EVP_CIPHER_CTX_new()))
            throw EncryptException("Cannot initialize context");

        if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, (unsigned char *) encKeys_->key.c_str(),
                                    (unsigned char *) encKeys_->iv.c_str())) {
            throw EncryptException("Cannot initialize encryption");
        }

        if (1 != EVP_EncryptUpdate(ctx, result, &len, decryptedData->result, decryptedData->length)) {
            throw EncryptException("Cannot encrypt");
        }
        ciphertext_len = len;

        if (1 != EVP_EncryptFinal_ex(ctx, result + len, &len)) {
            throw EncryptException("Validation failed");
        }
        ciphertext_len += len;

        EVP_CIPHER_CTX_free(ctx);

        encryptedData->rawData = std::string((char *) result);
        encryptedData->length = ciphertext_len;
    } catch (std::exception &e) {
        logException();
        throw;
    }
}

void Encryptor::logException() const {
    ERR_print_errors_fp(log_);
}

void Encryptor::decrypt(EncryptedData *encryptedData,
                        DecryptedData *decryptedData) const {
    try {
        EVP_CIPHER_CTX *ctx;

        int len;

        int plaintext_len;

        if (!(ctx = EVP_CIPHER_CTX_new())) {
            throw DecryptException("Cannot initialize context");
        }

        if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, (unsigned char *) encKeys_->key.c_str(),
                                    (unsigned char *) encKeys_->iv.c_str())) {
            throw DecryptException("Cannot initialize decryption");
        }

        if (1 != EVP_DecryptUpdate(ctx, decryptedData->result, &len, (unsigned char *) encryptedData->rawData.c_str(),
                                   encryptedData->length)) {
            throw DecryptException("cannot decrypt message");
        }
        plaintext_len = len;

        if (1 != EVP_DecryptFinal_ex(ctx, decryptedData->result + len, &len)) {
            throw DecryptException("Validation failed");
        }
        plaintext_len += len;

        EVP_CIPHER_CTX_free(ctx);
        decryptedData->length = plaintext_len;
    } catch (std::exception &e) {
        logException();
        throw;
    }
}

EncKeys *Encryptor::getEncKeys() {
    return this->encKeys_.get();
}

void Encryptor::createDecryptedData(const std::string &value, DecryptedData *decryptedData) {
    memcpy(decryptedData->result, value.c_str(), value.size());
    decryptedData->length = value.size();
}
