

//
//  scp11.cpp
//  JubSDK
//
//  Created by Pan Min on 2020/6/9.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#include "mSIGNA/stdutils/uchar_vector.h"

#include "scp11/scp11.hpp"
#include "tlvparse.c"
#include <TrezorCrypto/rand.h>
#include <TrezorCrypto/aes.h>
#include <TrezorCrypto/cmac.h>
#include <utility/Debug.hpp>


/// scp11_sharedInfo
std::vector<unsigned char> scp11_sharedInfo::encodeLV() {

    std::vector<unsigned char> out;

    if (   key_usage.value.empty()
        || key_type.value.empty()
        || key_length.value.empty()
        || host_cardID.value.empty()
        ) {
        return out;
    }
    std::vector<unsigned char> keyUsageLV = key_usage.encodeLV();
    std::vector<unsigned char> keyTypeLV = key_type.encodeLV();
    std::vector<unsigned char> keyLengthLV = key_length.encodeLV();
    std::vector<unsigned char> hostCardIDLV = host_cardID.encodeLV();
    out.insert(out.end(), keyUsageLV.begin(), keyUsageLV.end());
    out.insert(out.end(), keyTypeLV.begin(), keyTypeLV.end());
    out.insert(out.end(), keyLengthLV.begin(), keyLengthLV.end());
    out.insert(out.end(), hostCardIDLV.begin(), hostCardIDLV.end());

    return out;
}


size_t scp11_sharedInfo::encodeLV(std::vector<unsigned char>& lv) {

    lv = encodeLV();

    return lv.size();
}


std::vector<unsigned char> scp11_sharedInfo::encode() {

    std::vector<unsigned char> out;

    if (   scp_id_param.value.empty()
        || key_usage.value.empty()
        || key_type.value.empty()
        || key_length.value.empty()
        || host_cardID.value.empty()
        ) {
        return out;
    }

    std::vector<unsigned char> scpIDParam = scp_id_param.encode();
    std::vector<unsigned char> keyUsage = key_usage.encode();
    std::vector<unsigned char> keyType = key_type.encode();
    std::vector<unsigned char> keyLength = key_length.encode();
    std::vector<unsigned char> hostCardID = host_cardID.encode();
    out.insert(out.end(), scpIDParam.begin(), scpIDParam.end());
    out.insert(out.end(), keyUsage.begin(), keyUsage.end());
    out.insert(out.end(), keyType.begin(), keyType.end());
    out.insert(out.end(), keyLength.begin(), keyLength.end());
    out.insert(out.end(), hostCardID.begin(), hostCardID.end());

    return out;
}


size_t scp11_sharedInfo::encode(std::vector<unsigned char>& code) {

    code = encode();

    return code.size();
}


std::vector<unsigned char> scp11_sharedInfo::encodeV(const std::vector<unsigned char>& card_groupID) {

    std::vector<unsigned char> out;

    if (   key_usage.value.empty()
        || key_type.value.empty()
        || key_length.value.empty()
        || host_cardID.value.empty()
        || card_groupID.empty()
        ) {
        return out;
    }
    std::vector<unsigned char> keyUsageV = key_usage.encodeV();
    std::vector<unsigned char> keyTypeV = key_type.encodeV();
    std::vector<unsigned char> keyLengthV = key_length.encodeV();
    std::vector<unsigned char> hostCardIDLV = host_cardID.encodeLV();
    out.insert(out.end(), keyUsageV.begin(), keyUsageV.end());
    out.insert(out.end(), keyTypeV.begin(), keyTypeV.end());
    out.insert(out.end(), keyLengthV.begin(), keyLengthV.end());
    out.insert(out.end(), hostCardIDLV.begin(), hostCardIDLV.end());
    out.insert(out.end(), card_groupID.begin(), card_groupID.end());

    return out;
}


size_t scp11_sharedInfo::encodeV(const std::vector<unsigned char>& card_groupID,
                                 std::vector<unsigned char>& v) {

    v = encodeV(card_groupID);

    return v.size();
}


