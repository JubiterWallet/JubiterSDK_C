//
//  comm_iOS.cpp
//  FTNFCDemo
//
//  Created by whl on 2020/3/30.
//  Copyright © 2020 whl. All rights reserved.
//

#include "comm.h"
#include <string.h>

#ifdef __ANDROID__
#include "android/FTMacro.h"
#include "android/FTNFCTransmission.h"
#else
#include "ios/FTMacro.h"
#include "ios/FTNFCTransmission.h"
#endif

unsigned int InitializeNFC(const NFC_INIT_PARAM& initParam) {

    NFC_AUX_INIT_PARAM paramTmp;
    memset(&paramTmp, 0, sizeof(paramTmp));
    paramTmp.scanFuncCallBack = initParam.scanCallBack;
#ifdef __ANDROID__
    paramTmp.jvm = initParam.jvm;
    paramTmp.activity = initParam.context;
#endif
    return FTInitializeNFC(&paramTmp);
}


unsigned int ConnectNFC(const char* pcuuid, unsigned long* pulHandle) {

    return FTConnectNFC(pcuuid, pulHandle);
}


unsigned int DisconnectNFC(unsigned long ulHandle) {

    return FTDisconnectNFC(ulHandle);
}


bool IsConnectedNFC(unsigned long ulHandle) {

    return FTIsConnectedNFC(ulHandle);
}


int TransmitNFC(unsigned long ulHandle,
                unsigned char* sendBuf, unsigned int sendLen,
                unsigned char* recvBuf, unsigned int* recvLen,
                int timeoutSec) {

    return FTTransmitNFC(ulHandle,
                         sendBuf, sendLen,
                         recvBuf, recvLen,
                         timeoutSec);
}

/**
 *  设置会话提示信息
 *
 *  @pragma  msg  提示信息内容
 *  @return 错误码
*/
unsigned int SetAlertMessageNFC(char *msg) {
#ifdef __ANDROID__
    return 0;
#else
    return FTSetAlertMessageNFC(msg);
#endif
}

