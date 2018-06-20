#include <utility/util.hpp>


namespace jub {


/*
 * 字符串拆分
*/
std::vector<std::string> split(std::string str, std::string pattern)
{
	std::string::size_type pos;
	std::vector<std::string> result;
	str += pattern; // 扩展字符串以方便操作
	size_t size = str.size();

	for (size_t i = 0; i<size; i++) {
		pos = str.find(pattern, i);
		if (pos < size) {
			std::string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}

	return result;
}

/*
 * 合并字符串
*/
std::string join(std::vector<std::string> v, std::string pattern)
{
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

/*
 * char array<uint8_t, 32> -> 十六进制字符串转换
 */
std::string CharArray2HexStr(std::array<uint8_t, 32> v) {

    std::stringstream ss;
    ss << std::hex;

    size_t len = v.size();
    for (size_t i(0); i < len; ++i) {
        if ((int)v[i] <= 0x0F) {
            ss << 0;
        }
        ss << (int)v[i];
    }

    return ss.str();
}

/*
 * 十六进制字符串转换 -> char array<uint8_t, 32>
 */
/*
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
}*/

/*
 * char * -> 十六进制字符串转换
*/
std::string CharPtr2HexStr(std::vector<unsigned char> v)
{
	std::stringstream ss;
	ss << std::hex;

	size_t len = v.size();
	for (size_t i(0); i < len; ++i) {
		if ((int)v[i] <= 0x0F) {
			ss << 0;
		}
		ss << (int)v[i];
	}

	return ss.str();
}

/*
* 十六进制字符串转换 -> char *
*/
/*
std::vector<unsigned char> HexStr2CharPtr(std::string str)
{
	std::vector<unsigned char> v;

	auto l = str.size();
	unsigned int c;
	std::string sub;
	for (int i = 0; i < l; i += 2) {
		sub = str.substr(i, 2);
		std::istringstream hex_chars_stream(sub.c_str());
		hex_chars_stream >> std::hex >> c;
		v.push_back(c);
	}

	return v;
}
*/

/*
 * char * -> 十六进制字符串转换
 */
std::string ETHCharPtr2HexStr(std::vector<unsigned char> v)
{
    std::string str = ETH_PRDFIX;
    str += CharPtr2HexStr(v);
    return str;
}
/*
 * 十六进制字符串转换 -> char *
 */
/*
std::vector<unsigned char> ETHHexStr2CharPtr(std::string str)
{
    std::string _str = str;
    size_t pos = _str.find(ETH_PRDFIX);
    _str.erase(pos, strlen(ETH_PRDFIX));
    return HexStr2CharPtr(_str);
}*/

/*
 * 字典排序
*/
std::vector<char*> sortLexicographically(std::vector<char*> v)
{
	std::sort(v.begin(), v.end(), [](char* a, char* b) {
		return strcmp(a, b) < 0;
	});

	return v;
}

//
std::vector<std::string> sortLexicographically(std::vector<std::string> v)
{
	std::sort(v.begin(), v.end(), [](std::string a, std::string b) {
		return strcmp(a.c_str(), b.c_str()) < 0;
	});

	return v;
}

//
void InvertBuffer(unsigned char* pBuffer, unsigned long ulBufferLen)
{
	unsigned char tmp = 0;
	for (unsigned long i = 0; i < ulBufferLen / 2; ++i)
	{
		tmp = pBuffer[i];
		pBuffer[i] = pBuffer[ulBufferLen - i - 1];
		pBuffer[ulBufferLen - i - 1] = tmp;
	}
}

//
std::vector<std::string> CharPtrArr2StrVector(const char* Arr[])
{
	std::vector<std::string> v;
	if (Arr == nullptr)
	{
		return v;
	}

	int l = 0;
	const char* p = Arr[l];
	while (p)
	{
		v.push_back(p);
		l++;
		p = Arr[l];
	}

	return v;
}

std::string
dataToString(const DataSlice &slice)
{
	// Due to a bug, lots of AirBitz encrypted blobs end with a null byte.
	// Get rid of those:
	auto size = slice.size();
	if (0 < size && !slice.data()[size - 1])
		size--;

	return std::string(reinterpret_cast<const char *>(slice.data()), size);
}

DataChunk
buildData(std::initializer_list<DataSlice> slices)
{
	size_t size = 0;
	for (auto slice : slices)
		size += slice.size();

	DataChunk out;
	out.reserve(size);
	for (auto slice : slices)
		out.insert(out.end(), slice.begin(), slice.end());
	return out;
}

/*
bool AddPKCSPadding(DataChunk &data, unsigned int block_size)
{
	if (data.empty()) {
		return false;
	}

	auto padLen = block_size - (data.size() % block_size);
	for (auto i = 0; i < padLen; i++)
		data.push_back((unsigned char)padLen);

	return true;
}*/

bool StripPKCSPadding(DataChunk &data, unsigned long block_size)
{
	if (data.empty()) {
		return false;
	}

	if (data.size() < block_size)
		return false;

	auto padValue = data.back();
	if ((padValue > block_size) || (padValue == 0)) {
		return false;
	}

	for (auto i = 0; i < padValue; i++)
		data.pop_back();

	return true;
}

DataChunkList parseTlv(const DataSlice &data) {
	DataChunkList dataList;

	for (auto pos = data.begin(); pos < data.end(); ) {
		++pos; // tag, one byte

		unsigned int len = 0;

		if (*pos == 0x82) {
			++pos; // 0x82
			len = *pos << 8; // high

			++pos; // low
			len |= *pos & 0xff;

			++pos; // value
		}
		else if (*pos == 0x81) {
			++pos; // 0x81
			len = *pos;

			++pos; // value
		}
		else {
			len = *pos;
			++pos; // value
		}

		dataList.push_back(DataChunk(pos, pos + len));
		pos += len;
	}

	return dataList;
}

DataChunk toTlv(uint8_t tag, const DataSlice &data) {

	DataChunk tlvData;
	
	unsigned int len = static_cast<unsigned int>(data.size());

	tlvData.push_back(tag);
	if (len >= 0x100) {   // max size: two bytes 2^16
		tlvData.push_back(0x82);
		tlvData.push_back(len >> 8);
		tlvData.push_back(len & 0xFF);
	}
	else if (len > 0x80) {
		tlvData.push_back(0x81);
		tlvData.push_back(len);
	}
	else {
		tlvData.push_back(len);
	}

	tlvData.insert(tlvData.end(), data.begin(), data.end());

	return tlvData;
}

} // namespace jub end
