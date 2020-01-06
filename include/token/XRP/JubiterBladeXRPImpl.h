#pragma once

#include <token/JubiterBlade/JubiterBladeToken.h>
#include <token/interface/XRPTokenInterface.hpp>

namespace jub {
	namespace token {

		class JubiterBladeXRPImpl :
			public JubiterBladeToken,
			public XRPTokenInterface {

		public:
			JubiterBladeXRPImpl(JUB_UINT16 deviceID) :
				JubiterBladeToken(deviceID) {};

			~JubiterBladeXRPImpl() {};

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