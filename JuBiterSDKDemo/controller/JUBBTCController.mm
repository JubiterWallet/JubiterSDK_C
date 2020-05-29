//
//  JUBBTCController.mm
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/4/28.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBBTCController.h"


@interface JUBBTCController ()

@end


@implementation JUBBTCController


- (void)viewDidLoad {
    
    self.selfClass = self;
    self.optItem = JUB_NS_ENUM_MAIN::OPT_BTC;
    
    self.coinTypeArray = @[BUTTON_TITLE_BTCP2PKH,
                           BUTTON_TITLE_BTCP2WPKH,
                           BUTTON_TITLE_LTC,
                           BUTTON_TITLE_DASH,
                           BUTTON_TITLE_BCH,
                           BUTTON_TITLE_QTUM,
                           BUTTON_TITLE_QTUM_QRC20,
                           BUTTON_TITLE_USDT,
//                           BUTTON_TITLE_HCASH
    ];
    
    [super viewDidLoad];
    // Do any additional setup after loading the view.
}


#pragma mark - 通讯库寻卡回调
- (void)CoinBTCOpt:(JUB_UINT16)deviceID {
    
    const char* json_file = "";
    JUB_ENUM_COINTYPE_BTC coinType = JUB_ENUM_COINTYPE_BTC::COINBTC;
    switch ((JUB_NS_ENUM_BTC_COINTYPE)self.optCoinType) {
    case JUB_NS_ENUM_BTC_COINTYPE::COIN_BTC_P2PKH:
    {
        json_file = JSON_FILE_BTC_44;
        coinType = COINBTC;
        break;
    }
    case JUB_NS_ENUM_BTC_COINTYPE::COIN_BTC_P2WPKH:
    {
        json_file = JSON_FILE_BTC_49;
        coinType = COINBTC;
        break;
    }
    case JUB_NS_ENUM_BTC_COINTYPE::COIN_LTC:
    {
        json_file = JSON_FILE_LTC;
        coinType = COINLTC;
        break;
    }
    case JUB_NS_ENUM_BTC_COINTYPE::COIN_DASH:
    {
        json_file = JSON_FILE_DASH;
        coinType = COINDASH;
        break;
    }
    case JUB_NS_ENUM_BTC_COINTYPE::COIN_BCH:
    {
        json_file = JSON_FILE_BCH;
        coinType = COINBCH;
        break;
    }
    case JUB_NS_ENUM_BTC_COINTYPE::COIN_QTUM:
    {
        json_file = JSON_FILE_QTUM;
        coinType = COINQTUM;
        break;
    }
    case JUB_NS_ENUM_BTC_COINTYPE::COIN_QTUM_QRC20:
    {
        json_file = JSON_FILE_QTUM_QRC20;
        coinType = COINQTUM;
        break;
    }
    case JUB_NS_ENUM_BTC_COINTYPE::COIN_USDT:
    {
        json_file = JSON_FILE_BTC_USDT;
        coinType = COINUSDT;
        break;
    }
    default:
//        json_file = JSON_FILE_HCASH;
        break;
    }
    
    NSString *filePath = [[NSBundle mainBundle] pathForResource:[NSString stringWithFormat:@"%s", json_file]
                                                         ofType:@"json"];
    Json::Value root = readJSON([filePath UTF8String]);
    
    switch (self.optCoinType) {
//    case JUB_NS_ENUM_BTC_COINTYPE::COIN_HCASH:
//        HC_test(deviceID, root, (int)self.optIndex);
//        break;
    default:
        [self BTC_test:deviceID
                  root:root
                choice:(int)self.optIndex
              coinType:coinType];
        break;
    }
}


