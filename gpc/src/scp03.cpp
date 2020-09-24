//
//  scp03.cpp
//  JubSDK
//
//  Created by Pan Min on 2020/6/21.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#include "scp03/scp03.hpp"
#include <TrezorCrypto/aes.h>
#include <TrezorCrypto/cmac.h>

int gpc_scp03::counter = 1;

bool scp03::pad80(const unsigned char* text, int offset, int length, int blocksize,
                  unsigned char* pad, int* ppadlength) {

    int totalLength = length;
    for (totalLength++; (totalLength % blocksize) != 0; totalLength++) {
        ;
    }
    int padlength = totalLength - length;
    *ppadlength = (length+padlength);
    if (nullptr == pad) {
        return true;
    }
    else if ((*ppadlength)<padlength) {
        return false;
    }

    memcpy(pad, text+offset, length);
    pad[length] = 0x80;
    for (int i = 1; i < padlength; i++) {
        pad[length+i] = 0x00;
    }

    return true;
}


bool scp03::unpad80(unsigned char* pad, int padlength,
                    unsigned char* unpad, int* punpadlength) {

    int offset = padlength - 1;
    while (offset > 0 && pad[offset] == 0) {
        offset--;
    }
    if (pad[offset] != 0x80) {
        return false;
    }

    if (nullptr == unpad) {
        *punpadlength = offset;
        return true;
    }

    *punpadlength = offset;
    memcpy(unpad, pad, offset);
    return true;
}


bool scp03::icv(const unsigned char *key, const int keyLen,
                unsigned char *icv, int* icvLen, bool forWrap) {

    aes_encrypt_ctx encCtx;
    jub::JUB_DebugLog("scp03::icv::key            [%d]: %s\n", keyLen, uchar_vector(key, keyLen).getHex().c_str());
    if (EXIT_SUCCESS != aes_encrypt_key(key, keyLen, &encCtx)) {
        return false;
    }

    std::vector<unsigned char> counter;
    if (forWrap) {
        counter.push_back((uint8_t)getCounter());
        counter.resize(AES_BLOCK_SIZE);
        std::reverse(std::begin(counter), std::end(counter));
    }
    else {
        counter.push_back((uint8_t)incCounter());
        counter.resize(AES_BLOCK_SIZE);
        std::reverse(std::begin(counter), std::end(counter));
        counter[0] = 0x80;
    }
    jub::JUB_DebugLog("scp03::encrypt::counter blk[%d]: %s\n", counter.size(), uchar_vector(counter).getHex().c_str());

    memset(const_Zero, 0x00, sizeof(const_Zero)/sizeof(unsigned char));
    if (EXIT_SUCCESS != aes_cbc_encrypt(counter.data(), icv,
                                        AES_BLOCK_SIZE, const_Zero,
                                        &encCtx)) {
        return false;
    }
    jub::JUB_DebugLog("scp03::icv::icv            [%d]: %s\n", AES_BLOCK_SIZE, uchar_vector(icv, AES_BLOCK_SIZE).getHex().c_str());

    return true;
}


bool scp03::encrypt(const unsigned char *key, const int keyLen,
                    const unsigned char *icv, const int icvLen,
                    const unsigned char *cmd, const int cmdLen,
                    unsigned char *enc, int* pencLen) {

    // pading
    unsigned char *padding = nullptr;
    int paddingLen = 0;
    if (!pad80(cmd, 0, cmdLen,
               AES_BLOCK_SIZE,
               padding, &paddingLen)) {
        return false;
    }
    if (nullptr == enc) {
        *pencLen = paddingLen;
        return true;
    }
    else if ((*pencLen) < paddingLen) {
        return false;
    }

    // AES encrypt
    aes_encrypt_ctx ctx;
    if (EXIT_SUCCESS != aes_encrypt_key(key, keyLen, &ctx)) {
        return false;
    }

    // pading
    padding = new unsigned char[paddingLen+1];
    memset(padding, 0x00, paddingLen+1);
    if (!pad80(cmd, 0, cmdLen,
               AES_BLOCK_SIZE,
               padding, &paddingLen)) {
        delete [] padding; padding = nullptr;
        return false;
    }

    jub::JUB_DebugLog("scp03::encrypt::        cmd[%d]: %s\n", cmdLen, uchar_vector(cmd, cmdLen).getHex().c_str());
    jub::JUB_DebugLog("scp03::encrypt::pading  cmd[%d]: %s\n", paddingLen, uchar_vector(padding, paddingLen).getHex().c_str());

    if (EXIT_SUCCESS != aes_cbc_encrypt(padding, enc,
                                        paddingLen, (unsigned char*)icv,
                                        &ctx)) {
        delete [] padding; padding = nullptr;
        return false;
    }

    jub::JUB_DebugLog("scp03::encrypt::enc     cmd[%d]: %s\n", paddingLen, uchar_vector(enc, paddingLen).getHex().c_str());

    delete [] padding; padding = nullptr;

    return true;
}


