#pragma once
//only for BTC series coins now, maybe for all coins later
#include <string>
#include <memory>

#include "utility/xFactory.hpp"
#include "utility/Singleton.h"

#if defined(SIM_MODE)
#include "device/JubiterBridgeDevice.hpp"
#endif // #if defined(SIM_MODE) end
#include "device/JubiterSWIDevice.hpp"
#include "device/JubiterHidDevice.hpp"
#include "device/JubiterBLEDevice.hpp"
#if defined(NFC_MODE)
#include "device/JubiterNFCDevice.hpp"
#endif // #if defined(NFC_MODE) end

#include "token/interface/BTCTokenInterface.hpp"
#include "token/interface/CKBTokenInterface.hpp"

#include "token/BTC/JubiterBladeBTCImpl.h"
#include "token/BTC/JubiterBladeBCHImpl.h"
#include "token/BTC/JubiterBladeLTCImpl.h"
#include "token/BTC/JubiterBladeUSDTImpl.h"
#include "token/BTC/JubiterBladeDashImpl.h"
#include "token/BTC/JubiterBladeQTUMImpl.h"

#include "token/BTC/JubiterBIOBTCImpl.h"

#include "token/BTC/JubiterLiteBTCImpl.h"
#include "token/BTC/JubiterLiteBCHImpl.h"
#include "token/BTC/JubiterLiteLTCImpl.h"
#include "token/BTC/JubiterLiteUSDTImpl.h"
#include "token/BTC/JubiterLiteDashImpl.h"
#include "token/BTC/JubiterLiteQTUMImpl.h"

#include "token/BTC/TrezorCryptoBTCImpl.h"
#include "token/BTC/TrezorCryptoBCHImpl.h"
#include "token/BTC/TrezorCryptoLTCImpl.h"
#include "token/BTC/TrezorCryptoUSDTImpl.h"
#include "token/BTC/TrezorCryptoDashImpl.h"
#include "token/BTC/TrezorCryptoQTUMImpl.h"
#include "token/CKB/TrezorCryptoCKBImpl.h"


