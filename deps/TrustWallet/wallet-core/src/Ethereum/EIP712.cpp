#include "EIP712.h"
#include "utility/util.h"
#include <list>

namespace jub {
namespace eth {

namespace {
    static std::map<std::string, nlohmann::json> mapType;
}

bool EIP712::parseJSON(const nlohmann::json& root) {

    if (   root["domain"].empty()
        || root["primaryType"].empty()
        || root["message"].empty()
        || root["types"].empty()
        ) {
        return false;
    }

    if (   nlohmann::detail::value_t::object != root["domain"].type()
        || nlohmann::detail::value_t::string != root["primaryType"].type()
        || nlohmann::detail::value_t::object != root["message"].type()
        || nlohmann::detail::value_t::object != root["types"].type()
        ) {
        return false;
    }

    for (auto it=root["types"].begin(); it != root["types"].end(); ++it) {
        mapType.insert(std::make_pair(it.key(), it.value()));
    }

    return true;
}


bool EIP712::hash(const unsigned char* d, size_t d_len, unsigned char hash_type[sha3_256_hash_size]) {

    keccak_256(d, d_len, hash_type);

    return true;
}


std::vector<uint8_t> EIP712::hash(const unsigned char* d, size_t d_len) {

    unsigned char digest[sha3_256_hash_size] = {0x00,};
    if (!hash(d, d_len, digest)) {
        return {};
    }

    return {digest, digest + sha3_256_hash_size};
}


std::vector<uint8_t> EIP712::hash(const std::string& d) {

    return hash((unsigned char*)d.c_str(), d.size());
}


std::vector<uint8_t> EIP712::hash(const std::vector<uint8_t>& d) {

    unsigned char *p = new unsigned char[d.size()+1];
    memset(p, 0x00, d.size()+1);
    for(size_t i=0; i<d.size(); ++i) {
        p[i] = d[i];
    }

    std::vector<uint8_t> h = hash(p, d.size());
    delete [] p; p = NULL;
    if (0 == h.size()) {
        return {};
    }
    return h;
}


//std::string EIP712::typed_data_struct_envelope(const std::string& st_name, const nlohmann::json& st) {
//
//    if (nlohmann::detail::value_t::array != st.type()) {
//        return "";
//    }
//
//    std::string encode_type = st_name;
//    encode_type += '(';
//
//    for (int i=0; i<st.size(); ++i) {
//        encode_type += st[i]["type"];
//        encode_type += ' ';
//        encode_type += st[i]["name"];
//        if (i < st.size()-1)
//            encode_type += ',';
//    }
//    encode_type += ')';
//
//    return encode_type;
//}
//
//
std::string EIP712::typed_data_struct_envelope(const std::string& st_name) {

    if (nlohmann::detail::value_t::array != mapType[st_name].type()) {
        return "";
    }

    //Mail(Person from,Person[] to,string contents)Person(string name,address wallet)
    std::string encode_type = st_name;
    encode_type += '(';

    std::list<std::string> subTypes;
    for (int i=0; i<mapType[st_name].size(); ++i) {
        std::string sub_type = "";
        auto type_name = mapType[st_name][i]["type"].get<std::string>();
        if (!is_atomic_type(type_name)) {
            auto typed_data_envelope = typed_data_struct_envelope(type_name.c_str());
            auto it = std::find(subTypes.begin(), subTypes.end(), typed_data_envelope);
            if (it == subTypes.end()) {
                subTypes.push_back(typed_data_envelope);
            }
        }

        encode_type += mapType[st_name][i]["type"];
        encode_type += ' ';
        encode_type += mapType[st_name][i]["name"];
        if (i < mapType[st_name].size()-1)
            encode_type += ',';
    }
    encode_type += ')';

    for (const auto& item:subTypes) {
        encode_type += item;
    }

    return encode_type;
}


bool EIP712::is_atomic_type(const std::string& type_name) {
    return (0 == std::memcmp(type_name.c_str(), "uint256",   std::string("uint256").size())
        || 0 == std::memcmp(type_name.c_str(), "int256",    std::string("int256").size())
        || 0 == type_name.rfind("bytes", 0)
        || 0 == std::memcmp(type_name.c_str(), "string",    std::string("string").size())
        || 0 == std::memcmp(type_name.c_str(), "bool",      std::string("bool").size())
        || 0 == std::memcmp(type_name.c_str(), "address",   std::string("address").size())
        || 0 == std::memcmp(type_name.c_str(), "array",     std::string("array").size())
        || 0 == std::memcmp(type_name.c_str(), "struct",    std::string("struct").size()));
}

EthereumDataType type_name_2_type(const std::string& type_name) {
#define is_type_t(t, type) t == type
    EthereumDataType type = EthereumDataType::EthereumDataType_STRUCT;
    if (is_type_t(type_name, "uint256")) {
        type = EthereumDataType::EthereumDataType_UINT;
    }
    else if (is_type_t(type_name, "int256")) {
        type = EthereumDataType::EthereumDataType_INT;
    }
    else if (0 == type_name.rfind("bytes", 0)) {
        type = EthereumDataType::EthereumDataType_BYTES;
    }
    else if (is_type_t(type_name, "string")) {
        type = EthereumDataType::EthereumDataType_STRING;
    }
    else if (is_type_t(type_name, "bool")) {
        type = EthereumDataType::EthereumDataType_BOOL;
    }
    else if (is_type_t(type_name, "address")) {
        type = EthereumDataType::EthereumDataType_ADDRESS;
    }
    else if (is_type_t(type_name, "array")) {
        type = EthereumDataType::EthereumDataType_ARRAY;
    }
    else if (is_type_t(type_name, "struct")) {
        type = EthereumDataType::EthereumDataType_STRUCT;
    }

    return type;
}

std::vector<uint8_t> EIP712::atomic_typed_data_envelope(const std::string& type_name, const nlohmann::json& json_value){
    uchar_vector encode_atomic;
    switch (type_name_2_type(type_name)) {
        case EthereumDataType::EthereumDataType_UINT: {
            int uinteger = json_value.get<int>();
            encode_atomic += encode_atomic_type_data(
                                type_name,
                                (void*)(&uinteger), 1);
        } break;
        case EthereumDataType::EthereumDataType_INT: {
            int integer = json_value.get<int>();
            encode_atomic += encode_atomic_type_data(
                                type_name,
                                (void*)(&integer), 1);
        } break;
        case EthereumDataType::EthereumDataType_BYTES: {
            auto v = json_value.get<std::string>();
            encode_atomic += encode_atomic_type_data(
                                type_name,
                                (void*)v.data(),
                                v.size());
        } break;
        case EthereumDataType::EthereumDataType_STRING: {
            auto v = json_value.get<std::string>();
            encode_atomic += encode_atomic_type_data(
                                type_name,
                                (void*)v.data(),
                                v.size());
        } break;
        case EthereumDataType::EthereumDataType_BOOL: {
            auto v = json_value.get<std::string>();
            encode_atomic += encode_atomic_type_data(
                                type_name,
                                (void*)v.data(),
                                v.size());
        } break;
        case EthereumDataType::EthereumDataType_ADDRESS: {
            auto v = json_value.get<std::string>();
            encode_atomic += encode_atomic_type_data(
                                type_name,
                                (void*)v.data(),
                                v.size());
        } break;
        case EthereumDataType::EthereumDataType_ARRAY:
        case EthereumDataType::EthereumDataType_STRUCT:
        default: {
            return {};
        } break;
    }

    return std::move(encode_atomic);
}

std::vector<uint8_t> EIP712::encode_atomic_type_data(const std::string& type_name, void* value, const size_t value_len) {

//    size_t enc_len = sha3_256_hash_size;
    if (NULL == value
        || 0 == value_len
        ) {
        return {};
    }

    unsigned char *p = NULL;
    size_t index = 0;

    unsigned char encode_atomic[sha3_256_hash_size];
    memset(encode_atomic, 0x00, sha3_256_hash_size);

    switch (type_name_2_type(type_name)) {
    case EthereumDataType_UINT: {
        uchar_vector vUint(bigUnsignedToString(BigUnsigned(decode_unsigned_big((uint8_t*)value, (int)value_len)), 16));
        index = sha3_256_hash_size-vUint.size();
        for (size_t i=index; i<sha3_256_hash_size; ++i) {
            encode_atomic[i] = vUint[index-i];
        }
    } break;
    case EthereumDataType_INT: {
        uchar_vector vInt(bigIntegerToString(BigInteger(decode_signed_big((uint8_t*)value, (int)value_len))));
        index = sha3_256_hash_size-vInt.size();
        for (size_t i=index; i<sha3_256_hash_size; ++i) {
            encode_atomic[i] = vInt[index-i];
        }
    } break;
    case EthereumDataType_BYTES: {
        index = 0;
        std::string str = std::string((char*)value, value_len);
        if (   NULL != strstr((char*)value, "0x")
            || NULL != strstr((char*)value, "0X")
        ) {
            index += 2;
        }
        p = new unsigned char[value_len];
        memset(p, 0x00, value_len);
        memcpy(p, ((unsigned char*)value+index), (value_len-index));
        uchar_vector vBytes(std::string((char*)p, (value_len-index)));

        // Support bytes0~bytes32, that is 'bytes', 'bytes1'..'bytes32'.
        // When the type is 'bytes', the encoded length depends on the length of the v;
        // When the type is 'bytes1'..'bytes32', the encoded length depends on the suffix.
        size_t pos = 0;
        std::string suffix = type_name.substr(pos+strlen("bytes"), type_name.size());
        size_t sz = ("" == suffix) ? 0 : std::stoi(suffix);
        if (sz) {
            // zero-padded at the end to bytes
            memset(encode_atomic, 0x00, sha3_256_hash_size);
            for (size_t i=0; i<vBytes.size(); ++i) {
                encode_atomic[i] = vBytes[i];
            }
        }
        else {
            if (NULL != p) {
                delete [] p; p = NULL;
            }
            return vBytes;
        }
    } break;
    case EthereumDataType_STRING: {
        hash((unsigned char*)value, value_len, encode_atomic);
    } break;
    case EthereumDataType_BOOL: {
        if (true == *((bool*)value)) {
            encode_atomic[sha3_256_hash_size - 1] = 1;
        }
    } break;
    case EthereumDataType_ADDRESS: {
        index = 0;
        std::string str = std::string((char*)value, value_len);
        if (   NULL != strstr((char*)value, "0x")
            || NULL != strstr((char*)value, "0X")
        ) {
            index += 2;
        }
        p = new unsigned char[value_len];
        memset(p, 0x00, value_len);
        memcpy(p, ((unsigned char*)value+index), (value_len-index));
        // zero-padded at the start to address
        memset(encode_atomic, 0x00, sha3_256_hash_size);
        uchar_vector vAddr(std::string((char*)p, (value_len-index)));
        index = sha3_256_hash_size-vAddr.size();
        for (size_t i=index; i<sha3_256_hash_size; ++i) {
            encode_atomic[i] = vAddr[i-index];
        }
    } break;
    case EthereumDataType_ARRAY:
    case EthereumDataType_STRUCT:
    default:
        return {};
    }

    if (NULL != p) {
        delete [] p; p = NULL;
    }

    return uchar_vector(encode_atomic, sha3_256_hash_size);
}

//std::vector<uint8_t> EIP712::struct_type_hash(const std::string& st_name, const nlohmann::json& st) {
//
//    //EIP712Domain(string name,string version,uint256 chainId,address verifyingContract)
//    std::string encode_type = typed_data_struct_envelope(st_name, st);
//    if (0 == encode_type.size()) {
//        return {};
//    }
//
//    return hash(encode_type);
//}
//
//
std::vector<uint8_t> EIP712::struct_type_hash(const std::string& st_name) {

    //EIP712Domain(string name,string version,uint256 chainId,address verifyingContract)
    std::string encode_type = typed_data_struct_envelope(st_name);
    if (0 == encode_type.size()) {
        return {};
    }

    return hash(encode_type);
}


std::vector<uint8_t> EIP712::typed_data_envelope(const std::string& st_name, const nlohmann::json& json_value, const bool is_metamask_v4_compat) {

    uchar_vector encode;
    if (is_atomic_type(st_name)) {
        encode += atomic_typed_data_envelope(st_name, json_value);
        return std::move(encode);
    }

    bool b_array_type = !(std::string::npos == st_name.find("[]"));
    std::string st_n = b_array_type ? st_name.substr(0, st_name.size()-std::string("[]").size()) : st_name;
    auto jsonType = mapType[st_n];
    if (b_array_type) {
        if (nlohmann::detail::value_t::array != json_value.type()) {
            return {};
        }
        for (size_t j=0; j<json_value.size(); ++j) {
            auto json_v = json_value[j];
            if (is_metamask_v4_compat) {
                encode += typed_data_envelope(st_n, json_v);
            }
            else {
                for (size_t k=0; k<jsonType.size(); ++k) {
                    auto typeInJSON = jsonType[k];
                    auto name = typeInJSON["name"].get<std::string>();
                    auto type = typeInJSON["type"].get<std::string>();
                    encode += typed_data_envelope(type, json_v[name]);
                }
            }
        }

        return hash(encode);
    }

    uchar_vector type_hash = struct_type_hash(st_n);
    encode += type_hash;

    for (size_t i=0; i<jsonType.size(); ++i) {
        auto typeInJSON = jsonType[i];
        auto name = typeInJSON["name"].get<std::string>();
        auto type = typeInJSON["type"].get<std::string>();
        encode += typed_data_envelope(type, json_value[name]);
    }

    //keccak
    return hash(encode);
}


} // namespace eth end
} // namespace jub end
