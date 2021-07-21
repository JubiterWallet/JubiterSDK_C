#pragma once
#ifndef COINPASS_BIO_UTIL_HPP
#define COINPASS_BIO_UTIL_HPP

#include <assert.h>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <cstring>
#include "mSIGNA/stdutils/uchar_vector.h"
#include <bigint/BigIntegerLibrary.hh>
#include <Data.h>
#include <regex>
#include <map>
namespace jub {

#define JUB_VERIFY_RET(x, r)    \
do {                            \
    if (!(x)) {                 \
        return (r);             \
    }                           \
} while (0)

#define JUB_VERIFY_RV(x)    \
do {                        \
    JUB_RV r = (x);         \
    if (JUBR_OK != r) {     \
        return r;           \
    }                       \
} while (0)

#define JUB_CHECK_NULL(x)               \
do {                                    \
    if (!x) {                           \
        assert(false);                  \
        return JUBR_MEMORY_NULL_PTR;    \
    }                                   \
} while (0)

#define JUB_UNREFERENCED_PARAMETER(P) (P)

// based on BIP-0032
//https://github.com/bitcoin/bips/blob/master/bip-0049.mediawiki
//https://github.com/bitcoin/bips/blob/master/bip-0044.mediawiki
typedef struct stBip32Path {
    char  purpose[5];
    char  coin_type[5];
    char  account[5];
    bool   change;
    unsigned int  addressIndex;
} JUB_BIP32_PATH;

template <class _Ty, size_t _Size>
constexpr size_t array_size(const _Ty (&)[_Size]) noexcept {
    return (_Size);
}

/*
* Convert character array to string in C++
*/
std::string convertToString(char* a, int size);

/*
* String splitting function
*/
std::vector<std::string> Split(std::string str, std::string pattern);
/*
* Merge string function
*//*
std::string Join(std::vector<std::string> v, std::string pattern);
*/
/*
 * Decimal string conversion -> Hexadecimal string
 */
std::string DecStringToHexString(std::string str);

std::string numberToHexString(unsigned long val);

/*
 * char array<uint8_t, 32> -> Hexadecimal string conversion
 *//*
std::string CharArray2HexStr(std::array<uint8_t, 32> v);
*/
/*
 * Hexadecimal string conversion -> char array<uint8_t, 32>
 *//*
std::array<uint8_t, 32> HexStr2CharArray(std::string str);
*/
/*
* std::vector<unsigned char> -> Hexadecimal string conversion
*/
std::string CharPtr2HexStr(std::vector<unsigned char> v);

/*
* Hexadecimal string conversion -> std::vector<unsigned char>
*/
std::vector<unsigned char> HexStr2CharPtr(std::string str);
/*
// Put an array of strings into a vector
std::vector<std::string> CharPtrArr2StrVector(const char* Arr[]);
*/

#define ETH_PRDFIX "0x"
/*
 * std::vector<unsigned char> -> Hexadecimal string conversion
 *//*
std::string ETHCharPtr2HexStr(std::vector<unsigned char> v);
*/
/*
 * Hexadecimal string conversion -> std::vector<unsigned char>
 */
std::vector<unsigned char> ETHHexStr2CharPtr(std::string str);

TW::Data ToTlv(uint8_t tag, const TW::Data &data);
std::vector<TW::Data> ParseTlv(const TW::Data &data);
TW::Data Tollv(const std::string& strData);

void decToBinary(uint8_t n, std::vector<uint8_t> &v);
/**
 spilt mainpth, following 5 levels in BIP32 path:
 m / purpose' / coin_type' / account' / change / address_index
 */
JUB_BIP32_PATH spiltMainPath(std::string mainpath,char ch);

//Is it a string or number
bool isStringOrNumber(std::string str);
//Is it a string
bool isString(std::string str);
//Is it a number
bool isNumber(std::string str);
//apeend zero
std::string appendZero(std::string str, int size);
//str to ascii str
std::string get_raw_string(std::string const& s);
std::string polkadot_string_to_hex(long long num);
void EndianSwap(std::uint8_t *pData, unsigned long length);


} // namespace jub

#endif
