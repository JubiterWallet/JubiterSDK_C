#ifndef __DeviceIOLogHelper__
#define __DeviceIOLogHelper__


#include <JUB_SDK.h>
#include <utility/uchar_vector.h>

#define RESET "\033[0m"
#define RED "\033[31m"
#define ORANGE "\033[33m"
#define GRAY "\033[90m"
#define Red "\033[31m"
#define Green "\033[32m"

#define Bold  "\033[1m"
#define Dim  "\033[2m"
#define Underlined  "\033[4m"
#define Blink  "\033[5m"
#define Reverse  "\033[7m"
#define Hidden  "\033[8m"

#define BackgroundDarkGray "\033[100m"

class DeviceIOLogHelper
{
public:
	DeviceIOLogHelper(JUB_BYTE_CPTR sendData,
		JUB_ULONG sendLen,
		JUB_BYTE_PTR pRetData,
		JUB_ULONG_PTR pulRetLen) {

		_sendData = sendData;
		_sendLen = sendLen;
		_pRetData = pRetData;
		_pulRetLen = pulRetLen;

	}
	~DeviceIOLogHelper()
	{
		JUB_UINT16 wRet = _pRetData[*_pulRetLen - 2] * 0x100 + _pRetData[*_pulRetLen - 1];
		auto style = Green ;
		if (wRet != 0x9000)
		{
			style = Red;
		}

		uchar_vector logSend(_sendData, _sendData + _sendLen);
		std::cout << GRAY <<"<<" << style << logSend.getHex() << std::endl;


		uchar_vector logRecv(_pRetData, _pRetData + *_pulRetLen);
		std::cout << GRAY <<">>" << style << logRecv.getHex() << std::endl;

		std::cout << "------------------------------" << std::endl;

		std::cout << RESET;
	}

private:
	JUB_BYTE_CPTR   _sendData;
	JUB_ULONG       _sendLen;
	JUB_BYTE_PTR    _pRetData;
	JUB_ULONG_PTR   _pulRetLen;

};











#endif //DeviceIOLogHelper

