#ifndef __JubiterBLEDevice__
#define __JubiterBLEDevice__


#include "device/DeviceTypeBase.hpp"

#if defined(BLE_MODE)
#include "bleTransmit/bleTransmit.h"


#define PREFIX_BLD "JuBiter"
#define PREFIX_BIO "JuBio"


namespace jub {
namespace device {


class JubiterBLEDevice :
public DeviceTypeBase {
public:
    JubiterBLEDevice();
    ~JubiterBLEDevice();

public:
    // for common device
    virtual JUB_RV Connect() override;
    virtual JUB_RV Disconnect() override;

    virtual JUB_RV SendData(IN JUB_BYTE_CPTR sendData, IN JUB_ULONG ulSendLen,
                            OUT JUB_BYTE_PTR retData, INOUT JUB_ULONG_PTR pulRetDataLen,
                            IN JUB_ULONG ulMiliSecondTimeout = 1200000) override;

public:
    // for ble device
    virtual unsigned int Initialize(const BLE_INIT_PARAM& params);
    virtual unsigned int Finalize();

    virtual unsigned int Scan();
    virtual unsigned int StopScan();

    virtual unsigned int Connect(unsigned char* devName,    /**< ble device name>*/
                                 unsigned char* bBLEUUID,   /**< ble device UUID */
                                 unsigned int connectType,  /**< ble device connect type */
                                 unsigned long* pdevHandle, /**< output ble device connect handle */
                                 unsigned int timeout);

    virtual unsigned int CancelConnect(unsigned char* devName, unsigned char* bBLEUUID);

    virtual unsigned int Disconnect(unsigned long handle);
    virtual unsigned int IsConnect(unsigned long handle);
    virtual unsigned int SetAlertMessage(unsigned long handle, unsigned char* data);

    virtual unsigned long GetHandle();
    virtual void SetHandle(unsigned long handle);

    virtual void SetConnectStatuteFalse();

    BLE_INIT_PARAM outerParams;

protected:
    static int BLE_ReadCallBack(unsigned long devHandle,
                                unsigned char* data, unsigned int uiDataLen);

    static void BLE_ScanCallBack(unsigned char* devName,
                                 unsigned char* uuid,
                                 unsigned int type);

    static void BLE_DiscCallBack(unsigned char* uuid);

    JUB_RV MatchErrorCode(unsigned long error);

    // check ble version, and set ble library reconnect flag
    void ExtraSetting();

protected:
    /* data */
    BLE_INIT_PARAM _param;
    unsigned long _handle;
    bool _bConnected;
}; // class JubiterBLEDevice end


class JubiterBLEBLDDevice :
public JubiterBLEDevice {
public:
    //for Factory
    static std::shared_ptr<JubiterBLEDevice> Create(const std::string& name) {
        if (0 != name.find(PREFIX_BLD)) {
            return std::make_shared<JubiterBLEBLDDevice>();
        }

        return nullptr;
    }
    static DeviceTypeBase* Create(const JUB_ENUM_DEVICE& type, std::shared_ptr<device::DeviceTypeBase> devicePtr) {

        switch (type) {
        case JUB_ENUM_DEVICE::BLADE:
//            return new JubiterBLEBLDDevice();
            return Singleton<JubiterBLEBLDDevice>::GetInstance();
        default:
            break;
        }   // switch (type) end

        return nullptr;
    }

public:
    JubiterBLEBLDDevice() :
        JubiterBLEDevice() {}
    ~JubiterBLEBLDDevice() {}
};  // class JubiterBLEBLDDevice end


class JubiterBLEBIODevice :
public JubiterBLEDevice {
public:
    //for Factory
    static std::shared_ptr<JubiterBLEDevice> Create(const std::string& name) {
        if (   std::string::npos != name.find(PREFIX_BIO)
            || std::string::npos != name.find("hpy2")
            || std::string::npos != name.find("HPYG2")
            ) {
            return std::make_shared<JubiterBLEBIODevice>();
        }

        return nullptr;
    }
    static DeviceTypeBase* Create(const JUB_ENUM_DEVICE& type, std::shared_ptr<device::DeviceTypeBase> devicePtr) {

        switch (type) {
        case JUB_ENUM_DEVICE::BIO:
//            return new JubiterBLEBIODevice();
            return Singleton<JubiterBLEBIODevice>::GetInstance();
        default:
            break;
        }   // switch (type) end

        return nullptr;
    }

public:
    JubiterBLEBIODevice() :
        JubiterBLEDevice() {}
    ~JubiterBLEBIODevice() {}
};  // class JubiterBIOBLDDevice end


} // namespace device end
} // namespace jub end

#endif  // #if defined(BLE_MODE) end
#endif  // __JubiterBLEDevice__
