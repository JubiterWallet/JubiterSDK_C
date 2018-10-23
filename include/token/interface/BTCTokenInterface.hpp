#ifndef __BTCTokenInterface__
#define __BTCTokenInterface__

#include <JUB_SDK.h>
#include <vector>

namespace jub {

	class BTCTokenInterface {

	public:
		virtual JUB_RV selectApplet_BTC() = 0;
		virtual JUB_RV setUnit_BTC(JUB_BTC_UNIT_TYPE unit) = 0;
		virtual JUB_RV setForkID_BTC(JUB_UINT16 forkID) = 0;
		virtual JUB_RV signTX_BTC(JUB_BTC_TRANS_TYPE type, JUB_UINT16 input_count, std::vector<JUB_UINT64> input_amount, std::vector<std::string> input_path, std::vector<JUB_UINT16> change_index, \
			std::vector<std::string> change_path, std::vector<JUB_BYTE> unsiged_trans, std::vector<JUB_BYTE>& raw) = 0;

		virtual JUB_RV getHDNode_BTC(JUB_BTC_TRANS_TYPE type, std::string path, std::string& xpub) = 0;
		virtual JUB_RV getAddress_BTC(JUB_BTC_TRANS_TYPE type, std::string path, JUB_UINT16 bshow, std::string& address) = 0;
	};

}



























#endif