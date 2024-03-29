//
//  FILTokenInterface.hpp
//  JubSDK
//
//  Created by Pan Min on 2020/11/12.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#ifndef __FILTokenInterface__
#define __FILTokenInterface__

#include "token/interface/BaseToken.h"
#include <uint256_t/uint256_t.h>
#include <vector>
#include <string>

namespace jub {
namespace token {

class FILTokenInterface : virtual public BaseToken {
  public:
    virtual JUB_RV SelectApplet()                                                                  = 0;
    virtual JUB_RV SetCoin()                                                                       = 0;
    virtual JUB_RV GetAddress(const std::string &path, const JUB_UINT16 tag, std::string &address) = 0;
    virtual JUB_RV CheckAddress(const std::string &address)                                        = 0;
    virtual JUB_RV GetHDNode(const JUB_BYTE format, const std::string &path, std::string &pubkey)  = 0;
    virtual JUB_RV SignTX(const uint64_t &nonce, const int64_t &glimit, const uint256_t &gfeeCap,
                          const uint256_t &gpremium, const std::string &to, const uint256_t &value,
                          const std::string &input, const std::string &path,
                          std::vector<uchar_vector> &vSignatureRaw)                                = 0;
}; // class FILTokenInterface end

} // namespace token
} // namespace jub

#endif
