#pragma once
//only for other than ETH series coins now, maybe for all coins later
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

#include "token/interface/ETHTokenInterface.hpp"

#include "token/ETH/JubiterBladeETHImpl.h"

#include "token/ETH/JubiterBIOETHImpl.h"

#include "token/ETH/TrezorCryptoETHImpl.h"

#include "token/ETH/JubiterLiteETHImpl.h"

#include <TrustWalletCore/TWCoinType.h>


namespace jub {
namespace token {


typedef std::shared_ptr<BaseToken>(*CreateTrezorCryptoETHFn)(const std::string&);
typedef std::shared_ptr<BaseToken>(*CreateJubiterETHFn)(JUB_UINT16);


class xTrezorCryptoETHFactory :
public xFactory<std::shared_ptr<BaseToken>,
                TWCoinType,
                CreateTrezorCryptoETHFn> {
public:
    xTrezorCryptoETHFactory() {
        Register(TWCoinType::TWCoinTypeEthereum,        &TrezorCryptoETHImpl::Create);
//        Register(TWCoinType::TWCoinTypeEthereumClassic, &TrezorCryptoETHImpl::Create);
    }
}; // class xTrezorCryptoETHFactory end


class xJuBiterBladeETHFactory :
public xFactory<std::shared_ptr<BaseToken>,
                TWCoinType,
                CreateJubiterETHFn> {
public:
    xJuBiterBladeETHFactory() {
        Register(TWCoinType::TWCoinTypeEthereum,        &JubiterBladeETHImpl::Create);
//        Register(TWCoinType::TWCoinTypeEthereumClassic, &JubiterBladeETHImpl::Create);
    }
}; // class xJuBiterBladeETHFactory end


class xJuBiterBIOETHFactory :
public xFactory<std::shared_ptr<BaseToken>,
                TWCoinType,
                CreateJubiterETHFn> {
public:
    xJuBiterBIOETHFactory() {
        Register(TWCoinType::TWCoinTypeEthereum,        &JubiterBIOETHImpl::Create);
//        Register(TWCoinType::TWCoinTypeEthereumClassic, &JubiterBIOETHImpl::Create);
    }
}; // class xJuBiterBIOETHFactory end


//#if defined(NFC_MODE)
class xJuBiterLiteETHFactory :
public xFactory<std::shared_ptr<BaseToken>,
                TWCoinType,
                CreateJubiterETHFn> {
public:
    xJuBiterLiteETHFactory() {
        Register(TWCoinType::TWCoinTypeEthereum,        &JubiterLiteETHImpl::Create);
//        Register(TWCoinType::TWCoinTypeEthereumClassic, &JubiterLiteETHImpl::Create);
    }
}; // class xJuBiterLiteETHFactory end
//#endif // #if defined(NFC_MODE) end


class xETHTokenFactory {
protected:
    xTrezorCryptoETHFactory     trezorFactory;
    xJuBiterBladeETHFactory jubiterBLDFactory;
    xJuBiterBIOETHFactory   jubiterBIOFactory;
//#if defined(NFC_MODE)
    xJuBiterLiteETHFactory  jubiterLiteFactory;
//#endif // #if defined(NFC_MODE) end

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
            return jubiterLiteFactory.Create(type, deviceID);
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