/// scp11_mutual_auth
std::vector<unsigned char> scp11_mutual_auth::encode() {

    std::vector<unsigned char> out;

    if (empty()) {
        return out;
    }

    std::vector<unsigned char> kA = ka.encode();
    out.insert(out.end(), kA.begin(), kA.end());
    std::vector<unsigned char> ePK = e_pk.encode();
    out.insert(out.end(), ePK.begin(), ePK.end());
    if (!sd_pk.empty()) {
        std::vector<unsigned char> sdPK = sd_pk.encode();
        out.insert(out.end(), sdPK.begin(), sdPK.end());
    }

    return out;
}


size_t scp11_mutual_auth::encode(std::vector<unsigned char>& code) {

    code = encode();

    return code.size();
}


/// scp11_pk
/// Decodes the provided buffer into the pk.
bool scp11_pk::decode(const std::vector<unsigned char>& data) {

    unsigned char *p = NULL;
    p = new unsigned char[data.size()+1];
    memset(p, 0x00, data.size()+1);
    std::copy(data.begin(), data.end(), p);

    // parse skip
    size_t len = data.size();
    uint16_t tag = 0;
    unsigned char e = *(p+len);
    if (0 != gpc_tlv_get_tag(&p, &e, &tag, &len)) {
        delete[] p; p = NULL;
        return false;
    }

    if (e == *p) {
        delete[] p; p = NULL;
        return true;
    }

    tlv_buf buf;
    unsigned char skip = 1;
    switch (tag) {
    case GPC_TLV_PK_Q:
    case GPC_TLV_PK_PARAM:
    {
        skip = 2;
        break;
    }
    default:
        return false;
    }

    // copy tag into buf
    buf.tag = tag;
    if (!buf.value.empty()) {
        buf.value.clear();
    }
    buf.value.assign(data.begin()+skip, data.begin()+skip+len);
    skip += len;

    // copy buf into member variable
    switch (tag) {
    case GPC_TLV_PK_Q:
    {
        q.tag = buf.tag;
        q.value = buf.value;
        break;
    }
    case GPC_TLV_PK_PARAM:
    {
        param.tag = buf.tag;
        param.value = buf.value;
        break;
    }
    default:
        break;
    }

    std::vector<unsigned char> recursive;
    recursive.insert(recursive.begin(), data.begin()+skip, data.end());

    delete[] p; p = NULL;
    if (0 == recursive.size()) {
        return true;
    }
    else {
        return decode(recursive);
    }
}

//7F21 81E8
//93   10 434552545F4150505F45434B41303031 - Certificate Serial Number(18)
//42   0D 6A75626974657277616C6C6574   - CA-KLOC (or KA-KLOC) Identifier(15)
//5F20 0D 6A75626974657277616C6C6574   - Subject Identifier(16)
//95   02 0080                         - Key Usage(4) Key agreement
//5F25 04 20200525                     - Effective  Date (YYYYMMDD, O BCD format)(7)
//5F24 04 20250524                     - Expiration Date (YYYYMMDD, BCD M format)(7)
//53 00                                - Discretionary Data(2)
//BF20 0E                              - Restrictions under SCP11c(17)
//        EF 0C                          - System specific parameters
//              8D 0A                      - List of CLA/INS command combinations which are allowed in addition to the GlobalPlatform commands listed below in Table B-3.
//                    82 01                  - MUTUAL AUTHENTICATE(6.5.2.3 Data Field Sent in the Command Message)
//                    82 02
//                    82 03
//                    82 04
//                    82 05
//7F49 46                              - Public Key(+3)
//        B0 41 048FD3FAB3907C5CC8CD193EB2B653EA179115B7F305C9E21DE6D29C0736A3B82025B219F24BDA86D80F5AE262521E124F4C6691A0C47B1FB72D95895E9312CB0D           - public_key_q(67)
//        F0 01 00                       - key_parameter_reference(3)
//5F37 46 304402204D75EAA2F09604A9597DA905D680EB619B8ADCF080E5AD6950E1DBF26195C9E2022067649AFB4A8BC380B382520499C6F2BB350A8519B0ECDBE0B7374AA898826D0E(73)
bool scp11_crt::decode() {
    if (raw.value.empty()) {
        return false;
    }

    return decode(raw.value);
}


