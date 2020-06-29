#include <utility/util.h>
#include <token/BTC/JubiterBladeQTUMImpl.h>
#include <token/ErrorHandler.h>


namespace jub {
namespace token {


JUB_RV JubiterBladeQTUMImpl::SetQRC20ETHToken(const std::string& tokenName,
                                              const JUB_UINT16 unitDP,
                                              const std::string& contractAddress) {

    uchar_vector lvName = Tollv(tokenName);
    uchar_vector address;
    address << ETHHexStr2CharPtr(contractAddress);

    uchar_vector data;
    data << (uint8_t)unitDP;
    data << (uint8_t)lvName.size();
    data << lvName;
    data << (uint8_t)address.size();
    data << address;

    APDU apdu(0x00, 0xC7, 0x00, 0x00, (JUB_ULONG)data.size(), data.data());
    JUB_UINT16 ret = 0;
    JUB_VERIFY_RV(_SendApdu(&apdu, ret));
    JUB_VERIFY_COS_ERROR(ret);

    return JUBR_OK;
}


} // namespace token end
} // namespace jub end