#pragma mark - BTC applet
- (void)BTC_test:(JUB_UINT16)deviceID
            root:(Json::Value)root
          choice:(int)choice
        coinType:(JUB_ENUM_COINTYPE_BTC)coinType {
    
    JUB_RV rv = JUBR_ERROR;
    
    try {
        JUB_UINT16 contextID = 0;

        CONTEXT_CONFIG_BTC cfg;
        cfg.mainPath = (char*)root["main_path"].asCString();
        cfg.coinType = coinType;

        if (COINBCH == coinType) {
            cfg.transType = p2pkh;
        }
        else {
            if (root["p2sh-segwit"].asBool()) {
                cfg.transType = p2sh_p2wpkh;
            }
            else {
                cfg.transType = p2pkh;
            }
        }

        rv = JUB_CreateContextBTC(cfg, deviceID, &contextID);
        if (JUBR_OK != rv) {
            [self addMsgData:[NSString stringWithFormat:@"[JUB_CreateContextBTC() return 0x%2lx.]", rv]];
            return;
        }
        [self addMsgData:[NSString stringWithFormat:@"[JUB_CreateContextBTC() OK.]"]];
        
        switch (choice) {
        case JUB_NS_ENUM_OPT::GET_ADDRESS:
            [self get_address_test:contextID
                              root:root];
            break;
        case JUB_NS_ENUM_OPT::SHOW_ADDRESS:
//            [self show_address_test:contextID];
            break;
        case JUB_NS_ENUM_OPT::TRANSACTION:
        {
            switch((JUB_NS_ENUM_BTC_COINTYPE)self.optCoinType) {
            case JUB_NS_ENUM_BTC_COINTYPE::COIN_QTUM_QRC20:
                [self transactionQTUM_test:contextID
                                      root:root];
                break;
            case JUB_NS_ENUM_BTC_COINTYPE::COIN_USDT:
                [self transactionUSDT_test:contextID
                                      root:root];
                break;
            default:
                [self transaction_test:contextID
                                  root:root];
                break;
            }
            break;
        }
        case JUB_NS_ENUM_OPT::SET_MY_ADDRESS:
//            [self set_my_address_test_BTC:contextID];
            break;
        case JUB_NS_ENUM_OPT::SET_TIMEOUT:
            break;
        default:
            break;
        }
    }
    catch (...) {
        error_exit("[Error format json file.]\n");
        [self addMsgData:[NSString stringWithFormat:@"[Error format json file.]"]];
    }
}


- (void)get_address_test:(JUB_UINT16)contextID
                    root:(Json::Value)root {
    
    JUB_RV rv = JUBR_ERROR;
    
    JUB_CHAR_PTR mainXpub;
    rv = JUB_GetMainHDNodeBTC(contextID, &mainXpub);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetMainHDNodeBTC() return 0x%2lx.]", rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_GetMainHDNodeBTC() OK.]"]];
    
    [self addMsgData:[NSString stringWithFormat:@"Main xpub : %s.", mainXpub]];
    
    JUB_FreeMemory(mainXpub);
    
