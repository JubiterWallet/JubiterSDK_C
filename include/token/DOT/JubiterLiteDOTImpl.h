#pragma once

#include "token/JubiterLite/JubiterLiteToken.h"
#include "token/JubiterLite/JubiterLiteImpl.h"
#include "token/DOT/JubiterBaseDOTImpl.h"

namespace jub {
namespace token {


class JubiterLiteDOTImpl :
        public JubiterLiteImpl,
virtual public JubiterBaseDOTImpl {

public:
    //for Factory
    static std::shared_ptr<BaseToken> Create(JUB_UINT16 deviceID) {
        return std::make_shared<JubiterLiteDOTImpl>(deviceID);
    }

    JubiterLiteDOTImpl(JUB_UINT16 deviceID) :
        JubiterLiteImpl(deviceID) {}
    ~JubiterLiteDOTImpl() {}

    //DOT functions
    virtual JUB_RV SelectApplet() override;
    virtual JUB_RV SetCoin() override;
    virtual JUB_RV SetCoinCurvePublicKeyType(JUB_ENUM_COINTYPE_DOT coinType, JUB_ENUM_CURVES curve) override;
    virtual JUB_RV GetAddress(const std::string& path, const JUB_UINT16 tag, std::string& address, const TWCoinType& coinNet=TWCoinType::TWCoinTypeBitcoin) override;
    virtual JUB_RV GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey, const TWCoinType& coinNet=TWCoinType::TWCoinTypeBitcoin) override;
    virtual JUB_RV SignTX(const std::string &path,
                          const std::string &genesisHash,
                          const std::string &blockHash,
                          const uint64_t& nonce,
                          const uint32_t& specVersion,
                          const uint64_t& network,
                          const uint32_t& transaction_version,
                          const uint64_t& blockNumber,
                          const uint64_t& eraPeriod,
                          const std::string& tip,
                          const TW::Data& call,
                          std::vector<JUB_BYTE>& vSignatureRaw) override;

protected:
    virtual JUB_RV _encodeRSV(const std::vector<JUB_BYTE>& vRSV, std::vector<JUB_BYTE>& signature);
}; // class JubiterLiteDOTImpl end


} // namespace token end
} // namespace jub end
