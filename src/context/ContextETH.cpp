#include "utility/util.h"

#include "context/ContextETH.h"
#include "token/interface/TokenInterface.hpp"
#include "libETH/ERC20Abi.h"

namespace jub {

JUB_RV ContextETH::ActiveSelf() {

    auto token = dynamic_cast<ETHTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    auto ctoken = dynamic_cast<CommonTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);
    JUB_CHECK_NULL(ctoken);

    JUB_VERIFY_RV(token->SelectAppletETH());
    JUB_VERIFY_RV(ctoken->SetTimeout(_timeout));

    //ETH don`t set unit
    return JUBR_OK;
}

JUB_RV ContextETH::GetAddress(BIP44_Path path, JUB_UINT16 tag, std::string& address) {

    auto token = dynamic_cast<ETHTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);

    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(token->GetAddressETH(strPath, tag, address));

    return JUBR_OK;
}

JUB_RV ContextETH::GetMainHDNode(JUB_BYTE format, std::string& xpub) {

    auto token = dynamic_cast<ETHTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);

    JUB_VERIFY_RV(token->GetHDNodeETH(format, _mainPath, xpub));

    return JUBR_OK;
}

JUB_RV ContextETH::SetMyAddress(BIP44_Path path, std::string& address) {

    auto token = dynamic_cast<ETHTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);

    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(token->GetAddressETH(strPath, 0x02, address));

    return JUBR_OK;
}

JUB_RV ContextETH::GetHDNode(JUB_BYTE format, BIP44_Path path, std::string& pubkey) {

    auto token = dynamic_cast<ETHTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);

    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(token->GetHDNodeETH(format, strPath, pubkey));

    return JUBR_OK;
}

JUB_RV ContextETH::SignTransaction(IN BIP44_Path path,
                                   IN JUB_UINT32 nonce,
                                   IN JUB_UINT32 gasLimit,
                                   IN JUB_CHAR_PTR gasPriceInWei,
                                   IN JUB_CHAR_PTR to,
                                   IN JUB_CHAR_PTR valueInWei,
                                   IN JUB_CHAR_PTR input,
                                   OUT std::string& strRaw) {

    JUB_CHECK_NULL(gasPriceInWei);
    JUB_CHECK_NULL(to);
//    JUB_CHECK_NULL(valueInWei);// it can be nullptr
    JUB_CHECK_NULL(input);

    auto token = dynamic_cast<ETHTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);

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
    }

    std::string strPath = _FullBip44Path(path);
    std::vector<JUB_BYTE> vPath(strPath.begin(), strPath.end());

    std::vector<JUB_BYTE> vChainID;
    vChainID.push_back(_chainID);

    bool bERC20 = false;
    if (0 == memcmp(input, ABI_METHOD_ID_TRANSFER, strlen(ABI_METHOD_ID_TRANSFER))) { // erc20 function sign
        bERC20 = true;
    }

    uchar_vector raw;
    JUB_VERIFY_RV(token->SignTXETH(bERC20,
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

JUB_RV ContextETH::BuildERC20Abi(JUB_CHAR_PTR to, JUB_CHAR_PTR value, std::string& abi) {

    std::vector<JUB_BYTE> vTo = jub::ETHHexStr2CharPtr(to);
    std::vector<JUB_BYTE> vValue = jub::HexStr2CharPtr(DecStringToHexString(std::string(value)));
    uchar_vector vAbi = jub::eth::ERC20Abi::serialize(vTo, vValue);
    abi = std::string(ETH_PRDFIX) + vAbi.getHex();

    return JUBR_OK;
}

} // namespace jub end
