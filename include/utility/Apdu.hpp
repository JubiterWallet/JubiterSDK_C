
#ifndef __Apdu__
#define __Apdu__

#include "JUB_SDK.h"

#include <vector>

namespace jub {

class APDU {

public:
    APDU() {
        cla = 0;
        ins = 0;
        p1 = 0;
        p2 = 0;
        lc = 0;
        le = 0;
        data.clear();
    }
    APDU(JUB_ULONG ncla,
         JUB_ULONG nins,
         JUB_ULONG np1,
         JUB_ULONG np2,
         JUB_ULONG nlc,
         const JUB_BYTE* pData = nullptr,
         JUB_ULONG nle = 0) :
        cla(ncla),
        ins(nins),
        p1(np1),
        p2(np2),
        lc(nlc),
        le(nle) {
        if (         0 != lc
            && nullptr != pData
            ) {
            SetData(pData, lc);
        }
        else {
            data.clear();
        }
    }
    ~APDU() {}

public:
    JUB_ULONG cla, ins, p1, p2, lc, le;
    std::vector<JUB_BYTE> data;

public:
    void SetApdu(JUB_ULONG ncla,
                 JUB_ULONG nins,
                 JUB_ULONG np1,
                 JUB_ULONG np2,
                 JUB_ULONG nlc,
                 JUB_BYTE* pData = nullptr,
                 JUB_ULONG nle = 0) {
        cla = ncla;
        ins = nins;
        p1 = np1;
        p2 = np2;
        lc = nlc;
        le = nle;
        if (         0 != lc
            && nullptr != pData
            ) {
            SetData(pData, lc);
        }
        else {
            data.clear();
        }
    }
    void SetData(const JUB_BYTE* pData, JUB_ULONG ulDataSize) {
        // assert(ulDataSize == lc);
        data.clear();
//        data.resize(ulDataSize);
        data.insert(data.begin(), pData, pData + ulDataSize);
    }
}; // class APDU end


class ResponseAPDU {

public:
    ResponseAPDU() {
        raw.clear();
        clear();
    }
    ResponseAPDU(std::vector<JUB_BYTE> raw)
        : raw(raw) {
        clear();
    }
    ResponseAPDU(JUB_BYTE_PTR ret, JUB_ULONG retLen) {
        raw.insert(raw.end(), ret, ret+retLen);
        clear();
    }
    ~ResponseAPDU() {
        raw.clear();
        clear();
    }

    void clear() {
        data.clear();
        cmac.clear();
        SW1 = 0;
        SW2 = 0;
    }
    bool empty() {
        return (   data.empty()
                && cmac.empty()
        );
    }

    bool decode() {
        if (raw.empty()) {
            return false;
        }

        if ( CMAC_LEN  > raw.size()
            && SW_LEN != raw.size()
            ) {
            return false;
        }

        clear();

        // parse SW
        int swIndex1 = (int)raw.size() - SW_LEN;
        int swIndex2 = (int)raw.size() - 1;
        SW1 = raw[swIndex1];
        SW2 = raw[swIndex2];
        if (SW_LEN == raw.size()) {
            return true;
        }

        int cmacIndex = (int)raw.size() - SW_LEN - CMAC_LEN;
        data.insert(data.end(), raw.begin(), raw.begin()+cmacIndex);
        cmac.insert(cmac.end(), raw.begin()+cmacIndex, raw.begin()+swIndex1);

        return true;
    }

public:
    const int CMAC_LEN = 8;
    const int   SW_LEN = 2;

    std::vector<JUB_BYTE> data;
    std::vector<JUB_BYTE> cmac;
    JUB_BYTE SW1;
    JUB_BYTE SW2;

private:
    std::vector<JUB_BYTE> raw;
};  // class ResponseAPDU end


} // namespace jub end

#endif  // __Apdu__
