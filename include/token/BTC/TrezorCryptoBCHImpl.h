#pragma once
#include <memory>

#include <token/BTC/TrezorCryptoBTCImpl.h>
#include <token/BTC/JubiterBaseBCHImpl.h>


namespace jub {
namespace token {


class TrezorCryptoBCHImpl :
        public TrezorCryptoBTCImpl,
virtual public  JubiterBaseBCHImpl {
public:
    //for Factory
    static std::shared_ptr<BaseToken> Create(const std::string& XPRVorXPUB) {
        return std::make_shared<TrezorCryptoBCHImpl>(XPRVorXPUB);
    }

public:
    TrezorCryptoBCHImpl(const std::string& XPRVorXPUB) :
        TrezorCryptoBTCImpl(XPRVorXPUB) {
            _hashType = TWBitcoinSigHashTypeForkBCH;
    }
    ~TrezorCryptoBCHImpl() {}

    virtual JUB_RV SignTX(const JUB_BYTE addrFmt,
                          const JUB_ENUM_BTC_TRANS_TYPE& type,
                          const JUB_UINT16 inputCount,
                          const std::vector<JUB_UINT64>& vInputAmount,
                          const std::vector<std::string>& vInputPath,
                          const std::vector<JUB_UINT16>& vChangeIndex,
                          const std::vector<std::string>& vChangePath,
                          const std::vector<JUB_BYTE>& vUnsigedTrans,
                          std::vector<JUB_BYTE>& vRaw) override;
}; // class TrezorCryptoBCHImpl end


} // namespace token end
} // namespace jub end
