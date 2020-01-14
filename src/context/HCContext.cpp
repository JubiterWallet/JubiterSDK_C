#include "JUB_SDK_Hcash.h"
#include "context/HCContext.h"
#include "utility/util.h"
#include "token/interface/HCTokenInterface.hpp"
#include "mSIGNA/stdutils/uchar_vector.h"
#include <TrezorCrypto/ecdsa.h>
#include <TrezorCrypto/secp256k1.h>

namespace jub {
	namespace context {
		JUB_RV HCContext::ActiveSelf() {

			JUB_VERIFY_RV(_tokenPtr->selectApplet_HC());
			BTCContext::SetTimeout(_timeout);

			return JUBR_OK;
		}

		JUB_RV HCContext::signTX(const JUB_ENUM_BTC_ADDRESS_FORMAT& addrFmt, const std::vector<INPUT_HC>& vInputs, const std::vector<OUTPUT_HC>& vOutputs, const std::string& unsignedTrans, std::string& raw) {

			//deal inputs
			std::vector<JUB_UINT64> vInputAmount;
			std::vector<std::string> vInputPath;
			for (auto input : vInputs) {
				vInputAmount.push_back(input.amount);
				vInputPath.push_back(_FullBip44Path(input.path));
			}

			//deal outputs
			std::vector<JUB_UINT16> vChangeIndex;
			std::vector<std::string> vChangePath;
			for (std::size_t i = 0, e = vOutputs.size(); i != e; ++i) {
				if (vOutputs[i].changeAddress) {
					vChangeIndex.push_back((JUB_UINT16)i);
					vChangePath.push_back(_FullBip44Path(vOutputs[i].path));
				}
			}

			uchar_vector vUnsignedTrans(unsignedTrans);

			uchar_vector vRaw;
			JUB_VERIFY_RV(_tokenPtr->SignTXBTC(JUB_ENUM_BTC_ADDRESS_FORMAT::OWN,
				p2pkh,
				(JUB_UINT16)vInputs.size(),
				vInputAmount,
				vInputPath,
				vChangeIndex,
				vChangePath,
				vUnsignedTrans,
				vRaw));

			raw = vRaw.getHex();

			return JUBR_OK;
		}
	}//namespace context end
} // namespace jub end