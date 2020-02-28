#pragma once
#include <token/JubiterNFC/JubiterNFCToken.h>
#include <token/JubiterNFC/JubiterNFCImpl.h>
#include <token/BTC/JubiterBaseBTCImpl.h>

#include <memory>

namespace jub {
namespace token {


class JubiterNFCBTCImpl :
        public JubiterNFCImpl,
virtual public JubiterBaseBTCImpl {

public:
    JubiterNFCBTCImpl(JUB_UINT16 deviceID) :
    JubiterNFCImpl(deviceID) {};

    virtual ~JubiterNFCBTCImpl() = default;

    virtual JUB_RV SelectApplet();

    //BTC functions
    virtual JUB_RV GetHDNode(const JUB_ENUM_BTC_TRANS_TYPE& type, const std::string& path, std::string& xpub);
    virtual JUB_RV GetAddress(const JUB_BYTE addrFmt, const JUB_ENUM_BTC_TRANS_TYPE& type, const std::string& path, const JUB_UINT16 tag, std::string& address);
    virtual JUB_RV SetUnit(const JUB_ENUM_BTC_UNIT_TYPE& unit);
    virtual JUB_RV SetCoinType(const JUB_ENUM_COINTYPE_BTC& type);
    virtual JUB_RV SignTX(const JUB_BYTE addrFmt,
                          const JUB_ENUM_BTC_TRANS_TYPE& type,
                          const JUB_UINT16 inputCount,
                          const std::vector<JUB_UINT64>& vInputAmount,
                          const std::vector<std::string>& vInputPath,
                          const std::vector<JUB_UINT16>& vChangeIndex,
                          const std::vector<std::string>& vChangePath,
                          const std::vector<JUB_BYTE>& vUnsigedTrans,
                          std::vector<JUB_BYTE>& vRaw);
    virtual JUB_RV VerifyTX(const JUB_ENUM_BTC_TRANS_TYPE& type,
                            const std::vector<JUB_UINT64>& vInputAmount,
                            const std::vector<std::string>& vInputPath,
                            const std::vector<JUB_BYTE>& vSigedTrans);

	//for Factory
	static std::shared_ptr<BTCTokenInterface> Create(JUB_UINT16 deviceID) { return std::make_shared<JubiterNFCBTCImpl>(deviceID); }
}; // class JubiterNFCBTCImpl end


} // namespace token end
} // namespace jub end
