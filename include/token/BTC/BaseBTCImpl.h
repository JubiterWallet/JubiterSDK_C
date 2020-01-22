#pragma once
#include <token/interface/BTCTokenInterface.hpp>
#include <TrustWalletCore/TWCoinType.h>

namespace jub {
	namespace token {
		class BaseBTCImpl :
			virtual public BTCTokenInterface {
		public:
			BaseBTCImpl() {
				_coin = TWCoinType::TWCoinTypeBitcoin;
			};


		protected:
			// add curve, prefix here
			TWCoinType _coin;

		};
	}
}