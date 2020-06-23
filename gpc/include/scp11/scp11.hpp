//
//  scp11.hpp
//  JubSDK
//
//  Created by Pan Min on 2020/6/9.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#ifndef gpc_scp11_hpp
#define gpc_scp11_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include "tag.h"

#include <TrezorCrypto/bip32.h>
#include <TrezorCrypto/curves.h>


enum GPC_TLV_KEY_USAGE {
    CERT_VERIFY = 0x82,     // Digital signature verification (CERT .KA-KLOC.ECDSA)
    CERT_AGGREEMENT = 0x80, // Key agreement (CERT .OCE.ECKA)
};


typedef struct gpc_tlv_buf {

public:
    gpc_tlv_buf() {
        clear();
    }
    gpc_tlv_buf(const int t, const std::vector<unsigned char>& v) :
        tag(t),
        value(v) {}
    gpc_tlv_buf(const std::vector<unsigned char>& v) :
        tag(0x00),
        value(v) {}
    ~gpc_tlv_buf() {
        clear();
    }

    bool operator==(const std::vector<unsigned char>& rhs) const {
        return std::equal(value.begin(), value.end(), rhs.begin());
    }
    bool operator==(const gpc_tlv_buf& rhs) const {
        if (!((*this) == rhs.value)
            ||    tag != rhs.tag
            ) {
            return false;
        }
        else {
            return true;
        }
    }
    bool operator!=(const std::vector<unsigned char>& rhs) const {
        return !((*this) == rhs);
    }
    bool operator!=(const gpc_tlv_buf& rhs) const {
        return !((*this) == rhs);
    }

    gpc_tlv_buf& operator=(const gpc_tlv_buf& rhs) {
        this->tag = rhs.tag;
        this->value.clear();
        this->value.insert(std::end(this->value), std::begin(rhs.value), std::end(rhs.value));
        return *this;
    }

    void clear() {
        tag = 0;
        value.clear();
    }
    bool empty() {
        return value.empty();
    }

    std::vector<unsigned char> encodeV();
    size_t encodeV(std::vector<unsigned char>& v);
    std::vector<unsigned char> encodeLV();
    size_t encodeLV(std::vector<unsigned char>& lv);
    std::vector<unsigned char> encode();
    size_t encode(std::vector<unsigned char>& code);

public:
    int tag;                /**< TLV type, e.g. TLV_UTF8_STRING. */
    std::vector<unsigned char> value;   /**< TLV data, e.g. in ASCII. */
} tlv_buf;


/**
 * SharedInfo.
*/
typedef struct gpc_scp11_sharedInfo {

public:
    gpc_scp11_sharedInfo() {
        clear();
    }
    gpc_scp11_sharedInfo(const std::vector<unsigned char>& keyUsage,
                         const std::vector<unsigned char>& keyType,
                         const std::vector<unsigned char>& keyLength,
                         const std::vector<unsigned char>& hostCardID) {
        key_usage    = tlv_buf(GPC_TLV_SHAREDINFO_KEYUSAGE, keyUsage);
        key_type     = tlv_buf(GPC_TLV_SHAREDINFO_KEYTYPE, keyType);
        key_length   = tlv_buf(GPC_TLV_SHAREDINFO_KEYLENGTH, keyLength);
        host_cardID  = tlv_buf(GPC_TLV_SHAREDINFO_HOSTID, hostCardID);
    }
    gpc_scp11_sharedInfo(const std::vector<unsigned char>& scpIDParam,
                         const std::vector<unsigned char>& keyUsage,
                         const std::vector<unsigned char>& keyType,
                         const std::vector<unsigned char>& keyLength,
                         const std::vector<unsigned char>& hostCardID) {
        scp_id_param = tlv_buf(GPC_TLV_SHAREDINFO_SCP_ID_PARAM, scpIDParam);
        key_usage    = tlv_buf(GPC_TLV_SHAREDINFO_KEYUSAGE, keyUsage);
        key_type     = tlv_buf(GPC_TLV_SHAREDINFO_KEYTYPE, keyType);
        key_length   = tlv_buf(GPC_TLV_SHAREDINFO_KEYLENGTH, keyLength);
        host_cardID  = tlv_buf(GPC_TLV_SHAREDINFO_HOSTID, hostCardID);
    }
    gpc_scp11_sharedInfo(const gpc_scp11_sharedInfo& sharedInfo) {
        *this = sharedInfo;
    }
    ~gpc_scp11_sharedInfo() {
        clear();
    }

    gpc_scp11_sharedInfo& operator=(const gpc_scp11_sharedInfo& rhs) {
        this->scp_id_param = rhs.scp_id_param;
        this->key_usage    = rhs.key_usage;
        this->key_type     = rhs.key_type;
        this->key_length   = rhs.key_length;
        this->host_cardID  = rhs.host_cardID;
        return *this;
    }

    void clear() {
        scp_id_param.clear();
        key_usage.clear();
        key_type.clear();
        key_length.clear();
        host_cardID.clear();
    }
    bool empty() {
        return (scp_id_param.empty()
                || key_usage.empty()
                || key_type.empty()
                || key_length.empty()
                || host_cardID.empty()
        );
    }

    std::vector<unsigned char> encodeV(const std::vector<unsigned char>& ca_klocID);
    size_t encodeV(const std::vector<unsigned char>& ca_klocID, std::vector<unsigned char>& v);
    std::vector<unsigned char> encodeLV();
    size_t encodeLV(std::vector<unsigned char>& lv);
    std::vector<unsigned char> encode();
    size_t encode(std::vector<unsigned char>& code);

private:
    tlv_buf scp_id_param;
    tlv_buf key_usage;
    tlv_buf key_type;
    tlv_buf key_length;
    tlv_buf host_cardID;
} scp11_sharedInfo;


