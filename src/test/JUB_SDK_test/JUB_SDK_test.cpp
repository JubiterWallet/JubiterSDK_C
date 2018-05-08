// JUB_SDK_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../../../include/JUB_SDK.h"


int main()
{
	JUB_UINT16 deviceIDs[MAX_DEVICE] = { 0xffff };
	Jub_ListDeviceHid(deviceIDs);

	Jub_ConnetDeviceHid(deviceIDs[0]);
    return 0;
}

