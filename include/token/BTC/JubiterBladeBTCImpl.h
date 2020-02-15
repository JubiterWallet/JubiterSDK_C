#pragma once
#include <token/JubiterBlade/JubiterBladeToken.h>
#include <token/BTC/JubiterBaseBTCImpl.h>

#include <memory>

namespace jub {
namespace token {


constexpr JUB_BYTE kPKIAID_BTC[16] = {
    0xD1, 0x56, 0x00, 0x01, 0x32, 0x83, 0x00, 0x42, 0x4C, 0x44, 0x00, 0x00, 0x42, 0x54, 0x43, 0x01
};


class JubiterBladeBTCImpl :
        public JubiterBladeToken,
virtual public JubiterBaseBTCImpl {

public:
    JubiterBladeBTCImpl(JUB_UINT16 deviceID) :
    JubiterBladeToken(deviceID) {
		_coin = TWCoinType::TWCoinTypeBitcoin;
	};

    virtual ~JubiterBladeBTCImpl() = default;

    //BTC functions
    virtual JUB_RV SelectApplet();
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
	static std::shared_ptr<BTCTokenInterface> Create(JUB_UINT16 deviceID) { return std::make_shared<JubiterBladeBTCImpl>(deviceID); }

protected:
    //BTC functions
    bool _isSupportLegacyAddress();
    JUB_BYTE _RealAddressFormat(const JUB_ULONG& addrFmt);
    JUB_BYTE _RealAddressFormat(const JUB_ENUM_BTC_ADDRESS_FORMAT& addrFmt);
}; // class JubiterBladeBTCImpl end


} // namespace token end
} // namespace jub end
