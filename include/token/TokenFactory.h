#pragma once
//only for BTC series coins now, maybe for all coins later

#include <utility/xFactory.hpp>
#include <utility/Singleton.h>
#include <device/JubiterHidDevice.hpp>
#include <device/JubiterBLEDevice.hpp>
#include <device/JubiterNFCDevice.hpp>

#include <token/interface/BTCTokenInterface.hpp>

#include <token/BTC/JubiterBladeBTCImpl.h>
#include <token/BTC/JubiterBladeBCHImpl.h>
#include <token/BTC/JubiterBladeLTCImpl.h>
#include <token/BTC/JubiterBladeUSDTImpl.h>
#include <token/BTC/JubiterBladeDashImpl.h>
#include <token/BTC/JubiterBladeQTUMImpl.h>

#include <token/BTC/JubiterBIOImpl.h>

#include <token/BTC/JubiterNFCBTCImpl.h>
#include <token/BTC/JubiterNFCBCHImpl.h>
#include <token/BTC/JubiterNFCLTCImpl.h>
#include <token/BTC/JubiterNFCUSDTImpl.h>
#include <token/BTC/JubiterNFCDashImpl.h>
#include <token/BTC/JubiterNFCQTUMImpl.h>

#include <token/BTC/TrezorCryptoBTCImpl.h>
#include <token/BTC/TrezorCryptoBCHImpl.h>
#include <token/BTC/TrezorCryptoLTCImpl.h>
#include <token/BTC/TrezorCryptoUSDTImpl.h>
#include <token/BTC/TrezorCryptoDashImpl.h>
#include <token/BTC/TrezorCryptoQTUMImpl.h>

#include <string>
#include <memory>

