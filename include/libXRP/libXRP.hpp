#pragma once

#ifndef __libXRP__
#define __libXRP__

#include "JUB_SDK_XRP.h"
#include "mSIGNA/stdutils/uchar_vector.h"

namespace jub {
namespace xrp {


JUB_RV serializePreimage(const JUB_TX_XRP& tx,
                         uchar_vector& preimageRaw);


} // namespace xrp end
} // namespace jub end

#endif
