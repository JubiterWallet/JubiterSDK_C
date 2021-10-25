#include "token/CKB/JubiterBaseCKBImpl.h"
#include <TrustWalletCore/TWCurve.h>
#include <TrustWalletCore/TWNervosCKBSigHashType.h>
#include <NervosCKB/SegwitAddress.h>
#include <NervosCKB/Serialization.hpp>
#include <TrezorCrypto/ecdsa.h>

namespace jub {
namespace token {


JUB_RV JubiterBaseCKBImpl::ParseAddress(const std::string& address,
                                        int* format,
                                        int* codeHashIndex,
                                        std::string& codeHash,
                                        std::string& args) {

    auto result = TW::NervosCKB::SegwitAddress::decode(address);
    if (!result.second) {
        return JUBR_ARGUMENTS_BAD;
    }

    *format = result.first.format;
    *codeHashIndex = result.first.codeHashIndex;

    if (0 < result.first.codeHash.size()) {
        codeHash = uchar_vector(result.first.codeHash).getHex();
    }

    if (0 < result.first.args.size()) {
        args = uchar_vector(result.first.args).getHex();
    }

    return JUBR_OK;
}


JUB_RV JubiterBaseCKBImpl::IsValidAddress(const std::string& address) {

    return !TW::NervosCKB::SegwitAddress::isValid(address);
}


JUB_RV JubiterBaseCKBImpl::CheckAddress(const std::string& address, const TWCoinType& coinNet) {

    return JubiterBaseCKBImpl::IsValidAddress(address);
}


JUB_RV JubiterBaseCKBImpl::_unsignedTx(const uint32_t coin,
                                       const JUB_UINT32 version,
                                       const std::vector<CELL_DEP>& vDeps,
                                       const std::vector<CELL_INPUT>& vInputs,
                                       const std::vector<CELL_OUTPUT>& vOutputs,
                                       TW::NervosCKB::Transaction& tx) {

    JUB_RV rv = JUBR_OK;

    auto lambdaHex2Bytes = [](JUB_CHAR_PTR hex) -> TW::Data {

        return (nullptr == hex) ? TW::Data()
                                : uchar_vector(ETHHexStr2CharPtr(std::string(hex)));
    };

    tx.version = version;

    // deps
    for (const auto& dep:vDeps) {
        TW::NervosCKB::TransactionCellDep *txDep = new TW::NervosCKB::TransactionCellDep(
            TW::Bitcoin::OutPoint(
                lambdaHex2Bytes(dep.txHash),
                dep.index),
            dep.type);

        tx.cellDeps.push_back(txDep);
    }   // for (const auto& dep:vDeps) end

    // inputs
    for (const auto& input:vInputs) {
        TW::Data lockScript;
        if (!TW::NervosCKB::Script::buildLockScript(
                lambdaHex2Bytes(input.lock.codeHash),
                input.lock.hashType,
                lambdaHex2Bytes(input.lock.args),
                lockScript)) {
            continue;
        }

        TW::NervosCKB::TransactionInput *txInput = new TW::NervosCKB::TransactionInput(
            TW::Bitcoin::OutPoint(
                lambdaHex2Bytes(input.preHash),
                input.preIndex),
            input.since,
            lockScript);

        tx.inputs.push_back(txInput);
    }   // for (const auto& input:vInputs) end

    // outputs
    for (const auto& output:vOutputs) {
        TW::Data lockScript;
        TW::Data typeScript;
        if (   !TW::NervosCKB::Script::buildLockScript(
                    lambdaHex2Bytes(output.lock.codeHash),
                    output.lock.hashType,
                    lambdaHex2Bytes(output.lock.args),
                    lockScript)
            || !TW::NervosCKB::Script::buildTypeScript(
                    lambdaHex2Bytes(output.type.codeHash),
                    output.type.hashType,
                    lambdaHex2Bytes(output.type.args),
                    typeScript)
            ) {
            continue;
        }

        TW::NervosCKB::TransactionOutput *txOutput = new TW::NervosCKB::TransactionOutput(
            output.capacity,
            lockScript,
            typeScript,
            lambdaHex2Bytes(output.data));

        tx.outputs.push_back(txOutput);
    }   // for (const auto& output:vOutputs) end
    if (!tx.isValid()) {
        return JUBR_ARGUMENTS_BAD;
    }

    return rv;
}


JUB_RV JubiterBaseCKBImpl::SerializeUnsignedTx(const JUB_UINT32 version,
                                               const std::vector<CELL_DEP>& vDeps,
                                               const std::vector<CELL_INPUT>& vInputs,
                                               const std::vector<CELL_OUTPUT>& vOutputs,
                                               TW::Data& unsignedRaw,
                                               const TWCoinType& coinNet) {

    JUB_RV rv = JUBR_ERROR;

    TW::NervosCKB::Transaction tx(version);
    rv = UnsignedTx(version,
                    vDeps,
                    vInputs,
                    vOutputs,
                    tx);
    if (JUBR_OK != rv) {
        return rv;
    }

    tx.serialize(unsignedRaw);

    return JUBR_OK;
}


JUB_RV JubiterBaseCKBImpl::UnsignedTx(const JUB_UINT32 version,
                                      const std::vector<CELL_DEP>& vDeps,
                                      const std::vector<CELL_INPUT>& vInputs,
                                      const std::vector<CELL_OUTPUT>& vOutputs,
                                      TW::NervosCKB::Transaction& unsignedTx,
                                      const TWCoinType& coinNet) {

    JUB_RV rv = JUBR_ERROR;

    rv = JubiterBaseCKBImpl::_unsignedTx((coinNet?coinNet:_coin),
                                         version,
                                         vDeps,
                                         vInputs,
                                         vOutputs,
                                         unsignedTx);
    if (JUBR_OK != rv) {
        return rv;
    }

    return JUBR_OK;
}


JUB_RV JubiterBaseCKBImpl::SignTX(const JUB_ENUM_BTC_TRANS_TYPE& type,
                                  const std::vector<std::string>& vInputPath,
                                  const JUB_UINT32 version,
                                  const std::vector<CELL_DEP>& vDeps,
                                  const std::vector<CELL_INPUT>& vInputs,
                                  const std::vector<CELL_OUTPUT>& vOutputs,
                                  std::vector<uchar_vector>& vSignatureRaw,
                                  const TWCoinType& coinNet) {

    JUB_RV rv = JUBR_OK;

    try {
        TW::NervosCKB::Transaction tx;
        JUB_VERIFY_RV(UnsignedTx(version, vDeps, vInputs, vOutputs, tx));

        JUB_VERIFY_RV(_SignTX(vInputPath, tx, coinNet));

        for (const auto& witness:tx.witnesses) {
            vSignatureRaw.push_back(witness.lock);
        }
    }
    catch (...) {
        rv = JUBR_ERROR;
    }

    return rv;
}


JUB_RV JubiterBaseCKBImpl::VerifyTX(const JUB_ENUM_BTC_TRANS_TYPE& type,
                                    const std::vector<std::string>& vInputPath,
                                    const JUB_UINT32 version,
                                    const std::vector<CELL_DEP>& vDeps,
                                    const std::vector<CELL_INPUT>& vInputs,
                                    const std::vector<CELL_OUTPUT>& vOutputs,
                                    const std::vector<uchar_vector>& vSignatureRaw,
                                    const TWCoinType& coinNet) {

    JUB_RV rv = JUBR_ARGUMENTS_BAD;

    try {
        TW::NervosCKB::Transaction unsignedTx;
        JUB_VERIFY_RV(UnsignedTx(version, vDeps, vInputs, vOutputs, unsignedTx));

        return _VerifyTX(vInputPath,
                         unsignedTx,
                         vSignatureRaw,
                         coinNet);
    }
    catch (...) {
        rv = JUBR_ERROR;
    }

    return rv;
}


JUB_RV JubiterBaseCKBImpl::_verifyTX(const TW::NervosCKB::Transaction& tx,
                                     const std::vector<TW::PublicKey>& vInputPublicKey,
                                     const TWCoinType& coinNet) {

    JUB_RV rv = JUBR_ERROR;

    for (size_t index=0; index<vInputPublicKey.size(); ++index) {
        TW::PublicKey twpk = TW::PublicKey(vInputPublicKey[index]);
        if (   !tx.witnesses[index].isZero()
            && twpk.isValid()
            ) {
            Data preImageHash = tx.getPreImageHash(index);
            TW::Data sign = tx.witnesses[index].lock;
            if (!twpk.verify(sign, preImageHash)) {
                rv = JUBR_VERIFY_SIGN_FAILED;
                break;
            }
            else {
                rv = JUBR_OK;
            }
        }
    }

    return rv;
}


JUB_RV JubiterBaseCKBImpl::_getSegwitAddress(const TW::Data& publicKey, std::string& address, const TWCoinType& coinNet) {

    try {
        auto segwitAddress = TW::NervosCKB::SegwitAddress::fromPublickey(
                                std::string(stringForHRP(TWCoinTypeHRP(_coin, coinNet))),
                                TW::NervosCKB::TWNervosCKBAddressFormat::TWNervosCKBAddressShort,
                                TWNervosCKBCodeHashIndex::TWNervosCKBSingleSig,
                                publicKey);
        if (!segwitAddress.second) {
            return JUBR_ARGUMENTS_BAD;
        }

        address = segwitAddress.first.string();
    }
    catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}


} // namespace token end
} // namespace jub end
