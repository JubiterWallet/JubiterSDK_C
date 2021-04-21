#pragma once
//only for other than BTC/ETH series coins now, maybe for all coins later
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
#include "token/interface/ETHTokenInterface.hpp"
#include "token/interface/EOSTokenInterface.hpp"
#include "token/interface/XRPTokenInterface.hpp"
#include "token/interface/TRXTokenInterface.hpp"

#include "token/HC/JubiterBladeHCImpl.h"
#include "token/ETH/JubiterBladeETHImpl.h"
#include "token/EOS/JubiterBladeEOSImpl.h"
#include "token/XRP/JubiterBladeXRPImpl.h"
#include "token/TRX/JubiterBladeTRXImpl.h"

#include "token/HC/JubiterBIOHCImpl.h"
#include "token/ETH/JubiterBIOETHImpl.h"
#include "token/EOS/JubiterBIOEOSImpl.h"
#include "token/XRP/JubiterBIOXRPImpl.h"
#include "token/TRX/JubiterBIOTRXImpl.h"

#include "token/HC/TrezorCryptoHCImpl.h"
#include "token/ETH/TrezorCryptoETHImpl.h"
#include "token/EOS/TrezorCryptoEOSImpl.h"
#include "token/XRP/TrezorCryptoXRPImpl.h"
#include "token/TRX/TrezorCryptoTRXImpl.h"

#include "token/HC/JubiterLiteHCImpl.h"
#include "token/ETH/JubiterLiteETHImpl.h"
#include "token/EOS/JubiterLiteEOSImpl.h"
#include "token/XRP/JubiterLiteXRPImpl.h"
#include "token/TRX/JubiterLiteTRXImpl.h"

#include <TrustWalletCore/TWCoinType.h>


namespace jub {
namespace token {


typedef std::shared_ptr<BaseToken>(*CreateJubiterMISCFn)(JUB_UINT16);


class xTrezorCryptoMISCFactory :
public xFactory<std::shared_ptr<BaseToken>,
                TWCoinType,
                CreateJubiterMISCFn> {
public:
    xTrezorCryptoMISCFactory() {
        Register(TWCoinType::TWCoinTypeHcash,       &TrezorCryptoToken::Create<TrezorCryptoHCImpl>);
        Register(TWCoinType::TWCoinTypeEOS,         &TrezorCryptoToken::Create<TrezorCryptoEOSImpl>);
        Register(TWCoinType::TWCoinTypeXRP,         &TrezorCryptoToken::Create<TrezorCryptoXRPImpl>);
        Register(TWCoinType::TWCoinTypeTron,        &TrezorCryptoToken::Create<TrezorCryptoTRXImpl>);
    }
}; // class xTrezorCryptoMISCFactory end


class xJuBiterBladeMISCFactory :
public xFactory<std::shared_ptr<BaseToken>,
                TWCoinType,
                CreateJubiterMISCFn> {
public:
    xJuBiterBladeMISCFactory() {
        Register(TWCoinType::TWCoinTypeHcash,       &HardwareTokenInterface::Create<JubiterBladeHCImpl>);
        Register(TWCoinType::TWCoinTypeEOS,         &HardwareTokenInterface::Create<JubiterBladeEOSImpl>);
        Register(TWCoinType::TWCoinTypeXRP,         &HardwareTokenInterface::Create<JubiterBladeXRPImpl>);
        Register(TWCoinType::TWCoinTypeTron,        &HardwareTokenInterface::Create<JubiterBladeTRXImpl>);
    }
}; // class xJuBiterBladeMISCFactory end


class xJuBiterBIOMISCFactory :
public xFactory<std::shared_ptr<BaseToken>,
                TWCoinType,
                CreateJubiterMISCFn> {
public:
    xJuBiterBIOMISCFactory() {
        Register(TWCoinType::TWCoinTypeHcash,       &HardwareTokenInterface::Create<JubiterBIOHCImpl>);
        Register(TWCoinType::TWCoinTypeEOS,         &HardwareTokenInterface::Create<JubiterBIOEOSImpl>);
        Register(TWCoinType::TWCoinTypeXRP,         &HardwareTokenInterface::Create<JubiterBIOXRPImpl>);
        Register(TWCoinType::TWCoinTypeTron,        &HardwareTokenInterface::Create<JubiterBIOTRXImpl>);
    }
}; // class xJuBiterBIOMISCFactory end


class xJuBiterLITEMISCFactory :
public xFactory<std::shared_ptr<BaseToken>,
                TWCoinType,
                CreateJubiterMISCFn> {
public:
    xJuBiterLITEMISCFactory() {
        Register(TWCoinType::TWCoinTypeHcash,       &HardwareTokenInterface::Create<JubiterLiteHCImpl>);
        Register(TWCoinType::TWCoinTypeEthereum,    &HardwareTokenInterface::Create<JubiterLiteETHImpl>);
        Register(TWCoinType::TWCoinTypeEOS,         &HardwareTokenInterface::Create<JubiterLiteEOSImpl>);
        Register(TWCoinType::TWCoinTypeXRP,         &HardwareTokenInterface::Create<JubiterLiteXRPImpl>);
        Register(TWCoinType::TWCoinTypeTron,        &HardwareTokenInterface::Create<JubiterLiteTRXImpl>);
    }
}; // class xJuBiterLITEMISCFactory end


class xMISCTokenFactory {
protected:
    xTrezorCryptoMISCFactory     trezorFactory;
    xJuBiterBladeMISCFactory jubiterBLDFactory;
    xJuBiterBIOMISCFactory   jubiterBIOFactory;
    xJuBiterLITEMISCFactory jubiterLITEFactory;

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
            return jubiterLITEFactory.Create(type, deviceID);
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
            return jubiterLITEFactory.Create(type, deviceID);
        }
#endif  // #if defined(NFC_MODE) end
//        else {
            return nullptr;
//        }
    }
}; // class xMISCTokenFactory end

using MISCseriesTokenFactory = Singleton<xMISCTokenFactory>;


} // namespace token end
} // namespace jub end
