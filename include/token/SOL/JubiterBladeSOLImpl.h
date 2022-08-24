#pragma once
#include <memory>

#include "token/JubiterBlade/JubiterBladeToken.h"
#include "token/SOL/JubiterBaseSOLImpl.h"
#include <string>
#include <vector>

namespace jub {
namespace token {

class JubiterBladeSOLImpl : public JubiterBladeToken, virtual public JubiterBaseSOLImpl {
  public:
    JubiterBladeSOLImpl(JUB_UINT16 deviceID) : JubiterBladeToken(deviceID) {}

    // SOL functions
    virtual JUB_RV SelectApplet() override;
    virtual JUB_RV GetAppletVersion(stVersion &version) override;
    virtual JUB_RV SetCoin() override;

    virtual JUB_RV GetAddress(const std::string &path, const JUB_UINT16 tag, std::string &address) override;
    virtual JUB_RV GetHDNode(const JUB_BYTE format, const std::string &path, std::string &pubkey) override;
    // override this, because address in solana is base58 encoding, not hex
    virtual JUB_RV SetERC20Token(JUB_CHAR_CPTR tokenName, JUB_UINT16 unitDP, JUB_CHAR_CPTR contractAddress) override;
    virtual JUB_RV SetTokenInfo(const std::string &name, JUB_UINT8 decimal, const std::string &tokenMint) override;

    // Trasfer `SOL`
    virtual JUB_RV SignTransferTx(const std::string &path, const std::vector<JUB_BYTE> &recentHash,
                                  const std::vector<JUB_BYTE> &dest, JUB_UINT64 amount,
                                  std::vector<JUB_BYTE> &raw) override;

    // Trasfer `Token`
    virtual JUB_RV SignTokenTransferTx(const std::string &path, const std::vector<JUB_BYTE> &recentHash,
                                       const std::vector<JUB_BYTE> token, const std::vector<JUB_BYTE> &from,
                                       const std::vector<JUB_BYTE> &dest, JUB_UINT64 amount, JUB_UINT8 decimal,
                                       std::vector<JUB_BYTE> &raw) override;

    // create token address
    virtual JUB_RV SignCreateTokenAccountTx(const std::string &path, const std::vector<JUB_BYTE> &recentHash,
                                            const std::vector<JUB_BYTE> &owner, const std::vector<JUB_BYTE> &token,
                                            std::vector<JUB_BYTE> &raw) override;

  protected:
}; // class JubiterBladeSOLImpl end

} // namespace token
} // namespace jub
