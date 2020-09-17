

//
//  scp11c.cpp
//  JubSDK
//
//  Created by Pan Min on 2020/6/9.
//  Copyright © 2020 JuBiter. All rights reserved.
//


#include <TrezorCrypto/aes.h>
#include <TrezorCrypto/cmac.h>
#include "scp11/scp11c.hpp"


std::vector<unsigned char> scp11c::getMutualAuthData() {

    // 6.5 MUTUAL AUTHENTICATE Command
    // Table 6-17: MUTUAL AUTHENTICATE Data Field
    scp11_mutual_auth ma(&shared_info,
                         e_pk);
    return ma.encode();
}


bool scp11c::keyDerivation(const scp11_response_msg &response_msg, scp11_session_key& session_key) {

    unsigned char ShSss[SHA1_DIGEST_LENGTH] = {0x00,};
    if (!_calcShSss(response_msg.sd_pk.value.data(),
                    oce_rk.data(),
                    ShSss)) {
        return false;
    }

    unsigned char ShSes[SHA1_DIGEST_LENGTH] = {0x00,};
    if (!_calcShSes(response_msg.sd_pk.value.data(),
                    e_rk.data(),
                    ShSes)) {
        return false;
    }

    // OCE derives AES sessionkeys from ShSes and ShSss
    unsigned char z[SHA1_DIGEST_LENGTH*2] = {0x00,};
    unsigned int zLen = sizeof(z)/sizeof(unsigned char);
    memcpy(z, ShSes, SHA1_DIGEST_LENGTH);
    memcpy(z + SHA1_DIGEST_LENGTH, ShSss, SHA1_DIGEST_LENGTH);

    std::vector<unsigned char> shared = scp11_sharedInfo(shared_info).encodeV(oce_crt.cakloc_id.value);
    unsigned int shared_info_len = (unsigned int)shared.size();
    if (0 == shared_info_len) {
        return false;
    }

    unsigned int outKeyLen = scp11_session_key::size();
    unsigned char *outKey = new unsigned char[outKeyLen+1];
    memset(outKey, 0x00, outKeyLen+1);
    outKeyLen = kdf2_sha256(z, zLen,
                            shared.data(), (int)shared.size(),
                            outKey, outKeyLen);

    std::vector<unsigned char> session;
    session.insert(session.begin(), outKey, outKey+outKeyLen);
    delete [] outKey; outKey = nullptr;

    return session_key.decode(session, scp11_session_key::KEY_LENGTH);
}


bool scp11c::checkReceipt(const scp11_response_msg &response_msg, const std::vector<unsigned char> &key_dek) {

    scp11_mutual_auth ma(&shared_info,
                         e_pk,
                         response_msg.sd_pk.value);
    std::vector<unsigned char> receiptInputData = ma.encode();

    // check if receipt == mac
    unsigned char outmac[16] = {0x00,};
    std::vector<unsigned char> key(key_dek);
    AES_CMAC(key.data(), receiptInputData.data(), (int)receiptInputData.size(),
             outmac);

    // using c++11 instead of c++14
//    return std::equal(std::begin(response_msg.receipt.value), std::end(response_msg.receipt.value),
//    std::begin(outmac));
    std::vector<unsigned char> vOutmac(outmac, outmac + sizeof(outmac)/sizeof(unsigned char));
    return (response_msg.receipt.value == vOutmac);
}


bool scp11c::openSecureChannel(const std::vector<unsigned char>& response) {

    scp11_response_msg response_msg;
    if (!response_msg.decode(response)) {
        return false;
    }

    scp11_session_key session_key;
    if (!keyDerivation(response_msg, session_key)) {
        return false;
    }

    // check the receipt
    if (!checkReceipt(response_msg, session_key.key_dek)) {
        return false;
    }

    setSessionKey(session_key);
    setResponseMsg(response_msg);

    return true;
}
