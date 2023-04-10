#pragma once
#ifndef __ExtMutiSigContextBTC__
#define __ExtMutiSigContextBTC__

#include "JUB_SDK_BTC.h"
#include <map>
#include <vector>
#include <string>
#include <TrezorCrypto/bip32.h>
#include "mSIGNA/stdutils/uchar_vector.h"
#include <TrezorCrypto/curves.h>

namespace jub {
namespace context {

//input index, cosigner signature list
#define MULTISIG_COSIGNER_MAX_CNT 15
typedef std::map<std::string, std::vector<std::string>> MAP_COSIGNER_SIGNATURE;
class BTCContextMultiSigExt {
public:
    BTCContextMultiSigExt() {}
    BTCContextMultiSigExt(CONTEXT_CONFIG_MULTISIG_EXT cfg) {
        _m = cfg.m;
        _n = cfg.n;
        _cosignerIndex = -1;
        _orderedIndex = -1;

//        _transType = p2sh_multisig;

        if (   MULTISIG_COSIGNER_MAX_CNT <= cfg.cosignerCnt
            || MULTISIG_COSIGNER_MAX_CNT <  _m
            || MULTISIG_COSIGNER_MAX_CNT <  _n
            ) {
            _isValid = false;
            return;
        }
        else {
            _isValid = true;
        }

        for (unsigned long i=0; i<cfg.cosignerCnt; ++i) {
            HDNode hdkey;
            uint32_t parentFingerprint;
            if (0 != hdnode_deserialize((const char *)cfg.vCosignerMainXpub[i],
                                        _xpubPrefix, _xprvPrefix,
                                        _curveName.c_str(),
                                        &hdkey, &parentFingerprint)) {
                _isValid = false;
                break;
            }

            uchar_vector mainPubKey(hdkey.public_key, sizeof(hdkey.public_key)/sizeof(uint8_t));
            std::string hexMainPubKey = mainPubKey.getHex();
            _vCosignerMainPubKey.push_back(hexMainPubKey);
            _mapCosignerMainKey.insert({hexMainPubKey, cfg.vCosignerMainXpub[i]});
        }
        if (cfg.coSignedTx) setIncTx(cfg.coSignedTx);
    }

protected:
    void sort() {
        std::sort(_vCosignerMainPubKey.begin(), _vCosignerMainPubKey.end());
        std::vector<std::string>::iterator pos = std::unique(_vCosignerMainPubKey.begin(), _vCosignerMainPubKey.end());
        _vCosignerMainPubKey.erase(pos, _vCosignerMainPubKey.end());

        pos = std::find(_vCosignerMainPubKey.begin(), _vCosignerMainPubKey.end(), _mainPubKey);
        if (-1 == _cosignerIndex) {
            _cosignerIndex = pos - _vCosignerMainPubKey.begin();
        }
    }

    void addMyMainXPub(const std::string mainXpub) {
        HDNode hdkey;
        uint32_t parentFingerprint;
        if (0 != hdnode_deserialize(mainXpub.c_str(),
                                    _xpubPrefix, _xprvPrefix,
                                    _curveName.c_str(),
                                    &hdkey, &parentFingerprint)) {
            return ;
        }

        uchar_vector mainPubKey(hdkey.public_key, sizeof(hdkey.public_key)/sizeof(uint8_t));
        _mainPubKey = mainPubKey.getHex();
        _vCosignerMainPubKey.push_back(_mainPubKey);
        _mapCosignerMainKey.insert({_mainPubKey, mainXpub});
    }

    void order(std::string mainXpub) {
        addMyMainXPub(mainXpub);
        sort();
    }

    void setIncTx(std::string hexIncTx) {
        _hexIncTx = hexIncTx;
    }

public:
    virtual JUB_RV GetAddress(std::shared_ptr<token::BaseToken> tokenPtr,
                              const std::string _mainPath,
                              const JUB_ENUM_BTC_ADDRESS_FORMAT& addrFmt, const JUB_ENUM_BTC_TRANS_TYPE _transType, const BIP44_Path& path, const JUB_UINT16 tag, std::string& address,
                              const TWCoinType& _coinNet=TWCoinType::TWCoinTypeBitcoin);
    virtual JUB_RV SignTX(std::shared_ptr<token::BaseToken> tokenPtr,
                          const std::string _mainPath, const JUB_ENUM_COINTYPE_BTC _coinType,
                          const JUB_ENUM_BTC_ADDRESS_FORMAT& addrFmt, const JUB_ENUM_BTC_TRANS_TYPE _transType, const JUB_UINT32 version, const std::vector<INPUT_BTC>& vInputs, const std::vector<OUTPUT_BTC>& vOutputs, const JUB_UINT32 lockTime, std::string& raw,
                          const TWCoinType& _coinNet=TWCoinType::TWCoinTypeBitcoin);

