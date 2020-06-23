#include "device/ApduBuilder.hpp"
#include "scp03/scp03.hpp"
#include "scp11/scp11c.hpp"
#include "util.h"

namespace jub {


JUB_RV JubApduBuiler::BuildApdu(const APDU *apdu,
                                std::vector<JUB_BYTE> &vSafeApdu) {
    // copy header
    vSafeApdu.push_back((JUB_BYTE)apdu->cla);
    vSafeApdu.push_back((JUB_BYTE)apdu->ins);
    vSafeApdu.push_back((JUB_BYTE)apdu->p1);
    vSafeApdu.push_back((JUB_BYTE)apdu->p2);

    if (0 != apdu->lc) {
        // get lc,data
        if (0xFF < apdu->lc) {
            vSafeApdu.push_back((JUB_BYTE)( (apdu->lc)/0x10000));
            vSafeApdu.push_back((JUB_BYTE)(((apdu->lc)/0x100)%0x100));
            vSafeApdu.push_back((JUB_BYTE)( (apdu->lc)%0x100));
            vSafeApdu.insert(vSafeApdu.end(), apdu->data.begin(),
                             apdu->data.begin() + apdu->lc);
        }
        else {
            vSafeApdu.push_back((JUB_BYTE)(apdu->lc));
            vSafeApdu.insert(vSafeApdu.end(), apdu->data.begin(),
                             apdu->data.begin() + apdu->lc);
        }
    }

    if (0 != apdu->le) {
        if (0xFF < apdu->le) {
            vSafeApdu.push_back((JUB_BYTE)( (apdu->le)/0x10000));
            vSafeApdu.push_back((JUB_BYTE)(((apdu->le)/0x100)%0x100));
            vSafeApdu.push_back((JUB_BYTE)( (apdu->le)%0x100));
        }
        else {
            vSafeApdu.push_back((JUB_BYTE)(apdu->le));
        }
    }

    return JUBR_OK;
}


JUB_RV JubApduBuiler::SetSCP03(void *scp03Ptr) {
    _scp03Ptr = scp03Ptr;
    return JUBR_OK;
}


JUB_RV JubApduBuiler::SetSCP11(void *scp11Ptr) {
    auto ptr = (scp11*)scp11Ptr;
    if (ptr->empty()) {
        return JUBR_ARGUMENTS_BAD;
    }

    _scp11Ptr = scp11Ptr;
    return JUBR_OK;
}


JUB_RV JubApduBuiler::PackData(std::vector<JUB_BYTE> &vDest,
                               const std::vector<JUB_BYTE> &vSrc) {

    if (   nullptr == _scp03Ptr
        && nullptr == _scp11Ptr
        ) {
        vDest = vSrc;
    }

    auto scp03Ptr = (scp03*)_scp03Ptr;
    auto scp11Ptr = (scp11*)_scp11Ptr;

    std::vector<unsigned char> encKey = scp11Ptr->sk.s_enc;
    // initial chaining vector
    unsigned char icv[16] = {0x00,};
    int icvLen = sizeof(icv)/sizeof(unsigned char);
    if (!scp03Ptr->icv(encKey.data(), (int)encKey.size(),
                       icv, &icvLen)) {
        return JUBR_ERROR;
    }

    // encrypt apdu
    unsigned char *enc = nullptr;
    int encLen = 0;
    if (!scp03Ptr->encrypt(encKey.data(), (int)encKey.size(),
                           icv, icvLen,
                           vSrc.data(), (int)vSrc.size(),
                           enc, &encLen)) {
        return JUBR_ERROR;
    }

    enc = new unsigned char[encLen+1];
    memset(enc, 0x00, encLen+1);
    if (!scp03Ptr->encrypt(encKey.data(), (int)encKey.size(),
                           icv, icvLen,
                           vSrc.data(), (int)vSrc.size(),
                           enc, &encLen)) {
        delete [] enc; enc = nullptr;
        return JUBR_ERROR;
    }

    vDest.insert(vDest.end(), enc, enc+encLen);
    delete [] enc; enc = nullptr;

    return JUBR_OK;
}


JUB_RV JubApduBuiler::UnPackData(std::vector<JUB_BYTE> &vDest,
                                 const std::vector<JUB_BYTE> &vSrc) {

    if (   nullptr == _scp03Ptr
        && nullptr == _scp11Ptr
        ) {
        vDest = vSrc;
    }

    auto scp03Ptr = (scp03*)_scp03Ptr;
    auto scp11Ptr = (scp11*)_scp11Ptr;

    // initial chaining vector
    unsigned char icv[16] = {0x00,};
    int icvLen = sizeof(icv)/sizeof(unsigned char);
    if (!scp03Ptr->icv(scp11Ptr->sk.s_enc.data(), (int)scp11Ptr->sk.s_enc.size(),
                       icv, &icvLen, false)) {
        return JUBR_ERROR;
    }

    // decrypt apdu
    unsigned char *dec = nullptr;
    int decLen = 0;
    if (!scp03Ptr->decrypt(scp11Ptr->sk.s_enc.data(), (int)scp11Ptr->sk.s_enc.size(),
                           icv, icvLen,
                           vSrc.data(), (int)vSrc.size(),
                           dec, &decLen)) {
        return JUBR_ERROR;
    }

    dec = new unsigned char[decLen+1];
    memset(dec, 0x00, decLen+1);
    if (!scp03Ptr->decrypt(scp11Ptr->sk.s_enc.data(), (int)scp11Ptr->sk.s_enc.size(),
                           icv, icvLen,
                           vSrc.data(), (int)vSrc.size(),
                           dec, &decLen)) {
        delete [] dec; dec = nullptr;
        return JUBR_ERROR;
    }

    vDest.insert(vDest.end(), dec, dec+decLen);
    delete [] dec; dec = nullptr;

    return JUBR_OK;
}


JUB_RV JubApduBuiler::PlusCMAC(const APDU *apdu, APDU *apduSafe) {

    if (   nullptr == _scp03Ptr
        && nullptr == _scp11Ptr
        ) {
        *apduSafe = *apdu;
    }

    auto scp03Ptr = (scp03*)_scp03Ptr;
    auto scp11Ptr = (scp11*)_scp11Ptr;

    // calculate CMAC
    std::vector<JUB_BYTE> vEnc;
    vEnc.insert(vEnc.end(), apdu->data.begin(), apdu->data.end());

    std::vector<unsigned char> macChain = scp03Ptr->getMacChain();
    APDU safeApdu(apdu->cla, apdu->ins,
                  apdu->p1, apdu->p2, apdu->lc+8);
    safeApdu.cla |= 0x04;
    std::vector<JUB_BYTE> calMAC;
    calMAC.insert(calMAC.end(), macChain.begin(), macChain.end());    // mac_chaining
    calMAC.push_back(safeApdu.cla);
    calMAC.push_back(safeApdu.ins);
    calMAC.push_back(safeApdu.p1);
    calMAC.push_back(safeApdu.p2);
    calMAC.push_back(safeApdu.lc);
    calMAC.insert(calMAC.end(), vEnc.begin(), vEnc.end());

    unsigned char outmac[16] = {0x00,};
    int outmacLen = sizeof(outmac)/sizeof(unsigned char);
    if (!scp03Ptr->cmac(scp11Ptr->sk.s_mac.data(), (int)scp11Ptr->sk.s_mac.size(),
                        calMAC.data(), (int)calMAC.size(),
                        outmac, &outmacLen)) {
        return JUBR_ERROR;
    }

    vEnc.insert(vEnc.end(), std::begin(outmac), std::begin(outmac)+8);
    safeApdu.SetData(vEnc.data(), vEnc.size());

    *apduSafe = safeApdu;

    // reset mac chain
    macChain.clear();
    macChain.insert(macChain.end(), std::begin(outmac), std::end(outmac));
    scp03Ptr->macChaining(macChain);

    return JUBR_OK;
}


JUB_RV JubApduBuiler::CheckCMAC(const ResponseAPDU &respApdu) {

    if (   nullptr == _scp03Ptr
        && nullptr == _scp11Ptr
        ) {
        return JUBR_OK;
    }

    auto scp03Ptr = (scp03*)_scp03Ptr;
    auto scp11Ptr = (scp11*)_scp11Ptr;


    std::vector<unsigned char> macChain = scp03Ptr->getMacChain();

    std::vector<JUB_BYTE> calMAC;
    calMAC.insert(calMAC.end(), macChain.begin(), macChain.end());    // mac_chaining
//    calMAC.insert(calMAC.end(), resp.begin(), resp.begin()+(resp.size()-8));
//    calMAC.push_back(0x63);
//    calMAC.push_back(0xc7);
    calMAC.insert(calMAC.end(), respApdu.data.begin(), respApdu.data.end());
    calMAC.push_back(respApdu.SW1);
    calMAC.push_back(respApdu.SW2);

    unsigned char outmac[16] = {0x00,};
    int outmacLen = sizeof(outmac)/sizeof(unsigned char);
    if (!scp03Ptr->cmac(scp11Ptr->sk.s_rmac.data(), (int)scp11Ptr->sk.s_rmac.size(),
                        calMAC.data(), (int)calMAC.size(),
                        outmac, &outmacLen)) {
        return JUBR_ERROR;
    }

    macChain.clear();
    macChain.insert(macChain.end(), respApdu.cmac.begin(), respApdu.cmac.end());

    return (std::equal(std::begin(outmac), std::begin(outmac)+respApdu.CMAC_LEN, std::begin(macChain)) ? JUBR_OK : JUBR_ERROR);
}


JUB_RV JubApduBuiler::BuildSafeApdu(const APDU *apdu, std::vector<JUB_BYTE> &vSafeApdu) {

    std::vector<JUB_BYTE> apduData;
    apduData.push_back(apdu->data.size());
    apduData.insert(apduData.end(), apdu->data.begin(), apdu->data.end());
    std::cout << "BuildSafeApdu::apduData[" << apduData.size() << "]: " << uchar_vector(apduData).getHex() << std::endl;

    // encrypt APDU
    std::vector<JUB_BYTE> vEnc;
    JUB_VERIFY_RV(PackData(vEnc, apduData));

    APDU encApdu;
    encApdu.SetApdu(apdu->cla, apdu->ins, apdu->p1, apdu->p2, vEnc.size(), vEnc.data());
    APDU safeApdu;
    JUB_VERIFY_RV(PlusCMAC(&encApdu, &safeApdu));

    return BuildApdu(&safeApdu, vSafeApdu);
}


JUB_RV JubApduBuiler::ParseSafeApduResp(const JUB_BYTE_PTR respData, const JUB_ULONG respDataLen,
                                        JUB_BYTE_PTR retData, JUB_ULONG_PTR pretDataLen,
                                        JUB_UINT16 &wRet) {

    ResponseAPDU respApdu(respData, respDataLen);
    if (!respApdu.decode()) {
        return JUBR_ARGUMENTS_BAD;
    }

    // check cmac
    JUB_VERIFY_RV(CheckCMAC(respApdu));

    // decrypt
    std::vector<JUB_BYTE> vRecv;
    vRecv.insert(vRecv.end(), respApdu.data.begin(), respApdu.data.end());
    std::vector<JUB_BYTE> vDec;
    JUB_VERIFY_RV(UnPackData(vDec, vRecv));

    if ((*pretDataLen) < vRecv.size()) {
        return JUBR_BUFFER_TOO_SMALL;
    }

    *pretDataLen = vDec.size();
    std::copy(vDec.begin(), vDec.end(), retData);

    wRet = respApdu.SW1 * 0x100 + respApdu.SW2;

    return JUBR_OK;
}


}  // namespace jub end
