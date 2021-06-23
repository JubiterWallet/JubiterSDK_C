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
#include "EOS/Prefixes.h"

#include <vector>
#include "token/interface/BaseToken.h"
#include <nlohmann/json.hpp>


namespace jub {
namespace token {


class EOSTokenInterface :
virtual public BaseToken {
public:
    virtual ~EOSTokenInterface() = default;

    virtual JUB_RV SelectApplet() = 0;
    virtual JUB_RV GetAppletVersion(stVersion& version) = 0;
    virtual JUB_RV SetCoin() = 0;
    virtual JUB_RV GetAddress(const TW::EOS::Type& type, const std::string& path, const JUB_UINT16 tag, std::string& address) = 0;
    virtual JUB_RV CheckAddress(const TW::EOS::Type& type, const std::string& address) = 0;
    virtual JUB_RV GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey) = 0;
    virtual JUB_RV SignTX(const TW::EOS::Type& type,
                          const std::vector<JUB_BYTE>& vPath,
                          const std::vector<JUB_BYTE>& vChainId,
                          const std::vector<JUB_BYTE>& vRaw,
                          std::vector<uchar_vector>& vSignatureRaw,
                          const bool bWithType = false) = 0;

    virtual nlohmann::json SerializeAction(const JUB_ACTION_EOS& action) = 0;
    virtual JUB_RV SerializeActions(const JUB_ACTION_EOS_PTR actions, JUB_UINT16 actionCount,
                                    std::string& actionsInJSON) = 0;

    virtual JUB_RV SerializePreimage(const std::string& expiration,
                                     const std::string& referenceBlockId,
                                     const std::string& referenceBlockTime,
                                     const std::string& actionsInJSON,
                                     uchar_vector& preimageRaw,
                                     const bool bWithType) = 0;
}; // class EOSTokenInterface end


} // namespace token end
} // namespace jub end

#endif
