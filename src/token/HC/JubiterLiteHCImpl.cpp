#include "token/HC/JubiterLiteHCImpl.h"
#include "JUB_SDK_COMM.h"
#include <TrezorCrypto/base58.h>
#include <string>
#include <vector>

namespace jub {
namespace token {

JUB_RV JubiterLiteHCImpl::SignTX(const JUB_BYTE addrFmt, const JUB_ENUM_BTC_TRANS_TYPE &type,
                                 const JUB_UINT16 inputCount, const std::vector<JUB_UINT64> &vInputAmount,
                                 const std::vector<std::string> &vInputPath,
                                 const std::vector<JUB_UINT16> &vChangeIndex,
                                 const std::vector<std::string> &vChangePath,
                                 const std::vector<JUB_BYTE> &vUnsigedTrans, std::vector<JUB_BYTE> &vRaw,
                                 const TWCoinType &coinNet) {

    auto witness = type == p2sh_p2wpkh;

    TW::Hcash::Transaction tx;
    if (!tx.decode(!witness, vUnsigedTrans)) {
        return JUBR_ARGUMENTS_BAD;
    }

    std::vector<TW::Data> vInputPublicKey;
    std::vector<uchar_vector> vSignatureRaw;
    JUB_VERIFY_RV(_SignTx(!witness, type, vInputAmount, vInputPath, vChangeIndex, vChangePath, tx, vInputPublicKey,
                          vSignatureRaw, coinNet));

    uchar_vector signedRaw;
    JUB_VERIFY_RV(_serializeTx(type, vInputAmount, vInputPublicKey, vSignatureRaw, &tx, signedRaw));

    vRaw = signedRaw;

    return JUBR_OK;
}

} // namespace token
} // namespace jub
