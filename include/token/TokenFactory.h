#pragma once
//only for BTC series coins now, maybe for all coins later

#include <JUB_SDK_BTC.h>
#include <utility/xFactory.hpp>
#include <utility/Singleton.h>

#include <token/interface/BTCTokenInterface.hpp>
#include <token/BTC/JubiterBladeBTCImpl.h>
#include <token/BTC/TrezorCryptoBTCImpl.h>

#include <string>
#include <memory>



namespace jub {
	namespace token {
		typedef  std::shared_ptr<BTCTokenInterface>(*CreateTrezorCryptoBTCFn)(const std::string&);
		typedef  std::shared_ptr<BTCTokenInterface>(*CreateJubiterBladeBTCFn)(JUB_UINT16);


		class xTrezorCryptoBTCFactory :public xFactory<std::shared_ptr<BTCTokenInterface>, JUB_ENUM_COINTYPE_BTC, CreateTrezorCryptoBTCFn> {
		public:
			xTrezorCryptoBTCFactory() {

				Register(JUB_ENUM_COINTYPE_BTC::COINBTC, &TrezorCryptoBTCImpl::Create);

				//if we have tokens each coin, modify this.
				Register(JUB_ENUM_COINTYPE_BTC::COINBCH, &TrezorCryptoBTCImpl::Create);
				Register(JUB_ENUM_COINTYPE_BTC::COINLTC, &TrezorCryptoBTCImpl::Create);
				Register(JUB_ENUM_COINTYPE_BTC::COINDASH, &TrezorCryptoBTCImpl::Create);
				Register(JUB_ENUM_COINTYPE_BTC::COINQTUM, &TrezorCryptoBTCImpl::Create);

			};
		};

		class xJubiterBladeBTCFactory : public xFactory<std::shared_ptr<BTCTokenInterface>, JUB_ENUM_COINTYPE_BTC, CreateJubiterBladeBTCFn> {
		public:
			xJubiterBladeBTCFactory() {
				Register(JUB_ENUM_COINTYPE_BTC::COINBTC, &JubiterBladeBTCImpl::Create);

				//if we have tokens each coin, modify this.
				Register(JUB_ENUM_COINTYPE_BTC::COINBCH, &JubiterBladeBTCImpl::Create);
				Register(JUB_ENUM_COINTYPE_BTC::COINLTC, &JubiterBladeBTCImpl::Create);
				Register(JUB_ENUM_COINTYPE_BTC::COINDASH, &JubiterBladeBTCImpl::Create);
				Register(JUB_ENUM_COINTYPE_BTC::COINQTUM, &JubiterBladeBTCImpl::Create);
			};
		};


		class xBTCTokenFactory {

			protected:
				xTrezorCryptoBTCFactory  trezorFactory;
				xJubiterBladeBTCFactory  jubiterFactory;

			public:
				std::shared_ptr<BTCTokenInterface> CreateToken(JUB_ENUM_COINTYPE_BTC type, std::string xprv) {
					return trezorFactory.Create(type, xprv);
				};
				std::shared_ptr<BTCTokenInterface> CreateToken(JUB_ENUM_COINTYPE_BTC type, JUB_UINT16 devieID) {
					return jubiterFactory.Create(type, devieID);
				};
		};

		using BTCseriesTokenFactory = Singleton<xBTCTokenFactory>;
	}
}


