
#include "JUB_SDK_COMM.h"
#if defined(NFC_MODE)

#include <utility/util.h>
#include <token/BTC/JubiterNFCQTUMImpl.h>
#include <token/ErrorHandler.h>


namespace jub {
namespace token {


JUB_RV JubiterNFCQTUMImpl::SetQRC20ETHToken(const std::string& tokenName,
                                            const JUB_UINT16 unitDP,
                                            const std::string& contractAddress) {

    return JUBR_OK;
}


} // namespace token end
} // namespace jub end

#endif //end NFC_MODE