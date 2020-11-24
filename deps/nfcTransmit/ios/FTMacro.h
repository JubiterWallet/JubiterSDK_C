//
//  FTMacro.h
//  FTNFCTransmission
//
//  Created by whl on 2020/1/17.
//  Copyright © 2020 whl. All rights reserved.
//

#ifndef FTMacro_h
#define FTMacro_h

//错误码
#define CKR_VENDOR_NFC_DEFINED                    0x82000000 //NFC错误码开始

#define  FT_SUCCESS                               0x00000000  //操作成功
#define  FT_UNSUPPORTED_FEATURE                   (CKR_VENDOR_NFC_DEFINED + 1)  //不支持此功能
#define  FT_SECURITY_VIOLATION                    (CKR_VENDOR_NFC_DEFINED + 2)  //安全问题
#define  FT_TAG_CONNECTION_LOST                   (CKR_VENDOR_NFC_DEFINED + 3)  //标签连接丢失
#define  FT_RETRY_EXCEEDED                        (CKR_VENDOR_NFC_DEFINED + 4)  //重连次数过多
#define  FT_TAG_RESPONSE_ERROR                    (CKR_VENDOR_NFC_DEFINED + 5)  //标签响应错误
#define  FT_USER_CANCELED                         (CKR_VENDOR_NFC_DEFINED + 6)  //用户取消会话
#define  FT_SESSION_TIMEOUT                       (CKR_VENDOR_NFC_DEFINED + 7)  //会话时间超时
#define  FT_SESSION_TERMINATED_UNEXPECTEDLY       (CKR_VENDOR_NFC_DEFINED + 8)  //会话意外终止
#define  FT_SYSTEM_IS_BUSY                        (CKR_VENDOR_NFC_DEFINED + 9)  //系统正忙，会话失败
#define  FT_FIRST_NDEF_TAG_READ                   (CKR_VENDOR_NFC_DEFINED + 10)  //读取的第一个NDEF
#define  FT_TAG_CONFIGURATION_INVALID_PARAMTERS   (CKR_VENDOR_NFC_DEFINED + 11)  //标签配置无效参数
#define  FT_PLATFORM_NOT_SUPPORT_NFC              (CKR_VENDOR_NFC_DEFINED + 12)  //系统不支持NFC
#define  FT_NO_NFC                                (CKR_VENDOR_NFC_DEFINED + 13)  //NFC功能没有打开
#define  FT_ALREADY_INITIALIZED                   (CKR_VENDOR_NFC_DEFINED + 14)  //SDK已经初始化
#define  FT_NOT_INITIALIZED                       (CKR_VENDOR_NFC_DEFINED + 15)  //SDK尚未初始化
#define  FT_DEVICE_BUSY                           (CKR_VENDOR_NFC_DEFINED + 16)  //设备忙
#define  FT_INVALID_PARAMETER                     (CKR_VENDOR_NFC_DEFINED + 17)  //参数错误
#define  FT_COMM_ERROR                            (CKR_VENDOR_NFC_DEFINED + 18)  //通讯错误
#define  FT_OPERATION_TIMEOUT                     (CKR_VENDOR_NFC_DEFINED + 19)  //操作超时
#define  FT_SESSION_INVALIDATED                   (CKR_VENDOR_NFC_DEFINED + 20)  //会话终止
#define  FT_RECV_DATA_LEN_ERROR                   (CKR_VENDOR_NFC_DEFINED + 21)  //接收数据长度错误
#define  FT_RECB_DATA_ERROR                       (CKR_VENDOR_NFC_DEFINED + 22)  //接收数据错误
#define  FT_CONNECT_ERROR                         (CKR_VENDOR_NFC_DEFINED + 23)  //连接失败
#define  FT_DISCONNECT_ERROR                      (CKR_VENDOR_NFC_DEFINED + 24)  //断开连接失败
#define  FT_NO_TAG                                (CKR_VENDOR_NFC_DEFINED + 25)  //没有发现tag


#endif /* FTMacro_h */
