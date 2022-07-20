// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Address.h"
#include <string>
#include <vector>

//#include "../uint256.h"

namespace TW::Ethereum {

// Transactions can be:
// - Non-typed (legacy, pre-EIP2718) transactions:
//  -- simple ETH transfer
//  -- others with payload, function call, e.g. ERC20 / ERC721 transfer
// - Typed transactions (enveloped, EIP2718), with specific type and transaction payload
// - Personal transaction (enveloped, EIP191)
// - TypedData transaction (enveloped, EIP712)

struct Access {
    Address address;
    std::vector<Data> storageKeys;
}; // struct Access end

std::vector<Access> accessListFromJson(const std::string &json);

/// R-S-V Signature values
struct Signature {
  public:
    //    uint256_t r, s, v;
    Data r, s, v;

    // JuBiter-defined
    bool empty() const { return (r.empty() && s.empty() && v.empty()); }

    // JuBiter-defined
    Data serialize() const;

    // JuBiter-defined
    bool isValid() const {
        if (0 == r.size() || 0 == s.size() || 0 == v.size()) {
            return false;
        }

        return true;
    }
}; // struct Signature end

/// Base class for all transactions.
/// Non-typed and various typed transactions derive from this.
class TransactionBase {
  public:
    //    uint256_t nonce;
    Data nonce;
    Data payload;

  public:
    //    TransactionBase(const uint256_t& nonce, const Data& payload): nonce(nonce), payload(payload) {}
    TransactionBase(const Data &nonce, const Data &payload) : nonce(nonce), payload(payload) {}
    virtual ~TransactionBase() {
        nonce.clear();
        payload.clear();
    }
    // JuBiter-modified
    // pre-sign hash of the tx, for signing
    //    virtual Data preHash(const uint256_t chainID) const = 0;
    virtual Data preHash(const Data &chainID) const;
    // pre-sign image of tx
    //    virtual Data serialize(const uint256_t chainID) const = 0;
    virtual Data serialize(const Data &chainID) const = 0;
    // JuBiter-defined
    virtual Data encoded(const Signature &signature) const;

  protected:
    // JuBiter-modified
    // encoded tx (signed)
    //    virtual Data encoded(const Signature& signature, const uint256_t chainID) const = 0;
    virtual Data encoded(const Signature &signature, const Data &chainID) const;

  public:
    // JuBiter-defined
    virtual bool decoded(const Data &encoded, Signature &signature);
    // Signals wether this tx type uses Eip155-style replay protection in the signature
    virtual bool usesReplayProtection() const = 0;

  protected:
    TransactionBase() {}
}; // class TransactionBase end

/// Original transaction format, with no explicit type, legacy as pre-EIP2718
class TransactionNonTyped : public TransactionBase {
  public:
    //    uint256_t gasPrice;
    //    uint256_t gasLimit;
    Data gasPrice;
    Data gasLimit;
    Address to;
    //    uint256_t amount;
    Data amount;

    //    virtual Data preHash(const uint256_t chainID) const;
    //    virtual Data serialize(const uint256_t chainID) const;
    //    virtual Data encoded(const Signature& signature, const uint256_t chainID) const;
    virtual Data serialize(const Data &chainID) const override;
    virtual Data encoded(const Signature &signature, const Data &chainID) const override;
    // JuBiter-defined
    virtual bool decoded(const Data &encoded, Signature &signature) override;
    virtual bool usesReplayProtection() const override { return true; }

  public:
    // JuBiter-defined
    TransactionNonTyped() {}
    //    TransactionNonTyped(const uint256_t& nonce,
    //        const uint256_t& gasPrice, const uint256_t& gasLimit,
    //        const Data& to, const uint256_t& amount, const Data& payload = {})
    TransactionNonTyped(const Data &nonce, const Data &gasPrice, const Data &gasLimit, const Address &to,
                        const Data &amount, const Data &payload = {})
        : TransactionBase(nonce, payload), gasPrice(std::move(gasPrice)), gasLimit(std::move(gasLimit)),
          to(std::move(to)), amount(std::move(amount)) {}
    virtual ~TransactionNonTyped() {
        gasPrice.clear();
        gasLimit.clear();
        amount.clear();
    }

