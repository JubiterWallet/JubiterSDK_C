#include "token/CKB/JubiterBladeCKBImpl.h"
#include "BinaryCoding.h"
#include "JUB_SDK_BTC.h"
#include "JUB_SDK_COMM.h"
#include "NervosCKB/SegwitAddress.h"
#include "mSIGNA/stdutils/uchar_vector.h"
#include "token/ErrorHandler.h"
#include "utility/util.h"
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

namespace jub {
namespace token {
JUB_RV JubiterBladeCKBImpl::SelectApplet() { return JubiterBladeBTCImpl::SelectApplet(); }

JUB_RV JubiterBladeCKBImpl::SetCoin(const JUB_ENUM_COINTYPE_BTC &type) { return JubiterBladeBTCImpl::SetCoin(type); }

JUB_RV JubiterBladeCKBImpl::SetUnit(const JUB_ENUM_BTC_UNIT_TYPE &unit) { return JUBR_OK; }

JUB_RV JubiterBladeCKBImpl::GetAddress(const JUB_BYTE addrFmt, const JUB_ENUM_BTC_TRANS_TYPE &type,
                                       const std::string &path, const JUB_UINT16 tag, std::string &address,
                                       const TWCoinType &coinNet) {

    JUB_BYTE p1 = (JUB_BYTE)tag;
    if (_isSupportLegacyAddress()) {
        p1 |= _RealAddressFormat(addrFmt);
    }
    JUB_BYTE sigType;
    switch (type) {
    case p2pkh: {
        sigType = TW::NervosCKB::TWNervosCKBAddressFormat::TWNervosCKBAddressShort;
        break;
    } // case p2pkh end
    case p2sh_p2wpkh: {
        sigType = TW::NervosCKB::TWNervosCKBAddressFormat::TWNervosCKBAddressShort;
        break;
    } // case p2sh_p2wpkh end
    default:
        return JUBR_IMPL_NOT_SUPPORT;
    } // switch (type) end

    uchar_vector vPath;
    vPath << path;
    uchar_vector apduData = ToTlv(JUB_ENUM_APDU_DATA::TAG_PATH_08, vPath);

    TW::Data vAddress;
    JUB_VERIFY_RV(JubiterBladeToken::GetAddress(p1, sigType, apduData, vAddress));

    address = std::string(vAddress.begin(), vAddress.end());

    return JUBR_OK;
}

JUB_RV JubiterBladeCKBImpl::GetHDNode(const JUB_ENUM_BTC_TRANS_TYPE &type, const std::string &path, std::string &xpub,
                                      const TWCoinType &coinNet) {

    JUB_BYTE p2 = 0x00;
    switch (type) {
    case p2pkh: {
        p2 = 0x00; // xpub  format
        break;
    } // case p2pkh end
    case p2sh_p2wpkh: {
        p2 = 0x01; // ypub  format
        break;
    } // case p2sh_p2wpkh end
    default:
        return JUBR_ARGUMENTS_BAD;
    } // switch (type) end

    // path = "m/44'/X'/0'";
    uchar_vector vPubkey;
    JUB_VERIFY_RV(JubiterBladeToken::GetHDNode(0x00, p2, path, vPubkey));

    xpub = std::string(vPubkey.begin(), vPubkey.end());

    return JUBR_OK;
}

JUB_RV JubiterBladeCKBImpl::SignTX(const JUB_ENUM_BTC_TRANS_TYPE &type, const std::vector<std::string> &vInputPath,
                                   const JUB_UINT32 version, const std::vector<CELL_DEP> &vDeps,
                                   const std::vector<CELL_INPUT> &vInputs, const std::vector<CELL_OUTPUT> &vOutputs,
                                   std::vector<uchar_vector> &vSignatureRaw, const TWCoinType &coinNet) {
    try {
        auto inputCount = vInputs.size();
        constexpr JUB_UINT32 kSendOnceLen = 230;
        uchar_vector apduData;
        // number of inputs/paths
        apduData << (JUB_BYTE)(inputCount);

        // input amounts
        uchar_vector amountTLV;
        for (const auto &input : vInputs) {
            auto amount = input.capacity;
            amountTLV << (uint64_t)amount;
        }
        apduData << ToTlv(0x0e, amountTLV);

        //  first pack
        JUB_BYTE p1 = 0x01;
        JUB_BYTE sigType = 0x00;

        APDU apdu(0x00, 0xF8, p1, sigType, (JUB_ULONG)apduData.size(), apduData.data());
        JUB_UINT16 ret = 0;
        JUB_VERIFY_RV(_SendApdu(&apdu, ret));
        JUB_VERIFY_COS_ERROR(ret);
        apduData.clear();

        // pathTLV
        uchar_vector pathLV;
        for (auto path : vInputPath) {
            pathLV << (JUB_BYTE)(path.size());
            pathLV << path;
        }
        apduData << ToTlv(0x0F, pathLV);
        JUB_VERIFY_RV(_TranPack(apduData, 0x00, sigType, kSendOnceLen));
        apduData.clear();

        // tx TLV
        TW::NervosCKB::Transaction tx;
        JubiterBaseCKBImpl::_unsignedTx(0, 0, vDeps, vInputs, vOutputs, tx);
        Data serialzedTx;
        tx.serialize(serialzedTx);
        std::cout << "serialized tx: " << uchar_vector(serialzedTx).getHex() << std::endl;
        apduData << ToTlv(0x0D, serialzedTx);
        JUB_VERIFY_RV(_TranPack(apduData, 0x00, sigType, kSendOnceLen));
        apduData.clear();

        uchar_vector changeIndexTLV;
        changeIndexTLV = ToTlv(0x10, {}); // not use
        apduData << changeIndexTLV;

        JUB_VERIFY_RV(_TranPack(apduData, 0x00, sigType, kSendOnceLen,
                                true)); // last data.
        apduData.clear();

        //  sign transactions
        JUB_BYTE retData[2] = {
            0,
        };
        JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
        apdu.SetApdu(0x00, JUB_ENUM_APDU_CMD::INS_SIGN_TX_2A, 0x00, sigType, 0);
        JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
        if (0x6f09 == ret) {
            return JUBR_USER_CANCEL;
        }
        JUB_VERIFY_COS_ERROR(ret);

        // get transactions (pack by pack)
        if (2 != ulRetDataLen) { // total length
            return JUBR_TRANSMIT_DEVICE_ERROR;
        }

        JUB_UINT16 totalReadLen = TW::decode16BE(retData);
        TW::Data sigRawTx(totalReadLen, 0x00);

        constexpr JUB_UINT16 kReadOnceLen = 256;
        JUB_ULONG ulRetLen = kReadOnceLen;

        apdu.SetApdu(0x00, 0xF9, 0x00, 0x00, 0x00);
        apdu.le = kReadOnceLen;
        JUB_UINT16 times = 0;
        for (times = 0; times < (totalReadLen / kReadOnceLen); times++) {

            JUB_UINT16 offset = times * kReadOnceLen;
            apdu.p1 = offset >> 8;
            apdu.p2 = offset & 0x00ff;

            JUB_VERIFY_RV(_SendApdu(&apdu, ret, sigRawTx.data() + times * kReadOnceLen, &ulRetLen));
            JUB_VERIFY_COS_ERROR(ret);
        }

        apdu.le = totalReadLen % kReadOnceLen;
        if (apdu.le) {
            JUB_UINT16 offset = times * kReadOnceLen;
            apdu.p1 = offset >> 8;
            apdu.p2 = offset & 0x00ff;

            ulRetLen = totalReadLen - times * kReadOnceLen;

            JUB_VERIFY_RV(_SendApdu(&apdu, ret, sigRawTx.data() + times * kReadOnceLen, &ulRetLen));
            JUB_VERIFY_COS_ERROR(ret);
        }
        vSignatureRaw.clear();
        // count || witness || witness || ...
        auto count = sigRawTx.front(); // not bigger than 0xff
        assert(count == inputCount);
        auto p = sigRawTx.data();
        p += 4;

        while (count-- > 0) {
            auto total = decode32LE(p);
            uchar_vector witness = uchar_vector(p, p + total);
            p += total;
            vSignatureRaw.push_back(witness);
        }

        return JUBR_OK;

    } catch (...) {
        return JUBR_ERROR_ARGS;
    }
}

JUB_RV JubiterBladeCKBImpl::_SignTX(const std::vector<std::string> &vInputPath, TW::NervosCKB::Transaction &tx,
                                    const TWCoinType &coinNet) {

    return JUBR_IMPL_NOT_SUPPORT;
}

JUB_RV
JubiterBladeCKBImpl::_VerifyTX(const std::vector<std::string> &vInputPath, const TW::NervosCKB::Transaction &tx,
                               const std::vector<uchar_vector> &vSignatureRaw, const TWCoinType &coinNet) {

    JUB_RV rv = JUBR_OK;

    TW::NervosCKB::Transaction signedTx(tx);

    auto witness = signedTx.witnessGroups();
    if (vSignatureRaw.size() != witness.size()) {
        return JUBR_ARGUMENTS_BAD;
    }

    std::vector<TW::PublicKey> vInputPublicKey;
    for (size_t index = 0; index < witness.size(); ++index) {
        // derive key using BTC version
        std::string xpub;
        JUB_UINT32 parentFingerprint;
        JUB_VERIFY_RV(GetHDNode(p2pkh, vInputPath[index], xpub));

        uchar_vector publicKey;
        JUB_VERIFY_RV(_getPubkeyFromXpub(xpub, publicKey));
        TW::PublicKey twpk = TW::PublicKey(TW::Data(publicKey), _publicKeyType);
        vInputPublicKey.push_back(twpk);

        if (!witness[index].isZero()) {
            TW::NervosCKB::WitnessArgs arg = witness[index];
            arg.lock = vSignatureRaw[index];
            signedTx.witnesses.push_back(arg);
        } else {
            signedTx.witnesses.push_back(TW::NervosCKB::WitnessArgs(0x00));
        }
    }

    JUB_VERIFY_RV(_verifyTX(signedTx, vInputPublicKey, coinNet));

    return rv;
}

} // namespace token
} // namespace jub
