#pragma once
#include <sys/types.h>
#include "token/interface/ETHTokenInterface.hpp"
#include <TrustWalletCore/TWCoinType.h>
#include <string>
#include <vector>


namespace jub {
namespace token {

enum JUB_ENUM_APDU_DATA_ETH : uint8_t {
               TAG_NONCE_41 = 0x41,
           TAG_GAS_PRICE_42 = 0x42,
           TAG_GAS_LIMIT_43 = 0x43,
                  TAG_TO_44 = 0x44,
               TAG_VALUE_45 = 0X45,
               TAG_INPUT_46 = 0x46,
                TAG_PATH_47 = 0X47,
            TAG_CHAIN_ID_48 = 0x48,
                 TAG_MSG_49 = 0x49,
          TAG_CID_PREFIX_4A = 0x4A,
             TAG_VERSION_4B = 0x4B,
       TAG_ABI_METHODNUM_4C = 0x4C,
                TAG_DATA_4D = 0x4D,
         TAG_GAS_FEE_CAP_4E = 0x4E,
         TAG_GAS_PREMIUM_4F = 0x4F,
         TAG_DOMAIN_NAME_50 = 0x50,
    TAG_DOMAIN_SEPARATOR_51 = 0x51,
     TAG_STRUCT_MSG_HASH_52 = 0x52,
         TAG_ACCESS_LIST_53 = 0x53,
MAX_PRIORITY_FEE_PER_GAS_54 = 0x54,
         MAX_FEE_PER_GAS_55 = 0x55,
};

class JubiterBaseETHImpl :
virtual public ETHTokenInterface {
public:
    JubiterBaseETHImpl() {
        _coin = TWCoinType::TWCoinTypeEthereum;
        _curve_name = (char*)SECP256K1_ETH_NAME;
    };

    virtual JUB_RV VerifyTx(const std::vector<JUB_BYTE>& vChainID,
                            const uchar_vector& signedRaw,
                            const TW::Data& publicKey);
    virtual JUB_RV VerifyBytestring(const uchar_vector& vData,
                                    const uchar_vector& vSignature,
                                    const TW::Data& publicKey);

    virtual JUB_RV VerifyTypedData(const bool& bMetamaskV4Compat,
                                   const std::string& typedDataInJSON,
                                   const std::vector<JUB_BYTE>& vSignature,
                                   const TW::Data& publicKey);

protected:
    virtual JUB_RV _getPubkeyFromXpub(const std::string& xpub, TW::Data& publicKey,
                                      uint32_t hdVersionPub=TWCoinType2HDVersionPublic(TWCoinType::TWCoinTypeBitcoin),
                                      uint32_t hdVersionPrv=TWCoinType2HDVersionPrivate(TWCoinType::TWCoinTypeBitcoin));
    virtual JUB_RV _getAddress(const TW::Data& publicKey, std::string& address);
}; // class JubiterBaseETHImpl end


} // namespace token end
} // namespace jub end
