//
//  JubiterNFCDevice.hpp
//  JubSDK
//
//  Created by Pan Min on 2020/3/27.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#ifndef JubiterNFCDevice_hpp
#define JubiterNFCDevice_hpp

#include <stdio.h>

#include "device/DeviceTypeBase.hpp"
#include "JUB_SDK_DEV.h"

#ifdef NFC_MODE
#include <memory>

#include "nfcTransmit/comm.h"
#include "nfcTransmit/ios/FTMacro.h"

namespace jub {
namespace device {


class JubiterNFCDevice :
public DeviceTypeBase {

public:
     JubiterNFCDevice();
    ~JubiterNFCDevice();

public:
    // for common device
    virtual JUB_RV Connect();
    virtual JUB_RV Disconnect();

    virtual JUB_RV SendData(IN JUB_BYTE_CPTR sendData, IN JUB_ULONG ulSendLen,
                            OUT JUB_BYTE_PTR retData, INOUT JUB_ULONG_PTR pulRetDataLen,
                            IN JUB_ULONG ulMiliSecondTimeout = 1200000);

public:
    // for NFC device
    virtual unsigned int Initialize(const NFC_INIT_PARAM& params);
    virtual unsigned int Finalize();

    virtual unsigned int Scan();
    virtual unsigned int StopScan();

    virtual unsigned int Connect(unsigned char* bUUID,      /**< nfc device UUID */
                                 unsigned long* pdevHandle  /**< output nfc device connect handle */
    );

    virtual unsigned int CancelConnect(unsigned char* bBLEUUID);

    virtual unsigned int Disconnect(unsigned long handle);
    virtual unsigned int IsConnect(unsigned long handle);

    virtual unsigned long GetHandle();
    virtual void SetHandle(unsigned long handle);

    virtual void SetConnectStatuteFalse();

    NFC_INIT_PARAM outerParams;

protected:
    static void NFC_ScanFuncCallBack(unsigned int errorCode,/* 错误码 */
                                     const char*  uuid,     /* tag uuid */
                                     unsigned int devType   /* 设备类型 */
    );
    JUB_RV MatchErrorCode(int error);

    //static std::shared_ptr<jub::JubiterNFCDevice> getThis();

protected:
    /* data */
    NFC_INIT_PARAM _param;
    unsigned long _handle;
    bool _bConnected;
}; // class JubiterNFCDevice end


} // namespace device end
} // namespace jub end

#endif // NFC_MODE
#endif /* JubiterNFCDevice_hpp */
