// JUB_SDK_test.cpp : Defines the entry point for the console application.
//

#ifdef _WIN32
#include "stdafx.h"
#endif

#include "../../include/JUB_SDK.h"
#include <vector>
#include <iostream>
#include <json/json.h>
#include <iostream>  
#include <fstream>  
#include <string>
#include <sstream>
#include <iostream>
#include <cstring>
using namespace std;

using std::getline;
using std::istringstream;


vector<string> split(const string &str, char delim, bool skip_empty = true) {
	istringstream iss(str);
	vector<string> elems;
	for (string item; getline(iss, item, delim); )
		if (skip_empty && item.empty()) continue;
		else elems.push_back(item);
	return elems;
}

void error_exit(char* message)
{
	cout << message << endl;
	cout << "press any key to exit" << endl;
	char str[9] = { 0 };
	cin >> str;
	//exit(0);
}
void main_test();

void get_device_info_test()
{

	JUB_UINT16 deviceIDs[MAX_DEVICE] = { 0xffff };
	JUB_ListDeviceHid(deviceIDs);


	JUB_RV rv = JUB_ConnetDeviceHid(deviceIDs[0]);
	if (rv != JUBR_OK)
	{
		error_exit("cannot find JubtierWallet");
	}

	JUB_UINT16 deviceID = deviceIDs[0];


	char* applist;
	JUB_EnumApplets(deviceID, &applist);
	std::string str_applist = applist;
	JUB_FreeMemory(applist);

	auto v_applist = split(str_applist, ' ');

	for (auto appid : v_applist)
	{
		char* version;
		auto rv = JUB_GetAppletVersion(deviceID,(char*)appid.c_str(),&version);
		if (rv == JUBR_OK)
		{
			cout << appid << " version : " << version << endl;;
		}
	}


	JUB_DEVICE_INFO info;
	rv = JUB_GetDeviceInfo(deviceID, info);
	if (rv != JUBR_OK)
	{
		cout << "get device info error" << endl;
		return;
	}

	cout << "device Label :" << info.label << endl;
	cout << "device sn :" << info.sn << endl;
	cout << "device pinRetry :" << info.pin_retry << endl;
	cout << "device pinMaxRetry :" << info.pin_max_retry << endl;
	JUB_BYTE ble_version[5] = { 0 };
	JUB_BYTE fw_version[5] = { 0 };
	memcpy(ble_version, info.ble_version, 4);
	memcpy(fw_version, info.firmware_version, 4);
	cout << "device ble_version :" << ble_version << endl;
	cout << "device fw_version :" << fw_version << endl;


	char* cert;
	rv = JUB_GetDeviceCert(deviceID, &cert);
	if (rv != JUBR_OK)
	{
		cout << " JUB_GetDeviceCert error " << endl;
		return;
	}

	cout << "device cert is :" << cert << endl;
	JUB_FreeMemory(cert);
}


void set_timeout_test(JUB_UINT16 contextID)
{

	cout << "* Please enter timeout in second ( < 600 ):" << endl;

	int timeout = 0;
	cin >> timeout;
	JUB_SetTimeOut(contextID,timeout);

}

void send_apud_test()
{
	JUB_UINT16 deviceIDs[MAX_DEVICE] = { 0xffff };
	JUB_ListDeviceHid(deviceIDs);


	JUB_RV rv = JUB_ConnetDeviceHid(deviceIDs[0]);
	if (rv != JUBR_OK)
	{
		error_exit("cannot find JubtierWallet");
	}

	JUB_UINT16 deviceID = deviceIDs[0];

	cout << "please input apdu in hex:" << endl;
	char apdu[4096+6] = { 0 };
	cin >> apdu;

	char* response = nullptr;
	rv = JUB_SendOneApdu(deviceID, apdu, &response);

	if (rv != JUBR_OK)
	{
		cout << "device error";
		return;
	}
	
	cout << response <<endl;
	JUB_FreeMemory(response);
	return;
}



