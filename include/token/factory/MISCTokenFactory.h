#pragma once
//only for other than BTC series coins now, maybe for all coins later
#include <string>
#include <memory>

#include "utility/xFactory.hpp"
#include "utility/Singleton.h"

#if defined(GRPC_MODE)
#include "device/JubiterBridgeDevice.hpp"
#endif // #if defined(GRPC_MODE) end
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

#include "token/ETH/JubiterNFCETHImpl.h"
#include "token/EOS/JubiterNFCEOSImpl.h"
#include "token/XRP/JubiterNFCXRPImpl.h"
#include "token/TRX/JubiterNFCTRXImpl.h"

#include <TrustWalletCore/TWCoinType.h>


namespace jub {
namespace token {


typedef std::shared_ptr<BaseToken>(*CreateTrezorCryptoMISCFn)(const std::string&);
typedef std::shared_ptr<BaseToken>(*CreateJubiterMISCFn)(JUB_UINT16);


class xTrezorCryptoMISCFactory :
public xFactory<std::shared_ptr<BaseToken>,
                TWCoinType,
                CreateTrezorCryptoMISCFn> {
public:
    xTrezorCryptoMISCFactory() {
        Register(TWCoinType::TWCoinTypeHcash,            &TrezorCryptoHCImpl::Create);
        Register(TWCoinType::TWCoinTypeEthereum,        &TrezorCryptoETHImpl::Create);
//        Register(TWCoinType::TWCoinTypeEthereumClassic, &TrezorCryptoETHImpl::Create);
        Register(TWCoinType::TWCoinTypeEOS,             &TrezorCryptoEOSImpl::Create);
        Register(TWCoinType::TWCoinTypeXRP,             &TrezorCryptoXRPImpl::Create);
        Register(TWCoinType::TWCoinTypeTron,            &TrezorCryptoTRXImpl::Create);
    }
}; // class xTrezorCryptoMISCFactory end


class xJuBiterBladeMISCFactory :
public xFactory<std::shared_ptr<BaseToken>,
                TWCoinType,
                CreateJubiterMISCFn> {
public:
    xJuBiterBladeMISCFactory() {
        Register(TWCoinType::TWCoinTypeHcash,            &JubiterBladeHCImpl::Create);
        Register(TWCoinType::TWCoinTypeEthereum,        &JubiterBladeETHImpl::Create);
//        Register(TWCoinType::TWCoinTypeEthereumClassic, &JubiterBladeETHImpl::Create);
        Register(TWCoinType::TWCoinTypeEOS,             &JubiterBladeEOSImpl::Create);
        Register(TWCoinType::TWCoinTypeXRP,             &JubiterBladeXRPImpl::Create);
        Register(TWCoinType::TWCoinTypeTron,            &JubiterBladeTRXImpl::Create);
    }
}; // class xJuBiterBladeMISCFactory end


class xJuBiterBIOMISCFactory :
public xFactory<std::shared_ptr<BaseToken>,
                TWCoinType,
                CreateJubiterMISCFn> {
public:
    xJuBiterBIOMISCFactory() {
        Register(TWCoinType::TWCoinTypeHcash,            &JubiterBIOHCImpl::Create);
        Register(TWCoinType::TWCoinTypeEthereum,        &JubiterBIOETHImpl::Create);
//        Register(TWCoinType::TWCoinTypeEthereumClassic, &JubiterBIOETHImpl::Create);
        Register(TWCoinType::TWCoinTypeEOS,             &JubiterBIOEOSImpl::Create);
        Register(TWCoinType::TWCoinTypeXRP,             &JubiterBIOXRPImpl::Create);
        Register(TWCoinType::TWCoinTypeTron,            &JubiterBIOTRXImpl::Create);
    }
}; // class xJuBiterBIOMISCFactory end


class xJuBiterNFCMISCFactory :
public xFactory<std::shared_ptr<BaseToken>,
                TWCoinType,
                CreateJubiterMISCFn> {
public:
    xJuBiterNFCMISCFactory() {
        Register(TWCoinType::TWCoinTypeEthereum,        &JubiterNFCETHImpl::Create);
//        Register(TWCoinType::TWCoinTypeEthereumClassic, &JubiterNFCETHImpl::Create);
        Register(TWCoinType::TWCoinTypeEOS,             &JubiterNFCEOSImpl::Create);
        Register(TWCoinType::TWCoinTypeXRP,             &JubiterNFCXRPImpl::Create);
        Register(TWCoinType::TWCoinTypeTron,            &JubiterNFCTRXImpl::Create);
    }
}; // class xJuBiterNFCMISCFactory end


class xMISCTokenFactory {
protected:
    xTrezorCryptoMISCFactory     trezorFactory;
    xJuBiterBladeMISCFactory jubiterBLDFactory;
    xJuBiterBIOMISCFactory   jubiterBIOFactory;
    xJuBiterNFCMISCFactory   jubiterNFCFactory;

public:
    std::shared_ptr<BaseToken> CreateToken(const TWCoinType& type, const std::string& XPRVorXPUB) {
        return trezorFactory.Create(type, XPRVorXPUB);
    }

    std::shared_ptr<BaseToken> CreateToken(const TWCoinType& type, const JUB_UINT16 deviceID) {
#if defined(GRPC_MODE)
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
            return jubiterNFCFactory.Create(type, deviceID);
        }
#endif  // #if defined(GRPC_MODE) end
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
            return jubiterNFCFactory.Create(type, deviceID);
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
