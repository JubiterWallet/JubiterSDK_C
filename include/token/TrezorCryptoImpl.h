/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TrezorCryptoImpl.h
 * Author: zxc_work
 *
 * Created on 2019年7月19日, 上午10:28
 */

#ifndef TREZORCRYPTOIMPL_H
#define TREZORCRYPTOIMPL_H

#include "token/interface/TokenInterface.hpp" 

namespace jub {

class TrezorCryptoImpl :
    public CommonTokenInterface,
    public BTCTokenInterface,
    public ETHTokenInterface,
    public EOSTokenInterface,
    public XRPTokenInterface
{
public:
    TrezorCryptoImpl(std::string masterkey_XPRV);
    ~TrezorCryptoImpl() = default;

private:
    std::string _MasterKey_XPRV;

public:
    /* functions */
    virtual JUB_RV ConnectToken();
    virtual JUB_RV DisconnectToken();

    //BTC functions
    virtual JUB_RV SelectAppletBTC();
    virtual JUB_RV GetHDNodeBTC(JUB_ENUM_BTC_TRANS_TYPE type, std::string path, std::string& xpub);
    virtual JUB_RV GetAddressBTC(JUB_ENUM_BTC_TRANS_TYPE type, std::string path, JUB_UINT16 tag, std::string& address);
    virtual JUB_RV SetUnitBTC(JUB_ENUM_BTC_UNIT_TYPE unit);
    virtual JUB_RV SetCoinTypeBTC(JUB_ENUM_COINTYPE_BTC type);
    virtual JUB_RV SignTXBTC(JUB_ENUM_BTC_TRANS_TYPE type,
                             JUB_UINT16 inputCount,
                             std::vector<JUB_UINT64> vInputAmount,
                             std::vector<std::string> vInputPath,
                             std::vector<JUB_UINT16> vChangeIndex,
                             std::vector<std::string> vChangePath,
                             std::vector<JUB_BYTE> vUnsigedTrans,
                             std::vector<JUB_BYTE>& vRaw);

    //ETH functions
    virtual JUB_RV SelectAppletETH();
    virtual JUB_RV GetAddressETH(std::string path, JUB_UINT16 tag, std::string& address);
    virtual JUB_RV GetHDNodeETH(JUB_BYTE format,std::string path, std::string& pubkey);
    virtual JUB_RV SignTXETH(bool bERC20,
                             std::vector<JUB_BYTE> vNonce,
                             std::vector<JUB_BYTE> vGasPrice,
                             std::vector<JUB_BYTE> vGasLimit,
                             std::vector<JUB_BYTE> vTo,
                             std::vector<JUB_BYTE> vValue,
                             std::vector<JUB_BYTE> vData,
                             std::vector<JUB_BYTE> vPath,
                             std::vector<JUB_BYTE> vChainID,
                             std::vector<JUB_BYTE>& vRaw);

    //EOS functions
    virtual JUB_RV SelectAppletEOS();
    virtual JUB_RV GetAddressEOS(const TW::EOS::Type& type, const std::string& path, const JUB_UINT16 tag, std::string& address);
    virtual JUB_RV GetHDNodeEOS(const JUB_BYTE format, const std::string& path, std::string& pubkey);
    virtual JUB_RV SignTXEOS(const TW::EOS::Type& type,
                             const std::vector<JUB_BYTE>& vPath,
                             const std::vector<JUB_BYTE>& vChainId,
                             const std::vector<JUB_BYTE>& vRaw,
                             std::vector<uchar_vector>& vSignatureRaw,
                             const bool bWithType=false);
    //XRP functions
    virtual JUB_RV SelectAppletXRP();
    virtual JUB_RV GetAddressXRP(const std::string& path, const JUB_UINT16 tag, std::string& address);
    virtual JUB_RV GetHDNodeXRP(const JUB_BYTE format, const std::string& path, std::string& pubkey);
    virtual JUB_RV SignTXXRP(const std::vector<JUB_BYTE>& vPath,
                             const std::vector<JUB_BYTE>& vUnsignedRaw,
                             std::vector<uchar_vector>& vSignatureRaw);

    //common token functions
    virtual JUB_RV QueryBattery(JUB_BYTE &percent);
    virtual JUB_RV ShowVirtualPwd();
    virtual JUB_RV CancelVirtualPwd();
    virtual bool   IsInitialize();
    virtual bool   IsBootLoader();
    virtual JUB_RV GetSN(JUB_BYTE sn[24]);
    virtual JUB_RV GetLabel(JUB_BYTE label[32]);
    virtual JUB_RV GetPinRetry(JUB_BYTE& retry);
    virtual JUB_RV GetPinMaxRetry(JUB_BYTE& maxRetry);
    virtual JUB_RV GetBleVersion(JUB_BYTE bleVersion[4]);
    virtual JUB_RV GetFwVersion(JUB_BYTE fwVersion[4]);

    virtual JUB_RV EnumApplet(std::string& appletList);
    virtual JUB_RV GetAppletVersion(std::string appID, std::string& version);
    virtual JUB_RV EnumSupportCoins(std::string& coinList);
    virtual JUB_RV GetDeviceCert(std::string& cert);
    virtual JUB_RV SendOneApdu(const std::string& apdu, std::string& response);

    virtual JUB_RV VerifyPIN(const std::string &pinMix, OUT JUB_ULONG &retry);

    virtual JUB_RV SetTimeout(JUB_UINT16 timeout);
};  // class TrezorCryptoImpl end

}   // namespace jub end

#endif /* TREZORCRYPTOIMPL_H */