/**
 * Data for mutual MUTUAL AUTHENTICATE Command.
*/
typedef struct gpc_scp11_mutual_auth {

public:
    gpc_scp11_mutual_auth() {
        clear();
    }
    gpc_scp11_mutual_auth(const scp11_sharedInfo* psharedInfo,
                          const std::vector<unsigned char>& ePk,
                          const std::vector<unsigned char>& sdPK={}) {
        std::vector<unsigned char> vSharedInfo = scp11_sharedInfo(*psharedInfo).encode();
        ka = tlv_buf(GPC_TLV_MA_CR, vSharedInfo);
        e_pk = tlv_buf(GPC_TLV_MA_PK, ePk);
       sd_pk = tlv_buf(GPC_TLV_MA_PK,sdPK);
    }
    ~gpc_scp11_mutual_auth() {
        clear();
    }

    void clear() {
        sd_pk.clear();
           ka.clear();
         e_pk.clear();
    }
    bool empty() {
        return ( e_pk.empty()
                || ka.empty()
        );
    }

    std::vector<unsigned char> encode();
    size_t encode(std::vector<unsigned char>& code);

private:
    tlv_buf ka;     // Control Reference Template (Key Agreement)
    tlv_buf e_pk;   // ePK.OCE.ECKA
    tlv_buf sd_pk;  // PK.SD.ECKA
} scp11_mutual_auth;


/**
 * Public key for an TLV certificate.
*/
typedef struct gpc_scp11_pk {

public:
    gpc_scp11_pk() {
        clear();
    }
    ~gpc_scp11_pk() {
        clear();
    }

    void clear() {
        q.clear();
        param.clear();
    }
    bool empty() {
        return (param.empty()
                ||  q.empty()
        );
    }

    /// Decodes the provided buffer into the certificate.
    virtual bool decode(const std::vector<unsigned char>& data);

public:
    tlv_buf q;
    tlv_buf param;
} scp11_pk;


/**
 * Container for an X.509 certificate. The certificate may be chained.
 */
typedef struct gpc_scp11_crt {

public:
    gpc_scp11_crt() {
        next = nullptr;
        clear();
    }
    gpc_scp11_crt(const std::vector<unsigned char>& data) {
        raw.value.assign(data.begin(), data.end());
        decode(data);

        next = nullptr;
    }
    ~gpc_scp11_crt() {
        clear();
    }

    void clear() {
        raw.clear();
        tbs.clear();

        serial.clear();
        cakloc_id.clear();

        valid_from.clear();
        valid_to.clear();

        pk.clear();

        subject_id.clear();

        key_usage = 0;

        sig.clear();

        if (next) {
            delete next; next = nullptr;
        }
    }
    bool empty() {
        return (   raw.empty()
                || tbs.empty()
                || serial.empty()
                || cakloc_id.empty()
                || valid_from.empty()
                || valid_to.empty()
                || pk.empty()
                || subject_id.empty()
                || sig.empty()
        );
    }

    /// Decodes the provided buffer into the certificate.
    virtual bool decode(const std::vector<unsigned char>& data);
    virtual bool decode();

public:
    tlv_buf raw;                    /**< The raw certificate data (TLV). */
    tlv_buf tbs;                    /**< The raw certificate body (TLV). The part that is To Be Signed. */

    tlv_buf serial;                 /**< Unique id for certificate issued by a specific CA. */
    tlv_buf cakloc_id;

    tlv_buf valid_from;             /**< Start time of certificate validity. */
    tlv_buf valid_to;               /**< End time of certificate validity. */

    scp11_pk pk;                    /**< Container for the public key context. */

    tlv_buf subject_id;             /**< Optional TLV certificate subject unique identifier. */

    unsigned int key_usage;         /**< Optional key usage extension value: See the values in scp11_pk.hpp */
    tlv_buf sig;                    /**< Signature: hash of the tbs part signed with the private key. */
    struct gpc_scp11_crt *next;     /**< Next certificate in the CA-chain. */
} scp11_crt;


