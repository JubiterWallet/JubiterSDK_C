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
#define  FT_SUCCESS                               0x00000000  //操作成功
#define  FT_UNSUPPORTED_FEATURE                   0x00000001  //不支持此功能
#define  FT_SECURITY_VIOLATION                    0x00000002  //安全问题
#define  FT_TAG_CONNECTION_LOST                   0x00000003  //标签连接丢失
#define  FT_RETRY_EXCEEDED                        0x00000004  //重连次数过多
#define  FT_TAG_RESPONSE_ERROR                    0x00000005  //标签响应错误
#define  FT_USER_CANCELED                         0x00000006  //用户取消会话
#define  FT_SESSION_TIMEOUT                       0x00000007  //会话时间超时
#define  FT_SESSION_TERMINATED_UNEXPECTEDLY       0x00000008  //会话意外终止
#define  FT_SYSTEM_IS_BUSY                        0x00000009  //系统正忙，会话失败
#define  FT_FIRST_NDEF_TAG_READ                   0x0000000A  //读取的第一个NDEF
#define  FT_TAG_CONFIGURATION_INVALID_PARAMTERS   0x0000000B  //标签配置无效参数
#define  FT_PLATFORM_NOT_SUPPORT_NFC              0x0000000C  //系统不支持NFC
#define  FT_NO_NFC                                0x0000000D  //NFC功能没有打开
#define  FT_ALREADY_INITIALIZED                   0x0000000E  //SDK已经初始化
#define  FT_NOT_INITIALIZED                       0x0000000F  //SDK尚未初始化
#define  FT_DEVICE_BUSY                           0x00000010  //设备忙
#define  FT_INVALID_PARAMETER                     0x00000011  //参数错误
#define  FT_COMM_ERROR                            0x00000012  //通讯错误
#define  FT_OPERATION_TIMEOUT                     0x00000013  //操作超时
#define  FT_SESSION_INVALIDATED                   0x00000014  //会话终止
#define  FT_RECV_DATA_LEN_ERROR                   0x00000015  //接收数据长度错误
#define  FT_RECB_DATA_ERROR                       0x00000016  //接收数据错误
#define  FT_CONNECT_ERROR                         0x00000017  //连接失败
#define  FT_DISCONNECT_ERROR                      0x00000018  //断开连接失败
#define  FT_NO_TAG                                0x00000019  //没有发现tag


#endif /* FTMacro_h */
