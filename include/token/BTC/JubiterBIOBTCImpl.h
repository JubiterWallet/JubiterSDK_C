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
    //for Factory
    static std::shared_ptr<BaseToken> Create(const JUB_UINT16 deviceID) {
        return std::make_shared<JubiterBIOBTCImpl>(deviceID);
    }

public:
    JubiterBIOBTCImpl(const JUB_UINT16 deviceID) :
        JubiterBIOToken(deviceID),
        JubiterBladeBTCImpl(deviceID) {}
    virtual ~JubiterBIOBTCImpl() = default;
}; // class JubiterBIOBTCImpl end


class JubiterBIOBCHImpl :
        public JubiterBIOToken,
virtual public JubiterBladeBCHImpl {
public:
    //for Factory
    static std::shared_ptr<BaseToken> Create(const JUB_UINT16 deviceID) {
        return std::make_shared<JubiterBIOBCHImpl>(deviceID);
    }

public:
    JubiterBIOBCHImpl(const JUB_UINT16 deviceID) :
        JubiterBIOToken(deviceID),
        JubiterBladeBCHImpl(deviceID) {}
    virtual ~JubiterBIOBCHImpl() = default;
}; // class JubiterBIOBCHImpl end


class JubiterBIOLTCImpl :
        public JubiterBIOToken,
virtual public JubiterBladeLTCImpl {
public:
    //for Factory
    static std::shared_ptr<BaseToken> Create(const JUB_UINT16 deviceID) {
        return std::make_shared<JubiterBIOLTCImpl>(deviceID);
    }

public:
    JubiterBIOLTCImpl(const JUB_UINT16 deviceID) :
        JubiterBIOToken(deviceID),
        JubiterBladeLTCImpl(deviceID) {}
    virtual ~JubiterBIOLTCImpl() = default;
}; // class JubiterBIOLTCImpl end


class JubiterBIOUSDTImpl :
        public JubiterBIOToken,
virtual public JubiterBladeUSDTImpl {
public:
    //for Factory
    static std::shared_ptr<BaseToken> Create(const JUB_UINT16 deviceID) {
        return std::make_shared<JubiterBIOUSDTImpl>(deviceID);
    }

public:
    JubiterBIOUSDTImpl(const JUB_UINT16 deviceID) :
        JubiterBIOToken(deviceID),
        JubiterBladeUSDTImpl(deviceID) {}
    virtual ~JubiterBIOUSDTImpl() = default;
}; // class JubiterBIOUSDTImpl end


class JubiterBIODashImpl :
        public JubiterBIOToken,
virtual public JubiterBladeDashImpl {
public:
    //for Factory
    static std::shared_ptr<BaseToken> Create(const JUB_UINT16 deviceID) {
        return std::make_shared<JubiterBIODashImpl>(deviceID);
    }

public:
    JubiterBIODashImpl(const JUB_UINT16 deviceID) :
        JubiterBIOToken(deviceID),
        JubiterBladeDashImpl(deviceID) {}
    virtual ~JubiterBIODashImpl() = default;
}; // class JubiterBIODashImpl end


class JubiterBIOQTUMImpl :
        public JubiterBIOToken,
virtual public JubiterBladeQTUMImpl {
public:
    //for Factory
    static std::shared_ptr<BaseToken> Create(const JUB_UINT16 deviceID) {
        return std::make_shared<JubiterBIOQTUMImpl>(deviceID);
    }

public:
    JubiterBIOQTUMImpl(const JUB_UINT16 deviceID) :
        JubiterBIOToken(deviceID),
        JubiterBladeQTUMImpl(deviceID) {}
    virtual ~JubiterBIOQTUMImpl() = default;
}; // class JubiterBIOQTUMImpl end


} // namespace token end
} // namespace jub end
