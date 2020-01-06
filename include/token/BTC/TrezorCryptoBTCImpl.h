#pragma once
#include <token/TrezorCrypto/TrezorCryptoToken.h>
#include <token/interface/BTCTokenInterface.hpp>

namespace jub {
	namespace token {
		class TrezorCryptoBTCImpl :
			public TrezorCryptoToken,
			virtual public BTCTokenInterface {

		public:
			TrezorCryptoBTCImpl(const std::string& xprv) :
				TrezorCryptoToken(xprv){};
			~TrezorCryptoBTCImpl() {};

			//BTC functions
			virtual JUB_RV SelectAppletBTC();
			virtual JUB_RV GetHDNodeBTC(const JUB_ENUM_BTC_TRANS_TYPE& type, const std::string& path, std::string& xpub);
			virtual JUB_RV GetAddressBTC(const JUB_BYTE addrFmt, const JUB_ENUM_BTC_TRANS_TYPE& type, const std::string& path, const JUB_UINT16 tag, std::string& address);
			virtual JUB_RV SetUnitBTC(const JUB_ENUM_BTC_UNIT_TYPE& unit);
			virtual JUB_RV SetCoinTypeBTC(const JUB_ENUM_COINTYPE_BTC& type);
			virtual JUB_RV SignTXBTC(const JUB_BYTE addrFmt,
				const JUB_ENUM_BTC_TRANS_TYPE& type,
				const JUB_UINT16 inputCount,
				const std::vector<JUB_UINT64>& vInputAmount,
				const std::vector<std::string>& vInputPath,
				const std::vector<JUB_UINT16>& vChangeIndex,
				const std::vector<std::string>& vChangePath,
				const std::vector<JUB_BYTE>& vUnsigedTrans,
				std::vector<JUB_BYTE>& vRaw);

			//QTUM functions
			virtual JUB_RV SetQRC20ETHToken(const std::string& tokenName,
				const JUB_UINT16 unitDP,
				const std::string& contractAddress);

		};
	}//namespace token
}//namespace jub