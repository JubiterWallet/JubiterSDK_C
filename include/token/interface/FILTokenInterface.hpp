//
//  FILTokenInterface.hpp
//  JubSDK
//
//  Created by Pan Min on 2020/11/12.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#ifndef __FILTokenInterface__
#define __FILTokenInterface__

#include "JUB_SDK_FIL.h"

#include <vector>
#include "token/interface/BaseToken.h"

#include "mSIGNA/stdutils/uchar_vector.h"
#include "utility/util.h"


namespace jub {
namespace token {


class FILTokenInterface :
virtual public BaseToken {

public:
    virtual JUB_RV SelectApplet() = 0;
    virtual JUB_RV SetCoin() = 0;
    virtual JUB_RV GetAddress(const std::string& path, const JUB_UINT16 tag, std::string& address) = 0;
    virtual JUB_RV GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey) = 0;
    virtual JUB_RV SignTX(const uint64_t& nonce,
                          const uint64_t& gprice,
                          const uint64_t& glimit,
                          const std::string& to,
                          const uint64_t& value,
                          const std::string& input,
                          const std::string& path,
                          std::vector<JUB_BYTE>& vSignatureRaw) = 0;
}; // class FILTokenInterface end


} // namespace token end
} // namespace jub end

#endif
