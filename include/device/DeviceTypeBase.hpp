
#ifndef __DeviceTypeBase__
#define __DeviceTypeBase__

#include <string>
#include <JUB_SDK.h>

namespace jub {

class DeviceTypeBase
{
public:
    /* functions */
    virtual JUB_RV connect(const std::string path ) = 0;
    virtual JUB_RV disconnect() = 0;

    virtual JUB_RV sendData(IN JUB_BYTE_CPTR sendData, IN JUB_ULONG sendLen, 
                            OUT JUB_BYTE_PTR pRetData, INOUT JUB_ULONG_PTR pulRetLen, 
                            IN JUB_ULONG ulMiliSecondTimeout = 120000) = 0;
};

#define FT3KHN_READWRITE_SIZE_ONCE_NEW 4096
}

#endif // __DeviceTypeBase__
