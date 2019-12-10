#ifndef __ETHTokenInterface__
#define __ETHTokenInterface__

#include "JUB_SDK_ETH.h"

#include <vector>

// ETH token extension apdu
#define APPLET_VERSION_SUPPORT_EXT_TOKEN "01040109"

namespace jub {

class ETHTokenInterface {

public:
    virtual JUB_RV SelectAppletETH() = 0;
    virtual JUB_RV GetAppletVersionETH(std::string& version) = 0;
    virtual JUB_RV GetAddressETH(std::string path, JUB_UINT16 tag, std::string& address) = 0;
    virtual JUB_RV GetHDNodeETH(JUB_BYTE format ,std::string path, std::string& pubkey) = 0;
    virtual JUB_RV SignTXETH(bool bERC20,
                             std::vector<JUB_BYTE> vNonce,
                             std::vector<JUB_BYTE> vGasPrice,
                             std::vector<JUB_BYTE> vGasLimit,
                             std::vector<JUB_BYTE> vTo,
                             std::vector<JUB_BYTE> vValue,
                             std::vector<JUB_BYTE> vData,
                             std::vector<JUB_BYTE> vPath,
                             std::vector<JUB_BYTE> vChainID,
                             std::vector<JUB_BYTE>& vRaw) = 0;
    virtual JUB_RV SetERC20ETHToken(const std::string& tokenName,
                                    const JUB_UINT16 unitDP,
                                    const std::string& contractAddress) = 0;
}; // class ETHTokenInterface end

} // namespace jub end

#endif
