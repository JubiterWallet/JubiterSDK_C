/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "token/TrezorCryptoImpl.h"

namespace jub {
    
TrezorCryptoImpl::TrezorCryptoImpl(std::string masterkey_XPRV){
    
    // chd here
    return;
}
    
JUB_RV TrezorCryptoImpl::ConnectToken() {
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::DisconnectToken() {
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::SelectAppletBTC(){
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::GetHDNodeBTC(JUB_BTC_TRANS_TYPE type, std::string path, std::string& xpub){
    return JUBR_OK;
}
JUB_RV TrezorCryptoImpl::GetAddressBTC(JUB_BTC_TRANS_TYPE type, std::string path, JUB_UINT16 tag, std::string& address){
    return JUBR_OK;
}
JUB_RV TrezorCryptoImpl::SetUnitBTC(JUB_BTC_UNIT_TYPE unit){

    return JUBR_OK;
}
JUB_RV TrezorCryptoImpl::SetCoinTypeBTC(JUB_ENUM_COINTYPE_BTC type){
    return JUBR_OK;
}
JUB_RV TrezorCryptoImpl::SignTXBTC(JUB_BTC_TRANS_TYPE type,
                             JUB_UINT16 inputCount,
                             std::vector<JUB_UINT64> vInputAmount,
                             std::vector<std::string> vInputPath,
                             std::vector<JUB_UINT16> vChangeIndex,
                             std::vector<std::string> vChangePath,
                             std::vector<JUB_BYTE> vUnsigedTrans,
                             std::vector<JUB_BYTE>& vRaw){
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::SelectAppletETH(){
    return JUBR_OK;
}
JUB_RV TrezorCryptoImpl::GetAddressETH(std::string path, JUB_UINT16 tag, std::string& address){
    return JUBR_OK;
}
JUB_RV TrezorCryptoImpl::GetHDNodeETH(JUB_BYTE format,std::string path, std::string& pubkey){
    return JUBR_OK;
}
JUB_RV TrezorCryptoImpl::SignTXETH(bool bERC20,
                         std::vector<JUB_BYTE> vNonce,
                         std::vector<JUB_BYTE> vGasPrice,
                         std::vector<JUB_BYTE> vGasLimit,
                         std::vector<JUB_BYTE> vTo,
                         std::vector<JUB_BYTE> vValue,
                         std::vector<JUB_BYTE> vData,
                         std::vector<JUB_BYTE> vPath,
                         std::vector<JUB_BYTE> vChainID,
                         std::vector<JUB_BYTE>& vRaw){
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::QueryBattery(JUB_BYTE &percent){
    return JUBR_OK;
}
JUB_RV TrezorCryptoImpl::ShowVirtualPwd(){
    return JUBR_OK;
}
JUB_RV TrezorCryptoImpl::CancelVirtualPwd(){
    return JUBR_OK;
}
 bool   TrezorCryptoImpl::IsInitialize(){
    return true;
 }
 bool   TrezorCryptoImpl::IsBootLoader(){
    return false;
 }
JUB_RV TrezorCryptoImpl::GetSN(JUB_BYTE sn[24]){
    return JUBR_OK;
}
JUB_RV TrezorCryptoImpl::GetLabel(JUB_BYTE label[32]){
    return JUBR_OK;
}
JUB_RV TrezorCryptoImpl::GetPinRetry(JUB_BYTE& retry){
    return JUBR_OK;
}
JUB_RV TrezorCryptoImpl::GetPinMaxRetry(JUB_BYTE& maxRetry){
    return JUBR_OK;
}
JUB_RV TrezorCryptoImpl::GetBleVersion(JUB_BYTE bleVersion[4]){
    return JUBR_OK;
}
JUB_RV TrezorCryptoImpl::GetFwVersion(JUB_BYTE fwVersion[4]){
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::EnumApplet(std::string& appletList){
    return JUBR_OK;
}
JUB_RV TrezorCryptoImpl::GetAppletVersion(std::string appID, std::string& version){
    return JUBR_OK;
}
JUB_RV TrezorCryptoImpl::EnumSupportCoins(std::string& coinList){
    coinList = "BTC LTC BCH ETH USDT ";
    return JUBR_OK;
}
JUB_RV TrezorCryptoImpl::GetDeviceCert(std::string& cert){
    return JUBR_OK;
}
JUB_RV TrezorCryptoImpl::SendOneApdu(const std::string& apdu, std::string& response){
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::VerifyPIN(const std::string &pinMix, OUT JUB_ULONG &retry){
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::SetTimeout(JUB_UINT16 timeout){
    return JUBR_OK;
}
    
    
}