//    int inputNumber = root["inputs"].size();
//    for (int i = 0; i < inputNumber; i++) {
//        JUB_CHAR_PTR xpub;
//
//        BIP44_Path path;
//        path.change = (JUB_ENUM_BOOL)root["inputs"][i]["bip32_path"]["change"].asBool();
//        path.addressIndex = root["inputs"][i]["bip32_path"]["addressIndex"].asInt();
//
//        rv = JUB_GetHDNodeBTC(contextID, path, &xpub);
//        if (JUBR_OK != rv) {
//            [self addMsgData:[NSString stringWithFormat:@"[JUB_GetHDNodeBTC() return 0x%2lx.]", rv]];
//            break;
//        }
//
//        [self addMsgData:[NSString stringWithFormat:@"input %d xpub: %s.", i, xpub]];
//
//        JUB_FreeMemory(xpub);
//
//        JUB_CHAR_PTR address;
//        rv = JUB_GetAddressBTC(contextID, path, BOOL_FALSE, &address);
//        if (JUBR_OK != rv) {
//            [self addMsgData:[NSString stringWithFormat:@"[JUB_GetAddressBTC() return 0x%2lx.]", rv]];
//            break;
//        }
//
//        [self addMsgData:[NSString stringWithFormat:@"input %d address: %s.", i, address]];
//
//        JUB_FreeMemory(address);
//    }
//    if (JUBR_OK != rv) {
//        return;
//    }
    
    JUB_CHAR_PTR xpub;
    
    BIP44_Path path;
    path.change = (self.change ? JUB_ENUM_BOOL::BOOL_TRUE:JUB_ENUM_BOOL::BOOL_FALSE);
    path.addressIndex = self.addressIndex;
    
    rv = JUB_GetHDNodeBTC(contextID, path, &xpub);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetHDNodeBTC() return 0x%2lx.]", rv]];
        return;
    }
    else {
        [self addMsgData:[NSString stringWithFormat:@"input xpub(%d/%d): %s.", path.change, path.addressIndex, xpub]];
        
        JUB_FreeMemory(xpub);
    }
    
    JUB_CHAR_PTR address;
    rv = JUB_GetAddressBTC(contextID, path, BOOL_FALSE, &address);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetAddressBTC() return 0x%2lx.]", rv]];
    }
    else {
        [self addMsgData:[NSString stringWithFormat:@"input address(%d/%d): %s.", path.change, path.addressIndex, address]];
        
        JUB_FreeMemory(address);
    }
}


- (void)show_address_test:(JUB_UINT16)contextID {
    
    JUB_RV rv = JUBR_ERROR;
    
    int change = 0;
    JUB_UINT64 index = 0;
//    std::cout << "please input change level (non-zero means 1):" << std::endl;
//    std::cin >> change;
//    std::cout << "please input index " << std::endl;
//    std::cin >> index;
    
    BIP44_Path path;
    path.change = JUB_ENUM_BOOL(change);
    path.addressIndex = index;
    
    JUB_CHAR_PTR address;
    rv = JUB_GetAddressBTC(contextID, path, BOOL_TRUE, &address);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetAddressBTC() return 0x%2lx.]", rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"show address is: %s.", address]];
    
    JUB_FreeMemory(address);
}

- (void)set_my_address_test_BTC:(JUB_UINT16)contextID {
    
    JUB_RV rv = JUBR_ERROR;
    
//    rv = verify_pin(contextID);
//    if (JUBR_OK != rv) {
//        return;
//    }
    
    int change = 0;
    JUB_UINT64 index = 0;
//    std::cout << "please input change level (non-zero means 1):" << std::endl;
//    std::cin >> change;
//    std::cout << "please input index " << std::endl;
//    std::cin >> index;
    
    BIP44_Path path;
    path.change = JUB_ENUM_BOOL(change);
    path.addressIndex = index;
    
    JUB_CHAR_PTR address = nullptr;
    rv = JUB_SetMyAddressBTC(contextID, path, &address);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_SetMyAddressBTC() return 0x%2lx.]", rv]];
        return;
    }
    else {
        [self addMsgData:[NSString stringWithFormat:@"set my address is: %s.", address]];
        JUB_FreeMemory(address);
    }
}

- (void)transaction_test:(JUB_UINT16)contextID
                    root:(Json::Value)root {
    
    JUB_RV rv = JUBR_ERROR;
    
//    JUB_ENUM_BTC_UNIT_TYPE unit = mBTC;
//
//    cout << "Please input BTCunit on JubiterBLD" << endl;
//    cout << "1: BTC" << endl;
//    cout << "2: cBTC" << endl;
//    cout << "3: mBTC" << endl;
//    cout << "4: uBTC" << endl;
//    cout << "5: Satoshi" << endl;
//
//    int choice = 0;
//    cin >> choice;
//
//    switch (choice) {
//    case 1:
//        unit = BTC;
//        break;
//    case 2:
//        unit = cBTC;
//        break;
//    case 3:
//        unit = mBTC;
//        break;
//    case 4:
//        unit = uBTC;
//        break;
//    case 5:
//        unit = Satoshi;
//        break;
//    }
//
//    rv = JUB_SetUnitBTC(contextID, unit);
//    if (   JUBR_OK               != rv
//        && JUBR_IMPL_NOT_SUPPORT != rv
//        ) {
//        cout << "JUB_SetUnitBTC() return " << GetErrMsg(rv) << endl;
//        return;
//    }
    
    rv = [self verify_pin:contextID
                      pin:[self.userPIN UTF8String]];
    if (JUBR_OK != rv) {
        return;
    }
    
    rv = [self transaction_proc:contextID
                           root:root];
    if (JUBR_OK != rv) {
        return;
    }
}

