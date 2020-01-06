#pragma once
#include <string>
#include <token/interface/SoftwareTokenInterface.h>

namespace jub {
	namespace token {
		class TrezorCryptoToken :public SoftwareToken {

		public:
			TrezorCryptoToken(const std::string& xprv) :
				_MasterKey_XPRV(xprv) {};
			~TrezorCryptoToken() {};


		protected:
			std::string _MasterKey_XPRV;

		};
	}//namespace token
}//namespace jub