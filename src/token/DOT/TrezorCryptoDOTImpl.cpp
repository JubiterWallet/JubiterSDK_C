#include "token/DOT/TrezorCryptoDOTImpl.h"

#include <TrezorCrypto/bip32.h>
#include <TrezorCrypto/curves.h>
#include "HDKey/HDKey.hpp"

#include <Polkadot/Signer.h>
#include "uint256.h"
#include <Polkadot/Address.h>
#include <Polkadot/Extrinsic.h>
#include <Kusama/Address.h>
extern "C" {
#include "sr25519.h"
}
namespace jub {
namespace token {


JUB_RV TrezorCryptoDOTImpl::SelectApplet() {

    return JUBR_OK;
}


JUB_RV TrezorCryptoDOTImpl::SetCoin() {

    return JUBR_OK;
}

JUB_RV TrezorCryptoDOTImpl::SetCoinCurvePublicKeyType(JUB_ENUM_COINTYPE_DOT coinType, JUB_ENUM_CURVES curve) {
    
    _curve = curve;
    _coin = coinType == JUB_ENUM_COINTYPE_DOT::COINDOT ? TWCoinType::TWCoinTypePolkadot : TWCoinType::TWCoinTypeKusama;
    _publicKeyType = _curve == JUB_ENUM_CURVES::ED25519 ?  TWPublicKeyType::TWPublicKeyTypeED25519 : TWPublicKeyType::TWPublicKeyTypeCURVE25519;
    switch (curve) {
        case JUB_ENUM_CURVES::SECP256K1:
            _curve_name = (char *)SECP256K1_NAME;
            break;
        case JUB_ENUM_CURVES::SR25519:
            ///noting
            break;
        case JUB_ENUM_CURVES::ED25519:
            _curve_name = (char *)ED25519_NAME;
            break;
            
        default:
            break;
    }
    
    return JUBR_OK;
}


JUB_RV TrezorCryptoDOTImpl::GetAddress(const std::string& path, const JUB_UINT16 tag, std::string& address) {
    
    std::string pubStr = _MasterKey_XPUB;
    std::string derivPrv;
    std::string derivpub;
    if (JUB_ENUM_CURVES::SR25519 == _curve && !path.empty()) {
        JUB_VERIFY_RV(_getSr25519KeypairFromMasterKp(_MasterKey_XPRV, derivPrv, derivpub, path, _curve));
        pubStr = derivpub;
    }
    else if (JUB_ENUM_CURVES::ED25519 == _curve && !path.empty()) {
        JUB_VERIFY_RV(_getEd25519PrvKeyFromMasterKey(_MasterKey_XPRV, derivPrv, derivpub, path, _curve));
        pubStr = derivpub;
    }
    uchar_vector pub(pubStr);
    TW::Data publicKey(pub);
    return _getAddress(publicKey, address);
}


JUB_RV TrezorCryptoDOTImpl::GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey) {
    
    if (JUB_ENUM_CURVES::SR25519 == _curve && !path.empty()) {
        std::string derivPrv;
        std::string derivpub;
        JUB_VERIFY_RV(_getSr25519KeypairFromMasterKp(_MasterKey_XPRV, derivPrv, derivpub, path, _curve));
        pubkey = derivpub;
        
    } else if (JUB_ENUM_CURVES::ED25519 == _curve && !path.empty()) {
        std::string derivPrv;
        std::string derivpub;
        JUB_VERIFY_RV(_getEd25519PrvKeyFromMasterKey(_MasterKey_XPRV, derivPrv, derivpub, path, _curve));
        pubkey = derivpub;
        
    } else {
        pubkey = _MasterKey_XPUB;
    }

    return JUBR_OK;
}


JUB_RV TrezorCryptoDOTImpl::SignTX(const std::string &path,
                                   const std::string &genesisHash,
                                   const std::string &blockHash,
                                   const std::string &to,
                                   const uint64_t& nonce,
                                   const uint32_t& specVersion,
                                   const uint64_t& network,
                                   const uint32_t& transaction_version,
                                   const uint64_t& blockNumber,
                                   const std::string& value,
                                   const uint64_t& eraPeriod,
                                   const std::string& tip,
                                   std::vector<JUB_BYTE>& vSignatureRaw) {
    try {
        TW::SS58Address toAddress;
        if (TW::Polkadot::Address::isValid(to)) {
            toAddress = TW::Polkadot::Address(to);
        } else if (TW::Kusama::Address::isValid(to)) {
            toAddress = TW::Kusama::Address(to);
        } else {
            return JUBR_ERROR_ARGS;
        }
        
        std::string prv = _MasterKey_XPRV.substr(0,SR25519_SECRET_SIZE * 2);
        std::string pub = _MasterKey_XPUB;

        if (JUB_ENUM_CURVES::SR25519 == _curve && !path.empty()) {
            std::string derivPrv;
            std::string derivpub;
            JUB_VERIFY_RV(_getSr25519KeypairFromMasterKp(_MasterKey_XPRV, derivPrv, derivpub, path, _curve));
            prv = derivPrv.substr(0,SR25519_SECRET_SIZE * 2);
            pub = derivpub;
            
        } else if (JUB_ENUM_CURVES::ED25519 == _curve && !path.empty()) {
            std::string derivPrv;
            std::string derivpub;
            JUB_VERIFY_RV(_getEd25519PrvKeyFromMasterKey(_MasterKey_XPRV, derivPrv, derivpub, path, _curve));
            prv = derivPrv;
            pub = derivpub;
        }
        
        uchar_vector privateData(prv);
        uchar_vector vector_pub(pub);
        auto genesis = uchar_vector(genesisHash);
        auto bHash = uchar_vector(blockHash);
        
        TWSS58AddressType netWork = network == 0 ? TWSS58AddressTypePolkadot : TWSS58AddressTypeKusama;
        
        TW::Polkadot::Extrinsic extrinsic = TW::Polkadot::Extrinsic(bHash, genesis, nonce, specVersion, transaction_version, value, toAddress, "", netWork, blockNumber, eraPeriod);
        TW::Data preimage = extrinsic.encodePayload();
        std::cout << "preimage:" << uchar_vector(preimage).getHex() << std::endl;
        
        if (JUB_ENUM_CURVES::ED25519 == _curve) {
            vSignatureRaw = TW::Polkadot::Signer::sign(privateData,extrinsic);
        } else if (JUB_ENUM_CURVES::SR25519 == _curve) {
            std::vector<uint8_t> sig(SR25519_SIGNATURE_SIZE, 0);
            sr25519_sign(sig.data(), vector_pub.data(), privateData.data(), preimage.data(), sizeof(preimage));
            TW::Data pubKey = uchar_vector(vector_pub);
            vSignatureRaw = extrinsic.encodeSignature(pubKey, sig, TWCurveSR25519);
        }
    }
    catch (...) {
        return JUBR_ERROR_ARGS;
    }

    return JUBR_OK;
}


} // namespace token end
} // namespace jub end
