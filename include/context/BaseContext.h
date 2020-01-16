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
#include <token/interface/BaseToken.h>

namespace jub {
namespace context {


class BaseContext {
public:
    BaseContext(std::shared_ptr<token::BaseToken> tokenPtr) :_tokenPtr(tokenPtr) {};
    virtual ~BaseContext() {};

    virtual JUB_RV ShowVirtualPwd();
    virtual JUB_RV CancelVirtualPwd();
    virtual JUB_RV VerifyPIN(JUB_CHAR_CPTR pinMix, OUT JUB_ULONG &retry);
    virtual JUB_RV ActiveSelf() = 0;
    virtual JUB_RV SetTimeout(const JUB_UINT16 timeout);

protected:
    std::string _mainPath;
    JUB_UINT16  _timeout;

    virtual std::string _FullBip44Path(const BIP44_Path& path);
    virtual std::string _FullBip48Path(const BIP48_Path& path);
    std::string _appletVersion;
    std::shared_ptr<token::BaseToken> _tokenPtr;
}; // class BaseContext end


class AutoContextManager :
    public xManager<BaseContext> {
public:
    BaseContext* GetOne(JUB_UINT16 ID) {

        auto it = _mapList.find(ID);
        if (it != _mapList.end()) {
            if (_last != it->second) {
                it->second->ActiveSelf();
            }
            _last = it->second;

            return it->second;
        }

        return nullptr;
    }
}; // class AutoContextManager end


using ContextManager = Singleton<AutoContextManager>;


} // namespace context end
} // namespace jub end

#endif // #pragma once
