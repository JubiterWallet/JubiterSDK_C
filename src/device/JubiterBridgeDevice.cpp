//
//  JubiterBridgeDevice.cpp
//  JubSDK
//
//  Created by Chen ruquan on 2020/12/01.
//  Copyright © 2020 JuBiter. All rights reserved.
//
#include "JUB_SDK.h"

#if defined(SIM_MODE)
#include <memory>
#include <optional>
#include <string>
#include <fstream>
#if defined(_WIN32)
#include <WinSock2.h>
#elif defined(__APPLE__)
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#endif

#include "utility/util.h"
#include "device/JubiterBridgeDevice.hpp"
#include "device/DeviceIOLogHelper.hpp"

#include <google/protobuf/empty.pb.h>
#include "jubiter_bridge.pb.h"

using AppletBridge::Reader;
using AppletBridge::APDURequest;
using AppletBridge::APDUResponse;
using AppletBridge::ListReadersResponse;
using google::protobuf::Empty;


enum struct Operation : std::uint8_t {
    None,
    ListReaders,
    Connect,
    Disconnect,
    SendApdu,
    Dog
};


namespace {
    const unsigned char kMagicCode[] = "Bridge Device";
    const int kMagicCodeLength = sizeof(kMagicCode);
    const int kMaxLength = 1024 * 4;
}


