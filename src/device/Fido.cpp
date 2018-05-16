//
// Created by zhaojifeng on 2018/4/17.
//
#include <unistd.h>
#include <logUtils.h>
#include <JUB_SDK.h>
#include <bleTransmit/bleTransmit.h>
#include <mutex>
#include <device/Fido.h>


#define __PASTE(x, y)                           x##y
#define ADDPRE(name)                           __PASTE(FT_, name)

#define CTOI(x)   (unsigned int)(*(x) * 0x100           \
                  | (*((x) + 1)))

static std::mutex _mutex;
static Fido *fidoToken = new Fido();

Fido *GetFido() {
    return fidoToken;
}

Fido::Fido() {
    mIsInit = false;

    std::string strNULL;
    for (int i = 0; i < 100; i++) {
        mSlotInfo[i].ulHeader = -1;
        mSlotInfo[i].uuid = strNULL;
        mSlotInfo[i].name = strNULL;
        mSlotInfo[i].devType = 0;
    }
    pthread_mutex_init(&mMutex, 0);
    pthread_mutex_init(&mSendRecvMutex, 0);
    pthread_mutex_init(&mRecvBufMutex, 0);
}

Fido::~Fido() {
    pthread_mutex_lock(&mMutex);
    pthread_mutex_lock(&mSendRecvMutex);
    pthread_mutex_lock(&mRecvBufMutex);


    std::string strNULL;
    for (int i = 0; i < 100; i++) {
        mSlotInfo[i].ulHeader = -1;
        mSlotInfo[i].uuid = strNULL;
        mSlotInfo[i].name = strNULL;
        mSlotInfo[i].devType = 0;
    }
    mIsInit = false;
    ADDPRE(BLE_Finalize)();
    pthread_mutex_destroy(&mMutex);
    pthread_mutex_destroy(&mSendRecvMutex);
    pthread_mutex_destroy(&mRecvBufMutex);
}

unsigned int Fido::finalize() {
    pthread_mutex_lock(&mMutex);
    pthread_mutex_lock(&mSendRecvMutex);
    pthread_mutex_lock(&mRecvBufMutex);
    std::string strNULL;
    for (int i = 0; i < 100; i++) {
        mSlotInfo[i].ulHeader = -1;
        mSlotInfo[i].uuid = strNULL;
        mSlotInfo[i].name = strNULL;
        mSlotInfo[i].devType = 0;
    }
    mIsInit = false;
    ADDPRE(BLE_Finalize)();
    pthread_mutex_destroy(&mMutex);
    pthread_mutex_destroy(&mSendRecvMutex);
    pthread_mutex_destroy(&mRecvBufMutex);
    return 0;
}

unsigned int Fido::clear() {
    mRecvMsg.clear();
    return 0;
}

int Fido::RecvCallBack(
    unsigned long devHandle,
    unsigned char *data,
    unsigned int dataLen
) {

    LOG_BIN("", data, dataLen);
    if (memcmp(data, "0x82", 1) != 0) {//过滤keep live
        pthread_mutex_lock(&(GetFido()->mRecvBufMutex));
        GetFido()->mRecvMsg.insert(GetFido()->mRecvMsg.end(), data, data + dataLen);
        pthread_mutex_unlock(&(GetFido()->mRecvBufMutex));
    }
    return 0;
}

bool Fido::getRecv(MSGTYPE *vRecv) {
    bool bRet = false;
    pthread_mutex_lock(&mRecvBufMutex);
    do {
        MSGTYPE tmp;
        if (NULL == vRecv) {
            mRecvMsg.swap(tmp);
            break;
        }

        if (mRecvMsg.size() <= FIDO_HEADER_LEN)
            break;

        unsigned int recvLen = CTOI(&mRecvMsg[1]);
        int fLen = 0;
        if (recvLen > 17) {
            fLen = (recvLen - 17) / 19;
            int lastLen = (recvLen - 17) % 19;
            if (lastLen > 0) fLen += 1;
        }

        // 数据长度判断，是否接收完成
        if (recvLen + FIDO_HEADER_LEN + fLen > mRecvMsg.size())
            break;

        vRecv->swap(tmp);

        LOG_ERR("getRecv Done");

        vRecv->insert(
            vRecv->begin(),
            mRecvMsg.begin(),
            mRecvMsg.end()
        );

        mRecvMsg.swap(tmp);

        bRet = true;
    } while (0);

    pthread_mutex_unlock(&mRecvBufMutex);
    return bRet;
}

