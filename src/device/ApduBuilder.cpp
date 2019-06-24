#include <device/ApduBuilder.hpp>

namespace jub {

JUB_RV JubApudBuiler::buildApdu(const APDU *apdu,
                                std::vector<JUB_BYTE> &safeApdu) {
    // copy header
    safeApdu.push_back((JUB_BYTE)apdu->cla);
    safeApdu.push_back((JUB_BYTE)apdu->ins);
    safeApdu.push_back((JUB_BYTE)apdu->p1);
    safeApdu.push_back((JUB_BYTE)apdu->p2);

    if (0 != apdu->lc) {
        // get lc,data
        if (0xFF < apdu->lc) {
            safeApdu.push_back((JUB_BYTE)( (apdu->lc) / 0x10000));
            safeApdu.push_back((JUB_BYTE)(((apdu->lc) / 0x100) % 0x100));
            safeApdu.push_back((JUB_BYTE)( (apdu->lc) % 0x100));
            safeApdu.insert(safeApdu.end(), apdu->data.begin(),
                            apdu->data.begin() + apdu->lc);
        }
        else {
            safeApdu.push_back((JUB_BYTE)(apdu->lc));
            safeApdu.insert(safeApdu.end(), apdu->data.begin(),
                            apdu->data.begin() + apdu->lc);
        }
    }

    if (0 != apdu->le) {
        if (0xFF < apdu->le) {
            safeApdu.push_back((JUB_BYTE)( (apdu->le) / 0x10000));
            safeApdu.push_back((JUB_BYTE)(((apdu->le) / 0x100) % 0x100));
            safeApdu.push_back((JUB_BYTE)( (apdu->le) % 0x100));
        }
        else {
            safeApdu.push_back((JUB_BYTE)(apdu->le));
        }
    }

    return JUBR_OK;
}

JUB_RV JubApudBuiler::packData(std::vector<JUB_BYTE> &dest,
                               std::vector<JUB_BYTE> &src) {

	dest = src;
    return JUBR_OK;
}

JUB_RV JubApudBuiler::unPackData(std::vector<JUB_BYTE> &dest,
                                 std::vector<JUB_BYTE> &src) {
	dest = src;
    return JUBR_OK;
}

}  // namespace jub end
