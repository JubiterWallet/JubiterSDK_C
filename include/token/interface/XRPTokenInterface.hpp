//
//  XRPTokenInterface.hpp
//  JubSDK
//
//  Created by Pan Min on 2019/11/25.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#ifndef __XRPTokenInterface__
#define __XRPTokenInterface__

#include "JUB_SDK_XRP.h"

#include <vector>
#include "token/interface/BaseToken.h"


namespace jub {
namespace token {


class XRPTokenInterface :
virtual public BaseToken {

public:
    virtual JUB_RV SelectApplet() = 0;
    virtual JUB_RV GetAppletVersion(stVersion& version) = 0;
    virtual JUB_RV SetCoin() = 0;
    virtual JUB_RV GetAddress(const std::string& path, const JUB_UINT16 tag, std::string& address) = 0;
    virtual JUB_RV GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey) = 0;
    virtual JUB_RV SignTX(const std::vector<JUB_BYTE>& vPath,
                          std::vector<JUB_BYTE>& vUnsignedRaw,
                          std::vector<uchar_vector>& vSignatureRaw) = 0;

    virtual JUB_RV SerializePreimage(const JUB_TX_XRP& tx,
                                     uchar_vector& preimageRaw) = 0;
    virtual JUB_RV CheckAddress(const std::string& address, std::string& rAddress, std::string& tagged) = 0;
}; // class XRPTokenInterface end


} // namespace token end
} // namespace jub end

#endif
