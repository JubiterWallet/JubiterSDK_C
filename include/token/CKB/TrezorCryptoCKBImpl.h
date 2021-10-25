#pragma once
#include <memory>

#include "token/BTC/TrezorCryptoBTCImpl.h"
#include "token/CKB/JubiterBaseCKBImpl.h"


namespace jub {
namespace token {


class TrezorCryptoCKBImpl :
        public TrezorCryptoToken,
virtual public JubiterBaseCKBImpl {
public:
    TrezorCryptoCKBImpl(JUB_UINT16 deviceID, const JUB_ENUM_CURVES curve=JUB_ENUM_CURVES::SECP256K1) :
        TrezorCryptoToken(deviceID, curve) {
    }
    ~TrezorCryptoCKBImpl() {}

    //CKB functions
    virtual JUB_RV SelectApplet() override;
    virtual JUB_RV SetUnit(const JUB_ENUM_BTC_UNIT_TYPE& unit) override;
    virtual JUB_RV SetCoin(const JUB_ENUM_COINTYPE_BTC& type) override;
    virtual JUB_RV GetHDNode(const JUB_ENUM_BTC_TRANS_TYPE& type, const std::string& path, std::string& hex, const TWCoinType& coinNet) override;
    virtual JUB_RV GetAddress(const JUB_BYTE addrFmt, const JUB_ENUM_BTC_TRANS_TYPE& type, const std::string& path, const JUB_UINT16 tag, std::string& address, const TWCoinType& coinNet) override;

protected:
    virtual JUB_RV _SignTX(const std::vector<std::string>& vInputPath,
                          TW::NervosCKB::Transaction& tx,
                          const TWCoinType& coinNet) override;
    virtual JUB_RV _VerifyTX(const std::vector<std::string>& vInputPath,
                             const TW::NervosCKB::Transaction& tx,
                             const std::vector<uchar_vector>& vSignatureRaw,
                             const TWCoinType& coinNet) override;
}; // class TrezorCryptoCKBImpl end


} // namespace token end
} // namespace jub end
