#pragma once
#include "JUB_SDK.h"
#include <assert.h>
#include <TrezorCrypto/curves.h>
#include <TrustWalletCore/TWCoinType.h>
#include "mSIGNA/stdutils/uchar_vector.h"


//extern struct stVersion;
struct stVersionExp : stVersion {

    friend bool operator<(const stVersionExp& lhs, const stVersionExp& rhs) {
//        return  (lhs.major  < rhs.major)
//            || ((lhs.major == rhs.major) && (lhs.minor  < rhs.minor))
//            || ((lhs.major == rhs.major) && (lhs.minor == rhs.minor) && (lhs.patch < rhs.patch));
        return std::tie(lhs.major, lhs.minor, lhs.patch) < std::tie(rhs.major, rhs.minor, rhs.patch);
    }

    friend bool operator>(const stVersionExp& lhs, const stVersionExp& rhs) {
//        return  (lhs.major  > rhs.major)
//            || ((lhs.major == rhs.major) && (lhs.minor  > rhs.minor))
//            || ((lhs.major == rhs.major) && (lhs.minor == rhs.minor) && (lhs.patch > rhs.patch));
        return std::tie(lhs.major, lhs.minor, lhs.patch) > std::tie(rhs.major, rhs.minor, rhs.patch);
    }

    friend bool operator==(const stVersionExp& lhs, const stVersionExp& rhs) {
//        return lhs.major == rhs.major
//            && lhs.minor == rhs.minor
//            && lhs.patch == rhs.patch;
        return std::tie(lhs.major, lhs.minor, lhs.patch) == std::tie(rhs.major, rhs.minor, rhs.patch);
    }

    friend bool operator>=(const stVersionExp& lhs, const stVersionExp& rhs) {
        return !(lhs < rhs);
    }

    static stVersionExp FromString(std::string versionStr) {
        stVersionExp v;
        // versionStr is hex string of 4 bytes
        assert(versionStr.size() == 8);
        // decode hex string to bytes
        uchar_vector versionBytes(versionStr);

        v.major = versionBytes[0];
        v.minor = versionBytes[1];

        if (1 == v.major) {
            // 1.x version update logic is different
            v.patch = versionBytes[2]*10 + versionBytes[3];
        } else {
            // other version logic
            v.patch = versionBytes[2]*256 + versionBytes[3];
        }

        return v;
    }
};


namespace jub {
namespace token {


class BaseToken {
public:
    virtual ~BaseToken() {}

protected:
    TWCoinType _coin;
    char *_curve_name = (char*)SECP256K1_NAME;
    TWPublicKeyType _publicKeyType = TWPublicKeyType::TWPublicKeyTypeSECP256k1;
}; // class BaseToken end


} // namespace token end
} // namespace jub end
