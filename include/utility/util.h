#pragma once
#ifndef COINPASS_BIO_UTIL_HPP
#define COINPASS_BIO_UTIL_HPP

#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <cstring>
#include "endian.h"

/*
* String splitting function
*/
std::vector<std::string> Split(std::string str, std::string pattern);
/*
* Merge string function
*/
std::string Join(std::vector<std::string> v, std::string pattern);

/*
* std::vector<unsigned char> -> Hexadecimal string conversion
*/
std::string CharPtr2HexStr(std::vector<unsigned char> v);
/*
* Hexadecimal string conversion -> std::vector<unsigned char>
*/
std::vector<unsigned char> HexStr2CharPtr(std::string str);

/*
* Dictionary order
*/
std::vector<char*> SortLexicographically(std::vector<char*> v);
std::vector<std::string> SortLexicographically(std::vector<std::string> v);

// flip
void InvertBuffer(unsigned char* pBuffer, unsigned long ulBufferLen);

// Put an array of strings into a vector
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

#endif
