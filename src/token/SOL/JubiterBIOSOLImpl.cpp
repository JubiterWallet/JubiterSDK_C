#include <token/SOL/JubiterBIOSOLImpl.h>
#include "token/JubiterBlade/JubiterBladeToken.h"

namespace jub {
namespace token {


JUB_RV JubiterBIOSOLImpl::SelectApplet() {

    JUB_VERIFY_RV(JubiterBIOToken::_SelectApp(kPKIAID_SOL, sizeof(kPKIAID_SOL)/sizeof(JUB_BYTE)));
    return JUBR_OK;
}


} // namespace token end
} // namespace jub end
