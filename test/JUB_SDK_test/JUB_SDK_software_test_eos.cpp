//
//  JUB_SDK_software_test_eos.cpp
//  JubSDKTest
//
//  Created by panmin on 2019/9/19.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#include "JUB_SDK_test.h"
#include "JUB_SDK_software_test_eos.hpp"

#include "mSIGNA/stdutils/uchar_vector.h"
#include "HexCoding.h"
#include "PublicKey.h"
#include "PrivateKey.h"

#include "EOS/Address.h"
#include "EOS/Action.h"
#include "EOS/Signer.h"
#include "EOS/Transaction.h"

#include "Bravo/Transaction.h"
#include "Bravo/Operation.h"
#include "Bravo/Serialization.h"
#include "Bravo/Deserialization.h"

void var_test_eos() {
    uint64_t x = 160;//1234567890;//0x499602D2
    Data enData;
    TW::Bravo::encodeVarInt64(x, enData);
    uchar_vector vOs(enData);
    std::cout << "[" << x << "]os: " << vOs.getHex() << std::endl;

    uint64_t y = 0;
    int varIntByteSize = 0;
    TW::Bravo::decodeVarInt64(enData, y, varIntByteSize);
    std::cout << "[" << y << "]os: " << vOs.getHex() << std::endl;
}

void address_test_eos() {
    //
    //BRV5LZBF18uucMKVyKMQ7r6qnXBWn9StM5AJYHs1kidPeyNexuBUm
    if(!TW::EOS::Address::isValid("EOS8gWdmWT5wTD2BwAfZpx1fGnh1h5De9cwZsbk8X9G9EXAG9u2Nk")) {
        std::cout << "Address::inValid." << std::endl;
    }
}

void permissionLevel_test_eos() {
    TW::EOS::PermissionLevel permissionLevel =
    TW::EOS::PermissionLevel(TW::EOS::Name("eosio"),
                             TW::EOS::Name("active"));
    Data level;
    permissionLevel.serialize(level);
    uchar_vector vPermissionLevel(level);
    std::cout << "permission level: " << vPermissionLevel.getHex() << std::endl;
}

void asset_test_eos() {
//    std::string assetString = "5.001 XYZ";
//    std::string assetString = "2.100 EOS";
//    std::string assetString = "2.1000 EOS";
    std::string assetString = "0.0001 EOS";
    TW::Bravo::Asset asset = TW::Bravo::Asset::fromString(assetString);
    if (0 == assetString.compare(asset.string())) {
        std::cout << "serialize/deserialize TW::Bravo::Asset OK!" << std::endl;
    }
    else {
        std::cout << "serialize/deserialize TW::Bravo::Asset Failure!" << std::endl;
    }

    TW::Bravo::Asset assetFromiItem(asset.amount, asset.getDecimals(), asset.getSymbol());
    if (0 == assetString.compare(assetFromiItem.string())) {
        std::cout << "serialize/deserialize TW::Bravo::Asset OK!" << std::endl;
    }
    else {
        std::cout << "serialize/deserialize TW::Bravo::Asset Failure!" << std::endl;
    }

    Data os;
    asset.serialize(os);
    uchar_vector vAsset(os);
    std::cout << "asset hex: " << vAsset.getHex() << std::endl;
    TW::Bravo::Asset assetFromHex;
    assetFromHex.deserialize(os);
    if (0 == assetString.compare(assetFromHex.string())) {
        std::cout << "serialize/deserialize TW::Bravo::Asset OK!" << std::endl;
    }
    else {
        std::cout << "serialize/deserialize TW::Bravo::Asset Failure!" << std::endl;
    }

    uchar_vector hex_data("e80300000000000004454f5300000000");
    Data data(hex_data);
    TW::Bravo::Asset assetHex;
    assetHex.deserialize(data);
    std::cout << assetHex.string() << std::endl;
}

