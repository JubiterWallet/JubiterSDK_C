#include <context/Context.h>
#include <token/interface/TokenInterface.hpp>
#include <utility/util.hpp>

namespace jub {

JUB_RV Context::showVirtualPwd() {

    auto token = jub::TokenManager::GetInstance()->getOne(_deviceID);
    JUB_CHECK_NULL(token);

    JUB_VERIFY_RV(token->showVirtualPwd());

    return JUBR_OK;
}

JUB_RV Context::cancelVirtualPwd() {

    auto token = jub::TokenManager::GetInstance()->getOne(_deviceID);
    JUB_CHECK_NULL(token);

    JUB_VERIFY_RV(token->cancelVirtualPwd());

    return JUBR_OK;
}

JUB_RV Context::verifyPIN(JUB_CHAR_PTR pinMix, OUT JUB_ULONG &retry) {

    auto token = jub::TokenManager::GetInstance()->getOne(_deviceID);
    JUB_CHECK_NULL(token);

    JUB_VERIFY_RV(token->verifyPIN(pinMix, retry));

    return JUBR_OK;
}

JUB_RV Context::setTimeout(JUB_UINT16 timeout) {

    _timeout = timeout;

    auto token = jub::TokenManager::GetInstance()->getOne(_deviceID);
    JUB_CHECK_NULL(token);

    JUB_VERIFY_RV(token->setTimeout(_timeout));

    return JUBR_OK;
}

std::string Context::full_bip32_path(BIP32_Path path) {

    return _main_path + "/" + jub::to_string(path.change) + "/" + jub::to_string(path.addressIndex);
}

} // namespace jub end
