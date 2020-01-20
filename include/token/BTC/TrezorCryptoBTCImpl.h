#pragma once
#include <token/TrezorCrypto/TrezorCryptoToken.h>
#include <token/interface/BTCTokenInterface.hpp>
#include <TrustWalletCore/TWCoinType.h>

#include <memory>

namespace jub {
namespace token {


class TrezorCryptoBTCImpl :
        public TrezorCryptoToken,
virtual public BTCTokenInterface {

public:
    TrezorCryptoBTCImpl(const std::string& xprv) :
        TrezorCryptoToken(xprv) {
            _coin = TWCoinType::TWCoinTypeBitcoin;
        };
    ~TrezorCryptoBTCImpl() {};

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

    //QTUM functions
    virtual JUB_RV SetQRC20ETHToken(const std::string& tokenName,
                                    const JUB_UINT16 unitDP,
                                    const std::string& contractAddress);

	//for Factory
	static std::shared_ptr<BTCTokenInterface> Create(const std::string& xprv) { return std::make_shared<TrezorCryptoBTCImpl>(xprv); }

protected:
    TWCoinType _coin;
}; // class TrezorCryptoBTCImpl end


} // namespace token end
} // namespace jub end
