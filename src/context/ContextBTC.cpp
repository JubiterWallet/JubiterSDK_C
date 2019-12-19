#include "utility/util.h"

#include "context/ContextBTC.h"
#include "token/interface/TokenInterface.hpp"
#include "libBTC/libBTC.hpp"
#include "libBCH/libBCH.hpp"
#include "libETH/ERC20Abi.h"
#include <TrezorCrypto/base58.h>

namespace jub {

JUB_RV ContextBTC::GetHDNode(const BIP44_Path& path, std::string& xpub) {

    auto token = dynamic_cast<BTCTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);

    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(token->GetHDNodeBTC(_transType, strPath, xpub));

    return JUBR_OK;
}

JUB_RV ContextBTC::GetMainHDNode(std::string& xpub) {

    auto token = dynamic_cast<BTCTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);

    JUB_VERIFY_RV(token->GetHDNodeBTC(_transType, _mainPath, xpub));

    return JUBR_OK;
}

JUB_RV ContextBTC::GetAddress(const JUB_ENUM_BTC_ADDRESS_FORMAT& addrFmt, const BIP44_Path& path, const JUB_UINT16 tag, std::string& address) {

    auto token = dynamic_cast<BTCTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);

    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(token->GetAddressBTC(_RealAddressFormat(addrFmt), _transType, strPath, tag, address));

    return JUBR_OK;
}

JUB_RV ContextBTC::SetMyAddress(const JUB_ENUM_BTC_ADDRESS_FORMAT& addrFmt, const BIP44_Path& path, std::string& address) {

    auto token = dynamic_cast<BTCTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);

    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(token->GetAddressBTC(_RealAddressFormat(addrFmt), _transType, strPath, 0x02, address));

    return JUBR_OK;
}

JUB_RV ContextBTC::SetUnit(const JUB_ENUM_BTC_UNIT_TYPE& unitType) {

    _unitType = unitType;

    auto token = dynamic_cast<BTCTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);

    JUB_VERIFY_RV(token->SetUnitBTC(_unitType));

    return JUBR_OK;
}

JUB_RV ContextBTC::ActiveSelf() {

    auto token  = dynamic_cast<BTCTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    auto ctoken = dynamic_cast<CommonTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);
    JUB_CHECK_NULL(ctoken);
    JUB_VERIFY_RV(token->SelectAppletBTC());
    JUB_VERIFY_RV(ctoken->SetTimeout(_timeout));
    JUB_VERIFY_RV(token->SetUnitBTC(_unitType));
    JUB_VERIFY_RV(token->SetCoinTypeBTC(_coinType));

    return JUBR_OK;
}

JUB_RV ContextBTC::BuildUSDTOutputs(IN JUB_CHAR_CPTR USDTTo, IN JUB_UINT64 amount, OUT OUTPUT_BTC outputs[2]) {

    //build return0 output
    outputs[0].type = JUB_ENUM_SCRIPT_BTC_TYPE::RETURN0;
    outputs[0].return0.amount = 0;
    outputs[0].return0.dataLen = 20;
    uchar_vector usdtData("6f6d6e69000000000000001f");
    uint8_t amountBE[8] = { 0x00, };
    WriteBE64(amountBE, amount);
    usdtData.insert(usdtData.end(), amountBE, amountBE + 8);
    memcpy(outputs[0].return0.data, &usdtData[0], 20);

    //build dust output
    outputs[1].type = JUB_ENUM_SCRIPT_BTC_TYPE::P2PKH;
    outputs[1].stdOutput.address = (JUB_CHAR_PTR)USDTTo;
    outputs[1].stdOutput.amount = 546;
    outputs[1].stdOutput.changeAddress = BOOL_FALSE;

    return JUBR_OK;
}

JUB_RV ContextBTC::SetQRC20Token(IN JUB_CHAR_CPTR contractAddress,IN JUB_UINT8 decimal,IN JUB_CHAR_CPTR symbol){

    //use ETHTokenInterface may case error later. JubiterBLD has no problem.
    auto token = dynamic_cast<ETHTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);

    std::string tokenName = std::string(symbol);
//    abcd::DataChunk vContractAddress;
//    bool rv = base58::DecodeBase58Check(contractAddress, vContractAddress);
//    if (!rv) {
//        JUB_VERIFY_RV(JUBR_ARGUMENTS_BAD);
//    }
//    vContractAddress.erase(vContractAddress.begin());
//       uchar_vector address = contractAddress;

    std::string _contractAddress = std::string(ETH_PRDFIX) + contractAddress;
    JUB_VERIFY_RV(token->SetERC20ETHToken(tokenName,
                                          decimal,
                                          _contractAddress));

    return JUBR_OK;
}

