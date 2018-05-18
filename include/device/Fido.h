//
// Created by zhaojifeng on 2018/4/17.
//

#ifndef JUBITER_FIDO_FIDO_H
#define JUBITER_FIDO_FIDO_H

#include <stdio.h>
#include <vector>
#include <string>
#include <JUB_SDK.h>


#define ERR_INVALID_CMD   0x01; //The command in the request is unknown/invalid
#define ERR_INVALID_PAR   0x02; //The parameter(s) of the command is/are invalid or missing
#define ERR_INVALID_LEN   0x03; //The length of the request is invalid
#define ERR_INVALID_SEQ   0x04; //The sequence number is invalid
#define ERR_REQ_TIMEOUT   0x05; //The request timed out
#define ERR_OTHER         0x7f; //Other, unspecified error


// 协议——协议头长度
#define FIDO_HEADER_LEN                 3
#define FIDO_DATA_BLOCK_SIZE            19
#define FIDO_BLOCK_SIZE                 20


typedef std::vector<unsigned char> MSGTYPE;

typedef struct {
    std::string  name;
    std::string  uuid;
    JUB_ULONG    devType;
    unsigned int ulHeader;
} slotInfo;


class Fido
{
public:
    friend Fido* GetFido();

    virtual ~Fido();

    Fido();

public:

    virtual unsigned int finalize();

    virtual unsigned int clear();

    virtual unsigned int sendAPDU (
            unsigned long  devHandle,
            unsigned char* bOrder,
            unsigned int   orderLen,
            unsigned char* bResponse,
            unsigned int*  responseLen,
            unsigned int   timeout = 2000
    );

    static int RecvCallBack (
            unsigned long  devHandle,
            unsigned char* data,
            unsigned int   dataLen
    );

    virtual bool getRecv (
            /**< recvMsg */
            MSGTYPE* vRecv
    );

    virtual unsigned int sendToDev (
            unsigned long  devHandle,
            unsigned char* pSendMsg,
            unsigned int   ulSendMsgLen,
            unsigned char* pRecvMsg,
            unsigned int*  pulRecvLen,
            unsigned int   timeout = 2000
    );

    virtual unsigned int basicApduAddHeadAndLength(
            unsigned char paramHeader,
            unsigned char* pSendMsg,
            unsigned int   ulSendMsgLen,
            unsigned char* pRecvMsg,
            unsigned int*  pulRecvLen

    );

    virtual unsigned int wrapFidoApdu(
            unsigned char paramHeader,
            unsigned char* pSendMsg,
            unsigned int   ulSendMsgLen,
            unsigned char* pRecvMsg,
            unsigned int*  pulRecvLen
    );

    virtual unsigned int parseFidoApdu(
            unsigned char *pSendMsg,
            unsigned int ulSendMsgLen,
            unsigned char *pRecvMsg,
            unsigned int *pulRecvLen
    );

    virtual unsigned int removePkgIndex(
            unsigned char *pSendMsg,
            unsigned int ulSendMsgLen,
            unsigned char *pRecvMsg,
            unsigned int *pulRecvLen
    );

    virtual unsigned int removeCmdHeadAndStateCode(
            unsigned char *pSendMsg,
            unsigned int ulSendMsgLen,
            unsigned char *pRecvMsg,
            unsigned int *pulRecvLen
    );


private:
//    CK_INITPARAM mParam;

    slotInfo     mSlotInfo[100];

    // 互斥锁，设备添加与删除
    pthread_mutex_t      mMutex;
    pthread_mutex_t mSendRecvMutex; /**< sendToDev 互斥锁，防止指令重入 */
    pthread_mutex_t mRecvBufMutex;  /**< 接收数据存储区互斥锁 */


    // 异步消息处理，用于判断是否操时
    struct timeval  mStart;         /**< 指令开始时间 */
    unsigned long   mTimeUsed;      /**< 已耗时 */

    // 接收数据BUG
    MSGTYPE         mRecvMsg;       /**< 接收数据缓冲区 */

    bool  mIsInit;
};

Fido* GetFido();

#endif //JUBITER_FIDO_FIDO_H
