#pragma once

#include <token/BTC/JubiterBladeBTCImpl.h>

namespace jub {
	namespace token {


		class JubiterBladeLTCImpl : public JubiterBladeBTCImpl {

		public:
			JubiterBladeLTCImpl(JUB_UINT16 deviceID) :
				JubiterBladeBTCImpl(deviceID) {};
			~JubiterBladeLTCImpl() {};

			static std::shared_ptr<BTCTokenInterface> Create(JUB_UINT16 deviceID) { return std::make_shared<JubiterBladeLTCImpl>(deviceID); }

		};

	}
}