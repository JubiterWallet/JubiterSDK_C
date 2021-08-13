#include "ERC1155Abi.h"
#include "utility/util.h"

#include <vector>

namespace jub {
namespace eth {


extern const size_t kETHEREUM_SIZE_VARIABLE_FUNCTION_CONTRACT;
extern const size_t kETH_METHOD_HASH_SIZE;


std::vector<uint8_t> ERC1155Abi::serialize(const std::vector<uint8_t>& from, const std::vector<uint8_t>& to,
                                           const std::string& tokenID, const std::string& value,
                                           const std::string methodID) {

    TW::Data vData = toTwosComplement(std::to_string(32*5), 10);    // offset
    vData.resize(32+32);    // length
    vData.resize(32+32+32); // zero-bytes
    return serialize(from, to, tokenID, value, vData, methodID);
}


std::vector<uint8_t> ERC1155Abi::serialize(const std::vector<uint8_t>& from, const std::vector<uint8_t>& to,
                                           const std::string& tokenID, const std::string& value,
                                           const std::vector<uint8_t>& data,
                                           const std::string methodID) {

    TW::Data vMethodID = HexStr2CharPtr(methodID);

    EthereumContractMethodHash hash;
    std::memcpy(&hash[0], &vMethodID[0], vMethodID.size());

    EthereumContractPayloadStream stream;
    stream << hash;
    stream << from;
    stream << to;
    stream << toTwosComplement(tokenID, 10);
    stream << toTwosComplement(value, 10);
    if (0 < data.size()) {
        stream << data;
    }

    return stream.get_data();
}


std::vector<uint8_t> ERC1155Abi::serialize(const std::vector<uint8_t>& from, const std::vector<uint8_t>& to,
                                           const std::vector<std::string>& tokenIDs, const std::vector<std::string>& values,
                                           const std::string methodID) {

    TW::Data vData;
    vData.resize(32+32);
    return serialize(from, to, tokenIDs, values, vData, methodID);
}


std::vector<uint8_t> ERC1155Abi::serialize(const std::vector<uint8_t>& from, const std::vector<uint8_t>& to,
                                           const std::vector<std::string>& tokenIDs, const std::vector<std::string>& values,
                                           const std::vector<uint8_t>& data,
                                           const std::string methodID) {

    TW::Data vMethodID = HexStr2CharPtr(methodID);

    EthereumContractMethodHash hash;
    std::memcpy(&hash[0], &vMethodID[0], vMethodID.size());

    EthereumContractPayloadStream stream;
    stream << hash;
    stream << from;
    stream << to;
    stream << toTwosComplement(std::to_string(32*(2+3)), 10);           // offset of tokenIDs: from + to + 3offsets
    stream << toTwosComplement(std::to_string(32*(2+3+1+tokenIDs.size())), 10);                 // offset of values
    stream << toTwosComplement(std::to_string(32*(2+3+1+tokenIDs.size()+1+values.size())), 10); // offset of data
    stream << toTwosComplement(std::to_string(tokenIDs.size()), 10);    // count of tokenIDs
    for (size_t i=0; i<tokenIDs.size(); ++i) {
        stream << toTwosComplement(tokenIDs[i], 10);                   // tokenID_i
    }
    stream << toTwosComplement(std::to_string(values.size()), 10);      // count of values
    for (size_t i=0; i<values.size(); ++i) {
        stream << toTwosComplement(values[i], 10);                     // value_i
    }
    if (0 < data.size()) {
        stream << data;
    }

    return stream.get_data();
}


} // namespace eth end
} // namespace jub end
