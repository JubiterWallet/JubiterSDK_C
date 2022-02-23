#include "token/TRX/JubiterBaseTRXImpl.h"
#include <bigint/BigIntegerUtils.hh>
#include <TrezorCrypto/bip32.h>
#include "utility/util.h"

#include <Tron/Address.h>
#include <Tron/Transaction.h>
#include <Tron/Serialization.h>
#include <PublicKey.h>
#include <../PrivateKey.h>
#include <../HexCoding.h>

namespace jub {
namespace token {


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


JUB_RV JubiterBaseTRXImpl::IsValidAddress(const std::string& address, std::string& hex) {

    try {
        if (!TW::Tron::Address::isValid(address)) {
            return JUBR_ARGUMENTS_BAD;
        }

        hex = uchar_vector(TW::Tron::Address::toHex(address)).getHex();
    }
    catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}


JUB_RV JubiterBaseTRXImpl::CheckAddress(const std::string& address, std::string& hex) {

    return JubiterBaseTRXImpl::IsValidAddress(address, hex);
}


JUB_RV JubiterBaseTRXImpl::SerializePreimage(const std::string& packedContractInPb,
                                             uchar_vector& preimageRaw) {

    try {
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
        case JUB_ENUM_TRX_CONTRACT_TYPE::ACCT_PERM_UPDATE_CONTRACT: {
            JUB_CHECK_NULL(contract.acctPermUpdate.owner_address);

            if (!TW::Tron::Address::isValid(contract.acctPermUpdate.owner_address)) {
                return JUBR_ARGUMENTS_BAD;
            }

            std::vector<TW::Tron::Key> owner_keys;
            for (JUB_UINT16 j=0; j<contract.acctPermUpdate.owner.keyCount; ++j) {
                owner_keys.push_back(
                    TW::Tron::Key(
                        std::string(contract.acctPermUpdate.owner.keys[j].address),
                        contract.acctPermUpdate.owner.keys[j].weight
                    )
                );
            }

            std::vector<TW::Tron::Permission> actives;
            for (JUB_UINT16 i=0; i<contract.acctPermUpdate.activeCount; ++i) {
                std::vector<TW::Tron::Key> keys;
                for (JUB_UINT16 j=0; j<contract.acctPermUpdate.actives[i].keyCount; ++j) {
                    keys.push_back(
                        TW::Tron::Key(
                            std::string(contract.acctPermUpdate.actives[i].keys[j].address),
                            contract.acctPermUpdate.actives[i].keys[j].weight
                        )
                    );
                }
                actives.push_back(
                    TW::Tron::Permission(
                        std::string(contract.acctPermUpdate.actives[i].permission_name),
                        contract.acctPermUpdate.actives[i].threshold,
                        uchar_vector(contract.acctPermUpdate.actives[i].operations),
                        keys
                    )
                );
            }

            parameter = TW::Tron::TransactionContract::to_parameter(
                            TW::Tron::AccountPermissionUpdateContract(
                                std::string(contract.acctPermUpdate.owner_address),
                                TW::Tron::Permission(
                                    std::string(contract.acctPermUpdate.owner.permission_name),
                                    contract.acctPermUpdate.owner.threshold,
                                    owner_keys
                                ),
                                TW::Tron::Permission(),
                                actives
                            )
                        );
        } break;
        case JUB_ENUM_TRX_CONTRACT_TYPE::XFER_CONTRACT: {
            JUB_CHECK_NULL(contract.transfer.owner_address);
            JUB_CHECK_NULL(contract.transfer.to_address);

            if (   !TW::Tron::Address::isValid(contract.transfer.owner_address)
                || !TW::Tron::Address::isValid(contract.transfer.to_address)
                ) {
                return JUBR_ARGUMENTS_BAD;
            }

            parameter = TW::Tron::TransactionContract::to_parameter(
                            TW::Tron::TransferContract(
                                contract.transfer.owner_address,
                                contract.transfer.to_address,
                                contract.transfer.amount
                            )
                        );
        } break;
        case JUB_ENUM_TRX_CONTRACT_TYPE::XFER_ASSET_CONTRACT: {
            JUB_CHECK_NULL(contract.transferAsset.asset_name);
            JUB_CHECK_NULL(contract.transferAsset.owner_address);
            JUB_CHECK_NULL(contract.transferAsset.to_address);

            if (   !TW::Tron::Address::isValid(contract.transferAsset.owner_address)
                || !TW::Tron::Address::isValid(contract.transferAsset.to_address)
                ) {
                return JUBR_ARGUMENTS_BAD;
            }

            parameter = TW::Tron::TransactionContract::to_parameter(
                            TW::Tron::TransferAssetContract(
                                contract.transferAsset.asset_name,
                                contract.transferAsset.owner_address,
                                contract.transferAsset.to_address,
                                contract.transferAsset.amount
                            )
                        );
        } break;
        case JUB_ENUM_TRX_CONTRACT_TYPE::FRZ_BLA_CONTRACT: {
            JUB_CHECK_NULL(contract.freezeBalance.owner_address);
            JUB_CHECK_NULL(contract.freezeBalance.receiver_address);

            if (   !TW::Tron::Address::isValid(contract.freezeBalance.owner_address)
                || !TW::Tron::Address::isValid(contract.freezeBalance.receiver_address)
                ) {
                return JUBR_ARGUMENTS_BAD;
            }

            // When resource == bandwidth, pb will not encode this item,
            // so we need to override it's serialize()
            TW::Tron::FreezeBalanceContract value(
                contract.freezeBalance.owner_address,
                contract.freezeBalance.frozen_balance,
                contract.freezeBalance.frozen_duration,
                TW::Tron::toInternal(contract.freezeBalance.resource),
                contract.freezeBalance.receiver_address
            );
            parameter.set_type_url(TW::Tron::TransactionContract::to_parameter(value).type_url());
            auto vValue = value.serialize();
            parameter.set_value(&vValue[0], vValue.size());
        } break;
        case JUB_ENUM_TRX_CONTRACT_TYPE::UNFRZ_BLA_CONTRACT: {
            JUB_CHECK_NULL(contract.unfreezeBalance.owner_address);
            JUB_CHECK_NULL(contract.unfreezeBalance.receiver_address);

            if (   !TW::Tron::Address::isValid(contract.unfreezeBalance.owner_address)
                || !TW::Tron::Address::isValid(contract.unfreezeBalance.receiver_address)
                ) {
                return JUBR_ARGUMENTS_BAD;
            }

            // When resource == bandwidth, pb will not encode this item,
            // so we need to override it's serialize()
            TW::Tron::UnfreezeBalanceContract value(
                contract.unfreezeBalance.owner_address,
                TW::Tron::toInternal(contract.unfreezeBalance.resource),
                contract.unfreezeBalance.receiver_address
            );
            parameter.set_type_url(TW::Tron::TransactionContract::to_parameter(value).type_url());
            auto vValue = value.serialize();
            parameter.set_value(&vValue[0], vValue.size());
        } break;
        case JUB_ENUM_TRX_CONTRACT_TYPE::CREATE_SMART_CONTRACT: {
            JUB_CHECK_NULL(contract.createSmart.owner_address);
            JUB_CHECK_NULL(contract.createSmart.bytecode);

            if (!TW::Tron::Address::isValid(contract.createSmart.owner_address)) {
                return JUBR_ARGUMENTS_BAD;
            }

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
        } break;
        case JUB_ENUM_TRX_CONTRACT_TYPE::TRIG_SMART_CONTRACT: {
            JUB_CHECK_NULL(contract.triggerSmart.owner_address);
            JUB_CHECK_NULL(contract.triggerSmart.contract_address);
            JUB_CHECK_NULL(contract.triggerSmart.data);

            if (   !TW::Tron::Address::isValid(contract.triggerSmart.owner_address)
//                || !TW::Tron::Address::isValid(contract.triggerSmart.contract_address)
                ) {
                return JUBR_ARGUMENTS_BAD;
            }

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
        } break;
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
