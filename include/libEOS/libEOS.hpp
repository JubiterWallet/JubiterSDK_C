#pragma once

#ifndef __libEOS__
#define __libEOS__

#include "JUB_SDK_EOS.h"
#include "context/ContextEOS.h"

#include "mSIGNA/stdutils/uchar_vector.h"

namespace jub {

namespace eos {

JUB_RV serializePreimage(const std::string& expiration,
                         const std::string& referenceBlockId,
                         const std::string& referenceBlockTime,
                         const std::string& currency,
                         const std::string& from,
                         const std::string& to,
                         const std::string& asset,
                         const std::string& memo,
                         uchar_vector& preimageRaw);

} // namespace eos end

} // namespace jub end

#endif
