#ifndef __ETHTokenInterface__
#define __ETHTokenInterface__

#include "JUB_SDK_ETH.h"

#include <vector>
#include "token/interface/BaseToken.h"
#include "utility/util.h"


namespace jub {
namespace token {


enum JUB_ENUM_APDU_ERC_ETH : uint16_t {
    ERC_INVALID = 0,
    ERC_20   = 0x0014,
    ERC_721  = 0x02D1,
};
enum JUB_ENUM_APDU_ERC_P1 : uint8_t {
    ERC20  = 0x00,
    ERC721 = 0x01,
    TOKENS_INFO = 0x02,  // Compatible with both ERC20 & ERC721
};


typedef enum {
    CREATE_CONTRACT = 0,
    WITH_ADDRESS = 1,           // abandon
    WITH_ADDRESS_AMOUNT = 2,    // abandon
    WITH_TXID = 3,
    WITH_AMOUNT = 4,            // abandon
    WITH_ADDRESS_AMOUNT_DATA = 5,
    NS_ITEM
} ENUM_CONTRACT_ABI;


class ETHTokenInterface:
virtual public BaseToken {
public:
    virtual JUB_RV SelectApplet() = 0;
    virtual JUB_RV GetAppletVersion(stVersion& version) = 0;
    virtual JUB_RV GetAddress(const std::string& path, const JUB_UINT16 tag, std::string& address) = 0;
    virtual JUB_RV GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey) = 0;
    virtual JUB_RV SignTX(const int erc,
                          const std::vector<JUB_BYTE>& vNonce,
                          const std::vector<JUB_BYTE>& vGasPrice,
                          const std::vector<JUB_BYTE>& vGasLimit,
                          const std::vector<JUB_BYTE>& vTo,
                          const std::vector<JUB_BYTE>& vValue,
                          const std::vector<JUB_BYTE>& vInput,
                          const std::vector<JUB_BYTE>& vPath,
                          const std::vector<JUB_BYTE>& vChainID,
                          std::vector<JUB_BYTE>& vRaw) = 0;
    virtual JUB_RV VerifyTX(const std::vector<JUB_BYTE>& vChainID,
                            const std::string& path,
                            const std::vector<JUB_BYTE>& vSigedTrans) = 0;
    virtual JUB_RV SetERC20ETHTokens(const ERC20_TOKEN_INFO tokens[],
                                     const JUB_UINT16 iCount) = 0;
    virtual JUB_RV SetERC20ETHToken(const std::string& tokenName,
                                    const JUB_UINT16 unitDP,
                                    const std::string& contractAddress) = 0;
    virtual JUB_RV SetERC721ETHToken(const std::string& tokenName,
                                     const std::string& contractAddress) = 0;

    virtual JUB_RV SignContract(const JUB_BYTE inputType,
                                const std::vector<JUB_BYTE>& vNonce,
                                const std::vector<JUB_BYTE>& vGasPrice,
                                const std::vector<JUB_BYTE>& vGasLimit,
                                const std::vector<JUB_BYTE>& vTo,
                                const std::vector<JUB_BYTE>& vValue,
                                const std::vector<JUB_BYTE>& vInput,
                                const std::vector<JUB_BYTE>& vPath,
                                const std::vector<JUB_BYTE>& vChainID,
                                std::vector<JUB_BYTE>& vRaw) = 0;
    virtual JUB_RV SignContractHash(const JUB_BYTE inputType,
                                    const std::vector<JUB_BYTE>& vNonce,
                                    const std::vector<JUB_BYTE>& vGasPrice,
                                    const std::vector<JUB_BYTE>& vGasLimit,
                                    const std::vector<JUB_BYTE>& vTo,
                                    const std::vector<JUB_BYTE>& vValue,
                                    const std::vector<JUB_BYTE>& vInput,
                                    const std::vector<JUB_BYTE>& vPath,
                                    const std::vector<JUB_BYTE>& vChainID,
                                    std::vector<JUB_BYTE>& vRaw) = 0;

    virtual JUB_RV SignBytestring(const std::vector<JUB_BYTE>& vTypedData,
                                  const std::vector<JUB_BYTE>& vPath,
                                  const std::vector<JUB_BYTE>& vChainID,
                                  std::vector<JUB_BYTE>& signatureRaw) = 0;
    virtual JUB_RV VerifyBytestring(const std::vector<JUB_BYTE>& vChainID,
                                    const std::string& path,
                                    const std::vector<JUB_BYTE>& vTypedData,
                                    const std::vector<JUB_BYTE>& vSignature) = 0;
}; // class ETHTokenInterface end


} // namespace token end
} // namespace jub end

#endif
