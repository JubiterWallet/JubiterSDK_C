//
//  comm.h
//  FTNFCDemo
//
//  Created by whl on 2020/3/30.
//  Copyright © 2020 whl. All rights reserved.
//

#ifndef comm_h
#define comm_h

//通讯库对外设备类型
#define DEV_TYPE_NFC                4   /**< NFC设备 */

typedef void (*NFC_ScanFuncCallBack)(
    unsigned int errorCode,  /* 错误码 */
    const char* uuid,        /* tag uuid */
    unsigned int devType     /* 设备类型 */
);

typedef struct {
    void* context;                                /* android activity */
    NFC_ScanFuncCallBack     scanCallBack;        /* scan callback */
}__attribute__ ((packed)) NFC_INIT_PARAM;


/**
 *  模块初始化函数
 *
 *  @return 错误码
*/
unsigned int InitializeNFC(const NFC_INIT_PARAM& initParam);


/**
 *  NFC连接函数
 *
 *  @pragma  pcuuid 设备uuid
 *  @pragma pulHandle 设备句柄
 *  @return 错误码
*/
unsigned int ConnectNFC(const char* pcuuid, unsigned long* pulHandle);


/**
 *  NFC断开连接函数
 *
 *  @pragma  ulHandle 设备句柄
 *  @return 错误码
*/
unsigned int DisconnectNFC(unsigned long ulHandle);


/**
 *  查询连接状态
 *
 *  @pragma  ulHandle 设备句柄
 *  @return 错误码
*/
bool IsConnectedNFC(unsigned long ulHandle);


/**
 *  NFC断开连接函数
 *
 *  @pragma  ulHandle 设备句柄
 *  @pragma sendBuf    发送数据缓冲区
 *  @pragma sendLen    发送数据长度
 *  @pragma recvBuf      接收数据缓冲区
 *  @pragma recvLen     接收数据长度
 *  @return 错误码
*/
int TransmitNFC(unsigned long ulHandle,
                unsigned char* sendBuf, unsigned int sendLen,
                unsigned char* recvBuf, unsigned int* recvLen,
                int timeoutSec);


#endif /* comm_h */
