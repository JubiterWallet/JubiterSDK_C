#pragma once
#include <memory>

#include "token/JubiterBlade/JubiterBladeToken.h"
#include "token/ETH/JubiterBladeETHImpl.h"
#include "token/DOT/JubiterBaseDOTImpl.h"


namespace jub {
namespace token {


class JubiterBladeDOTImpl :
        public JubiterBladeToken,
virtual public JubiterBaseDOTImpl {

public:
    //for Factory
    static std::shared_ptr<BaseToken> Create(JUB_UINT16 deviceID) {
        return std::make_shared<JubiterBladeDOTImpl>(deviceID);
    }

    JubiterBladeDOTImpl(JUB_UINT16 deviceID) :
        JubiterBladeToken(deviceID) {}
    ~JubiterBladeDOTImpl() {}


    //DOT functions
    virtual JUB_RV SelectApplet() override;
    virtual JUB_RV SetCoin() override;
    virtual JUB_RV SetCoinCurvePublicKeyType(JUB_ENUM_COINTYPE_DOT coinType, JUB_ENUM_CURVES curve) override;
    virtual JUB_RV GetAddress(const std::string& path, const JUB_UINT16 tag, std::string& address, const TWCoinType& coinNet=TWCoinType::TWCoinTypeBitcoin) override;
    virtual JUB_RV GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey, const TWCoinType& coinNet=TWCoinType::TWCoinTypeBitcoin) override;
    virtual JUB_RV SignTX(const std::string &path,
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
                          std::vector<JUB_BYTE>& vSignatureRaw) override;
}; // class JubiterBladeDOTImpl end


} // namespace token end
} // namespace jub end
