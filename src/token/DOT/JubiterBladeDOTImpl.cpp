#include "token/DOT/JubiterBladeDOTImpl.h"
#include "token/ETH/JubiterBaseETHImpl.h"

//#include <Polkadot/Transaction.h>
#include "Cbor.h"
#include "token/ErrorHandler.h"


namespace jub {
namespace token {


#define SWITCH_TO_DOT_APP                       \
do {                                            \
    JUB_VERIFY_RV(_SelectApp(kPKIAID_ETH, sizeof(kPKIAID_ETH)/sizeof(JUB_BYTE)));\
} while (0);                                    \


JUB_RV JubiterBladeDOTImpl::SelectApplet() {

    SWITCH_TO_DOT_APP;
    return JUBR_OK;
}


//MISC functions
JUB_RV JubiterBladeDOTImpl::SetCoin() {

    APDU apdu(0x00, 0xF5, (JUB_BYTE)JUB_ENUM_COINTYPE_ETH::COINFIL, 0x00, 0x00);
    JUB_UINT16 ret = 0;
    JUB_VERIFY_RV(_SendApdu(&apdu, ret));
    if (   0x9000 == ret
        || 0x6d00 == ret
        ) {
        return JUBR_OK;
    }

    return JUBR_ERROR;
}

JUB_RV JubiterBladeDOTImpl::SetCoinCurvePublicKeyType(JUB_ENUM_COINTYPE_DOT coinType, JUB_ENUM_CURVES curves) {
    
    return JUBR_IMPL_NOT_SUPPORT;
}

JUB_RV JubiterBladeDOTImpl::GetAddress(const std::string& path, const JUB_UINT16 tag, std::string& address, const TWCoinType& coinNet) {

//    uchar_vector vPath;
//    vPath << path;
//
//    uchar_vector apduData = ToTlv(JUB_ENUM_APDU_DATA::TAG_PATH_08, vPath);
//    JUB_BYTE p1 = (JUB_BYTE)tag;
//
//    APDU apdu(0x00, 0xF6, p1, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
//    JUB_UINT16 ret = 0;
//    JUB_BYTE retData[2048] = { 0, };
//    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
//    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
//    JUB_VERIFY_COS_ERROR(ret);
//
//    address = (JUB_CHAR_PTR)retData;

    return JUBR_IMPL_NOT_SUPPORT;
}


JUB_RV JubiterBladeDOTImpl::GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey, const TWCoinType& coinNet) {

//    //path = "m/44'/354'/0'";
//    uchar_vector vPath;
//    vPath << path;
//    uchar_vector apduData = ToTlv(JUB_ENUM_APDU_DATA::TAG_PATH_08, vPath);
//
//    //0x00 for hex
//    if (JUB_ENUM_PUB_FORMAT::HEX != format
//        ) {
//        return JUBR_ERROR_ARGS;
//    }
//
//    APDU apdu(0x00, 0xE6, 0x00, format, (JUB_ULONG)apduData.size(), apduData.data());
//    JUB_UINT16 ret = 0;
//    JUB_BYTE retData[2048] = { 0, };
//    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
//    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
//    JUB_VERIFY_COS_ERROR(ret);
//
//    uchar_vector vPubkey(retData, (unsigned int)ulRetDataLen);
//    pubkey = vPubkey.getHex();

    return JUBR_IMPL_NOT_SUPPORT;
}


JUB_RV JubiterBladeDOTImpl::SignTX(const std::string &path,
                                   const std::string &genesisHash,
                                   const std::string &blockHash,
                                   const std::string &to,
                                   const uint64_t& nonce,
                                   const uint32_t& specVersion,
                                   const uint64_t& network,
                                   const uint32_t& transaction_version,
                                   const uint64_t& blockNumber,
                                   const std::string& value,
                                   const uint64_t& eraPeriod,
                                   const std::string& tip,
                                   std::vector<JUB_BYTE>& vSignatureRaw){

    

    return JUBR_IMPL_NOT_SUPPORT;
}


} // namespace token end
} // namespace jub end
