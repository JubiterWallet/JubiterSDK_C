#ifndef __ETHTokenInterface__
#define __ETHTokenInterface__

#include <JUB_SDK.h>
#include <vector>

namespace jub {

	class ETHTokenInterface {

	public:
		virtual JUB_RV selectApplet_ETH() = 0;
		virtual JUB_RV getAddress_ETH(std::string path, JUB_UINT16 tag, std::string& address) = 0;
		virtual JUB_RV getHDNode_ETH(JUB_BYTE format ,std::string path, std::string& pubkey) = 0;
		virtual JUB_RV signTX_ETH(bool Is_ERC20, std::vector<JUB_BYTE> nonce, 
			std::vector<JUB_BYTE> gasPrice,
			std::vector<JUB_BYTE> gasLimit,
			std::vector<JUB_BYTE> To, 
			std::vector<JUB_BYTE> value, 
			std::vector<JUB_BYTE> data, 
			std::vector<JUB_BYTE> path, 
			std::vector<JUB_BYTE> chainID,
			std::vector<JUB_BYTE>& raw) = 0;
	};

}


#endif
