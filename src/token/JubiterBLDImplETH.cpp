#include <JUB_SDK.h>
#include <token/JubiterBLDImpl.h>
#include <utility/util.hpp>
#include <utility/util.h>
#include <cassert>
#include <utility/uchar_vector.h>


namespace jub {



// 	constexpr JUB_BYTE PKIAID_ETH[16] = {
// 		0xD1, 0x56, 0x00, 0x01, 0x32, 0x83, 0x00, 0x42, 0x4C, 0x44, 0x00, 0x00, 0x45, 0x54, 0x48, 0x01
// 	};


	constexpr JUB_BYTE PKIAID_ETH[15] = {
		0xD1, 0x56, 0x00, 0x01, 0x32, 0x83, 0x00, 0x42, 0x4C, 0x44, 0x00, 0x45, 0x54, 0x48, 0x01
	};



#define SWITCH_TO_ETH_APP  do {				                    \
		JUB_VERIFY_RV(_selectApp(PKIAID_ETH,15));				\
	} while (0);                                                \


	JUB_RV JubiterBLDImpl::selectApplet_ETH()
	{
		SWITCH_TO_ETH_APP;
		return JUBR_OK;
	}




}