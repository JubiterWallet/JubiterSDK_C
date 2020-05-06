//
//  comm_android.cpp
//  FTNFCDemo
//
//  Created by whl on 2020/3/30.
//  Copyright © 2020 whl. All rights reserved.
//

#include "comm.h"

#ifdef ANDROID_API_LEVEL_H

#include <jni.h>

JavaVM* g_vm = NULL;

unsigned int  InitializeNFC(void* param,NFC_INIT_PARAM* initPara){
    g_vm = (JavaVM*) param;
    return 0;
}

unsigned int ConnectNFC(const char* pcuuid, unsigned long* pulHandle){
    return 0;
}

unsigned int DisconnectNFC(unsigned long ulHandle){
    return 0;
}

bool IsConnectedNFC(unsigned long ulHandle){
    return false;
}

int TransmitNFC(unsigned long ulHandle, unsigned char* sendBuf,unsigned int sendLen,unsigned char * recvBuf,unsigned int* recvLen,int timeoutSec){
    JNIEnv* env = NULL;
    if (g_vm->GetEnv((void**) &env, JNI_VERSION_1_6) != JNI_OK)
    {
        return FTR_FAILED;
    }
    return 0;
}

#endif
