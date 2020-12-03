#include "JUB_SDK_Hcash.h"
#include "context/HCContext.h"
#include "token/JubiterBlade/JubiterBladeToken.h"
#include "token/JubiterBIO/JubiterBIOToken.h"
#include "token/JubiterNFC/JubiterNFCToken.h"
#include "token/interface/BTCTokenInterface.hpp"
#include "utility/util.h"
#include "mSIGNA/stdutils/uchar_vector.h"
#include <TrezorCrypto/ecdsa.h>
#include <TrezorCrypto/secp256k1.h>


namespace jub {
namespace context {


JUB_RV HCContext::ActiveSelf() {

    auto token = std::dynamic_pointer_cast<jub::token::BTCTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    JUB_VERIFY_RV(token->SelectApplet());

    if (   std::dynamic_pointer_cast<token::JubiterBladeToken>(_tokenPtr)
        || std::dynamic_pointer_cast<token::JubiterBIOToken>(_tokenPtr)
        ) {
        BTCContext::SetTimeout(_timeout);
    }

#if defined(NFC_MODE)
    // For NFC devices, the session is cleaned up so that the ActiveSelf() function can be started at every session level operation.
    if (std::dynamic_pointer_cast<token::JubiterNFCToken>(_tokenPtr)) {
        jub::context::ContextManager::GetInstance()->ClearLast();
    }
#endif // #if defined(NFC_MODE) end

    return JUBR_OK;
}


JUB_RV HCContext::signTX(const JUB_ENUM_BTC_ADDRESS_FORMAT& addrFmt, const JUB_UINT32 version, const std::vector<INPUT_HC>& vInputs, const std::vector<OUTPUT_HC>& vOutputs, const std::string& unsignedTrans, std::string& raw) {

    auto token = std::dynamic_pointer_cast<jub::token::BTCTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    CONTEXT_CHECK_TYPE_PRIVATE
    //deal inputs
    std::vector<JUB_UINT64> vInputAmount;
    std::vector<std::string> vInputPath;
    for (auto input : vInputs) {
        vInputAmount.push_back(input.amount);
        vInputPath.push_back(_FullBip44Path(input.path));
    }

    //deal outputs
    std::vector<JUB_UINT16> vChangeIndex;
    std::vector<std::string> vChangePath;
    for (std::size_t i = 0, e = vOutputs.size(); i != e; ++i) {
        if (vOutputs[i].changeAddress) {
            vChangeIndex.push_back((JUB_UINT16)i);
            vChangePath.push_back(_FullBip44Path(vOutputs[i].path));
        }
    }

    uchar_vector vUnsignedTrans(unsignedTrans);

    uchar_vector vRaw;
    JUB_VERIFY_RV(token->SignTX(JUB_ENUM_BTC_ADDRESS_FORMAT::OWN,
                                p2pkh,
                                (JUB_UINT16)vInputs.size(),
                                vInputAmount,
                                vInputPath,
                                vChangeIndex,
                                vChangePath,
                                vUnsignedTrans,
                                vRaw));

#if defined(DEBUG)
    JUB_VERIFY_RV(token->VerifyTX(_transType,
                                  vInputAmount,
                                  vInputPath,
                                  vRaw));
#endif

    raw = vRaw.getHex();

    return JUBR_OK;
}


} // namespace context end
} // namespace jub end
