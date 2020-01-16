#include "utility/util.h"

#include "context/BaseContext.h"
#include <token/interface/HardwareTokenInterface.hpp>

namespace jub {
namespace context {
// 		template<typename T>
// 		T* CheckImpl(std::shared_ptr<void> f) { auto token = std::dynamic_pointer_cast<T>(f); }


JUB_RV BaseContext::ShowVirtualPwd() {

    auto token = std::dynamic_pointer_cast<token::HardwareTokenInterface>(_tokenPtr);
    if (!token) { return JUBR_IMPL_NOT_SUPPORT; }
    JUB_VERIFY_RV(token->ShowVirtualPwd());

    return JUBR_OK;
}


JUB_RV BaseContext::CancelVirtualPwd() {

    auto token = std::dynamic_pointer_cast<token::HardwareTokenInterface>(_tokenPtr);
    if (!token) { return JUBR_IMPL_NOT_SUPPORT; }

    JUB_VERIFY_RV(token->CancelVirtualPwd());

    return JUBR_OK;
}


JUB_RV BaseContext::VerifyPIN(JUB_CHAR_CPTR pinMix, OUT JUB_ULONG &retry) {

    auto token = std::dynamic_pointer_cast<token::HardwareTokenInterface>(_tokenPtr);
    if (!token) { return JUBR_IMPL_NOT_SUPPORT; }

    JUB_VERIFY_RV(token->VerifyPIN(pinMix, retry));

    return JUBR_OK;
}


JUB_RV BaseContext::SetTimeout(const JUB_UINT16 timeout) {

    _timeout = timeout;

    auto token = std::dynamic_pointer_cast<token::HardwareTokenInterface>(_tokenPtr);
    if (!token) { return JUBR_IMPL_NOT_SUPPORT; }

    JUB_VERIFY_RV(token->SetTimeout(_timeout));

    return JUBR_OK;
}


std::string BaseContext::_FullBip44Path(const BIP44_Path& path) {

    return _mainPath + "/"
        + std::to_string(path.change) + "/"
        + std::to_string(path.addressIndex);
}


std::string BaseContext::_FullBip48Path(const BIP48_Path& path) {

    return _mainPath + "/"
        + std::to_string(path.network) + "'" + "/"
        + std::to_string((int)path.role) + "'" + "/"
        + std::to_string(path.accountIndex) + "'" + "/"
        + std::to_string(path.keyIndex) + "'";
}


} // namespace context end
} // namespace jub end
