#pragma once
#ifndef __Context__
#define __Context__

#include "JUB_SDK.h"

#include <string>
#include <vector>
#include <iostream>
#include <memory>

#include "utility/Singleton.h"
#include "utility/xManager.hpp"

#include "token/interface/BaseToken.h"
#include "token/interface/SoftwareTokenInterface.hpp"

namespace jub {
namespace context {
#define CONTEXT_CHECK_TYPE(t)                                                   \
do {                                                                            \
    auto token = std::dynamic_pointer_cast<token::SoftwareTokenInterface>(_tokenPtr);    \
    if(token != nullptr) {                                                      \
        if(token->Type() < t) {                                                 \
            return JUBR_CONTEXT_NOT_SATISFIED;                                  \
        }                                                                       \
    }                                                                           \
} while (0);    

#define CONTEXT_CHECK_TYPE_NONE       CONTEXT_CHECK_TYPE(token::JUB_SoftwareTokenType::NONE)
#define CONTEXT_CHECK_TYPE_PUBLIC     CONTEXT_CHECK_TYPE(token::JUB_SoftwareTokenType::PUBLIC)
#define CONTEXT_CHECK_TYPE_PRIVATE    CONTEXT_CHECK_TYPE(token::JUB_SoftwareTokenType::PRIVATE)

class BaseContext {
public:
    //for Factory
    template<typename T, typename U>
    static T* Create(const U& cfg,
                     std::shared_ptr<token::BaseToken> tokenPtr) {
        return new T(cfg, tokenPtr);
    }

public:
    BaseContext(std::shared_ptr<token::BaseToken> tokenPtr) :_tokenPtr(tokenPtr) {}
    virtual ~BaseContext() {}

    virtual JUB_RV GetDeviceClass(OUT JUB_ENUM_DEVICE_PTR deviceClass);

    virtual JUB_RV ShowVirtualPwd();
    virtual JUB_RV CancelVirtualPwd();
    virtual JUB_RV VerifyPIN(JUB_CHAR_CPTR pinMix, OUT JUB_ULONG &retry);
    virtual JUB_RV ActiveSelf() = 0;
    virtual JUB_RV PreparatoryFlow() { return JUBR_OK; }
    virtual JUB_RV ParseTransaction(const std::string& incRaw,
                                    std::vector<INPUT_BTC>& vInputs,
                                    std::vector<OUTPUT_BTC>& vOutputs,
                                    JUB_UINT32_PTR plockTime) { return JUBR_OK; }
    virtual JUB_RV SetTimeout(const JUB_UINT16 timeout);
    virtual JUB_RV VerifyFingerprint(OUT JUB_ULONG &retry);

protected:
    std::string _mainPath;
    // Mainnet(TWCoinTypeBitcoin) or Testnet(TWCoinTypeBitcoinTestNet)
    TWCoinType _coinNet;
    JUB_UINT16 _timeout;

    virtual std::string _FullBip44Path(const BIP44_Path& path);
    virtual std::string _FullBip48Path(const BIP48_Path& path);
    stVersionExp _appletVersion;
    std::shared_ptr<token::BaseToken> _tokenPtr;
}; // class BaseContext end


class AutoContextManager :
    public xManager<BaseContext> {
public:
	template<typename T>
    T* GetOneSafe(const JUB_UINT16 ID) {
		BaseContext* pContext= GetOne(ID);
		T* t = dynamic_cast<T*>(pContext); 
		if (t != nullptr) {
			if (_last != pContext) {
				pContext->ActiveSelf();
				_last = pContext;
			}
		}
		return t;
    }
}; // class AutoContextManager end


using ContextManager = Singleton<AutoContextManager>;


} // namespace context end
} // namespace jub end

#endif // #pragma once
