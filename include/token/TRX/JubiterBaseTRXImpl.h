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

    static JUB_RV IsValidAddress(const std::string& address, std::string& hex);
    virtual JUB_RV CheckAddress(const std::string& address, std::string& hex) override;

    virtual JUB_RV PackTransactionRaw(const JUB_TX_TRX& tx,
                                      std::string& packedContractInPB) override;
    virtual JUB_RV SerializeContract(const JUB_CONTRACT_TRX& contract,
                                     ::google::protobuf::Any& parameter) override;
    virtual JUB_RV SerializePreimage(const std::string& packedContractInPb,
                                     uchar_vector& preimageRaw) override;

protected:
    virtual JUB_RV _getAddress(const TW::Data& publicKey, std::string& address);
}; // class JubiterBaseTRXImpl end


} // namespace token end
} // namespace jub end
