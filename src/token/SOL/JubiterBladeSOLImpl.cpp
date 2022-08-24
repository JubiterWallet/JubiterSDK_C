#include "Address.h"
#include "BinaryCoding.h"
#include "Data.h"
#include "JUB_SDK_COMM.h"
#include "JUB_SDK_DEV.h"
#include "Solana/Program.h"
#include "mSIGNA/stdutils/uchar_vector.h"
#include "token/JubiterBlade/JubiterBladeToken.h"
#include "utility/Apdu.hpp"
#include "utility/util.h"
#include <Solana/Address.h>
#include <Solana/Signer.h>
#include <Solana/Transaction.h>
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <string>
#include <token/ErrorHandler.h>
#include <token/SOL/JubiterBladeSOLImpl.h>
#include <type_traits>
#include <vector>

namespace jub {
namespace token {
namespace {
enum SOL_APDU_TAG : uint8_t {
    TAG_PATH = 0x08,
    TAG_TO   = 0x41,
    TAG_AMOUNT,
    TAG_RECENT_HASH,
    TAG_SOURCE,
    TAG_TOKEN_MINT,
    TAG_NEWER_TOKEN_ADDRESS,
    TAG_TOKEN_ADDRESS_OWNER,
    TAG_BUMP
};

enum SOL_PROGRAM_P1 : uint8_t { P1_SYS_PROGRAM = 0x00, P1_SPL_TOKEN, P1_ASSOCIATED_TOKEN };

/// enum tag number in `instruction`
namespace sintruction_p2 {
static uint8_t SYS_INS_TRANSFER         = 0x02;
static uint8_t SPL_INS_TRANSFER         = 0x03;
static uint8_t SPL_INS_TRANSFER_CHECKED = 0x0C;
static uint8_t ASSOCIATED_INS_CREATE    = 0x00;
} // namespace sintruction_p2
} // namespace

#define SWITCH_TO_SOL_APP                                                                                              \
    do {                                                                                                               \
        JUB_VERIFY_RV(_SelectApp(kPKIAID_MISC, sizeof(kPKIAID_MISC) / sizeof(JUB_BYTE)));                              \
    } while (0);

JUB_RV JubiterBladeSOLImpl::SelectApplet() {

    SWITCH_TO_SOL_APP;
    return JUBR_OK;
}

JUB_RV JubiterBladeSOLImpl::GetAppletVersion(stVersion &version) {

    uchar_vector appID(kPKIAID_MISC, sizeof(kPKIAID_MISC) / sizeof(JUB_BYTE));
    JUB_VERIFY_RV(JubiterBladeToken::GetAppletVersion(CharPtr2HexStr(appID), version));

    return JUBR_OK;
}
JUB_RV JubiterBladeSOLImpl::SetCoin() {

    APDU apdu(0x00, 0xF5, (JUB_BYTE)JUB_ENUM_COINTYPE_MISC::COINSOL, 0x00, 0x00);
    JUB_UINT16 ret = 0;
    JUB_VERIFY_RV(_SendApdu(&apdu, ret));
    if (0x9000 == ret || 0x6d00 == ret) {
        return JUBR_OK;
    }

    return JUBR_ERROR;
}

JUB_RV JubiterBladeSOLImpl::GetAddress(const std::string &path, const JUB_UINT16 tag, std::string &address) {

    uchar_vector vAddress;
    auto data = uchar_vector{};
    data << ToTlv(SOL_APDU_TAG::TAG_PATH, {path.begin(), path.end()});
    JUB_VERIFY_RV(JubiterBladeToken::GetAddress(tag, 0x00, data, vAddress));
    address = std::string{vAddress.begin(), vAddress.end()};

    return JUBR_OK;
}

JUB_RV JubiterBladeSOLImpl::GetHDNode(const JUB_BYTE format, const std::string &path, std::string &pubkey) {

    switch (format) {
    case JUB_ENUM_PUB_FORMAT::HEX:
    case JUB_ENUM_PUB_FORMAT::XPUB:
        break;
    default:
        return JUBR_ERROR_ARGS;
    }

    // path = "m/44'/60'/0'";
    uchar_vector vPubkey;
    JUB_VERIFY_RV(JubiterBladeToken::GetHDNode(0x00, format, path, vPubkey));

    JUB_NOT_USED(format);
    pubkey = vPubkey.getHex();
    return JUBR_OK;
}

JUB_RV JubiterBladeSOLImpl::SetTokenInfo(const std::string &name, JUB_UINT8 decimal, const std::string &tokenMint) {
    return SetERC20Token(name.c_str(), decimal, tokenMint.c_str());
}
JUB_RV JubiterBladeSOLImpl::SetERC20Token(JUB_CHAR_CPTR tokenName, JUB_UINT16 unitDP, JUB_CHAR_CPTR contractAddress) {

    uchar_vector lvName  = Tollv(tokenName);
    uchar_vector address = TW::Solana::Address{contractAddress}.vector();

    uchar_vector apduData;
    apduData << (uint8_t)unitDP;
    apduData << (uint8_t)lvName.size();
    apduData << lvName;
    apduData << (uint8_t)address.size();
    apduData << address;

    APDU apdu(0x00, 0xC7, 0x00, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_VERIFY_RV(_SendApdu(&apdu, ret));
    JUB_VERIFY_COS_ERROR(ret);

    return JUBR_OK;
}

JUB_RV JubiterBladeSOLImpl::SignTransferTx(const std::string &path, const std::vector<JUB_BYTE> &recentHash,
                                           const std::vector<JUB_BYTE> &dest, JUB_UINT64 amount,
                                           std::vector<JUB_BYTE> &raw) {
    auto data = uchar_vector{};
    using TAG = SOL_APDU_TAG;
    // path
    data << ToTlv(TAG::TAG_PATH, {path.begin(), path.end()});
    // to
    data << ToTlv(TAG::TAG_TO, dest);
    // recent hash
    data << ToTlv(TAG::TAG_RECENT_HASH, recentHash);
    // amount
    data << ToTlv(TAG::TAG_AMOUNT, TW::encode64LE(amount));

    // can send by one pack
    auto apdu = APDU{0x00, 0xF8, 0x00, 0x00, data.size(), data.data()};
    JUB_UINT16 ret;
    JUB_VERIFY_RV(_SendApdu(&apdu, ret));
    if (ret != 0x9000) {
        return JUBR_TRANSMIT_DEVICE_ERROR;
    }

    using P1     = SOL_PROGRAM_P1;
    namespace P2 = sintruction_p2;

    apdu = APDU{0x00, 0x2A, P1::P1_SYS_PROGRAM, P2::SYS_INS_TRANSFER, 0};

    auto resp    = std::array<JUB_BYTE, 2>{};
    auto respLen = JUB_ULONG{2};
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, resp.data(), &respLen));
    JUB_VERIFY_COS_ERROR(ret);
    // retrieve transaction
    auto txLen = TW::decode16BE(resp.data());
    auto tx    = TW::Data(txLen);
    respLen    = txLen;

