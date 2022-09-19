#pragma once
#include <memory>

#include "JubiterBladeBCHImpl.h"
#include "JubiterBladeBTCImpl.h"
#include "JubiterBladeDashImpl.h"
#include "JubiterBladeLTCImpl.h"
#include "JubiterBladeQTUMImpl.h"
#include "JubiterBladeUSDTImpl.h"

#include "token/JubiterBIO/JubiterBIOToken.h"
#include "token/JubiterBlade/JubiterBladeToken.h"

namespace jub {
namespace token {

class JubiterBIOBTCImpl : public JubiterBIOToken, public JubiterBladeBTCImpl {
  public:
    JubiterBIOBTCImpl(const JUB_UINT16 deviceID)
        : JubiterBIOToken(deviceID), JubiterBladeBTCImpl(deviceID), JubiterBladeToken(deviceID) {}
    virtual ~JubiterBIOBTCImpl() = default;
}; // class JubiterBIOBTCImpl end

class JubiterBIOBCHImpl : public JubiterBIOToken, public JubiterBladeBCHImpl {
  public:
    JubiterBIOBCHImpl(const JUB_UINT16 deviceID)
        : JubiterBIOToken(deviceID), JubiterBladeBCHImpl(deviceID), JubiterBladeToken(deviceID) {}
    virtual ~JubiterBIOBCHImpl() = default;
}; // class JubiterBIOBCHImpl end

class JubiterBIOLTCImpl : public JubiterBIOToken, public JubiterBladeLTCImpl {
  public:
    JubiterBIOLTCImpl(const JUB_UINT16 deviceID)
        : JubiterBIOToken(deviceID), JubiterBladeLTCImpl(deviceID), JubiterBladeToken(deviceID) {}
    virtual ~JubiterBIOLTCImpl() = default;
}; // class JubiterBIOLTCImpl end

class JubiterBIOUSDTImpl : public JubiterBIOToken, public JubiterBladeUSDTImpl {
  public:
    JubiterBIOUSDTImpl(const JUB_UINT16 deviceID)
        : JubiterBIOToken(deviceID), JubiterBladeUSDTImpl(deviceID), JubiterBladeToken(deviceID) {}
    virtual ~JubiterBIOUSDTImpl() = default;
}; // class JubiterBIOUSDTImpl end

class JubiterBIODashImpl : public JubiterBIOToken, public JubiterBladeDashImpl {
  public:
    JubiterBIODashImpl(const JUB_UINT16 deviceID)
        : JubiterBIOToken(deviceID), JubiterBladeDashImpl(deviceID), JubiterBladeToken(deviceID) {}
    virtual ~JubiterBIODashImpl() = default;
}; // class JubiterBIODashImpl end

class JubiterBIOQTUMImpl : public JubiterBIOToken, public JubiterBladeQTUMImpl {
  public:
    JubiterBIOQTUMImpl(const JUB_UINT16 deviceID)
        : JubiterBIOToken(deviceID), JubiterBladeQTUMImpl(deviceID), JubiterBladeToken(deviceID) {}
    virtual ~JubiterBIOQTUMImpl() = default;
}; // class JubiterBIOQTUMImpl end

} // namespace token
} // namespace jub
