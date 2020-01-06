#pragma once
#ifndef __ContextHC__
#define __ContextHC__

#include <JUB_SDK_Hcash.h>
#include <string>
#include <vector>
#include <utility/Singleton.h>
#include <utility/xManager.hpp>
#include <context/BTCContext.h>
#include <token/interface/HCTokenInterface.hpp>
#include "../libHC/libHC.h"

#define JUB_CHECK_CONTEXT_HC(x)                                     \
do {                                                                \
    auto context = jub::context::ContextManager::GetInstance()->GetOne(x);   \
    JUB_CHECK_NULL(context);                                        \
    const std::type_info& tCtx = typeid(*context);                  \
    const std::type_info& tCtxBTC = typeid(jub::context::HCContext);         \
    if (tCtx.hash_code() != tCtxBTC.hash_code()) {                  \
        return JUBR_ERROR_ARGS;                                     \
    }                                                               \
} while(0)

namespace jub {
	namespace context {

		class HCContextBase {

		public:
			virtual JUB_RV signTX(const JUB_ENUM_BTC_ADDRESS_FORMAT& addrFmt, const std::vector<INPUT_HC>& vInputs, const std::vector<OUTPUT_HC>& vOutputs, const std::string& unsignedTrans, std::string& raw) = 0;
			JUB_RV DeserializeTxHC(const std::string& raw) {
				return hc::DeserializeTx(raw, _tx);
			};
			JUB_RV SerializeTxHC(std::string& raw) {
				return hc::SerializeTx(_tx, raw);
			};

		protected:
			hc::TX_Hcash _tx;
		}; // class HCContextBase end

		class HCContext : public BTCContext,
			public HCContextBase {

		public:
			HCContext(CONTEXT_CONFIG_HC cfg, std::shared_ptr<token::HCTokenInterface> tokenPtr):
			BTCContext(CONTEXT_CONFIG_BTC{ cfg.mainPath,COINBTC,p2pkh },std::dynamic_pointer_cast<token::BTCTokenInterface>(tokenPtr)),
			_tokenPtr(tokenPtr)
			{
				_timeout = 120 * 2;
			};
			// Remove c++ features for swift framework end
			~HCContext() {};

			virtual JUB_RV signTX(const JUB_ENUM_BTC_ADDRESS_FORMAT& addrFmt, const std::vector<INPUT_HC>& vInputs, const std::vector<OUTPUT_HC>& vOutputs, const std::string& unsignedTrans, std::string& raw) override;
			virtual HCContext* GetClassType(void) {
				return this;
			}
			virtual JUB_RV ActiveSelf() override;
		protected:
			std::shared_ptr<token::HCTokenInterface> _tokenPtr;
		}; // class ContextBTC end
	}//namespace context end
} // namespace jub end

#endif // #pragma once
