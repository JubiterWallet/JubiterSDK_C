#pragma once
#include <cstdint>
#ifndef __ContextETH__
#define __ContextETH__

#include "JUB_SDK_ETH.h"

#include <string>
#include <vector>

#include "utility/Singleton.h"
#include "utility/xManager.hpp"

#include "context/BaseContext.h"
#include "token/interface/BaseToken.h"

#include <Ethereum/ContractAbi.h>
#include <map>


namespace jub {
namespace context {


class ETHContext :
    public BaseContext {
public:
    ETHContext(const CONTEXT_CONFIG_ETH& cfg, std::shared_ptr<token::BaseToken> tokenPtr):
        BaseContext(tokenPtr) {
            _mainPath = cfg.mainPath;
            _chainID = cfg.chainID;
            _timeout = 120 * 2;
    }
    ~ETHContext() {}

    virtual JUB_RV GetAddress(const BIP44_Path& path, const JUB_UINT16 tag, std::string& address);
    virtual JUB_RV SetMyAddress(const BIP44_Path& path, std::string& address);
    virtual JUB_RV GetHDNode(const JUB_BYTE format, const BIP44_Path& path, std::string& pubkey);
    virtual JUB_RV GetMainHDNode(const JUB_BYTE format, std::string& xpub);

    virtual JUB_RV SignTransaction(const BIP44_Path& path,
                                   const JUB_UINT32 nonce,
                                   const JUB_UINT32 gasLimit,
                                   JUB_CHAR_CPTR gasPriceInWei,
                                   JUB_CHAR_CPTR to,
                                   JUB_CHAR_CPTR valueInWei,
                                   JUB_CHAR_CPTR input,
                                   OUT std::string& strRaw);
    virtual JUB_RV SignTransaction(const BIP44_Path& path,
                                   const JUB_UINT32 nonce,
                                   const JUB_UINT32 gasLimit,
                                   JUB_CHAR_CPTR gasPriceInWei,
                                   JUB_CHAR_CPTR to,
                                   JUB_CHAR_CPTR valueInWei,
                                   JUB_CHAR_CPTR data,
                                   JUB_CHAR_CPTR accessListInJSON,
                                   OUT std::string& strRaw);
    virtual JUB_RV SignTransaction(const BIP44_Path& path,
                                   const JUB_UINT32 nonce,
                                   const JUB_UINT32 gasLimit,
                                   JUB_CHAR_CPTR maxPriorityFeePerGas,
                                   JUB_CHAR_CPTR maxFeePerGas,
                                   JUB_CHAR_CPTR destination,
                                   JUB_CHAR_CPTR valueInWei,
                                   JUB_CHAR_CPTR data,
                                   JUB_CHAR_CPTR accessListInJSON,
                                   OUT std::string& strRaw);

    virtual JUB_RV SignContract(const BIP44_Path& path,
                                const JUB_UINT32 nonce,
                                const JUB_UINT32 gasLimit,
                                JUB_CHAR_CPTR gasPriceInWei,
                                JUB_CHAR_CPTR to,
                                IN JUB_CHAR_CPTR valueInWei,
                                IN JUB_CHAR_CPTR input,
                                OUT std::string& strRaw);

    virtual JUB_RV SignBytestring(const BIP44_Path& path,
                                  const JUB_CHAR_CPTR data,
                                  OUT std::string& signature);
    virtual JUB_RV SignTypedData(const JUB_BBOOL bMetamaskV4Compat,
                                 const BIP44_Path& path,
                                 const JUB_CHAR_CPTR typedDataInJSON,
                                 OUT std::string& signature);

    virtual JUB_RV BuildERC20TransferAbi(JUB_CHAR_CPTR to, JUB_CHAR_CPTR value, std::string& abi);
    virtual JUB_RV SetERC20ETHToken(JUB_CHAR_CPTR pTokenName,
                                    JUB_UINT16 unitDP,
                                    JUB_CHAR_CPTR pContractAddress);
    virtual JUB_RV SetERC20ETHTokens(ERC20_TOKEN_INFO tokens[],
                                     JUB_UINT16 iCount);

    virtual JUB_RV BuildERC721TransferAbi(JUB_CHAR_CPTR from, JUB_CHAR_CPTR to, JUB_CHAR_CPTR pTokenID, std::string& abi);
    virtual JUB_RV SetERC721ETHToken(JUB_CHAR_CPTR pTokenName,
                                     JUB_CHAR_CPTR pContractAddress);

    virtual JUB_RV BuildERC1155TransferAbi(JUB_CHAR_CPTR from, JUB_CHAR_CPTR to, JUB_CHAR_CPTR pTokenID, JUB_CHAR_CPTR value, JUB_CHAR_CPTR data, std::string& abi);
    virtual JUB_RV BuildERC1155BatchTransferAbi(JUB_CHAR_CPTR from, JUB_CHAR_CPTR to, std::vector<std::string>& tokenIDs, std::vector<std::string>& values, JUB_CHAR_CPTR data, std::string& abi);

    virtual JUB_RV ActiveSelf() override;

private:
    void _addContrFunc(const std::string& methodID, const jub::eth::ENUM_CONTRACT_ABI& type) {
        if (_contrFuncList.end() == _contrFuncList.find(methodID)) {
            _contrFuncList.insert({methodID, (int)type});
        }
    }
    jub::eth::ENUM_CONTRACT_ABI _getInputType(const std::string& methodID) {
        std::map<std::string, int>::iterator it = _contrFuncList.find(methodID);
        if (_contrFuncList.end() != it) {
            return (jub::eth::ENUM_CONTRACT_ABI)(it->second);
        }

        //Enforce to use the "Signning Hash" mode
//        return jub::eth::ENUM_CONTRACT_ABI::NS_ITEM;
        return jub::eth::ENUM_CONTRACT_ABI::CREATE_CONTRACT;
    }

    uint32_t _chainID;
    // "methodID" - input type
    std::map<std::string, int> _contrFuncList;
}; // class ETHContext end


} // namespace context end
} // namespace jub end

#endif // #pragma once
