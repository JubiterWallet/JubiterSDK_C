#pragma once

#include <token/interface/HCTokenInterface.hpp>
#include <token/BTC/TrezorCryptoBTCImpl.h>

namespace jub {
	namespace token {

		class TrezorCryptoHCImpl :
			public TrezorCryptoBTCImpl,
			public HCTokenInterface {

		public:
			TrezorCryptoHCImpl(const std::string& xprv) :
				TrezorCryptoBTCImpl(xprv) {};

			~TrezorCryptoHCImpl() {};

			//HC functions
			virtual JUB_RV selectApplet_HC();

		};

	}
}