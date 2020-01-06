#pragma once

#include <token/interface/EOSTokenInterface.hpp>
#include <token/TrezorCrypto/TrezorCryptoToken.h>

namespace jub {
	namespace token {

		class TrezorCryptoEOSImpl :
			public TrezorCryptoToken,
			public EOSTokenInterface {

		public:
			TrezorCryptoEOSImpl(const std::string& xprv) :
				TrezorCryptoToken(xprv) {};

			~TrezorCryptoEOSImpl() {};


			//EOS functions
			virtual JUB_RV SelectAppletEOS();
			virtual JUB_RV SetCoinTypeEOS();
			virtual JUB_RV GetAddressEOS(const TW::EOS::Type& type, const std::string& path, const JUB_UINT16 tag, std::string& address);
			virtual JUB_RV GetHDNodeEOS(const JUB_BYTE format, const std::string& path, std::string& pubkey);
			virtual JUB_RV SignTXEOS(const TW::EOS::Type& type,
				const std::vector<JUB_BYTE>& vPath,
				const std::vector<JUB_BYTE>& vChainId,
				const std::vector<JUB_BYTE>& vRaw,
				std::vector<uchar_vector>& vSignatureRaw,
				const bool bWithType = false);

		};

	}
}