unsigned int Fido::sendAPDU(unsigned long devHandle, unsigned char *bOrder, unsigned int orderLen,
                            unsigned char *bResponse, unsigned int *responseLen,
                            unsigned int timeout) {

    unsigned int ret = JUBR_OK;
    unsigned char sendMsg[512] = {0};
    unsigned int sendLen = (unsigned int) sizeof(sendMsg);
    do {
        ret = wrapFidoApdu('\x83', bOrder, orderLen, sendMsg, &sendLen);
        if (JUBR_OK != ret) break;

        ret = sendToDev(devHandle, sendMsg, sendLen, bResponse, responseLen);
        if (JUBR_OK != ret) break;

    } while (0);
    sendLen = (unsigned int) sizeof(sendMsg);
    return ret;
}

unsigned int Fido::sendToDev(unsigned long devHandle, unsigned char *pSendMsg,
                             unsigned int ulSendMsgLen, unsigned char *pRecvMsg,
                             unsigned int *pulRecvLen, unsigned int timeout) {
    unsigned int ret = JUBR_OK;

    do {

        struct timeval end;
        ret = ADDPRE(BLE_SendAPDU)(devHandle, pSendMsg, ulSendMsgLen);
        if (ret != 0) {
            break;
        }

        // 开始计算超时
        gettimeofday(&mStart, NULL);
        // 获取指令结果
        MSGTYPE recv, tmp;
        long timeused = 0;
        while (true) {
            bool isGetRecv = getRecv(&recv);
            if (false == isGetRecv) {
                gettimeofday(&end, NULL);

                long timeuse =
                    1000000 * (end.tv_sec - mStart.tv_sec) + end.tv_usec - mStart.tv_usec;
                if (timeused >= timeuse && 0 < timeused) {
                    ret = JUBR_OTHER_ERROR;
                    break;
                }

                if (timeout <= timeuse / 1000 && 0 < timeuse) {
                    ret = JUBR_TRANSACT_TIMEOUT;
                    break;
                }

                timeused = timeuse;
                usleep(10);
            } else {
                // TODO
                switch (recv[0]) {
                    case 0x81:
                    case 0x83:
                        break;
                    case 0xbf:
                        ret = -1;
                        break;
                    default:
                        ret = -1;
                        break;
                }
                if (ret != 0) {
                    break;
                }
                if (NULL == pRecvMsg || NULL == pulRecvLen) {
                    break;
                }
                ret = parseFidoApdu(&recv[0], recv.size(), pRecvMsg, pulRecvLen);
                break;
            }
        }

        recv.swap(tmp);
    } while (0);

    // 清空BUFF
    MSGTYPE tmp;

    mRecvMsg.swap(tmp);

    return ret;
}

unsigned int Fido::basicApduAddHeadAndLength(unsigned char paramHeader, unsigned char *pSendMsg,
                                             unsigned int ulSendMsgLen, unsigned char *pRecvMsg,
                                             unsigned int *pulRecvLen) {
    unsigned int ret = JUBR_OK;
    if (pRecvMsg == NULL) {
        ret = JUBR_ARGUMENTS_BAD;
        return ret;
    }
    unsigned int index = 0;

    // bMessageType
    pRecvMsg[index++] = paramHeader;
    pRecvMsg[index++] = (unsigned char) ((ulSendMsgLen & 0x0000FF00) >> 8);
    pRecvMsg[index++] = (unsigned char) ((ulSendMsgLen & 0x000000FF) >> 0);

    memcpy(&pRecvMsg[index], pSendMsg, ulSendMsgLen);
    index += ulSendMsgLen;

    *pulRecvLen = index;
    return ret;
}

unsigned int Fido::wrapFidoApdu(unsigned char paramHeader,
                                unsigned char *pSendMsg,
                                unsigned int ulSendMsgLen,
                                unsigned char *pRecvMsg,
                                unsigned int *pulRecvLen) {
    unsigned int ret = JUBR_OK;
    if (pRecvMsg == NULL) {
        ret = JUBR_ARGUMENTS_BAD;
        return ret;
    }
    do {
        ret = basicApduAddHeadAndLength(paramHeader, pSendMsg, ulSendMsgLen, pRecvMsg, pulRecvLen);
        if (ret != 0) break;
        if (*pulRecvLen <= FIDO_BLOCK_SIZE) {
            break;
        }

        unsigned int sendMsgIndex = 17;
        MSGTYPE cmd;
        cmd.insert(cmd.end(), pRecvMsg, pRecvMsg + FIDO_BLOCK_SIZE);

        unsigned int fIndex = 0x00;
        while (true) {
            cmd.push_back((unsigned char) fIndex);
            unsigned int capyEnd = sendMsgIndex + FIDO_DATA_BLOCK_SIZE;
            if (sendMsgIndex + FIDO_DATA_BLOCK_SIZE > ulSendMsgLen) {
                capyEnd = ulSendMsgLen;
            }

            cmd.insert(cmd.end(), pSendMsg + sendMsgIndex,
                       pSendMsg + capyEnd);

            sendMsgIndex += FIDO_DATA_BLOCK_SIZE;
            if (sendMsgIndex > ulSendMsgLen) {
                break;
            }
            fIndex++;
        }

        *pulRecvLen = cmd.size();
        memcpy(pRecvMsg, &cmd[0], *pulRecvLen);
    } while (0);
    return ret;
}

