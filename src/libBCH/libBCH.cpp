#include "JUB_SDK.h"
#include "libBCH/libBCH.hpp"

#include "TrustWalletCore/TWBitcoinOpCodes.h"
#include "TrustWalletCore/TWHRP.h"
#include "Bitcoin/CashAddress.h"
#include "Bech32Address.h"
#include "utility/util.h"

namespace jub {
namespace bch {


JUB_RV buildScriptPubFromAddress(const std::string& address, uchar_vector& scriptPub) {

    if (!TW::Bitcoin::CashAddress::isValid(address)) {
        return JUBR_ERROR;
    }

    std::pair<TW::Bitcoin::CashAddress, bool> cashAddrPair = TW::Bitcoin::CashAddress::decode(address);
    TW::Bitcoin::CashAddress cashAddr = cashAddrPair.first;
    if (0 != cashAddr.hrp.compare(HRP_BITCOINCASH)) {
        return JUBR_ERROR;
    }

    if (TypeBitsP2PKH == cashAddr.program[0]) { //p2pkh
        scriptPub << OpCode::OP_DUP;
        scriptPub << OpCode::OP_HASH160;
        scriptPub & uchar_vector(cashAddr.program.begin() + 1, cashAddr.program.end());
        scriptPub << OpCode::OP_EQUALVERIFY;
        scriptPub << OpCode::OP_CHECKSIG;
    }
    else if (TypeBitsP2SH == cashAddr.program[0]) { //p2sh
        scriptPub << OpCode::OP_HASH160;
        scriptPub & uchar_vector(cashAddr.program.begin() + 1, cashAddr.program.end());
        scriptPub << OpCode::OP_EQUAL;
    }
    else {
        JUB_VERIFY_RV(JUBR_ERROR);
    }

    return JUBR_OK;
}


JUB_RV serializeUnsignedTx(const JUB_ENUM_BTC_TRANS_TYPE& type,
                           const std::vector<INPUT_BTC>& vInputs,
                           const std::vector<OUTPUT_BTC>& vOutputs,
                           const JUB_UINT32 lockTime,
                           uchar_vector& unsignedRaw) {

    uchar_vector unsignedTrans;
    unsignedTrans << version1;

    //input
    unsignedTrans << (JUB_BYTE)vInputs.size();
    for (auto input : vInputs) {
        switch (input.type) {
        case JUB_ENUM_SCRIPT_BTC_TYPE::P2PKH:
        {
            //preHash
            uchar_vector preHash = std::string(input.preHash);
            preHash.reverse();
            unsignedTrans << preHash;
            //preIndex
            unsignedTrans << (JUB_UINT32)input.preIndex;
            //sig
            unsignedTrans << (JUB_BYTE)0;
            unsignedTrans << sequence;
            break;
        } // case JUB_ENUM_SCRIPT_BTC_TYPE::P2PKH end
        default:
            JUB_VERIFY_RV(JUBR_ARGUMENTS_BAD);
        }
    }

    //output
    unsignedTrans << (JUB_BYTE)vOutputs.size();
    for (auto output : vOutputs) {
        switch (output.type) {
        case JUB_ENUM_SCRIPT_BTC_TYPE::P2PKH:
        {
            //amount
            unsignedTrans << (uint64_t)output.stdOutput.amount;
            //script_pub
            uchar_vector scriptPub;
            if (JUBR_OK != buildScriptPubFromAddress(output.stdOutput.address, scriptPub)) {
                JUB_VERIFY_RV(JUBR_ERROR);
            }
            unsignedTrans && scriptPub;
            break;
        } // case JUB_ENUM_SCRIPT_BTC_TYPE::P2PKH end
        case JUB_ENUM_SCRIPT_BTC_TYPE::RETURN0:
        {
            unsignedTrans << (uint64_t)output.return0.amount;
            uchar_vector scriptPub;
            scriptPub << OpCode::OP_RETURN;
            scriptPub << (JUB_BYTE)output.return0.dataLen;
            scriptPub.insert(scriptPub.end(), output.return0.data, output.return0.data + output.return0.dataLen);

            unsignedTrans && scriptPub;
            break;
        } // case JUB_ENUM_SCRIPT_BTC_TYPE::RETURN0 end
        default:
            break;
        } // switch (output.type) end
    }

    unsignedTrans << lockTime;
    unsignedRaw = unsignedTrans;

    return JUBR_OK;
}


} // namespace bch end
} // namespace jub end
