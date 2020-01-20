#pragma once

#include <token/BTC/TrezorCryptoBTCImpl.h>

namespace jub {
	namespace token {


		class TrezorCryptoQTUMImpl : public TrezorCryptoBTCImpl {

		public:
			TrezorCryptoQTUMImpl(const std::string& xprv) :
				TrezorCryptoBTCImpl(xprv) {};
			~TrezorCryptoQTUMImpl() {};

			static std::shared_ptr<BTCTokenInterface> Create(const std::string& xprv) { return std::make_shared<TrezorCryptoQTUMImpl>(xprv); }

		};

	}
}