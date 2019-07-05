#ifndef __util__
#define __util__

#include <stdint.h>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <set>
#include <algorithm>
#include <sstream>
#include <iterator>
#include <cassert>
#include <iostream>
#include <cstring>
#include <bigint/BigIntegerLibrary.hh>
#include <JUB_SDK.h>
#include "endian.h"

namespace jub {

#define JUB_VERIFY_RET(x,r) do{if(!(x))return(r);}while(0)

#define JUB_VERIFY_RV(x)	    \
do{	                            \
	JUB_RV r = (x);				\
	if(JUBR_OK != r) {          \
		return r;		        \
	}				            \
} while(0)

#define JUB_CHECK_NULL(x)	            \
do{	                                    \
	if(!x) {	                        \
		assert(false);   		        \
		return JUBR_MEMORY_NULL_PTR;    \
	}						            \
} while(0)

#define JUB_UNREFERENCED_PARAMETER(P)          (P)

template<class _Ty,
	size_t _Size>
	constexpr size_t array_size(const _Ty(&)[_Size]) noexcept {
	return (_Size);
}

std::vector<std::string> Split(std::string str, std::string pattern);

std::string Join(std::vector<std::string> v, std::string pattern);

std::string CharArray2HexStr(std::array<uint8_t, 32> v);

std::array<uint8_t, 32> HexStr2CharArray(std::string str);

std::string CharPtr2HexStr(std::vector<unsigned char> v);

std::vector<unsigned char> HexStr2CharPtr(std::string str);

std::string DecStringToHexString(std::string str);

std::string numberToHexString(unsigned int val);

#define ETH_PRDFIX  "0x"

std::string ETHCharPtr2HexStr(std::vector<unsigned char> v);

std::vector<unsigned char> ETHHexStr2CharPtr(std::string str);

std::vector<char*> SortLexicographically(std::vector<char*> v);
std::vector<std::string> SortLexicographically(std::vector<std::string> v);

void InvertBuffer(unsigned char* pBuffer, unsigned long ulBufferLen);

std::vector<std::string> CharPtrArr2StrVector(const char* Arr[]);

uint16_t static inline ReadLE16(const unsigned char* ptr) {

	uint16_t x;
	memcpy((char*)&x, ptr, 2);

	return le16toh(x);
}

uint32_t static inline ReadLE32(const unsigned char* ptr) {

	uint32_t x;
	memcpy((char*)&x, ptr, 4);

	return le32toh(x);
}

uint64_t static inline ReadLE64(const unsigned char* ptr) {

	uint64_t x;
	memcpy((char*)&x, ptr, 8);

	return le64toh(x);
}

void static inline WriteLE16(unsigned char* ptr, uint16_t x) {

	uint16_t v = htole16(x);
	memcpy(ptr, (char*)&v, 2);
}

void static inline WriteLE32(unsigned char* ptr, uint32_t x) {

	uint32_t v = htole32(x);
	memcpy(ptr, (char*)&v, 4);
}

void static inline WriteLE64(unsigned char* ptr, uint64_t x) {

	uint64_t v = htole64(x);
	memcpy(ptr, (char*)&v, 8);
}

uint16_t static inline ReadBE16(const unsigned char* ptr) {

	uint16_t x;
	memcpy((char*)&x, ptr, 2);

	return be16toh(x);
}

uint32_t static inline ReadBE32(const unsigned char* ptr) {

	uint32_t x;
	memcpy((char*)&x, ptr, 4);

	return be32toh(x);
}

uint64_t static inline ReadBE64(const unsigned char* ptr) {

	uint64_t x;
	memcpy((char*)&x, ptr, 8);

	return be64toh(x);
}

void static inline WriteBE32(unsigned char* ptr, uint32_t x) {

	uint32_t v = htobe32(x);
	memcpy(ptr, (char*)&v, 4);
}

void static inline WriteBE64(unsigned char* ptr, uint64_t x) {

	uint64_t v = htobe64(x);
	memcpy(ptr, (char*)&v, 8);
}

template <typename T>
std::string to_string(T value) {

	std::ostringstream os;
	os << value;
	return os.str();
}
/**
* A reference to a block of raw data.
*/
class DataSlice {

public:
	DataSlice() :
		begin_(nullptr), end_(nullptr)
	{}

	DataSlice(const uint8_t *begin, const uint8_t *end) :
		begin_(begin), end_(end)
	{}

	template<typename Container>
	DataSlice(const Container &container) :
		begin_(container.data()), end_(container.data() + container.size())
	{}

	DataSlice(const std::string &s) :
		begin_(reinterpret_cast<const uint8_t *>(s.data())),
		end_(reinterpret_cast<const uint8_t *>(s.data()) + s.size())
	{}

	// STL-sytle accessors:
	bool empty()            const { return end_ == begin_; }
	std::size_t size()      const { return end_ - begin_; }
	const uint8_t *data()   const { return begin_; }
	const uint8_t *begin()  const { return begin_; }
	const uint8_t *end()    const { return end_; }

private:
	const uint8_t *begin_;
	const uint8_t *end_;
}; // class DataSlice end

/**
* Casts a data slice to a string.
*/
std::string dataToString(const DataSlice &slice);

/**
* A block of data with its size fixed at compile time.
*/
template<size_t Size> using DataArray = std::array<uint8_t, Size>;

/**
* A block of data with a run-time variable size.
*/
typedef std::vector<JUB_UCHAR> DataChunk;
typedef std::vector<DataChunk> DataChunkList;

/**
* Concatenates several data slices into a single buffer.
*/
DataChunk
buildData(std::initializer_list<DataSlice> slices);

JUB_RV encryptDataAES(const DataSlice &password, const DataSlice &data, DataChunk &encData);
JUB_RV decryptDataAES(const DataSlice &password, const DataSlice &data, DataChunk &decData);

std::string SHA2Pwd(const DataSlice &password);

DataChunk ToTlv(uint8_t tag, const DataSlice &data);
DataChunkList ParseTlv(const DataSlice &data);
} // namespace jub end

#endif // __util__
