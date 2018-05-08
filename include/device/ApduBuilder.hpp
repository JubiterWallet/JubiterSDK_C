
#ifndef __ApduBuilder__
#define __ApduBuilder__

#include <utility/Apdu.hpp>

namespace jub {

class ApduBuilder {
   public:
    /* functions */
    virtual JUB_RV buildApdu(const APDU *apdu,
                             std::vector<JUB_BYTE> &safeApdu) = 0;

    // do something (for example: encrypt data)
    virtual JUB_RV packData(std::vector<JUB_BYTE> &dest,
                            std::vector<JUB_BYTE> &src) = 0;

    // do something (for example: decrypt data)
    virtual JUB_RV unPackData(std::vector<JUB_BYTE> &dest,
                              std::vector<JUB_BYTE> &src) = 0;
};

class JubApudBuiler : public ApduBuilder {
   public:
    /* functions */
    virtual JUB_RV buildApdu(const APDU *apdu, std::vector<JUB_BYTE> &safeApdu);

    virtual JUB_RV packData(std::vector<JUB_BYTE> &dest,
                            std::vector<JUB_BYTE> &src);

    virtual JUB_RV unPackData(std::vector<JUB_BYTE> &dest,
                              std::vector<JUB_BYTE> &src);
};

}  // namespace jub
#endif  // __ApduBuilder__
