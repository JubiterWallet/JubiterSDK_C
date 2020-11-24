// JUB_SDK_test.cpp : Defines the entry point for the console application.
//

#ifdef _WIN32
#include "stdafx.h"
#endif

#include "JUB_SDK_test.h"
#include "JUB_SDK_test_dev.hpp"
#include "JUB_SDK_test_btc.hpp"
#include "JUB_SDK_test_hcash.hpp"
#include "JUB_SDK_test_qtum.hpp"
#include "JUB_SDK_test_eth.hpp"
#include "JUB_SDK_test_eos.hpp"
#include "JUB_SDK_test_xrp.hpp"
#include "../../include/JUB_CORE.h"

#include "JUB_SDK_main.h"

#include <TrezorCrypto/rand.h>
#include <TrezorCrypto/bip32.h>
#include <TrezorCrypto/ecdsa.h>
#include <TrezorCrypto/curves.h>
#include <TrezorCrypto/sha2.h>
#include <TrezorCrypto/aes.h>
#include "scp11/scp11c.hpp"

#include "mSIGNA/stdutils/uchar_vector.h"

using std::getline;
using std::istringstream;


//OCE 证书-公钥-私钥
//LINE14 传给卡片，用 LINE10 验证
//LINE32，临时公钥对，曲线参数在 LINE21


void getSharedInfo(unsigned int keyUsage,
                   uchar_vector keyType,
                   unsigned int keyLength,
                   uchar_vector hostID,
                   uchar_vector& sharedInfo) {
    sharedInfo.push_back(1);
    sharedInfo << keyUsage;
    sharedInfo.push_back(keyType.size());
    sharedInfo << keyType;
    sharedInfo.push_back(1);
    sharedInfo << keyLength;
    sharedInfo.push_back(hostID.size());
    sharedInfo << hostID;
}


