// JUB_SDK_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../../../include/JUB_SDK.h"
#include <vector>
#include <iostream>
#include <json/json.h>
#include <iostream>  
#include <fstream>  
using namespace std;

void error_exit(char* message)
{
	cout << message << endl;
	getchar();
	exit(0);
}

int main()
{

	JUB_UINT16 deviceIDs[MAX_DEVICE] = { 0xffff };
	Jub_ListDeviceHid(deviceIDs);

	JUB_RV rv = Jub_ConnetDeviceHid(deviceIDs[0]);
	if (rv != JUBR_OK)
	{
		error_exit("cannot find JubtierWallet");
	}

	Json::Reader reader;
	Json::Value root;
	ifstream in("test.json", ios::binary);
	if (!in.is_open())
	{
		error_exit("Error opening json file\n");
	}


	if (!reader.parse(in, root))
	{
		error_exit("Error parse json file\n");
	}

	try
	{
		CONTEXT_CONFIG_BTC cfg;
		cfg.main_path = (char*)root["main_path"].asCString();
		cfg.forkID = 0;
		if (root["p2sh-segwit"].asBool())
		{
			cfg.type = p2sh_p2wpkh;
		}
		else
			cfg.type = p2pkh;


		JUB_UINT16 contextID = 0;
		Jub_CreateContextBTC(cfg, deviceIDs[0], &contextID);


		std::vector<INPUT_BTC> inputs;
		std::vector<OUTPUT_BTC> outputs;
		int input_number = root["inputs"].size();

		for (int i = 0;i < input_number;i++)
		{
			INPUT_BTC input;
			input.preHash = (char*)root["inputs"][i]["preHash"].asCString();
			input.preIndex = root["inputs"][i]["preIndex"].asInt();
			input.addressIndex = root["inputs"][i]["addressIndex"].asInt();
			input.amount = root["inputs"][i]["amount"].asInt();
			inputs.push_back(input);
		}


		int output_number = root["outputs"].size();



		for (int i = 0; i< output_number; i++)
		{
			OUTPUT_BTC output;
			output.address = (char*)root["outputs"][i]["address"].asCString();
			output.amount = root["outputs"][i]["amount"].asInt();
			output.change = root["outputs"][i]["change"].asBool();
			if (output.change)
			{
				output.path = (char*)root["outputs"][i]["path"].asCString();
			}
			outputs.push_back(output);
		}

		JUB_RV  rv = JUBR_ERROR;
		while (rv)
		{
			JUB_ShowVirtualPwd(contextID);
			//输入pin的位置，横着数123456789
			cout << "1 2 3" << endl;
			cout << "4 5 6" << endl;
			cout << "7 8 9" << endl;


			char str[9];
			cin.getline(str, 9);
			cout << str << endl;

			JUB_ULONG retry;
			rv = JUB_VerifyPIN(contextID, str, retry);
			if (rv != JUBR_OK)
			{
				cout << "wrong pin!!" << endl;
			}
		}


		char* raw = nullptr;
		rv = JUB_SignTransactionBTC(contextID, &inputs[0], (JUB_UINT16)inputs.size(), &outputs[0], (JUB_UINT16)outputs.size(), 0, &raw);

		JUB_ClearContext(contextID);
		if (rv != JUBR_OK || raw == nullptr)
		{
			error_exit("error sign tx");
		}
		if (raw)
		{ 
			cout << raw;
			JUB_FreeMemory(raw);
		}
			

	}
	catch (const std::exception&)
	{
		error_exit("Error format json file\n");
	}

	getchar();
    return 0;
}

