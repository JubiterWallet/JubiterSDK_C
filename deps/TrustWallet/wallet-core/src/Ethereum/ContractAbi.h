#pragma once

#include <vector>
#include <stdint.h>
#include <cstdint>
#include <array>

namespace jub {
namespace eth {

typedef enum {
    CREATE_CONTRACT = 0,
    WITH_ADDRESS = 1,           // abandon
    WITH_ADDRESS_AMOUNT = 2,    // abandon
    WITH_TXID = 3,
    WITH_AMOUNT = 4,            // abandon
    WITH_ADDRESS_AMOUNT_DATA = 5,
    NS_ITEM
} ENUM_CONTRACT_ABI;


class ContractAbi
{
public:
    virtual ~ContractAbi();

//    static std::vector<uint8_t> serialize(const std::vector<uint8_t>& methodID, const std::vector<uint8_t>& address);
//    static std::vector<uint8_t> serialize(const std::vector<uint8_t>& methodID, const std::vector<uint8_t>& address, const std::vector<uint8_t>& amount);
    static std::vector<uint8_t> serializeWithTxID(const std::vector<uint8_t>& methodID, const std::vector<uint8_t>& transactionID);
//    static std::vector<uint8_t> serializeWithAmt(const std::vector<uint8_t>& methodID, const std::vector<uint8_t>& amount);
    static std::vector<uint8_t> serialize(const std::vector<uint8_t>& methodID, const std::vector<uint8_t>& address, const std::vector<uint8_t>& amount, const std::vector<uint8_t>& data);

    static std::string parseMethodID(const std::vector<uint8_t>& data);
}; // class ContractAbi end
} // namespace eth end
} // namespace jub end
