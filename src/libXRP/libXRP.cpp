#include "JUB_SDK_XRP.h"
#include "libXRP/libXRP.hpp"

#include <bigint/BigIntegerUtils.hh>
#include "Ripple/Transaction.h"
#include "HexCoding.h"

namespace jub {
namespace xrp {


// without "signingPubKey"
JUB_RV serializePreimage(const JUB_TX_XRP& tx,
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
            uchar_vector vRaw(raw);
            preimageRaw = vRaw;
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


} // namespace xrp end
} // namespace jub end
