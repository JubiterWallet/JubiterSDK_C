#pragma once

#include <vector>
#include <stdint.h>

namespace jub
{
namespace eth
{

class ERC20Abi
{
public:
    virtual ~ERC20Abi();

    static std::vector<uint8_t> serialize(std::vector<uint8_t>address, std::vector<uint8_t> value);
}; // class ERC20Abi end

} // namespace eth end

} // namespace jub end