- (JUB_RV)transaction_proc:(JUB_UINT16)contextID
                      root:(Json::Value)root {
    
    JUB_RV rv = JUBR_ERROR;
    
    JUB_UINT32 version = root["ver"].asInt();
    
    std::vector<INPUT_BTC> inputs;
    std::vector<OUTPUT_BTC> outputs;
    int inputNumber = root["inputs"].size();
    
    for (int i = 0; i < inputNumber; i++) {
        INPUT_BTC input;
        input.type = JUB_ENUM_SCRIPT_BTC_TYPE::P2PKH;
        input.preHash = (char*)root["inputs"][i]["preHash"].asCString();
        input.preIndex = root["inputs"][i]["preIndex"].asInt();
        input.path.change = (JUB_ENUM_BOOL)root["inputs"][i]["bip32_path"]["change"].asBool();
        input.path.addressIndex = root["inputs"][i]["bip32_path"]["addressIndex"].asInt();
        input.amount = root["inputs"][i]["amount"].asUInt64();
        input.nSequence = 0xffffffff;
        inputs.push_back(input);
    }
    
    int outputNumber = root["outputs"].size();
    
    for (int i = 0; i < outputNumber; i++) {
        OUTPUT_BTC output;
        output.type = JUB_ENUM_SCRIPT_BTC_TYPE::P2PKH;
        output.stdOutput.address = (char*)root["outputs"][i]["address"].asCString();
        output.stdOutput.amount = root["outputs"][i]["amount"].asUInt64();
        output.stdOutput.changeAddress = (JUB_ENUM_BOOL)root["outputs"][i]["change_address"].asBool();
        if (output.stdOutput.changeAddress) {
            output.stdOutput.path.change = (JUB_ENUM_BOOL)root["outputs"][i]["bip32_path"]["change"].asBool();
            output.stdOutput.path.addressIndex = root["outputs"][i]["bip32_path"]["addressIndex"].asInt();
        }
        outputs.push_back(output);
    }
    
    char* raw = nullptr;
    rv = JUB_SignTransactionBTC(contextID, version, &inputs[0], (JUB_UINT16)inputs.size(), &outputs[0], (JUB_UINT16)outputs.size(), 0, &raw);
    [self addMsgData:[NSString stringWithFormat:@"[JUB_SignTransactionBTC() return 0x%2lx.]", rv]];
    
    if (JUBR_USER_CANCEL == rv) {
        [self addMsgData:[NSString stringWithFormat:@"[User cancel the transaction !]"]];
        return rv;
    }
    if (   JUBR_OK != rv
        || nullptr == raw
        ) {
        [self addMsgData:[NSString stringWithFormat:@"[Error sign tx.]"]];
        return rv;
    }
    if (raw) {
        size_t txLen = strlen(raw)/2;
        [self addMsgData:[NSString stringWithFormat:@"tx raw[%lu]: %s.", txLen, raw]];
        
        JUB_FreeMemory(raw);
    }
    
    return rv;
}


- (void)transactionQTUM_test:(JUB_UINT16)contextID
                        root:(Json::Value)root {
    
    JUB_RV rv = JUBR_ERROR;
    
    rv = [self verify_pin:contextID
                      pin:[self.userPIN UTF8String]];
    if (JUBR_OK != rv) {
        return;
    }
    
    rv = [self transactionQTUM_proc:contextID
                               root:root];
    if (JUBR_OK != rv) {
        return;
    }
}


