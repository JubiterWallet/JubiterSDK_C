#pragma once
#include <memory>

#include "JUB_SDK_COMM.h"
#include "token/BTC/JubiterBladeBTCImpl.h"
#include "token/CKB/JubiterBaseCKBImpl.h"
#include <string>
#include <vector>

namespace jub {
namespace token {

class JubiterBladeCKBImpl : public JubiterBladeBTCImpl,
                            public JubiterBaseCKBImpl {
public:
  JubiterBladeCKBImpl(const JUB_UINT16 deviceID)
      : JubiterBladeBTCImpl(deviceID) {}
  ~JubiterBladeCKBImpl() {}
  JUB_RV SelectApplet() override;
  JUB_RV SetCoin(const JUB_ENUM_COINTYPE_BTC &type) override;
  JUB_RV SetUnit(const JUB_ENUM_BTC_UNIT_TYPE &unit) override;
  
  virtual JUB_RV GetHDNode(
          const JUB_ENUM_BTC_TRANS_TYPE &type, const std::string &path,
          std::string &xpub,
          const TWCoinType &coinNet = TWCoinType::TWCoinTypeBitcoin) override;
  virtual JUB_RV GetAddress(
      const JUB_BYTE addrFmt, const JUB_ENUM_BTC_TRANS_TYPE &type,
      const std::string &path, const JUB_UINT16 tag, std::string &address,
      const TWCoinType &coinNet = TWCoinType::TWCoinTypeBitcoin) override;

  virtual JUB_RV
  SignTX(const JUB_ENUM_BTC_TRANS_TYPE &type,
         const std::vector<std::string> &vInputPath, const JUB_UINT32 version,
         const std::vector<CELL_DEP> &vDeps,
         const std::vector<CELL_INPUT> &vInputs,
         const std::vector<CELL_OUTPUT> &vOutputs,
         std::vector<uchar_vector> &vSignatureRaw,
         const TWCoinType &coinNet = TWCoinType::TWCoinTypeBitcoin) override;

protected:
  virtual JUB_RV _SignTX(const std::vector<std::string> &vInputPath,
                         TW::NervosCKB::Transaction &tx,
                         const TWCoinType &coinNet) override;
  virtual JUB_RV _VerifyTX(const std::vector<std::string> &vInputPath,
                           const TW::NervosCKB::Transaction &tx,
                           const std::vector<uchar_vector> &vSignatureRaw,
                           const TWCoinType &coinNet) override;
}; // class JubiterBladeCKBImpl end

} // namespace token
} // namespace jub
