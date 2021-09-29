#ifndef __JubiterSWIDevice__
#define __JubiterSWIDevice__

#include "device/DeviceTypeBase.hpp"

#if defined(SWI_MODE)
#include <vector>


namespace jub {
namespace device {


class JubiterSWIDevice
    : public DeviceTypeBase {
public:
    JubiterSWIDevice();
    ~JubiterSWIDevice();

public:
    virtual JUB_RV Connect() override;
    virtual JUB_RV Disconnect() override;
    virtual JUB_RV IsConnect() override;

    virtual JUB_RV SendData(IN JUB_BYTE_CPTR sendData, IN JUB_ULONG ulSendLen,
                            OUT JUB_BYTE_PTR retData, INOUT JUB_ULONG_PTR pulRetDataLen,
                            IN JUB_ULONG ulMiliSecondTimeout = 1200000) override;

    virtual JUB_RV ImportMnemonic(IN JUB_CHAR_CPTR passphrase, IN JUB_CHAR_CPTR mnemonic);
    virtual JUB_RV ImportMasterPrivateKey(IN JUB_ENUM_CURVES curve,
                                          IN JUB_CHAR_CPTR masterPrivateKey);

    static std::string randomName();
    virtual std::string getName();

    virtual std::string getPassphrase();
    virtual std::string getMnemonic();
    virtual size_t privKeySize();
    virtual std::string getPrivKey(const JUB_ENUM_CURVES& curve);

protected:
    std::string _name;

    std::string _passphrase{""};
    std::string _mnemonic{""};

    std::map<JUB_ENUM_CURVES, std::string> _privKeys;
}; // class JubiterSWIDevice end


class JubiterSWIImplDevice
    : public JubiterSWIDevice {
public:
    static DeviceTypeBase* Create(const JUB_ENUM_DEVICE& type, std::shared_ptr<device::DeviceTypeBase> devicePtr) {

        switch (type) {
        case JUB_ENUM_DEVICE::VD:
            return new JubiterSWIImplDevice(std::dynamic_pointer_cast<device::JubiterSWIImplDevice>(devicePtr)->getName());
        default:
            break;
        }   // switch (type) end

        return nullptr;
    }

public:
    JubiterSWIImplDevice(const std::string& name);
    ~JubiterSWIImplDevice();

protected:
    unsigned long _handle;
}; // class JubiterSWIImplDevice end


} // namespace device end
} // namespace jub end


#endif  // #if defined(SWI_MODE) end
#endif  // __JubiterSWIDevice__