JUB_RV ContextBTC::BuildQRC20Outputs(JUB_UINT64 gasLimit,JUB_UINT64 gasPrice,IN JUB_CHAR_CPTR contractAddress, JUB_CHAR_CPTR to, JUB_CHAR_CPTR value, OUT OUTPUT_BTC outputs[1]){
    outputs[0].type = JUB_ENUM_SCRIPT_BTC_TYPE::QRC20;

    uchar_vector data;

    data << (JUB_UINT8)0x01;
    data << (JUB_UINT8)0x04;
    data << (JUB_UINT8)0x08;
    data << (uint64_t)gasLimit;
    data << (JUB_UINT8)0x08;
    data << (uint64_t)gasPrice;

//    abcd::DataChunk vContractAddress;
//    bool rv = base58::DecodeBase58Check(contractAddress, vContractAddress);
//    if (!rv) {
//        JUB_VERIFY_RV(JUBR_ARGUMENTS_BAD);
//    }
//    vContractAddress.erase(vContractAddress.begin());

    uchar_vector vContractAddress = std::string(contractAddress);

    abcd::DataChunk vToAddress;
    bool rv = false;
    int toAddressLen = (int)strlen(to);
    uint8_t* toAddress = new uint8_t[toAddressLen];
    memset(toAddress, 0x00, toAddressLen);
    toAddressLen = base58_decode_check(to, HasherType::HASHER_SHA2D, toAddress, toAddressLen);
    if (toAddressLen > 0) {
        uchar_vector v(toAddress, toAddressLen);
        vToAddress = v;
        rv = true;
    }
    delete[] toAddress; toAddress = NULL;
    if (!rv) {
        JUB_VERIFY_RV(JUBR_ARGUMENTS_BAD);
    }
    vToAddress.erase(vToAddress.begin());

    std::vector<JUB_BYTE> vValue = jub::HexStr2CharPtr(DecStringToHexString(std::string(value)));
    uchar_vector vAbi = jub::eth::ERC20Abi::serialize(vToAddress, vValue);

    data && vAbi;
    data && vContractAddress;
    data << (JUB_UINT8)0xc2;

    //build qrc20 here
    outputs[0].qrc20.dataLen = data.size();
    memcpy(outputs[0].qrc20.data,&data[0],data.size());

    return JUBR_OK;
}

JUB_RV ContextBTC::SignTX(const JUB_ENUM_BTC_ADDRESS_FORMAT& addrFmt, const std::vector<INPUT_BTC>& vInputs, const std::vector<OUTPUT_BTC>& vOutputs, const JUB_UINT32 lockTime, std::string& raw) {

    auto token = dynamic_cast<BTCTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);

    //JUB_VERIFY_RV(token->setUnit_BTC(_unitType));
    //JUB_VERIFY_RV(token->setTimeout_BTC(_timeout));

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
        if (JUB_ENUM_SCRIPT_BTC_TYPE::P2PKH == vOutputs[i].type) {
            if (vOutputs[i].stdOutput.changeAddress) {
                vChangeIndex.push_back((JUB_UINT16)i);
                vChangePath.push_back(_FullBip44Path(vOutputs[i].stdOutput.path));
            }
        }
    }

    //build unsigned transaction
    uchar_vector unsignedTrans;
    JUB_RV ret = JUBR_OK;

    if (   COINBTC  == _coinType
        || COINLTC  == _coinType
        || COINUSDT == _coinType
        ) { //BTC&LTC
        ret = jub::btc::serializeUnsignedTx(_transType, vInputs, vOutputs, lockTime, unsignedTrans);
    }
    else if (COINBCH == _coinType) { //BCH
        ret = jub::bch::serializeUnsignedTx(_transType, vInputs, vOutputs, lockTime, unsignedTrans);
    }
    else {
        ret = JUBR_IMPL_NOT_SUPPORT;
    }
    JUB_VERIFY_RV(ret);

    uchar_vector vRaw;
    JUB_VERIFY_RV(token->SignTXBTC(_RealAddressFormat(addrFmt),
                                   _transType,
                                   (JUB_UINT16)vInputs.size(),
                                   vInputAmount,
                                   vInputPath,
                                   vChangeIndex,
                                   vChangePath,
                                   unsignedTrans,
                                   vRaw));

    raw = vRaw.getHex();

    return JUBR_OK;
}

} // namespace jub end
