#ifndef __BTCTokenInterface__
#define __BTCTokenInterface__

#include "JUB_SDK_BTC.h"

#include <vector>
#include <string>
#include <token/interface/BaseToken.h>

namespace jub {
	namespace token {

		class BTCTokenInterface :virtual public BaseToken{
		public:
			virtual ~BTCTokenInterface() = default;

			virtual JUB_RV SelectAppletBTC() = 0;
			virtual JUB_RV SetUnitBTC(const JUB_ENUM_BTC_UNIT_TYPE& unit) = 0;
			virtual JUB_RV SetCoinTypeBTC(const JUB_ENUM_COINTYPE_BTC& type) = 0;
			virtual JUB_RV SignTXBTC(const JUB_BYTE addrFmt,
				const JUB_ENUM_BTC_TRANS_TYPE& type,
				const JUB_UINT16 inputCount,
				const std::vector<JUB_UINT64>& vInputAmount,
				const std::vector<std::string>& vInputPath,
				const std::vector<JUB_UINT16>& vChangeIndex,
				const std::vector<std::string>& vChangePath,
				const std::vector<JUB_BYTE>& vUnsigedTrans,
				std::vector<JUB_BYTE>& vRaw) = 0;

			virtual JUB_RV GetHDNodeBTC(const JUB_ENUM_BTC_TRANS_TYPE& type, const std::string& path, std::string& xpub) = 0;
			virtual JUB_RV GetAddressBTC(const JUB_BYTE addrFmt,
				const JUB_ENUM_BTC_TRANS_TYPE& type,
				const std::string& path,
				const JUB_UINT16 tag,
				std::string& address) = 0;

			virtual JUB_RV SetQRC20ETHToken(const std::string& tokenName,
				const JUB_UINT16 unitDP,
				const std::string& contractAddress) = 0;
		}; // class BTCTokenInterface end
	}//namespace token end
} // namespace jub end

#endif
