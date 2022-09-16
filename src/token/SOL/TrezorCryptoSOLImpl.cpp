#include "token/SOL/TrezorCryptoSOLImpl.h"
#include "Data.h"
#include "HDKey/HDKey.hpp"
#include "JUB_SDK_COMM.h"
#include "PrivateKey.h"
#include "Solana/Address.h"
#include "Solana/Program.h"
#include "Solana/Transaction.h"
#include "TrezorCrypto/bip32.h"
#include "mSIGNA/stdutils/uchar_vector.h"
#include "utility/util.h"
#include <Solana/Signer.h>
#include <string>
#include <vector>

namespace jub {
namespace token {

JUB_RV TrezorCryptoSOLImpl::SelectApplet() { return JUBR_OK; }

JUB_RV TrezorCryptoSOLImpl::GetAppletVersion(stVersion &version) { return JUBR_OK; }
JUB_RV TrezorCryptoSOLImpl::SetCoin() { return JUBR_OK; }

JUB_RV TrezorCryptoSOLImpl::GetAddress(const std::string &path, const JUB_UINT16 tag, std::string &address) {
    std::string derivPrv;
    std::string derivpub;
    JUB_VERIFY_RV(DeriveChildKey(_MasterKey_XPRV, path, derivPrv, derivpub));

    uchar_vector pub(derivpub);
    // only need 32 bytes
    return _getAddress({pub.begin() + 1, pub.end()}, address);
}

JUB_RV TrezorCryptoSOLImpl::GetHDNode(const JUB_BYTE format, const std::string &path, std::string &pubkey) {

    if (!path.empty()) {
        std::string derivPrv;
        std::string derivpub;
        JUB_VERIFY_RV(DeriveChildKey(_MasterKey_XPRV, path, derivPrv, derivpub));
        pubkey = derivpub;
    } else {
        pubkey = _MasterKey_XPUB;
    }

    return JUBR_OK;
}

JUB_RV TrezorCryptoSOLImpl::DeriveChildKey(const std::string &prvKey, const std::string &path, std::string &derivPrv,
                                           std::string &derivPub) {
    // drive from master key, see caller
    uchar_vector seed(prvKey);
    HDNode node;
    hdnode_from_seed(seed.data(), seed.size(), _curve_name, &node);
    auto paths = parsePath(path);
    for (auto path : paths) {
        if (!hdnode_private_ckd(&node, path)) {
            return JUBR_ARGUMENTS_BAD;
        }
    }
    hdnode_fill_public_key(&node);
    derivPrv = uchar_vector{node.private_key, node.private_key + 32}.getHex();
    derivPub = uchar_vector{node.public_key, node.public_key + 33}.getHex();

    return JUBR_OK;
}

JUB_RV TrezorCryptoSOLImpl::SetTokenInfo(const std::string &tokenName, JUB_UINT8 unitDP, const std::string &tokenMint) {
    return JUBR_OK;
}

JUB_RV TrezorCryptoSOLImpl::SignTransferTx(const std::string &path, const std::vector<JUB_BYTE> &recentHash,
                                           const std::vector<JUB_BYTE> &dest, JUB_UINT64 amount,
                                           std::vector<JUB_BYTE> &raw) {
    using namespace TW::Solana;
    auto from = std::string{};
    JUB_VERIFY_RV(GetAddress(path, JUB_FALSE, from));

    auto h = Hash{};
    std::copy(recentHash.begin(), recentHash.end(), h.bytes.begin());
    auto msg = Message::createTransfer(Address{from}, Address{dest}, amount, h);

    auto tx = Transaction{msg};
    JUB_VERIFY_RV(SignTx(path, tx));
    raw = tx.serialize();
    return JUBR_OK;
}
JUB_RV TrezorCryptoSOLImpl::SignTokenTransferTx(const std::string &path, const std::vector<JUB_BYTE> &recentHash,
                                                const std::vector<JUB_BYTE> token, const std::vector<JUB_BYTE> &from,
                                                const std::vector<JUB_BYTE> &dest, JUB_UINT64 amount, JUB_UINT8 decimal,
                                                std::vector<JUB_BYTE> &raw) {
    using namespace TW::Solana;
    auto signer = std::string{};
    JUB_VERIFY_RV(GetAddress(path, JUB_FALSE, signer));
    auto h = Hash{};
    std::copy(recentHash.begin(), recentHash.end(), h.bytes.begin());
    auto msg =
        Message::createTokenTransfer(Address{signer}, Address{token}, Address{from}, Address{dest}, amount, decimal, h);

    auto tx = Transaction{msg};
    JUB_VERIFY_RV(SignTx(path, tx));
    raw = tx.serialize();
    return JUBR_OK;
}

JUB_RV TrezorCryptoSOLImpl::SignCreateTokenAccountTx(const std::string &path, const std::vector<JUB_BYTE> &recentHash,
                                                     const std::vector<JUB_BYTE> &owner,
                                                     const std::vector<JUB_BYTE> &token, std::vector<JUB_BYTE> &raw) {
    using namespace TW::Solana;
    auto signer = std::string{};
    JUB_VERIFY_RV(GetAddress(path, JUB_FALSE, signer));
    auto h = Hash{};
    std::copy(recentHash.begin(), recentHash.end(), h.bytes.begin());
    auto tokenAccount = TokenProgram::defaultTokenAddress(Address{owner}, Address{token});
    auto msg = Message::createTokenCreateAccount(Address{signer}, Address{owner}, Address{token}, tokenAccount, h);

    auto tx = Transaction{msg};
    JUB_VERIFY_RV(SignTx(path, tx));
    raw = tx.serialize();
    return JUBR_OK;
}

JUB_RV TrezorCryptoSOLImpl::SignTx(const std::string &path, TW::Solana::Transaction &tx) {
    std::string key, pubKey;
    JUB_VERIFY_RV(DeriveChildKey(_MasterKey_XPRV, path, key, pubKey));
    auto bytes   = uchar_vector{key};
    auto privKey = TW::PrivateKey{TW::Data{bytes}};

    TW::Solana::Signer::sign({privKey}, tx);
    return JUBR_OK;
}

} // namespace token
} // namespace jub