namespace jub {
namespace token {


typedef std::shared_ptr<BaseToken>(*CreateTrezorCryptoBTCFn)(const std::string&);
typedef std::shared_ptr<BaseToken>(*CreateJubiterBTCFn)(JUB_UINT16);


class xTrezorCryptoBTCFactory :
public xFactory<std::shared_ptr<BaseToken>,
                JUB_ENUM_COINTYPE_BTC,
                CreateTrezorCryptoBTCFn> {
public:
    xTrezorCryptoBTCFactory() {
        Register(JUB_ENUM_COINTYPE_BTC::COINBTC,   &TrezorCryptoBTCImpl::Create);
        Register(JUB_ENUM_COINTYPE_BTC::COINBCH,   &TrezorCryptoBCHImpl::Create);
        Register(JUB_ENUM_COINTYPE_BTC::COINLTC,   &TrezorCryptoLTCImpl::Create);
        Register(JUB_ENUM_COINTYPE_BTC::COINUSDT, &TrezorCryptoUSDTImpl::Create);
        Register(JUB_ENUM_COINTYPE_BTC::COINDASH, &TrezorCryptoDashImpl::Create);
        Register(JUB_ENUM_COINTYPE_BTC::COINQTUM, &TrezorCryptoQTUMImpl::Create);
    }
}; // class xTrezorCryptoBTCFactory end


class xJuBiterBladeBTCFactory :
public xFactory<std::shared_ptr<BaseToken>,
                JUB_ENUM_COINTYPE_BTC,
                CreateJubiterBTCFn> {
public:
    xJuBiterBladeBTCFactory() {
        Register(JUB_ENUM_COINTYPE_BTC::COINBTC,   &JubiterBladeBTCImpl::Create);
        Register(JUB_ENUM_COINTYPE_BTC::COINBCH,   &JubiterBladeBCHImpl::Create);
        Register(JUB_ENUM_COINTYPE_BTC::COINLTC,   &JubiterBladeLTCImpl::Create);
        Register(JUB_ENUM_COINTYPE_BTC::COINUSDT, &JubiterBladeUSDTImpl::Create);
        Register(JUB_ENUM_COINTYPE_BTC::COINDASH, &JubiterBladeDashImpl::Create);
        Register(JUB_ENUM_COINTYPE_BTC::COINQTUM, &JubiterBladeQTUMImpl::Create);
    }
}; // class xJuBiterBladeBTCFactory end


class xJuBiterBIOBTCFactory :
public xFactory<std::shared_ptr<BaseToken>,
                JUB_ENUM_COINTYPE_BTC,
                CreateJubiterBTCFn> {
public:
    xJuBiterBIOBTCFactory() {
        Register(JUB_ENUM_COINTYPE_BTC::COINBTC,   &JubiterBIOBTCImpl::Create);
        Register(JUB_ENUM_COINTYPE_BTC::COINBCH,   &JubiterBIOBCHImpl::Create);
        Register(JUB_ENUM_COINTYPE_BTC::COINLTC,   &JubiterBIOLTCImpl::Create);
        Register(JUB_ENUM_COINTYPE_BTC::COINUSDT, &JubiterBIOUSDTImpl::Create);
        Register(JUB_ENUM_COINTYPE_BTC::COINDASH, &JubiterBIODashImpl::Create);
        Register(JUB_ENUM_COINTYPE_BTC::COINQTUM, &JubiterBIOQTUMImpl::Create);
    }
}; // class xJuBiterBIOBTCFactory end


class xJuBiterNFCBTCFactory :
public xFactory<std::shared_ptr<BaseToken>,
                JUB_ENUM_COINTYPE_BTC,
                CreateJubiterBTCFn> {
public:
    xJuBiterNFCBTCFactory() {
        Register(JUB_ENUM_COINTYPE_BTC::COINBTC,   &JubiterNFCBTCImpl::Create);
        Register(JUB_ENUM_COINTYPE_BTC::COINBCH,   &JubiterNFCBCHImpl::Create);
        Register(JUB_ENUM_COINTYPE_BTC::COINLTC,   &JubiterNFCLTCImpl::Create);
        Register(JUB_ENUM_COINTYPE_BTC::COINUSDT, &JubiterNFCUSDTImpl::Create);
        Register(JUB_ENUM_COINTYPE_BTC::COINDASH, &JubiterNFCDashImpl::Create);
        Register(JUB_ENUM_COINTYPE_BTC::COINQTUM, &JubiterNFCQTUMImpl::Create);
    }
}; // class xJuBiterNFCBTCFactory end


class xBTCTokenFactory {
protected:
    xTrezorCryptoBTCFactory     trezorFactory;
    xJuBiterBladeBTCFactory jubiterBLDFactory;
    xJuBiterBIOBTCFactory   jubiterBIOFactory;
    xJuBiterNFCBTCFactory   jubiterNFCFactory;

public:
    std::shared_ptr<BaseToken> CreateToken(const JUB_ENUM_COINTYPE_BTC& type, const std::string& XPRVorXPUB) {
        return trezorFactory.Create(type, XPRVorXPUB);
    }

    std::shared_ptr<BaseToken> CreateToken(const JUB_ENUM_COINTYPE_BTC& type, const JUB_UINT16 deviceID) {
#if defined(HID_MODE)
        if (dynamic_cast<jub::device::JubiterHidBLDDevice*>
        (jub::device::DeviceManager::GetInstance()->GetOne(deviceID))
        ) {
            return jubiterBLDFactory.Create(type, deviceID);
        }
        else if (dynamic_cast<jub::device::JubiterHidBIODevice*>
        (jub::device::DeviceManager::GetInstance()->GetOne(deviceID))
        ) {
            return jubiterBIOFactory.Create(type, deviceID);
        }
#endif  // #if defined(HID_MODE) end
#if defined(BLE_MODE)
        if (dynamic_cast<jub::device::JubiterBLEDevice*>
            (jub::device::DeviceManager::GetInstance()->GetOne(deviceID))
            ) {
            return jubiterBLDFactory.Create(type, deviceID);
        }
#endif  // #if defined(BLE_MODE) end
#if defined(NFC_MODE)
        if (dynamic_cast<jub::device::JubiterNFCDevice*>
            (jub::device::DeviceManager::GetInstance()->GetOne(deviceID))
            ) {
            return jubiterNFCFactory.Create(type, deviceID);
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
