#pragma once
#include <memory>

#include "token/BTC/TrezorCryptoBTCImpl.h"
#include "token/HC/JubiterBaseHCImpl.h"


namespace jub {
namespace token {


class TrezorCryptoHCImpl :
        public TrezorCryptoBTCImpl,
virtual public JubiterBaseHCImpl {

public:
    //for Factory
    static std::shared_ptr<BaseToken> Create(const std::string& XPRVorXPUB) {
        return std::make_shared<TrezorCryptoHCImpl>(XPRVorXPUB);
    }

    TrezorCryptoHCImpl(const std::string& xprv) :
        TrezorCryptoBTCImpl(xprv) {
            _hashType = TWHcashSigHashAll;
    }
    ~TrezorCryptoHCImpl() {}

    virtual JUB_RV SignTX(const JUB_BYTE addrFmt,
                          const JUB_ENUM_BTC_TRANS_TYPE& type,
                          const JUB_UINT16 inputCount,
                          const std::vector<JUB_UINT64>& vInputAmount,
                          const std::vector<std::string>& vInputPath,
                          const std::vector<JUB_UINT16>& vChangeIndex,
                          const std::vector<std::string>& vChangePath,
                          const std::vector<JUB_BYTE>& vUnsigedTrans,
                          std::vector<JUB_BYTE>& vRaw) override;

private:
    virtual JUB_RV _HdnodeCkd(std::string path, HDNode* node, JUB_UINT32* parentFingerprint) override;
    virtual JUB_RV _xpub2dpub(const std::string& xpub, std::string& dpub);
}; // class TrezorCryptoHCImpl end


} // namespace token end
} // namespace jub end
