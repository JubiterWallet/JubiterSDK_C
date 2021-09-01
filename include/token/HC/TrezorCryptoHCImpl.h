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
    TrezorCryptoHCImpl(JUB_UINT16 deviceID, const JUB_ENUM_CURVES curve=JUB_ENUM_CURVES::SECP256K1) :
        TrezorCryptoBTCImpl(deviceID, curve) {
            _hashType = TWHcashSigHashAll;
    }
    ~TrezorCryptoHCImpl() {}

    virtual JUB_RV MnemonicToSeed(const std::string& passphrase, const std::string& mnemonic,
                                  uchar_vector& vSeed,
                                  void (*progress_callback)(JUB_UINT32 current, JUB_UINT32 total)) override;

    virtual JUB_RV SignTX(const JUB_BYTE addrFmt,
                          const JUB_ENUM_BTC_TRANS_TYPE& type,
                          const JUB_UINT16 inputCount,
                          const std::vector<JUB_UINT64>& vInputAmount,
                          const std::vector<std::string>& vInputPath,
                          const std::vector<JUB_UINT16>& vChangeIndex,
                          const std::vector<std::string>& vChangePath,
                          const std::vector<JUB_BYTE>& vUnsigedTrans,
                          std::vector<JUB_BYTE>& vRaw,
                          const TWCoinType& coinNet) override;

private:
    virtual JUB_RV _HdnodeCkd(const std::string& path, HDNode* node, JUB_UINT32* parentFingerprint, const TWCoinType& coinNet) override;
    virtual JUB_RV _xpub2dpub(const std::string& xpub, std::string& dpub);
}; // class TrezorCryptoHCImpl end


} // namespace token end
} // namespace jub end
