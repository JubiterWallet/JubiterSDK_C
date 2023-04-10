#ifndef __BTCTokenInterface__
#define __BTCTokenInterface__

#include "JUB_SDK_BTC.h"

#include "token/interface/BaseToken.h"
#include "utility/util.h"


namespace jub {
namespace token {

typedef std::map<std::string, std::vector<std::string>> MAP_COSIGNER_SIGNATURE;
constexpr JUB_UINT32 version1 = 0x01;

class BTCTokenInterface :
virtual public BaseToken {
public:
    virtual ~BTCTokenInterface() = default;

    virtual JUB_RV SelectApplet() = 0;
    virtual JUB_RV SetUnit(const JUB_ENUM_BTC_UNIT_TYPE& unit) = 0;
    virtual JUB_RV SetCoin(const JUB_ENUM_COINTYPE_BTC& type) = 0;
    virtual JUB_RV SignTX(const JUB_BYTE addrFmt,
                          const JUB_ENUM_BTC_TRANS_TYPE& type,
                          const JUB_UINT16 inputCount,
                          const std::vector<JUB_UINT64>& vInputAmount,
                          const std::vector<std::string>& vInputPath,
                          const std::vector<JUB_UINT16>& vChangeIndex,
                          const std::vector<std::string>& vChangePath,
                          const std::vector<JUB_BYTE>& vUnsigedTrans,
                          std::vector<JUB_BYTE>& vRaw,
                          const TWCoinType& coinNet=TWCoinType::TWCoinTypeBitcoin) = 0;
    virtual JUB_RV VerifyTX(const JUB_ENUM_BTC_TRANS_TYPE& type,
                            const std::vector<JUB_UINT64>& vInputAmount,
                            const std::vector<std::string>& vInputPath,
                            const std::vector<JUB_BYTE>& vSigedTrans,
                            const TWCoinType& coinNet=TWCoinType::TWCoinTypeBitcoin) = 0;

    virtual JUB_RV GetHDNode(const JUB_ENUM_BTC_TRANS_TYPE& type, const std::string& path, std::string& xpub, const TWCoinType& coinNet=TWCoinType::TWCoinTypeBitcoin) = 0;
    virtual JUB_RV GetAddress(const JUB_BYTE addrFmt,
                              const JUB_ENUM_BTC_TRANS_TYPE& type,
                              const std::string& path,
                              const JUB_UINT16 tag,
                              std::string& address,
                              const TWCoinType& coinNet=TWCoinType::TWCoinTypeBitcoin) = 0;
    
    virtual JUB_RV CheckAddress(const std::string& address, const TWCoinType& coinNet=TWCoinType::TWCoinTypeBitcoin) = 0;

    virtual JUB_RV SerializeUnsignedTx(const JUB_ENUM_BTC_TRANS_TYPE& type,
                                       const JUB_UINT32 version,
                                       const std::vector<INPUT_BTC>& vInputs,
                                       const std::vector<OUTPUT_BTC>& vOutputs,
                                       const JUB_UINT32 lockTime,
                                       uchar_vector& unsignedRaw, const TWCoinType& coinNet=TWCoinType::TWCoinTypeBitcoin) = 0;
    virtual JUB_RV SerializeTx(const JUB_ENUM_COINTYPE_BTC coinType,
                               const JUB_ENUM_BTC_TRANS_TYPE type,
                               const JUB_UINT32 version,
                               const unsigned long m,
                               const std::vector<INPUT_BTC>& vInputs,
                               const MAP_COSIGNER_SIGNATURE& mapCosignerSignatures,
                               const std::vector<uchar_vector>& vRedeemScript,
                               const std::vector<OUTPUT_BTC>& vOutputs,
                               const JUB_UINT32 lockTime,
                               uchar_vector& signedIncRaw) = 0;
    virtual JUB_RV SerializePreimage(const JUB_ENUM_COINTYPE_BTC coinType,
                                     const JUB_ENUM_BTC_TRANS_TYPE type,
                                     const JUB_UINT32 version,
                                     const std::size_t for_sign_input_index,
                                     const std::vector<INPUT_BTC>& vInputs,
                                     const std::vector<uchar_vector>& vRedeemScript,
                                     const std::vector<OUTPUT_BTC>& vOutputs,
                                     const JUB_UINT32 lockTime,
                                     uchar_vector& preimageRaw,
                                     const bool bWitness = false) = 0;

    // !!Not handling witness yet
    virtual JUB_RV DeserializeTx(bool bWitness,
                                 const std::string hexTx,
                                 std::vector<INPUT_BTC>& vInputs,
                                 std::vector<OUTPUT_BTC>& vOutputs,
                                 JUB_UINT32_PTR plockTime) = 0;
    // !!Not handling witness yet
    virtual JUB_RV DeserializeTx(bool bWitness,
                                 const std::vector<uchar_vector>& vRedeemScript,
                                 const std::string hexIncTx,
                                 MAP_COSIGNER_SIGNATURE& mapCosignerSignature,
                                 uchar_vector& unsignedTx) = 0;

    virtual JUB_RV GetAddressMultiSig(const JUB_BYTE addrFmt,
                                      const JUB_ENUM_BTC_TRANS_TYPE& type,
                                      const std::string& path,
                                      const JUB_UINT16 tag,
                                      const uchar_vector& vRedeemScriptTlv,
                                      std::string& address,
                                      const TWCoinType& coinNet=TWCoinType::TWCoinTypeBitcoin) = 0;
    virtual JUB_RV SignTXMultiSig(const JUB_BYTE addrFmt,
                                  const JUB_ENUM_BTC_TRANS_TYPE& type,
                                  const JUB_UINT16 inputCount,
                                  const std::vector<JUB_UINT64>& vInputAmount,
                                  const std::vector<std::string>& vInputPath,
                                  const std::vector<uchar_vector>& vRedeemScriptTlv,
                                  const std::vector<JUB_UINT16>& vChangeIndex,
                                  const std::vector<std::string>& vChangePath,
                                  const std::vector<uchar_vector>& vChangeRedeemScriptTlv,
                                  const std::vector<JUB_BYTE>& vUnsigedTrans,
                                  std::vector<uchar_vector>& vSignatureRaw,
                                  const TWCoinType& coinNet=TWCoinType::TWCoinTypeBitcoin) = 0;
}; // class BTCTokenInterface end


} // namespace token end
} // namespace jub end

#endif
