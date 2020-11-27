#pragma once
#include "token/interface/TRXTokenInterface.hpp"
#include <TrustWalletCore/TWCoinType.h>


namespace jub {
namespace token {


class JubiterBaseTRXImpl :
virtual public TRXTokenInterface {
public:
    JubiterBaseTRXImpl() {
        _coin = TWCoinType::TWCoinTypeTron;
        _curve_name = (char*)SECP256K1_TRX_NAME;
    };

    virtual JUB_RV PackTransactionRaw(const JUB_TX_TRX& tx,
                                      std::string& packedContractInPB);
    virtual JUB_RV SerializeContract(const JUB_CONTRACT_TRX& contract,
                                     ::google::protobuf::Any& parameter);
    virtual JUB_RV SerializePreimage(const std::string& packedContractInPb,
                                     uchar_vector& preimageRaw);

protected:
    virtual JUB_RV _getPubkeyFromXpub(const std::string& xpub, TW::Data& publicKey,
                                      uint32_t hdVersionPub=TWCoinType2HDVersionPublic(TWCoinType::TWCoinTypeBitcoin),
                                      uint32_t hdVersionPrv=TWCoinType2HDVersionPrivate(TWCoinType::TWCoinTypeBitcoin));
    virtual JUB_RV _getAddress(const TW::Data& publicKey, std::string& address);
}; // class JubiterBaseTRXImpl end


} // namespace token end
} // namespace jub end
