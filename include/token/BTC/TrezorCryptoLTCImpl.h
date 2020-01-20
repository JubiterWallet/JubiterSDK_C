#pragma once

#include <token/BTC/TrezorCryptoBTCImpl.h>

namespace jub {
	namespace token {


		class TrezorCryptoLTCImpl : public TrezorCryptoBTCImpl {

		public:
			TrezorCryptoLTCImpl(const std::string& xprv) :
				TrezorCryptoBTCImpl(xprv) {};
			~TrezorCryptoLTCImpl() {};

			static std::shared_ptr<BTCTokenInterface> Create(const std::string& xprv) { return std::make_shared<TrezorCryptoLTCImpl>(xprv); }

		};

	}
}