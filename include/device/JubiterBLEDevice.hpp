#ifndef __JubiterBLEDevice__
#define __JubiterBLEDevice__

#include <bleTransmit/bleTransmit.h>
#include <device/DeviceTypeBase.hpp>

namespace jub {

class JubiterBLEDevice : public DeviceTypeBase {
   public:
    JubiterBLEDevice();
    ~JubiterBLEDevice();

   public:
	// for common device
    virtual JUB_RV connect(const std::string path);
    virtual JUB_RV disconnect();

    virtual JUB_RV sendData(IN JUB_BYTE_CPTR sendData, IN JUB_ULONG sendLen,
                            OUT JUB_BYTE_PTR pRetData,
                            INOUT JUB_ULONG_PTR pulRetLen,
                            IN JUB_ULONG ulMiliSecondTimeout = 1200000);

   public:
	// for ble device
    virtual unsigned int initialize(const BLE_INIT_PARAM& params);
    virtual unsigned int finalize();

    virtual unsigned int scan();
    virtual unsigned int stopScan();

    virtual unsigned int connect(
        unsigned char* bBLEUUID,   /**< ble device UUID */
        unsigned int connectType,  /**< ble device connect type */
        unsigned long* pDevHandle, /**< output ble device connect handle */
        unsigned int timeout);

    virtual unsigned int cancelConnect(unsigned char* bBLEUUID);

    virtual unsigned int disconnect(unsigned long handle);
    virtual unsigned int isConnect(unsigned long handle);

    virtual unsigned long getHandle();
    virtual void setHandle(unsigned long handle);

	BLE_INIT_PARAM outerParams;

   protected:
    static int BLE_ReadCallBack(unsigned long devHandle, unsigned char* data,
                                unsigned int dataLen);

	static void BLE_ScanCallBack(unsigned char* devName, unsigned char* uuid,
                                 unsigned int type);

    static void BLE_DiscCallBack(unsigned char* uuid);

	static JubiterBLEDevice* getThis();

   protected:
    /* data */
    BLE_INIT_PARAM _param;
    unsigned long _handle;

};

}  // namespace jub

#endif  // __JubiterBLEDevice__
