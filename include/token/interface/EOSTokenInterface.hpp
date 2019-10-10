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
}; // class EOSTokenInterface end

} // namespace jub end

#endif
