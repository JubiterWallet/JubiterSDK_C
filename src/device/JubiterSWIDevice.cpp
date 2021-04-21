#include "JUB_SDK.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#if defined(SWI_MODE)
#include "utility/util.h"
#include "device/JubiterSWIDevice.hpp"


namespace jub {
namespace device {


JubiterSWIDevice::JubiterSWIDevice() {

}


JubiterSWIDevice::~JubiterSWIDevice() {

}


JUB_RV JubiterSWIDevice::Connect() {

    return JUBR_OK;
}


JUB_RV JubiterSWIDevice::Disconnect() {

    return JUBR_OK;
}


JUB_RV JubiterSWIDevice::SendData(IN JUB_BYTE_CPTR sendData, IN JUB_ULONG ulSendLen,
                                  OUT JUB_BYTE_PTR retData, INOUT JUB_ULONG_PTR pulRetDataLen,
                                  IN JUB_ULONG ulMiliSecondTimeout) {
    return JUBR_OK;
}


JUB_RV JubiterSWIDevice::ImportMnemonic(IN JUB_CHAR_CPTR passphrase, IN JUB_CHAR_CPTR mnemonic) {

    _passphrase = std::string(passphrase);
    _mnemonic   = std::string(mnemonic);

    return JUBR_OK;
}


JUB_RV JubiterSWIDevice::ImportMasterPrivateKey(IN JUB_ENUM_CURVES curve,
                                                IN JUB_CHAR_CPTR masterPrivateKey) {

    _privKeys[curve] = std::string(masterPrivateKey);

    return JUBR_OK;
}


std::string JubiterSWIDevice::getPassphrase() {
    return _passphrase;
}


std::string JubiterSWIDevice::getMnemonic() {
    return _mnemonic;
}


size_t JubiterSWIDevice::privKeySize() {
    return _privKeys.size();
}


std::string JubiterSWIDevice::getPrivKey(const JUB_ENUM_CURVES& curve) {
    return _privKeys[curve];
}


std::string RandomString(int len) {
    std::string str = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::string newstr;

    int pos;
    while(newstr.size() != len) {
        pos = ((rand() % (str.size() - 1)));
        newstr += str.substr(pos,1);
    }

    return newstr;
}


std::string JubiterSWIDevice::randomName() {
    return RandomString(16);
}


JubiterSWIImplDevice::JubiterSWIImplDevice(const std::string& name) {
    _name = name;
}


JubiterSWIImplDevice::~JubiterSWIImplDevice() {

}


std::string JubiterSWIDevice::getName() {
    return _name;
}


}  // namespace device end
}  // namespace jub end

#endif // #if defined(SWI_MODE) end
