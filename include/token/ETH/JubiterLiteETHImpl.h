#pragma once
#include "token/JubiterLite/JubiterLiteToken.h"
#include "token/JubiterLite/JubiterLiteImpl.h"
#include "token/ETH/JubiterBaseETHImpl.h"

#include <memory>

namespace jub {
namespace token {


class JubiterLiteETHImpl :
        public JubiterLiteImpl,
virtual public JubiterBaseETHImpl {

public:
    //for Factory
    static std::shared_ptr<BaseToken> Create(JUB_UINT16 deviceID) {
        return std::make_shared<JubiterLiteETHImpl>(deviceID);
    }

    JubiterLiteETHImpl(JUB_UINT16 deviceID) :
        JubiterLiteImpl(deviceID) {}
    ~JubiterLiteETHImpl() {}

    //ETH functions
    virtual JUB_RV SelectApplet();
    virtual JUB_RV GetAppletVersion(stVersion& version);
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
    virtual JUB_RV VerifyTX(const std::vector<JUB_BYTE>& vChainID,
                            const std::string& path,
                            const std::vector<JUB_BYTE>& vSigedTrans);
    virtual JUB_RV SetERC20ETHToken(const std::string& tokenName,
                                    const JUB_UINT16 unitDP,
                                    const std::string& contractAddress);

    virtual JUB_RV SignContract(const JUB_BYTE inputType,
                                const std::vector<JUB_BYTE>& vNonce,
                                const std::vector<JUB_BYTE>& vGasPrice,
                                const std::vector<JUB_BYTE>& vGasLimit,
                                const std::vector<JUB_BYTE>& vTo,
                                const std::vector<JUB_BYTE>& vValue,
                                const std::vector<JUB_BYTE>& vInput,
                                const std::vector<JUB_BYTE>& vPath,
                                const std::vector<JUB_BYTE>& vChainID,
                                std::vector<JUB_BYTE>& vRaw);

    virtual JUB_RV SignBytestring(const std::vector<JUB_BYTE>& vTypedData,
                                  const std::vector<JUB_BYTE>& vPath,
                                  const std::vector<JUB_BYTE>& vChainID,
                                  std::vector<JUB_BYTE>& vSignature);
    virtual JUB_RV VerifyBytestring(const std::vector<JUB_BYTE>& vChainID,
                                    const std::string& path,
                                    const std::vector<JUB_BYTE>& vTypedData,
                                    const std::vector<JUB_BYTE>& vSignature);

protected:
    virtual JUB_RV _encodeRSV(const std::vector<JUB_BYTE>& vRSV,
                              const std::vector<JUB_BYTE>& vChainID,
                              TW::Data& r, TW::Data& s, TW::Data& v);
    virtual JUB_RV SignTx(const std::vector<JUB_BYTE>& vNonce,
                          const std::vector<JUB_BYTE>& vGasPrice,
                          const std::vector<JUB_BYTE>& vGasLimit,
                          const std::vector<JUB_BYTE>& vTo,
                          const std::vector<JUB_BYTE>& vValue,
                          const std::vector<JUB_BYTE>& vData,
                          const std::vector<JUB_BYTE>& vPath,
                          const std::vector<JUB_BYTE>& vChainID,
                          std::vector<JUB_BYTE>& vRaw);
}; // class JubiterLiteETHImpl end


} // namespace token end
} // namespace jub end
