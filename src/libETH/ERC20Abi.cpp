#include <libETH/ERC20Abi.h>
#include <vector>
#include <array>

namespace jub {

namespace eth {

const size_t kETHEREUM_SIZE_VARIABLE_FUNCTION_CONTRACT = 32;
const size_t kETH_METHOD_HASH_SIZE = 4;
typedef std::array<uint8_t, kETH_METHOD_HASH_SIZE> EthereumContractMethodHash;

struct BinaryData {
    const unsigned char* data;
    size_t len;
}; // struct BinaryData end

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

std::vector<uint8_t> ERC20Abi::serialize(std::vector<uint8_t>address, std::vector<uint8_t> value) {

    EthereumContractPayloadStream stream;
    EthereumContractMethodHash hash = { 0xa9, 0x05, 0x9c, 0xbb };
    stream << hash;
    stream << address;
    stream << value;

    return stream.get_data();
}
} // namespace eth end

} // namespace jub end
