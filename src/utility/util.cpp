#include "utility/util.h"

namespace jub {

/*
* Convert character array to string in C++
*/
std::string convertToString(char* a, int size) {
    int i;
    std::string s = "";
    for (i = 0; i < size; i++) {
        s = s + a[i];
    }

    return s;
}

/*
 * String splitting function
*/
std::vector<std::string> Split(std::string str, std::string pattern) {

	std::string::size_type pos;
	std::vector<std::string> vResult;
	str += pattern; // extend the string for convenience
	size_t size = str.size();

	for (size_t i = 0; i < size; i++) {
		pos = str.find(pattern, i);
		if (pos < size) {
			std::string s = str.substr(i, pos - i);
            if (0 < s.length()) {
                vResult.push_back(s);
            }
			i = pos + pattern.size() - 1;
		}
	}

	return vResult;
}

/*
 * Merge string function
*//*
std::string Join(std::vector<std::string> v, std::string pattern) {

	std::string strJoin;
	int i = 0;
	auto sz = v.size();
	for (auto it : v) {
		i++;
		strJoin += it;
		if (i != sz) {
			strJoin += pattern;
		}
	}

	return strJoin;
}
*/
/*
 * Decimal string conversion -> Hexadecimal string
 */
std::string DecStringToHexString(std::string str) {

    return bigUnsignedToString(stringToBigUnsigned(str.c_str()), 16);
}

std::string numberToHexString(unsigned long val) {

    return bigUnsignedToString(val, 16);
}

/*
 * char array<uint8_t, 32> -> Hexadecimal string conversion
 *//*
std::string CharArray2HexStr(std::array<uint8_t, 32> v) {

    std::stringstream ss;
    ss << std::hex;

    size_t len = v.size();
    for (size_t i(0); i < len; ++i) {
        if (0x0F >= (int)v[i]) {
            ss << 0;
        }
        ss << (int)v[i];
    }

    return ss.str();
}
*/
/*
 * Hexadecimal string conversion -> char array<uint8_t, 32>
 *//*
std::array<uint8_t, 32> HexStr2CharArray(std::string str) {

    std::array<uint8_t, 32> v;

    auto l = str.size();
	unsigned int c;
    std::string sub;
	int j = 0;
	for (int i = 0; i < l; i += 2, j++) {
        sub = str.substr(i, 2);
        std::istringstream hex_chars_stream(sub.c_str());
        hex_chars_stream >> std::hex >> c;
		v[j] = c;
    }

    return v;
}
*/
/*
 * std::vector<unsigned char> -> Hexadecimal string conversion
*/
std::string CharPtr2HexStr(std::vector<unsigned char> v) {

	std::stringstream ss;
	ss << std::hex;

	size_t len = v.size();
	for (size_t i(0); i < len; ++i) {
		if (0x0F >= (int)v[i]) {
			ss << 0;
		}
		ss << (int)v[i];
	}

	return ss.str();
}

/*
* Hexadecimal string conversion -> std::vector<unsigned char>
*/
std::vector<unsigned char> HexStr2CharPtr(std::string str) {

    std::vector<unsigned char> v;

    std::string strHex;
    auto l = str.size();
    if (l%2) {
        strHex = "0";
    }
    else {
        strHex = "";
    }
    strHex += str;
    unsigned int c;
    std::string sub;
    for (int i = 0; i < l; i += 2) {
        sub = strHex.substr(i, 2);
        std::istringstream hex_chars_stream(sub.c_str());
        hex_chars_stream >> std::hex >> c;
        v.push_back(c);
    }

    return v;
}

/*
 * std::vector<unsigned char> -> Hexadecimal string conversion
 *//*
std::string ETHCharPtr2HexStr(std::vector<unsigned char> v) {

    std::string str(ETH_PRDFIX);
    str += CharPtr2HexStr(v);
    return str;
}*/

/*
 * Hexadecimal string conversion -> std::vector<unsigned char>
 */
std::vector<unsigned char> ETHHexStr2CharPtr(std::string str) {
    if (str.empty()) {
        return std::vector<unsigned char>();
    }

    std::string _str = str;
    size_t pos = _str.find(ETH_PRDFIX);
    if (std::string::npos == pos) {
//        return std::vector<unsigned char>();
        return HexStr2CharPtr(_str);
    }
    _str.erase(pos, strlen(ETH_PRDFIX));
    return HexStr2CharPtr(_str);
}

/*
// Put an array of strings into a vector
std::vector<std::string> CharPtrArr2StrVector(const char* Arr[]) {

	std::vector<std::string> v;
	if (nullptr == Arr) {
		return v;
	}

	int l = 0;
	const char* p = Arr[l];
	while (p) {
		v.push_back(p);
		l++;
		p = Arr[l];
	}

	return v;
}
*/
/*
bool AddPKCSPadding(TW::Data &data, unsigned int block_size) {

	if (data.empty()) {
		return false;
	}

	auto padLen = block_size - (data.size() % block_size);
    for (auto i = 0; i < padLen; i++) {
        data.push_back((unsigned char)padLen);
    }

	return true;
}
*/
/*
bool StripPKCSPadding(TW::Data &data, unsigned long block_size) {

    if (data.empty()) {
        return false;
    }

    if (data.size() < block_size) {
        return false;
    }

    auto padValue = data.back();
    if (   (block_size < padValue)
        || (        0 == padValue)
        ) {
        return false;
    }

    for (auto i = 0; i < padValue; i++) {
        data.pop_back();
    }

    return true;
}
*/

std::vector<TW::Data> ParseTlv(const TW::Data &data) {
    std::vector<TW::Data> dataList;

    for (auto pos = data.begin(); pos < data.end(); ) {
        ++pos; // tag, one byte

        unsigned int len = 0;

        if (0x82 == (*pos)) {
            ++pos; // 0x82
            len = *pos << 8; // high

            ++pos; // low
            len |= *pos & 0xff;

            ++pos; // value
        }
        else if (0x81 == (*pos)) {
            ++pos; // 0x81
            len = *pos;

            ++pos; // value
        }
        else {
            len = *pos;
            ++pos; // value
        }

        dataList.push_back(TW::Data(pos, pos + len));
        pos += len;
    }

    return dataList;
}


TW::Data ToTlv(uint8_t tag, const TW::Data &data) {

    TW::Data tlvData;

    unsigned int len = static_cast<unsigned int>(data.size());

    tlvData.push_back(tag);
    if (0x100 <= len) {   // max size: two bytes 2^16
        tlvData.push_back(0x82);
        tlvData.push_back(len >> 8);
        tlvData.push_back(len & 0xFF);
    }
    else if (0x80 < len) {
        tlvData.push_back(0x81);
        tlvData.push_back(len);
    }
    else {
        tlvData.push_back(len);
    }

    tlvData.insert(tlvData.end(), data.begin(), data.end());

    return tlvData;
}


TW::Data Tollv(const std::string& strData) {

    TW::Data vData;
    vData.push_back(' ');
    uchar_vector data;
    data << strData;
    for (size_t i=0; i<data.size(); ++i) {
        vData.push_back(data[i]);
    }
    vData.push_back(0x00);

    TW::Data llvData;
    unsigned int len = static_cast<unsigned int>(vData.size());

    llvData.push_back(len);
    llvData.insert(llvData.end(), vData.begin(), vData.end());

    return llvData;
}


// function to convert decimal to binary
void decToBinary(uint8_t n, std::vector<uint8_t> &v) {
    // array to store binary number
    uint8_t binaryNum[8*8] = {0x00,};

    // counter for binary array
    int i = 0;
    while (n > 0) {
        // storing remainder in binary array
        binaryNum[i] = n % 2;
        n = n / 2;
        i++;
    }

    // printing binary array in order
    for (int8_t j=0; j<i; ++j) {
        v.push_back(binaryNum[j]);
    }
}

JUB_BIP32_PATH spiltMainPath(std::string mainpath, char ch)
{
    int start = 0;
    int end = 0;
    int index = 0;
    JUB_BIP32_PATH path;
    mainpath.push_back(ch);

    for(int i = 0;i < mainpath.size();i++)
    {
        if(mainpath[i]==ch)
        {
            index++;
            end = i;
            std::string temp = mainpath.substr(start,end-start);
            start = i+1;
            if (index == 2) {
                memcpy(path.purpose, (char*)temp.data(), sizeof(temp.data()));
            } else if (index == 3) {
                memcpy(path.coin_type, (char*)temp.data(), sizeof(temp.data()));
            } else if (index == 4) {
                memcpy(path.account, (char*)temp.data(), sizeof(temp.data()));
            } else if (index == 5) {
                path.change = strcmp(temp.data(), "0") == 0 ? false: true;
            } else if (index == 6) {
                path.addressIndex = atoi(temp.c_str());
            }

        }
    }
    return path;
}

//Is it A string containing a number
bool isString(std::string str)
{
    if (!str.length()) return false;
    if (isNumber(str)) {
        return false;
    }

    std::regex reg("^[0-9a-zA-Z]+$");
    if (std::regex_match(str.begin(),str.end(),reg) && str.length() < 10) {
        return true;
    } else
        return false;
}
//Is it a Number
bool isNumber(std::string str)
{
    if (!str.length()) return false;
    std::regex reg("[0-9]+");
    if (std::regex_match(str.begin(),str.end(),reg)) {
//        if (atoi(str.c_str()) < 256) return true;
        return true;
    }
    return false;
}

std::string appendZero(std::string str , int size)
{
    if (str.length() >= size) {
        return str;
    }
    
    for (unsigned long i = str.length(); i < size; i++) {
        str.append("0");
    }
    return str;
}


std::string get_raw_string(std::string const& s)
{
    std::ostringstream out;
    out << std::hex;
    for (std::string::const_iterator it = s.begin(); it != s.end(); ++it)
    {
        out << (static_cast<short>(*it) & 0xff);
    }
    return out.str();
}

void EndianSwap(std::uint8_t *pData, unsigned long length)
{
    unsigned long i,cnt,end,start;
    cnt = length / 2;
    start = 0;
    end  = length - 1;
    std::uint8_t tmp;
    for (i = 0; i < cnt; i++)
    {
        tmp            = pData[start+i];
        pData[start+i] = pData[end-i];
        pData[end-i]   = tmp;
    }
}
std::string polkadot_string_to_hex(long long num)
{
    std::string hexStr = numberToHexString(num);
    if (hexStr.length() % 2) {
        hexStr.insert(0, "0");
    }
    return hexStr;
}


int decode_unsigned_big(uint8_t* encode, int encode_len) {
  int integer = 0;
  for (int i = 0; i < encode_len; ++i) {
    integer += (encode[i] << 8 * (encode_len - i - 1));
  }
  return integer;
}


int decode_signed_big(uint8_t* encode, int encode_len) {
  int integer = 0;

  while (encode_len-- > 0) {
    uint8_t v = *encode++;
    v ^= 0xff;
    integer *= 256;
    integer += v;
  }
  return -(integer + 1);
}


} // namespace jub end
