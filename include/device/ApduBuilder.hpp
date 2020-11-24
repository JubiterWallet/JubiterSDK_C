#ifndef __ApduBuilder__
#define __ApduBuilder__

#include "utility/Apdu.hpp"

namespace jub {


class ApduBuilder {
public:
    /* functions */
    virtual JUB_RV BuildApdu(const APDU *apdu,
                             std::vector<JUB_BYTE> &vSafeApdu) = 0;
    virtual JUB_RV BuildSafeApdu(const APDU *apdu, std::vector<JUB_BYTE> &vSafeApdu) = 0;
    virtual JUB_RV ParseSafeApduResp(const JUB_BYTE_PTR respData, const JUB_ULONG respDataLen,
                                     JUB_BYTE_PTR retData, JUB_ULONG_PTR pretDataLen,
                                     JUB_UINT16 &wRet) = 0;

    // do something (for example: encrypt data)
    virtual JUB_RV PackData(std::vector<JUB_BYTE> &vDest,
                            const std::vector<JUB_BYTE> &vSrc) = 0;
    // do something (for example: decrypt data)
    virtual JUB_RV UnPackData(std::vector<JUB_BYTE> &vDest,
                              const std::vector<JUB_BYTE> &vSrc) = 0;

    virtual JUB_RV  PlusCMAC(const APDU *apdu, APDU *apduSafe) = 0;
    virtual JUB_RV CheckCMAC(const ResponseAPDU &respApdu) = 0;

    virtual JUB_RV SetSCP03(void *pscp03) = 0;
    virtual JUB_RV SetSCP11(void *pscp11) = 0;

protected:
    virtual JUB_RV IncCounter() = 0;
}; // class ApduBuilder end


class JubApduBuiler
: public ApduBuilder {
public:
    /* functions */
    virtual JUB_RV BuildApdu(const APDU *apdu, std::vector<JUB_BYTE> &vSafeApdu) override;
    virtual JUB_RV BuildSafeApdu(const APDU *apdu, std::vector<JUB_BYTE> &vSafeApdu) override;
    virtual JUB_RV ParseSafeApduResp(const JUB_BYTE_PTR respData, const JUB_ULONG respDataLen,
                                     JUB_BYTE_PTR retData, JUB_ULONG_PTR pretDataLen,
                                     JUB_UINT16 &wRet) override;

    virtual JUB_RV PackData(std::vector<JUB_BYTE> &vDest,
                            const std::vector<JUB_BYTE> &vSrc) override;
    virtual JUB_RV UnPackData(std::vector<JUB_BYTE> &vDest,
                              const std::vector<JUB_BYTE> &vSrc) override;

    virtual JUB_RV  PlusCMAC(const APDU *apdu, APDU *apduSafe) override;
    virtual JUB_RV CheckCMAC(const ResponseAPDU &respApdu) override;

    virtual JUB_RV SetSCP03(void *scp03Ptr) override;
    virtual JUB_RV SetSCP11(void *scp11Ptr) override;

protected:
    virtual JUB_RV IncCounter() override;

private:
    void *_scp03Ptr;
    void *_scp11Ptr;
}; // class JubApduBuiler end


}  // namespace jub end
#endif  // __ApduBuilder__
