#include <context/ContextETH.h>
#include <token/interface/TokenInterface.hpp>
#include <utility/util.hpp>
#include <utility/uchar_vector.h>
#include <libETH/ByteConverter.h>
#include <libETH/ERC20Abi.h>

namespace jub {

JUB_RV ContextETH::ActiveSelf() {

    auto token = jub::TokenManager::GetInstance()->GetOne(_deviceID);
    JUB_CHECK_NULL(token);

    JUB_VERIFY_RV(token->SelectAppletETH());
    JUB_VERIFY_RV(token->SetTimeout(_timeout));

    //ETH don`t set unit
    return JUBR_OK;
}

JUB_RV ContextETH::GetAddress(BIP32_Path path, JUB_UINT16 tag, std::string& address) {

    auto token = jub::TokenManager::GetInstance()->GetOne(_deviceID);
    JUB_CHECK_NULL(token);

    std::string strPath = _FullBip32Path(path);
    JUB_VERIFY_RV(token->GetAddressETH(strPath, tag, address));

    return JUBR_OK;
}

JUB_RV ContextETH::GetMainHDNode(JUB_BYTE format, std::string& xpub) {

    auto token = jub::TokenManager::GetInstance()->GetOne(_deviceID);
    JUB_CHECK_NULL(token);

    JUB_VERIFY_RV(token->GetHDNodeETH(format, _mainPath, xpub));

    return JUBR_OK;
}

JUB_RV ContextETH::SetMyAddress(BIP32_Path path, std::string& address) {

    auto token = jub::TokenManager::GetInstance()->GetOne(_deviceID);
    JUB_CHECK_NULL(token);

    std::string strPath = _FullBip32Path(path);
    JUB_VERIFY_RV(token->GetAddressETH(strPath, 0x02, address));

    return JUBR_OK;
}

JUB_RV ContextETH::GetHDNode(JUB_BYTE format, BIP32_Path path, std::string& pubkey) {

    auto token = jub::TokenManager::GetInstance()->GetOne(_deviceID);
    JUB_CHECK_NULL(token);

    std::string strPath = _FullBip32Path(path);
    JUB_VERIFY_RV(token->GetHDNodeETH(format, strPath, pubkey));

    return JUBR_OK;
}

JUB_RV ContextETH::SignTransaction(IN BIP32_Path path,
                                   IN JUB_UINT32 nonce,
                                   IN JUB_UINT32 gasLimit,
                                   IN JUB_CHAR_PTR gasPriceInWei,
                                   IN JUB_CHAR_PTR to,
                                   IN JUB_CHAR_PTR valueInWei,
                                   IN JUB_CHAR_PTR input,
                                   OUT std::string& strRaw) {

    JUB_CHECK_NULL(gasPriceInWei);
    JUB_CHECK_NULL(to);
    JUB_CHECK_NULL(valueInWei);
    JUB_CHECK_NULL(input);

    auto token = jub::TokenManager::GetInstance()->GetOne(_deviceID);
    JUB_CHECK_NULL(token);

    std::vector<JUB_BYTE> vNonce = ByteConverter::numberToBytes(nonce);
    std::vector<JUB_BYTE> vGasLimit = ByteConverter::numberToBytes(gasLimit);
    std::vector<JUB_BYTE> vGasPriceInWei = ByteConverter::stringToBytes(ByteConverter::DecStringToHexString(gasPriceInWei));
    std::vector<JUB_BYTE> vTo = ByteConverter::stringToBytes(to);
    std::vector<JUB_BYTE> vValueInWei;
    if (nullptr != valueInWei
        &&    0 != strlen(valueInWei)
        ) {
        vValueInWei = ByteConverter::stringToBytes(ByteConverter::DecStringToHexString(valueInWei));
    }

    std::vector<JUB_BYTE> vInput;
    if (nullptr != input
        &&    0 != strlen(input)
        ) {
        vInput = ByteConverter::stringToBytes(input);
    }

    std::string strPath = _FullBip32Path(path);
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

    std::vector<JUB_BYTE> vTo = ByteConverter::stringToBytes(to);
    std::vector<JUB_BYTE> vValue = ByteConverter::stringToBytes(ByteConverter::DecStringToHexString(value));
    uchar_vector vAbi = jub::eth::ERC20Abi::serialize(vTo, vValue);
    abi = std::string(ETH_PRDFIX) + vAbi.getHex();

    return JUBR_OK;
}

} // namespace jub end
