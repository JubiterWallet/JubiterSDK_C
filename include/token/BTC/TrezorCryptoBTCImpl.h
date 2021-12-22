#pragma once
#include <memory>

#include "token/BTC/JubiterBaseBTCImpl.h"
#include "token/TrezorCrypto/TrezorCryptoToken.h"
#include <string>
#include <vector>

namespace jub {
namespace token {

class TrezorCryptoBTCImpl : public TrezorCryptoToken, virtual public JubiterBaseBTCImpl {
  public:
    TrezorCryptoBTCImpl(JUB_UINT16 deviceID, const JUB_ENUM_CURVES curve = JUB_ENUM_CURVES::SECP256K1)
        : TrezorCryptoToken(deviceID, curve) {}
    ~TrezorCryptoBTCImpl() {}

    // BTC functions
    virtual JUB_RV SelectApplet() override;
    virtual JUB_RV GetHDNode(const JUB_ENUM_BTC_TRANS_TYPE &type, const std::string &path, std::string &xpub,
                             const TWCoinType &coinNet) override;
    virtual JUB_RV GetAddress(const JUB_BYTE addrFmt, const JUB_ENUM_BTC_TRANS_TYPE &type, const std::string &path,
                              const JUB_UINT16 tag, std::string &address, const TWCoinType &coinNet) override;
    virtual JUB_RV SetUnit(const JUB_ENUM_BTC_UNIT_TYPE &unit) override;
    virtual JUB_RV SetCoin(const JUB_ENUM_COINTYPE_BTC &type) override;
    virtual JUB_RV SignTX(const JUB_BYTE addrFmt, const JUB_ENUM_BTC_TRANS_TYPE &type, const JUB_UINT16 inputCount,
                          const std::vector<JUB_UINT64> &vInputAmount, const std::vector<std::string> &vInputPath,
                          const std::vector<JUB_UINT16> &vChangeIndex, const std::vector<std::string> &vChangePath,
                          const std::vector<JUB_BYTE> &vUnsigedTrans, std::vector<JUB_BYTE> &vRaw,
                          const TWCoinType &coinNet) override;
    virtual JUB_RV VerifyTX(const JUB_ENUM_BTC_TRANS_TYPE &type, const std::vector<JUB_UINT64> &vInputAmount,
                            const std::vector<std::string> &vInputPath, const std::vector<JUB_BYTE> &vSigedTrans,
                            const TWCoinType &coinNet) override;

  protected:
    virtual JUB_RV _SignTx(bool witness, const std::vector<JUB_UINT64> &vInputAmount,
                           const std::vector<std::string> &vInputPath, const std::vector<JUB_UINT16> &vChangeIndex,
                           const std::vector<std::string> &vChangePath, const TW::Bitcoin::Transaction &tx,
                           std::vector<TW::Data> &vInputPublicKey, std::vector<uchar_vector> &vSignatureRaw,
                           const TWCoinType &coinNet);
    JUB_RV _SignTaproot(const std::vector<JUB_UINT64> &vInputAmount, const std::vector<std::string> &vInputPath,
                        const std::vector<JUB_UINT16> &vChangeIndex, const std::vector<std::string> &vChangePath,
                        const TW::Bitcoin::Transaction &tx, std::vector<TW::Data> &vInputPublicKey,
                        std::vector<uchar_vector> &vSignatureRaw, const TWCoinType &coinNet);

}; // class TrezorCryptoBTCImpl end

} // namespace token
} // namespace jub