void op_test_eos() {

//    [
//     "transfer",
//     {
//        "amount":"0.1000 EOS",
//        "from":"ypqazokngspt1",
//        "memo":"20,,",
//        "to":"godice.e1"
//     }]
    TW::Bravo::TransferOperation op =
//    TW::Bravo::TransferOperation("ypqazokngspt1",
//                                 "godice.e1",
//                                 TW::Bravo::Asset::fromString("0.1000 EOS"),
//                                 "20,,");
    TW::Bravo::TransferOperation("ypqazokngspt1",
                                 "godice.e1",
                                 "0.1000 EOS",
                                 "20,,");
    nlohmann::json opInJSON = op.serialize();
    std::cout << "opInJSON: " << opInJSON << std::endl;

    //02
    //0d797071617a6f6b6e677370743109
    //676f646963652e6531
    //e80300000000000004454f5300000000
    //0432302c2c
    Data os;
    op.serialize(os);
    uchar_vector hex_data(os);
    std::cout << "operation: " << hex_data.getHex() << std::endl;

    uchar_vector vHex("020d797071617a6f6b6e677370743109676f646963652e6531e80300000000000004454f53000000000432302c2c");
    TW::Bravo::TransferOperation decodeTransfer;
    Data decodeOs(vHex);
    decodeTransfer.deserialize(decodeOs);
//    decodeTransfer.deserialize(os);
    if (opInJSON == decodeTransfer.serialize()) {
        std::cout << "serialize/deserialize TW::Bravo::TransferOperation OK!" << std::endl;
    }
    else {
        std::cout << "serialize/deserialize TW::Bravo::TransferOperation Failure!" << std::endl;
    }
}

void action_test_eos() {
//    {
//        "account":"eosio.token1",
//        "authorizations":[
//        {
//            "actor":"ypqazokngspt1",
//            "permission":"active"
//        }],
//        "data":"912b6613d26f6cf50000080a28e41265e80300000000000004454f53000000000432302c2c",
//        "name":"transfer"
//    }

//{
//    "account":"eosio.token1",
//    "authorizations":[
//    {
//        "actor":"ypqazokngspt1",
//        "permission":"active"
//    }],
//    "data":{
//        "from":"ypqazokngspt1",
//        "memo":"20,,",
//        "quantity":"0.1000 EOS",
//        "to":"ypqazokngspt1"
//    },
//    "hex_data":"912b6613d26f6cf5912b6613d26f6cf5e80300000000000004454f53000000000432302c2c",
//    "name":"transfer"
//}
    TW::EOS::TransferAction transfer =
    TW::EOS::TransferAction("eosio.token1",
                            "ypqazokngspt1", "godice.e1",
                            TW::Bravo::Asset::fromString("0.1000 EOS"),
                            "20,,");
    nlohmann::json transferInJSON = transfer.serialize();
    std::cout << "transferInJSON: " << transferInJSON << std::endl;

//    Data transferHex;
//    transfer.TW::EOS::Action::serialize(transferHex);
//    uchar_vector vTransfer(transferHex);
//    std::cout << "transfer hex: " << vTransfer.getHex() << std::endl;

//    Data transferData(transfer.data);
//    uchar_vector vTransferData(transferData);
//    std::cout << "hex_data: " << vTransferData.getHex() << std::endl;

//    "data":{
//        "from":"ypqazokngspt1",
//        "memo":"20,,",
//        "quantity":"0.1000 EOS",
//        "to":"ypqazokngspt1"
//    }
    TW::EOS::TransferAction transferDecode;
    transferDecode.deserialize(transfer.data);
    nlohmann::json transferDecodeInJSON = transferDecode.serialize();
    std::cout << "transferDecodeInJSON: " << transferDecodeInJSON << std::endl;

//    "data": {
//        "from": "bluebetrobot",
//        "memo": "每万股分红保底 >2EOS，低价买入，随时卖出，最高分红！Per 10, 000 BLUE share of dividends > 2EOS，Go to 【OTC】--->  https://bluebet.one",
//        "quantity": "0.0001 EOS",
//        "to": "zhanglt12345"
//    }
    std::string hex_data = "90e9a137aba3743c50c8102147364dfb010000000000000004454f5300000000a001e6af8fe4b887e882a1e58886e7baa2e4bf9de5ba95203e32454f53efbc8ce4bd8ee4bbb7e4b9b0e585a5efbc8ce99a8fe697b6e58d96e587baefbc8ce69c80e9ab98e58886e7baa2efbc815065722031302c2030303020424c5545207368617265206f66206469766964656e6473203e2032454f53efbc8c476f20746f20e380904f5443e380912d2d2d3e202068747470733a2f2f626c75656265742e6f6e65";
    const TW::Data validHexData = TW::parse_hex(hex_data.c_str());
    TW::EOS::TransferAction trans;
    trans.deserialize(validHexData);
    nlohmann::json transInJSON = trans.serialize();
    std::cout << "transInJSON: " << transInJSON << std::endl;
}

