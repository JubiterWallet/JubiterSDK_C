#pragma once

#include <vector>
#include <stdint.h>
#include <cstdint>

// Allowance is a free data retrieval call binding the contract method 0xdd62ed3e.
// Solidity: function allowance( address,  address) constant returns(uint256)
#define ERC20_ABI_METHOD_ID_ALLOWANCE           "dd62ed3e"

// BalanceOf is a free data retrieval call binding the contract method 0x70a08231.
// Solidity: function balanceOf( address) constant returns(uint256)
#define ERC20_ABI_METHOD_ID_BALANCE_OF          "70a08231"//    *

// Decimals is a free data retrieval call binding the contract method 0x313ce567.
// Solidity: function decimals() constant returns(uint8)
#define ERC20_ABI_METHOD_ID_DECIMALS            "313ce567"

// Name is a free data retrieval call binding the contract method 0x06fdde03.
// Solidity: function name() constant returns(string)
#define ERC20_ABI_METHOD_ID_NAME                "06fdde03"

// SpentAllowance is a free data retrieval call binding the contract method 0xdc3080f2.
// Solidity: function spentAllowance( address,  address) constant returns(uint256)
#define ERC20_ABI_METHOD_ID_SPENT_ALLOWANCE     "dc3080f2"

// Symbol is a free data retrieval call binding the contract method 0x95d89b41.
// Solidity: function symbol() constant returns(string)
#define ERC20_ABI_METHOD_ID_SYMBOL              "95d89b41"

// ApproveAndCall is a paid mutator transaction binding the contract method 0xcae9ca51.
// Solidity: function approveAndCall(_spender address, _value uint256, _extraData bytes) returns(success bool)
#define ERC20_ABI_METHOD_ID_APPROVE_AND_CALL    "cae9ca51"

// Transfer is a paid mutator transaction binding the contract method 0xa9059cbb.
// Solidity: function transfer(_to address, _value uint256) returns()
#define ERC20_ABI_METHOD_ID_TRANSFER            "a9059cbb"//    *

// TransferFrom is a paid mutator transaction binding the contract method 0x23b872dd.
// Solidity: function transferFrom(_from address, _to address, _value uint256) returns(success bool)
#define ERC20_ABI_METHOD_ID_TRANSFER_FROM       "23b872dd"

namespace jub {
namespace eth {


const size_t kETHEREUM_SIZE_VARIABLE_FUNCTION_CONTRACT = 32;
const size_t kETH_METHOD_HASH_SIZE = 4;
typedef std::array<uint8_t, kETH_METHOD_HASH_SIZE> EthereumContractMethodHash;


struct EthereumContractPayloadStream {
public:
    EthereumContractPayloadStream()
        : m_data() {
        m_data.reserve(256);
    }

    void write_data(const void* data, size_t size) {
        const uint8_t* d = reinterpret_cast<const uint8_t*>(data);
        m_data.insert(m_data.end(), d, d + size);
    }

    std::vector<std::uint8_t> get_data() {
        return m_data;
    }

    ~EthereumContractPayloadStream() {
    }

protected:
    std::vector<std::uint8_t> m_data;
}; // struct EthereumContractPayloadStream end


EthereumContractPayloadStream& operator<<(EthereumContractPayloadStream& stream, const std::vector<std::uint8_t>& data);
EthereumContractPayloadStream& operator<<(EthereumContractPayloadStream& stream, const EthereumContractMethodHash& method_hash);


class ERC20Abi {
public:
    virtual ~ERC20Abi();

    static std::vector<uint8_t> serialize(const std::vector<uint8_t>& address, const std::vector<uint8_t>& value);
}; // class ERC20Abi end


} // namespace eth end
} // namespace jub end