    // JuBiter-defined
    bool isValid() const {
        if (0 == nonce.size() || 0 == gasPrice.size() || 0 == gasLimit.size() || !Address::isValid(to.string())
            //            || 0 == amount.size()
            //            || 0 == payload.size()
        ) {
            return false;
        }

        return true;
    }
}; // class TransactionNonTyped end

// JuBiter-defined
/// Original transaction format, with no explicit type, legacy as pre-EIP191
class TransactionPersonal : public TransactionBase {
  public:
    // Reuse base class payload as data to be signed (byteString)
    TransactionPersonal(const Data &bytestring) : TransactionBase({}, bytestring) {}

    virtual Data preHash() const;

  protected:
    virtual Data serialize(const Data &chainID) const override;

  public:
    virtual Data serialize() const;
    virtual bool decoded(const Data &encoded, Signature &signature) override;
    virtual bool usesReplayProtection() const override { return true; }
}; // class TransactionPersonal end

// JuBiter-defined
/// Original transaction format, with no explicit type, legacy as pre-EIP712
class TransactionTypedData : public TransactionBase {
  public:
    bool bMetamaskV4Compat;
    // Reuse base class payload as data in JSON to be signed (JSON data)
    TransactionTypedData(const std::string &dataInJSON, const bool bMetamaskV4Compat = true)
        : bMetamaskV4Compat(bMetamaskV4Compat)
        , TransactionBase({}, Data(dataInJSON.begin(), dataInJSON.end())) {}

    virtual Data preHash() const;

  protected:
    virtual Data serialize(const Data &chainID) const override;

  public:
    virtual Data serialize() const;
    virtual bool decoded(const Data &encoded, Signature &signature) override;
    virtual bool usesReplayProtection() const override { return true; }
}; // class TransactionTypedData end

enum TransactionType : uint8_t {
    TxType_OptionalAccessList = 0x01,
    TxType_Eip1559 = 0x02,
}; // enum TransactionType end

/// Base class for various typed transactions.
class TransactionTyped : public TransactionBase {
  public:
    // transaction type
    TransactionType type;
    // JuBiter-defined
    std::vector<Access> accessList;

    // JuBiter-defined
    TransactionTyped() {}
    // JuBiter-modified
    //    TransactionTyped(TransactionType type, const uint256_t& nonce, const Data& payload)
    TransactionTyped(TransactionType type, const Data &nonce, const Data &payload,
                     const std::string &accessListInJSON = "")
        : TransactionBase(nonce, payload), type(type), accessList(accessListFromJson(accessListInJSON)) {}
    virtual ~TransactionTyped() {
        accessList.clear();
    }

    virtual bool usesReplayProtection() const override { return false; }

  protected:
    // JuBiter-defined
    virtual Data serializePart(const Data &chainID) const = 0;
    // JuBiter-defined
    virtual Data encodeAccessList() const;
    // JuBiter-defined
    virtual bool decodeAccessList(const Data &chainID, const Signature &signature, const Data &encoded);

  public:
    // JuBiter-defined
    bool isValid() const {
        if (!(TxType_OptionalAccessList == type || TxType_Eip1559 == type)) {
            return false;
        }

        return true;
    }
}; // class TransactionTyped end

// JuBiter-defined
/// Original transaction format, with no explicit type, legacy as pre-EIP2930
class TransactionOptionalAccessList : public TransactionTyped {
  public:
    const size_t ITEM_COUNT = 11;
    const size_t ITEM_ACCESS_LIST_INDEX = 7;
    //    uint256_t gasPrice;
    //    uint256_t gasLimit;
    Data gasPrice;
    Data gasLimit;
    Address to;
    //    uint256_t amount;
    Data amount;

