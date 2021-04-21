/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   JUB_CORE.h
 * Author: zxc
 *
 * Created on 2019年7月18日, 上午10:58
 */

#ifndef JUB_CORE_H
#define JUB_CORE_H

//硬件接口 + 软件接口的头文件，外部使用，只需要包含这一个

#include "JUB_SDK.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

typedef enum {
    STRENGTH128 = 128,
    STRENGTH192 = 192,
    STRENGTH256 = 256
} JUB_ENUM_MNEMONIC_STRENGTH;

JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GenerateMnemonic(IN JUB_ENUM_MNEMONIC_STRENGTH strength,
                            OUT JUB_CHAR_PTR_PTR mnemonic);

JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_CheckMnemonic(IN JUB_CHAR_CPTR mnemonic);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus
#endif /* JUB_CORE_H */
