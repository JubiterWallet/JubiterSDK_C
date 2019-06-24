//
// AnyLedger - Created by Bogdan Djukic on 11-04-2018. Inspired by Okada Takahiro, https://github.com/kopanitsa/web3-arduino.
// Licensed under GNU GENERAL PUBLIC LICENSE.
//

#include <libETH/ByteConverter.h>
#include <cstdlib>
#include <vector>
#include <stdio.h>

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
    if (0 <= (uint32_t)(val / 256)) {
        while ((uint32_t)(val / 256) > 0) {
            tmp.push_back((uint8_t)(val % 256));
            val = (uint32_t)(val / 256);
        }
        tmp.push_back((uint8_t)(val % 256));
        uint8_t len = tmp.size();
        for (int i=0; i<len; i++) {
            ret.push_back(tmp[len-i-1]);
        }
    }
    else {
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
    if (1 != strlen(ptr)) {
        ptr = (char *)tmp;
    }
    else {
        ptr = strtok(NULL, "x");
    }

    size_t lenstr = strlen(ptr);
	char tmpeven[256] = {0,};
	if (0 != (lenstr % 2)) {
		tmpeven[0] = 0x30;
		strcpy(&tmpeven[1], ptr);
		lenstr += 1;
	}
    else {
		strcpy(tmpeven, ptr);
    }

    for (int i=0; i<lenstr; i+=2) {
        char c[3];
        c[0] = *(tmpeven +i);
        c[1] = *(tmpeven +i+1);
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

std::string ByteConverter::DecStringToHexString(std::string DecString) {
	// use short division to divide by 16 and add the remainder to the string result
	std::string result = "";
	std::string s = DecString;
	char _16[] = {
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
	};

	// hexadecimal divided by 16
	const string radix = "16";
	while (s != "0") {
		string tmp = MOD_INT(s, radix);     // remainder
		int n = 0;
		// converts a string to int
		for (unsigned long i = tmp.length() - 1, j = 1; i >= 0; i--) {
			n += (tmp[i] - '0') * j;
			j *= 10;
		}
		result = _16[n] + result;           // The hexadecimal digit corresponding to
                                            // the remainder is added to the result
		s = DIV_INT(s, radix);              // Divide by 16 to get the quotient,
                                            // the dividend of the next round
	}

	return "0x" + result;
}

std::string ByteConverter::bytesToString(const vector<uint8_t> buf) {

	std::string ret = "0x";

    for (int i = 0; i < buf.size(); i++) {
        char v[3];
        memset(v, 0, sizeof(v)/sizeof(char));
        sprintf(v, "%02x", buf[i]);

        ret = ret + std::string(v);
    }

    return ret;
}

inline int compare(string str1, string str2) {

    // Integers with long lengths are greater than integers with small lengths
    if (str1.size() > str2.size()) {
		return 1;
    }
    else if (str1.size() < str2.size()) {
		return -1;
    }
    // If the length is the same, compare it from start to finish.
    // The compare function returns 0, greater than 1, and less than -1
    else {
		return str1.compare(str2);
    }
}

// High precision division
// Flag=1, return to quotient; When flag=0, the remainder is returned
string DIVIDE_INT(string str1, string str2, int flag) {

	string quotient, residue;
	int sign1 = 1, sign2 = 1;

    // Determine if the divisor is 0
	if ("0" == str2) {
		quotient = "ERROR!";
		residue = "ERROR!";
        if (1 == flag) {
            return quotient;
        }
        else {
            return residue;
        }
	}

    // Determine if the dividend is 0
	if ("0" == str1) {
		quotient = "0";
		residue = "0";
	}

	if ('-' == str1[0]) {
		str1 = str1.erase(0, 1);
		sign1 *= -1;
		sign2 = -1;
	}

	if ('-' == str2[0]) {
		str2 = str2.erase(0, 1);
		sign1 *= -1;
	}

	int res = compare(str1, str2);
	if (0 > res) {
		quotient = "0";
		residue = str1;
	}
	else if (0 == res) {
		quotient = "1";
		residue = "0";
	}
	else {
		string::size_type l1, l2;
		l1 = str1.size(); l2 = str2.size();
		string tempstr;
		tempstr.append(str1, 0, l2 - 1);
		// Simulate manual division
		for (unsigned long i = l2 - 1; i < l1; i++) {
			tempstr = tempstr + str1[i];
			for (char ch = '9'; ch >= '0'; ch--) { //Êtry quotient
				string str;
				str = str + ch;
				if (0 >= compare(MUL_INT(str2, str), tempstr)) {
					quotient = quotient + ch;
					tempstr = SUB_INT(tempstr, MUL_INT(str2, str));
					break;
				}
			}
		}
		residue = tempstr;
	}

	// Remove the leading 0 from the result
	quotient.erase(0, quotient.find_first_not_of('0'));
    if (quotient.empty()) {
        quotient = "0";
    }
    if (   ( -1 == sign1)
        && ('0' != quotient[0])
        ) {
		quotient = "-" + quotient;
    }
    if (   ( -1 == sign2)
        && ('0' != residue[0])
        ) {
		residue = "-" + residue;
    }
    if (1 == flag) {
        return quotient;
    }
    else {
        return residue;
    }
}

// High precision division, return quotient
string DIV_INT(string str1, string str2) {
	return DIVIDE_INT(str1, str2, 1);
}

// High precision division, return remainder
string MOD_INT(string str1, string str2) {
	return DIVIDE_INT(str1, str2, 0);
}

// High precision subtraction
string SUB_INT(string str1, string str2) {

	string MUL_INT(string str1, string str2);
	int sign = 1; // sign bit
	string str;

    if ('-' == str2[0]) {
		str = ADD_INT(str1, str2.erase(0, 1));
    }
	else {
		int res = compare(str1, str2);
        if (0 == res) {
            return "0";
        }
		if (0 > res) {
			sign = -1;
			string temp = str1;
			str1 = str2;
			str2 = temp;
		}
		string::size_type tempint;
		tempint = str1.size() - str2.size();
		for (unsigned long i = str2.size() - 1; i >= 0; i--) {
			if (str1[i + tempint] < str2[i]) {
				str1[i + tempint - 1] = char(int(str1[i + tempint - 1]) - 1);
				str = char(str1[i + tempint] - str2[i] + ':') + str;
			}
            else {
				str = char(str1[i + tempint] - str2[i] + '0') + str;
            }
		}
        for (unsigned long i = tempint - 1; i >= 0; i--) {
			str = str1[i] + str;
        }
	}

	// Remove the extra leading 0 from the result
	str.erase(0, str.find_first_not_of('0'));
    if (str.empty()) {
        str = "0";
    }
    if (   ( -1 == sign)
        && ('0' != str[0])
        ) {
		str = "-" + str;
    }

	return str;
}

// High precision multiplication
string MUL_INT(string str1, string str2) {

	int sign = 1; // sign bit
	string str;

	if ('-' == str1[0]) {
		sign *= -1;
		str1 = str1.erase(0, 1);
	}
	if ('-' == str2[0]) {
		sign *= -1;
		str2 = str2.erase(0, 1);
	}

	string::size_type l1 = str1.size();
    string::size_type l2 = str2.size();
    unsigned long i, j;
    //ÊImplement manual multiplication
	for (i = l2 - 1; i >= 0; i--) {
		string tempstr;
		int int1 = 0, int2 = 0, int3 = int(str2[i]) - '0';
		if (0 != int3) {
            for (j = 1; j <= (int)(l2 - 1 - i); j++) {
				tempstr = "0" + tempstr;
            }
			for (j = l1 - 1; j >= 0; j--) {
				int1 = (int3 * (int(str1[j]) - '0') + int2) % 10;
				int2 = (int3 * (int(str1[j]) - '0') + int2) / 10;
				tempstr = char(int1 + '0') + tempstr;
			}
            if (0 != int2) {
                tempstr = char(int2 + '0') + tempstr;
            }
		}

		str = ADD_INT(str, tempstr);
	}
	// Remove the leading 0 from the result
	str.erase(0, str.find_first_not_of('0'));
    if (str.empty()) {
        str = "0";
    }
    if (   ( -1 == sign)
        && ('0' != str[0])
        ) {
		str = "-" + str;
    }

	return str;
}

// High precision addition
string ADD_INT(string str1, string str2) {

	string SUB_INT(string str1, string str2);
	int sign = 1; //sign bit
	string str;

	if ('-' == str1[0]) {
		if ('-' == str2[0]) {
			sign = -1;
			str = ADD_INT(str1.erase(0, 1), str2.erase(0, 1));
		}
		else {
			str = SUB_INT(str2, str1.erase(0, 1));
		}
	}
	else {
        if ('-' == str2[0]) {
			str = SUB_INT(str1, str2.erase(0, 1));
        }
		else {
			// Align the two integers and prefix the short integer with a 0
			string::size_type l1 = str1.size();
            string::size_type l2 = str2.size();
			if (l1 < l2) {
                for (unsigned long i = 1; i <= l2 - l1; i++) {
					str1 = "0" + str1;
                }
			}
			else {
                for (unsigned long i = 1; i <= l1 - l2; i++) {
					str2 = "0" + str2;
                }
			}
			int int1 = 0, int2 = 0; // int2 records carry
			for (unsigned long i = str1.size() - 1; i >= 0; i--) {
				int1 = (int(str1[i]) - '0' + int(str2[i]) - '0' + int2) % 10;
				int2 = (int(str1[i]) - '0' + int(str2[i]) - '0' + int2) / 10;
				str = char(int1 + '0') + str;
			}
            if (0 != int2) {
                str = char(int2 + '0') + str;
            }
		}
	}

	// Sign bits after operation
    if (   ( -1 == sign)
        && ('0' != str[0])
        ) {
		str = "-" + str;
    }

	return str;
}
