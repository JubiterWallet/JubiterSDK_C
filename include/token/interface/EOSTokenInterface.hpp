//
//  EOSTokenInterface.hpp
//  JubSDK
//
//  Created by Pan Min on 2019/10/09.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#ifndef __EOSTokenInterface__
#define __EOSTokenInterface__

#include "JUB_SDK_EOS.h"

#include <vector>

namespace jub {

class EOSTokenInterface {

public:
    virtual JUB_RV SelectAppletEOS() = 0;
    virtual JUB_RV GetAddressEOS(std::string path, JUB_UINT16 tag, std::string& address) = 0;
    virtual JUB_RV GetHDNodeEOS(JUB_BYTE format ,std::string path, std::string& pubkey) = 0;
    virtual JUB_RV SignTXEOS(std::string path,
                             const std::string& chainId,
                             const std::string& referenceBlockId,
                             const JUB_UINT32&  referenceBlockTime,
                             const std::string& currency,
                             const std::string& from,
                             const std::string& to,
                             const std::string& asset,
                             const std::string& memo,
                             std::string& rawInJSON) = 0;
}; // class EOSTokenInterface end

} // namespace jub end

#endif