    virtual JUB_RV DeserializeTx(std::shared_ptr<token::BaseToken> tokenPtr,
                                 const std::vector<uchar_vector>& vRedeemScript,
                                 const std::string& hexIncTx,
                                 MAP_COSIGNER_SIGNATURE& mapCosignerSignature,
                                 uchar_vector& unsignedTx);
    virtual JUB_RV DeserializeTx(std::shared_ptr<token::BaseToken> tokenPtr,
                                 const std::string& hexTx,
                                 std::vector<INPUT_BTC>& vInputs,
                                 std::vector<OUTPUT_BTC>& vOutputs,
                                 JUB_UINT32_PTR plockTime);

    virtual JUB_RV PreparatoryFlow(const std::string mainXpub);
    virtual JUB_RV ParseTransaction(std::shared_ptr<token::BaseToken> tokenPtr,
                                    const std::string& incRaw,
                                    std::vector<INPUT_BTC>& vInputs,
                                    std::vector<OUTPUT_BTC>& vOutputs,
                                    JUB_UINT32_PTR plockTime);

    virtual std::string _FullBip44Path(const std::string _mainPath,
                                       const BIP44_Path& path);

protected:
    JUB_RV SerializeUnsignedTx(const JUB_ENUM_BTC_TRANS_TYPE type,
                               const JUB_UINT32 version,
                               const std::vector<INPUT_BTC>& vInputs,
                               const std::vector<OUTPUT_BTC>& vOutputs,
                               const JUB_UINT32 lockTime,
                               uchar_vector& unsignedRaw);

    JUB_RV pushAll(const JUB_ENUM_COINTYPE_BTC _coinType, std::vector<uchar_vector>& results);
    JUB_RV pushAll(const JUB_ENUM_COINTYPE_BTC _coinType, uchar_vector& result);
    JUB_RV pullAll(const JUB_ENUM_COINTYPE_BTC _coinType, uchar_vector& result);
    JUB_RV OrderDHPublicKey(const std::string _mainPath,
                            const BIP44_Path& path,
                            std::vector<uchar_vector>& vOrderedPublickey);

    JUB_RV OrderSignatures(std::shared_ptr<token::BaseToken> tokenPtr,
                           const std::string _mainPath,
                           const JUB_ENUM_COINTYPE_BTC _coinType,
                           const JUB_ENUM_BTC_TRANS_TYPE _transType,
                           const JUB_UINT32 version,
                           const std::vector<INPUT_BTC>& vInputs, const std::vector<uchar_vector>& vRedeemScript,
                           const std::vector<OUTPUT_BTC>& vOutputs,
                           const JUB_UINT32 lockTime,
                           const MAP_COSIGNER_SIGNATURE& mapCosignerSignature,
                           const std::vector<uchar_vector>& vSignatureRaw,
                           MAP_COSIGNER_SIGNATURE& mapSignatureOrdered);

    virtual JUB_RV BuildRedeemScript(const std::string _mainPath,
                                     const BIP44_Path& path,
                                     uchar_vector& redeemScript);
    virtual JUB_RV BuildRedeemScriptTlv(const std::string _mainPath,
                                        const BIP44_Path& path,
                                        uchar_vector& redeemScriptTlv);

private:
    std::string getMainPath(const std::string _mainPath,
                            long cosignerIndex, bool isChange) {
        int change = 0;
        if (isChange) {
            change = 1;
        }
        return (_mainPath + "/" + std::to_string(cosignerIndex) + "/" + std::to_string(change));
    }

    long getCosignerIndex() {
        return _cosignerIndex;
    }

    void setOrderedIndex(long orderedIndex) {
        _orderedIndex = orderedIndex;
    }

    long getOrderedIndex() {
        return _orderedIndex;
    }

    std::string getCosignerMainXpub(long cosignerIndex) {
        try {
            if (_vCosignerMainPubKey.size() <= cosignerIndex) {
                return "";
            }
            std::string cosignerMainPubKey = _vCosignerMainPubKey.at(cosignerIndex);
            return getCosignerMainXpub(cosignerMainPubKey);
        }
        catch (...) {
            return "";
        }
    }

    std::string getCosignerMainXpub(std::string mainPubKey) {
        auto search = _mapCosignerMainKey.find(mainPubKey);
        if (search != _mapCosignerMainKey.end()) {
            return search->second;
        }
        else {
            return "";
        }
    }

    std::string getCosignerMainXpub() {
        return getCosignerMainXpub(_mainPubKey);
    }

    std::string getCosignerFullBip44Path(const std::string _mainPath, const BIP44_Path& path, const long cosignerIndex);

protected:
    unsigned long _m;
    unsigned long _n;
    long _cosignerIndex;
    long _orderedIndex;
    std::string _mainPubKey;
    std::vector<std::string> _vCosignerMainPubKey;
    //hex main public key, xpub
    std::map<std::string, std::string> _mapCosignerMainKey;
    std::string _hexIncTx;
    bool _isValid;

public:
    std::string _curveName={SECP256K1_NAME};
    JUB_UINT32 _xpubPrefix={0x0488B21E};//{bitcoinXPUB};
    JUB_UINT32 _xprvPrefix={0x0488ADE4};//{bitcoinXPRV};
}; // class BTCContextMultiSigExt end

} // namespace context end
} // namespace jub end

#endif // #pragma once
