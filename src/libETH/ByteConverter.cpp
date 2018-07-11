//
// AnyLedger - Created by Bogdan Djukic on 11-04-2018. Inspired by Okada Takahiro, https://github.com/kopanitsa/web3-arduino.
// Licensed under GNU GENERAL PUBLIC LICENSE.
//

#include <libETH/ByteConverter.h>
#include <cstdlib>
#include <vector>
#include <stdio.h>


/*
将10进制字符串转化为16进制字符串

1. 转化为int类型然后运用短除法得出结果
这种方法只能支撑10位数。。。

2. 使用大数运算

*/

inline int compare(string str1, string str2);
string DIVIDE_INT(string str1, string str2, int flag);
string DIV_INT(string str1, string str2);
string MOD_INT(string str1, string str2);

string MUL_INT(string str1, string str2);
string SUB_INT(string str1, string str2);
string ADD_INT(string str1, string str2);



vector<uint8_t> ByteConverter::numberToBytes(uint32_t val) {
    vector<uint8_t> tmp;
    vector<uint8_t> ret;
    if ((uint32_t)(val / 256) >= 0) {
        while ((uint32_t)(val / 256) > 0) {
            tmp.push_back((uint8_t)(val % 256));
            val = (uint32_t)(val / 256);
        }
        tmp.push_back((uint8_t)(val % 256));
        uint8_t len = tmp.size();
        for (int i=0; i<len; i++) {
            ret.push_back(tmp[len-i-1]);
        }
    } else {
        ret.push_back((uint8_t)val);
    }
    return ret;
}

vector<uint8_t> ByteConverter::charStrToBytes(const uint8_t *in) {
    uint32_t ret = 0;
    uint8_t tmp[256];
    strcpy((char *)tmp, (char *)in);
    vector<uint8_t> out;

    // Remove "0x"
    char * ptr = strtok((char*)tmp, "x");
    if (strlen(ptr)!=1) {
        ptr = (char *)tmp;
    } else {
        ptr = strtok(NULL, "x");
    }

    size_t lenstr = strlen(ptr);
    for (int i=0; i<lenstr; i+=2) {
        char c[3];
        c[0] = *(ptr+i);
        c[1] = *(ptr+i+1);
        c[2] = 0x00;
        uint8_t val = strtol(c, nullptr, 16);
        out.push_back(val);
        ret++;
    }
    return out;
}

vector<uint8_t> ByteConverter::stringToBytes(string str) {
    return charStrToBytes((uint8_t*)(str.c_str()));
}

std::string ByteConverter::DecStringToHexString(std::string DecString)
{
	// 运用短除法不断除以16取余数，并将其加入字符串结果中
	std::string result = "";
	std::string s = DecString;
	char _16[] = {
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
	};
	// 16进制，除以16
	const string radix = "16";
	while (s != "0") {
		string tmp = MOD_INT(s, radix);     // 余数
		int n = 0;
		// 将字符串转换为int类型
		for (int i = tmp.length() - 1, j = 1; i >= 0; i--) {
			n += (tmp[i] - '0') * j;
			j *= 10;
		}
		result = _16[n] + result;           // 将余数对应的十六进制数字加入结果
		s = DIV_INT(s, radix);              // 除以16获得商，最为下一轮的被除数
	}

	return "0x" + result;
}

std::string ByteConverter::bytesToString(const vector<uint8_t> buf) {
	std::string ret = "0x";

    for (int i = 0; i < buf.size(); i++) {
      char v[3];
      memset(v, 0, sizeof(v));
      sprintf(v, "%02x", buf[i]);

      ret = ret + std::string(v);
    }

    return ret;
}


inline int compare(string str1, string str2) {
	if (str1.size() > str2.size()) //长度长的整数大于长度小的整数
		return 1;
	else if (str1.size() < str2.size())
		return -1;
	else
		return str1.compare(str2); //若长度相等，从头到尾按位比较，compare函数：相等返回0，大于返回1，小于返回－1
}

string DIVIDE_INT(string str1, string str2, int flag) {//高精度除法
													   //flag = 1时,返回商; flag = 0时,返回余数
	string quotient, residue; //定义商和余数
	int sign1 = 1, sign2 = 1;
	if (str2 == "0") {  //判断除数是否为0
		quotient = "ERROR!";
		residue = "ERROR!";
		if (flag == 1) return quotient;
		else return residue;
	}
	if (str1 == "0") { //判断被除数是否为0
		quotient = "0";
		residue = "0";
	}
	if (str1[0] == '-') {
		str1 = str1.erase(0, 1);
		sign1 *= -1;
		sign2 = -1;
	}
	if (str2[0] == '-') {
		str2 = str2.erase(0, 1);
		sign1 *= -1;
	}
	int res = compare(str1, str2);
	if (res < 0) {
		quotient = "0";
		residue = str1;
	}
	else if (res == 0) {
		quotient = "1";
		residue = "0";
	}
	else {
		string::size_type l1, l2;
		l1 = str1.size(); l2 = str2.size();
		string tempstr;
		tempstr.append(str1, 0, l2 - 1);
		//模拟手工除法
		for (int i = l2 - 1; i < l1; i++) {
			tempstr = tempstr + str1[i];
			for (char ch = '9'; ch >= '0'; ch--) { //试商
				string str;
				str = str + ch;
				if (compare(MUL_INT(str2, str), tempstr) <= 0) {
					quotient = quotient + ch;
					tempstr = SUB_INT(tempstr, MUL_INT(str2, str));
					break;
				}
			}
		}
		residue = tempstr;
	}
	//去除结果中的前导0
	quotient.erase(0, quotient.find_first_not_of('0'));
	if (quotient.empty()) quotient = "0";
	if ((sign1 == -1) && (quotient[0] != '0'))
		quotient = "-" + quotient;
	if ((sign2 == -1) && (residue[0] != '0'))
		residue = "-" + residue;
	if (flag == 1) return quotient;
	else return residue;
}

