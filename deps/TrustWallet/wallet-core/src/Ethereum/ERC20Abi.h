#pragma once

#include <vector>
#include <stdint.h>

// Allowance is a free data retrieval call binding the contract method 0xdd62ed3e.
// Solidity: function allowance( address,  address) constant returns(uint256)
#define ABI_METHOD_ID_ALLOWANCE         "0xdd62ed3e"

// BalanceOf is a free data retrieval call binding the contract method 0x70a08231.
// Solidity: function balanceOf( address) constant returns(uint256)
#define ABI_METHOD_ID_BALANCE_OF        "0x70a08231"//    *

// Decimals is a free data retrieval call binding the contract method 0x313ce567.
// Solidity: function decimals() constant returns(uint8)
#define ABI_METHOD_ID_DECIMALS          "0x313ce567"

// Name is a free data retrieval call binding the contract method 0x06fdde03.
// Solidity: function name() constant returns(string)
#define ABI_METHOD_ID_NAME              "0x06fdde03"

// SpentAllowance is a free data retrieval call binding the contract method 0xdc3080f2.
// Solidity: function spentAllowance( address,  address) constant returns(uint256)
#define ABI_METHOD_ID_SPENT_ALLOWANCE   "0xdc3080f2"

// Symbol is a free data retrieval call binding the contract method 0x95d89b41.
// Solidity: function symbol() constant returns(string)
#define ABI_METHOD_ID_SYMBOL            "0x95d89b41"

// ApproveAndCall is a paid mutator transaction binding the contract method 0xcae9ca51.
// Solidity: function approveAndCall(_spender address, _value uint256, _extraData bytes) returns(success bool)
#define ABI_METHOD_ID_APPROVE_AND_CALL  "0xcae9ca51"

// Transfer is a paid mutator transaction binding the contract method 0xa9059cbb.
// Solidity: function transfer(_to address, _value uint256) returns()
#define ABI_METHOD_ID_TRANSFER          "0xa9059cbb"//    *

// TransferFrom is a paid mutator transaction binding the contract method 0x23b872dd.
// Solidity: function transferFrom(_from address, _to address, _value uint256) returns(success bool)
#define ABI_METHOD_ID_TRANSFER_FROM     "0x23b872dd"

namespace jub {
namespace eth {


class ERC20Abi {
public:
    virtual ~ERC20Abi();

    static std::vector<uint8_t> serialize(const std::vector<uint8_t>& address, const std::vector<uint8_t>& value);
}; // class ERC20Abi end


} // namespace eth end
} // namespace jub end
