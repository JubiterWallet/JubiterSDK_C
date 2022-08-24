#pragma once
#include "JUB_SDK_COMM.h"
#include "token/JubiterLite/JubiterLiteImpl.h"
#include "token/JubiterLite/JubiterLiteToken.h"
#include "token/SOL/JubiterBaseSOLImpl.h"

#include <memory>
#include <string>
#include <vector>

namespace jub {
namespace token {

class JubiterLiteSOLImpl : public JubiterLiteImpl, virtual public JubiterBaseSOLImpl {
  public:
    JubiterLiteSOLImpl(JUB_UINT16 deviceID) : JubiterLiteImpl(deviceID) {}

    // SOL functions
    virtual JUB_RV SelectApplet() override;
    virtual JUB_RV GetAppletVersion(stVersion &version) override;
    virtual JUB_RV SetCoin() override;

    virtual JUB_RV GetAddress(const std::string &path, const JUB_UINT16 tag, std::string &address) override;
    virtual JUB_RV GetHDNode(const JUB_BYTE format, const std::string &path, std::string &pubkey) override;
    virtual JUB_RV SetTokenInfo(const std::string &name, JUB_UINT8 decimal,
                                const std::string &tokenMint) override final;

    // Trasfer `SOL`
    virtual JUB_RV SignTransferTx(const std::string &path, const std::vector<JUB_BYTE> &recentHash,
                                  const std::vector<JUB_BYTE> &dest, JUB_UINT64 amount,
                                  std::vector<JUB_BYTE> &raw) override final;

    // Trasfer `Token`
    virtual JUB_RV SignTokenTransferTx(const std::string &path, const std::vector<JUB_BYTE> &recentHash,
                                       const std::vector<JUB_BYTE> token, const std::vector<JUB_BYTE> &from,
                                       const std::vector<JUB_BYTE> &dest, JUB_UINT64 amount, JUB_UINT8 decimal,
                                       std::vector<JUB_BYTE> &raw) override final;

    // create token address
    virtual JUB_RV SignCreateTokenAccountTx(const std::string &path, const std::vector<JUB_BYTE> &recentHash,
                                            const std::vector<JUB_BYTE> &owner, const std::vector<JUB_BYTE> &token,
                                            std::vector<JUB_BYTE> &raw) override final;

  protected:
}; // class JubiterLiteSOLImpl end

} // namespace token
} // namespace jub
