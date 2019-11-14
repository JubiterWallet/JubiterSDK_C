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
                         const std::string& actionsInJSON,
                         uchar_vector& preimageRaw,
                         const bool bWithType) {

    JUB_RV rv = JUBR_OK;

    try {
        // build transaction
        int32_t bigExpiration = stringToBigInteger(expiration).toInt();
        uchar_vector vReferenceBlockId(referenceBlockId);
        int32_t bigRefBlockTime = stringToBigInteger(referenceBlockTime).toInt();
        TW::EOS::Transaction tx {vReferenceBlockId, bigRefBlockTime, bigExpiration};

        nlohmann::json jsonActions = nlohmann::json::parse(actionsInJSON);
        if (nlohmann::detail::value_t::array != jsonActions.type()) {
            return JUBR_ARGUMENTS_BAD;
        }

        for (size_t i=0; i<jsonActions.size(); ++i) {
            JUB_ENUM_EOS_ACTION_TYPE type = jsonActions[i]["type"];
            switch (type) {
            case JUB_ENUM_EOS_ACTION_TYPE::XFER:
            {
                TW::EOS::TransferAction action;
                action.setType((uint8_t)type);
                action.deserialize(jsonActions[i]);
                tx.actions.push_back(action);
                break;
            }
            case JUB_ENUM_EOS_ACTION_TYPE::DELE:
            case JUB_ENUM_EOS_ACTION_TYPE::UNDELE:
            {
                TW::EOS::DelegateAction action;
                action.setType((uint8_t)type);
                action.deserialize(jsonActions[i]);
                tx.actions.push_back(action);
                break;
            }
            case JUB_ENUM_EOS_ACTION_TYPE::BUYRAM:
            {
                TW::EOS::BuyRamAction action;
                action.setType((uint8_t)type);
                action.deserialize(jsonActions[i]);
                tx.actions.push_back(action);
                break;
            }
            case JUB_ENUM_EOS_ACTION_TYPE::NS_ITEM_ACTION_TYPE:
            default:
                rv = JUBR_ARGUMENTS_BAD;
                break;
            }
            if (JUBR_OK != rv) {
                break;
            }
        }
        if (JUBR_OK != rv) {
            return rv;
        }

        //Data vRaw;
        tx.serialize(preimageRaw, bWithType);
    }
    catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}

nlohmann::json serializeAction(const JUB_ACTION_EOS& action) {

    switch (action.type) {
    case JUB_ENUM_EOS_ACTION_TYPE::XFER:
    {
        TW::EOS::TransferAction transferAction =
        TW::EOS::TransferAction(
            std::string(action.currency), std::string(action.name),
            std::string(action.transfer.from), std::string(action.transfer.to),
            TW::Bravo::Asset::fromString(std::string(action.transfer.asset)),
            std::string(action.transfer.memo));
        return transferAction.serialize();
    }
    case JUB_ENUM_EOS_ACTION_TYPE::DELE:
    case JUB_ENUM_EOS_ACTION_TYPE::UNDELE:
    {
        TW::EOS::DelegateAction delegateAction =
        TW::EOS::DelegateAction(
            std::string(action.currency), std::string(action.name),
            std::string(action.delegate.from), std::string(action.delegate.receiver),
            TW::Bravo::Asset::fromString(std::string(action.delegate.unstakeNetQty)),
            TW::Bravo::Asset::fromString(std::string(action.delegate.unstakeCpuQty)));
        return delegateAction.serialize();
    }
    case JUB_ENUM_EOS_ACTION_TYPE::BUYRAM:
    {
        TW::EOS::BuyRamAction buyRamAction =
        TW::EOS::BuyRamAction(
            std::string(action.currency), std::string(action.name),
            std::string(action.buyRam.payer), std::string(action.buyRam.receiver),
            TW::Bravo::Asset::fromString(std::string(action.buyRam.quant)));
        return buyRamAction.serialize();
    }
    default:
        return nlohmann::json::object();
    }

    return nlohmann::json::object();
}

JUB_RV serializeActions(const JUB_ACTION_EOS_PTR actions, JUB_UINT16 actionCount,
                        std::string& actionsInJSON) {

    JUB_RV rv = JUBR_OK;

    nlohmann::json jsonActions = nlohmann::json::array();
    for (JUB_UINT16 i=0; i<actionCount; ++i) {
        nlohmann::json jsonAction = serializeAction(actions[i]);
        if (jsonAction.empty()) {
            rv = JUBR_ARGUMENTS_BAD;
            break;
        }
        jsonAction["type"] = actions[i].type;
        jsonActions.push_back(jsonAction);
    }
    if (JUBR_OK != rv) {
        return rv;
    }
    actionsInJSON = jsonActions.dump();

    return JUBR_OK;
}
} // namespace eos end

} // namespace jub end