namespace jub {
namespace device {


std::string ReadServerIP(const std::string& ip) {
    std::string address = "127.0.0.1:5001";
    std::ifstream in(ip, std::ifstream::in);
    if (!in) {
        return address;
    }
    in >> address;

    return address;
}


std::map<std::string, uint16_t> ServerIP(const std::string& ip) {
    std::vector<std::string> split = Split(ip, ":");
    if (   2 != split.size()
        || 0 == std::atoi(split[1].c_str())
        ) {
        return std::map<std::string, uint16_t>{};
    }

    return std::map<std::string, uint16_t>{{split[0], std::atoi(split[1].c_str())}};
}


//PCSC device
class JubiterBridgeDevice::Impl final {
public:
    Impl(std::string server, int port) {
        sock_ = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in server_addr;

        // use DNS to get IP address
        struct hostent *hostEntry;
        hostEntry = gethostbyname(server.c_str());
        if (!hostEntry) {
            throw "gethostbyname failed";
        }

        // setup socket address structure
        memset(&server_addr,0,sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        memcpy(&server_addr.sin_addr, hostEntry->h_addr_list[0], hostEntry->h_length);

        connect(sock_, (sockaddr*)&server_addr, sizeof(server_addr));
    }
    ~Impl() {
        shutdown(sock_, SHUT_RDWR);
    }

    std::vector<std::string> ListReaders() {
        // send request
        auto status = SendRequest(Operation::ListReaders, std::nullopt);

        // block read response
        auto respData = ReceiveResponse();
        auto& data = *respData;
        auto resp = std::make_unique<ListReadersResponse>();
        resp->ParseFromArray(data.data(), (int)data.size());
        auto readerCount = resp->readers_size();
        std::vector<std::string> readers;
        for (auto i = 0; i < readerCount; i++) {
            auto& reader = resp->readers(i);
            readers.push_back(reader.name());
        }

        return readers;
    }

    std::optional<unsigned long> Connect(std::string name) {
        Reader reader;
        reader.set_name(name);
        std::vector<unsigned char> requestData;
        requestData.resize(reader.ByteSize());
        reader.SerializePartialToArray(requestData.data(), reader.ByteSize());
        SendRequest(Operation::Connect, requestData);
        auto respData = ReceiveResponse();
        auto& data = *respData;
        reader.ParseFromArray(data.data(), (int)data.size());

        return reader.handle();
    }

    void DisConnect(std::string name, unsigned long handle) {
        if (0 == handle) {
            return;
        }

        Reader reader;
        reader.set_name(name);
        reader.set_handle((unsigned int)handle);
        std::vector<unsigned char> requestData;
        requestData.resize(reader.ByteSize());
        reader.SerializePartialToArray(requestData.data(), reader.ByteSize());
        SendRequest(Operation::Disconnect, requestData);
        ReceiveResponse();
    }

    std::optional<std::vector<unsigned char>> SendApdu(std::string name, unsigned long handle, IN JUB_BYTE_CPTR sendData, IN JUB_ULONG ulSendLen) {
        APDURequest req;
        req.mutable_reader()->set_name(name);
        req.mutable_reader()->set_handle((unsigned int)handle);
        req.set_apdu(sendData, ulSendLen);
        std::vector<unsigned char> requestData;
        requestData.resize(req.ByteSize());
        req.SerializePartialToArray(requestData.data(), req.ByteSize());
        SendRequest(Operation::SendApdu, requestData);
        auto respData = ReceiveResponse();
        auto& data = *respData;

        APDUResponse resp;
        resp.ParseFromArray(data.data(), (int)data.size());
        return std::vector<unsigned char>(resp.resp().begin(), resp.resp().end());
    }

private:
    unsigned long SendRequest(Operation opt, std::optional<std::vector<unsigned char>> param) {
        // request format
        // magic|operation(1 byte)|length(2 bytes)|param(pb format)
        char request[kMaxLength] = { 0 };
        auto p = request;
        std::memcpy(p, kMagicCode, kMagicCodeLength);
        p += kMagicCodeLength;
        *p++ = (char)opt;
        if (param.has_value()) {
            auto v = *param;
            auto l = v.size();
            *p++ = (unsigned char)(l >> 8);
            *p++ = (unsigned char)(l);
            std::memcpy(p, v.data(), l);
            p += l;
        }
        else {
            *p++ = 0;
            *p++ = 0;
        }

        return send(sock_, request, p - request, 0);
    }

    std::optional<std::vector<unsigned char>> ReceiveResponse() {
        // response format
        // magic|length(2 bytes)|response(pb format)
        char response[kMaxLength] = {0,};
        auto p = response;
        bool checked = false;
        do {
            auto length = recv(sock_, p, kMaxLength, 0);
            // If the connection has been gracefully closed, the return value is zero
            if (0 == length) {
                return std::nullopt;
            }

            // Otherwise, a value of SOCKET_ERROR is returned
            if (-1 == length) {
                return std::nullopt;
            }

            // magic|length(2 bytes)
            if (length < kMagicCodeLength + 2) {
                p += length;
                continue;
            }

            if (!checked
                && 0 != std::memcmp(kMagicCode, response, kMagicCodeLength)
                ) {
                return std::nullopt;
            }
            checked = true;

            p += length;
            auto l = ResponseLength(response);
            if (p - response >= l + kMagicCodeLength + 2) {
                break;
            }
        } while (true);

        auto l = ResponseLength(response);
        // some operation not have response
        if (l == 0) {
            return std::nullopt;
        }

        // if have response remove magic|length(2 bytes)
        p = response;
        p += kMagicCodeLength;
        p += 2;
        return std::vector<unsigned char>(p, p + l);
    }

    int ResponseLength(char* p) {

        int l = p[kMagicCodeLength];
        l <<= 8;
        l += p[kMagicCodeLength + 1];
        return l;
    }

private:
    int sock_;
};


JubiterBridgeDevice::JubiterBridgeDevice(const std::string& ip, const std::string& name) : ip_(ip), name_(name) {
    std::map<std::string, uint16_t> pair = ServerIP(ReadServerIP(ip));
    for (const auto& [key, value] : pair) {
        impl_ = new JubiterBridgeDevice::Impl(key, value);
        break;
    }
    handle_ = 0;
}


JubiterBridgeDevice::~JubiterBridgeDevice() {
    delete impl_;
}


std::vector<std::string> JubiterBridgeDevice::EnumDevice(const std::string& ip) {

    std::vector<std::string> list;

    std::map<std::string, uint16_t> pair = ServerIP(ReadServerIP(ip));
    for (const auto& [key, value] : pair) {
        auto impl = std::make_unique<JubiterBridgeDevice::Impl>(key, value);
        std::vector<std::string> l = impl->ListReaders();
        list.insert(list.end(), l.begin(), l.end());
    }

    return list;
}


JUB_RV JubiterBridgeDevice::Connect() {
    auto handle = impl_->Connect(name_);
    if (!handle) {
        return -1;
    }

    handle_ = handle.value_or(0);
    return 0;
}


JUB_RV JubiterBridgeDevice::Disconnect() {
    impl_->DisConnect(name_, handle_);
    return 0;
}


JUB_RV JubiterBridgeDevice::SendData(IN JUB_BYTE_CPTR sendData, IN JUB_ULONG ulSendLen, OUT JUB_BYTE_PTR retData, INOUT JUB_ULONG_PTR pulRetDataLen, IN JUB_ULONG ulMiliSecondTimeout) {
    auto resp = impl_->SendApdu(name_, handle_, sendData, ulSendLen);
    if (!resp) {
        return -1;
    }

    *pulRetDataLen = resp->size();
    std::copy(resp->begin(), resp->end(), retData);
    return 0;
}


unsigned int JubiterBridgeLITEDevice::Initialize(const LITE_DEVICE_INIT_PARAM& params) {

    if (!_scp11.empty()) {
        return JUBR_OK;
    }

    return SetSCP11Param(params.crt,
                         params.sk,
                         params.hostID,
                         params.keyLength);
}


unsigned int JubiterBridgeLITEDevice::SetSCP11Param(const std::string& crt,
                                                    const std::string& rk,
                                                    const std::string& hostID,
                                                    const uint8_t keyLength) {

    scp11_crt oceCrt = scp11_crt(uchar_vector(crt));
    if (!oceCrt.decode()) {
        return JUBR_ARGUMENTS_BAD;
    }

    uchar_vector kl;
    kl.push_back(keyLength);
    _scp11 = scp11c(scp11_sharedInfo(uchar_vector("1107"),
                                     uchar_vector("3C"),
                                     uchar_vector("88"),
                                     kl,
                                     uchar_vector(hostID)),
                    oceCrt,
                    uchar_vector(rk));
    if (_scp11.empty()) {
        return JUBR_ARGUMENTS_BAD;
    }

    _scp03.resetCounter();

    return JUBR_OK;
}


}  // namespace device end
}  // namespace jub end

#endif  // #if defined(SIM_MODE) end
