#include "context/ETHContext.h"
#include "token/JubiterBlade/JubiterBladeToken.h"
#include "token/JubiterBIO/JubiterBIOToken.h"
#include "token/JubiterLite/JubiterLiteToken.h"
#include "token/interface/ETHTokenInterface.hpp"
#include <Ethereum/ERC20Abi.h>
#include "utility/util.h"


namespace jub {
namespace context {


JUB_RV ETHContext::ActiveSelf() {

    auto token = std::dynamic_pointer_cast<jub::token::ETHTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    JUB_VERIFY_RV(token->SelectApplet());
    JUB_VERIFY_RV(token->GetAppletVersion(_appletVersion));
    if (   std::dynamic_pointer_cast<token::JubiterBladeToken>(_tokenPtr)
        || std::dynamic_pointer_cast<token::JubiterBIOToken>(_tokenPtr)
        ) {
        JUB_VERIFY_RV(SetTimeout(_timeout));
    }

#if defined(NFC_MODE)
    // For NFC devices, the session is cleaned up so that the ActiveSelf() function can be started at every session level operation.
    if (std::dynamic_pointer_cast<token::JubiterLiteToken>(_tokenPtr)) {
        jub::context::ContextManager::GetInstance()->ClearLast();
    }
#endif // #if defined(NFC_MODE) end

    //ETH don`t set unit
    return JUBR_OK;
}


JUB_RV ETHContext::GetAddress(const BIP44_Path& path, const JUB_UINT16 tag, std::string& address) {

    CONTEXT_CHECK_TYPE_PUBLIC

    auto token = std::dynamic_pointer_cast<jub::token::ETHTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(token->GetAddress(strPath, tag, address));

    return JUBR_OK;
}


JUB_RV ETHContext::GetMainHDNode(const JUB_BYTE format, std::string& xpub) {

    CONTEXT_CHECK_TYPE_PUBLIC

    auto token = std::dynamic_pointer_cast<jub::token::ETHTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    JUB_VERIFY_RV(token->GetHDNode(format, _mainPath, xpub));

    return JUBR_OK;
}


JUB_RV ETHContext::SetMyAddress(const BIP44_Path& path, std::string& address) {

    CONTEXT_CHECK_TYPE_PUBLIC

    auto token = std::dynamic_pointer_cast<jub::token::ETHTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(token->GetAddress(strPath, 0x02, address));

    return JUBR_OK;
}


JUB_RV ETHContext::GetHDNode(const JUB_BYTE format, const BIP44_Path& path, std::string& pubkey) {

    CONTEXT_CHECK_TYPE_PUBLIC

    auto token = std::dynamic_pointer_cast<jub::token::ETHTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(token->GetHDNode(format, strPath, pubkey));

    return JUBR_OK;
}


JUB_RV ETHContext::SignTransaction(const BIP44_Path& path,
                                   const JUB_UINT32 nonce,
                                   const JUB_UINT32 gasLimit,
                                   JUB_CHAR_CPTR gasPriceInWei,
                                   JUB_CHAR_CPTR to,
                                   JUB_CHAR_CPTR valueInWei,
                                   JUB_CHAR_CPTR input,
                                   OUT std::string& strRaw) {

    CONTEXT_CHECK_TYPE_PRIVATE

    auto token = std::dynamic_pointer_cast<jub::token::ETHTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    JUB_CHECK_NULL(gasPriceInWei);
    JUB_CHECK_NULL(to);
//    JUB_CHECK_NULL(valueInWei);// it can be nullptr
    JUB_CHECK_NULL(input);

    std::vector<JUB_BYTE> vNonce = jub::HexStr2CharPtr(numberToHexString(nonce));
    std::vector<JUB_BYTE> vGasLimit = jub::HexStr2CharPtr(numberToHexString(gasLimit));
    std::vector<JUB_BYTE> vGasPriceInWei = jub::HexStr2CharPtr(DecStringToHexString(std::string(gasPriceInWei)));
    std::vector<JUB_BYTE> vTo = jub::ETHHexStr2CharPtr(to);
    std::vector<JUB_BYTE> vValueInWei;
    if (nullptr != valueInWei
        &&    0 != strlen(valueInWei)
        ) {
        vValueInWei = jub::HexStr2CharPtr(DecStringToHexString(std::string(valueInWei)));
    }

    std::vector<JUB_BYTE> vInput;
    if (nullptr != input
        &&    0 != strlen(input)
        ) {
        vInput = jub::ETHHexStr2CharPtr(input);
        if (0 == vInput.size()) {
            vInput = jub::HexStr2CharPtr(input);
        }
        if (0 == vInput.size()) {
            return JUBR_ARGUMENTS_BAD;
        }
    }

    std::string strPath = _FullBip44Path(path);
    std::vector<JUB_BYTE> vPath(strPath.begin(), strPath.end());

    std::vector<JUB_BYTE> vChainID;
    vChainID.push_back(_chainID);

    bool bERC20 = false;
    if (0 == memcmp(uchar_vector(vInput).getHex().c_str(),
                    ABI_METHOD_ID_TRANSFER, strlen(ABI_METHOD_ID_TRANSFER))
        ) { // erc20 function sign
        bERC20 = true;
    }

    uchar_vector raw;
    JUB_VERIFY_RV(token->SignTX(bERC20,
                                vNonce,
                                vGasPriceInWei,
                                vGasLimit,
                                vTo,
                                vValueInWei,
                                vInput,
                                vPath,
                                vChainID,
                                raw));

#if defined(DEBUG)
    //verify
    JUB_VERIFY_RV(token->VerifyTX(vChainID,
                                  strPath,
                                  raw));
#endif

    strRaw = std::string(ETH_PRDFIX) + raw.getHex();

    return JUBR_OK;
}


JUB_RV ETHContext::SignContract(const BIP44_Path& path,
                                const JUB_UINT32 nonce,
                                const JUB_UINT32 gasLimit,
                                JUB_CHAR_CPTR gasPriceInWei,
                                JUB_CHAR_CPTR to,
                                IN JUB_CHAR_CPTR valueInWei,
                                IN JUB_CHAR_CPTR input,
                                OUT std::string& strRaw) {

    CONTEXT_CHECK_TYPE_PRIVATE

    auto token = std::dynamic_pointer_cast<jub::token::ETHTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    JUB_CHECK_NULL(gasPriceInWei);
    JUB_CHECK_NULL(to);
//    JUB_CHECK_NULL(valueInWei);// it can be nullptr
    JUB_CHECK_NULL(input);

    std::vector<JUB_BYTE> vNonce = jub::HexStr2CharPtr(numberToHexString(nonce));
    std::vector<JUB_BYTE> vGasLimit = jub::HexStr2CharPtr(numberToHexString(gasLimit));
    std::vector<JUB_BYTE> vGasPriceInWei = jub::HexStr2CharPtr(DecStringToHexString(std::string(gasPriceInWei)));
    std::vector<JUB_BYTE> vTo = jub::ETHHexStr2CharPtr(to);
    std::vector<JUB_BYTE> vValueInWei;
    if (nullptr != valueInWei
        &&    0 != strlen(valueInWei)
        ) {
        vValueInWei = jub::HexStr2CharPtr(DecStringToHexString(std::string(valueInWei)));
    }

    std::vector<JUB_BYTE> vInput;
    if (nullptr != input
        &&    0 != strlen(input)
        ) {
        vInput = jub::ETHHexStr2CharPtr(input);
        if (0 == vInput.size()) {
            vInput = jub::HexStr2CharPtr(input);
        }
        if (0 == vInput.size()) {
            return JUBR_ARGUMENTS_BAD;
        }
    }

    std::string strPath = _FullBip44Path(path);
    std::vector<JUB_BYTE> vPath(strPath.begin(), strPath.end());

    std::vector<JUB_BYTE> vChainID;
    vChainID.push_back(_chainID);

    bool bERC20 = false;
    if (0 == memcmp(uchar_vector(vInput).getHex().c_str(),
                    ABI_METHOD_ID_TRANSFER, strlen(ABI_METHOD_ID_TRANSFER))
        ) { // erc20 function sign
        bERC20 = true;
    }

    uchar_vector raw;
    if (!bERC20) {
        JUB_VERIFY_RV(token->SignContract(jub::token::ENUM_CONTRACT_ABI::NS_ITEM,
                                          vNonce,
                                          vGasPriceInWei,
                                          vGasLimit,
                                          vTo,
                                          vValueInWei,
                                          vInput,
                                          vPath,
                                          vChainID,
                                          raw));
    }
    else {
        JUB_VERIFY_RV(token->SignTX(bERC20,
                                    vNonce,
                                    vGasPriceInWei,
                                    vGasLimit,
                                    vTo,
                                    vValueInWei,
                                    vInput,
                                    vPath,
                                    vChainID,
                                    raw));
    }

#if defined(DEBUG)
    //verify
    JUB_VERIFY_RV(token->VerifyTX(vChainID,
                                  strPath,
                                  raw));
#endif

    strRaw = std::string(ETH_PRDFIX) + raw.getHex();

    return JUBR_OK;
}


JUB_RV ETHContext::BuildERC20Abi(JUB_CHAR_CPTR to, JUB_CHAR_CPTR value, std::string& abi) {

    CONTEXT_CHECK_TYPE_NONE
    std::vector<JUB_BYTE> vTo = jub::ETHHexStr2CharPtr(to);
    std::vector<JUB_BYTE> vValue = jub::HexStr2CharPtr(DecStringToHexString(std::string(value)));
    uchar_vector vAbi = jub::eth::ERC20Abi::serialize(vTo, vValue);
    abi = std::string(ETH_PRDFIX) + vAbi.getHex();

    return JUBR_OK;
}


JUB_RV ETHContext::SetERC20ETHToken(JUB_CHAR_CPTR pTokenName,
                                    JUB_UINT16 unitDP,
                                    JUB_CHAR_CPTR pContractAddress) {

    CONTEXT_CHECK_TYPE_PRIVATE

    auto token = std::dynamic_pointer_cast<jub::token::ETHTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    // ETH token extension apdu
    if (0 > _appletVersion.compare(ETH_APPLET_VERSION_SUPPORT_EXT_TOKEN)) {
        return JUBR_OK;
    }

    JUB_CHECK_NULL(pTokenName);
    JUB_CHECK_NULL(pContractAddress);

    std::string tokenName = std::string(pTokenName);
    std::string contractAddress = uchar_vector(ETHHexStr2CharPtr(pContractAddress)).getHex();
    JUB_VERIFY_RV(token->SetERC20ETHToken(tokenName,
                                          unitDP,
                                          contractAddress));

    return JUBR_OK;
}


JUB_RV ETHContext::SignBytestring(const BIP44_Path& path,
                                  JUB_CHAR_CPTR data,
                                  OUT std::string& strSignature) {

    CONTEXT_CHECK_TYPE_PRIVATE

    auto token = std::dynamic_pointer_cast<jub::token::ETHTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    JUB_CHECK_NULL(data);

    std::vector<JUB_BYTE> vTypeData = jub::HexStr2CharPtr(data);
    if (0 >= vTypeData.size()) {
        return JUBR_ARGUMENTS_BAD;
    }

    std::string strPath = _FullBip44Path(path);
    std::vector<JUB_BYTE> vPath(strPath.begin(), strPath.end());

    std::vector<JUB_BYTE> vChainID;
    vChainID.push_back(_chainID);

    uchar_vector vSignature;
    JUB_VERIFY_RV(token->SignBytestring(vTypeData,
                                        vPath,
                                        vChainID,
                                        vSignature));
#if defined(DEBUG)
    //verify
    JUB_VERIFY_RV(token->VerifyBytestring(vChainID,
                                          strPath,
                                          vTypeData,
                                          vSignature));
#endif

    strSignature = std::string(ETH_PRDFIX) + vSignature.getHex();

    return JUBR_OK;
}


} // namespace context end
} // namespace jub end
