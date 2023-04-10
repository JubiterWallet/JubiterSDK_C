#include "token/BTC/JubiterBaseBTCImpl.h"
#include "Data.h"
#include "Hash.h"
#include "JUB_SDK_BTC.h"
#include "JUB_SDK_COMM.h"
#include "PublicKey.h"
#include "Script.h"
#include "TWPublicKeyType.h"
#include "Transaction.h"
#include "TrezorCrypto/bignum.h"
#include "TrezorCrypto/ecdsa.h"
#include "TrezorCrypto/secp256k1.h"
#include <Bitcoin/Address.h>
#include <Bitcoin/SegwitAddress.h>
#include <TrezorCrypto/base58.h>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include "utility/util.h"

namespace jub {
namespace token {

TW::Data JubiterBaseBTCImpl::pushAll(const TW::Data &results) {
    auto data = TW::Data{};

    data.insert(data.end(), results.begin(), results.begin() + results.size());

    switch (_hashType) {
    case TWBitcoinSigHashTypeAll:
    case TWBitcoinSigHashTypeForkBCH:
        data.push_back(_hashType);
        break;
    default:
        break;
    } // switch (_hashType) end

    return data;
}

JUB_RV JubiterBaseBTCImpl::_getAddress(const TW::Data &publicKey, std::string &address, const TWCoinType &coinNet) {
    try {
        TW::Bitcoin::Address addr(TW::PublicKey(publicKey, _publicKeyType),
                                  TWCoinTypeP2pkhPrefix((coinNet ? coinNet : _coin)));
        address = addr.string();
    } catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}

JUB_RV JubiterBaseBTCImpl::CheckAddress(const std::string &address, const TWCoinType &coinNet) {

    // check legacy address
    std::vector<TW::Data> prefixs;
    prefixs.push_back({TWCoinTypeP2pkhPrefix((coinNet ? coinNet : _coin))});
    prefixs.push_back({TWCoinTypeP2shPrefix((coinNet ? coinNet : _coin))});
    JUB_RV rvLegacy = !(TW::Bitcoin::Address::isValid(address, prefixs));
    // check segwit address
    JUB_RV rvSegwit =
        !(TW::Bitcoin::SegwitAddress::isValid(address, std::string(stringForHRP(TWCoinTypeHRP(_coin, coinNet)))));

    return rvLegacy & rvSegwit;
}

JUB_RV JubiterBaseBTCImpl::_getTaprootAddress(const TW::Data &publicKey, std::string &address,
                                              const TWCoinType &coinNet) {
    try {
        // 32 bytes output key
        // https://github.com/bitcoin/bips/blob/master/bip-0086.mediawiki#address-derivation
        auto pub = TW::PublicKey(publicKey, TWPublicKeyTypeSECP256k1).p2trPublicKey();

        TW::Data prog(pub.bytes.begin() + 1, pub.bytes.begin() + 33);
        TW::Bitcoin::SegwitAddress segwitAddr(std::string(stringForHRP(TWCoinTypeHRP(_coin, coinNet))), 1, prog);
        address = segwitAddr.string();
    } catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}

JUB_RV JubiterBaseBTCImpl::_getSegwitAddress(const TW::Data &publicKey, std::string &address,
                                             const TWCoinType &coinNet) {
    try {
        // keyhash
        TW::Bitcoin::SegwitAddress segwitAddr(TW::PublicKey(publicKey, _publicKeyType), OpCode::OP_0,
                                              std::string(stringForHRP(TWCoinTypeHRP(_coin, coinNet))));

        address = segwitAddr.string();
    } catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}

JUB_RV JubiterBaseBTCImpl::_getNestedSegwitAddress(const TW::Data &publicKey, std::string &address,
                                                   const TWCoinType &coinNet) {
    try {
        // keyhash
        TW::Bitcoin::SegwitAddress segwitAddr(TW::PublicKey(publicKey, _publicKeyType), OpCode::OP_0,
                                              std::string(stringForHRP(TWCoinTypeHRP(_coin, coinNet))));

        // redeemScript
        TW::Bitcoin::Script redeemScript = TW::Bitcoin::Script::buildPayToWitnessProgram(0, segwitAddr.witnessProgram);
        if (redeemScript.empty()) {
            return JUBR_ARGUMENTS_BAD;
        }
        TW::Data hRedeemScript = TW::Hash::sha256ripemd(&redeemScript.bytes[0], redeemScript.bytes.size());

        // address
        TW::Data bytes;
        bytes.insert(bytes.end(), TWCoinTypeP2shPrefix((coinNet ? coinNet : _coin)));
        bytes.insert(bytes.end(), hRedeemScript.begin(), hRedeemScript.end());

        address = TW::Base58::bitcoin.encodeCheck(bytes);
    } catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}

JUB_RV JubiterBaseBTCImpl::_unsignedTx(const uint32_t coin, const std::vector<INPUT_BTC> &vInputs,
                                       const std::vector<OUTPUT_BTC> &vOutputs, TW::Bitcoin::Transaction &tx) {
    JUB_RV rv = JUBR_OK;

    // inputs
    for (const auto &input : vInputs) {
        uchar_vector preHash(input.preHash);
        preHash.reverse();
        TW::Bitcoin::OutPoint outpoint(preHash, input.preIndex);
        TW::Bitcoin::TransactionInput *txInput = new TW::Bitcoin::TransactionInput();
        txInput->previousOutput                = outpoint;
        txInput->script                        = TW::Bitcoin::Script(); // null
        txInput->sequence                      = input.nSequence;
        tx.inputs.push_back(txInput);
    }

    // outputs
    for (const auto &output : vOutputs) {
        TW::Bitcoin::Script scriptPubkey;
        JUB_UINT64 amount = 0;
        switch (output.type) {
        case JUB_ENUM_SCRIPT_BTC_TYPE::RETURN0: {
            TW::Data return0(output.return0.dataLen);
            std::memcpy(&return0[0], output.return0.data, output.return0.dataLen);

            // Check if it is omni protocol
            scriptPubkey = TW::Bitcoin::Script::buildReturn0(return0, uchar_vector("6f6d6e69"));
            amount       = output.return0.amount;
            break;
        } // case JUB_ENUM_SCRIPT_BTC_TYPE::RETURN0 end
        case JUB_ENUM_SCRIPT_BTC_TYPE::QRC20: {
            TW::Data qrc20(output.qrc20.dataLen);
            std::memcpy(&qrc20[0], output.qrc20.data, output.qrc20.dataLen);

            scriptPubkey = TW::Bitcoin::Script(qrc20.begin(), qrc20.end());
            break;
        } // case JUB_ENUM_SCRIPT_BTC_TYPE::QRC20 end
        default: {
            scriptPubkey =
                TW::Bitcoin::Script::buildForAddress(std::string(output.stdOutput.address), (TWCoinType)coin);
            amount = output.stdOutput.amount;
            break;
        } // default
        } // switch (output.type) end
        if (scriptPubkey.empty()) {
            rv = JUBR_ARGUMENTS_BAD;
        }
        if (0 != rv) {
            break;
        }
        TW::Bitcoin::TransactionOutput *ptxOutput = new TW::Bitcoin::TransactionOutput();
        ptxOutput->value                          = TW::Bitcoin::Amount(amount);
        ptxOutput->script                         = scriptPubkey;
        tx.outputs.push_back(ptxOutput);
    } // for (const auto& output:vOutputs) end

    return rv;
}

JUB_RV JubiterBaseBTCImpl::SerializeUnsignedTx(const JUB_ENUM_BTC_TRANS_TYPE &type, const JUB_UINT32 version,
                                               const std::vector<INPUT_BTC> &vInputs,
                                               const std::vector<OUTPUT_BTC> &vOutputs, const JUB_UINT32 lockTime,
                                               uchar_vector &unsignedRaw, const TWCoinType &coinNet) {

    auto witness = type == p2sh_p2wpkh || type == p2wpkh || type == p2tr;

    TW::Bitcoin::Transaction tx(version, lockTime);
    JUB_VERIFY_RV(_unsignedTx((coinNet ? coinNet : _coin), vInputs, vOutputs, tx));

    tx.encode(witness, unsignedRaw);

    return JUBR_OK;
}


JUB_RV JubiterBaseBTCImpl::SerializeTx(const JUB_ENUM_COINTYPE_BTC coinType,
                                       const JUB_ENUM_BTC_TRANS_TYPE type,
                                       const JUB_UINT32 version,
                                       const unsigned long m,
                                       const std::vector<INPUT_BTC>& vInputs,
                                       const MAP_COSIGNER_SIGNATURE& mapCosignerSignatures,
                                       const std::vector<uchar_vector>& vRedeemScript,
                                       const std::vector<OUTPUT_BTC>& vOutputs,
                                       const JUB_UINT32 lockTime,
                                       uchar_vector& signedIncRaw) {

    JUB_RV rv = JUBR_OK;

    uchar_vector signedIncTrans;
    signedIncTrans << version;
    if (   p2sh_p2wpkh    == type
        || p2wsh_multisig == type
        || p2wpkh        == type
        ) {
        signedIncTrans << (uint8_t)0x00; //marker;
        signedIncTrans << (uint8_t)0x01; //flag;
    }
    //input
    std::vector<long> vSignatureCnt;
    signedIncTrans << (JUB_BYTE)vInputs.size();
    for (size_t inputIndex=0; inputIndex<vInputs.size(); ++inputIndex) {
        //preHash
        uchar_vector preHash = std::string(vInputs[inputIndex].preHash);
        std::string preID = preHash.getHex();
        preHash.reverse();
        signedIncTrans << preHash;
        //preIndex
        signedIncTrans << (JUB_UINT32)vInputs[inputIndex].preIndex;

        //scriptSig
        long signatureCnt = 0;
        switch (vInputs[inputIndex].type) {
        case JUB_ENUM_SCRIPT_BTC_TYPE::P2PKH:
        {
            //scriptSig: <sig> <pubkey>
//            signedIncTrans << (JUB_BYTE)0;
//            break;
            return JUBR_ARGUMENTS_BAD;
        } // case JUB_ENUM_SCRIPT_BTC_TYPE::P2PKH end
        case JUB_ENUM_SCRIPT_BTC_TYPE::P2SH_MULTISIG:
        case JUB_ENUM_SCRIPT_BTC_TYPE::P2WSH_MULTISIG:
        {
            uchar_vector scriptSig;
            //BTC/LTC/BCH scriptSig: OP_0 <A sig> [B sig] [C sig] ... <redeemScript>
            scriptSig << OpCode::OP_0; // OP_0

            std::vector<std::string> vSignature = getVectorFromMap(std::to_string(inputIndex), mapCosignerSignatures);
            for (size_t j=0; j<vSignature.size(); ++j) {
                uchar_vector signature;
                signature.setHex(vSignature[j]);
                scriptSig << signature;

                signatureCnt += 1;
            }

            scriptSig & vRedeemScript[inputIndex];

            signedIncTrans && scriptSig;

            vSignatureCnt.push_back(signatureCnt);
            break;
        } // case JUB_ENUM_SCRIPT_BTC_TYPE::P2SH_MULTISIG end
        default:
            break;
        } // switch (vInputs[i].type) end

        // sequence
        signedIncTrans << vInputs[inputIndex].nSequence;
    }
    JUB_VERIFY_RV(rv);

    //output
    signedIncTrans << (JUB_BYTE)vOutputs.size();
    for (auto output : vOutputs) {
        switch (output.type) {
        case JUB_ENUM_SCRIPT_BTC_TYPE::RETURN0:
        {
            signedIncTrans << (uint64_t)output.return0.amount;
            uchar_vector scriptPub;
            scriptPub << (JUB_BYTE)OpCode::OP_RETURN; //op_return0
            scriptPub << (JUB_BYTE)output.return0.dataLen;
            scriptPub.insert(scriptPub.end(), output.return0.data, output.return0.data + output.return0.dataLen);

            signedIncTrans && scriptPub;
            break;
        } // case SCRIPT_BTC_TYPE::RETURN0 end
        default:
        {
            //amount
            signedIncTrans << (uint64_t)output.stdOutput.amount;
            //script_pub
            uchar_vector scriptPub = TW::Bitcoin::Script::buildForAddress(output.stdOutput.address, TWCoinTypeBitcoin).bytes;
            if (0 == scriptPub.size()) {
                return JUBR_ERROR;
            }
            signedIncTrans && scriptPub;
            break;
        } // default
        } // switch (output.type) end
    }

    //witness 00
    if (   p2sh_p2wpkh    == type
        || p2wsh_multisig == type
        || p2wpkh        == type
        ) {
        signedIncTrans << (JUB_BYTE)0x00;
    }

    signedIncTrans << lockTime;
    signedIncRaw = signedIncTrans;

    // Determine whether to complete the transaction signature
    bool isFinished = true;
    for (auto signatureCnt:vSignatureCnt) {
        if (signatureCnt < m) {
            isFinished = false;
        }
    }

    if (!isFinished) {
        return JUBR_MULTISIG_OK;
    }

    return JUBR_OK;
}


JUB_RV JubiterBaseBTCImpl::SerializePreimage(const JUB_ENUM_COINTYPE_BTC coinType,
                                             const JUB_ENUM_BTC_TRANS_TYPE type, const JUB_UINT32 version,
                                             const std::size_t for_sign_input_index,
                                             const std::vector<INPUT_BTC>& vInputs,
                                             const std::vector<uchar_vector>& vRedeemScript,
                                             const std::vector<OUTPUT_BTC>& vOutputs,
                                             const JUB_UINT32 lockTime,
                                             uchar_vector& preimageRaw,
                                             const bool bWitness) {

    TW::Hash::Hasher hasher = TW::Hash::sha256d;

    JUB_ENUM_BTC_TRANS_TYPE txType = type;
    if (COINBCH == coinType) {
        if (p2sh_multisig == type) {
            txType = p2wsh_multisig;
        }
    }

    //preimage: [nVersion][nInputCount][txInputs][nOutputCount][txOutputs][nLockTime][nHashType]
    uchar_vector preimage;
    //[nVersion]
    preimage << version;

    //[txins]
    uchar_vector vPrevouts;
    uchar_vector vSequences;
    switch (txType) {
    case p2sh_multisig:
    {
    //       : [nInputCount]
    preimage << (JUB_BYTE)vInputs.size();
        break;
    }
    case p2wsh_multisig:
    {
        for (size_t i=0; i<vInputs.size(); ++i) {
            uchar_vector preHash = std::string(vInputs[i].preHash);
            preHash.reverse();

            // prevouts: preHash + preIndex
            vPrevouts << preHash;
            vPrevouts << (JUB_UINT32)vInputs[i].preIndex;

            // sequence
            vSequences << 0xffffffff;
        }
        break;
    }
    default:
        break;
    }

    uchar_vector vinputs;
    //                      : [txInputs]
    for (size_t i=0; i<vInputs.size(); ++i) {
        JUB_ENUM_SCRIPT_BTC_TYPE inputType = vInputs[i].type;
        if (COINBCH == coinType) {
            if (JUB_ENUM_SCRIPT_BTC_TYPE::P2SH_MULTISIG == inputType) {
                inputType = JUB_ENUM_SCRIPT_BTC_TYPE::P2WSH_MULTISIG;
            }
        }
        switch (inputType) {
        case JUB_ENUM_SCRIPT_BTC_TYPE::P2SH_MULTISIG:
        {
            //preHash
            uchar_vector preHash = std::string(vInputs[i].preHash);
            preHash.reverse();
            preimage << preHash;

            //preIndex
            preimage << (JUB_UINT32)vInputs[i].preIndex;

            //scriptCode
            if (for_sign_input_index == i) {
                //script code: 0xfd/0xfe/0xff <redeemScript>
                preimage && vRedeemScript[i];
            }
            else {
                preimage << (JUB_BYTE)0x00;
            }

            //sequence
            preimage << 0xffffffff;
            break;
        } // case JUB_ENUM_SCRIPT_BTC_TYPE::P2SH_MULTISIG end
        case JUB_ENUM_SCRIPT_BTC_TYPE::P2WSH_MULTISIG:
        {
            if (for_sign_input_index != i) {
                continue;
            }
            //hashPrevouts = sha256d(prevouts)
            preimage << uchar_vector(TW::Hash::hash(hasher, TW::Data(vPrevouts)));

            //hashSequence = sha256d(sequence)
            preimage << uchar_vector(TW::Hash::hash(hasher, TW::Data(vSequences)));

            //outpoint
            uchar_vector preHash = std::string(vInputs[i].preHash);
            preHash.reverse();
            preimage << preHash;

            //preIndex
            preimage << (JUB_UINT32)vInputs[i].preIndex;

            //scriptCode
            //script code: 0xfd/0xfe/0xff <redeemScript>
            preimage && vRedeemScript[i];

            //value
            preimage << (uint64_t)vInputs[i].amount;

            //sequence
            preimage << 0xffffffff;
        } // case JUB_ENUM_SCRIPT_BTC_TYPE::P2WSH_MULTISIG end
        default:
            break;
        } // switch (input.type) end
    }

    switch (txType) {
    case p2sh_multisig:
    {
        preimage << vinputs;
        break;
    }
    default:
        break;
    }

    switch (txType) {
    case p2sh_multisig:
    {
    //[txouts]
    //        : [nOutputCount]
    preimage << (JUB_BYTE)vOutputs.size();
        break;
    }
    default:
        break;
    }

    uchar_vector voutputs;
    //                        : [txOutputs]
    for (auto output : vOutputs) {
        switch (output.type) {
        case JUB_ENUM_SCRIPT_BTC_TYPE::RETURN0:
        {
            voutputs << (uint64_t)output.return0.amount;
            uchar_vector scriptPub;
            scriptPub << (JUB_BYTE)OpCode::OP_RETURN; //op_return0
            scriptPub << (JUB_BYTE)output.return0.dataLen;
            scriptPub.insert(scriptPub.end(), output.return0.data, output.return0.data + output.return0.dataLen);

            voutputs && scriptPub;
            break;
        } // case JUB_ENUM_SCRIPT_BTC_TYPE::RETURN0 end
        default:
        {
            //amount
            voutputs << (uint64_t)output.stdOutput.amount;
            //script_pub
            uchar_vector scriptPub = TW::Bitcoin::Script::buildForAddress(output.stdOutput.address, TWCoinTypeBitcoin).bytes;
            if (0 == scriptPub.size()) {
                return JUBR_ERROR;
            }

            voutputs && scriptPub;
            break;
        } // default
        } // switch (output.type) end
    }

    switch (txType) {
    case p2sh_multisig:
    {
        preimage << voutputs;
        break;
    }
    case p2wsh_multisig:
    {
        preimage << uchar_vector(TW::Hash::hash(hasher, TW::Data(voutputs)));
        break;
    }
    default:
        break;
    }

    //[nLockTime]
    preimage << lockTime;

    //[nHashType]
    uint8_t hashType = 0x01;//[ALL]
    if (COINBCH == coinType) {
        hashType |= 0x40;
    }
    preimage << (uint32_t)hashType;

    preimageRaw = preimage;

    return JUBR_OK;
}


// !!Not handling witness yet
JUB_RV JubiterBaseBTCImpl::DeserializeTx(bool bWitness,
                                         const std::string hexTx,
                                         std::vector<INPUT_BTC>& vInputs,
                                         std::vector<OUTPUT_BTC>& vOutputs,
                                         JUB_UINT32_PTR plockTime) {

    JUB_RV rv = JUBR_OK;

    if ("" == hexTx) {
        return JUBR_MEMORY_NULL_PTR;
    }

    vInputs.clear();
    vOutputs.clear();
    *plockTime = 0;

    // non-witness: [nVersion]              [nInputCount][txInputs][nOutputCount][txOutputs]         [nLockTime]
    //     witness: [nVersion][marker][flag][nInputCount][txInputs][nOutputCount][txOutputs][witness][nLockTime]
    uchar_vector incTx;
    incTx << HexStr2CharPtr(hexTx);
    incTx.reset_it();

    // [nVersion]
    auto version = incTx.read_le_uint32();
    if (version1 != version) {
        return JUBR_ARGUMENTS_BAD;
    }

    if (bWitness) {
        // [marker]
        auto marker = incTx.read_uint8();
        // [flag]
        auto flag = incTx.read_uint8();
        if (   0 != marker
            || 1 != flag
            ) {
            return JUBR_ARGUMENTS_BAD;
        }
    }

    // [nInputCount]
    auto inputCount = incTx.read_uint8();
    // [txInputs]
    for (auto i=0; i<inputCount; ++i) {
        INPUT_BTC input;
        input.amount = 0;

        // [preHash]
        size_t szPreHashLen = 32;
        uchar_vector preHash = incTx.read_vector(szPreHashLen);
        preHash.reverse();
        input.preHash = new JUB_CHAR[szPreHashLen*2+1];
        memset(input.preHash, 0x00, szPreHashLen*2+1);
        std::memcpy(input.preHash, preHash.getHex().c_str(), szPreHashLen*2);
        input.preIndex = incTx.read_le_uint32();

        // <scriptSig>
        uchar_vector scriptSig;
        incTx >> scriptSig;
        scriptSig.reset_it();
        if (0 == scriptSig.size()) {
            rv = JUBR_ARGUMENTS_BAD;
            break;
        }

        uint8_t scriptSigFirst = scriptSig.read_uint8();
        std::vector<std::string> vInputSignature;
        if ((uint8_t)OpCode::OP_0 == scriptSigFirst) {
            input.type = JUB_ENUM_SCRIPT_BTC_TYPE::P2SH_MULTISIG;
            if (bWitness) {
                input.type = JUB_ENUM_SCRIPT_BTC_TYPE::P2WSH_MULTISIG;
            }
            // SCRIPT_BTC_TYPE::P2SH_MULTISIG;
            auto scriptSigIt = scriptSig.begin();
            do {
                uint32_t itemLen = scriptSig.read_uint8();
                if ((uint8_t)OpCode::OP_PUSHDATA1 == itemLen) {
                    itemLen = scriptSig.read_uint8();
                }
                else if ((uint8_t)OpCode::OP_PUSHDATA2 == itemLen) {
                    itemLen = scriptSig.read_le_uint16();
                }
                else if ((uint8_t)OpCode::OP_PUSHDATA4 == itemLen) {
                    itemLen = scriptSig.read_le_uint32();
                }
                if (                  0 == itemLen
                    || scriptSig.size()  < itemLen
                    ) {
                    return JUBR_ARGUMENTS_BAD;
                }
                uchar_vector item = scriptSig.read_vector(itemLen);
                uint8_t tag = item[0];
                if (0x30 == tag) {
                    // signature
                }
                else {
                    // redeemScript
                }

                scriptSigIt = scriptSig.get_cur_it();
            } while (scriptSigIt != scriptSig.end());
        }
        else {
            input.type = JUB_ENUM_SCRIPT_BTC_TYPE::P2PKH;
        }

        input.nSequence = incTx.read_le_uint32();

        vInputs.push_back(input);
    }
    if (   JUBR_OK != rv
        || vInputs.size() != inputCount
        ) {
        vInputs.clear();
        return JUBR_ARGUMENTS_BAD;
    }

    // [nOutputCount]
    auto outputCount = incTx.read_uint8();
    // [txOutputs]
    for (auto i=0; i<outputCount; ++i) {
        OUTPUT_BTC output;

        output.stdOutput.amount = incTx.read_le_uint64();;

        // <scriptPubKey>
        uchar_vector scriptPub;
        incTx >> scriptPub;
        scriptPub.reset_it();
        if (0 == scriptPub.size()) {
            rv = JUBR_ARGUMENTS_BAD;
            break;
        }

        uint8_t scriptPubFirst = scriptPub.read_uint8();
        uchar_vector addrScript;
        if ((uint8_t)OpCode::OP_DUP == scriptPubFirst) {
            output.type = JUB_ENUM_SCRIPT_BTC_TYPE::P2PKH;

            uint8_t op = scriptPub.read_uint8();
            if ((uint8_t)OpCode::OP_HASH160 != op) {
                rv = JUBR_ARGUMENTS_BAD;
                break;
            }
            uint8_t scriptLen = scriptPub.read_uint8();
            uint8_t offset = scriptPub.get_cur_it() - scriptPub.begin();
            uint8_t penultimate = scriptPub[offset+scriptLen];
            uint8_t last = scriptPub[offset+scriptLen+1];
            if (   (uint8_t)OpCode::OP_EQUALVERIFY != penultimate
                && (uint8_t)OpCode::OP_CHECKSIG    != last
                ) {
                rv = JUBR_ARGUMENTS_BAD;
                break;
            }
            addrScript << (unsigned char)0x00;//jub::btc::p2pkhVersion;
            addrScript << scriptPub.read_vector(scriptLen);
        }
        else if ((uint8_t)OpCode::OP_HASH160 == scriptPubFirst) {
            output.type = JUB_ENUM_SCRIPT_BTC_TYPE::P2SH_MULTISIG;

            uint8_t scriptLen = scriptPub.read_uint8();
            uint8_t offset = scriptPub.get_cur_it() - scriptPub.begin();
            uint8_t last = scriptPub[offset+scriptLen];
            if ((uint8_t)OpCode::OP_EQUAL != last) {
                rv = JUBR_ARGUMENTS_BAD;
                break;
            }
            addrScript << (unsigned char)0x05;//jub::btc::p2shVersion;
            addrScript << scriptPub.read_vector(scriptLen);
        }
        else {
            rv = JUBR_ARGUMENTS_BAD;
            break;
        }

        int scriptLen = (int)addrScript.size();
        uint8_t *script = new uint8_t[scriptLen+1];
        memset(script, 0x00, scriptLen+1);
        std::copy(addrScript.begin(), addrScript.end(), script);

        int strsize = scriptLen*3;
        char *str = new char[strsize];
        strsize = base58_encode_check(script, scriptLen, HasherType::HASHER_SHA2D, str, strsize);
        delete[] script; script = NULL;
        if (strsize <= 0) {
            delete[] str; str = NULL;
            return JUBR_ARGUMENTS_BAD;
        }
        std::string address = std::string(str);
        delete[] str; str = NULL;

        output.stdOutput.address = new JUB_CHAR[address.length()+1];
        memset(output.stdOutput.address, 0x00, address.length()+1);
        memcpy(output.stdOutput.address, address.c_str(), address.length());

        vOutputs.push_back(output);
    }
    if (JUBR_OK != rv) {
        vInputs.clear();
        for (auto& output:vOutputs) {
            if (NULL != output.stdOutput.address) {
                delete[] output.stdOutput.address;
                output.stdOutput.address = NULL;
            }
        }
        vOutputs.clear();
        JUB_VERIFY_RV(rv);
    }

    // [witness]
    if (bWitness) {
        for (auto i=0; i<inputCount; ++i) {
            auto witness = incTx.read_uint8();
            if (0x02 != witness) {
                break;
            }

            // <scriptSig>
            uchar_vector scriptSig;
            incTx >> scriptSig;
            scriptSig.reset_it();
            if (0 == scriptSig.size()) {
                rv = JUBR_ARGUMENTS_BAD;
                break;
            }

            uint8_t scriptSigFirst = scriptSig.read_uint8();
            std::vector<std::string> vInputSignature;
            if ((uint8_t)OpCode::OP_0 == scriptSigFirst) {
                auto scriptSigIt = scriptSig.begin();
                do {
                    uint32_t itemLen = scriptSig.read_uint8();
                    if ((uint8_t)OpCode::OP_PUSHDATA1 == itemLen) {
                        itemLen = scriptSig.read_uint8();
                    }
                    else if ((uint8_t)OpCode::OP_PUSHDATA2 == itemLen) {
                        itemLen = scriptSig.read_le_uint16();
                    }
                    else if ((uint8_t)OpCode::OP_PUSHDATA4 == itemLen) {
                        itemLen = scriptSig.read_le_uint32();
                    }
                    if (                  0 == itemLen
                        || scriptSig.size()  < itemLen
                        ) {
                        return JUBR_ARGUMENTS_BAD;
                    }
                    uchar_vector item = scriptSig.read_vector(itemLen);
                    uint8_t tag = item[0];
                    if (0x30 == tag) {
                        // signature
                    }
                    else {
                        // redeemScript
                    }

                    scriptSigIt = scriptSig.get_cur_it();
                } while (scriptSigIt != scriptSig.end());
            }
        }
    }

    // [nLockTime]
    *plockTime = incTx.read_le_uint32();

    if (incTx.get_cur_it() != incTx.end()) {
        vInputs.clear();
        for (auto& output:vOutputs) {
            if (NULL != output.stdOutput.address) {
                delete[] output.stdOutput.address;
                output.stdOutput.address = NULL;
            }
        }
        vOutputs.clear();
        *plockTime = 0;
        JUB_VERIFY_RV(JUBR_ARGUMENTS_BAD);
    }

    return JUBR_OK;
}


// !!Not handling witness yet
JUB_RV JubiterBaseBTCImpl::DeserializeTx(bool bWitness,
                                         const std::vector<uchar_vector>& vRedeemScript,
                                         const std::string hexIncTx,
                                         MAP_COSIGNER_SIGNATURE& mapCosignerSignature,
                                         uchar_vector& unsignedTx) {

    JUB_RV rv = JUBR_OK;

    if ("" == hexIncTx) {
        return JUBR_MEMORY_NULL_PTR;
    }

    // [nVersion][nInputCount][txInputs][nOutputCount][txOutputs][nLockTime]
    uchar_vector incTx;
    incTx << HexStr2CharPtr(hexIncTx);
    incTx.reset_it();

    // [nVersion]
    auto version = incTx.read_le_uint32();
    if (version1 != version) {
        return JUBR_ARGUMENTS_BAD;
    }
    unsignedTx << version;

    // [nInputCount]
    auto inputCount = incTx.read_uint8();
    unsignedTx << inputCount;
    // [txInputs]
    for (auto inputIndex=0; inputIndex<inputCount; ++inputIndex) {
        uchar_vector preHash = incTx.read_vector(32);
        unsignedTx << preHash;
        preHash.reverse();
        unsignedTx << incTx.read_vector(4);

        // <scriptSig>
        uchar_vector scriptSig;
        incTx >> scriptSig;
        scriptSig.reset_it();
        if (0 == scriptSig.size()) {
            rv = JUBR_ARGUMENTS_BAD;
            break;
        }

        uint8_t scriptSigFirst = scriptSig.read_uint8();
        std::vector<std::string> vInputSignature;
        if ((uint8_t)OpCode::OP_0 == scriptSigFirst) {
            // SCRIPT_BTC_TYPE::P2SH_MULTISIG;
            auto scriptSigIt = scriptSig.begin();
            do {
                uint32_t itemLen = scriptSig.read_uint8();
                if ((uint8_t)OpCode::OP_PUSHDATA1 == itemLen) {
                    itemLen = scriptSig.read_uint8();
                }
                else if ((uint8_t)OpCode::OP_PUSHDATA2 == itemLen) {
                    itemLen = scriptSig.read_le_uint16();
                }
                else if ((uint8_t)OpCode::OP_PUSHDATA4 == itemLen) {
                    itemLen = scriptSig.read_le_uint32();
                }
                if (                  0 == itemLen
                    || scriptSig.size()  < itemLen
                    ) {
                    rv = JUBR_ARGUMENTS_BAD;
                    break;
                }
                uchar_vector item = scriptSig.read_vector(itemLen);
                uint8_t tag = item[0];
                if (0x30 == tag) {
                    // signature
                    uchar_vector signatureRaw;
                    signatureRaw & item;
                    vInputSignature.push_back(signatureRaw.getHex());
                }
                else {
                    // redeemScript check
                    if (item != vRedeemScript[inputIndex]) {
                        rv = JUBR_ARGUMENTS_BAD;
                        break;
                    }
                }

                scriptSigIt = scriptSig.get_cur_it();
            } while (scriptSigIt != scriptSig.end());
        }
        else {
            unsignedTx << scriptSig;
        }

        mapCosignerSignature.insert({std::to_string(inputIndex), vInputSignature});
        unsignedTx << (JUB_BYTE)0x00;
        unsignedTx << incTx.read_vector(4);
    }
    JUB_VERIFY_RV(rv);

    // [nOutputCount]
    auto outputCount = incTx.read_uint8();
    unsignedTx << outputCount;
    // [txOutputs]
    for (auto i=0; i<outputCount; ++i) {
        unsignedTx << incTx.read_vector(8);

        uchar_vector scriptPub;
        incTx >> scriptPub;
        unsignedTx && scriptPub;
    }

    // [nLockTime]
    unsignedTx << incTx.read_vector(4);

    if (incTx.get_cur_it() != incTx.end()) {
        unsignedTx.clear();
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}

JUB_RV JubiterBaseBTCImpl::_scriptPubKey(const TWCoinType &coin, const TW::PublicKey publicKey,
                                         TW::Bitcoin::Script &script) {

    // scriptPubKey
    uint8_t prefix = TWCoinTypeP2pkhPrefix(coin);
    TW::Bitcoin::Address addr(publicKey, prefix);
    script = TW::Bitcoin::Script::buildForAddress(addr.string(), coin);
    if (script.empty()) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}

JUB_RV JubiterBaseBTCImpl::_scriptCode(const TWCoinType &coin, const TW::PublicKey publicKey,
                                       TW::Bitcoin::Script &scriptCode) {

    return _scriptPubKey(coin, publicKey, scriptCode);
}

JUB_RV JubiterBaseBTCImpl::_verifyPayToPublicKeyHashScriptSig(const TWCoinType &coin,
                                                              const TW::Bitcoin::Transaction &tx, const size_t index,
                                                              const uint32_t &hashType, const uint64_t amount,
                                                              const TW::Data &signature, const TW::PublicKey publicKey,
                                                              bool witness) {

    JUB_RV rv = JUBR_OK;

    // script code - scriptPubKey
    TW::Bitcoin::Script scriptCode;
    rv = _scriptCode(coin, publicKey, scriptCode);
    if (JUBR_OK != rv) {
        return rv;
    }

    TW::Data preImage;
    if (!witness) {
        preImage = tx.getPreImage(scriptCode, index, hashType);
    } else {
        preImage = tx.getPreImage(scriptCode, index, hashType, amount);
    }
    const auto begin = reinterpret_cast<const uint8_t *>(preImage.data());
    TW::Data digest  = tx.hasher(begin, preImage.size());
    if (!publicKey.verifyAsDER(signature, digest)) {
        rv = JUBR_ERROR;
    }

    return rv;
}

JUB_RV JubiterBaseBTCImpl::_verifyPayToWitnessPublicKeyHashScriptSig(const TWCoinType &coin,
                                                                     const TW::Bitcoin::Transaction &tx,
                                                                     const size_t index, const uint32_t &hashType,
                                                                     const uint64_t amount, const TW::Data &signature,
                                                                     const TW::PublicKey publicKey) {
    auto program    = publicKey.hash({});
    auto scriptCode = TW::Bitcoin::Script::buildPayToPublicKeyHash(program);
    TW::Data preImage;
    preImage = tx.getPreImage(scriptCode, index, hashType, amount);

    const auto begin = reinterpret_cast<const uint8_t *>(preImage.data());
    TW::Data digest  = tx.hasher(begin, preImage.size());
    if (!publicKey.verifyAsDER(signature, digest)) {
        return JUBR_VERIFY_SIGN_FAILED;
    }

    return JUBR_OK;
}

JUB_RV JubiterBaseBTCImpl::_verifyPayToTaprootKeyPathSpending(const TWCoinType &coin,
                                                              const TW::Bitcoin::Transaction &tx, const size_t index,
                                                              const uint32_t &hashType, const TW::Data &signature,
                                                              const TW::PublicKey publicKey) {
    // notused
    (void)hashType;

    uint8_t hash_type = signature.size() == 64 ? 0 : signature.at(64);
    if (hash_type == 0 && signature.size() == 65) {
        return JUBR_VERIFY_SIGN_FAILED;
    }
    // key path spending not have annex
    auto sigMsg = tx.getSigMsg(index, hashType, 0, {});
    // insert epoch
    sigMsg.insert(sigMsg.begin(), 0);
    TW::Data digest = TW::Hash::hash_TapSighash(sigMsg);
    if (!publicKey.verifySchnorr(signature, digest)) {
        return JUBR_VERIFY_SIGN_FAILED;
    }

    return JUBR_OK;
}

JUB_RV JubiterBaseBTCImpl::_verifyTx(const JUB_ENUM_BTC_TRANS_TYPE &type, const TWCoinType &coin,
                                     const TW::Bitcoin::Transaction *tx, const uint32_t &hashType,
                                     const std::vector<JUB_UINT64> &vInputAmount,
                                     const std::vector<TW::PublicKey> &vInputPublicKey) {

    JUB_RV rv = JUBR_OK;

    for (size_t index = 0; index < tx->inputs.size(); ++index) {
        rv = JUBR_ERROR;

        TW::Bitcoin::Script script = tx->inputs[index]->script;
        // support native witness transaction
        if (script.empty() && tx->inputs[index]->scriptWitness.empty()) {
            rv = JUBR_ARGUMENTS_BAD;
            break;
        }

        // native witness transaction
        if (script.empty()) {
            TW::Data signature;
            TW::Data publicKey;
            switch (type) {
            case p2sh_p2wpkh: {
                // https://github.com/bitcoin/bips/blob/master/bip-0141.mediawiki#witness-program
                // The witness must consist of exactly 2 items (≤ 520 bytes each). The first one a signature, and the
                // second one a public key
                TW::Bitcoin::Script::parseWitnessStackToPayToWitnessScriptHash(tx->inputs[index]->scriptWitness,
                                                                               signature, publicKey);
                if (vInputPublicKey[index].bytes != publicKey) {
                    return JUBR_ERROR;
                }
                rv = _verifyPayToWitnessPublicKeyHashScriptSig(coin, *tx, index, hashType, vInputAmount[index],
                                                               signature, vInputPublicKey[index]);
            } break;
            case p2wpkh: {
                // https://github.com/bitcoin/bips/blob/master/bip-0141.mediawiki#witness-program
                // The witness must consist of exactly 2 items (≤ 520 bytes each).
                // The first one a signature, and the second one a public key
                TW::Bitcoin::Script::parseWitnessStackToPayToWitnessScriptHash(tx->inputs[index]->scriptWitness,
                                                                               signature, publicKey);
                // P2WPKH
                if (publicKey.size() != 33 || vInputPublicKey[index].bytes != publicKey) {
                    return JUBR_ERROR;
                }
                rv = _verifyPayToWitnessPublicKeyHashScriptSig(coin, *tx, index, hashType, vInputAmount[index],
                                                               signature, vInputPublicKey[index]);
            } break;
            case p2tr: {
                // https://github.com/bitcoin/bips/blob/master/bip-0341.mediawiki#constructing-and-spending-taproot-outputs
                // Spending using the key path A Taproot output can be spent with the secret key corresponding to the
                // internal_pubkey. To do so, a witness stack consists of a single element: a BIP340 signature on the
                // signature hash as defined above
                auto &signature = tx->inputs[index]->scriptWitness.front();
                rv = _verifyPayToTaprootKeyPathSpending(coin, *tx, index, hashType, signature, vInputPublicKey[index]);

            } break;
            default: {
                rv = JUBR_ARGUMENTS_BAD;
                break;
            }
            }
            if (rv) {
                break;
            }
            continue;
        } else {
            rv = JUBR_OK;
        }
        if (JUBR_OK != rv) {
            break;
        }

        TW::Bitcoin::Script witnessProgram;
        if (!witnessProgram.decode(script.bytes)) {
            rv = JUBR_ERROR;
            break;
        }

        TW::Data signature;
        TW::Data publicKey;
        switch (type) {
        case p2pkh: {
            if (!script.matchPayToPublicKeyHashScriptSig(signature, publicKey)) {
                rv = JUBR_ERROR;
                break;
            }
            if (vInputPublicKey[index].bytes != publicKey) {
                continue;
            }

            rv = _verifyPayToPublicKeyHashScriptSig(coin, *tx, index, hashType, vInputAmount[index], signature,
                                                    vInputPublicKey[index]);
        } break;
        case p2sh_p2wpkh: {
            if (!witnessProgram.isWitnessProgram()) {
                rv = JUBR_ERROR;
                break;
            }
            if (!TW::Bitcoin::Script::parseWitnessStackToPayToWitnessScriptHash(tx->inputs[index]->scriptWitness,
                                                                                signature, publicKey)) {
                rv = JUBR_ERROR;
                break;
            }

            if (vInputPublicKey[index].bytes != publicKey) {
                continue;
            }

            rv = _verifyPayToPublicKeyHashScriptSig(coin, *tx, index, hashType, vInputAmount[index], signature,
                                                    vInputPublicKey[index], true);
        } break;
        default: {
            rv = JUBR_ARGUMENTS_BAD;
        } break;
        }
        if (JUBR_OK != rv) {
            break;
        } else {
            rv = JUBR_OK;
        }
    }

    return rv;
}

JUB_RV JubiterBaseBTCImpl::_verifyTx(const JUB_ENUM_BTC_TRANS_TYPE &type, const uchar_vector &signedRaw,
                                     const std::vector<JUB_UINT64> &vInputAmount,
                                     const std::vector<TW::Data> &vInputPublicKey, const TWCoinType &coinNet) {

    JUB_RV rv = JUBR_ARGUMENTS_BAD;

    auto witness = type == p2sh_p2wpkh || type == p2wpkh || type == p2tr;

    // 目前来看我们组交易，在一个交易里的所有input都是同一类型
    TW::Hash::Hasher hasher =
        type == p2tr ? static_cast<TW::Hash::HasherSimpleType>(TW::Hash::sha256) : TW::Hash::sha256d;

    try {
        TW::Bitcoin::Transaction tx(hasher);
        if (!tx.decode(witness, signedRaw)) {
            return rv;
        }

        std::vector<TW::PublicKey> vInputPubkey;
        for (const auto &inputPublicKey : vInputPublicKey) {
            vInputPubkey.push_back(TW::PublicKey(TW::Data(inputPublicKey), _publicKeyType));
        }
        if (type == p2tr) {
            for (auto index = 0; index < vInputPublicKey.size(); index++) {
                auto pub    = vInputPublicKey.at(index);
                auto amount = vInputAmount.at(index);
                auto input  = tx.inputs.at(index);
                // https://github.com/bitcoin/bips/blob/master/bip-0341.mediawiki#taproot-key-path-spending-signature-validation
                auto script =
                    TW::Bitcoin::Script::buildPayToTaprootKeyPathSpending({pub.begin() + 1, pub.begin() + 33});
                input->spending(amount, script);
            }
        }

        return JubiterBaseBTCImpl::_verifyTx(type, (coinNet ? coinNet : _coin), &tx, _hashType, vInputAmount,
                                             vInputPubkey);
    } catch (...) {
        rv = JUBR_ERROR;
    }

    return rv;
}

JUB_RV JubiterBaseBTCImpl::_serializeTx(const JUB_ENUM_BTC_TRANS_TYPE &type,
                                        const std::vector<JUB_UINT64> &vInputAmount,
                                        const std::vector<TW::Data> &vInputPublicKey,
                                        const std::vector<uchar_vector> &vSignatureRaw, TW::Bitcoin::Transaction *tx,
                                        uchar_vector &signedRaw) {
    JUB_RV rv = JUBR_OK;

    auto witness = type == p2sh_p2wpkh || type == p2wpkh || type == p2tr;

    for (size_t index = 0; index < tx->inputs.size(); ++index) {
        switch (type) {
        case p2pkh: {
            tx->inputs[index]->script =
                TW::Bitcoin::Script::buildPayToPublicKeyHashScriptSig(vSignatureRaw[index], vInputPublicKey[index]);
        } break;
        case p2sh_p2wpkh: {
            TW::PublicKey twpk = TW::PublicKey(vInputPublicKey[index], _publicKeyType);

            TW::Data scriptPubkey;
            TW::Bitcoin::Script::buildPayToWitnessPubkeyHash(twpk.hash(TW::Data())).encode(scriptPubkey);
            tx->inputs[index]->script.bytes = scriptPubkey;

            tx->inputs[index]->scriptWitness = TW::Bitcoin::Script::buildPayToPublicKeyHashScriptSigWitness(
                vSignatureRaw[index], vInputPublicKey[index]);
            if (tx->inputs[index]->scriptWitness.empty()) {
                rv = JUBR_ARGUMENTS_BAD;
                break;
            }
        } break;
        case p2wpkh: {
            tx->inputs[index]->scriptWitness = TW::Bitcoin::Script::buildPayToPublicKeyHashScriptSigWitness(
                vSignatureRaw[index], vInputPublicKey[index]);
            if (tx->inputs[index]->scriptWitness.empty()) {
                rv = JUBR_ARGUMENTS_BAD;
                break;
            }
        } break;
        case p2tr: {
            tx->inputs[index]->scriptWitness =
                TW::Bitcoin::Script::buildPayToTaprootKeyPathSpendingWitness(vSignatureRaw[index]);
            if (tx->inputs[index]->scriptWitness.empty()) {
                rv = JUBR_ARGUMENTS_BAD;
                break;
            }
        } break;
        default: {
            rv = JUBR_ARGUMENTS_BAD;
        } break;
        }
    }
    if (JUBR_OK != rv) {
        return rv;
    }

    tx->encode(witness, signedRaw);
    if (0 >= signedRaw.size()) {
        return JUBR_ERROR;
    }

    return JUBR_OK;
}

} // namespace token
} // namespace jub
