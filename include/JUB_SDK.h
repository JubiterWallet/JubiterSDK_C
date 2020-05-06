#ifndef JUB_SDK_H
#define JUB_SDK_H
//硬件接口的头文件，外部使用，只需要包含这一个

#include "JUB_SDK_COMM.h"
#include "JUB_SDK_DEV.h"
#if defined(BLE_MODE)
#include "JUB_SDK_DEV_BLE.h"
#endif
#if defined(NFC_MODE)
#include "JUB_SDK_DEV_NFC.h"
#endif
#if defined(HID_MODE)
#include "JUB_SDK_DEV_HID.h"
#endif
#include "JUB_SDK_BTC.h"
#include "JUB_SDK_ETH.h"
#include "JUB_SDK_Hcash.h"
#include "JUB_SDK_EOS.h"
#include "JUB_SDK_XRP.h"

#endif /* JUB_SDK_H */