/// Decodes the provided buffer into the certificate.
bool scp11_crt::decode(const std::vector<unsigned char>& data) {

    unsigned char *p = NULL;
    p = new unsigned char[data.size()+1];
    memset(p, 0x00, data.size()+1);
    std::copy(data.begin(), data.end(), p);

    // parse skip
    size_t len = data.size();
    uint16_t tag = 0;
    unsigned char e = *(p+len);
    if (0 != gpc_tlv_get_tag(&p, &e, &tag, &len)) {
        delete[] p; p = NULL;
        return false;
    }

    if (e == *p) {
        delete[] p; p = NULL;
        return true;
    }

    tlv_buf buf;
    unsigned char skip = 1;
    switch (tag) {
    case GPC_TLV_SCP11CRT_ENTITY:
    {
        skip = 2;
        if (0x100 < len) {
            skip += 3;
        }
        else {
            skip += 2;
        }
        break;
    }
    case GPC_TLV_SCP11CRT_SN:
    case GPC_TLV_SCP11CRT_CAKLOCID:
    case GPC_TLV_SCP11CRT_KEYUSAGE:
    case GPC_TLV_SCP11CRT_DISC_53:
    case GPC_TLV_SCP11CRT_DISC_73:
    {
        skip = 2;
        break;
    }
    case GPC_TLV_SCP11CRT_PUBKEY:
    case GPC_TLV_SCP11CRT_SUBJECTID:
    case GPC_TLV_SCP11CRT_EFFEDATE:
    case GPC_TLV_SCP11CRT_EXPEDATE:
    case GPC_TLV_SCP11CRT_SIGNATURE:
    case GPC_TLV_SCP11CRT_BF_RESTR:
    {
        skip = 3;
        break;
    }
    default:
        return false;
    }

    // copy tag into buf
    buf.tag = tag;
    if(!buf.value.empty()) {
        buf.value.clear();
    }
    buf.value.assign(data.begin()+skip, data.begin()+skip+len);
    if (GPC_TLV_SCP11CRT_ENTITY != tag) {
        skip += len;
    }

    // copy buf into member variable
    switch (tag) {
    case GPC_TLV_SCP11CRT_ENTITY:
    {
        tbs.tag   = buf.tag;
        tbs.value = buf.value;
        break;
    }
    case GPC_TLV_SCP11CRT_SN:
    {
        serial.tag   = buf.tag;
        serial.value = buf.value;
        break;
    }
    case GPC_TLV_SCP11CRT_KEYUSAGE:
    {
        key_usage = 0;
        for (size_t i=0; i<len; ++i) {
            key_usage += buf.value[i];
        }
//        key_usage.tag   = buf.tag;
//        key_usage.value = buf.value;
        break;
    }
    case GPC_TLV_SCP11CRT_SUBJECTID:
    {
        subject_id.tag   = buf.tag;
        subject_id.value = buf.value;
        break;
    }
    case GPC_TLV_SCP11CRT_EFFEDATE:
    {
        valid_from.tag   = buf.tag;
        valid_from.value = buf.value;
        break;
    }
    case GPC_TLV_SCP11CRT_EXPEDATE:
    {
        valid_to.tag   = buf.tag;
        valid_to.value = buf.value;
        break;
    }
    case GPC_TLV_SCP11CRT_CAKLOCID:
    {
        cakloc_id.tag   = buf.tag;
        cakloc_id.value = buf.value;
        break;
    }
    case GPC_TLV_SCP11CRT_PUBKEY:
    {
        std::vector<unsigned char> recursive;
        recursive.insert(recursive.begin(), buf.value.begin(), buf.value.end());
        if (0 < recursive.size()) {
            if (!pk.decode(recursive)) {
                delete[] p; p = NULL;
                return false;
            }
        }
        break;
    }
    case GPC_TLV_SCP11CRT_SIGNATURE:
    {
        sig.tag   = buf.tag;
        sig.value = buf.value;
        break;
    }
    default:
        break;
    }

    std::vector<unsigned char> recursive;
    recursive.insert(recursive.begin(), data.begin()+skip, data.end());

    delete[] p; p = NULL;
    if (0 == recursive.size()) {
        return true;
    }
    else {
        return decode(recursive);
    }
}


