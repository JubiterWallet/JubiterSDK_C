#include <context/BTCContext.h>
#include <token/interface/QTUMTokenInterface.hpp>
#include "Ethereum/ERC20Abi.h"
#include <TrezorCrypto/base58.h>
#include <Bitcoin/Address.h>
#include "utility/util.h"


namespace jub {
namespace context {


JUB_RV QTUMContext::SetQRC20Token(IN JUB_CHAR_CPTR contractAddress, IN JUB_UINT8 decimal, IN JUB_CHAR_CPTR symbol) {

    CONTEXT_CHECK_TYPE_PRIVATE
    //use ETHTokenInterface may case error later. JubiterBLD has no problem.
//    auto token = dynamic_cast<ETHTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
//    JUB_CHECK_NULL(token);
//
//    std::string tokenName = std::string(symbol);
//    std::string _contractAddress = std::string(ETH_PRDFIX) + contractAddress;
//    JUB_VERIFY_RV(token->SetERC20ETHToken(tokenName,
//        decimal,
//        _contractAddress));

    std::string tokenName = std::string(symbol);
    std::string _contractAddress = std::string(ETH_PRDFIX) + contractAddress;

    auto token = std::dynamic_pointer_cast<token::QTUMTokenInterface>(_tokenPtr);
    JUB_CHECK_NULL(token);
    JUB_VERIFY_RV(token->SetQRC20ETHToken(tokenName,
                                          decimal,
                                          contractAddress));

    return JUBR_OK;
}

void encodeInteger(JUB_UINT64 value, uchar_vector& data) {
    uchar_vector d;
    if (0xFF > value) {
        d << (uint8_t)value;
    }
    else if (   0xFF   < value
             && 0xFFFF > value
             ) {
        d << (uint16_t)value;
    }
    else if (   0xFFFF     < value
             && 0xFFFFFFFF > value
             ) {
        d << (uint64_t)value;
    }

    // remove 0x00
    for (uchar_vector::iterator it=d.begin(); it!=d.end(); ++it) {
        if (0x00 == (*it)) {
            break;
        }
        data.push_back(*it);
    }
}

JUB_RV QTUMContext::BuildQRC20Outputs(JUB_UINT64 gasLimit, JUB_UINT64 gasPrice, IN JUB_CHAR_CPTR contractAddress, JUB_CHAR_CPTR to, JUB_CHAR_CPTR value, OUT OUTPUT_BTC outputs[1]) {

    CONTEXT_CHECK_TYPE_NONE
    outputs[0].type = JUB_ENUM_SCRIPT_BTC_TYPE::QRC20;

    uchar_vector data;

    // 4 250000 40
    // 4
    uchar_vector vFour;
    encodeInteger(0x04, vFour);
    data << (JUB_UINT8)vFour.size();
    data << vFour;
    // 250000
    uchar_vector vGasLimit;
    encodeInteger(gasLimit, vGasLimit);
    data << (JUB_UINT8)vGasLimit.size();
    data << vGasLimit;
    // 40
    uchar_vector vGasPrice;
    encodeInteger(gasPrice, vGasPrice);
    data << (JUB_UINT8)vGasPrice.size();
    data << vGasPrice;

    TW::Bitcoin::Address contractAddr((std::string(contractAddress)));
    TW::Data vContractAddress;
    vContractAddress.insert(vContractAddress.begin(), contractAddr.bytes.begin(), contractAddr.bytes.end());
    vContractAddress.erase(vContractAddress.begin());

    TW::Bitcoin::Address       toAddr((std::string(to)));
    TW::Data vToAddress;
    vToAddress.insert(vToAddress.begin(), toAddr.bytes.begin(), toAddr.bytes.end());
    vToAddress.erase(vToAddress.begin());

    std::vector<JUB_BYTE> vValue = jub::HexStr2CharPtr(DecStringToHexString(std::string(value)));
    uchar_vector vAbi = jub::eth::ERC20Abi::serialize(vToAddress, vValue);

    data && vAbi;
    data && vContractAddress;
    data << (JUB_UINT8)0xc2;//OP_CALL

    //build qrc20 here
    outputs[0].qrc20.dataLen = data.size();
    memcpy(outputs[0].qrc20.data, &data[0], data.size());

    return JUBR_OK;
}


} // namespace context end
} // namespace jub end
