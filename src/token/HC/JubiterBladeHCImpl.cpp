#include "token/HC/JubiterBladeHCImpl.h"
#include "utility/util.h"

namespace jub {
namespace token {


constexpr JUB_BYTE PKIAID_HC[14] = {
    0x63, 0x6F, 0x6D, 0x2E, 0x66, 0x74, 0x2E, 0x68, 0x63, 0x72, 0x61, 0x73, 0x68, 0x01
};


JUB_RV JubiterBladeHCImpl::SelectApplet() {

	JUB_VERIFY_RV(_SelectApp(PKIAID_HC, sizeof(PKIAID_HC)/sizeof(JUB_BYTE)));
    return JUBR_OK;
}


} // namespace token end
} // namespace jub end
