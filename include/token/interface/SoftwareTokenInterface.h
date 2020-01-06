#pragma once
#include "BaseToken.h"

namespace jub {
	namespace token {
		class SoftwareToken :virtual public BaseToken {
		public:
			virtual ~SoftwareToken() {};
		};
	}//namespace token
}//namespace jub