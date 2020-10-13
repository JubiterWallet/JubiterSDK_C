#include <token/TRX/JubiterBaseTRXImpl.h>
#include <bigint/BigIntegerUtils.hh>
#include <TrezorCrypto/bip32.h>
#include <utility/util.h>

#include "Tron/Address.h"
#include "Tron/Transaction.h"
#include "Tron/Serialization.h"
#include <PublicKey.h>
#include "../PrivateKey.h"
#include "../HexCoding.h"

namespace jub {
namespace token {


JUB_RV JubiterBaseTRXImpl::_getPubkeyFromXpub(const std::string& xpub, TW::Data& publicKey,
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


JUB_RV JubiterBaseTRXImpl::_getAddress(const TW::Data& publicKey, std::string& address) {

    try {
        TW::PublicKey twpk = TW::PublicKey(publicKey, _publicKeyType);
        TW::Tron::Address addr(twpk.extended());
        address = addr.string();
        if (!addr.isValid(address)) {
            return JUBR_ERROR;
        }
    }
    catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}


JUB_RV JubiterBaseTRXImpl::SerializePreimage(const std::string& packedContractInPb,
                                             uchar_vector& preimageRaw) {

//    JUB_RV rv = JUBR_OK;

    try {
        // build transaction
//        TW::Tron::Transaction tx;
//        tx.deserialize(uchar_vector(packedContractInPb));
        preimageRaw = uchar_vector(packedContractInPb);
    }
    catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}


JUB_RV JubiterBaseTRXImpl::PackTransactionRaw(const JUB_TX_TRX& tx,
                                              std::string& packedContractInPB) {

    try {
        uchar_vector vRefBlockBytes(tx.ref_block_bytes);

        int64_t bigRefBlockNum = 0;
        if (nullptr != tx.ref_block_num) {
            bigRefBlockNum = stringToBigInteger(tx.ref_block_num).toLong();
        }

        uchar_vector vRefBlockHash(tx.ref_block_hash);

        int64_t bigExpiration = stringToBigInteger(tx.expiration).toLong();
        int64_t bigFeeLimit   = stringToBigInteger(tx.fee_limit).toLong();

        std::vector<TW::Tron::Authority> vAuthorities = {};

        uchar_vector vData;
        if (nullptr != tx.data) {
            vData = uchar_vector(tx.data);
        }

        std::vector<TW::Tron::TransactionContract> vContracts;
        for (int i=0; i<tx.contractCount; ++i) {
            TW::Tron::TransactionContract contract;
            contract.type = tx.contracts[i].type;
            JUB_VERIFY_RV(SerializeContract(tx.contracts[i], contract.parameter));
            contract.permission_id = 0;
            vContracts.push_back(contract);
        }

        int64_t bigTimestamp = 0;
        if (nullptr != tx.timestamp) {
            bigTimestamp = stringToBigInteger(tx.timestamp).toLong();
        }

        TW::Tron::TransactionRaw txRaw(vRefBlockBytes, bigRefBlockNum, vRefBlockHash,
                                       bigExpiration,
                                       vAuthorities,
                                       vData,
                                       vContracts,
                                       {},
                                       bigTimestamp,
                                       bigFeeLimit);
        packedContractInPB = uchar_vector(txRaw.serialize()).getHex();
    }
    catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}


JUB_RV JubiterBaseTRXImpl::SerializeContract(const JUB_CONTRACT_TRX& contract,
                                             ::google::protobuf::Any& parameter) {

    try {
        switch (contract.type) {
        case JUB_ENUM_TRX_CONTRACT_TYPE::XFER_CONTRACT:
        {
            JUB_CHECK_NULL(contract.transfer.owner_address);
            JUB_CHECK_NULL(contract.transfer.to_address);

            parameter = TW::Tron::TransactionContract::to_parameter(
                            TW::Tron::TransferContract(
                                contract.transfer.owner_address,
                                contract.transfer.to_address,
                                contract.transfer.amount
                            )
                        );
            break;
        }
        case JUB_ENUM_TRX_CONTRACT_TYPE::XFER_ASSET_CONTRACT:
        {
            JUB_CHECK_NULL(contract.transferAsset.asset_name);
            JUB_CHECK_NULL(contract.transferAsset.owner_address);
            JUB_CHECK_NULL(contract.transferAsset.to_address);

            parameter = TW::Tron::TransactionContract::to_parameter(
                            TW::Tron::TransferAssetContract(
                                contract.transferAsset.asset_name,
                                contract.transferAsset.owner_address,
                                contract.transferAsset.to_address,
                                contract.transferAsset.amount
                            )
                        );
            break;
        }
        case JUB_ENUM_TRX_CONTRACT_TYPE::CREATE_SMART_CONTRACT:
        {
            JUB_CHECK_NULL(contract.createSmart.owner_address);
            JUB_CHECK_NULL(contract.createSmart.bytecode);
            ::protocol::SmartContract pb;
            TW::Data bytecode(uchar_vector(contract.createSmart.bytecode));
            TW::Tron::SmartContract::deserialize(bytecode);
            parameter = TW::Tron::TransactionContract::to_parameter(
                            TW::Tron::CreateSmartContract(
                                contract.createSmart.owner_address,
                                pb,
                                contract.createSmart.call_token_value,
                                contract.createSmart.token_id
                            )
                        );
            break;
        }
        case JUB_ENUM_TRX_CONTRACT_TYPE::TRIG_SMART_CONTRACT:
        {
            JUB_CHECK_NULL(contract.triggerSmart.owner_address);
            JUB_CHECK_NULL(contract.triggerSmart.contract_address);
            JUB_CHECK_NULL(contract.triggerSmart.data);

            parameter = TW::Tron::TransactionContract::to_parameter(
                            TW::Tron::TriggerSmartContract(
                                contract.triggerSmart.owner_address,
                                contract.triggerSmart.contract_address,
                                contract.triggerSmart.call_value,
                                TW::parse_hex(contract.triggerSmart.data),
                                contract.triggerSmart.call_token_value,
                                contract.triggerSmart.token_id
                            )
                        );
            break;
        }
        default:
            return JUBR_ARGUMENTS_BAD;
        }
    }
    catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}


} // namespace token end
} // namespace jub end