namespace jub {
namespace token {


typedef std::shared_ptr<BaseToken>(*CreateJubiterBTCFn)(JUB_UINT16, const JUB_ENUM_CURVES);


class xTrezorCryptoBTCFactory :
public xFactory<std::shared_ptr<BaseToken>,
                JUB_ENUM_COINTYPE_BTC,
                CreateJubiterBTCFn> {
public:
    xTrezorCryptoBTCFactory() {
        Register(JUB_ENUM_COINTYPE_BTC::COINBTC,  &TrezorCryptoToken::Create<TrezorCryptoBTCImpl>);
        Register(JUB_ENUM_COINTYPE_BTC::COINBCH,  &TrezorCryptoToken::Create<TrezorCryptoBCHImpl>);
        Register(JUB_ENUM_COINTYPE_BTC::COINLTC,  &TrezorCryptoToken::Create<TrezorCryptoLTCImpl>);
        Register(JUB_ENUM_COINTYPE_BTC::COINUSDT, &TrezorCryptoToken::Create<TrezorCryptoUSDTImpl>);
        Register(JUB_ENUM_COINTYPE_BTC::COINDASH, &TrezorCryptoToken::Create<TrezorCryptoDashImpl>);
        Register(JUB_ENUM_COINTYPE_BTC::COINQTUM, &TrezorCryptoToken::Create<TrezorCryptoQTUMImpl>);
        Register(JUB_ENUM_COINTYPE_BTC::COINCKB,  &TrezorCryptoToken::Create<TrezorCryptoCKBImpl>);
    }
}; // class xTrezorCryptoBTCFactory end


class xJuBiterBladeBTCFactory :
public xFactory<std::shared_ptr<BaseToken>,
                JUB_ENUM_COINTYPE_BTC,
                CreateJubiterBTCFn> {
public:
    xJuBiterBladeBTCFactory() {
        Register(JUB_ENUM_COINTYPE_BTC::COINBTC,  &HardwareTokenInterface::Create<JubiterBladeBTCImpl>);
        Register(JUB_ENUM_COINTYPE_BTC::COINBCH,  &HardwareTokenInterface::Create<JubiterBladeBCHImpl>);
        Register(JUB_ENUM_COINTYPE_BTC::COINLTC,  &HardwareTokenInterface::Create<JubiterBladeLTCImpl>);
        Register(JUB_ENUM_COINTYPE_BTC::COINUSDT, &HardwareTokenInterface::Create<JubiterBladeUSDTImpl>);
        Register(JUB_ENUM_COINTYPE_BTC::COINDASH, &HardwareTokenInterface::Create<JubiterBladeDashImpl>);
        Register(JUB_ENUM_COINTYPE_BTC::COINQTUM, &HardwareTokenInterface::Create<JubiterBladeQTUMImpl>);
    }
}; // class xJuBiterBladeBTCFactory end


class xJuBiterBIOBTCFactory :
public xFactory<std::shared_ptr<BaseToken>,
                JUB_ENUM_COINTYPE_BTC,
                CreateJubiterBTCFn> {
public:
    xJuBiterBIOBTCFactory() {
        Register(JUB_ENUM_COINTYPE_BTC::COINBTC,  &HardwareTokenInterface::Create<JubiterBIOBTCImpl>);
        Register(JUB_ENUM_COINTYPE_BTC::COINBCH,  &HardwareTokenInterface::Create<JubiterBIOBCHImpl>);
        Register(JUB_ENUM_COINTYPE_BTC::COINLTC,  &HardwareTokenInterface::Create<JubiterBIOLTCImpl>);
        Register(JUB_ENUM_COINTYPE_BTC::COINUSDT, &HardwareTokenInterface::Create<JubiterBIOUSDTImpl>);
        Register(JUB_ENUM_COINTYPE_BTC::COINDASH, &HardwareTokenInterface::Create<JubiterBIODashImpl>);
        Register(JUB_ENUM_COINTYPE_BTC::COINQTUM, &HardwareTokenInterface::Create<JubiterBIOQTUMImpl>);
    }
}; // class xJuBiterBIOBTCFactory end


class xJuBiterLiteBTCFactory :
public xFactory<std::shared_ptr<BaseToken>,
                JUB_ENUM_COINTYPE_BTC,
                CreateJubiterBTCFn> {
public:
    xJuBiterLiteBTCFactory() {
        Register(JUB_ENUM_COINTYPE_BTC::COINBTC,  &HardwareTokenInterface::Create<JubiterLiteBTCImpl>);
        Register(JUB_ENUM_COINTYPE_BTC::COINBCH,  &HardwareTokenInterface::Create<JubiterLiteBCHImpl>);
        Register(JUB_ENUM_COINTYPE_BTC::COINLTC,  &HardwareTokenInterface::Create<JubiterLiteLTCImpl>);
        Register(JUB_ENUM_COINTYPE_BTC::COINUSDT, &HardwareTokenInterface::Create<JubiterLiteUSDTImpl>);
        Register(JUB_ENUM_COINTYPE_BTC::COINDASH, &HardwareTokenInterface::Create<JubiterLiteDashImpl>);
        Register(JUB_ENUM_COINTYPE_BTC::COINQTUM, &HardwareTokenInterface::Create<JubiterLiteQTUMImpl>);
    }
}; // class xJuBiterLiteBTCFactory end


class xBTCTokenFactory {
protected:
    xTrezorCryptoBTCFactory      trezorFactory;
    xJuBiterBladeBTCFactory  jubiterBLDFactory;
      xJuBiterBIOBTCFactory  jubiterBIOFactory;
     xJuBiterLiteBTCFactory jubiterLITEFactory;

public:
    std::shared_ptr<BaseToken> CreateToken(const JUB_UINT16 deviceID, const JUB_ENUM_COINTYPE_BTC& type, const JUB_ENUM_CURVES curve=JUB_ENUM_CURVES::SECP256K1) {
#if defined(SIM_MODE)
        if (dynamic_cast<jub::device::JubiterBridgeBLDDevice*>(
                         jub::device::DeviceManager::GetInstance()->GetOne(deviceID))
        ) {
            return jubiterBLDFactory.Create(type, deviceID, curve);
        }
        else if (dynamic_cast<jub::device::JubiterBridgeBIODevice*>(
                              jub::device::DeviceManager::GetInstance()->GetOne(deviceID))
        ) {
            return jubiterBIOFactory.Create(type, deviceID, curve);
        }
        else if (dynamic_cast<jub::device::JubiterBridgeLITEDevice*>(
                              jub::device::DeviceManager::GetInstance()->GetOne(deviceID))
        ) {
            return jubiterLITEFactory.Create(type, deviceID, curve);
        }
#endif  // #if defined(SIM_MODE) end
#if defined(SWI_MODE)
        if (dynamic_cast<jub::device::JubiterSWIImplDevice*>(
                         jub::device::DeviceManager::GetInstance()->GetOne(deviceID))
        ) {
            return trezorFactory.Create(type, deviceID, curve);
        }
#endif  // #if defined(SWI_MODE) end
#if defined(HID_MODE)
        if (dynamic_cast<jub::device::JubiterHidBLDDevice*>(
                         jub::device::DeviceManager::GetInstance()->GetOne(deviceID))
        ) {
            return jubiterBLDFactory.Create(type, deviceID, curve);
        }
        else if (dynamic_cast<jub::device::JubiterHidBIODevice*>(
                              jub::device::DeviceManager::GetInstance()->GetOne(deviceID))
        ) {
            return jubiterBIOFactory.Create(type, deviceID, curve);
        }
#endif  // #if defined(HID_MODE) end
#if defined(BLE_MODE)
        if (dynamic_cast<jub::device::JubiterBLEBLDDevice*>(
                         jub::device::DeviceManager::GetInstance()->GetOne(deviceID))
        ) {
            return jubiterBLDFactory.Create(type, deviceID, curve);
        }
        else if (dynamic_cast<jub::device::JubiterBLEBIODevice*>(
                              jub::device::DeviceManager::GetInstance()->GetOne(deviceID))
        ) {
            return jubiterBIOFactory.Create(type, deviceID, curve);
        }
#endif  // #if defined(BLE_MODE) end
#if defined(NFC_MODE)
        if (dynamic_cast<jub::device::JubiterNFCDevice*>(
                         jub::device::DeviceManager::GetInstance()->GetOne(deviceID))
        ) {
            return jubiterLITEFactory.Create(type, deviceID, curve);
        }
#endif  // #if defined(NFC_MODE) end
//        else {
            return nullptr;
//        }
    }
}; // class xBTCTokenFactory end

using BTCseriesTokenFactory = Singleton<xBTCTokenFactory>;


} // namespace token end
} // namespace jub end
