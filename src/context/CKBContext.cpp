#include "context/CKBContext.h"
#include "token/JubiterBlade/JubiterBladeToken.h"
#include "token/JubiterBIO/JubiterBIOToken.h"
#include "token/JubiterLite/JubiterLiteToken.h"
#include "token/interface/CKBTokenInterface.hpp"

#include <NervosCKB/Transaction.h>
#include <NervosCKB/WitnessArgs.h>

namespace jub {
namespace context {


JUB_RV CKBContext::ActiveSelf() {

    JUB_CHECK_NULL(_tokenPtr);

    auto token = std::dynamic_pointer_cast<jub::token::CKBTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    JUB_VERIFY_RV(token->SelectApplet());
    if ( std::dynamic_pointer_cast<token::JubiterBladeToken>(_tokenPtr)
        || std::dynamic_pointer_cast<token::JubiterBIOToken>(_tokenPtr)
        ) {
        JUB_VERIFY_RV(SetTimeout(_timeout));
    }
    JUB_VERIFY_RV(token->SetUnit(_unitType));
    JUB_VERIFY_RV(token->SetCoin(_coinType));

    // For NFC devices, the session is cleaned up so that the ActiveSelf() function can be started at every session level operation.
    if (std::dynamic_pointer_cast<token::JubiterLiteToken>(_tokenPtr)) {
        jub::context::ContextManager::GetInstance()->ClearLast();
    }

    return JUBR_OK;
}


JUB_RV CKBContext::GetHDNode(const BIP44_Path& path, std::string& xpub) {

    CONTEXT_CHECK_TYPE_PUBLIC

    auto token = std::dynamic_pointer_cast<jub::token::CKBTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(token->GetHDNode(_transType, strPath, xpub, _coinNet));

    return JUBR_OK;
}


JUB_RV CKBContext::GetMainHDNode(std::string& xpub) {

    CONTEXT_CHECK_TYPE_PUBLIC

    auto token = std::dynamic_pointer_cast<jub::token::CKBTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    JUB_VERIFY_RV(token->GetHDNode(_transType, _mainPath, xpub, _coinNet));

    return JUBR_OK;
}


JUB_RV CKBContext::GetAddress(const JUB_ENUM_BTC_ADDRESS_FORMAT& addrFmt, const BIP44_Path& path, const JUB_UINT16 tag, std::string& address) {

    CONTEXT_CHECK_TYPE_PUBLIC

    auto token = std::dynamic_pointer_cast<jub::token::CKBTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(token->GetAddress(addrFmt, _transType, strPath, tag, address, _coinNet));

    return JUBR_OK;
}


JUB_RV CKBContext::CheckAddress(const std::string& address) {

    CONTEXT_CHECK_TYPE_NONE

    auto token = std::dynamic_pointer_cast<jub::token::CKBTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    JUB_VERIFY_RV(token->CheckAddress(address));

    return JUBR_OK;
}


JUB_RV CKBContext::signTX(const JUB_ENUM_BTC_ADDRESS_FORMAT& addrFmt,
                          const JUB_UINT32 version,
                          const std::vector<CELL_DEP>& vDeps,
                          const std::vector<CELL_INPUT>& vInputs,
                          const std::vector<CELL_OUTPUT>& vOutputs,
                          std::string& rawInJSON) {

    CONTEXT_CHECK_TYPE_PRIVATE

    auto token = std::dynamic_pointer_cast<jub::token::CKBTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    //deal inputs
    std::vector<std::string> vInputPath;
    for (auto input : vInputs) {
        vInputPath.push_back(_FullBip44Path(input.path));
    }

    std::vector<uchar_vector> vSignatureRaw;
    JUB_VERIFY_RV(token->SignTX(_transType,
                                vInputPath,
                                version,
                                vDeps,
                                vInputs,
                                vOutputs,
                                vSignatureRaw,
                                _coinNet));

#if defined(DEBUG)
    JUB_VERIFY_RV(token->VerifyTX(_transType,
                                  vInputPath,
                                  version,
                                  vDeps,
                                  vInputs,
                                  vOutputs,
                                  vSignatureRaw,
                                  _coinNet));
#endif

    // finish transaction
    try {
        TW::Data vUnsignedRaw;
        JUB_VERIFY_RV(token->SerializeUnsignedTx(version, vDeps, vInputs, vOutputs, vUnsignedRaw, _coinNet));

        TW::NervosCKB::Transaction tx;
        if (!tx.deserialize(vUnsignedRaw)) {
            return JUBR_ARGUMENTS_BAD;
        }

        for (size_t index=0; index<vSignatureRaw.size(); ++index) {
            TW::NervosCKB::WitnessArgs arg;
            arg.lock = vSignatureRaw[index];
            tx.witnesses.push_back(arg);
        }

        rawInJSON = tx.serialize().dump();
    }
    catch (...) {
        return JUBR_ERROR;
    }

    return JUBR_OK;
}


} // namespace context end
} // namespace jub end