    apdu = APDU{0x00, 0xF9, 0x00, 0x00, 0x00, nullptr, txLen};
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, tx.data(), &respLen));
    if (ret != 0x9000) {
        return JUBR_TRANSMIT_DEVICE_ERROR;
    }
    raw = tx;
    return JUBR_OK;
}
JUB_RV JubiterBladeSOLImpl::SignTokenTransferTx(const std::string &path, const std::vector<JUB_BYTE> &recentHash,
                                                const std::vector<JUB_BYTE> token, const std::vector<JUB_BYTE> &from,
                                                const std::vector<JUB_BYTE> &dest, JUB_UINT64 amount, JUB_UINT8 decimal,
                                                std::vector<JUB_BYTE> &raw) {
    auto data = uchar_vector{};
    using TAG = SOL_APDU_TAG;
    // path: signer/delegate
    data << ToTlv(TAG::TAG_PATH, {path.begin(), path.end()});
    // source
    data << ToTlv(TAG::TAG_SOURCE, from);
    // to
    data << ToTlv(TAG::TAG_TO, dest);
    // token mint
    data << ToTlv(TAG::TAG_TOKEN_MINT, token);
    // recent hash
    data << ToTlv(TAG::TAG_RECENT_HASH, recentHash);
    // amount
    data << ToTlv(TAG::TAG_AMOUNT, TW::encode64LE(amount));
    // decimal
    JUB_NOT_USED(decimal);

    // can send by one pack
    auto apdu = APDU{0x00, 0xF8, 0x00, 0x00, data.size(), data.data()};
    JUB_UINT16 ret;
    JUB_VERIFY_RV(_SendApdu(&apdu, ret));
    if (ret != 0x9000) {
        return JUBR_TRANSMIT_DEVICE_ERROR;
    }

    using P1     = SOL_PROGRAM_P1;
    namespace P2 = sintruction_p2;

    apdu = APDU{0x00, 0x2A, P1::P1_SPL_TOKEN, P2::SPL_INS_TRANSFER_CHECKED, 0};

    auto resp    = std::array<JUB_BYTE, 2>{};
    auto respLen = JUB_ULONG{2};
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, resp.data(), &respLen));
    JUB_VERIFY_COS_ERROR(ret);
    // retrieve transaction
    auto txLen  = TW::decode16BE(resp.data());
    auto tx     = TW::Data(txLen);
    auto p      = tx.data();
    auto offset = JUB_UINT16{0};

    constexpr JUB_ULONG kSendOnceLen = 230;
    while (txLen != 0) {
        auto pack = std::min((JUB_ULONG)txLen, kSendOnceLen);

        apdu = APDU{0x00, 0xF9, (JUB_ULONG)(offset >> 8), (JUB_ULONG)(offset & 0xff), 0x00, nullptr, pack};
        JUB_VERIFY_RV(_SendApdu(&apdu, ret, p, &pack));
        if (ret != 0x9000) {
            return JUBR_TRANSMIT_DEVICE_ERROR;
        }
        p += pack;
        offset += pack;
        txLen -= pack;
    }

    raw = tx;
    return JUBR_OK;
}
JUB_RV JubiterBladeSOLImpl::SignCreateTokenAccountTx(const std::string &path, const std::vector<JUB_BYTE> &recentHash,
                                                     const std::vector<JUB_BYTE> &owner,
                                                     const std::vector<JUB_BYTE> &token, std::vector<JUB_BYTE> &raw) {
    using namespace TW::Solana;
    // we need address and index, see `TokenProgram::defaultTokenAddress`
    auto programId = Address(TOKEN_PROGRAM_ID_ADDRESS);

    auto seeds = std::vector<TW::Data>{owner, programId.vector(), token};

    auto [newer, index] = TokenProgram::findProgramAddress(seeds, Address(ASSOCIATED_TOKEN_PROGRAM_ID_ADDRESS));

    auto data = uchar_vector{};
    using TAG = SOL_APDU_TAG;
    // path: signer
    data << ToTlv(TAG::TAG_PATH, {path.begin(), path.end()});
    // new address to be created
    data << ToTlv(TAG::TAG_NEWER_TOKEN_ADDRESS, newer.vector());
    // owner
    data << ToTlv(TAG::TAG_TOKEN_ADDRESS_OWNER, owner);
    // token mint
    data << ToTlv(TAG::TAG_TOKEN_MINT, token);
    // recent hash
    data << ToTlv(TAG::TAG_RECENT_HASH, recentHash);
    // bump
    data << ToTlv(TAG::TAG_BUMP, {index});

    // can send by one pack
    auto apdu = APDU{0x00, 0xF8, 0x00, 0x00, data.size(), data.data()};
    JUB_UINT16 ret;
    JUB_VERIFY_RV(_SendApdu(&apdu, ret));
    if (ret != 0x9000) {
        return JUBR_TRANSMIT_DEVICE_ERROR;
    }

    using P1     = SOL_PROGRAM_P1;
    namespace P2 = sintruction_p2;

    apdu = APDU{0x00, 0x2A, P1::P1_ASSOCIATED_TOKEN, P2::ASSOCIATED_INS_CREATE, 0};

    auto resp    = std::array<JUB_BYTE, 2>{};
    auto respLen = JUB_ULONG{2};

    // sign tx
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, resp.data(), &respLen));
    JUB_VERIFY_COS_ERROR(ret);

    // retrieve transaction
    auto txLen  = TW::decode16BE(resp.data());
    auto tx     = TW::Data(txLen);
    auto p      = tx.data();
    auto offset = JUB_UINT16{0};

    constexpr JUB_ULONG kSendOnceLen = 230;
    while (txLen != 0) {
        auto pack = std::min((JUB_ULONG)txLen, kSendOnceLen);

        apdu = APDU{0x00, 0xF9, (JUB_ULONG)(offset >> 8), (JUB_ULONG)(offset & 0xff), 0x00, nullptr, pack};
        JUB_VERIFY_RV(_SendApdu(&apdu, ret, p, &pack));
        if (ret != 0x9000) {
            return JUBR_TRANSMIT_DEVICE_ERROR;
        }
        p += pack;
        offset += pack;
        txLen -= pack;
    }

    raw = tx;
    return JUBR_OK;
}

} // namespace token
} // namespace jub
