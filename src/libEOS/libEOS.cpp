#include "JUB_SDK_EOS.h"
#include "libEOS/libEOS.hpp"

#include <bigint/BigIntegerUtils.hh>
#include "EOS/Transaction.h"
#include "HexCoding.h"

namespace jub {

namespace eos {

JUB_RV serializePreimage(const std::string& expiration,
                         const std::string& referenceBlockId,
                         const std::string& referenceBlockTime,
                         const std::string& currency,
                         const std::string& from,
                         const std::string& to,
                         const std::string& asset,
                         const std::string& memo,
                         uchar_vector& preimageRaw) {

    try {
        // build transaction
        int32_t bigExpiration = stringToBigInteger(expiration).toInt();
        uchar_vector vReferenceBlockId(referenceBlockId);
        int32_t bigRefBlockTime = stringToBigInteger(referenceBlockTime).toInt();
        TW::EOS::Transaction tx {vReferenceBlockId, bigRefBlockTime, bigExpiration};

        TW::EOS::TransferAction action =
        TW::EOS::TransferAction(currency,
                                from, to,
                                TW::Bravo::Asset::fromString(asset),
                                memo);

        tx.actions.push_back(action);

        //Data vRaw;
        tx.serialize(preimageRaw);
        std::cout << "preimage: " << preimageRaw.getHex() << std::endl;
    }
    catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}

} // namespace eos end

} // namespace jub end
