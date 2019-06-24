#include <token/JubiterBLDImpl.h>

namespace jub {

constexpr JUB_BYTE PKIAID_ETH[16] = {
    0xD1, 0x56, 0x00, 0x01, 0x32, 0x83, 0x00, 0x42, 0x4C, 0x44, 0x00, 0x00, 0x45, 0x54, 0x48, 0x01
};

#define SWITCH_TO_ETH_APP                       \
do {				                            \
    JUB_VERIFY_RV(_selectApp(PKIAID_ETH,16));   \
} while (0);                                    \

JUB_RV JubiterBLDImpl::selectApplet_ETH() {
    SWITCH_TO_ETH_APP;
    return JUBR_OK;
}

JUB_RV JubiterBLDImpl::getAddress_ETH(std::string path, JUB_UINT16 tag, std::string& address) {

    uchar_vector data(path.begin(), path.end());

    APDU apdu(0x00, 0xf6, 0x00, (JUB_BYTE)tag, data.size(), data.data(), 0x14);
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[2048] = {0,};
    JUB_ULONG retLen = sizeof(retData)/sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_sendApdu(&apdu, ret, retData, &retLen));
    if (0x9000 != ret) {
        return JUBR_TRANSMIT_DEVICE_ERROR;
    }

    uchar_vector v_address(retData, (unsigned int)retLen);
    address = "0x" + v_address.getHex();

    return JUBR_OK;
}

JUB_RV JubiterBLDImpl::getHDNode_ETH(JUB_BYTE format, std::string path, std::string& pubkey) {

    //path = "m/44'/60'/0'";
    uchar_vector vPath;
    vPath << path;
    uchar_vector apduData = toTlv(0x08, vPath);

    //0x00 for hex,0x01 for xpub
    if (    JUB_ETH_PUB_FORMAT::HEX  != format
        &&  JUB_ETH_PUB_FORMAT::XPUB != format
        ) {
        return JUBR_ERROR_ARGS;
    }

    APDU apdu(0x00, 0xe6, 0x00, format, apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[2048] = {0,};
    JUB_ULONG retLen = sizeof(retData)/sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_sendApdu(&apdu, ret, retData, &retLen));
    if (0x9000 != ret) {
        return JUBR_TRANSMIT_DEVICE_ERROR;
    }

    if (0x00 == format) {
        uchar_vector v_pubkey(retData, (unsigned int)retLen);
        pubkey = "0x" + v_pubkey.getHex();
    }
    else if (0x01 == format) {
        pubkey = (JUB_CHAR_PTR)retData;
    }

    return JUBR_OK;
}

JUB_RV JubiterBLDImpl::signTX_ETH(bool Is_ERC20,
                                  std::vector<JUB_BYTE> nonce,
                                  std::vector<JUB_BYTE> gasPrice,
                                  std::vector<JUB_BYTE> gasLimit,
                                  std::vector<JUB_BYTE> To,
                                  std::vector<JUB_BYTE> value,
                                  std::vector<JUB_BYTE> input,
                                  std::vector<JUB_BYTE> path,
                                  std::vector<JUB_BYTE> chainID,
                                  std::vector<JUB_BYTE>& raw) {

    uchar_vector data;

    if (0x00 == nonce[0]) {
        data << (JUB_BYTE)0x41;
        data << (JUB_BYTE)0x00;
    }
    else {
        data << toTlv(0x41, nonce);
    }

    data << toTlv(0x42, gasPrice);
    data << toTlv(0x43, gasLimit);
    data << toTlv(0x44, To);
    data << toTlv(0x45, value);
    data << toTlv(0x46, input);
    data << toTlv(0x47, path);
    data << toTlv(0x48, chainID);

    JUB_BYTE ins = 0x2a;
    if (Is_ERC20) {
        ins = 0xc8;
    }

    //one pack can do it
    APDU apdu(0x00, ins, 0x01, 0x00, data.size(), data.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[2048] = {0,};
    JUB_ULONG retLen = sizeof(retData)/sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_sendApdu(&apdu, ret, retData, &retLen));
    if (0x9000 != ret) {
        return JUBR_TRANSMIT_DEVICE_ERROR;
    }

    raw.clear();
    raw.insert(raw.end(), retData, retData + retLen);

    return JUBR_OK;
}

} // namespace jub end
