#include "ERC721Abi.h"
#include "utility/util.h"

#include <vector>

namespace jub {
namespace eth {


extern const size_t kETHEREUM_SIZE_VARIABLE_FUNCTION_CONTRACT;
extern const size_t kETH_METHOD_HASH_SIZE;


std::vector<uint8_t> toTwosComplement(const uint256_t& number) {

    TW::Data vNumber;
    TW::encode256BE(vNumber, number, 256);

    return vNumber;
}


std::vector<uint8_t> toTwosComplement(const std::string& number, const uint8_t base) {

    return toTwosComplement(uint256_t(number.c_str(), base));
}


std::vector<uint8_t> ERC721Abi::serialize(const std::vector<uint8_t>& from, const std::vector<uint8_t>& to, const std::string& tokenID, const std::string methodID) {

    EthereumContractPayloadStream stream;
    TW::Data vMethodID = HexStr2CharPtr(methodID);
    EthereumContractMethodHash hash;
    std::memcpy(&hash[0], &vMethodID[0], vMethodID.size());
    stream << hash;
    stream << from;
    stream << to;
    stream << toTwosComplement(tokenID, 10);

    return stream.get_data();
}


std::vector<uint8_t> ERC721Abi::serialize(const std::vector<uint8_t>& from, const std::vector<uint8_t>& to, const std::string& tokenID, const std::vector<uint8_t>& data, const std::string methodID) {

    TW::Data vMethodID = HexStr2CharPtr(methodID);

    EthereumContractMethodHash hash;
    std::memcpy(&hash[0], &vMethodID[0], vMethodID.size());

    EthereumContractPayloadStream stream;
    stream << hash;
    stream << from;
    stream << to;
    stream << toTwosComplement(tokenID, 10);
    if (0 < data.size()) {
        stream << data;
    }

    return stream.get_data();
}


} // namespace eth end
} // namespace jub end
