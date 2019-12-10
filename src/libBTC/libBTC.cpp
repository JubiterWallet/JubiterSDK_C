#include "JUB_SDK_BTC.h"
#include "libBTC/libBTC.hpp"

#include <vector>

#include "airbitz-core/abcd/util/Data.hpp"
#include <TrezorCrypto/base58.h>
#include "bitcoin/bech32.h"
#include "bitcoin/segwit_addr.h"
#include "machine/opcode.hpp"
#include "TrezorCrypto/bip32.h"

namespace jub {

namespace btc {

JUB_RV buildScriptPubFromAddress(const std::string& address, uchar_vector& scriptPub) {

    abcd::DataChunk vScriptPub;
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
            ) { // p2sh
            scriptPub << libbitcoin::machine::opcode::hash160;
            scriptPub & uchar_vector(vScriptPub.begin() + 1, vScriptPub.end());
            scriptPub << libbitcoin::machine::opcode::equal;
        }
        else if (   p2pkhVersion     == vScriptPub[0]
                 || p2pkhVersion_LTC == vScriptPub[0]
                 ) { //p2pkh
            scriptPub << libbitcoin::machine::opcode::dup;
            scriptPub << libbitcoin::machine::opcode::hash160;
            scriptPub & uchar_vector(vScriptPub.begin()+1, vScriptPub.end());
            scriptPub << libbitcoin::machine::opcode::equalverify;
            scriptPub << libbitcoin::machine::opcode::checksig;
        }
        else {
            return JUBR_ERROR;
        }

        return JUBR_OK;
    }
    else {
        std::pair<std::string, abcd::DataChunk> dec = bech32::decode(address);
        if (dec.first != std::string(segwit_hrl)) {
            return JUBR_ERROR;
        }
        abcd::DataChunk conv;
        segwit_addr::convertbits<5, 8, false>(conv, abcd::DataChunk(dec.second.begin() + 1, dec.second.end()));

        if (0x14 == conv.size()) { //p2wpkh
            scriptPub << (JUB_BYTE)0x00;
            scriptPub & conv;
        }
        else if (0x20 == conv.size()) { //p2wsh
            scriptPub << (JUB_BYTE)0x00;
            scriptPub & conv;
        }
        else {
            return JUBR_ERROR;
        }
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
            scriptPub << (JUB_BYTE)libbitcoin::machine::opcode::return_; //op_return0
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
