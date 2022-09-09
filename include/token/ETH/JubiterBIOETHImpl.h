#pragma once
#include <memory>

#include "token/JubiterBIO/JubiterBIOToken.h"
#include "token/ETH/JubiterBladeETHImpl.h"


namespace jub {
namespace token {


class JubiterBIOETHImpl :
        public JubiterBIOToken,
virtual public JubiterBladeETHImpl {
public:
    JubiterBIOETHImpl(JUB_UINT16 deviceID) :
        JubiterBIOToken(deviceID),
        JubiterBladeETHImpl(deviceID) {}
    virtual ~JubiterBIOETHImpl() = default;

    virtual JUB_RV SelectApplet() override;

    virtual JUB_RV SignTX(const int erc,
                          const std::vector<JUB_BYTE>& vNonce,
                          const std::vector<JUB_BYTE>& vGasPrice,
                          const std::vector<JUB_BYTE>& vGasLimit,
                          const std::vector<JUB_BYTE>& vTo,
                          const std::vector<JUB_BYTE>& vValue,
                          const std::vector<JUB_BYTE>& vData,
                          const std::vector<JUB_BYTE>& vPath,
                          const std::vector<JUB_BYTE>& vChainID,
                          std::vector<JUB_BYTE>& vRaw) override;

    virtual JUB_RV SetERC20ETHTokens(const ERC20_TOKEN_INFO tokens[],
                                     const JUB_UINT16 iCount) override;
    virtual JUB_RV SetERC20ETHToken(const std::string& tokenName,
                                    const JUB_UINT16 unitDP,
                                    const std::string& contractAddress) override;
    virtual JUB_RV SetERC721ETHToken(const std::string& tokenName,
                                     const std::string& contractAddress) override;
}; // class JubiterBIOETHImpl end


} // namespace token end
} // namespace jub end
