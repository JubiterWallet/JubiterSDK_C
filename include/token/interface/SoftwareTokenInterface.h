#pragma once
#include "BaseToken.h"

namespace jub {
namespace token {

typedef enum {
    NONE  = 0x00,
    PUBLIC = 0x01,
    PRIVATE = 0x02,
} JUB_SoftwareTokenType;


class SoftwareToken :
virtual public BaseToken {
public:
    virtual ~SoftwareToken() {};
    virtual JUB_SoftwareTokenType Type(){return _type;};
protected:
    JUB_SoftwareTokenType _type{NONE};
}; // class SoftwareToken end


} // namespace token end
} // namespace jub end
