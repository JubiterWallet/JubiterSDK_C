//
//  TRXTokenInterface.hpp
//  JubSDK
//
//  Created by Pan Min on 2020/09/10.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#ifndef __TRXTokenInterface__
#define __TRXTokenInterface__

#include "JUB_SDK_TRX.h"

#include <vector>
#include <token/interface/BaseToken.h>
#include <mSIGNA/stdutils/uchar_vector.h>
#include <google/protobuf/any.pb.h>


// TRX token extension apdu
#define MISC_APPLET_VERSION_SUPPORT_EXT_TOKEN "01000006"


namespace jub {
namespace token {


class TRXTokenInterface :
virtual public BaseToken {
public:
    virtual ~TRXTokenInterface() = default;

    virtual JUB_RV SelectApplet() = 0;
    virtual JUB_RV GetAppletVersion(std::string& version) = 0;
    virtual JUB_RV SetCoin() = 0;
    virtual JUB_RV GetAddress(const std::string& path, const JUB_UINT16 tag, std::string& address) = 0;
    virtual JUB_RV GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey) = 0;
    virtual JUB_RV SignTX(const std::vector<JUB_BYTE>& vPath,
                          const std::vector<JUB_BYTE>& vRaw,
                          std::vector<uchar_vector>& vSignatureRaw) = 0;
    virtual JUB_RV SetTRC20Token(const std::string& tokenName,
                                 const JUB_UINT16 unitDP,
                                 const std::string& contractAddress) = 0;

    virtual JUB_RV SerializeContract(const JUB_CONTRACT_TRX& contract,
                                     ::google::protobuf::Any& parameter) = 0;
    virtual JUB_RV PackTransactionRaw(const JUB_TX_TRX& tx,
                                      std::string& packedContractInPB) = 0;
    virtual JUB_RV SerializePreimage(const std::string& packedContractInPb,
                                     uchar_vector& preimageRaw) = 0;
}; // class TRXTokenInterface end


} // namespace token end
} // namespace jub end

#endif
