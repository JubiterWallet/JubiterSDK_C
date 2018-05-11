// JUB_SDK_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../../../include/JUB_SDK.h"
#include <vector>
#include <iostream>
using namespace std;

int main()
{

	JUB_UINT16 deviceIDs[MAX_DEVICE] = { 0xffff };
	Jub_ListDeviceHid(deviceIDs);

	Jub_ConnetDeviceHid(deviceIDs[0]);


	CONTEXT_CONFIG_BTC cfg;
	cfg.main_path = "m/44'/0'/0'";
	cfg.forkID = 0;
	cfg.type = JUB_BTC_TRANS_TYPE::p2pkh;

	JUB_UINT16 contextID = 0;
	Jub_CreateContextBTC(cfg, deviceIDs[0], &contextID);


	JUB_ShowVirtualPwd(contextID);

	//输入pin的位置，横着数123456789
	cout << "1 2 3" << endl;
	cout << "4 5 6" << endl;
	cout << "7 8 9" << endl;


	JUB_RV  rv = JUBR_ERROR;
	while (rv)
	{
		char str[9];
		cin.getline(str, 9);
		cout << str << endl;

		JUB_ULONG retry;
		rv = JUB_VerifyPIN(contextID, str, retry);
	}

	std::vector<INPUT_BTC> inputs;
	std::vector<OUTPUT_BTC> outputs;

	INPUT_BTC input1;
	input1.preHash = "6b19d2b4c9d7e0dc177077fcdd23c5a224a14246a4f728ea3244a08969b41385";
	input1.addressIndex = 0x00;
	input1.preIndex = 0x01;
	input1.amount = 10000;
	inputs.push_back(input1);


	OUTPUT_BTC output1;
	output1.address = "18MSnVZFj6hcsWxAM2qL88XHzRFRZCN49u";
	output1.amount = 5000;
	output1.change = false;
	outputs.push_back(output1);

	char* raw = nullptr;
	JUB_SignTransactionBTC(contextID, &inputs[0], (JUB_UINT16)inputs.size(), &outputs[0], (JUB_UINT16)outputs.size(), 0, &raw);

	if (raw)
		delete raw;
    return 0;
}

