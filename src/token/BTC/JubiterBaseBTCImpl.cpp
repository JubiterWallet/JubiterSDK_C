#include <token/BTC/JubiterBaseBTCImpl.h>
#include <libBTC/libBTC.hpp>


namespace jub {
namespace token {


TW::Hash::Hasher JubiterBaseBTCImpl::_getHasherForPublicKey() {
    switch (_publicKeyType) {
        case TWPublicKeyTypeSECP256k1:
        case TWPublicKeyTypeSECP256k1Extended:
        case TWPublicKeyTypeNIST256p1:
        case TWPublicKeyTypeNIST256p1Extended:
        case TWPublicKeyTypeED25519:
        case TWPublicKeyTypeED25519Blake2b:
            return TW::Hash::sha256ripemd;
    }
}


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
