/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   JUB_core.h
 * Author: zxc
 *
 * Created on 2019年7月18日, 上午10:58
 */


#ifndef JUB_CORE_H
#define JUB_CORE_H

#include "JUB_SDK.h"

typedef enum {
    STRENGTH128 = 128,
    STRENGTH192 = 192,
    STRENGTH256 = 256
} JUB_MNEMONIC_STRENGTH;

JUB_RV JUB_GenerateMnemonic(IN JUB_MNEMONIC_STRENGTH strength, JUB_CHAR_PTR_PTR mnemonic);


#endif /* JUB_CORE_H */
