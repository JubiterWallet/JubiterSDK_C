#include "context/BTCContext.h"
#include "token/JubiterBlade/JubiterBladeToken.h"
#include "token/JubiterBIO/JubiterBIOToken.h"
#include "token/JubiterNFC/JubiterNFCToken.h"
#include "token/interface/BTCTokenInterface.hpp"
#include "utility/util.h"


namespace jub {
namespace context {


JUB_RV BTCContext::GetHDNode(const BIP44_Path& path, std::string& xpub) {

    CONTEXT_CHECK_TYPE_PUBLIC

    auto token = std::dynamic_pointer_cast<jub::token::BTCTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(token->GetHDNode(_transType, strPath, xpub));

    return JUBR_OK;
}


JUB_RV BTCContext::GetMainHDNode(std::string& xpub) {

    CONTEXT_CHECK_TYPE_PUBLIC

    auto token = std::dynamic_pointer_cast<jub::token::BTCTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    JUB_VERIFY_RV(token->GetHDNode(_transType, _mainPath, xpub));

    return JUBR_OK;
}


JUB_RV BTCContext::GetAddress(const JUB_ENUM_BTC_ADDRESS_FORMAT& addrFmt, const BIP44_Path& path, const JUB_UINT16 tag, std::string& address) {

    CONTEXT_CHECK_TYPE_PUBLIC

    auto token = std::dynamic_pointer_cast<jub::token::BTCTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(token->GetAddress(addrFmt, _transType, strPath, tag, address));

    return JUBR_OK;
}


JUB_RV BTCContext::SetMyAddress(const JUB_ENUM_BTC_ADDRESS_FORMAT& addrFmt, const BIP44_Path& path, std::string& address) {

    CONTEXT_CHECK_TYPE_PUBLIC

    auto token = std::dynamic_pointer_cast<jub::token::BTCTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(token->GetAddress(addrFmt, _transType, strPath, 0x02, address));

    return JUBR_OK;
}

JUB_RV BTCContext::CheckAddress(const std::string& address) {

    CONTEXT_CHECK_TYPE_NONE

    auto token = std::dynamic_pointer_cast<jub::token::BTCTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    JUB_VERIFY_RV(token->CheckAddress(address));
    return JUBR_OK;
}


JUB_RV BTCContext::SetUnit(const JUB_ENUM_BTC_UNIT_TYPE& unitType) {

    CONTEXT_CHECK_TYPE_NONE

    auto token = std::dynamic_pointer_cast<jub::token::BTCTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    _unitType = unitType;
    JUB_VERIFY_RV(token->SetUnit(_unitType));

    return JUBR_OK;
}


JUB_RV BTCContext::ActiveSelf() {

    JUB_CHECK_NULL(_tokenPtr);

    auto token = std::dynamic_pointer_cast<jub::token::BTCTokenInterface>(_tokenPtr);
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
    if (std::dynamic_pointer_cast<token::JubiterNFCToken>(_tokenPtr)) {
        jub::context::ContextManager::GetInstance()->ClearLast();
    }

    return JUBR_OK;
}


JUB_RV BTCContext::BuildUSDTOutputs(IN JUB_CHAR_CPTR USDTTo, IN JUB_UINT64 amount, OUT OUTPUT_BTC outputs[2]) {

    CONTEXT_CHECK_TYPE_NONE
    //build return0 output
    outputs[0].type = JUB_ENUM_SCRIPT_BTC_TYPE::RETURN0;
    outputs[0].return0.amount = 0;
    outputs[0].return0.dataLen = 20;
    uchar_vector usdtData("6f6d6e69000000000000001f");
    uchar_vector amountBE;
    TW::encode64BE(amount, amountBE);
    usdtData.insert(usdtData.end(), amountBE.begin(), amountBE.end());
    memcpy(outputs[0].return0.data, &usdtData[0], 20);

    //build dust output
    outputs[1].type = JUB_ENUM_SCRIPT_BTC_TYPE::P2PKH;
    outputs[1].stdOutput.address = (JUB_CHAR_PTR)USDTTo;
    outputs[1].stdOutput.amount = 546;
    outputs[1].stdOutput.changeAddress = BOOL_FALSE;

    return JUBR_OK;
}

JUB_RV BTCContext::SignTX(const JUB_ENUM_BTC_ADDRESS_FORMAT& addrFmt, const JUB_UINT32 version, const std::vector<INPUT_BTC>& vInputs, const std::vector<OUTPUT_BTC>& vOutputs, const JUB_UINT32 lockTime, std::string& raw) {

    CONTEXT_CHECK_TYPE_PRIVATE

    auto token = std::dynamic_pointer_cast<jub::token::BTCTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

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
        if (JUB_ENUM_SCRIPT_BTC_TYPE::P2PKH != vOutputs[i].type) {
            continue;
        }
        if (vOutputs[i].stdOutput.changeAddress) {
            vChangeIndex.push_back((JUB_UINT16)i);
            vChangePath.push_back(_FullBip44Path(vOutputs[i].stdOutput.path));
        }
    }

    //build unsigned transaction
    uchar_vector unsignedTrans;
    JUB_VERIFY_RV(token->SerializeUnsignedTx(_transType,
                                             version,
                                             vInputs,
                                             vOutputs,
                                             lockTime,
                                             unsignedTrans));

    uchar_vector vRaw;
    JUB_VERIFY_RV(token->SignTX(addrFmt,
                                _transType,
                                (JUB_UINT16)vInputs.size(),
                                vInputAmount,
                                vInputPath,
                                vChangeIndex,
                                vChangePath,
                                unsignedTrans,
                                vRaw));

    JUB_VERIFY_RV(token->VerifyTX(_transType,
                                  vInputAmount,
                                  vInputPath,
                                  vRaw));

    raw = vRaw.getHex();

    return JUBR_OK;
}


} // namespace context end
} // namespace jub end