int scp11_session_key::N = 5;
int scp11_session_key::KEY_LENGTH = 0x10;
/// Decodes the provided buffer into the scp11_session_key.
bool scp11_session_key::decode() {
    if (raw.empty()) {
        return false;
    }

    return decode(raw.value);
}


bool scp11_session_key::decode(const std::vector<unsigned char>& data,
                               const int key_length) {

    if (scp11_session_key::N != data.size()/key_length
        ||                      data.size()%key_length
        ) {
        return false;
    }

    for (int i=0; i<scp11_session_key::N; ++i) {
        switch (i) {
        case ENUM_SCP11_SESSION::KEY_DEK:
        {
            key_dek.resize(key_length);
            std::copy(std::begin(data)+i*key_length, std::begin(data)+(i+1)*key_length,
                      std::begin(key_dek));
            DEBUG_LOG("scp11_session_key::decode::key_dek[%d]: %s\n", key_dek.size(), uchar_vector(key_dek).getHex().c_str());
            break;
        }
        case ENUM_SCP11_SESSION::S_ENC:
        {
            s_enc.resize(key_length);
            std::copy(std::begin(data)+i*key_length, std::begin(data)+(i+1)*key_length,
                      std::begin(s_enc));
            DEBUG_LOG("scp11_session_key::decode::s_enc[%d]:   %s\n", s_enc.size(), uchar_vector(s_enc).getHex().c_str());
            break;
        }
        case ENUM_SCP11_SESSION::S_MAC:
        {
            s_mac.resize(key_length);
            std::copy(std::begin(data)+i*key_length, std::begin(data)+(i+1)*key_length,
                      std::begin(s_mac));
            DEBUG_LOG("scp11_session_key::decode::s_mac[%d]:   %s\n", s_mac.size(), uchar_vector(s_mac).getHex().c_str());
            break;
        }
        case ENUM_SCP11_SESSION::S_RMAC:
        {
            s_rmac.resize(key_length);
            std::copy(std::begin(data)+i*key_length, std::begin(data)+(i+1)*key_length,
                      std::begin(s_rmac));
            DEBUG_LOG("scp11_session_key::decode::s_rmac[%d]:  %s\n", s_rmac.size(), uchar_vector(s_rmac).getHex().c_str());
            break;
        }
        case ENUM_SCP11_SESSION::S_DEK:
        {
            s_dek.resize(key_length);
            std::copy(std::begin(data)+i*key_length, std::begin(data)+(i+1)*key_length,
                      std::begin(s_dek));
            DEBUG_LOG("scp11_session_key::decode::s_dek[%d]:   %s\n", s_dek.size(), uchar_vector(s_dek).getHex().c_str());
            break;
        }
        default:
            break;
        }
    }

    return true;
}


std::vector<unsigned char> scp11_session_key::encode() {
    
    std::vector<unsigned char> out;

    if (empty()) {
        return out;
    }

    out.insert(out.end(), key_dek.begin(), key_dek.end());
    out.insert(out.end(), s_enc.begin(), s_enc.end());
    out.insert(out.end(), s_mac.begin(), s_mac.end());
    out.insert(out.end(), s_rmac.begin(), s_rmac.end());
    out.insert(out.end(), s_dek.begin(), s_dek.end());

    return out;
}


