#include "utility/util.h"

#include "context/ETHContext.h"
#include "token/interface/ETHTokenInterface.hpp"
#include <token/interface/HardwareTokenInterface.hpp>
#include "Ethereum/ERC20Abi.h"

namespace jub {
namespace context {


JUB_RV ETHContext::ActiveSelf() {

    JUB_VERIFY_RV(_tokenPtr->SelectApplet());
    JUB_VERIFY_RV(_tokenPtr->GetAppletVersion(_appletVersion));
    auto token = std::dynamic_pointer_cast<token::HardwareTokenInterface>(_tokenPtr);
    if (token) {  JUB_VERIFY_RV(SetTimeout(_timeout)); }

    //ETH don`t set unit
    return JUBR_OK;
}


JUB_RV ETHContext::GetAddress(const BIP44_Path& path, const JUB_UINT16 tag, std::string& address) {

    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(_tokenPtr->GetAddress(strPath, tag, address));

    return JUBR_OK;
}


JUB_RV ETHContext::GetMainHDNode(const JUB_BYTE format, std::string& xpub) {

    JUB_VERIFY_RV(_tokenPtr->GetHDNode(format, _mainPath, xpub));

    return JUBR_OK;
}


JUB_RV ETHContext::SetMyAddress(const BIP44_Path& path, std::string& address) {

    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(_tokenPtr->GetAddress(strPath, 0x02, address));

    return JUBR_OK;
}


JUB_RV ETHContext::GetHDNode(const JUB_BYTE format, const BIP44_Path& path, std::string& pubkey) {

    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(_tokenPtr->GetHDNode(format, strPath, pubkey));

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
        && 0 != strlen(input)
        ) {
        vInput = jub::ETHHexStr2CharPtr(input);
    }

    std::string strPath = _FullBip44Path(path);
    std::vector<JUB_BYTE> vPath(strPath.begin(), strPath.end());

    std::vector<JUB_BYTE> vChainID;
    vChainID.push_back(_chainID);

    bool bERC20 = false;
    if (0 == memcmp(uchar_vector(vInput).getHex().c_str(), ABI_METHOD_ID_TRANSFER, strlen(ABI_METHOD_ID_TRANSFER))) { // erc20 function sign
        bERC20 = true;
    }

    uchar_vector raw;
    JUB_VERIFY_RV(_tokenPtr->SignTX(bERC20,
                                    vNonce,
                                    vGasPriceInWei,
                                    vGasLimit,
                                    vTo,
                                    vValueInWei,
                                    vInput,
                                    vPath,
                                    vChainID,
                                    raw));
    strRaw = std::string(ETH_PRDFIX) + raw.getHex();

    return JUBR_OK;
}


JUB_RV ETHContext::BuildERC20Abi(JUB_CHAR_CPTR to, JUB_CHAR_CPTR value, std::string& abi) {

    std::vector<JUB_BYTE> vTo = jub::ETHHexStr2CharPtr(to);
    std::vector<JUB_BYTE> vValue = jub::HexStr2CharPtr(DecStringToHexString(std::string(value)));
    uchar_vector vAbi = jub::eth::ERC20Abi::serialize(vTo, vValue);
    abi = std::string(ETH_PRDFIX) + vAbi.getHex();

    return JUBR_OK;
}


JUB_RV ETHContext::SetERC20ETHToken(JUB_CHAR_CPTR pTokenName,
                                    JUB_UINT16 unitDP,
                                    JUB_CHAR_CPTR pContractAddress) {

    // ETH token extension apdu
    if (0 > _appletVersion.compare(APPLET_VERSION_SUPPORT_EXT_TOKEN)) {
        return JUBR_OK;
    }

    JUB_CHECK_NULL(pTokenName);
    JUB_CHECK_NULL(pContractAddress);

    std::string tokenName = std::string(pTokenName);
    std::string contractAddress = std::string(pContractAddress);
    JUB_VERIFY_RV(_tokenPtr->SetERC20ETHToken(tokenName,
                                              unitDP,
                                              contractAddress));

    return JUBR_OK;
}


} // namespace context end
} // namespace jub end
