#include "ERC20Abi.h"
#include "utility/util.h"

#include <vector>
#include <array>

namespace jub {
namespace eth {


EthereumContractPayloadStream& operator<<(EthereumContractPayloadStream& stream, const std::vector<std::uint8_t>& data) {

    static const std::array<uint8_t, kETHEREUM_SIZE_VARIABLE_FUNCTION_CONTRACT> ZEROES = {0,};
    stream.write_data(ZEROES.data(), kETHEREUM_SIZE_VARIABLE_FUNCTION_CONTRACT - data.size());
    stream.write_data(&data[0], data.size());

    return stream;
}


EthereumContractPayloadStream& operator<<(EthereumContractPayloadStream& stream, const EthereumContractMethodHash& method_hash) {

    stream.write_data(method_hash.data(), method_hash.size());

    return stream;
}


std::vector<uint8_t> ERC20Abi::serialize(const std::vector<uint8_t>& address, const std::vector<uint8_t>& value) {

    TW::Data vMethodID = HexStr2CharPtr(ERC20_ABI_METHOD_ID_TRANSFER);

    EthereumContractMethodHash hash;
    std::memcpy(&hash[0], &vMethodID[0], vMethodID.size());

    EthereumContractPayloadStream stream;
    stream << hash;
    stream << address;
    stream << value;

    return stream.get_data();
}


} // namespace eth end
} // namespace jub end
