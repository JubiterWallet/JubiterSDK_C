#include <token/BTC/TrezorCryptoBCHImpl.h>
#include <Bitcoin/CashAddress.h>

namespace jub {
namespace token {


JUB_RV TrezorCryptoBCHImpl::_GetAddress(const TW::Data publicKey, std::string& address) {
    
    try {
        TW::Bitcoin::CashAddress addr(TW::PublicKey(TW::Data(publicKey), _publicKeyType));
        address = addr.string();
    }
    catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }
    
    return JUBR_OK;
}


} // namespace token end
} // namespace jub end
