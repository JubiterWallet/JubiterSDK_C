#include <token/XRP/JubiterBaseXRPImpl.h>
#include <bigint/BigIntegerUtils.hh>
#include <TrezorCrypto/bip32.h>
#include <PublicKey.h>
#include <utility/util.h>
#include "Ripple/Address.h"
#include "Ripple/Transaction.h"
#include "HexCoding.h"


namespace jub {
namespace token {


// without "signingPubKey"
JUB_RV JubiterBaseXRPImpl::SerializePreimage(const JUB_TX_XRP& tx,
                                             uchar_vector& preimageRaw) {

    JUB_RV rv = JUBR_OK;

    int64_t fee   = stringToBigInteger(tx.fee).toLong();
    int64_t flags = stringToBigInteger(tx.flags).toLong();
    int32_t sequence = stringToBigInteger(tx.sequence).toInt();
    int32_t last_ledger_sequence = stringToBigInteger(tx.lastLedgerSequence).toInt();
    TW::Ripple::Address account(tx.account);
    switch (tx.type) {
    case JUB_ENUM_XRP_TX_TYPE::PYMT:
    {
        switch (tx.pymt.type) {
        case JUB_ENUM_XRP_PYMT_TYPE::DXRP:
        {
            int64_t amount = stringToBigInteger(tx.pymt.amount.value).toLong();
            TW::Ripple::Address destination(tx.pymt.destination);
            int64_t destination_tag = stringToBigInteger(tx.pymt.destinationTag).toLong();
            TW::Ripple::Transaction xrp =
            TW::Ripple::Transaction(amount, fee,
                                    flags, sequence, last_ledger_sequence,
                                    account,
                                    destination, destination_tag);

            TW::Data raw = xrp.getPreImage();
            preimageRaw = uchar_vector(raw);
            break;
        }
        case JUB_ENUM_XRP_PYMT_TYPE::NS_ITEM_PYMT_TYPE:
        default:
            rv = JUBR_ARGUMENTS_BAD;
            break;
        }
        break;
    }
    case JUB_ENUM_XRP_TX_TYPE::NS_ITEM_TX_TYPE:
    default:
        rv = JUBR_ARGUMENTS_BAD;
        break;
    }

    return rv;
}


JUB_RV JubiterBaseXRPImpl::_getPubkeyFromXpub(const std::string& xpub, TW::Data& publicKey,
                                              uint32_t hdVersionPub, uint32_t hdVersionPrv) {

    try {
        HDNode hdkey;
        uint32_t fingerprint = 0;
        if (0 != hdnode_deserialize(xpub.c_str(),
                                    hdVersionPub, hdVersionPrv,
                                    _curve_name, &hdkey, &fingerprint)) {
            return JUBR_ARGUMENTS_BAD;
        }

        uchar_vector vPublicKey(hdkey.public_key, sizeof(hdkey.public_key)/sizeof(uint8_t));
        publicKey = TW::Data(vPublicKey);
    }
    catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}


JUB_RV JubiterBaseXRPImpl::_getAddress(const TW::Data publicKey, std::string& address) {

    try {
        TW::PublicKey twpk = TW::PublicKey(publicKey, _publicKeyType);
        TW::Ripple::Address addr(twpk);
        address = addr.string();

    }
    catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}


} // namespace token end
} // namespace jub end
