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


void get_device_info_test(JUB_UINT16 deviceID)
{
	JUB_DEVICE_INFO info;
	JUB_RV rv = Jub_GetDeviceInfo(deviceID, info);
	if (rv != JUBR_OK)
	{
		error_exit("get device info error");
	}

	cout << "device Label :" << info.label << endl;
	cout << "device sn :" << info.sn << endl;
	cout << "device pinRetry :" << info.pin_retry << endl;
	cout << "device pinMaxRetry :" << info.pin_max_retry << endl;
	JUB_BYTE ble_version[5] = { 0 };
	JUB_BYTE fw_version[5] = { 0 };
	memcpy_s(ble_version, 5, info.ble_version, 4);
	memcpy_s(fw_version, 5, info.firmware_version, 4);
	cout << "device ble_version :" << ble_version << endl;
	cout << "device fw_version :" << fw_version << endl;
}


void verify_pin(JUB_UINT16 contextID)
{
	JUB_RV rv = JUBR_ERROR;
	while (rv)
	{
		//输入pin的位置，横着数123456789
		cout << "1 2 3" << endl;
		cout << "4 5 6" << endl;
		cout << "7 8 9" << endl;

		JUB_ShowVirtualPwd(contextID);

		cin.ignore();
		char str[9] = { 0 };
		cin.getline(str, 9);
		cout << str << endl;

		JUB_ULONG retry;
		rv = JUB_VerifyPIN(contextID, str, retry);
		if (rv != JUBR_OK)
		{
			cout << "wrong pin!!" << endl;
		}
	}
}



void get_address_test(JUB_UINT16 contextID, Json::Value root)
{
	verify_pin(contextID);
	try
	{
		int input_number = root["inputs"].size();
		for (int i = 0;i < input_number;i++)
		{
			JUB_CHAR_PTR xpub;
			JUB_RV rv = JUB_GetHDNodeBTC(contextID, root["inputs"][i]["addressIndex"].asInt(), &xpub);
			cout << "input " << i << " xpub : " << xpub << endl;
			JUB_FreeMemory(xpub);

			JUB_CHAR_PTR address;
			rv = JUB_GetAddressBTC(contextID, root["inputs"][i]["addressIndex"].asInt(), BOOL_FALSE, &address);
			if (rv != JUBR_OK)
			{
				error_exit("get address error!");
			}
			cout << "input " << i << " address : " << address << endl;
			JUB_FreeMemory(address);

		}
	}
	catch (...)
	{
		error_exit("Error format json file\n");
	}

}


void transaction_test(JUB_UINT16 contextID, Json::Value root)
{
	verify_pin(contextID);
	try
	{
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



		for (int i = 0; i < output_number; i++)
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


		char* raw = nullptr;
		JUB_RV rv = JUB_SignTransactionBTC(contextID, &inputs[0], (JUB_UINT16)inputs.size(), &outputs[0], (JUB_UINT16)outputs.size(), 0, &raw);

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
	catch (...)
	{
		error_exit("Error format json file\n");
	}
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
	JUB_UINT16 contextID = 0;

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
		Jub_CreateContextBTC(cfg, deviceIDs[0], &contextID);
	}
	catch (...)
	{
		error_exit("Error format json file\n");
	}

	while (true)
	{
		cout << "--------------------------------------" << endl;
		cout << "|******* Jubiter Wallet Test ********|" << endl;
		cout << "| 1. get_device_info_test            |" << endl;
		cout << "| 2. get_address_test.               |" << endl;
		cout << "| 3. transaction_test test.          |" << endl;
		cout << "| 0. exit.                           |" << endl;
		cout << "--------------------------------------" << endl;
		cout << "* Please enter your choice:" << endl;

		int choice = 0;
		cin >> choice;

		switch (choice)
		{
		case 1:
			get_device_info_test(deviceIDs[0]);
			break;
		case 2:
			get_address_test(contextID, root);
			break;
		case 3:
			transaction_test(contextID, root);
			break;
		case 0:
			exit(0);
		default:
			continue;
		}
	}

    return 0;
}

