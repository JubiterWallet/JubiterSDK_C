#pragma once

#include <token/interface/HCTokenInterface.hpp>
#include <token/BTC/JubiterBladeBTCImpl.h>

namespace jub {
	namespace token {
	
		class JubiterBladeHCImpl :
			public JubiterBladeBTCImpl,
			public HCTokenInterface {

		public:
			JubiterBladeHCImpl(JUB_UINT16 deviceID) :
				JubiterBladeBTCImpl(deviceID) {};

			~JubiterBladeHCImpl() {};

			//HC functions
			virtual JUB_RV selectApplet_HC();

		};
	
	}
}