- (JUB_RV)transactionQTUM_proc:(JUB_UINT16)contextID
                          root:(Json::Value)root {
    
    JUB_RV rv = JUBR_ERROR;
    
    JUB_UINT32 version = root["ver"].asInt();
    
    std::vector<INPUT_BTC> inputs;
    std::vector<OUTPUT_BTC> outputs;
    int inputNumber = root["inputs"].size();
    
    for (int i = 0; i < inputNumber; i++) {
        INPUT_BTC input;
        input.type = JUB_ENUM_SCRIPT_BTC_TYPE::P2PKH;
        input.preHash = (char*)root["inputs"][i]["preHash"].asCString();
        input.preIndex = root["inputs"][i]["preIndex"].asInt();
        input.path.change = (JUB_ENUM_BOOL)root["inputs"][i]["bip32_path"]["change"].asBool();
        input.path.addressIndex = root["inputs"][i]["bip32_path"]["addressIndex"].asInt();
        input.amount = root["inputs"][i]["amount"].asUInt64();
        inputs.push_back(input);
    }
    
    int outputNumber = root["outputs"].size();
    
    for (int i = 0; i < outputNumber; i++) {
        OUTPUT_BTC output;
        output.type = JUB_ENUM_SCRIPT_BTC_TYPE::P2PKH;
        output.stdOutput.address = (char*)root["outputs"][i]["address"].asCString();
        output.stdOutput.amount = root["outputs"][i]["amount"].asUInt64();
        output.stdOutput.changeAddress = (JUB_ENUM_BOOL)root["outputs"][i]["change_address"].asBool();
        if (output.stdOutput.changeAddress) {
            output.stdOutput.path.change = (JUB_ENUM_BOOL)root["outputs"][i]["bip32_path"]["change"].asBool();
            output.stdOutput.path.addressIndex = root["outputs"][i]["bip32_path"]["addressIndex"].asInt();
        }
        outputs.push_back(output);
    }
    
    OUTPUT_BTC QRC20_output;
    JUB_CHAR_CPTR contractAddress = (char*)root["QRC20_contractAddr"].asCString();
    JUB_UINT8 decimal = root["QRC20_decimal"].asUInt64();
    JUB_CHAR_CPTR symbol = (char*)root["QRC20_symbol"].asCString();
    JUB_UINT64 gasLimit = root["gasLimit"].asUInt64();
    JUB_UINT64 gasPrice = root["gasPrice"].asUInt64();
    JUB_CHAR_CPTR to = (char*)root["QRC20_to"].asCString();
    JUB_CHAR_CPTR value = (char*)root["QRC20_amount"].asCString();
    rv = JUB_BuildQRC20Outputs(contextID,
                               contractAddress, decimal, symbol,
                               gasLimit, gasPrice,
                               to, value,
                               &QRC20_output);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_BuildQRC20Outputs() return 0x%2lx.]", rv]];
        return rv;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_BuildQRC20Outputs() OK.]"]];
    outputs.emplace_back(QRC20_output);
    
    char* raw = nullptr;
    rv = JUB_SignTransactionBTC(contextID, version, &inputs[0], (JUB_UINT16)inputs.size(), &outputs[0], (JUB_UINT16)outputs.size(), 0, &raw);
    [self addMsgData:[NSString stringWithFormat:@"[JUB_SignTransactionBTC() return 0x%2lx.]", rv]];

    if (JUBR_USER_CANCEL == rv) {
        [self addMsgData:[NSString stringWithFormat:@"[User cancel the transaction !]"]];
        return rv;
    }
    if (   JUBR_OK != rv
        || nullptr == raw
        ) {
        [self addMsgData:[NSString stringWithFormat:@"[Error sign tx.]"]];
        return rv;
    }
    if (raw) {
        size_t txLen = strlen(raw)/2;
        [self addMsgData:[NSString stringWithFormat:@"tx raw[%lu]: %s.", txLen, raw]];
        
        JUB_FreeMemory(raw);
    }
    
    return rv;
}


