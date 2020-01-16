#pragma once

#ifndef __libEOS__
#define __libEOS__

#include "JUB_SDK_EOS.h"

#include "mSIGNA/stdutils/uchar_vector.h"

namespace jub {
namespace eos {


JUB_RV serializePreimage(const std::string& expiration,
                         const std::string& referenceBlockId,
                         const std::string& referenceBlockTime,
                         const std::string& actionsInJSON,
                         uchar_vector& preimageRaw,
                         const bool bWithType=false);

JUB_RV serializeActions(const JUB_ACTION_EOS_PTR actions,
                        JUB_UINT16 actionCount,
                        std::string& actionInJSON);


} // namespace eos end
} // namespace jub end

#endif