unsigned int Fido::parseFidoApdu(unsigned char *pSendMsg, unsigned int ulSendMsgLen,
                                 unsigned char *pRecvMsg, unsigned int *pulRecvLen) {
    LOG_ERR("enter parseFidoApdu");
    unsigned int ret = JUBR_OK;
    if (pRecvMsg == NULL) {
        ret = JUBR_ARGUMENTS_BAD;
        return ret;
    }
    do {
        unsigned char tempMsg[512] = {0};
        unsigned int tempLen = (unsigned int) sizeof(tempMsg);
        ret = removePkgIndex(pSendMsg, ulSendMsgLen, tempMsg, &tempLen);
        if (ret != 0) break;

        ret = removeCmdHeadAndStateCode(tempMsg, tempLen, pRecvMsg, pulRecvLen);
        tempLen = (unsigned int) sizeof(tempMsg);
    } while (0);
    return ret;
}

unsigned int Fido::removePkgIndex(unsigned char *pSendMsg, unsigned int ulSendMsgLen,
                                  unsigned char *pRecvMsg, unsigned int *pulRecvLen) {
    LOG_ERR("enter removePkgIndex");
    unsigned int ret = JUBR_OK;
    if (pRecvMsg == NULL) {
        ret = JUBR_ARGUMENTS_BAD;
        return ret;
    }
    do {
        unsigned int recvLen = CTOI(&pSendMsg[1]) + FIDO_HEADER_LEN;
        if (recvLen <= FIDO_BLOCK_SIZE) {
            *pulRecvLen = ulSendMsgLen;
            memcpy(pRecvMsg, pSendMsg, ulSendMsgLen);
            break;
        }

        MSGTYPE temp;
        unsigned int removeIndex = 0;
        temp.insert(temp.end(), pSendMsg, pSendMsg + FIDO_BLOCK_SIZE);
        removeIndex += FIDO_BLOCK_SIZE;
        while (1) {
            removeIndex += 1;
            unsigned int endIndex = removeIndex + FIDO_DATA_BLOCK_SIZE;
            if (removeIndex + FIDO_DATA_BLOCK_SIZE > ulSendMsgLen) {
                endIndex = ulSendMsgLen;
            }
            temp.insert(temp.end(), pSendMsg + removeIndex, pSendMsg + endIndex);
            if (endIndex == ulSendMsgLen) {
                break;
            }
            removeIndex = endIndex;
        }
        if (recvLen != temp.size()) {
            LOG_ERR("temp size %d", temp.size());
            ret = JUBR_ARGUMENTS_BAD;
            break;
        }
        *pulRecvLen = temp.size();
        memcpy(pRecvMsg, &temp[0], *pulRecvLen);
    } while (0);
    return ret;
}

unsigned int Fido::removeCmdHeadAndStateCode(unsigned char *pSendMsg, unsigned int ulSendMsgLen,
                                             unsigned char *pRecvMsg, unsigned int *pulRecvLen) {
    LOG_ERR("enter removeCmdHeadAndStateCode");

    unsigned int ret = JUBR_OK;
    if (pRecvMsg == NULL) {
        ret = JUBR_ARGUMENTS_BAD;
        return ret;
    }
    do {
        MSGTYPE temp;
//        if(memcmp(pSendMsg,"0x81",1) == 0){
//            temp.insert(temp.end(),pSendMsg + FIDO_HEADER_LEN,pSendMsg + ulSendMsgLen);
//        } else {
//            temp.insert(temp.end(),pSendMsg + FIDO_HEADER_LEN,pSendMsg + ulSendMsgLen - 2);
//        }
        temp.insert(temp.end(), pSendMsg + FIDO_HEADER_LEN, pSendMsg + ulSendMsgLen);
        *pulRecvLen = temp.size();
        memcpy(pRecvMsg, &temp[0], *pulRecvLen);
    } while (0);
    return ret;
}
