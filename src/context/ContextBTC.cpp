#include "utility/util.h"

#include "context/ContextBTC.h"
#include "token/interface/TokenInterface.hpp"
#include "libBTC/libBTC.hpp"
#include "libBCH/libBCH.hpp"

namespace jub {

JUB_RV ContextBTC::GetHDNode(BIP32_Path path, std::string& xpub) {

    auto token = dynamic_cast<BTCTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);

    std::string strPath = _FullBip32Path(path);
    JUB_VERIFY_RV(token->GetHDNodeBTC(_transType, strPath, xpub));

    return JUBR_OK;
}

JUB_RV ContextBTC::GetMainHDNode(std::string& xpub) {

    auto token = dynamic_cast<BTCTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);

    JUB_VERIFY_RV(token->GetHDNodeBTC(_transType, _mainPath, xpub));

    return JUBR_OK;
}

JUB_RV ContextBTC::GetAddress(BIP32_Path path, JUB_UINT16 tag, std::string& address) {

    auto token = dynamic_cast<BTCTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);

    std::string strPath = _FullBip32Path(path);
    JUB_VERIFY_RV(token->GetAddressBTC(_transType, strPath, tag, address));

    return JUBR_OK;
}

JUB_RV ContextBTC::SetMyAddress(BIP32_Path path, std::string& address) {

    auto token = dynamic_cast<BTCTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);

    std::string strPath = _FullBip32Path(path);
    JUB_VERIFY_RV(token->GetAddressBTC(_transType, strPath, 0x02, address));

    return JUBR_OK;
}

JUB_RV ContextBTC::SetUnit(JUB_BTC_UNIT_TYPE unitType) {

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

JUB_RV ContextBTC::BuildUSDTOutputs(IN JUB_CHAR_PTR USDTTo, IN JUB_UINT64 amount, OUT OUTPUT_BTC outputs[2]) {

    //build return0 output
    outputs[0].type = OUTPUT_BTC_TYPE::RETURN0;
    outputs[0].outputReturn0.amount = 0;
    outputs[0].outputReturn0.dataLen = 20;
    uchar_vector usdtData("6f6d6e69000000000000001f");
    uint8_t amountBE[8] = { 0x00, };
    WriteBE64(amountBE, amount);
    usdtData.insert(usdtData.end(), amountBE, amountBE + 8);
    memcpy(outputs[0].outputReturn0.data, &usdtData[0], 20);

    //build dust output
    outputs[1].type = OUTPUT_BTC_TYPE::STANDARD;
    outputs[1].outputStandard.address = USDTTo;
    outputs[1].outputStandard.amount = 546;
    outputs[1].outputStandard.changeAddress = BOOL_FALSE;

    return JUBR_OK;
}

JUB_RV ContextBTC::SignTX(std::vector<INPUT_BTC> vInputs, std::vector<OUTPUT_BTC> vOutputs, JUB_UINT32 lockTime, std::string& raw) {

    auto token = dynamic_cast<BTCTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);

    //JUB_VERIFY_RV(token->setUnit_BTC(_unitType));
    //JUB_VERIFY_RV(token->setTimeout_BTC(_timeout));

    //deal inputs
    std::vector<JUB_UINT64> vInputAmount;
    std::vector<std::string> vInputPath;
    for (auto input : vInputs) {
        vInputAmount.push_back(input.amount);
        vInputPath.push_back(_FullBip32Path(input.path));
    }

    //deal outputs
    std::vector<JUB_UINT16> vChangeIndex;
    std::vector<std::string> vChangePath;
    for (std::size_t i = 0, e = vOutputs.size(); i != e; ++i) {
        if (OUTPUT_BTC_TYPE::STANDARD == vOutputs[i].type) {
            if (vOutputs[i].outputStandard.changeAddress) {
                vChangeIndex.push_back((JUB_UINT16)i);
                vChangePath.push_back(_FullBip32Path(vOutputs[i].outputStandard.path));
            }
        }
    }

    //build unsinged transaction
    uchar_vector unsignedTrans;
    JUB_RV ret = JUBR_OK;

    if (   COINBTC  == _coinType
        || COINLTC  == _coinType
        || COINUSDT == _coinType
        ) { //BTC&LTC
        ret = jub::btc::serializeUnsignedTX(_transType, vInputs, vOutputs, lockTime, unsignedTrans);
    }
    else if (COINBCH == _coinType) { //BCH
        ret = jub::bch::serializeUnsignedTX(_transType, vInputs, vOutputs, lockTime, unsignedTrans);
    }
    else {
        return JUBR_IMPL_NOT_SUPPORT;
    }
    if (JUBR_OK != ret) {
        return ret;
    }

    uchar_vector vRaw;
    JUB_VERIFY_RV(token->SignTXBTC(_transType,
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
