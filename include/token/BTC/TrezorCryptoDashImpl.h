#pragma once

#include <token/BTC/TrezorCryptoBTCImpl.h>

namespace jub {
	namespace token {


		class TrezorCryptoDashImpl : public TrezorCryptoBTCImpl {

		public:
			TrezorCryptoDashImpl(const std::string& xprv) :
				TrezorCryptoBTCImpl(xprv) {};
			~TrezorCryptoDashImpl() {};

			static std::shared_ptr<BTCTokenInterface> Create(const std::string& xprv) { return std::make_shared<TrezorCryptoDashImpl>(xprv); }

		};

	}
}