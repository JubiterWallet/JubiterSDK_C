#ifndef JUB_SDK_H
#define JUB_SDK_H
//硬件接口的头文件，外部使用，只需要包含这一个

#include "JUB_SDK_COMM.h"
#include "JUB_SDK_DEV.h"
#if defined(BLE_MODE)
#include "JUB_SDK_DEV_BLE.h"
#include "JUB_SDK_DEV_BIO.h"
#endif // #if defined(BLE_MODE) end
#if defined(NFC_MODE)
#include "JUB_SDK_DEV_NFC.h"
#endif // #if defined(NFC_MODE) end
#if defined(HID_MODE)
#include "JUB_SDK_DEV_HID.h"
#include "JUB_SDK_DEV_BIO.h"
#endif // #if defined(HID_MODE) end
#if defined(GRPC_MODE)
#include "JUB_SDK_DEV_GRPC.h"
#endif // #if defined(GRPC_MODE) end
#include "JUB_SDK_BTC.h"
#include "JUB_SDK_Hcash.h"
#include "JUB_SDK_ETH.h"
#include "JUB_SDK_EOS.h"
#include "JUB_SDK_XRP.h"
#include "JUB_SDK_TRX.h"

#endif /* JUB_SDK_H */
