#include <token/BTC/JubiterBaseBTCImpl.h>
#include <libBTC/libBTC.hpp>


namespace jub {
namespace token {


JUB_RV JubiterBaseBTCImpl::SerializeUnsignedTx(const JUB_ENUM_BTC_TRANS_TYPE& type,
                                               const std::vector<INPUT_BTC>& vInputs,
                                               const std::vector<OUTPUT_BTC>& vOutputs,
                                               const JUB_UINT32 lockTime,
                                               uchar_vector& unsignedRaw) {

    return jub::btc::serializeUnsignedTx(_coin,
                                         type,
                                         vInputs,
                                         vOutputs,
                                         lockTime,
                                         unsignedRaw);
}


} // namespace token end
} // namespace jub end