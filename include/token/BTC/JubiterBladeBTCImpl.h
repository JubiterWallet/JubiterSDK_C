#pragma once
#include <memory>

#include "token/BTC/JubiterBaseBTCImpl.h"
#include "token/JubiterBlade/JubiterBladeToken.h"
#include <string>
#include <vector>

namespace jub {
namespace token {

constexpr JUB_BYTE kPKIAID_BTC[16] = {0xD1, 0x56, 0x00, 0x01, 0x32, 0x83, 0x00, 0x42,
                                      0x4C, 0x44, 0x00, 0x00, 0x42, 0x54, 0x43, 0x01};

class JubiterBladeBTCImpl : virtual public JubiterBladeToken, virtual public JubiterBaseBTCImpl {
  public:
    JubiterBladeBTCImpl(const JUB_UINT16 deviceID) : JubiterBladeToken(deviceID) {}
    virtual ~JubiterBladeBTCImpl() = default;

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
    // BTC functions
    bool _isSupportLegacyAddress();
    JUB_BYTE _RealAddressFormat(const JUB_ULONG &addrFmt);
    JUB_BYTE _RealAddressFormat(const JUB_ENUM_BTC_ADDRESS_FORMAT &addrFmt);
}; // class JubiterBladeBTCImpl end

} // namespace token
} // namespace jub
