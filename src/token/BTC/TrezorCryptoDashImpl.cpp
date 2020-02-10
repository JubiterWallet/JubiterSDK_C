#include <token/BTC/TrezorCryptoDashImpl.h>

namespace jub {
namespace token {


JUB_RV TrezorCryptoDashImpl::SignTX(const JUB_BYTE addrFmt,
                                    const JUB_ENUM_BTC_TRANS_TYPE& type,
                                    const JUB_UINT16 inputCount,
                                    const std::vector<JUB_UINT64>& vInputAmount,
                                    const std::vector<std::string>& vInputPath,
                                    const std::vector<JUB_UINT16>& vChangeIndex,
                                    const std::vector<std::string>& vChangePath,
                                    const std::vector<JUB_BYTE>& vUnsigedTrans,
                                    std::vector<JUB_BYTE>& vRaw) {

    bool witness = false;
    if (p2sh_p2wpkh == type) {
        witness = true;
    }

    TW::Bitcoin::DashTransaction tx;
    if (!tx.decode(witness, vUnsigedTrans)) {
        return JUBR_ARGUMENTS_BAD;
    }

    return _SignTx(witness,
                   vInputAmount,
                   vInputPath,
                   vChangeIndex,
                   vChangePath,
                   tx,
                   vRaw);
}


} // namespace token end
} // namespace jub end
