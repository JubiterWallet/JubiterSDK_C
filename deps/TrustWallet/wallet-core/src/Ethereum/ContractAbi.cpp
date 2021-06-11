#include "ERC20Abi.h"
#include "ContractAbi.h"
#include "utility/util.h"

#include <vector>

namespace jub {
namespace eth {


//std::vector<uint8_t> ContractAbi::serialize(const std::vector<uint8_t>& methodID, const std::vector<uint8_t>& address) {
//
//    EthereumContractPayloadStream stream;
//    EthereumContractMethodHash hash;
//    std::copy_n(methodID.begin(), methodID.size(), hash.begin());
//    stream << hash;
//    stream << address;
//
//    return stream.get_data();
//}
//
//
//std::vector<uint8_t> ContractAbi::serialize(const std::vector<uint8_t>& methodID, const std::vector<uint8_t>& address, const std::vector<uint8_t>& amount) {
//
//    EthereumContractPayloadStream stream;
//    EthereumContractMethodHash hash;
//    std::copy_n(methodID.begin(), methodID.size(), hash.begin());
//    stream << hash;
//    stream << address;
//    stream << amount;
//
//    return stream.get_data();
//}
//
//
std::vector<uint8_t> ContractAbi::serializeWithTxID(const std::vector<uint8_t>& methodID, const std::vector<uint8_t>& transactionID) {

    EthereumContractPayloadStream stream;
    EthereumContractMethodHash hash;
    std::copy_n(methodID.begin(), methodID.size(), hash.begin());
    stream << hash;
    stream << transactionID;

    return stream.get_data();
}


//std::vector<uint8_t> ContractAbi::serializeWithAmt(const std::vector<uint8_t>& methodID, const std::vector<uint8_t>& amount) {
//
//    EthereumContractPayloadStream stream;
//    EthereumContractMethodHash hash;
//    std::copy_n(methodID.begin(), methodID.size(), hash.begin());
//    stream << hash;
//    stream << amount;
//
//    return stream.get_data();
//}
//
//
std::vector<uint8_t> ContractAbi::serialize(const std::vector<uint8_t>& methodID, const std::vector<uint8_t>& address, const std::vector<uint8_t>& amount, const std::vector<uint8_t>& data) {

    EthereumContractPayloadStream stream;
    EthereumContractMethodHash hash;
    std::copy_n(methodID.begin(), methodID.size(), hash.begin());
    stream << hash;
    stream << address;
    stream << amount;

    std::vector<uint8_t> functionId;
    functionId.push_back(0x60);
    stream << functionId;

    std::vector<uint8_t> dataLen;
    dataLen.push_back(data.size());
    stream << dataLen;
    stream << data;

    return stream.get_data();
}


std::string ContractAbi::parseMethodID(const std::vector<uint8_t>& data) {

    if (0 == data.size()) {
        return "";
    }

    uchar_vector v(data);

    uchar_vector vMethodID;
    vMethodID << v.read_vector(kETH_METHOD_HASH_SIZE);

    return vMethodID.getHex();
}


} // namespace eth end
} // namespace jub end
