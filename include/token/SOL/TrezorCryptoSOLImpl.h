#pragma once
#include <memory>

#include "JUB_SDK_COMM.h"
#include "Solana/Transaction.h"
#include "token/SOL/JubiterBaseSOLImpl.h"
#include "token/TrezorCrypto/TrezorCryptoToken.h"
#include <string>
#include <vector>

namespace jub {
namespace token {

class TrezorCryptoSOLImpl : public TrezorCryptoToken, public JubiterBaseSOLImpl {
  public:
    TrezorCryptoSOLImpl(JUB_UINT16 deviceID, const JUB_ENUM_CURVES curve = JUB_ENUM_CURVES::ED25519)
        : TrezorCryptoToken(deviceID, curve) {}

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

    JUB_RV SignTx(const std::string &path, TW::Solana::Transaction &tx);

    JUB_RV DeriveChildKey(const std::string &prvKey, const std::string &path, std::string &derivPrv,
                          std::string &derivPub);
}; // class TrezorCryptoSOLImpl end

} // namespace token
} // namespace jub