void verify_pin(JUB_UINT16 contextID)
{
	JUB_RV rv = JUBR_ERROR;
	while (rv)
	{
		//����pin��λ�ã�������123456789
		cout << "1 2 3" << endl;
		cout << "4 5 6" << endl;
		cout << "7 8 9" << endl;


		cout << "to cancel the virtualpwd iput 'c'" << endl;
		JUB_ShowVirtualPwd(contextID);


 		char str[9] = { 0 };


		cin >> str;
		cout << str << endl;

		if (str[0] == 'c' || str[0] == 'C')
		{
			cout << "cancel the VirtualPwd "<< endl;
			JUB_CancelVirtualPwd(contextID);
			return;
		}

		JUB_ULONG retry;
		rv = JUB_VerifyPIN(contextID, str, retry);
		if (rv != JUBR_OK)
		{
			cout << "wrong pin!! pin retry : " << retry << endl;
		}
	}
}


void show_address_test(JUB_UINT16 contextID)
{
	int change = 0;
	JUB_UINT64 index = 0;
	cout << "please input change level (non-zero means 1):" << endl;
	cin >> change;
	cout << "please input index " << endl;
	cin >> index;

	BIP32_Path path;
	path.change = JUB_ENUM_BOOL(change);
	path.addressIndex = index;

	JUB_CHAR_PTR address;
	JUB_RV rv = JUB_GetAddressBTC(contextID, path, BOOL_TRUE, &address);
	if (rv != JUBR_OK)
	{
		cout << "show address error" << endl;
		return;
	}
	cout << "show address is : " << address << endl;
	JUB_FreeMemory(address);
}


void set_my_address_test_BTC(JUB_UINT16 contextID)
{
	verify_pin(contextID);
	int change = 0;
	JUB_UINT64 index = 0;
	cout << "please input change level (non-zero means 1):" << endl;
	cin >> change;
	cout << "please input index " << endl;
	cin >> index;

	BIP32_Path path;
	path.change = JUB_ENUM_BOOL(change);
	path.addressIndex = index;

	JUB_CHAR_PTR address = "";
	JUB_RV rv = JUB_SetMyAddressBTC(contextID, path, &address);
	if (rv != JUBR_OK)
	{
		cout << "set address error" << endl;
	}
	else
	{
		cout << "set my address is : " << address << endl;
		JUB_FreeMemory(address);
	}

}


void set_my_address_test_ETH(JUB_UINT16 contextID)
{
	verify_pin(contextID);
	int change = 0;
	JUB_UINT64 index = 0;
	cout << "please input change level (non-zero means 1):" << endl;
	cin >> change;
	cout << "please input index " << endl;
	cin >> index;

	BIP32_Path path;
	path.change = JUB_ENUM_BOOL(change);
	path.addressIndex = index;

	JUB_CHAR_PTR address = "";
	JUB_RV rv = JUB_SetMyAddressETH(contextID, path, &address);
	if (rv != JUBR_OK)
	{
		cout << "set address error" << endl;
	}
	else
	{
		cout << "set my address is : " << address << endl;
		JUB_FreeMemory(address);
	}

}


