#pragma once
#include <memory>

#include "token/JubiterBlade/JubiterBladeToken.h"
#include "token/ETH/JubiterBaseETHImpl.h"


namespace jub {
namespace token {


constexpr JUB_BYTE kPKIAID_ETH[16] = {
    0xD1, 0x56, 0x00, 0x01, 0x32, 0x83, 0x00, 0x42, 0x4C, 0x44, 0x00, 0x00, 0x45, 0x54, 0x48, 0x01
};


class JubiterBladeETHImpl :
        public JubiterBladeToken,
virtual public JubiterBaseETHImpl {
public:
    JubiterBladeETHImpl(JUB_UINT16 deviceID) :
        JubiterBladeToken(deviceID) {}
    ~JubiterBladeETHImpl() {}

    //ETH functions
    virtual JUB_RV SelectApplet() override;
    virtual JUB_RV GetAppletVersion(stVersion& version) override;

    virtual JUB_RV GetAddress(const std::string& path, const JUB_UINT16 tag, std::string& address) override;
    virtual JUB_RV GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey) override;

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
    virtual JUB_RV SignTX(const int erc,
                          const std::vector<JUB_BYTE>& vNonce,
                          const std::vector<JUB_BYTE>& vGasPrice,
                          const std::vector<JUB_BYTE>& vGasLimit,
                          const std::vector<JUB_BYTE>& vTo,
                          const std::vector<JUB_BYTE>& vValue,
                          const std::vector<JUB_BYTE>& vInput,
                          const std::string& accessListInJSON,
                          const std::vector<JUB_BYTE>& vPath,
                          const std::vector<JUB_BYTE>& vChainID,
                          std::vector<JUB_BYTE>& vRaw) override;
    virtual JUB_RV SignTX(const int erc,
                          const std::vector<JUB_BYTE>& vNonce,
                          const std::vector<JUB_BYTE>& vGasLimit,
                          const std::vector<JUB_BYTE>& vMaxPriorityFeePerGas,
                          const std::vector<JUB_BYTE>& vMaxFeePerGas,
                          const std::vector<JUB_BYTE>& vDestination,
                          const std::vector<JUB_BYTE>& vValue,
                          const std::vector<JUB_BYTE>& vData,
                          const std::string& accessListInJSON,
                          const std::vector<JUB_BYTE>& vPath,
                          const std::vector<JUB_BYTE>& vChainID,
                          std::vector<JUB_BYTE>& vRaw) override;

    virtual JUB_RV VerifyTX(const std::vector<JUB_BYTE>& vChainID,
                            const std::string& path,
                            const std::vector<JUB_BYTE>& vSigedTrans) override;

    virtual JUB_RV SetERC20ETHTokens(const ERC20_TOKEN_INFO tokens[],
                                     const JUB_UINT16 iCount) override;
    virtual JUB_RV SetERC20ETHToken(const std::string& tokenName,
                                    const JUB_UINT16 unitDP,
                                    const std::string& contractAddress) override;
    virtual JUB_RV SetERC721ETHToken(const std::string& tokenName,
                                     const std::string& contractAddress) override;

    virtual JUB_RV SignContract(const JUB_BYTE inputType,
                                const std::vector<JUB_BYTE>& vNonce,
                                const std::vector<JUB_BYTE>& vGasPrice,
                                const std::vector<JUB_BYTE>& vGasLimit,
                                const std::vector<JUB_BYTE>& vTo,
                                const std::vector<JUB_BYTE>& vValue,
                                const std::vector<JUB_BYTE>& vInput,
                                const std::vector<JUB_BYTE>& vPath,
                                const std::vector<JUB_BYTE>& vChainID,
                                std::vector<JUB_BYTE>& vRaw) override;
    virtual JUB_RV SignContractHash(const JUB_BYTE inputType,
                                    const std::vector<JUB_BYTE>& vNonce,
                                    const std::vector<JUB_BYTE>& vGasPrice,
                                    const std::vector<JUB_BYTE>& vGasLimit,
                                    const std::vector<JUB_BYTE>& vTo,
                                    const std::vector<JUB_BYTE>& vValue,
                                    const std::vector<JUB_BYTE>& vInput,
                                    const std::vector<JUB_BYTE>& vPath,
                                    const std::vector<JUB_BYTE>& vChainID,
                                    std::vector<JUB_BYTE>& vRaw) override;
    virtual JUB_RV SignContractHash(const JUB_BYTE inputType,
                                    const std::vector<JUB_BYTE>& vGasLimit,
                                    const std::vector<JUB_BYTE>& vInputHash,
                                    const std::vector<JUB_BYTE>& vUnsignedTxHash,
                                    const std::vector<JUB_BYTE>& vPath,
                                    const std::vector<JUB_BYTE>& vChainID,
                                    std::vector<JUB_BYTE>& signatureRaw);

    virtual JUB_RV SignBytestring(const std::vector<JUB_BYTE>& vData,
                                  const std::vector<JUB_BYTE>& vPath,
                                  const std::vector<JUB_BYTE>& vChainID,
                                  std::vector<JUB_BYTE>& signatureRaw) override;
    virtual JUB_RV VerifyBytestring(const std::string& path,
                                    const std::vector<JUB_BYTE>& vData,
                                    const std::vector<JUB_BYTE>& vSignature) override;

    virtual JUB_RV SignTypedData(const bool& bMetamaskV4Compat,
                                 const std::string& typedDataInJSON,
                                 const std::vector<JUB_BYTE>& vPath,
                                 const std::vector<JUB_BYTE>& vChainID,
                                 std::vector<JUB_BYTE>& signatureRaw) override;
    virtual JUB_RV VerifyTypedData(const bool& bMetamaskV4Compat,
                                   const std::string& path,
                                   const std::string& typedDataInJSON,
                                   const std::vector<JUB_BYTE>& vSignature) override;

protected:
    virtual JUB_RV _SignTX(const int erc,
                           const std::vector<JUB_BYTE>& vNonce,
                           const std::vector<JUB_BYTE>& vGasPrice,
                           const std::vector<JUB_BYTE>& vGasLimit,
                           const std::vector<JUB_BYTE>& vTo,
                           const std::vector<JUB_BYTE>& vValue,
                           const std::vector<JUB_BYTE>& vData,
                           const std::vector<JUB_BYTE>& vPath,
                           const std::vector<JUB_BYTE>& vChainID,
                           std::vector<JUB_BYTE>& vRaw);
    virtual JUB_RV _SignTXUpgrade(const int erc,
                                  const std::vector<JUB_BYTE>& vNonce,
                                  const std::vector<JUB_BYTE>& vGasPrice,
                                  const std::vector<JUB_BYTE>& vGasLimit,
                                  const std::vector<JUB_BYTE>& vTo,
                                  const std::vector<JUB_BYTE>& vValue,
                                  const std::vector<JUB_BYTE>& vData,
                                  const std::vector<JUB_BYTE>& vPath,
                                  const std::vector<JUB_BYTE>& vChainID,
                                  std::vector<JUB_BYTE>& vRaw);
}; // class JubiterBladeETHImpl end


} // namespace token end
} // namespace jub end
