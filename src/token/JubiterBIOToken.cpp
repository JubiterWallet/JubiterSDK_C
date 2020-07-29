#include <token/JubiterBIO/JubiterBIOToken.h>
#include <device/DeviceTypeBase.hpp>
#include <device/JubiterHidDevice.hpp>
#include <utility/util.h>
#include <token/ErrorHandler.h>
#include "tlv.hpp"


namespace jub {
namespace token {


JubiterBIOToken::JubiterBIOToken(JUB_UINT16 deviceID)
    : JubiterBladeToken(deviceID) {

}


bool JubiterBIOToken::IsBootLoader() {

    APDU apdu(0x00, 0xa4, 0x04, 0x00, 0x00);
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = {0,};
    JUB_ULONG ulRetDataLen = sizeof(retData)/sizeof(JUB_BYTE);
    auto rv = _SendApdu(&apdu, ret, retData, &ulRetDataLen);
    if (  JUBR_OK == rv
        && 0x9000 == ret
        ) {
        return true;
    }

    return false;
}


JUB_RV JubiterBIOToken::VerifyFingerprint(OUT JUB_ULONG &retry) {

    APDU apdu(0x00, 0x20, 0x04, 0x00, 0x00);
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    if (0x6985 == ret) { //locked
        JUB_VERIFY_RV(JUBR_PIN_LOCKED);
    }
    if (0x63C0 == (ret & 0xfff0)) {
        retry = (ret & 0xf);
        JUB_VERIFY_RV(JUBR_DEVICE_PIN_ERROR);
    }
    JUB_VERIFY_COS_ERROR(ret);

    return JUBR_OK;
}


JUB_RV JubiterBIOToken::EnrollFingerprint(INOUT JUB_BYTE_PTR fgptIndex, OUT JUB_ULONG_PTR ptimes,
                                          OUT JUB_BYTE_PTR fgptID) {

    uchar_vector apduData = tlv_buf(0xDFFE, tlv_buf(0x8210, *fgptIndex).encode()).encode();
    APDU apdu(0x80, 0xCB, 0x80, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = {0,};
    JUB_ULONG ulRetDataLen = sizeof(retData)/sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    if (0x9000 != ret) {
        return JUBR_ERROR;
    }

    TW::Data v;
    size_t vSize = tlv_buf::decodeTV(0x8210, TW::Data(retData, retData + ulRetDataLen), v);
    if (!(v.size() == vSize
          &&     3 == vSize)
        ) {
        return JUBR_ERROR;
    }

    *fgptID = v[0];     // Modality ID.
    *fgptIndex = v[1];  // The number of times of current fingerprint modality, start from 1.
    *ptimes = v[2];     // The total times that current fingerprint needs.

    return JUBR_OK;
}


JUB_RV JubiterBIOToken::EnumFingerprint(std::string& fgptList) {

    uchar_vector apduData = tlv_buf(0xDFFF, tlv_buf(0x8211).encode()).encode();
    APDU apdu(0x80, 0xCB, 0x80, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = {0,};
    JUB_ULONG ulRetDataLen = sizeof(retData)/sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    if (0x9000 != ret) {
        return JUBR_ERROR;
    }

    // (  Enroll FP  )     (   Enum FP     )           (binary)    (ID list)
    //     fgptID
    //      |  |           00000000 00000000     0..56 00000000     null
    //-------------------------------------------------------------------------------------
    // 8210 |00| xx 04     00000000 00000001     0..56 00000001     0
    // 8210 |01| xx 04     00000000 00000003     0..56 00000011     0, 1
    // 8210 |02| xx 04     00000000 00000007     0..56 00000111     0, 1, 2
    // 8210 |03| xx 04     00000000 0000000f     0..56 00001111     0, 1, 2, 3
    //-------------------------------------------------------------------------------------
    // 8210 |04| xx 04     00000000 0000001f     0..56 00011111     0, 1, 2, 3, 4
    // 8210 |05| xx 04     00000000 0000003f     0..56 00111111     0, 1, 2, 3, 4, 5
    // 8210 |06| xx 04     00000000 0000007f     0..56 01111111     0, 1, 2, 3, 4, 5, 6
    // 8210 |07| xx 04     00000000 000000ff     0..56 11111111     0, 1, 2, 3, 4, 5, 6, 7
    //-------------------------------------------------------------------------------------
    // delete   02
    //  07 dffe 04 8213 01 |02|
    //          02         00000000 000000fd     0..56 11111011     0, 1, _, 3, 4, 5, 6, 7
    uchar_vector vBitmap(retData, retData + ulRetDataLen);
    vBitmap.reverse();

    std::vector<uint16_t> vFgpt;
    for (uint16_t i=0; i<vBitmap.size(); ++i) {
        for (uint16_t shift=0; shift<2; ++shift) {
            uint8_t t = vBitmap[i];
            t >>= (shift*4);
            t  &= 0x0F;
            std::vector<uint8_t> v;
            decToBinary(t, v);
            for (uint16_t bit=0; bit<v.size(); ++bit) {
                if (v[bit]) {
                    vFgpt.push_back((shift*4 + bit)+(i*8));
                }
            }
        }
    }

    for (auto fgpt : vFgpt) {
        std::ostringstream o;
        o << fgpt;
        fgptList += o.str();
        fgptList += " ";
    }

    return JUBR_OK;
}


JUB_RV JubiterBIOToken::EraseFingerprint() {

    uchar_vector apduData = tlv_buf(0xDFFE, tlv_buf(0x8212).encode()).encode();
    APDU apdu(0x80, 0xCB, 0x80, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = {0,};
    JUB_ULONG ulRetDataLen = sizeof(retData)/sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    if (0x9000 == ret) {
        return JUBR_OK;
    }

    JUB_VERIFY_RV(JUBR_ERROR);

    return JUBR_OK;
}


JUB_RV JubiterBIOToken::DeleteFingerprint(JUB_BYTE fgptID) {

    uchar_vector apduData = tlv_buf(0xDFFE, tlv_buf(0x8213, fgptID).encode()).encode();
    APDU apdu(0x80, 0xCB, 0x80, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = {0,};
    JUB_ULONG ulRetDataLen = sizeof(retData)/sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    if (0x9000 == ret) {
        return JUBR_OK;
    }

    JUB_VERIFY_RV(JUBR_ERROR);

    return JUBR_OK;
}


} // namespace token end
} // namespace jub end
