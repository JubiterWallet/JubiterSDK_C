//
//  DOTTokenInterface.hpp
//  JubSDK
//
//  Created by Pan Min on 2020/11/12.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#ifndef __DOTTokenInterface__
#define __DOTTokenInterface__

#include "JUB_SDK_DOT.h"

#include <vector>
#include "token/interface/BaseToken.h"

#include "mSIGNA/stdutils/uchar_vector.h"
#include "utility/util.h"


namespace jub {
namespace token {


class DOTTokenInterface :
virtual public BaseToken {

public:
    virtual JUB_RV SelectApplet() = 0;
    virtual JUB_RV SetCoin() = 0;
    virtual JUB_RV SetCoinCurvePublicKeyType(JUB_ENUM_COINTYPE_DOT coinType, JUB_ENUM_CURVES curves) = 0;
    virtual JUB_RV GetAddress(const std::string& path, const JUB_UINT16 tag, std::string& address, const TWCoinType& coinNet=TWCoinType::TWCoinTypeBitcoin) = 0;
    virtual JUB_RV GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey, const TWCoinType& coinNet=TWCoinType::TWCoinTypeBitcoin) = 0;

    virtual JUB_RV CheckAddress(const std::string& address, const TWCoinType& coin, const TWCoinType& coinNet=TWCoinType::TWCoinTypeBitcoin) = 0;

    virtual JUB_RV SignTX(const std::string &path,
                          const std::string &genesisHash,
                          const std::string &blockHash,
                          const uint64_t& nonce,
                          const uint32_t& specVersion,
                          const uint64_t& network,
                          const uint32_t& transaction_version,
                          const uint64_t& blockNumber,
                          const uint64_t& eraPeriod,
                          const std::string& tip,
                          const std::string &to,
                          const std::string& value,
                          const bool keep_alive,
                          std::vector<JUB_BYTE>& vSignatureRaw) = 0;
}; // class DOTTokenInterface end


} // namespace token end
} // namespace jub end

#endif
