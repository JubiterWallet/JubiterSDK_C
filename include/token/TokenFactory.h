#pragma once
//only for BTC series coins now, maybe for all coins later

#include <JUB_SDK_BTC.h>
#include <utility/xFactory.hpp>
#include <utility/Singleton.h>

#include <token/interface/BTCTokenInterface.hpp>

#include <token/BTC/JubiterBladeBTCImpl.h>
#include <token/BTC/JubiterBladeBCHImpl.h>
#include <token/BTC/JubiterBladeLTCImpl.h>
#include <token/BTC/JubiterBladeUSDTImpl.h>
#include <token/BTC/JubiterBladeDashImpl.h>
#include <token/BTC/JubiterBladeQTUMImpl.h>

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


typedef  std::shared_ptr<BTCTokenInterface>(*CreateTrezorCryptoBTCFn)(const std::string&);
typedef  std::shared_ptr<BTCTokenInterface>(*CreateJubiterBladeBTCFn)(JUB_UINT16);


class xTrezorCryptoBTCFactory :
    public xFactory<std::shared_ptr<BTCTokenInterface>, JUB_ENUM_COINTYPE_BTC, CreateTrezorCryptoBTCFn> {
public:
    xTrezorCryptoBTCFactory() {
        Register(JUB_ENUM_COINTYPE_BTC::COINBTC, &TrezorCryptoBTCImpl::Create);
        Register(JUB_ENUM_COINTYPE_BTC::COINBCH, &TrezorCryptoBCHImpl::Create);
        Register(JUB_ENUM_COINTYPE_BTC::COINLTC, &TrezorCryptoLTCImpl::Create);
        Register(JUB_ENUM_COINTYPE_BTC::COINUSDT, &TrezorCryptoUSDTImpl::Create);
        Register(JUB_ENUM_COINTYPE_BTC::COINDASH, &TrezorCryptoDashImpl::Create);
        Register(JUB_ENUM_COINTYPE_BTC::COINQTUM, &TrezorCryptoQTUMImpl::Create);
    };
}; // class xTrezorCryptoBTCFactory end


class xJubiterBladeBTCFactory :
    public xFactory<std::shared_ptr<BTCTokenInterface>, JUB_ENUM_COINTYPE_BTC, CreateJubiterBladeBTCFn> {
public:
    xJubiterBladeBTCFactory() {
        Register(JUB_ENUM_COINTYPE_BTC::COINBTC, &JubiterBladeBTCImpl::Create);
        Register(JUB_ENUM_COINTYPE_BTC::COINBCH, &JubiterBladeBCHImpl::Create);
        Register(JUB_ENUM_COINTYPE_BTC::COINLTC, &JubiterBladeLTCImpl::Create);
        Register(JUB_ENUM_COINTYPE_BTC::COINUSDT, &JubiterBladeUSDTImpl::Create);
        Register(JUB_ENUM_COINTYPE_BTC::COINDASH, &JubiterBladeDashImpl::Create);
        Register(JUB_ENUM_COINTYPE_BTC::COINQTUM, &JubiterBladeQTUMImpl::Create);
    };
}; // class xJubiterBladeBTCFactory end


class xJubiterNFCBTCFactory :
    public xFactory<std::shared_ptr<BTCTokenInterface>, JUB_ENUM_COINTYPE_BTC, CreateJubiterBladeBTCFn> {
public:
    xJubiterNFCBTCFactory() {
        Register(JUB_ENUM_COINTYPE_BTC::COINBTC, &JubiterNFCBTCImpl::Create);
        Register(JUB_ENUM_COINTYPE_BTC::COINBCH, &JubiterNFCBCHImpl::Create);
        Register(JUB_ENUM_COINTYPE_BTC::COINLTC, &JubiterNFCLTCImpl::Create);
        Register(JUB_ENUM_COINTYPE_BTC::COINUSDT, &JubiterNFCUSDTImpl::Create);
        Register(JUB_ENUM_COINTYPE_BTC::COINDASH, &JubiterNFCDashImpl::Create);
        Register(JUB_ENUM_COINTYPE_BTC::COINQTUM, &JubiterNFCQTUMImpl::Create);
    };
}; // class xJubiterNFCBTCFactory end


class xBTCTokenFactory {
protected:
    xTrezorCryptoBTCFactory  trezorFactory;
    xJubiterBladeBTCFactory  jubiterFactory;
    xJubiterNFCBTCFactory  jubiterNFCFactory;

public:
    std::shared_ptr<BTCTokenInterface> CreateToken(JUB_ENUM_COINTYPE_BTC type, std::string xprv) {
        return trezorFactory.Create(type, xprv);
    };

    std::shared_ptr<BTCTokenInterface> CreateToken(JUB_ENUM_COINTYPE_BTC type, JUB_UINT16 devieID) {
//        return jubiterFactory.Create(type, devieID);
        return jubiterNFCFactory.Create(type, devieID);
    };
}; // class xBTCTokenFactory end

using BTCseriesTokenFactory = Singleton<xBTCTokenFactory>;


} // namespace token end
} // namespace jub end
