#pragma once
#include <sys/types.h>
#include "token/interface/ETHTokenInterface.hpp"
#include <TrustWalletCore/TWCoinType.h>


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
    virtual JUB_RV VerifyBytestring(const std::vector<JUB_BYTE>& vChainID,
                                    const uchar_vector& vData,
                                    const uchar_vector& vSignature,
                                    const TW::Data& publicKey);

    virtual JUB_RV VerifyTypedData(const bool& bMetamaskV4Compat,
                                   const std::string& typedDataInJSON,
                                   const std::vector<JUB_BYTE>& vSignature,
                                   const TW::Data& publicKey);

protected:
    virtual JUB_RV _getAddress(const TW::Data& publicKey, std::string& address);
}; // class JubiterBaseETHImpl end


} // namespace token end
} // namespace jub end