- (void)transactionUSDT_test:(JUB_UINT16)contextID
                        root:(Json::Value)root {
    
    JUB_RV rv = JUBR_ERROR;
    
    rv = [self verify_pin:contextID
                      pin:[self.userPIN UTF8String]];
    if (JUBR_OK != rv) {
        return;
    }
    
    rv = [self transactionUSDT_proc:contextID
                               root:root];
    if (JUBR_OK != rv) {
        return;
    }
}


- (JUB_RV)transactionUSDT_proc:(JUB_UINT16)contextID
                          root:(Json::Value)root {
    
    JUB_RV rv = JUBR_ERROR;
    
    JUB_UINT32 version = root["ver"].asInt();
    
    std::vector<INPUT_BTC> inputs;
    std::vector<OUTPUT_BTC> outputs;
    int inputNumber = root["inputs"].size();
    
    for (int i = 0; i < inputNumber; i++) {
        INPUT_BTC input;
        input.type = JUB_ENUM_SCRIPT_BTC_TYPE::P2PKH;
        input.preHash = (char*)root["inputs"][i]["preHash"].asCString();
        input.preIndex = root["inputs"][i]["preIndex"].asInt();
        input.path.change = (JUB_ENUM_BOOL)root["inputs"][i]["bip32_path"]["change"].asBool();
        input.path.addressIndex = root["inputs"][i]["bip32_path"]["addressIndex"].asInt();
        input.amount = root["inputs"][i]["amount"].asUInt64();
        input.nSequence = 0xffffffff;
        inputs.push_back(input);
    }
    
    int outputNumber = root["outputs"].size();
    
    for (int i = 0; i < outputNumber; i++) {
        OUTPUT_BTC output;
        output.type = JUB_ENUM_SCRIPT_BTC_TYPE::P2PKH;
        output.stdOutput.address = (char*)root["outputs"][i]["address"].asCString();
        output.stdOutput.amount = root["outputs"][i]["amount"].asUInt64();
        output.stdOutput.changeAddress = (JUB_ENUM_BOOL)root["outputs"][i]["change_address"].asBool();
        if (output.stdOutput.changeAddress) {
            output.stdOutput.path.change = (JUB_ENUM_BOOL)root["outputs"][i]["bip32_path"]["change"].asBool();
            output.stdOutput.path.addressIndex = root["outputs"][i]["bip32_path"]["addressIndex"].asInt();
        }
        outputs.push_back(output);
    }
    
    OUTPUT_BTC USDT_outputs[2] = {};
    rv = JUB_BuildUSDTOutputs(contextID, (char*)root["USDT_to"].asCString(), root["USDT_amount"].asUInt64(), USDT_outputs);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_BuildUSDTOutputs() return 0x%2lx.]", rv]];
        return rv;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_BuildUSDTOutputs() OK.]"]];
    
    outputs.emplace_back(USDT_outputs[0]);
    outputs.emplace_back(USDT_outputs[1]);
    
    char* raw = nullptr;
    rv = JUB_SignTransactionBTC(contextID, version, &inputs[0], (JUB_UINT16)inputs.size(), &outputs[0], (JUB_UINT16)outputs.size(), 0, &raw);
    [self addMsgData:[NSString stringWithFormat:@"[JUB_SignTransactionBTC() return 0x%2lx.]", rv]];

    if (JUBR_USER_CANCEL == rv) {
        [self addMsgData:[NSString stringWithFormat:@"[User cancel the transaction !]"]];
        return rv;
    }
    if (   JUBR_OK != rv
        || nullptr == raw
        ) {
        [self addMsgData:[NSString stringWithFormat:@"[Error sign tx.]"]];
        return rv;
    }
    if (raw) {
        size_t txLen = strlen(raw)/2;
        [self addMsgData:[NSString stringWithFormat:@"tx raw[%lu]: %s.", txLen, raw]];
        
        JUB_FreeMemory(raw);
    }
    
    return rv;
}