typedef enum GPC_SCP11_SESSION {
    KEY_DEK = 0,
    S_ENC,
    S_MAC,
    S_RMAC,
    S_DEK
} ENUM_SCP11_SESSION;


typedef struct gpc_scp11_session_key {

public:
    gpc_scp11_session_key() {
        clear();
    }
    gpc_scp11_session_key(const std::vector<unsigned char>& raw)
        : raw (0, raw) {
        clear();
    }
    ~gpc_scp11_session_key() {
        clear();
    }

    void clear() {
        key_dek.clear();
        s_enc.clear();
        s_mac.clear();
        s_rmac.clear();
        s_dek.clear();
    }
    bool empty() {
        return (   raw.empty()
                && (  key_dek.empty()
                    ||  s_enc.empty()
                    ||  s_mac.empty()
                    || s_rmac.empty()
                    ||  s_dek.empty())
        );
    }

    static int size() {
        return (KEY_LENGTH*N);
    }
    /// Decodes the provided buffer into the scp11_session_key.
    virtual bool decode(const std::vector<unsigned char>& data,
                        const int key_length = KEY_LENGTH);
    virtual bool decode();
    virtual std::vector<unsigned char> encode();

private:
    tlv_buf raw;

public:
    static int N;
    static int KEY_LENGTH;

    std::vector<unsigned char> key_dek; // Data Encryption Key
    std::vector<unsigned char> s_enc;   // Secure Channel session key for command and response encryption
    std::vector<unsigned char> s_mac;   // Message Authentication Code
    std::vector<unsigned char> s_rmac;  // Response MAC
    std::vector<unsigned char> s_dek;   // Session Data Encryption Key
} scp11_session_key;


typedef struct gpc_scp11_response_msg {

public:
    gpc_scp11_response_msg() {
        clear();
    }
    ~gpc_scp11_response_msg() {
        clear();
    }

    void clear() {
        sd_pk.clear();
        receipt.clear();
    }
    bool empty() {
        return ( receipt.empty()
                || sd_pk.empty()
        );
    }

    bool decode(const std::vector<unsigned char>& data);

public:
    tlv_buf sd_pk;          // SCP11c:  PK.SD.ECKA
                            // SCP11a: ePK.SD.ECKA
    tlv_buf receipt;        // Receipt
} scp11_response_msg;


typedef struct gpc_scp11 {

public:
    gpc_scp11() {
        clear();
        curi = (curve_info *)get_curve_by_name(NIST256P1_NAME);
    }
    gpc_scp11(const scp11_sharedInfo& info,
              const scp11_crt& crt,
              const std::vector<unsigned char>& rk) {
        clear();
        oce_crt = crt;
        oce_rk  = rk;
        shared_info = info;
        curi = (curve_info *)get_curve_by_name(NIST256P1_NAME);
    }
    ~gpc_scp11() {
        clear();
    }

public:
    void clear() {
        oce_crt.clear();
        oce_rk.clear();
        e_pk.clear();
        e_rk.clear();
        receipt_key.clear();
        sk.clear();
    }
    bool empty() {
        return (shared_info.empty()
                ||  oce_crt.empty()
                ||   oce_rk.empty()
        );
    }
    bool isValid() {
        return !empty();
    }

    bool initialize();
    void setResponseMsg(const scp11_response_msg& msg) {
        receipt_key = msg;
    }
    virtual std::vector<unsigned char> getOCECert() {
        return oce_crt.raw.value;
    }
    virtual std::vector<unsigned char> getMacChain() {
        return receipt_key.receipt.value;
    }
    virtual std::vector<unsigned char> getMutualAuthData() = 0;
    virtual bool openSecureChannel(const std::vector<unsigned char>& response) = 0;

protected:
    virtual bool keyDerivation() = 0;
    virtual bool  checkReceipt() = 0;
    bool _calcShSss(const unsigned char*pk, const unsigned char*rk,
                    unsigned char ShSss[SHA1_DIGEST_LENGTH]);
    bool _calcShSes(const unsigned char*pk, const unsigned char*rk,
                    unsigned char ShSes[SHA1_DIGEST_LENGTH]);

public:
    scp11_session_key sk;

protected:
    curve_info *curi;
    scp11_sharedInfo shared_info;

    scp11_crt oce_crt;
    std::vector<unsigned char> oce_rk;

    std::vector<unsigned char> e_pk;
    std::vector<unsigned char> e_rk;

    scp11_response_msg receipt_key;
} scp11;


int kdf2_sha256(unsigned char *z, unsigned int z_len,
                unsigned char *shareinfo, unsigned int shareinfo_len,
                unsigned char *keyout, unsigned int keyout_len);


#endif /* scp11_hpp */
