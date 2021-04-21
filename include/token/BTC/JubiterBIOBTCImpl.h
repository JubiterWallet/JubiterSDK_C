#pragma once
#include <memory>

#include "token/JubiterBIO/JubiterBIOToken.h"
#include "token/BTC/JubiterBladeBTCImpl.h"


namespace jub {
namespace token {


class JubiterBIOBTCImpl :
        public JubiterBIOToken,
        public JubiterBladeBTCImpl {
public:
    JubiterBIOBTCImpl(JUB_UINT16 deviceID) :
        JubiterBIOToken(deviceID),
        JubiterBladeBTCImpl(deviceID) {}
    virtual ~JubiterBIOBTCImpl() = default;
}; // class JubiterBIOBTCImpl end


class JubiterBIOBCHImpl :
        public JubiterBIOToken,
virtual public JubiterBladeBCHImpl {
public:
    JubiterBIOBCHImpl(JUB_UINT16 deviceID) :
        JubiterBIOToken(deviceID),
        JubiterBladeBCHImpl(deviceID) {}
    virtual ~JubiterBIOBCHImpl() = default;
}; // class JubiterBIOBCHImpl end


class JubiterBIOLTCImpl :
        public JubiterBIOToken,
virtual public JubiterBladeLTCImpl {
public:
    JubiterBIOLTCImpl(JUB_UINT16 deviceID) :
        JubiterBIOToken(deviceID),
        JubiterBladeLTCImpl(deviceID) {}
    virtual ~JubiterBIOLTCImpl() = default;
}; // class JubiterBIOLTCImpl end


class JubiterBIOUSDTImpl :
        public JubiterBIOToken,
virtual public JubiterBladeUSDTImpl {
public:
    JubiterBIOUSDTImpl(JUB_UINT16 deviceID) :
        JubiterBIOToken(deviceID),
        JubiterBladeUSDTImpl(deviceID) {}
    virtual ~JubiterBIOUSDTImpl() = default;
}; // class JubiterBIOUSDTImpl end


class JubiterBIODashImpl :
        public JubiterBIOToken,
virtual public JubiterBladeDashImpl {
public:
    JubiterBIODashImpl(JUB_UINT16 deviceID) :
        JubiterBIOToken(deviceID),
        JubiterBladeDashImpl(deviceID) {}
    virtual ~JubiterBIODashImpl() = default;
}; // class JubiterBIODashImpl end


class JubiterBIOQTUMImpl :
        public JubiterBIOToken,
virtual public JubiterBladeQTUMImpl {
public:
    JubiterBIOQTUMImpl(JUB_UINT16 deviceID) :
        JubiterBIOToken(deviceID),
        JubiterBladeQTUMImpl(deviceID) {}
    virtual ~JubiterBIOQTUMImpl() = default;
}; // class JubiterBIOQTUMImpl end


} // namespace token end
} // namespace jub end
