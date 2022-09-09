#pragma once

#include "JUB_CORE.h"

namespace jub {
namespace token {


inline JUB_RV CosError2SdkError(JUB_UINT32 cosError) {

    return JUBR_UNKNOWN_COS_ERROR + cosError;
}

#define JUB_VERIFY_COS_ERROR(x)         \
do {                                    \
    if (0x9000 != x) {					\
        return CosError2SdkError(x);	\
    }								    \
} while (0)


} // namespace token end
} // namespace jub end
