// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "RLP.h"
#include "Transaction.h"
#include "../Data.h"
#include "../Hash.h"
#include "../PrivateKey.h"
//#include "../uint256.h"
//#include "../proto/Ethereum.pb.h"

//#include <boost/multiprecision/cpp_int.hpp>
#include <cstdint>
#include <memory>
#include <vector>

namespace TW::Ethereum {

/// Helper class that performs Ethereum transaction signing.
class Signer {
//  public:
//    /// Signs a Proto::SigningInput transaction
//    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;
//    /// Signs a json Proto::SigningInput with private key
//    static std::string signJSON(const std::string& json, const Data& key);

  public:
    Signer() = delete;

    /// Signs the given transaction.
//    static Signature sign(const PrivateKey& privateKey, const uint256_t& chainID, std::shared_ptr<TransactionBase> transaction) noexcept;
    static Signature sign(const PrivateKey& privateKey, const Data& chainID, std::shared_ptr<TransactionBase> transaction) noexcept;
    // JuBiter-defined
    /// Verify the given signature of a transaction.
    static bool verify(const PublicKey& publicKey, const Data& chainID, std::shared_ptr<TransactionBase> transaction, const Data& signature) noexcept;

    // JuBiter-defined
    /// Signs the given bytestring (Eip191).
    static Signature sign(const PrivateKey& privateKey, const Data& bytestring) noexcept;
    // JuBiter-defined
    /// Verify the given signature of a bytestring (Eip191).
    static bool verify(const PublicKey& publicKey, const Data& bytestring, const Data& signature) noexcept;

    // JuBiter-defined
    /// Signs the given typedData (Eip712).
    static Signature sign(const PrivateKey& privateKey, const std::string& jsonData, const bool &bMetamaskV4Compat=true) noexcept;
    // JuBiter-defined
    /// Verify the given signature of a typedData (Eip712).
    static bool verify(const PublicKey& publicKey, const std::string& jsonData, const Data& signature, const bool &bMetamaskV4Compat=true) noexcept;

//    /// Compiles a Proto::SigningInput transaction, with external signature
//    static Proto::SigningOutput compile(const Proto::SigningInput& input, const Data& signature) noexcept;

  public:
//    /// build Transaction from signing input
//    static std::shared_ptr<TransactionBase> build(const Proto::SigningInput& input);

    /// Signs a hash with the given private key for the given chain identifier.
    ///
    /// @returns the r, s, and v values of the transaction signature
//    static Signature sign(const PrivateKey& privateKey, const Data& hash, bool includeEip155, const uint256_t& chainID) noexcept;
    static Signature sign(const PrivateKey& privateKey, const Data& hash, bool includeEip155, const Data& chainID) noexcept;
    // JuBiter-defined
    /// verify a hash with the given public key for the given chain identifier.
    ///
    /// @returns true if verify successed, otherwise return false.
//    static Signature sign(const PrivateKey& privateKey, const Data& hash, bool includeEip155, const uint256_t& chainID) noexcept;
    static bool verify(const PublicKey& publicKey, const Data& hash, bool includeEip155, const Data& chainID, const Data& signature) noexcept;

    /// Break up the signature into the R, S, and V values.
    /// @returns the r, s, and v values of the transaction signature
//    static Signature signatureDataToStruct(const Data& signature, bool includeEip155, const uint256_t& chainID) noexcept;
    static Signature signatureDataToStruct(const Data& signature, bool includeEip155, const Data& chainID) noexcept;
    // JuBiter-defined
    /// Break up the signature into Data..
    /// @returns the Data in r, s, and v values order of the transaction signature
    static Data structToSignatureData(Signature& signature, bool includeEip155, const Data& chainID) noexcept;

    /// Break up the signature into the R, S, and V values, with no replay protection.
    /// @returns the r, s, and v values of the transaction signature
    static Signature signatureDataToStructSimple(const Data& signature) noexcept;
    // JuBiter-defined
    /// Break up the signature into the R, S, and V values, with no replay protection.
    /// @returns the r, s, and v values of the transaction signature
    static Data structToSignatureDataSimple(const Signature& signature) noexcept;

    /// Break up the signature into the R, S, and V values, and include chainID in V for replay protection (Eip155)
    /// @returns the r, s, and v values of the transaction signature
//    static Signature signatureDataToStructWithEip155(const uint256_t& chainID, const Data& signature) noexcept;
    static Signature signatureDataToStructWithEip155(const Data& chainID, const Data& signature) noexcept;
    // JuBiter-defined
    /// Break up the signature into the Data in R, S, and V values order, and include chainID in V for replay protection (Eip155)
    /// @returns the Data in  r, s, and v values order of the transaction signature
    static Data structToSignatureDataWithEip155(const Data& chainID, const Signature& signature) noexcept;
};  // class Signer end

} // namespace TW::Ethereum
