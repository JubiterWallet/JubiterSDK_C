//
//  JubiterBridgeDevice.cpp
//  JubSDK
//
//  Created by Chen ruquan on 2020/12/01.
//  Copyright © 2020 JuBiter. All rights reserved.
//
#include "JUB_SDK.h"

#if defined(GRPC_MODE)
#include <memory>
#include <string>
#include <fstream>

#include "utility/util.h"
#include "device/JubiterBridgeDevice.hpp"
#include "device/DeviceIOLogHelper.hpp"

#include <grpcpp/grpcpp.h>
#include <google/protobuf/empty.pb.h>
#include "jubiter_bridge.pb.h"
#include "jubiter_bridge.grpc.pb.h"

using AppletBridge::Reader;
using AppletBridge::APDURequest;
using AppletBridge::APDUResponse;
using AppletBridge::ListReadersResponse;
using grpc::ClientContext;
using google::protobuf::Empty;

namespace jub {
namespace device {


std::string ServerAddress(const std::string& ip) {
    std::string address = "127.0.0.1:5001";
    std::ifstream in(ip, std::ifstream::in);
    if (!in) {
        return address;
    }
    in >> address;

    return address;
}


class JubiterBridgeDevice::Impl final {
private:
    std::unique_ptr<AppletBridge::AppletBridgeServer::Stub> stub_;

public:
    Impl(std::string server) {
        auto channel = grpc::CreateChannel(server, grpc::InsecureChannelCredentials());
        auto stub = AppletBridge::AppletBridgeServer::NewStub(channel);
        stub_.swap(stub);
    }

    std::vector<std::string> ListReaders() {
        Empty empty;
        ListReadersResponse resp;
        ClientContext ctx;
        auto status = stub_->ListReaders(&ctx, Empty(), &resp);
        auto readerCount = resp.readers_size();
        std::vector<std::string> readers;
        for (auto i = 0; i < readerCount; i++) {
            auto& reader = resp.readers(i);
            readers.push_back(reader.name());
        }

        return readers;
    }

    std::optional<unsigned long> Connect(std::string name) {
        Reader reader;
        reader.set_name(name);
        ClientContext ctx;
        auto status = stub_->Connect(&ctx, reader, &reader);
        if (!status.ok()) {
            return std::nullopt;
        }
        return reader.handle();
    }

    void DisConnect(std::string name, unsigned long handle) {
        if (handle == 0) {
            return;
        }
        Reader reader;
        reader.set_name(name);
        reader.set_handle((unsigned int)handle);
        Empty empty;
        ClientContext ctx;
        stub_->Disconnect(&ctx, reader, &empty);
    }

    std::optional<std::vector<unsigned char>> SendApdu(std::string name, unsigned long handle, IN JUB_BYTE_CPTR sendData, IN JUB_ULONG ulSendLen) {
        APDURequest req;
        req.mutable_reader()->set_name(name);
        req.mutable_reader()->set_handle((unsigned int)handle);
        req.set_apdu(sendData, ulSendLen);
        APDUResponse resp;
        ClientContext ctx;
        auto status = stub_->SendApdu(&ctx, req, &resp);
        if (!status.ok()) {
            return std::nullopt;
        }

        return std::vector<unsigned char>(resp.resp().begin(), resp.resp().end());
    }
};


JubiterBridgeDevice::JubiterBridgeDevice(const std::string& ip, const std::string& name) : ip_(ip), name_(name) {
    auto address = ServerAddress(ip);
    impl_ = new JubiterBridgeDevice::Impl(address);
    handle_ = 0;
}


JubiterBridgeDevice::~JubiterBridgeDevice() {
    delete impl_;
}


std::vector<std::string> JubiterBridgeDevice::EnumDevice(const std::string& ip) {
    auto address = ServerAddress(ip);
    auto impl = std::make_unique<JubiterBridgeDevice::Impl>(address);
    return impl->ListReaders();
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

#endif  // #if defined(GRPC_MODE) end
