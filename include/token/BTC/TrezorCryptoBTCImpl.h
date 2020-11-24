#pragma once
#include <memory>

#include "token/TrezorCrypto/TrezorCryptoToken.h"
#include "token/BTC/JubiterBaseBTCImpl.h"


namespace jub {
namespace token {


class TrezorCryptoBTCImpl :
        public TrezorCryptoToken,
virtual public JubiterBaseBTCImpl {
public:
    //for Factory
    static std::shared_ptr<BaseToken> Create(const std::string& XPRVorXPUB) {
        return std::make_shared<TrezorCryptoBTCImpl>(XPRVorXPUB);
    }

public:
    TrezorCryptoBTCImpl(const std::string& XPRVorXPUB) :
        TrezorCryptoToken(XPRVorXPUB) {}
    ~TrezorCryptoBTCImpl() {}

    //BTC functions
    virtual JUB_RV SelectApplet() override;
    virtual JUB_RV GetHDNode(const JUB_ENUM_BTC_TRANS_TYPE& type, const std::string& path, std::string& xpub) override;
    virtual JUB_RV GetAddress(const JUB_BYTE addrFmt, const JUB_ENUM_BTC_TRANS_TYPE& type, const std::string& path, const JUB_UINT16 tag, std::string& address) override;
    virtual JUB_RV SetUnit(const JUB_ENUM_BTC_UNIT_TYPE& unit) override;
    virtual JUB_RV SetCoin(const JUB_ENUM_COINTYPE_BTC& type) override;
    virtual JUB_RV SignTX(const JUB_BYTE addrFmt,
                          const JUB_ENUM_BTC_TRANS_TYPE& type,
                          const JUB_UINT16 inputCount,
                          const std::vector<JUB_UINT64>& vInputAmount,
                          const std::vector<std::string>& vInputPath,
                          const std::vector<JUB_UINT16>& vChangeIndex,
                          const std::vector<std::string>& vChangePath,
                          const std::vector<JUB_BYTE>& vUnsigedTrans,
                          std::vector<JUB_BYTE>& vRaw) override;
    virtual JUB_RV VerifyTX(const JUB_ENUM_BTC_TRANS_TYPE& type,
                            const std::vector<JUB_UINT64>& vInputAmount,
                            const std::vector<std::string>& vInputPath,
                            const std::vector<JUB_BYTE>& vSigedTrans) override;

protected:
    virtual JUB_RV _SignTx(bool witness,
                           const std::vector<JUB_UINT64>& vInputAmount,
                           const std::vector<std::string>& vInputPath,
                           const std::vector<JUB_UINT16>& vChangeIndex,
                           const std::vector<std::string>& vChangePath,
                           const TW::Bitcoin::Transaction& tx,
                           std::vector<TW::Data>& vInputPublicKey,
                           std::vector<uchar_vector>& vSignatureRaw);
}; // class TrezorCryptoBTCImpl end


} // namespace token end
} // namespace jub end