void tx_test_eos_deserialize() {
//    std::string packed_trx = "b927835df5fa29aaadc3000000000100a6823403ea3055000000572d3ccdcd01a098c997363cd55600000000a8ed323263a098c997363cd55680a9c254553d99c3881300000000000004454f53000000004268756d7261746d6f74696f6e2d454f5338573357596570473633665a5a4b76696635314c4151765955794c7646506e337a4161464d436e627971747a65646351513400";
//   std::string packed_trx = "d900875d8da49180df29000000000100147209000f918e0040346aabcbcd4d015035bd4c2197b23a00aeda57610f918e6aa01861fc489abe6a2a335f1ebec3d5cc08996992fc6f42b78dd9745c781a3dfc7858134ea1ba2b2c001f08d5aec9179b2a46d9b52f0cdd1c72c3d0815f99ca07b36cde4e02653e81ed4e65a08d3d0f2676830947954eb41614c72669ed144cd65a20f324185e7f086f5600";
//    std::string packed_trx = "79388c5d75f192360f4c000000000100a6823403ea3055000000572d3ccdcd0190e9a137aba3743c00000000a8ed3232c20190e9a137aba3743c50c8102147364dfb010000000000000004454f5300000000a001e6af8fe4b887e882a1e58886e7baa2e4bf9de5ba95203e32454f53efbc8ce4bd8ee4bbb7e4b9b0e585a5efbc8ce99a8fe697b6e58d96e587baefbc8ce69c80e9ab98e58886e7baa2efbc815065722031302c2030303020424c5545207368617265206f66206469766964656e6473203e2032454f53efbc8c476f20746f20e380904f5443e380912d2d2d3e202068747470733a2f2f626c75656265742e6f6e6500";
//1e04a25c
//dc46
//a452d92c
//00
//00
//00
//00
//01 0000000080a920cd 000000572d3ccdcd
//01 0000000000ea3055 00000000a8ed3232
//32 0000000000ea30550000000080a920cd121203000000000004544b4e00000000116d79206669727374207472616e73666572
//00
    std::string packed_trx = "1e04a25cdc46a452d92c00000000010000000080a920cd000000572d3ccdcd010000000000ea305500000000a8ed3232320000000000ea30550000000080a920cd121203000000000004544b4e00000000116d79206669727374207472616e7366657200";
    const TW::Data validPackedTrx = TW::parse_hex(packed_trx.c_str());
    TW::EOS::Transaction tx;
    tx.deserialize(validPackedTrx);
    for (const auto& act : tx.actions) {
        TW::EOS::TransferAction transfer;
        transfer.deserialize(act.data);
        std::cout << "action: " << transfer.serialize() << std::endl;
    }
    std::cout << "trx: " << tx.serialize() << std::endl;
}

void tx_test_eos_serialize() {

    TW::EOS::TransferAction action =
    TW::EOS::TransferAction("token",
                            "eosio", "token",
                            TW::Bravo::Asset::fromString("20.1234 TKN"),
                            "my first transfer");
    nlohmann::json actionInJSON = action.serialize();
    std::cout << "action:  " << actionInJSON << std::endl;

    Data referenceBlockId = TW::parse_hex("000046dc08ad384ca452d92c59348aba888fcbb6ef1ebffc3181617706664d4c");
    int32_t referenceBlockTime = 1554121728;
    TW::EOS::Transaction tx {referenceBlockId, referenceBlockTime};
    tx.actions.push_back(action);
    std::string txDump = tx.actions.back().serialize().dump();
    std::cout << "tx dump: " << actionInJSON << std::endl;

    Data buf;
    tx.serialize(buf);
//    uchar_vector vBuf(buf);
//    std::cout << "vBuf[" << vBuf.size()/2 << "]: " << vBuf.getHex() << std::endl;

    auto chainId = TW::parse_hex("cf057bbfb72640471fd910bcb67639c22df9f92470936cddc1ade0e2f2e7dc4f");
    TW::EOS::Signer signer {chainId};

    std::cout << "tx hex: " << TW::hex(buf) << std::endl;;
    std::cout << "tx hash hex: " << TW::hex(signer.hash(tx)) << std::endl;;

    // make transaction invalid and see if signing succeeds
//    tx.maxNetUsageWords = UINT32_MAX;
    //void sign(const PrivateKey& privateKey, Type type, Transaction& transaction);
    signer.sign(TW::PrivateKey(TW::Hash::sha256(std::string("A"))),
                TW::EOS::Type::ModernK1,
                tx);

    nlohmann::json txInJSON = tx.serialize();
    std::cout << "txInJSON: " << txInJSON << std::endl;
}