void get_address_test(JUB_UINT16 contextID, Json::Value root)
{

	try
	{
		JUB_CHAR_PTR main_xpub;
		JUB_RV rv = JUB_GetMainHDNodeBTC(contextID, &main_xpub);

		cout << "Main xpub : " << main_xpub << endl;
		JUB_FreeMemory(main_xpub);



		int input_number = root["inputs"].size();
		for (int i = 0;i < input_number;i++)
		{
			JUB_CHAR_PTR xpub;

			BIP32_Path path;
			path.change = (JUB_ENUM_BOOL)root["inputs"][i]["bip32_path"]["change"].asBool();
			path.addressIndex = root["inputs"][i]["bip32_path"]["addressIndex"].asInt();

			JUB_RV rv = JUB_GetHDNodeBTC(contextID, path, &xpub);
			cout << "input " << i << " xpub : " << xpub << endl;
			JUB_FreeMemory(xpub);

			JUB_CHAR_PTR address;
			rv = JUB_GetAddressBTC(contextID, path, BOOL_FALSE, &address);
			if (rv != JUBR_OK)
			{
				cout << "get address error" << endl;
				return;
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
	JUB_BTC_UNIT_TYPE unit = mBTC;

	cout << "Please input BTCunit on JubiterBLD" << endl;
	cout << "1: BTC" << endl;
	cout << "2: cBTC" << endl;
	cout << "3: mBTC" << endl;
	cout << "4: uBTC" << endl;
	cout << "5: Satoshi" << endl;

	int choice = 0;
	cin >> choice;

	switch (choice) {
	case 1:
		unit = BTC;
		break;
	case 2:
		unit = cBTC;
		break;
	case 3:
		unit = mBTC;
		break;
	case 4:
		unit = uBTC;
		break;
	case 5:
		unit = Satoshi;
		break;
	}

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
			input.path.change = (JUB_ENUM_BOOL)root["inputs"][i]["bip32_path"]["change"].asBool();
			input.path.addressIndex = root["inputs"][i]["bip32_path"]["addressIndex"].asInt();
			input.amount = root["inputs"][i]["amount"].asUInt64();
			inputs.push_back(input);
		}


		int output_number = root["outputs"].size();



		for (int i = 0; i < output_number; i++)
		{
			OUTPUT_BTC output;
			output.address = (char*)root["outputs"][i]["address"].asCString();
			output.amount = root["outputs"][i]["amount"].asUInt64();
			output.change_address = (JUB_ENUM_BOOL)root["outputs"][i]["change_address"].asBool();
			if (output.change_address)
			{
				output.path.change = (JUB_ENUM_BOOL)root["outputs"][i]["bip32_path"]["change"].asBool();
				output.path.addressIndex = root["outputs"][i]["bip32_path"]["addressIndex"].asInt();
			}
			outputs.push_back(output);
		}


		JUB_SetUnitBTC(contextID, unit);


		char* raw = nullptr;
		JUB_RV rv = JUB_SignTransactionBTC(contextID, &inputs[0], (JUB_UINT16)inputs.size(), &outputs[0], (JUB_UINT16)outputs.size(), 0, &raw);

		if (rv == JUBR_USER_CANCEL)
		{
			cout << "User cancel the transaction !" << endl;
			return;
		}
		if (rv != JUBR_OK || raw == nullptr)
		{
			cout << "error sign tx" << endl;
			return;

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

void BTC_test(char* json_file, JUB_ENUM_COINTYPE_BTC cointype)
{
	JUB_UINT16 deviceIDs[MAX_DEVICE] = { 0xffff };
	JUB_ListDeviceHid(deviceIDs);

	JUB_RV rv = JUB_ConnetDeviceHid(deviceIDs[0]);
	if (rv != JUBR_OK)
	{
		error_exit("cannot find JubtierWallet");
	}

	char* applist;
	rv = JUB_EnumApplets(deviceIDs[0], &applist);

	Json::CharReaderBuilder builder;
	Json::Value root;
	ifstream in(json_file, ios::binary);
	if (!in.is_open())
	{
		error_exit("Error opening json file\n");
	}
	JSONCPP_STRING errs;
	if (!parseFromStream(builder, in, &root, &errs))
	{
		error_exit("Error parse json file\n");
	}
	JUB_UINT16 contextID = 0;

	try
	{
		CONTEXT_CONFIG_BTC cfg;
		cfg.main_path = (char*)root["main_path"].asCString();
		cfg.cointype = cointype;

		if (cointype == COINBCH)
		{
			cfg.transtype = p2pkh;
		}
		else
		{
			if (root["p2sh-segwit"].asBool())
			{
				cfg.transtype = p2sh_p2wpkh;
			}
			else
				cfg.transtype = p2pkh;
		}


		JUB_CreateContextBTC(cfg, deviceIDs[0], &contextID);
	}
	catch (...)
	{
		error_exit("Error format json file\n");
	}


	while (true)
	{
		cout << "--------------------------------------" << endl;
		cout << "|******* Jubiter Wallet BTC  ********|" << endl;
		cout << "| 1. get_address_test.               |" << endl;
		cout << "| 2. show_address_test.              |" << endl;
		cout << "| 3. transaction_test.               |" << endl;
		cout << "| 4. set_my_address_test.            |" << endl;
		cout << "| 5. set_timeout_test.               |" << endl;
		cout << "| 0. return.                         |" << endl;
		cout << "--------------------------------------" << endl;
		cout << "* Please enter your choice:" << endl;

		int choice = 0;
		cin >> choice;

		switch (choice)
		{
		case 1:
			get_address_test(contextID, root);
			break;
		case 2:
			show_address_test(contextID);
			break;
		case 3:
			transaction_test(contextID, root);
			break;
		case 4:
			set_my_address_test_BTC(contextID);
			break;
		case 5:
			set_timeout_test(contextID);
			break;
		case 0:
			main_test();
		default:
			continue;
		}
	}
}


void get_address_pubkey_ETH(JUB_UINT16 contextID)
{
	int change = 0;
	JUB_UINT64 index = 0;
	cout << "please input change level (non-zero means 1):" << endl;
	cin >> change;
	cout << "please input index " << endl;
	cin >> index;

	BIP32_Path path;
	path.change = JUB_ENUM_BOOL(change);
	path.addressIndex = index;


	char* pubkey = nullptr;
	JUB_RV rv = JUB_GetMainHDNodeETH(contextID,HEX,&pubkey);
	if (rv != JUBR_OK)
	{
		cout << "JUB_GetMainHDNodeETH  error!" << endl;
		return;
	}

	cout << "MainXpub in  hex format :  " << pubkey << endl;
	JUB_FreeMemory(pubkey);



	pubkey = nullptr;
	rv = JUB_GetMainHDNodeETH(contextID, XPUB, &pubkey);
	if (rv != JUBR_OK)
	{
		cout << "JUB_GetMainHDNodeETH  error!" << endl;
		return;
	}

	cout << "MainXpub in  xpub format :  " << pubkey << endl;
	JUB_FreeMemory(pubkey);


	pubkey = nullptr;
	rv = JUB_GetHDNodeETH(contextID,HEX,path, &pubkey);
	if (rv != JUBR_OK)
	{
		cout << "JUB_GetHDNodeETH  error!" << endl;
		return;
	}

	cout << "pubkey in  hex format :  "<< pubkey << endl;
	JUB_FreeMemory(pubkey);


	pubkey = nullptr;
	rv = JUB_GetHDNodeETH(contextID,XPUB,path, &pubkey);
	if (rv != JUBR_OK)
	{
		cout << "JUB_GetHDNodeETH  error!" << endl;
		return;
	}

	cout << "pubkey in xpub format :  " << pubkey << endl;
	JUB_FreeMemory(pubkey);


	char* address = nullptr;
	rv = JUB_GetAddressETH(contextID, path, BOOL_TRUE, &address);

	if (rv != JUBR_OK)
	{
		cout << "JUB_GetAddressETH  error!" << endl;
		return;
	}
	cout << "address: " <<address << endl;
	JUB_FreeMemory(address);
}


void transaction_test_ETH(JUB_UINT16 contextID,Json::Value root)
{

	verify_pin(contextID);


	BIP32_Path path;
	path.change = (JUB_ENUM_BOOL)root["ETH"]["bip32_path"]["change"].asBool();
	path.addressIndex = root["ETH"]["bip32_path"]["addressIndex"].asUInt();


	//ETH Test

	uint32_t nonce = root["ETH"]["nonce"].asDouble();
	uint32_t gasLimit = root["ETH"]["gasLimit"].asDouble();
	char* gasPriceInWei = (char*)root["ETH"]["gasPriceInWei"].asCString();
	char* valueInWei = (char*)root["ETH"]["valueInWei"].asCString();
	char* to = (char*)root["ETH"]["to"].asCString();
	char* data = (char*)root["ETH"]["data"].asCString();


	char* raw = nullptr;
	JUB_RV rv = JUB_SignTransactionETH(contextID, path, nonce, gasLimit, gasPriceInWei, to, valueInWei, data, &raw);
	if (rv != JUBR_OK)
	{
		cout << "JUB_SignTransactionETH Error!" << endl;
	}
	else
	{
		cout << "raw : " << raw << endl;
		JUB_FreeMemory(raw);
	}


}


void transaction_ERC20_ETH(JUB_UINT16 contextID, Json::Value root)
{

	verify_pin(contextID);
	//ERC-20 Test

	uint32_t nonce = root["ERC20"]["nonce"].asDouble();
	uint32_t gasLimit = root["ERC20"]["gasLimit"].asDouble();
	char* gasPriceInWei = (char*)root["ERC20"]["gasPriceInWei"].asCString();
	char* to = (char*)root["ERC20"]["contract_address"].asCString();
	char* token_to = (char*)root["ERC20"]["token_to"].asCString();
	char* token_value = (char*)root["ERC20"]["token_value"].asCString();

	BIP32_Path path;
	path.change = (JUB_ENUM_BOOL)root["ERC20"]["bip32_path"]["change"].asBool();
	path.addressIndex = root["ERC20"]["bip32_path"]["addressIndex"].asUInt();

	char* abi = nullptr;
	JUB_RV rv = JUB_BuildERC20AbiETH(contextID, token_to, token_value, &abi);

	char* raw = nullptr;
	rv = JUB_SignTransactionETH(contextID, path, nonce, gasLimit, gasPriceInWei, to, 0, abi, &raw);

	JUB_FreeMemory(abi);
	if (rv != JUBR_OK)
	{
		cout << "JUB_SignTransactionETH for ERC20 Error!" << endl;
	}
	else
	{
		cout << raw << endl;
		JUB_FreeMemory(raw);
	}

}

void ETH_test()
{


	JUB_UINT16 deviceIDs[MAX_DEVICE] = { 0xffff };
	JUB_ListDeviceHid(deviceIDs);

	JUB_RV rv = JUB_ConnetDeviceHid(deviceIDs[0]);
	if (rv != JUBR_OK)
	{
		error_exit("cannot find JubtierWallet");
	}

	char* applist;
	rv = JUB_EnumApplets(deviceIDs[0], &applist);



	Json::CharReaderBuilder builder;
	Json::Value root;
	ifstream in("testETH.json", ios::binary);
	if (!in.is_open())
	{
		error_exit("Error opening json file\n");
	}
	JSONCPP_STRING errs;
	if (!parseFromStream(builder,in, &root , &errs))
	{
		error_exit("Error parse json file\n");
	}

	CONTEXT_CONFIG_ETH cfg;
	cfg.main_path = (char*)root["main_path"].asCString();
	cfg.chainID = root["chainID"].asInt();
	JUB_UINT16 contextID = 0;
	rv = JUB_CreateContextETH(cfg, deviceIDs[0], &contextID);


	while (true)
	{
		cout << "--------------------------------------" << endl;
		cout << "|******* Jubiter Wallet ETH  ********|" << endl;
		cout << "| 1. show_address_pubkey_test.       |" << endl;
		cout << "| 2. transaction_test.               |" << endl;
		cout << "| 3. transaction_ERC20_test.         |" << endl;
		cout << "| 4. set_my_address_test.            |" << endl;
		cout << "| 5. set_timeout_test.               |" << endl;
		cout << "| 0. return.                         |" << endl;
		cout << "--------------------------------------" << endl;
		cout << "* Please enter your choice:" << endl;


		int choice = 0;
		cin >> choice;

		switch (choice)
		{
		case 1:
			get_address_pubkey_ETH(contextID);
			break;
		case 2:
			transaction_test_ETH(contextID,root);
			break;
		case 3:
			transaction_ERC20_ETH(contextID, root);
			break;
		case 4:
			set_my_address_test_ETH(contextID);
			break;
		case 5:
			set_timeout_test(contextID);
			break;
		case 0:
			main_test();
		default:
			continue;
		}

	}
}

void getVersion()
{
	cout << "~~~~~~~~~~~Device Version ~~~~~~~~~~~~~~" << endl;

	JUB_UINT16 deviceIDs[MAX_DEVICE] = { 0xffff };
	JUB_ListDeviceHid(deviceIDs);


	JUB_RV rv = JUB_ConnetDeviceHid(deviceIDs[0]);
	if (rv != JUBR_OK)
	{
		error_exit("cannot find JubtierWallet");
	}

	JUB_UINT16 deviceID = deviceIDs[0];

	JUB_DEVICE_INFO info;
	rv = JUB_GetDeviceInfo(deviceID, info);
	if (rv != JUBR_OK)
	{
		cout << "get device info error" << endl;
		return;
	}


	JUB_BYTE ble_version[5] = { 0 };
	JUB_BYTE fw_version[5] = { 0 };
	memcpy(ble_version, info.ble_version, 4);
	memcpy(fw_version, info.firmware_version, 4);
	cout << "device ble_version :" << ble_version << endl;
	cout << "device fw_version :" << fw_version << endl;

	cout << "~~~~~~~~~~~Applet Version ~~~~~~~~~~~~~~" << endl;


	char* applist;
	JUB_EnumApplets(deviceID, &applist);
	std::string str_applist = applist;
	JUB_FreeMemory(applist);

	auto v_applist = split(str_applist, ' ');

	for (auto appid : v_applist)
	{
		char* version;
		auto rv = JUB_GetAppletVersion(deviceID, (char*)appid.c_str(), &version);
		if (rv == JUBR_OK)
		{
			cout << appid << " version : " << version << endl;;
		}
	}

	cout << "~~~~~~~~~~~SDK    Version ~~~~~~~~~~~~~~" << endl;



	cout <<"SDK Version:"<< JUB_GetVersion() << endl;



}

void main_test()
{

	JUB_UINT16 deviceIDs[MAX_DEVICE] = { 0xffff };
	JUB_ListDeviceHid(deviceIDs);


	JUB_RV rv = JUB_ConnetDeviceHid(deviceIDs[0]);
	if (rv != JUBR_OK)
	{
		error_exit("cannot find JubtierWallet");
	}
	

	auto deviceID = deviceIDs[0];

	while (true)
	{
		cout << "--------------------------------------" << endl;
		cout << "|******* Jubiter Wallet Test ********|" << endl;
		cout << "| 1. get_device_info_test            |" << endl;
		cout << "| 2. send_one_apdu_test.             |" << endl;
		cout << "| 3. BTC_test.                       |" << endl;
		cout << "| 4. BCH_test.                       |" << endl;
		cout << "| 5. LTC_test.                       |" << endl;
		cout << "| 6. ETH_test & ETC_test.            |" << endl;
		cout << "| 99. get_version.                   |" << endl;
		cout << "| 0. exit.                           |" << endl;
		cout << "--------------------------------------" << endl;
		cout << "* Please enter your choice:" << endl;

		JUB_DisconnetDeviceHid(deviceIDs[0]);

		int choice = 0;
		cin >> choice;

		switch (choice)
		{
		case 1:
			get_device_info_test();
			break;
		case 2:
			send_apud_test();
			break;
		case 3:
			BTC_test("test.json",COINBTC);
			break;
		case 4:
			BTC_test("testBCH.json",COINBCH);
			break;
		case 5:
			BTC_test("testLTC.json", COINLTC);
			break;
		case 6:
			ETH_test();
			break;
		case 99:
			getVersion();
			break;

		case 0:
			exit(0);
		default:
			continue;
		}

	}

}
#ifdef _WIN32 
#include <windows.h> 

#else 

#endif

void monitor_test()
{
	while (true)
	{
		JUB_UINT16 deviceIDs[MAX_DEVICE] = { 0 };
		fill_n(deviceIDs, MAX_DEVICE, 0xffff);
		auto count = 0;
		JUB_RV rv = JUB_ListDeviceHid(deviceIDs);
		for (auto id : deviceIDs)
		{
			if (id != 0xffff)
			{
				count++;
			}
		}
		cout << count << endl;
#ifdef _WIN32 
		Sleep(1000);
#else 
		usleep(100 * 1000);
#endif
	}
}

int main()
{
	//monitor_test();
	main_test();
    return 0;
}