    //    virtual Data preHash(const uint256_t chainID) const;
    //    virtual Data serialize(const uint256_t chainID) const;
    //    virtual Data encoded(const Signature& signature, const uint256_t chainID) const;
  protected:
    virtual Data serializePart(const Data &chainID) const override;

  public:
    virtual Data serialize(const Data &chainID) const override;
    virtual Data encoded(const Signature &signature, const Data &chainID) const override;
    virtual bool decoded(const Data &encoded, Signature &signature) override;

  public:
    TransactionOptionalAccessList() {}
    TransactionOptionalAccessList(const Data &nonce, const Data &gasPrice, const Data &gasLimit, const Address &to,
                                  const Data &amount, const Data &payload = {},
                                  const std::string &accessListInJSON = "")
        : TransactionTyped(TxType_OptionalAccessList, nonce, payload, accessListInJSON), gasPrice(std::move(gasPrice)),
          gasLimit(std::move(gasLimit)), to(std::move(to)), amount(std::move(amount)) {}
    virtual ~TransactionOptionalAccessList() {
        gasPrice.clear();
        gasLimit.clear();
        amount.clear();
    }

    bool isValid() const {
        if (0 == nonce.size() || 0 == gasPrice.size() || 0 == gasLimit.size() || !Address::isValid(to.string())
            //            || 0 == amount.size()
            //            || 0 == payload.size()
        ) {
            return false;
        }

        return true;
    }
}; // class TransactionOptionalAccessList end

/// EIP1559 transaction
class TransactionEip1559 : public TransactionTyped {
  public:
    const size_t ITEM_COUNT = 12;
    const size_t ITEM_ACCESS_LIST_INDEX = 8;
    //    uint256_t maxInclusionFeePerGas;
    //    uint256_t maxFeePerGas;
    //    uint256_t gasLimit;
    Data maxInclusionFeePerGas;
    Data maxFeePerGas;
    Data gasLimit;
    // Public key hash (Address.bytes)
    //    Data to;
    Address to;
    //    uint256_t amount;
    Data amount;

  protected:
    virtual Data serializePart(const Data &chainID) const override;

  public:
    virtual Data serialize(const Data &chainID) const override;
    virtual Data encoded(const Signature &signature, const Data &chainID) const override;
    // JuBiter-defined
    virtual bool decoded(const Data &encoded, Signature &signature) override;

  public:
    // JuBiter-defined
    TransactionEip1559() {}
    // JuBiter-modified
    //    TransactionEip1559(const uint256_t& nonce,
    //        const uint256_t& maxInclusionFeePerGas, const uint256_t& maxFeePerGas, const uint256_t& gasLimit,
    //        const Data& to, const uint256_t& amount, const Data& payload = {})
    TransactionEip1559(const Data &nonce, const Data &maxInclusionFeePerGas, const Data &maxFeePerGas,
                       const Data &gasLimit, const Address &to, const Data &amount, const Data &payload = {},
                       const std::string &accessListInJSON = "")
        : TransactionTyped(TxType_Eip1559, nonce, payload, accessListInJSON),
          maxInclusionFeePerGas(std::move(maxInclusionFeePerGas)), maxFeePerGas(std::move(maxFeePerGas)),
          gasLimit(std::move(gasLimit)), to(std::move(to)), amount(std::move(amount)) {}
    virtual ~TransactionEip1559() {
        maxInclusionFeePerGas.clear();
        maxFeePerGas.clear();
        gasLimit.clear();
        amount.clear();
    }

    // JuBiter-defined
    bool isValid() const {
        if (0 == nonce.size() || 0 == maxInclusionFeePerGas.size() || 0 == maxFeePerGas.size() ||
            0 == gasLimit.size() || !Address::isValid(to.string())
            //            || 0 == amount.size()
            //            || 0 == payload.size()
        ) {
            return false;
        }

        return TransactionTyped::isValid();
    }
}; // class TransactionEip1559 end

} // namespace TW::Ethereum
