#pragma once
#include <sys/types.h>
#include <token/interface/ETHTokenInterface.hpp>
#include <TrustWalletCore/TWCoinType.h>


namespace jub {
namespace token {


class JubiterBaseETHImpl :
virtual public ETHTokenInterface {
public:
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
    };

public:
    JubiterBaseETHImpl() {
        _coin = TWCoinType::TWCoinTypeEthereum;
        _curve_name = (char*)SECP256K1_ETH_NAME;
    };

    virtual JUB_RV VerifyTx(const std::vector<JUB_BYTE>& vChainID,
                            const uchar_vector& signedRaw,
                            const TW::Data& publicKey);
    virtual JUB_RV VerifyBytestring(const std::vector<JUB_BYTE>& vChainID,
                                    const uchar_vector& vTypedData,
                                    const uchar_vector& vSignature,
                                    const TW::Data& publicKey);

protected:
    virtual JUB_RV _getPubkeyFromXpub(const std::string& xpub, TW::Data& publicKey,
                                      uint32_t hdVersionPub=TWCoinType2HDVersionPublic(TWCoinType::TWCoinTypeBitcoin),
                                      uint32_t hdVersionPrv=TWCoinType2HDVersionPrivate(TWCoinType::TWCoinTypeBitcoin));
    virtual JUB_RV _getAddress(const TW::Data& publicKey, std::string& address);
}; // class JubiterBaseETHImpl end


} // namespace token end
} // namespace jub end
