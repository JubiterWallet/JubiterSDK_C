#pragma once
#include "token/JubiterLite/JubiterLiteToken.h"
#include "token/JubiterLite/JubiterLiteImpl.h"
#include "token/SOL/JubiterBaseSOLImpl.h"

#include <memory>

namespace jub {
namespace token {


class JubiterLiteSOLImpl :
        public JubiterLiteImpl,
virtual public JubiterBaseSOLImpl {
public:
    JubiterLiteSOLImpl(JUB_UINT16 deviceID) :
        JubiterLiteImpl(deviceID) {}
    ~JubiterLiteSOLImpl() {}

    //SOL functions
    virtual JUB_RV SelectApplet() override;
    virtual JUB_RV GetAppletVersion(stVersion& version) override;

    virtual JUB_RV GetAddress(const std::string& path, const JUB_UINT16 tag, std::string& address) override;
    virtual JUB_RV GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey) override;

protected:
}; // class JubiterLiteSOLImpl end


} // namespace token end
} // namespace jub end
