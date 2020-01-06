#pragma once
#include <token/interface/XRPTokenInterface.hpp>
#include <token/TrezorCrypto/TrezorCryptoToken.h>

namespace jub {
	namespace token {

		class TrezorCryptoXRPImpl :
			public TrezorCryptoToken,
			public XRPTokenInterface {

		public:
			TrezorCryptoXRPImpl(const std::string& xprv) :
				TrezorCryptoToken(xprv) {};

			~TrezorCryptoXRPImpl() {};


			//XRP functions
			virtual JUB_RV SelectAppletXRP();
			virtual JUB_RV SetCoinTypeXRP();
			virtual JUB_RV GetAddressXRP(const std::string& path, const JUB_UINT16 tag, std::string& address);
			virtual JUB_RV GetHDNodeXRP(const JUB_BYTE format, const std::string& path, std::string& pubkey);
			virtual JUB_RV SignTXXRP(const std::vector<JUB_BYTE>& vPath,
				std::vector<JUB_BYTE>& vUnsignedRaw,
				std::vector<uchar_vector>& vSignatureRaw);
		};

	}
}