//#pragma mark - Hcash applet
//void HC_test(JUB_UINT16 deviceID, Json::Value root, int choice) {
//
//    JUB_RV rv = JUBR_ERROR;
//
//    try {
//        JUB_UINT16 contextID = 0;
//        CONTEXT_CONFIG_HC cfg;
//        cfg.mainPath = (char*)root["main_path"].asCString();
//
//        rv = JUB_CreateContextHC(cfg, deviceID, &contextID);
//        if (JUBR_OK != rv) {
//            [self addMsgData:[NSString stringWithFormat:@"[JUB_CreateContextHC() return 0x%2lx.]", rv]];
//            return;
//        }
//        [self addMsgData:[NSString stringWithFormat:@"[JUB_CreateContextHC() OK.]"]];
//
//        switch (choice) {
//        case JUB_NS_ENUM_OPT::GET_ADDRESS:
//            get_address_test_HC(contextID, root);
//            break;
//        case JUB_NS_ENUM_OPT::SHOW_ADDRESS:
////            show_address_test_HC(contextID);
//            break;
//        case JUB_NS_ENUM_OPT::TRANSACTION:
//            transactionHC_test(contextID, root);
//            break;
//        case JUB_NS_ENUM_OPT::SET_MY_ADDRESS:
////            [self set_my_address_test_BTC:contextID];
//            break;
//        case JUB_NS_ENUM_OPT::SET_TIMEOUT:
//            break;
//        default:
//            break;
//        }
//    }
//    catch (...) {
//        error_exit("[Error format json file.]\n");
//        [self addMsgData:[NSString stringWithFormat:@"[JUB_CreateContextHC() OK.]"]];
//    }
//}
//
//
//void get_address_test_HC(JUB_UINT16 contextID, Json::Value root) {
//
//    JUB_CHAR_PTR mainXpub;
//    JUB_RV rv = JUB_GetMainHDNodeHC(contextID, &mainXpub);
//    if (JUBR_OK != rv) {
//        cout << "[JUB_GetMainHDNodeHC() return " << GetErrMsg(rv) << ".]" << endl;
//        return;
//    }
//    [self addMsgData:[NSString stringWithFormat:@"[JUB_GetMainHDNodeHC() OK.]"]];
//
//    cout << "Main xpub : " << mainXpub << endl;
//    JUB_FreeMemory(mainXpub);
//
//    int inputNumber = root["inputs"].size();
//    for (int i = 0; i < inputNumber; i++) {
//        JUB_CHAR_PTR xpub;
//
//        BIP44_Path path;
//        path.change = (JUB_ENUM_BOOL)root["inputs"][i]["bip32_path"]["change"].asBool();
//        path.addressIndex = root["inputs"][i]["bip32_path"]["addressIndex"].asInt();
//
//        JUB_RV rv = JUB_GetHDNodeHC(contextID, path, &xpub);
//        if (JUBR_OK != rv) {
//            cout << "[JUB_GetHDNodeHC() return " << GetErrMsg(rv) << ".]" << endl;
//            break;
//        }
//        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetHDNodeHC() OK.]"]];
//
//        cout << "input " << i << " xpub : " << xpub << endl;
//        [self addMsgData:[NSString stringWithFormat:@"input %d xpub : %s.", i, xpub]];
//        JUB_FreeMemory(xpub);
//
//        JUB_CHAR_PTR address;
//        rv = JUB_GetAddressHC(contextID, path, BOOL_FALSE, &address);
//        if (JUBR_OK != rv) {
//            cout << "[JUB_GetAddressHC() return " << GetErrMsg(rv) << ".]" << endl;
//            break;
//        }
//        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetAddressHC() OK.]"]];
//        cout << "input " << i << " address : " << address << endl;
//        [self addMsgData:[NSString stringWithFormat:@"input %d address : %s.", i, address]];
//        JUB_FreeMemory(address);
//    }
//    if (JUBR_OK != rv) {
//        return;
//    }
//}
//
//
//void show_address_test_HC(JUB_UINT16 contextID) {
//
//    int change = 0;
//    JUB_UINT64 index = 0;
//    cout << "please input change level (non-zero means 1):" << endl;
//    cin >> change;
//    cout << "please input index " << endl;
//    cin >> index;
//
//    BIP44_Path path;
//    path.change = JUB_ENUM_BOOL(change);
//    path.addressIndex = index;
//
//    JUB_CHAR_PTR address;
//    JUB_RV rv = JUB_GetAddressHC(contextID, path, BOOL_TRUE, &address);
//    if (JUBR_OK != rv) {
//        cout << "JUB_GetAddressHC() return " << GetErrMsg(rv) << endl;
//        return;
//    }
//    cout << "show address is : " << address << endl;
//
//    JUB_FreeMemory(address);
//}
//
//void transactionHC_test(JUB_UINT16 contextID, Json::Value root) {
//
//    JUB_RV rv = JUBR_ERROR;
//
//    rv = [self verify_pin:contextID
//                      pin:[self.userPIN UTF8String]];
//    if (JUBR_OK != rv) {
//        return;
//    }
//
//    rv = transactionHC_proc(contextID, root);
//    if (JUBR_OK != rv) {
//        return;
//    }
//}
//
//
//JUB_RV transactionHC_proc(JUB_UINT16 contextID, Json::Value root) {
//
//    JUB_RV rv = JUBR_ERROR;
//
//    std::vector<INPUT_HC> inputs;
//    std::vector<OUTPUT_HC> outputs;
//    int inputNumber = root["inputs"].size();
//
//    for (int i = 0; i < inputNumber; i++) {
//        INPUT_HC input;
//        input.path.change = (JUB_ENUM_BOOL)root["inputs"][i]["bip32_path"]["change"].asBool();
//        input.path.addressIndex = root["inputs"][i]["bip32_path"]["addressIndex"].asInt();
//        input.amount = root["inputs"][i]["amount"].asUInt64();
//        inputs.push_back(input);
//    }
//
//    int outputNumber = root["outputs"].size();
//
//    for (int i = 0; i < outputNumber; i++) {
//        OUTPUT_HC output;
//        output.changeAddress = (JUB_ENUM_BOOL)root["outputs"][i]["change_address"].asBool();
//        if (output.changeAddress) {
//            output.path.change = (JUB_ENUM_BOOL)root["outputs"][i]["bip32_path"]["change"].asBool();
//            output.path.addressIndex = root["outputs"][i]["bip32_path"]["addressIndex"].asInt();
//        }
//        outputs.push_back(output);
//    }
//
//    char* unsignedRaw = (char*)root["unsigned_tx"].asCString();
//    //NSString* unsignedTx = [NSString stringWithUTF8String:(char*)root["unsigned_tx"].asCString()];
//
//    char* raw = nullptr;
//    rv = JUB_SignTransactionHC(contextID, &inputs[0], (JUB_UINT16)inputs.size(), &outputs[0], (JUB_UINT16)outputs.size(), unsignedRaw, &raw);
//    cout << "JUB_SignTransactionHC() return " << GetErrMsg(rv) << endl;
//
//    if (JUBR_USER_CANCEL == rv) {
//        cout << "User cancel the transaction !" << endl;
//        return rv;
//    }
//    if (   JUBR_OK != rv
//        || nullptr == raw
//        ) {
//        cout << "error sign tx" << endl;
//        return rv;
//    }
//    if (raw) {
//        cout << raw;
//        JUB_FreeMemory(raw);
//    }
//
//    return rv;
//}
//
//
@end
