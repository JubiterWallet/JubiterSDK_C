#pragma once

#include <vector>
#include <stdint.h>
#include <cstdint>
#include <string>
#include <uint256_t/uint256_t.h>
#include <uint256.h>

#include "ERC20Abi.h"

// https://eips.ethereum.org/EIPS/eip-721


/// @notice Count all NFTs assigned to an owner
/// @dev NFTs assigned to the zero address are considered invalid, and this
///  function throws for queries about the zero address.
/// @param _owner An address for whom to query the balance
/// @return The number of NFTs owned by `_owner`, possibly zero
#define ERC721_ABI_METHOD_ID_BALANCE_OF                 "70a08231"

/// @notice Find the owner of an NFT
/// @dev NFTs assigned to zero address are considered invalid, and queries
///  about them do throw.
/// @param _tokenId The identifier for an NFT
/// @return The address of the owner of the NFT
#define ERC721_ABI_METHOD_ID_OWNER_OF                   "6352211e"

/// @notice Transfers the ownership of an NFT from one address to another address
/// @dev Throws unless `msg.sender` is the current owner, an authorized
///  operator, or the approved address for this NFT. Throws if `_from` is
///  not the current owner. Throws if `_to` is the zero address. Throws if
///  `_tokenId` is not a valid NFT. When transfer is complete, this function
///  checks if `_to` is a smart contract (code size > 0). If so, it calls
///  `onERC721Received` on `_to` and throws if the return value is not
///  `bytes4(keccak256("onERC721Received(address,address,uint256,bytes)"))`.
/// @param _from The current owner of the NFT
/// @param _to The new owner
/// @param _tokenId The NFT to transfer
/// @param data Additional data with no specified format, sent in call to `_to`
#define ERC721_ABI_METHOD_ID_SAFE_TRANSFER_FROM_DATA    "b88d4fde"

/// @notice Transfers the ownership of an NFT from one address to another address
/// @dev This works identically to the other function with an extra data parameter,
///  except this function just sets data to "".
/// @param _from The current owner of the NFT
/// @param _to The new owner
/// @param _tokenId The NFT to transfer
#define ERC721_ABI_METHOD_ID_SAFE_TRANSFER_FROM         "42842e0e"

/// @dev This emits when ownership of any NFT changes by any mechanism.
///  This event emits when NFTs are created (`from` == 0) and destroyed
///  (`to` == 0). Exception: during contract creation, any number of NFTs
///  may be created and assigned without emitting Transfer. At the time of
///  any transfer, the approved address for that NFT (if any) is reset to none.
/// @param _from The current owner of the NFT
/// @param _to The new owner
/// @param _tokenId The NFT to transfer
#define ERC721_ABI_METHOD_ID_TRANSFER_FROM              "23b872dd"

/// @notice Change or reaffirm the approved address for an NFT
/// @dev The zero address indicates there is no approved address.
///  Throws unless `msg.sender` is the current NFT owner, or an authorized
///  operator of the current owner.
/// @param _approved The new approved NFT controller
/// @param _tokenId The NFT to approve
#define ERC721_ABI_METHOD_ID_APPROVE                    "095ea7b3"

/// @notice Enable or disable approval for a third party ("operator") to manage
///  all of `msg.sender`'s assets
/// @dev Emits the ApprovalForAll event. The contract MUST allow
///  multiple operators per owner.
/// @param _operator Address to add to the set of authorized operators
/// @param _approved True if the operator is approved, false to revoke approval
#define ERC721_ABI_METHOD_ID_SET_APPROVAL_FOR_ALL       "a22cb465"

/// @notice Get the approved address for a single NFT
/// @dev Throws if `_tokenId` is not a valid NFT.
/// @param _tokenId The NFT to find the approved address for
/// @return The approved address for this NFT, or the zero address if there is none
#define ERC721_ABI_METHOD_ID_GET_APPROVED               "081812fc"

/// @notice Query if an address is an authorized operator for another address
/// @param _owner The address that owns the NFTs
/// @param _operator The address that acts on behalf of the owner
/// @return True if `_operator` is an approved operator for `_owner`, false otherwise
#define ERC721_ABI_METHOD_ID_IS_APPROVED_FOR_ALL        "e985e9c5"


namespace jub {
namespace eth {

std::vector<uint8_t> toTwosComplement(const uint256_t& number);
std::vector<uint8_t> toTwosComplement(const std::string& number, const uint8_t base);

class ERC721Abi {
public:
    virtual ~ERC721Abi();

    static std::vector<uint8_t> serialize(const std::vector<uint8_t>& from, const std::vector<uint8_t>& to, const std::string& tokenID, const std::string methodID=ERC721_ABI_METHOD_ID_TRANSFER_FROM);
    static std::vector<uint8_t> serialize(const std::vector<uint8_t>& from, const std::vector<uint8_t>& to, const std::string& tokenID, const std::vector<uint8_t>& data, const std::string methodID=ERC721_ABI_METHOD_ID_TRANSFER_FROM);
}; // class ERC721Abi end


} // namespace eth end
} // namespace jub end
