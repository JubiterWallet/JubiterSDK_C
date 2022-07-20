#pragma once

#include <vector>
#include <map>
#include <stdint.h>
#include <cstdint>
#include <string>
#include <TrezorCrypto/sha2.h>
#include <TrezorCrypto/sha3.h>
#include <nlohmann/json.hpp>

// https://eips.ethereum.org/EIPS/eip-712

namespace jub {
namespace eth {

typedef enum _EthereumDataType {
    EthereumDataType_UINT   = 1,
    EthereumDataType_INT    = 2,
    EthereumDataType_BYTES  = 3,    // handles 'bytes'
    EthereumDataType_STRING = 4,
    EthereumDataType_BOOL   = 5,
    EthereumDataType_ADDRESS = 6,
    EthereumDataType_ARRAY  = 7,
    EthereumDataType_STRUCT = 8,
    EthereumDataType_BYTESX = 9     // handles 'bytes0' ~ 'bytes32'
} EthereumDataType;

struct EIP712 {
public:
    virtual ~EIP712();

    static bool parseJSON(const nlohmann::json& root);
    static bool clearJSON();

    static bool hash(const unsigned char* d, size_t d_len, unsigned char hash_type[sha3_256_hash_size]);
    static std::vector<uint8_t> hash(const unsigned char* d, size_t d_len);
    static std::vector<uint8_t> hash(const std::string& d);
    static std::vector<uint8_t> hash(const std::vector<uint8_t>& d);

//    static std::string typed_data_struct_envelope(const std::string& st_name, const nlohmann::json& st);
    static std::string typed_data_struct_envelope(const std::string& st_name);

    static bool is_atomic_type(const std::string& type_name);

    static std::vector<uint8_t> encode_atomic_type_data(EthereumDataType type, void* value, const size_t value_len);

    static std::vector<uint8_t> atomic_typed_data_envelope(const std::string& type_name, const nlohmann::json& json_value);

//    static std::vector<uint8_t> struct_type_hash(const std::string& st_name, const nlohmann::json& st);
    static std::vector<uint8_t> struct_type_hash(const std::string& st_name);

    static std::vector<uint8_t> typed_data_envelope(const std::string& st_name, const nlohmann::json& json_value, const bool is_metamask_v4_compat=true);
}; // class EIP712 end

//std::map<std::string, nlohmann::json> EIP712::mapType = {};
//int EIP712::objectCount = 0;

} // namespace eth end
} // namespace jub end