bool scp03::decrypt(const unsigned char *key, const int keyLen,
                    const unsigned char *icv, const int icvLen,
                    const unsigned char *enc, const int encLen,
                    unsigned char *dec, int* pdecLen) {

    int paddingLen = encLen;
    *pdecLen = paddingLen;
    if (nullptr == dec) {
        return true;
    }

    aes_decrypt_ctx ctx;
    if (EXIT_SUCCESS != aes_decrypt_key(key, keyLen, &ctx)) {
        *pdecLen = 0;
        return false;
    }

    unsigned char *padding = new unsigned char[paddingLen+1];
    memset(padding, 0x00, paddingLen+1);
    if (EXIT_SUCCESS != aes_cbc_decrypt(enc, padding,
                                        encLen, (unsigned char*)icv,
                                        &ctx)) {
        delete [] padding; padding = nullptr;
        *pdecLen = 0;
        return false;
    }

    jub::JUB_DebugLog("scp03::decrypt::enc apdu   [%d]: %s\n", encLen, uchar_vector(enc, encLen).getHex().c_str());
    jub::JUB_DebugLog("scp03::decrypt::padding    [%d]: %s\n", encLen, uchar_vector(padding, paddingLen).getHex().c_str());

    // unpading
    int decLen = *pdecLen;
    if (!unpad80(padding, paddingLen,
                 dec, &decLen)) {
        delete [] padding; padding = nullptr;
        return false;
    }

    jub::JUB_DebugLog("scp03::decrypt::unpadd     [%d]: %s\n", decLen, uchar_vector(dec, decLen).getHex().c_str());

    delete [] padding; padding = nullptr;

    *pdecLen = decLen;

    return true;
}


bool scp03::cmac(const unsigned char *key, const int keyLen,
                 const unsigned char *data, const int dataLen,
                 unsigned char *mac, int* macLen) {
    jub::JUB_DebugLog("scp11::cmac::key  [%d]: %s\n", keyLen, uchar_vector(key,   keyLen).getHex().c_str());
    jub::JUB_DebugLog("scp11::cmac::data [%d]: %s\n", dataLen, uchar_vector(data, dataLen).getHex().c_str());

    int outmacLen = keyLen;
    if (nullptr == mac) {
        *macLen = outmacLen;
        return false;
    }

    unsigned char *outmac = new unsigned char[outmacLen+1];
    memset(outmac, 0x00, outmacLen+1);
    AES_CMAC((unsigned char *)key, (unsigned char *)data, dataLen,
             outmac);
    if ((*macLen) <= outmacLen) {
        memcpy(mac, outmac, *macLen);
    jub::JUB_DebugLog("scp11::cmac::mac  [%d]: %s\n", (*macLen), uchar_vector(mac, (*macLen)).getHex().c_str());
    }
    delete [] outmac; outmac = nullptr;

    return true;
}


void scp03::macChaining(const std::vector<unsigned char>& chain) {

    mac_chain.clear();
    mac_chain.insert(mac_chain.end(), chain.begin(), chain.end());
    jub::JUB_DebugLog("scp03::macChaining[%d]: %s\n", mac_chain.size(), uchar_vector(mac_chain).getHex().c_str());
}