string DIV_INT(string str1, string str2) {//高精度除法,返回商
	return DIVIDE_INT(str1, str2, 1);
}
string MOD_INT(string str1, string str2) {//高精度除法,返回余数
	return DIVIDE_INT(str1, str2, 0);
}

string SUB_INT(string str1, string str2) {//高精度减法
	string MUL_INT(string str1, string str2);
	int sign = 1; //sign 为符号位
	string str;
	int i;
	if (str2[0] == '-')
		str = ADD_INT(str1, str2.erase(0, 1));
	else {
		int res = compare(str1, str2);
		if (res == 0) return "0";
		if (res < 0) {
			sign = -1;
			string temp = str1;
			str1 = str2;
			str2 = temp;
		}
		string::size_type tempint;
		tempint = str1.size() - str2.size();
		for (i = str2.size() - 1; i >= 0; i--) {
			if (str1[i + tempint] < str2[i]) {
				str1[i + tempint - 1] = char(int(str1[i + tempint - 1]) - 1);
				str = char(str1[i + tempint] - str2[i] + ':') + str;
			}
			else
				str = char(str1[i + tempint] - str2[i] + '0') + str;
		}
		for (i = tempint - 1; i >= 0; i--)
			str = str1[i] + str;
	}
	//去除结果中多余的前导0
	str.erase(0, str.find_first_not_of('0'));
	if (str.empty()) str = "0";
	if ((sign == -1) && (str[0] != '0'))
		str = "-" + str;
	return str;
}

string MUL_INT(string str1, string str2) {//高精度乘法
	int sign = 1; //sign 为符号位
	string str;
	if (str1[0] == '-') {
		sign *= -1;
		str1 = str1.erase(0, 1);
	}
	if (str2[0] == '-') {
		sign *= -1;
		str2 = str2.erase(0, 1);
	}
	int i, j;
	string::size_type l1, l2;
	l1 = str1.size(); l2 = str2.size();
	for (i = l2 - 1; i >= 0; i--) {  //实现手工乘法
		string tempstr;
		int int1 = 0, int2 = 0, int3 = int(str2[i]) - '0';
		if (int3 != 0) {
			for (j = 1; j <= (int)(l2 - 1 - i); j++)
				tempstr = "0" + tempstr;
			for (j = l1 - 1; j >= 0; j--) {
				int1 = (int3 * (int(str1[j]) - '0') + int2) % 10;
				int2 = (int3 * (int(str1[j]) - '0') + int2) / 10;
				tempstr = char(int1 + '0') + tempstr;
			}
			if (int2 != 0) tempstr = char(int2 + '0') + tempstr;
		}
		str = ADD_INT(str, tempstr);
	}
	//去除结果中的前导0
	str.erase(0, str.find_first_not_of('0'));
	if (str.empty()) str = "0";
	if ((sign == -1) && (str[0] != '0'))
		str = "-" + str;
	return str;
}

string ADD_INT(string str1, string str2) {//高精度加法
	string SUB_INT(string str1, string str2);
	int sign = 1; //sign 为符号位
	string str;
	if (str1[0] == '-') {
		if (str2[0] == '-') {
			sign = -1;
			str = ADD_INT(str1.erase(0, 1), str2.erase(0, 1));
		}
		else {
			str = SUB_INT(str2, str1.erase(0, 1));
		}
	}
	else {
		if (str2[0] == '-')
			str = SUB_INT(str1, str2.erase(0, 1));
		else {
			//把两个整数对齐，短整数前面加0补齐
			string::size_type l1, l2;
			int i;
			l1 = str1.size(); l2 = str2.size();
			if (l1 < l2) {
				for (i = 1; i <= l2 - l1; i++)
					str1 = "0" + str1;
			}
			else {
				for (i = 1; i <= l1 - l2; i++)
					str2 = "0" + str2;
			}
			int int1 = 0, int2 = 0; //int2 记录进位
			for (i = str1.size() - 1; i >= 0; i--) {
				int1 = (int(str1[i]) - '0' + int(str2[i]) - '0' + int2) % 10;
				int2 = (int(str1[i]) - '0' + int(str2[i]) - '0' + int2) / 10;
				str = char(int1 + '0') + str;
			}
			if (int2 != 0) str = char(int2 + '0') + str;
		}
	}
	//运算后处理符号位
	if ((sign == -1) && (str[0] != '0'))
		str = "-" + str;
	return str;
}