void scp11_test(const char* json_file) {

    Json::Value root = readJSON(json_file);

    //7f2181d8931042584e46433230303532353030303031420d6a75626974657277616c6c65745f200d6a75626974657277616c6c65749501825f2504202005255f24042025052453007f4946b0410479704bdb2d3da2e547eb6de66e0073f6e61ae32076af007973b5fa1dbe07e0ef38bd84d85f1fe1e1410ff743e659691b36361c76bee2fac44fd88825759268cef001005f37483046022100b076674c9f0ea1ddee84517e2a53cb392ac2c8b25ca3a7d56558570a051737020221008a982e267ffcef5309a272ea492be489a233381c477e8803034a8f6789f2bbd9
    //7f21 81d8
    //93   10 42584e46433230303532353030303031
    //42   0d 6a75626974657277616c6c6574
    //5f20 0d 6a75626974657277616c6c6574
    //95   01 82
    //5f25 04 20200525
    //5f24 04 20250524
    //53   00
    //7f49 46
    //        b0 41
    //        0479704bdb2d3da2e547eb6de66e0073f6e61ae32076af007973b5fa1dbe07e0ef38bd84d85f1fe1e1410ff743e659691b36361c76bee2fac44fd88825759268ce
    //        f0 01 00
    //5f37 48 3046022100b076674c9f0ea1ddee84517e2a53cb392ac2c8b25ca3a7d56558570a051737020221008a982e267ffcef5309a272ea492be489a233381c477e8803034a8f6789f2bbd9

    //7F21 81E8
    //93   10 434552545F4150505F45434B41303031 - Certificate Serial Number(18)
    //42   0D 6A75626974657277616C6C6574   - CA-KLOC (or KA-KLOC) Identifier(15)
    //5F20 0D 6A75626974657277616C6C6574   - Subject Identifier(16)
    //95   02 0080                         - Key Usage(4) Key agreement
    //5F25 04 20200525                     - Effective Date (YYYYMMDD, O BCD format)(7)
    //5F24 04 20250524                     - Expiration Date (YYYYMMDD, BCD M format)(7)
    //53   00                              - Discretionary Data(2)
    //BF20 0E EF0C8D0A82018202820382048205 - Restrictions under SCP11c(17)
    //7F49 46                              - Public Key(+3)
    //        B0 41 048FD3FAB3907C5CC8CD193EB2B653EA179115B7F305C9E21DE6D29C0736A3B82025B219F24BDA86D80F5AE262521E124F4C6691A0C47B1FB72D95895E9312CB0D           - public_key_q(67)
    //        F0 01 00                       - key_parameter_reference(3)
    //5F37 46 304402204D75EAA2F09604A9597DA905D680EB619B8ADCF080E5AD6950E1DBF26195C9E2022067649AFB4A8BC380B382520499C6F2BB350A8519B0ECDBE0B7374AA898826D0E(73)
    char* p = (char*)root["SCP11c"]["OCE"][0][0].asCString();
    uchar_vector vOCECert(p);
    scp11_crt oce_crt(vOCECert);
    if (!oce_crt.decode()) {
        std::cout << "scp11_crt::decode error!" << std::endl;
    }
    uint8_t sd_pub_key[65] = {0x00,};
    std::copy(oce_crt.pk.q.value.begin(), oce_crt.pk.q.value.end(), std::begin(sd_pub_key));

    std::cout << "crt.tbs["         << oce_crt.tbs.value.size() << "]: " << uchar_vector(oce_crt.tbs.value).getHex() << std::endl;
    std::cout << "crt.serial["      << oce_crt.serial.value.size() << "]: " << uchar_vector(oce_crt.serial.value).getCharsAsString() << std::endl;
    std::cout << "crt.cakloc_id ["  << oce_crt.cakloc_id.value.size() << "]:  " << uchar_vector(oce_crt.cakloc_id.value).getCharsAsString() << std::endl;
    std::cout << "crt.subject_id["  << oce_crt.subject_id.value.size() << "]: " << uchar_vector(oce_crt.subject_id.value).getCharsAsString() << std::endl;
    std::cout << "crt.valid_from["  << oce_crt.valid_from.value.size() << "]: " << uchar_vector(oce_crt.valid_from.value).getHex() << std::endl;
    std::cout << "crt.valid_to  ["  << oce_crt.valid_to.value.size() << "]: " << uchar_vector(oce_crt.valid_to.value).getHex() << std::endl;
    std::cout << "crt.key_usage: "  << oce_crt.key_usage << std::endl;
    std::cout << "crt.pk.q["        << oce_crt.pk.q.value.size() << "]: " << uchar_vector(oce_crt.pk.q.value).getHex() << std::endl;
    std::cout << "crt.pk.param["    << oce_crt.pk.param.value.size() << "]: " << uchar_vector(oce_crt.pk.param.value).getHex() << std::endl;
    std::cout << "crt.sig["         << oce_crt.sig.value.size() << "]: " << uchar_vector(oce_crt.sig.value).getHex() << std::endl;

    int rv = -1;

    const curve_info *curi = get_curve_by_name(NIST256P1_NAME);//verify sign
    if(nullptr == curi) {
        return ;
    }

    // OCE key pair(SK.OCE.ECKA, PK.OCE.ECKA)
    unsigned char oce_priv_key[32] = {0x00,};
    uchar_vector privKey((char*)root["SCP11c"]["OCE"][1][2].asCString());
    std::copy(privKey.begin(), privKey.end(), std::begin(oce_priv_key));
    unsigned char oce_pub_key[65] = {0x00,};
    uchar_vector pubKey((char*)root["SCP11c"]["OCE"][1][1].asCString());
    std::copy(pubKey.begin(), pubKey.end(), std::begin(oce_pub_key));
//    ecdsa_get_public_key65(curi->params, oce_priv_key, oce_pub_key);

    // OCE generates ephemeral key pair(eSK.OCE.ECKA, ePK.OCE.ECKA)
    unsigned char oce_e_priv_key[32] = {0x00,};
    uchar_vector eprivKey((char*)root["SCP11c"]["eKeyPair"][1].asCString());
    std::copy(eprivKey.begin(), eprivKey.end(), std::begin(oce_e_priv_key));
//    random_buffer(oce_e_priv_key, sizeof(oce_e_priv_key)/sizeof(uint8_t));
    unsigned char oce_e_pub_key[65] = {0x00,};
    uchar_vector epubKey((char*)root["SCP11c"]["eKeyPair"][0].asCString());
    std::copy(epubKey.begin(), epubKey.end(), std::begin(oce_e_pub_key));
//    ecdsa_get_public_key65(curi->params, oce_e_priv_key, oce_e_pub_key);

    // OCE calculates ShSss from PK.SD.ECKA and SK.OCE.ECKA
    SHA1_CTX sha1;
    unsigned char session_key[65] = {0x00,};
    rv = ecdh_multiply(curi->params, oce_priv_key, sd_pub_key, session_key);
    if (JUBR_OK != rv) {
        return ;
    }
    std::cout << "session_key: " << uchar_vector(session_key, sizeof(session_key)/sizeof(uint8_t)).getHex() << std::endl;
    uint8_t ShSss[20] = {0,};
    unsigned int ShSssLen = sizeof(ShSss)/sizeof(uint8_t);
    sha1_Init(&sha1);
    sha1_Update(&sha1, session_key + 1, 32);
    sha1_Final(&sha1, ShSss);
    std::cout << "ShSss: " << uchar_vector(ShSss, ShSssLen).getHex() << std::endl;

    // OCE calculates ShSes from PK.SD.ECKA and eSK.OCE.ECKASD
    memset(session_key, 0x00, sizeof(session_key)/sizeof(uint8_t));
    rv = ecdh_multiply(curi->params, oce_e_priv_key, sd_pub_key, session_key);
    if (JUBR_OK != rv) {
        return ;
    }
    std::cout << "session_key: " << uchar_vector(session_key, sizeof(session_key)/sizeof(uint8_t)).getHex() << std::endl;
    uint8_t ShSes[20] = {0,};
    unsigned int ShSesLen = sizeof(ShSes)/sizeof(uint8_t);
    sha1_Init(&sha1);
    sha1_Update(&sha1, session_key + 1, 32);
    sha1_Final(&sha1, ShSes);
    std::cout << "ShSes: " << uchar_vector(ShSes, ShSesLen).getHex() << std::endl;

    // OCE derives AES sessionkeys from ShSes and ShSss
    uint8_t z[40] = {0x00,};
    unsigned int zLen = sizeof(z)/sizeof(uint8_t);
    memcpy(z, ShSes, ShSesLen);
    memcpy(z + ShSesLen, ShSss, ShSssLen);
    // sharedInfo, keyUsage = 0x88. 以 LV 形式组织。
    uchar_vector keyUsage;
    keyUsage.push_back(0x3C);
    uchar_vector keyType;
    keyType.push_back(0x88);
    uchar_vector keyLength;
    keyLength.push_back(root["SCP11c"]["KeyLength"].asUInt());
    uchar_vector hostID((char*)root["SCP11c"]["HostID"].asCString());
    scp11_sharedInfo shInfo(keyUsage,
                            keyType,
                            keyLength,
                            hostID);
    uchar_vector sharedInfo = shInfo.encodeLV();
    std::cout << "sharedInfo: " << sharedInfo.getHex() << std::endl;
    unsigned int sharedInfoLen = (unsigned int)sharedInfo.size();
    unsigned char* pSharedInfo = new unsigned char[sharedInfoLen+1];
    memset(pSharedInfo, 0x00, sharedInfoLen+1);
    std::copy(sharedInfo.begin(), sharedInfo.end(), pSharedInfo);
    unsigned char outKey[0x10*5] = {0x00,};
    uint32_t outKeyLen = sizeof(outKey)/sizeof(unsigned char);
    outKeyLen = kdf2_sha256(z, zLen,
                            pSharedInfo, sharedInfoLen,
                            outKey, outKeyLen);
    delete [] pSharedInfo; pSharedInfo = nullptr;
    std::cout << "AES sessionkeys[" << outKeyLen <<  "]: " << uchar_vector(outKey, outKeyLen).getHex() << std::endl;
    scp11_session_key sk(uchar_vector(outKey, outKeyLen));
    if (!sk.decode()) {
        return ;
    }

    // OCE prepares MUTUAL AUTHENTICATE prepare payload

    // OCE wraps APDUs into SCP03 using AES session keys("payload")
    // OCE prepares script and resources for APK file
    // OCE transmits script to REA via AppStore
}
