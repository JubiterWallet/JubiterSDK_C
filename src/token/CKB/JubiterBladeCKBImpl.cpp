#include "token/CKB/JubiterBladeCKBImpl.h"
#include "NervosCKB/SegwitAddress.h"
#include "utility/util.h"

namespace jub {
namespace token {


JUB_RV JubiterBladeCKBImpl::GetAddress(const JUB_BYTE addrFmt,
                                       const JUB_ENUM_BTC_TRANS_TYPE& type,
                                       const std::string& path,
                                       const JUB_UINT16 tag,
                                       std::string& address,
                                       const TWCoinType& coinNet) {

    JUB_BYTE p1 = (JUB_BYTE)tag;
    if (_isSupportLegacyAddress()) {
        p1 |= _RealAddressFormat(addrFmt);
    }
    JUB_BYTE sigType;
    switch (type) {
    case p2pkh:
    {
        sigType = TW::NervosCKB::TWNervosCKBAddressFormat::TWNervosCKBAddressShort;
        break;
    } // case p2pkh end
    case p2sh_p2wpkh:
    {
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


JUB_RV JubiterBladeCKBImpl::GetHDNode(const JUB_ENUM_BTC_TRANS_TYPE& type, const std::string& path, std::string& xpub, const TWCoinType& coinNet) {

    JUB_BYTE p2 = 0x00;
    switch (type) {
    case p2pkh:
    {
        p2 = 0x00;   //xpub  format
        break;
    } // case p2pkh end
    case p2sh_p2wpkh:
    {
        p2 = 0x01;   //ypub  format
        break;
    } // case p2sh_p2wpkh end
    default:
        return JUBR_ARGUMENTS_BAD;
    } // switch (type) end

    //path = "m/44'/X'/0'";
    uchar_vector vPubkey;
    JUB_VERIFY_RV(JubiterBladeToken::GetHDNode(0x00, p2, path, vPubkey));

    xpub = std::string(vPubkey.begin(), vPubkey.end());

    return JUBR_OK;
}


JUB_RV JubiterBladeCKBImpl::SignTX(const JUB_ENUM_BTC_TRANS_TYPE& type,
                                   const std::vector<std::string>& vInputPath,
                                   const JUB_UINT32 version,
                                   const std::vector<CELL_DEP>& vDeps,
                                   const std::vector<CELL_INPUT>& vInputs,
                                   const std::vector<CELL_OUTPUT>& vOutputs,
                                   std::vector<uchar_vector>& vSignatureRaw,
                                   const TWCoinType& coinNet) {

    try {
        uchar_vector apduData;

        //  first pack
//        APDU apdu(0x00, 0xF8, p1, sigType, (JUB_ULONG)apduData.size(), apduData.data());
//        JUB_UINT16 ret = 0;
//        JUB_VERIFY_RV(_SendApdu(&apdu, ret));
//        JUB_VERIFY_COS_ERROR(ret);
//        apduData.clear();

        // pathTLV
        uchar_vector pathLV;
        for (auto path : vInputPath) {
            pathLV << (JUB_BYTE)(path.size());
            pathLV << path;
        }

    }
    catch (...) {
        return JUBR_ERROR_ARGS;
    }

    return JUBR_IMPL_NOT_SUPPORT;
}


JUB_RV JubiterBladeCKBImpl::_SignTX(const std::vector<std::string>& vInputPath,
                                    TW::NervosCKB::Transaction& tx,
                                    const TWCoinType& coinNet) {

    return JUBR_IMPL_NOT_SUPPORT;
}


JUB_RV JubiterBladeCKBImpl::_VerifyTX(const std::vector<std::string>& vInputPath,
                                      const TW::NervosCKB::Transaction& tx,
                                      const std::vector<uchar_vector>& vSignatureRaw,
                                      const TWCoinType& coinNet) {

    return JUBR_IMPL_NOT_SUPPORT;
}


} // namespace token end
} // namespace jub end
