#pragma once
#include <token/JubiterBlade/JubiterBladeToken.h>
#include <token/interface/ETHTokenInterface.hpp>

namespace jub {
namespace token {

constexpr JUB_BYTE kPKIAID_ETH[16] = {
    0xD1, 0x56, 0x00, 0x01, 0x32, 0x83, 0x00, 0x42, 0x4C, 0x44, 0x00, 0x00, 0x45, 0x54, 0x48, 0x01
};

class JubiterBladeETHImpl :
    public JubiterBladeToken,
    public ETHTokenInterface {

public:
    JubiterBladeETHImpl(JUB_UINT16 deviceID) :
        JubiterBladeToken(deviceID) {};

    ~JubiterBladeETHImpl() {};

    //ETH functions
    virtual JUB_RV SelectApplet();
    virtual JUB_RV GetAppletVersion(std::string& version);
    virtual JUB_RV GetAddress(const std::string& path, const JUB_UINT16 tag, std::string& address);
    virtual JUB_RV GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey);
    virtual JUB_RV SignTX(const bool bERC20,
                          const std::vector<JUB_BYTE>& vNonce,
                          const std::vector<JUB_BYTE>& vGasPrice,
                          const std::vector<JUB_BYTE>& vGasLimit,
                          const std::vector<JUB_BYTE>& vTo,
                          const std::vector<JUB_BYTE>& vValue,
                          const std::vector<JUB_BYTE>& vData,
                          const std::vector<JUB_BYTE>& vPath,
                          const std::vector<JUB_BYTE>& vChainID,
                          std::vector<JUB_BYTE>& vRaw);
    virtual JUB_RV SetERC20ETHToken(const std::string& tokenName,
                                    const JUB_UINT16 unitDP,
                                    const std::string& contractAddress);
}; // class JubiterBladeETHImpl end


} // namespace token end
} // namespace jub end
