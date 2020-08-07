#pragma once
#include <memory>

#include <token/JubiterNFC/JubiterNFCToken.h>
#include <token/JubiterNFC/JubiterNFCImpl.h>
#include <token/BTC/JubiterNFCBTCImpl.h>
#include <token/BTC/JubiterBaseBCHImpl.h>


namespace jub {
namespace token {


class JubiterNFCBCHImpl :
        public JubiterNFCBTCImpl,
virtual public JubiterBaseBCHImpl {
public:
    //for Factory
    static std::shared_ptr<BaseToken> Create(JUB_UINT16 deviceID) {
        return std::make_shared<JubiterNFCBCHImpl>(deviceID);
    }

public:
    JubiterNFCBCHImpl(JUB_UINT16 deviceID) :
        JubiterNFCBTCImpl(deviceID) {
            _hashType = TWSignatureHashTypeAllFork;
	}
    ~JubiterNFCBCHImpl() {}

protected:
    virtual JUB_RV _SignTx(bool witness,
                           const JUB_ENUM_BTC_TRANS_TYPE& type,
                           const std::vector<JUB_UINT64>& vInputAmount,
                           const std::vector<std::string>& vInputPath,
                           const std::vector<JUB_UINT16>& vChangeIndex,
                           const std::vector<std::string>& vChangePath,
                           const TW::Bitcoin::Transaction& tx,
                           std::vector<TW::Data>& vInputPublicKey,
                           std::vector<uchar_vector>& vSignatureRaw) override;
}; // class JubiterNFCBCHImpl end


} // namespace token end
} // namespace jub end
