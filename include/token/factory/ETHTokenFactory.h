#pragma once
//only for other than ETH series coins now, maybe for all coins later
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

#include "token/interface/ETHTokenInterface.hpp"

#include "token/ETH/JubiterBladeETHImpl.h"
#include "token/FIL/JubiterBladeFILImpl.h"

#include "token/ETH/JubiterBIOETHImpl.h"
#include "token/FIL/JubiterBIOFILImpl.h"

#include "token/ETH/TrezorCryptoETHImpl.h"
#include "token/FIL/TrezorCryptoFILImpl.h"

#include "token/ETH/JubiterLiteETHImpl.h"
#include "token/FIL/JubiterLiteFILImpl.h"

#include <TrustWalletCore/TWCoinType.h>


namespace jub {
namespace token {


typedef std::shared_ptr<BaseToken>(*CreateJubiterETHFn)(JUB_UINT16);


class xTrezorCryptoETHFactory :
public xFactory<std::shared_ptr<BaseToken>,
                TWCoinType,
                CreateJubiterETHFn> {
public:
    xTrezorCryptoETHFactory() {
        Register(TWCoinType::TWCoinTypeEthereum,        &TrezorCryptoToken::Create<TrezorCryptoETHImpl>);
//        Register(TWCoinType::TWCoinTypeEthereumClassic, &TrezorCryptoToken::Create<TrezorCryptoETHImpl>);
        Register(TWCoinType::TWCoinTypeFilecoin,        &TrezorCryptoToken::Create<TrezorCryptoFILImpl>);
    }
}; // class xTrezorCryptoETHFactory end


class xJuBiterBladeETHFactory :
public xFactory<std::shared_ptr<BaseToken>,
                TWCoinType,
                CreateJubiterETHFn> {
public:
    xJuBiterBladeETHFactory() {
        Register(TWCoinType::TWCoinTypeEthereum,        &HardwareTokenInterface::Create<JubiterBladeETHImpl>);
//        Register(TWCoinType::TWCoinTypeEthereumClassic, &HardwareTokenInterface::Create<JubiterBladeETHImpl>);
        Register(TWCoinType::TWCoinTypeFilecoin,        &HardwareTokenInterface::Create<JubiterBladeFILImpl>);
    }
}; // class xJuBiterBladeETHFactory end


class xJuBiterBIOETHFactory :
public xFactory<std::shared_ptr<BaseToken>,
                TWCoinType,
                CreateJubiterETHFn> {
public:
    xJuBiterBIOETHFactory() {
        Register(TWCoinType::TWCoinTypeEthereum,        &HardwareTokenInterface::Create<JubiterBIOETHImpl>);
//        Register(TWCoinType::TWCoinTypeEthereumClassic, &HardwareTokenInterface::Create<JubiterBIOETHImpl>);
        Register(TWCoinType::TWCoinTypeFilecoin,        &HardwareTokenInterface::Create<JubiterBIOFILImpl>);
    }
}; // class xJuBiterBIOETHFactory end


//#if defined(NFC_MODE)
class xJuBiterLiteETHFactory :
public xFactory<std::shared_ptr<BaseToken>,
                TWCoinType,
                CreateJubiterETHFn> {
public:
    xJuBiterLiteETHFactory() {
        Register(TWCoinType::TWCoinTypeEthereum,        &HardwareTokenInterface::Create<JubiterLiteETHImpl>);
//        Register(TWCoinType::TWCoinTypeEthereumClassic, &HardwareTokenInterface::Create<JubiterLiteETHImpl>);
        Register(TWCoinType::TWCoinTypeFilecoin,        &HardwareTokenInterface::Create<JubiterLiteFILImpl>);
    }
}; // class xJuBiterLiteETHFactory end
//#endif // #if defined(NFC_MODE) end


class xETHTokenFactory {
protected:
    xTrezorCryptoETHFactory     trezorFactory;
    xJuBiterBladeETHFactory jubiterBLDFactory;
    xJuBiterBIOETHFactory   jubiterBIOFactory;
    xJuBiterLiteETHFactory  jubiterLiteFactory;

public:
    std::shared_ptr<BaseToken> CreateToken(const TWCoinType& type, const JUB_UINT16 deviceID) {
#if defined(SIM_MODE)
        if (dynamic_cast<jub::device::JubiterBridgeBLDDevice*>(
                         jub::device::DeviceManager::GetInstance()->GetOne(deviceID))
        ) {
            return jubiterBLDFactory.Create(type, deviceID);
        }
        else if (dynamic_cast<jub::device::JubiterBridgeBIODevice*>(
                              jub::device::DeviceManager::GetInstance()->GetOne(deviceID))
        ) {
            return jubiterBIOFactory.Create(type, deviceID);
        }
        else if (dynamic_cast<jub::device::JubiterBridgeLITEDevice*>(
                              jub::device::DeviceManager::GetInstance()->GetOne(deviceID))
        ) {
            return jubiterLiteFactory.Create(type, deviceID);
        }
#endif  // #if defined(SIM_MODE) end
#if defined(SWI_MODE)
        if (dynamic_cast<jub::device::JubiterSWIImplDevice*>(
                         jub::device::DeviceManager::GetInstance()->GetOne(deviceID))
        ) {
            return trezorFactory.Create(type, deviceID);
        }
#endif  // #if defined(SWI_MODE) end
#if defined(HID_MODE)
        if (dynamic_cast<jub::device::JubiterHidBLDDevice*>(
                         jub::device::DeviceManager::GetInstance()->GetOne(deviceID))
        ) {
            return jubiterBLDFactory.Create(type, deviceID);
        }
        else if (dynamic_cast<jub::device::JubiterHidBIODevice*>(
                              jub::device::DeviceManager::GetInstance()->GetOne(deviceID))
        ) {
            return jubiterBIOFactory.Create(type, deviceID);
        }
#endif  // #if defined(HID_MODE) end
#if defined(BLE_MODE)
        if (dynamic_cast<jub::device::JubiterBLEBLDDevice*>(
                         jub::device::DeviceManager::GetInstance()->GetOne(deviceID))
        ) {
            return jubiterBLDFactory.Create(type, deviceID);
        }
        else if (dynamic_cast<jub::device::JubiterBLEBIODevice*>(
                              jub::device::DeviceManager::GetInstance()->GetOne(deviceID))
        ) {
            return jubiterBIOFactory.Create(type, deviceID);
        }
#endif  // #if defined(BLE_MODE) end
#if defined(NFC_MODE)
        if (dynamic_cast<jub::device::JubiterNFCDevice*>(
                         jub::device::DeviceManager::GetInstance()->GetOne(deviceID))
        ) {
            return jubiterLiteFactory.Create(type, deviceID);
        }
#endif  // #if defined(NFC_MODE) end
//        else {
            return nullptr;
//        }
    }
}; // class xETHTokenFactory end

using ETHseriesTokenFactory = Singleton<xETHTokenFactory>;


} // namespace token end
} // namespace jub end
