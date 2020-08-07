#pragma once
#include <memory>

#include <token/JubiterBlade/JubiterBladeToken.h>
#include <device/ApduBuilder.hpp>
#include "utility/util.h"


namespace jub {
namespace token {


class JubiterBIOToken :
    public JubiterBladeToken {
public:
    JubiterBIOToken(JUB_UINT16 deviceID);
    virtual ~JubiterBIOToken() = default;

    virtual bool   IsBootLoader() override;

    virtual JUB_RV UIShowMain() override;

    virtual JUB_RV IdentityVerify(IN JUB_BYTE mode, OUT JUB_ULONG &retry) override;
    virtual JUB_RV IdentityVerifyPIN(IN JUB_BYTE mode, IN const std::string &pinMix, OUT JUB_ULONG &retry) override;
    virtual JUB_RV IdentityNineGrids(IN bool bShow) override;

    virtual JUB_RV VerifyFingerprint(OUT JUB_ULONG &retry) override;
    virtual JUB_RV VerifyFgptForIntl(OUT JUB_ULONG &retry) override;
    virtual JUB_RV EnrollFingerprint(INOUT JUB_BYTE_PTR fgptIndex, OUT JUB_ULONG_PTR ptimes,
                                     OUT JUB_BYTE_PTR fgptID) override;
    virtual JUB_RV EnumFingerprint(std::string& fgptList) override;
    virtual JUB_RV EraseFingerprint() override;
    virtual JUB_RV DeleteFingerprint(JUB_BYTE fgptID) override;
}; // class JubiterBIOToken end


} // namespace token end
} // namespace jub end
