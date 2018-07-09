#ifndef __ETHTokenInterface__
#define __ETHTokenInterface__

#include <JUB_SDK.h>
#include <vector>

namespace jub {

	class ETHTokenInterface {

	public:
		virtual JUB_RV selectApplet_ETH() = 0;

	};

}


#endif
