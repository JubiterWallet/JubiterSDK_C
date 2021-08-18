#pragma once

#include <vector>
#include <stdint.h>
#include <cstdint>
#include <string>

#include "ERC721Abi.h"

// https://eips.ethereum.org/EIPS/eip-1155


/// @notice Transfers `_value` amount of an `_id` from the `_from` address to the `_to` address specified (with safety call).
/// @dev Caller must be approved to manage the tokens being transferred out of the `_from` account (see "Approval" section of the standard).
/// MUST revert if `_to` is the zero address.
/// MUST revert if balance of holder for token `_id` is lower than the `_value` sent.
/// MUST revert on any other error.
/// MUST emit the `TransferSingle` event to reflect the balance change (see "Safe Transfer Rules" section of the standard).
/// After the above conditions are met, this function MUST check if `_to` is a smart contract (e.g. code size > 0). If so, it MUST call `onERC1155Received` on `_to` and act appropriately (see "Safe Transfer Rules" section of the standard).
/// @param _from    Source address
/// @param _to      Target address
/// @param _id      ID of the token type
/// @param _value   Transfer amount
/// @param _data    Additional data with no specified format, MUST be sent unaltered in call to `onERC1155Received` on `_to`
#define ERC1155_ABI_METHOD_ID_SAFE_TRANSFER_FROM            "f242432a"

/// @notice Transfers `_values` amount(s) of `_ids` from the `_from` address to the `_to` address specified (with safety call).
/// @dev Caller must be approved to manage the tokens being transferred out of the `_from` account (see "Approval" section of the standard).
/// MUST revert if `_to` is the zero address.
/// MUST revert if length of `_ids` is not the same as length of `_values`.
/// MUST revert if any of the balance(s) of the holder(s) for token(s) in `_ids` is lower than the respective amount(s) in `_values` sent to the recipient.
/// MUST revert on any other error.
/// MUST emit `TransferSingle` or `TransferBatch` event(s) such that all the balance changes are reflected (see "Safe Transfer Rules" section of the standard).
/// Balance changes and events MUST follow the ordering of the arrays (_ids[0]/_values[0] before _ids[1]/_values[1], etc).
/// After the above conditions for the transfer(s) in the batch are met, this function MUST check if `_to` is a smart contract (e.g. code size > 0). If so, it MUST call the relevant `ERC1155TokenReceiver` hook(s) on `_to` and act appropriately (see "Safe Transfer Rules" section of the standard).
/// @param _from    Source address
/// @param _to      Target address
/// @param _ids     IDs of each token type (order and length must match _values array)
/// @param _values  Transfer amounts per token type (order and length must match _ids array)
/// @param _data    Additional data with no specified format, MUST be sent unaltered in call to the `ERC1155TokenReceiver` hook(s) on `_to`
#define ERC1155_ABI_METHOD_ID_SAFE_BATCH_TRANSFER_FROM      "2eb2c2d6"

/// @notice Get the balance of an account's tokens.
/// @param _owner  The address of the token holder
/// @param _id         ID of the token
/// @return       The _owner's balance of the token type requested
#define ERC1155_ABI_METHOD_ID_BALANCE_OF                    "00fdd58e"

/// @notice Get the balance of multiple account/token pairs
/// @param _owners The addresses of the token holders
/// @param _ids       ID of the tokens
/// @return       The _owner's balance of the token types requested (i.e. balance for each (owner, id) pair)
#define ERC1155_ABI_METHOD_ID_BALANCE_OF_BATCH              "4e1273f4"

/// @notice Enable or disable approval for a third party ("operator") to manage all of the caller's tokens.
/// @dev MUST emit the ApprovalForAll event on success.
/// @param _operator  Address to add to the set of authorized operators
/// @param _approved  True if the operator is approved, false to revoke approval
#define ERC1155_ABI_METHOD_ID_SET_APPROVAL_FOR_ALL          "a22cb465"

/// @notice Queries the approval status of an operator for a given owner.
/// @param _owner     The owner of the tokens
/// @param _operator  Address of authorized operator
/// @return           True if the operator is approved, false if not
#define ERC1155_ABI_METHOD_ID_IS_APPROVED_FOR_ALL           "e985e9c5"


namespace jub {
namespace eth {


class ERC1155Abi {
public:
    virtual ~ERC1155Abi();

    static std::vector<uint8_t> serialize(const std::vector<uint8_t>& from, const std::vector<uint8_t>& to,
                                          const std::string& tokenID, const std::string& value,
                                          const std::string methodID=ERC1155_ABI_METHOD_ID_SAFE_TRANSFER_FROM);
    static std::vector<uint8_t> serialize(const std::vector<uint8_t>& from, const std::vector<uint8_t>& to,
                                          const std::string& tokenID, const std::string& value, const std::vector<uint8_t>& data,
                                          const std::string methodID=ERC1155_ABI_METHOD_ID_SAFE_TRANSFER_FROM);

    static std::vector<uint8_t> serialize(const std::vector<uint8_t>& from, const std::vector<uint8_t>& to,
                                          const std::vector<std::string>& tokenIDs, const std::vector<std::string>& values,
                                          const std::string methodID=ERC1155_ABI_METHOD_ID_SAFE_BATCH_TRANSFER_FROM);
    static std::vector<uint8_t> serialize(const std::vector<uint8_t>& from, const std::vector<uint8_t>& to,
                                          const std::vector<std::string>& tokenIDs, const std::vector<std::string>& values,
                                          const std::vector<uint8_t>& data,
                                          const std::string methodID=ERC1155_ABI_METHOD_ID_SAFE_BATCH_TRANSFER_FROM);
}; // class ERC1155Abi end


} // namespace eth end
} // namespace jub end
