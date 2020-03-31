#pragma once
#include <token/JubiterNFC/JubiterNFCToken.h>
#include <memory>

namespace jub {
namespace token {


class JubiterNFCImpl :
    public JubiterNFCToken {

public:
    JubiterNFCImpl(JUB_UINT16 deviceID) :
    JubiterNFCToken(deviceID) {
	};

    virtual ~JubiterNFCImpl() = default;

    virtual JUB_RV SelectApplet();
    virtual JUB_RV GetHDNode(const JUB_BYTE& type, const std::string& path, std::string& xpub);
    virtual JUB_RV GetCompPubKey(const JUB_BYTE& type, const std::string& path, TW::Data& pubkey);
    virtual JUB_RV GetAddress(const JUB_BYTE addrFmt, const JUB_ENUM_BTC_TRANS_TYPE& type, const std::string& path, const JUB_UINT16 tag, std::string& address);
    virtual JUB_RV SignTX(const JUB_UINT16 inputCount,
                          const std::vector<std::string>& vInputPath,
                          const JUB_BYTE& signType,
                          const JUB_BYTE& hashType,
                          const std::vector<TW::Data>& vPreImageHash,
                          std::vector<TW::Data>& vRSV);
    virtual JUB_RV SignTXPack(const JUB_UINT16 inputCount,
                              const std::vector<std::string>& vInputPath,
                              const JUB_BYTE& signType,
                              const JUB_BYTE& hashType,
                              const std::vector<TW::Data>& vPreImageHash,
                              std::vector<TW::Data>& vRSV);
    virtual JUB_RV SignOne(const JUB_UINT16 inputCount,
                           const JUB_UINT16 inputIndex,
                           const std::string& path,
                           const JUB_BYTE& signType,
                           const JUB_BYTE& hashType,
                           const TW::Data& preImageHash,
                           TW::Data& rsv);

protected:
    JUB_BYTE _getSignType(const char *curve_name);
    JUB_BYTE _getHalfHasher(const HasherType hasherType, TW::Hash::Hasher& halfHasher);
}; // class JubiterNFCImpl end


} // namespace token end
} // namespace jub end
