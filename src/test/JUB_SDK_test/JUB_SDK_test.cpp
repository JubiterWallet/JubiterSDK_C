// JUB_SDK_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../../../include/JUB_SDK.h"


int main()
{
	JUB_UINT16 deviceIDs[MAX_DEVICE] = { 0xffff };
	Jub_ListDeviceHid(deviceIDs);

	Jub_ConnetDeviceHid(deviceIDs[0]);


	CONTEXT_CONFIG_BTC cfg;
	cfg.main_path = "m/44'/0'/0'/0/0";
	cfg.forkID = 0;
	cfg.type = JUB_BTC_TRANS_TYPE::p2pkh;


	JUB_UINT16 contextID = 0;
	Jub_CreateContextBTC(cfg, deviceIDs[0], &contextID);
    return 0;
}

