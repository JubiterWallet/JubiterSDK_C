//
//  FTNFCTransmission.h
//  FTNFCTransmission
//
//  Created by whl on 2020/1/17.
//  Copyright © 2020 whl. All rights reserved.
//

#ifndef __FTNFCTransmission__
#define __FTNFCTransmission__

//通讯库对外设备类型
#define DEV_TYPE_NFC                4   /**< NFC设备 */

typedef void (*NFC_AUX_ScanFuncCallBack)(
    unsigned int errorCode,  /* 错误码 */
    const char* uuid,        /* tag uuid */
    unsigned int    devType  /* 设备类型 */
);

typedef struct {
    NFC_AUX_ScanFuncCallBack     scanFuncCallBack;        /* scan callback */
}__attribute__ ((packed)) NFC_AUX_INIT_PARAM;

/**
 *  获取通讯库版本
 *
 *  @return 通讯库版本字符串(年.月.日.版本号)
 */
const char* getLibVersion(void);

/**
 *  模块初始化函数
 *
 *  @return 错误码
*/
unsigned int  FTInitializeNFC(NFC_AUX_INIT_PARAM* initPara);

/**
 *  NFC连接函数
 *
 *  @pragma  pcuuid 设备uuid
 *  @pragma pulHandle 设备句柄
 *  @return 错误码
*/
unsigned int FTConnectNFC(const char* pcuuid, unsigned long* pulHandle);

/**
 *  NFC断开连接函数
 *
 *  @pragma  ulHandle 设备句柄
 *  @return 错误码
*/
unsigned int FTDisconnectNFC(unsigned long ulHandle);

/**
 *  查询连接状态
 *
 *  @pragma  ulHandle 设备句柄
 *  @return 错误码
*/
bool FTIsConnectedNFC(unsigned long ulHandle);

/**
 *  NFC收发数据函数
 *
 *  @pragma  ulHandle  设备句柄
 *  @pragma sendBuf    发送数据缓冲区
 *  @pragma sendLen    发送数据长度
 *  @pragma recvBuf      接收数据缓冲区
 *  @pragma recvLen     接收数据长度
 *  @return 错误码
*/
unsigned int FTTransmitNFC(unsigned long ulHandle, unsigned char* sendBuf,unsigned int sendLen,unsigned char * recvBuf,unsigned int* recvLen,int timeoutSec);

/**
 *  设置会话提示信息
 *
 *  @pragma  msg  提示信息内容
 *  @return 错误码
*/
unsigned int FTSetAlertMessageNFC(char *msg);

#endif /* defined(__FTNFCTransmission__) */
