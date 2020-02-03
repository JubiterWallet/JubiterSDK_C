#include "JUB_SDK_BTC.h"
#include "libBTC/libBTC.hpp"

#include <vector>

#include "TrezorCrypto/base58.h"
#include "TrezorCrypto/bip32.h"
#include "Bitcoin/SegwitAddress.h"
#include "Bitcoin/Script.h"

namespace jub {
namespace btc {


JUB_RV serializeUnsignedTx(const uint32_t coin,
                           const JUB_ENUM_BTC_TRANS_TYPE& type,
                           const std::vector<INPUT_BTC>& vInputs,
                           const std::vector<OUTPUT_BTC>& vOutputs,
                           const JUB_UINT32 lockTime,
                           uchar_vector& unsignedRaw) {

    JUB_RV rv = JUBR_ERROR;

    bool witness = false;
    if (p2sh_p2wpkh == type) {
        witness = true;
    }

    TW::Bitcoin::Transaction tx(lockTime);
    rv = serializeUnsignedTx(coin,
                             vInputs,
                             vOutputs,
                             tx);
    if (JUBR_OK != rv) {
        return rv;
    }

    tx.encode(witness, unsignedRaw);

    return JUBR_OK;
}

JUB_RV serializeUnsignedTx(const uint32_t coin,
                           const std::vector<INPUT_BTC>& vInputs,
                           const std::vector<OUTPUT_BTC>& vOutputs,
                           TW::Bitcoin::Transaction& tx) {

    // inputs
    for (const auto& input:vInputs) {
        uchar_vector preHash(input.preHash);
        preHash.reverse();
        TW::Bitcoin::OutPoint outpoint(preHash, input.preIndex);
        TW::Bitcoin::Script script;// null
        TW::Bitcoin::TransactionInput txInput(outpoint, script, input.nSequence);
        tx.inputs.push_back(txInput);
    }

    // outputs
    for (const auto& output:vOutputs) {
        TW::Bitcoin::Script scriptPubkey;
        JUB_UINT64 amount = 0;
        switch (output.type) {
            case JUB_ENUM_SCRIPT_BTC_TYPE::RETURN0:
            {
                TW::Data return0(output.return0.dataLen);
                std::memcpy(&return0[0], output.return0.data, output.return0.dataLen);

                scriptPubkey = TW::Bitcoin::Script::buildReturn0(return0);
                amount = output.return0.amount;
                break;
            }
            case JUB_ENUM_SCRIPT_BTC_TYPE::QRC20:
            {
                TW::Data qrc20(output.qrc20.dataLen);
                std::memcpy(&qrc20[0], output.qrc20.data, output.qrc20.dataLen);

                scriptPubkey = TW::Bitcoin::Script(qrc20.begin(), qrc20.end());
                break;
            }
            default:
            {
                scriptPubkey = TW::Bitcoin::Script::buildForAddress(std::string(output.stdOutput.address), (TWCoinType)coin);
                amount = output.stdOutput.amount;
                break;
            }
        }
        TW::Bitcoin::TransactionOutput txOutput(TW::Bitcoin::Amount(amount), scriptPubkey);
        tx.outputs.push_back(txOutput);
    }

    return JUBR_OK;
}


JUB_RV serializeTx(const uint32_t coin,
                   const JUB_ENUM_BTC_TRANS_TYPE& type,
                   const std::vector<INPUT_BTC>& vInputs,
                   const std::vector<OUTPUT_BTC>& vOutputs,
                   const JUB_UINT32 lockTime,
                   const std::vector<uchar_vector>& vSignatureRaw,
                   uchar_vector& signedRaw) {

    // JuBiter-not-finished
    return JUBR_IMPL_NOT_SUPPORT;
}


JUB_RV buildScriptPubFromAddress(const std::string& address, uchar_vector& scriptPub) {

    TW::Data vScriptPub;
    bool rv = false;
    int scriptLen = (int)address.length();
    uint8_t *script = new uint8_t[scriptLen];
    memset(script, 0x00, scriptLen);
    scriptLen = base58_decode_check(address.c_str(), HasherType::HASHER_SHA2D, script, scriptLen);
    if (scriptLen > 0) {
        uchar_vector vScript(script, scriptLen);
        vScriptPub = vScript;
        rv = true;
    }
    delete[] script; script = NULL;
    if (rv) {
        if (   p2shVersion     == vScriptPub[0]
            || p2shVersion_LTC == vScriptPub[0]
            || p2shVersion_QTUM == vScriptPub[0]
            ) { // p2sh
            scriptPub << OpCode::OP_HASH160;
            scriptPub & uchar_vector(vScriptPub.begin() + 1, vScriptPub.end());
            scriptPub << OpCode::OP_EQUAL;
        }
        else if (   p2pkhVersion     == vScriptPub[0]
                 || p2pkhVersion_LTC == vScriptPub[0]
                 || p2pkhVersion_QTUM == vScriptPub[0]
                 ) { //p2pkh
            scriptPub << OpCode::OP_DUP;
            scriptPub << OpCode::OP_HASH160;
            scriptPub & uchar_vector(vScriptPub.begin()+1, vScriptPub.end());
            scriptPub << OpCode::OP_EQUALVERIFY;
            scriptPub << OpCode::OP_CHECKSIG;
        }
        else {
            return JUBR_ERROR;
        }

        return JUBR_OK;
    }
    else {
        if (!TW::Bitcoin::SegwitAddress::isValid(address)) {
            return JUBR_ERROR;
        }

        std::pair<TW::Bitcoin::SegwitAddress, bool> segwitAddrPair = TW::Bitcoin::SegwitAddress::decode(address);
        TW::Bitcoin::SegwitAddress segwitAddr = segwitAddrPair.first;
        if (0 != segwitAddr.hrp.compare(HRP_BITCOIN)
            || !segwitAddrPair.second
            ) {
            return JUBR_ERROR;
        }

        if (0x14 == segwitAddr.witnessProgram.size()) { //p2wpkh
            scriptPub << (JUB_BYTE)0x00;
            scriptPub & segwitAddr.witnessProgram;
        }
        else if (0x20 == segwitAddr.witnessProgram.size()) { //p2wsh
            scriptPub << (JUB_BYTE)0x00;
            scriptPub & segwitAddr.witnessProgram;
        }
        else {
            return JUBR_ERROR;
        }

        return JUBR_OK;
    }

    return JUBR_ERROR;
}


JUB_RV serializeUnsignedTx(const JUB_ENUM_BTC_TRANS_TYPE& type,
                           const std::vector<INPUT_BTC>& vInputs,
                           const std::vector<OUTPUT_BTC>& vOutputs,
                           const JUB_UINT32 lockTime,
                           uchar_vector& unsignedRaw) {

    uchar_vector unsignedTrans;
    unsignedTrans << version1;
    if (p2sh_p2wpkh == type) {
        unsignedTrans << marker;
        unsignedTrans << flag;
    }

    //input
    unsignedTrans << (JUB_BYTE)vInputs.size();
    //[preTXID][preOutputIndex][0x00][nSequence]
    for (auto input : vInputs) {
        switch (input.type) {
        case JUB_ENUM_SCRIPT_BTC_TYPE::P2PKH:
//        case JUB_ENUM_SCRIPT_BTC_TYPE::P2SH_MULTISIG:
        {
            //preHash
            uchar_vector preHash = std::string(input.preHash);
            preHash.reverse();
            unsignedTrans << preHash;
            //preIndex
            unsignedTrans << (JUB_UINT32)input.preIndex;
            //scriptSig: <sig> <pubkey>
            //scriptSig: OP_0 <A sig> [B sig] [C sig] ... <redeemScript>
            unsignedTrans << (JUB_BYTE)0x00;
            unsignedTrans << input.nSequence;
            break;
        } // case JUB_ENUM_SCRIPT_BTC_TYPE::P2PKH
          // case JUB_ENUM_SCRIPT_BTC_TYPE::P2SH_MULTISIG end
        default:
            break;
        } // switch (input.type) end
    }
    //output
    unsignedTrans << (JUB_BYTE)vOutputs.size();
    for (auto output : vOutputs) {
        switch (output.type) {
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
        case JUB_ENUM_SCRIPT_BTC_TYPE::QRC20:
        {
            unsignedTrans << (uint64_t)0x00;
            uchar_vector scriptPub(output.qrc20.data,output.qrc20.data + output.qrc20.dataLen);
            unsignedTrans && scriptPub;
            break;
        } // case JUB_ENUM_SCRIPT_BTC_TYPE::QRC20 end
        default:
        {
            //amount
            unsignedTrans << (uint64_t)output.stdOutput.amount;
            //script_pub
            uchar_vector scriptPub;
            if (JUBR_OK != buildScriptPubFromAddress(output.stdOutput.address, scriptPub)) {
                return JUBR_ERROR;
            }

            unsignedTrans && scriptPub;
            break;
        } // default
        } // switch (output.type) end
    }

    //witness 00
    if (p2sh_p2wpkh == type) {
        unsignedTrans << (JUB_BYTE)0x00;
    }

    unsignedTrans << lockTime;
    unsignedRaw = unsignedTrans;

    return JUBR_OK;
}


} // namespace btc end
} // namespace jub end
