#pragma once
#include "HDKey/HDKey.hpp"
#include "JUB_SDK.h"
#include "mSIGNA/stdutils/uchar_vector.h"
#include <TrezorCrypto/curves.h>
#include <TrustWallet/wallet-core/src/BinaryCoding.h>
#include <TrustWalletCore/TWCoinType.h>
#include <assert.h>
#include <cstring>
#include <string>

// extern struct stVersion;
struct stVersionExp : stVersion {
    stVersionExp() {
        major = 0;
        minor = 0;
        patch = 0;
    }
    stVersionExp(JUB_UINT8 _major, JUB_UINT8 _minor, JUB_UINT16 _patch) {
        major = _major;
        minor = _minor;
        patch = _patch;
    }

    friend bool operator<(const stVersionExp &lhs, const stVersionExp &rhs) {
        //        return  (lhs.major  < rhs.major)
        //            || ((lhs.major == rhs.major) && (lhs.minor  < rhs.minor))
        //            || ((lhs.major == rhs.major) && (lhs.minor == rhs.minor) && (lhs.patch < rhs.patch));
        return std::tie(lhs.major, lhs.minor, lhs.patch) < std::tie(rhs.major, rhs.minor, rhs.patch);
    }

    friend bool operator>(const stVersionExp &lhs, const stVersionExp &rhs) {
        //        return  (lhs.major  > rhs.major)
        //            || ((lhs.major == rhs.major) && (lhs.minor  > rhs.minor))
        //            || ((lhs.major == rhs.major) && (lhs.minor == rhs.minor) && (lhs.patch > rhs.patch));
        return std::tie(lhs.major, lhs.minor, lhs.patch) > std::tie(rhs.major, rhs.minor, rhs.patch);
    }

    friend bool operator==(const stVersionExp &lhs, const stVersionExp &rhs) {
        //        return lhs.major == rhs.major
        //            && lhs.minor == rhs.minor
        //            && lhs.patch == rhs.patch;
        return std::tie(lhs.major, lhs.minor, lhs.patch) == std::tie(rhs.major, rhs.minor, rhs.patch);
    }

    friend bool operator>=(const stVersionExp &lhs, const stVersionExp &rhs) { return !(lhs < rhs); }

    friend bool operator!=(const stVersionExp &lhs, const stVersionExp &rhs) { return !(lhs == rhs); }

    static stVersionExp zeroVersion() {
        stVersionExp v;
        v.major = 0;
        v.minor = 0;
        v.patch = 0;
        return v;
    }

    static stVersionExp FromHex(const std::string &versionStr) {
        stVersionExp v;
        // versionStr is hex string of 4 bytes
        assert(versionStr.size() == 8);
        // decode hex string to bytes
        uchar_vector versionBytes(versionStr);

        v.major = versionBytes[0];
        v.minor = versionBytes[1];

        if (1 == v.major) {
            // 1.x version update logic is different
            v.patch = versionBytes[2] * 10 + versionBytes[3];
        } else {
            // other version logic
            v.patch = versionBytes[2] * 256 + versionBytes[3];
        }

        return v;
    }

    static std::string ToHex(const stVersionExp &version) {
        std::string str = "";
        uchar_vector versionBytes(4);
        versionBytes[0] = version.major;
        versionBytes[1] = version.minor;
        uchar_vector vPatch;
        TW::encode16BE(version.patch, vPatch);
        memcpy(&versionBytes[2], &vPatch[0], vPatch.size());
        str = versionBytes.getHex();
        return str;
    }

    static std::string ToString(const stVersionExp &version) {
        std::string str = "";
        str += std::to_string(version.major);
        str += ".";
        str += std::to_string(version.minor);
        str += ".";
        str += std::to_string(version.patch);
        return str;
    }
};

namespace jub {
namespace token {

class BaseToken {
  public:
    virtual ~BaseToken() {}

  protected:
    TWCoinType _coin;
    char *_curve_name              = (char *)SECP256K1_NAME;
    TWPublicKeyType _publicKeyType = TWPublicKeyType::TWPublicKeyTypeSECP256k1;
}; // class BaseToken end

} // namespace token
} // namespace jub
