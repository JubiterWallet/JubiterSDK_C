#include <token/ETH/JubiterBIOETHImpl.h>
#include "token/JubiterBlade/JubiterBladeToken.h"

namespace jub {
namespace token {


JUB_RV JubiterBIOETHImpl::SelectApplet() {

    JUB_VERIFY_RV(JubiterBIOToken::_SelectApp(kPKIAID_ETH, sizeof(kPKIAID_ETH)/sizeof(JUB_BYTE)));
    return JUBR_OK;
}


JUB_RV JubiterBIOETHImpl::SignTX(const int erc,
                                 const std::vector<JUB_BYTE>& vNonce,
                                 const std::vector<JUB_BYTE>& vGasPrice,
                                 const std::vector<JUB_BYTE>& vGasLimit,
                                 const std::vector<JUB_BYTE>& vTo,
                                 const std::vector<JUB_BYTE>& vValue,
                                 const std::vector<JUB_BYTE>& vInput,
                                 const std::vector<JUB_BYTE>& vPath,
                                 const std::vector<JUB_BYTE>& vChainID,
                                 std::vector<JUB_BYTE>& vRaw) {

    // Remove subpackage sending APDU.
    // ETH token extension apdu
    if (JubiterBIOToken::_appletVersion >= stVersionExp::FromString(JubiterBIOToken::ETH_APPLET_VERSION_SUPPORT_EXT_TOKENS)) {
        return _SignTXUpgrade(erc,
                              vNonce,
                              vGasPrice,
                              vGasLimit,
                              vTo,
                              vValue,
                              vInput,
                              vPath,
                              vChainID,
                              vRaw);
    }
    else {
        return _SignTX(erc,
                       vNonce,
                       vGasPrice,
                       vGasLimit,
                       vTo,
                       vValue,
                       vInput,
                       vPath,
                       vChainID,
                       vRaw);
    }
}


JUB_RV JubiterBIOETHImpl::SetERC20ETHTokens(const ERC20_TOKEN_INFO tokens[],
                                            const JUB_UINT16 iCount) {

    // ETH token extension apdu
    if (JubiterBIOToken::_appletVersion >= stVersionExp::FromString(JubiterBIOToken::ETH_APPLET_VERSION_SUPPORT_EXT_TOKENS)) {
        return JubiterBladeETHImpl::SetERC20Tokens(tokens, iCount);
    }
    else if (1 == iCount) {
        return SetERC20ETHToken(tokens[0].tokenName, tokens[0].unitDP, tokens[0].contractAddress);
    }

    return JUBR_ARGUMENTS_BAD;
}


JUB_RV JubiterBIOETHImpl::SetERC20ETHToken(const std::string& tokenName,
                                           const JUB_UINT16 unitDP,
                                           const std::string& contractAddress) {

    // ERC20 token extension apdu
    if (typeid(JubiterBIOETHImpl) == typeid(*this)) {
        if (JubiterBIOToken::_appletVersion < stVersionExp::FromString(JubiterBIOToken::ETH_APPLET_VERSION_SUPPORT_EXT_TOKEN)) {
            return JUBR_OK;
        }
    }

    return JubiterBladeETHImpl::SetERC20Token(tokenName.c_str(), unitDP, contractAddress.c_str());
}


JUB_RV JubiterBIOETHImpl::SetERC721ETHToken(const std::string& tokenName, const std::string& contractAddress) {

    // ERC20 token extension apdu
    if (typeid(JubiterBIOETHImpl) == typeid(*this)) {
        if (JubiterBIOToken::_appletVersion < stVersionExp::FromString(JubiterBIOToken::ETH_APPLET_VERSION_SUPPORT_EXT_TOKEN)) {
            return JUBR_OK;
        }
    }

    return JubiterBladeETHImpl::SetERC721Token(tokenName.c_str(), contractAddress.c_str());
}


} // namespace token end
} // namespace jub end