/// gpc_scp11_response_msg
/// Decodes the provided buffer into the receipt_key.
//Table 6-18: KeyData Assignment
// 5F 49
// 41 04B0AD102DDEE0BA9E584A4F07D50273B0CCAED60F43B0F29E37784700F9A73800F25599DDF497FCE3312785791222D065D5C4203BEF083468B0F616AAC08C2202
// 86 10 864D6867BAF9085B6C2F37EE11E40BD6
bool scp11_response_msg::decode(const std::vector<unsigned char>& data) {

    unsigned char *p = NULL;
    p = new unsigned char[data.size()+1];
    memset(p, 0x00, data.size()+1);
    std::copy(data.begin(), data.end(), p);

    // parse skip
    size_t len = data.size();
    uint16_t tag = 0;
    unsigned char e = *(p+len);
    if (0 != gpc_tlv_get_tag(&p, &e, &tag, &len)) {
        delete[] p; p = NULL;
        return false;
    }

    if (e == *p) {
        delete[] p; p = NULL;
        return true;
    }

    tlv_buf buf;
    unsigned char skip = 2;
    switch (tag) {
    case GPC_TLV_MA_PK:
    {
        skip += 1;
        break;
    }
    case GPC_TLV_MA_RECEIPT:
    {
        break;
    }
    default:
        return false;
    }

    // copy tag into buf
    buf.tag = tag;
    if (!buf.value.empty()) {
        buf.value.clear();
    }
    buf.value.assign(data.begin()+skip, data.begin()+skip+len);
    skip += len;

    // copy buf into member variable
    switch (tag) {
    case GPC_TLV_MA_PK:
    {
        sd_pk.tag   = buf.tag;
        sd_pk.value = buf.value;
        break;
    }
    case GPC_TLV_MA_RECEIPT:
    {
        receipt.tag   = buf.tag;
        receipt.value = buf.value;
        break;
    }
    default:
        break;
    }

    std::vector<unsigned char> recursive;
    recursive.insert(recursive.begin(), data.begin()+skip, data.end());

    delete[] p; p = NULL;
    if (0 == recursive.size()) {
        return true;
    }
    else {
        return decode(recursive);
    }
}


/// struct gpc_scp11
bool scp11::initialize() {

    // OCE key pair(SK.OCE.ECKA, PK.OCE.ECKA)
    if (empty()) {
        return false;
    }

    // OCE generates ephemeral key pair(eSK.OCE.ECKA, ePK.OCE.ECKA)
    unsigned char oce_e_priv_key[SHA256_DIGEST_LENGTH] = {0x00,};
    unsigned char oce_e_pub_key[MAX_ADDR_RAW_SIZE] = {0x00,};
    if (!generateKeyPair(NIST256P1_NAME,
                         oce_e_pub_key, oce_e_priv_key)) {
        return false;
    }

    e_rk.insert(e_rk.begin(), std::begin(oce_e_priv_key), std::end(oce_e_priv_key));
    e_pk.insert(e_pk.begin(), std::begin(oce_e_pub_key),  std::end(oce_e_pub_key));

    return true;
}


bool scp11::generateKeyPair(const char *curve_name,
                            unsigned char* e_pk, unsigned char*e_rk) {

    if (   nullptr == e_pk
        || nullptr == e_rk
        ) {
        return false;
    }

    unsigned char oce_e_priv_key[SHA256_DIGEST_LENGTH] = {0x00,};
    random_buffer(oce_e_priv_key, sizeof(oce_e_priv_key)/sizeof(unsigned char));
    unsigned char oce_e_pub_key[MAX_ADDR_RAW_SIZE] = {0x00,};
    curve_info *curi = (curve_info *)get_curve_by_name(curve_name);
    ecdsa_get_public_key65(curi->params, oce_e_priv_key, oce_e_pub_key);

    memcpy(e_pk, oce_e_pub_key, MAX_ADDR_RAW_SIZE);
    memcpy(e_rk, oce_e_priv_key, SHA256_DIGEST_LENGTH);

    return true;
}


bool scp11::_calcShSss(const unsigned char*pk, const unsigned char*rk,
                       unsigned char ShSss[SHA1_DIGEST_LENGTH]) {

    if (         0 == oce_rk.size()
        || nullptr == pk
        || nullptr == rk
        ) {
        return false;
    }

    // OCE calculates ShSss from PK.SD.ECKA and SK.OCE.ECKA
    unsigned char session_key[MAX_ADDR_RAW_SIZE] = {0x00,};
    if (0 != ecdh_multiply(curi->params, rk, pk, session_key)) {
        return false;
    }

    SHA1_CTX sha1;
    sha1_Init(&sha1);
    sha1_Update(&sha1, session_key + 1, SHA256_DIGEST_LENGTH);
    sha1_Final(&sha1, ShSss);

    DEBUG_LOG("scp11::_calcShSss::ShSss[%d]: %s\n", SHA256_DIGEST_LENGTH, uchar_vector(ShSss, SHA256_DIGEST_LENGTH).getHex().c_str());

    return true;
}


bool scp11::_calcShSes(const unsigned char*pk, const unsigned char*rk,
                       unsigned char ShSes[SHA1_DIGEST_LENGTH]) {

    if (         0 == e_rk.size()
        || nullptr == pk
        || nullptr == rk
        ) {
        return false;
    }

    // OCE calculates ShSes from PK.SD.ECKA and eSK.OCE.ECKASD
    unsigned char session_key[MAX_ADDR_RAW_SIZE] = {0x00,};
    if (0 != ecdh_multiply(curi->params, rk, pk, session_key)) {
        return false;
    }

    SHA1_CTX sha1;
    sha1_Init(&sha1);
    sha1_Update(&sha1, session_key + 1, SHA256_DIGEST_LENGTH);
    sha1_Final(&sha1, ShSes);

    DEBUG_LOG("scp11::_calcShSes::ShSes[%d]: %s\n", SHA1_DIGEST_LENGTH, uchar_vector(ShSes, SHA1_DIGEST_LENGTH).getHex().c_str());

    return true;
}


int kdf2_sha256(unsigned char *z, unsigned int z_len,
                unsigned char *shareinfo, unsigned int shareinfo_len,
                unsigned char *keyout, unsigned int keyout_len)
{
    int offset = 0;
    //first check zlen  and shareinfo_len and keyout_len

    if(   (nullptr == z)         || (0 == z_len)
       || (nullptr == shareinfo) || (0 == shareinfo_len)
       || (nullptr == keyout)    || (0 == keyout_len)
       ) {
        return 0;
    }

    //Judge LSB or MSB
    unsigned char isLSB = 0;
    int counterStart = 0x00000001;
    unsigned char *counterStartP = (unsigned char *)&counterStart;
    unsigned char counterMSB[4] = {0x00,};
    if (counterStartP[3] == 0x01) {
        isLSB = 0;
        counterMSB[0] = ((unsigned char *)counterStartP)[0];
        counterMSB[1] = ((unsigned char *)counterStartP)[1];
        counterMSB[2] = ((unsigned char *)counterStartP)[2];
        counterMSB[3] = ((unsigned char *)counterStartP)[3];
    }
    else {
        isLSB = 1;
        counterMSB[0] = ((unsigned char *)counterStartP)[3];
        counterMSB[1] = ((unsigned char *)counterStartP)[2];
        counterMSB[2] = ((unsigned char *)counterStartP)[1];
        counterMSB[3] = ((unsigned char *)counterStartP)[0];
    }

    unsigned int blocksize = (keyout_len + (SHA256_DIGEST_LENGTH - 1)) / SHA256_DIGEST_LENGTH;
    unsigned char digest[SHA256_DIGEST_LENGTH];

    SHA256_CTX sha256;
    for (unsigned int i = 0; i < blocksize; i++) {
        sha256_Init(&sha256);
        sha256_Update(&sha256, z, z_len);
        sha256_Update(&sha256, counterMSB, 4);
        sha256_Update(&sha256, shareinfo, shareinfo_len);
        sha256_Final(&sha256, digest);

        if (keyout_len > SHA256_DIGEST_LENGTH) {
            memcpy(keyout + offset, digest, SHA256_DIGEST_LENGTH);
            offset += SHA256_DIGEST_LENGTH;
            keyout_len -= SHA256_DIGEST_LENGTH;
        }
        else {
            memcpy(keyout + offset, digest, keyout_len);
            offset += keyout_len;
        }

        counterMSB[3] += 0x01;
        if (counterMSB[3] == 0x00) {
            counterStart += 256;
            if (isLSB == 1) {
                counterMSB[0] = ((unsigned char *)counterStartP)[3];
                counterMSB[1] = ((unsigned char *)counterStartP)[2];
                counterMSB[2] = ((unsigned char *)counterStartP)[1];
                counterMSB[3] = ((unsigned char *)counterStartP)[0];
            }
            else {
                counterMSB[0] = ((unsigned char *)counterStartP)[0];
                counterMSB[1] = ((unsigned char *)counterStartP)[1];
                counterMSB[2] = ((unsigned char *)counterStartP)[2];
                counterMSB[3] = ((unsigned char *)counterStartP)[3];
            }
        }
    }

    return offset;
}
