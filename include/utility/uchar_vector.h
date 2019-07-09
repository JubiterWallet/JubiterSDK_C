////////////////////////////////////////////////////////////////////////////////
//
// uchar_vector.h
//
// Copyright (c) 2011-2012 Eric Lombrozo
// Copyright (c) 2011-2016 Ciphrex Corp.
//
// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.
//

#ifndef UCHAR_VECTOR_H__
#define UCHAR_VECTOR_H__

#include <stdio.h>
#include <stdint.h>

#include <iostream>
#include <cstring>

#include <vector>
#include <string>
#include <algorithm>

#include "util.h"
#include "machine/opcode.hpp"

namespace bitcoin {
namespace util {
const char g_hexBytes[][3] = {
    "00","01","02","03","04","05","06","07","08","09","0a","0b","0c","0d","0e","0f",
    "10","11","12","13","14","15","16","17","18","19","1a","1b","1c","1d","1e","1f",
    "20","21","22","23","24","25","26","27","28","29","2a","2b","2c","2d","2e","2f",
    "30","31","32","33","34","35","36","37","38","39","3a","3b","3c","3d","3e","3f",
    "40","41","42","43","44","45","46","47","48","49","4a","4b","4c","4d","4e","4f",
    "50","51","52","53","54","55","56","57","58","59","5a","5b","5c","5d","5e","5f",
    "60","61","62","63","64","65","66","67","68","69","6a","6b","6c","6d","6e","6f",
    "70","71","72","73","74","75","76","77","78","79","7a","7b","7c","7d","7e","7f",
    "80","81","82","83","84","85","86","87","88","89","8a","8b","8c","8d","8e","8f",
    "90","91","92","93","94","95","96","97","98","99","9a","9b","9c","9d","9e","9f",
    "a0","a1","a2","a3","a4","a5","a6","a7","a8","a9","aa","ab","ac","ad","ae","af",
    "b0","b1","b2","b3","b4","b5","b6","b7","b8","b9","ba","bb","bc","bd","be","bf",
    "c0","c1","c2","c3","c4","c5","c6","c7","c8","c9","ca","cb","cc","cd","ce","cf",
    "d0","d1","d2","d3","d4","d5","d6","d7","d8","d9","da","db","dc","dd","de","df",
    "e0","e1","e2","e3","e4","e5","e6","e7","e8","e9","ea","eb","ec","ed","ee","ef",
    "f0","f1","f2","f3","f4","f5","f6","f7","f8","f9","fa","fb","fc","fd","fe","ff"
};

typedef unsigned int uint;

/// TODO: Add secure allocator capabilities
//template < class Allocator = allocator<unsigned char> >
class uchar_vector : public std::vector<unsigned char>//, Allocator>
{
public:
    uchar_vector() : std::vector<unsigned char>() {
        m_cur_it = begin(); m_last_op_it = begin();
    }
    uchar_vector(size_type n, const unsigned char& value = 0) : std::vector<unsigned char>(n, value) {
        m_cur_it = begin(); m_last_op_it = begin();
    }
    template <class InputIterator> uchar_vector(InputIterator first, InputIterator last) : std::vector<unsigned char>(first, last) {
        m_cur_it = begin(); m_last_op_it = begin();
    }
    uchar_vector(const std::vector<unsigned char>& vec) : std::vector<unsigned char>(vec) {
        m_cur_it = begin(); m_last_op_it = begin();
    }
    uchar_vector(const unsigned char* array, unsigned int size) : std::vector<unsigned char>(array, array + size) {
        m_cur_it = begin();
        m_last_op_it = begin();
    }
    uchar_vector(const std::string& hex) {
        this->setHex(hex);  m_cur_it = begin();
        m_last_op_it = begin();
    }

    uchar_vector(char* hex) {
        this->setHex(std::string(hex));  m_cur_it = begin();
        m_last_op_it = begin();
    }

    uchar_vector& operator+=(const std::vector<unsigned char>& rhs) {
        this->insert(this->end(), rhs.begin(), rhs.end());
        return *this;
    }

    uchar_vector& operator<<(const std::vector<unsigned char>& rhs) {
        this->insert(this->end(), rhs.begin(), rhs.end());
        return *this;
    }

    uchar_vector& operator<<(const std::string& str) {
        this->insert(this->end(), str.begin(), str.end());
        return *this;
    }

    uchar_vector& operator<<(unsigned char byte) {
        this->push_back(byte);
        return *this;
    }

    uchar_vector& operator<<(libbitcoin::machine::opcode byte) {
        this->push_back((unsigned char)byte);
        return *this;
    }

    uchar_vector& operator<<(uint16_t data) {
        uint8_t _data[2] = { 0x00, };
        WriteLE16(_data, (uint16_t)data);
        insert(end(), _data, _data + sizeof(_data));
        return *this;
    }

    uchar_vector& operator<<(uint32_t data) {
        uint8_t _data[4] = { 0x00, };
        WriteLE32(_data, (uint32_t)data);
        insert(end(), _data, _data + sizeof(_data));
        return *this;
    }

    uchar_vector& operator<<(uint64_t data) {
        uint8_t _data[8] = { 0x00, };
        WriteLE64(_data, (uint64_t)data);
        insert(end(), _data, _data + sizeof(_data));
        return *this;
    }

    uchar_vector& operator &(const std::vector<unsigned char>& rhs) {
        if (rhs.size() < (unsigned char)libbitcoin::machine::opcode::push_one_size) {
            insert(end(), (unsigned char)rhs.size());
        }
        else if (rhs.size() <= 0xff) {
            insert(end(), (unsigned char)libbitcoin::machine::opcode::push_one_size);
            insert(end(), (unsigned char)rhs.size());
        }
        else if (rhs.size() <= 0xffff) {
            insert(end(), (unsigned char)libbitcoin::machine::opcode::push_two_size);
            uint8_t _data[2] = { 0x00, };
            WriteLE16(_data, (uint16_t)rhs.size());
            insert(end(), _data, _data + sizeof(_data));
        }
        else {
            insert(end(), (unsigned char)libbitcoin::machine::opcode::push_four_size);
            uint8_t _data[4] = { 0x00, };
            WriteLE32(_data, (uint16_t)rhs.size());
            insert(end(), _data, _data + sizeof(_data));
        }
        insert(end(), rhs.begin(), rhs.end());
        return *this;
    }

    uchar_vector build_compact_size(uint64_t size) {
        uchar_vector v;
        if (size < 0xfd) {
            v.insert(v.end(), (unsigned char)size);
        }
        else if (size <= 0xffff) {
            v.insert(v.end(), 0xfd);
            uint8_t _hash[2] = { 0x00, };
            WriteLE16(_hash, (uint16_t)size);
            v.insert(v.end(), _hash, _hash + sizeof(_hash));
        }
        else if (size <= 0xffff) {
            v.insert(v.end(), 0xfe);
            uint8_t _hash[4] = { 0x00, };
            WriteLE32(_hash, (uint32_t)size);
            v.insert(v.end(), _hash, _hash + sizeof(_hash));
        }
        else {
            v.insert(v.end(), 0xff);
            uint8_t _hash[8] = { 0x00, };
            WriteLE64(_hash, size);
            v.insert(v.end(), _hash, _hash + sizeof(_hash));
        }
        return v;
    }

    uchar_vector& operator &&(const std::vector<unsigned char>& rhs) {
        auto v_size = build_compact_size(rhs.size());
        insert(end(), v_size.begin(), v_size.end());
        insert(end(), rhs.begin(), rhs.end());
        return *this;
    }

    uchar_vector& operator &&(const std::string& str) {
        auto v_size = build_compact_size(str.length());
        insert(end(), v_size.begin(), v_size.end());
        insert(end(), str.begin(), str.end());
        return *this;
    }

    std::string getHex(bool spaceBytes = false) const {
        std::string hex;
        hex.reserve(this->size() * 2);
        for (uint i = 0; i < this->size(); i++) {
            if (spaceBytes && (i > 0)) hex += " ";
            hex += g_hexBytes[(*this)[i]];
        }
        return hex;
    }

    void addHex(std::string hex) {
        if (hex.size() % 2 == 1) {
            hex = "0" + hex;
        }

        for (size_t i = 0; i < hex.size(); i += 2) {
            uint byte;
            sscanf(hex.substr(i, 2).c_str(), "%x", &byte);
            this->push_back(byte);
        }
    }

    void setHex(std::string hex) {
        this->clear();

        // pad on the left if hex contains an odd number of digits.
        if (hex.size() % 2 == 1) {
            hex = "0" + hex;
        }

        this->reserve(hex.size() / 2);

        for (size_t i = 0; i < hex.size(); i += 2) {
            uint byte;
            sscanf(hex.substr(i, 2).c_str(), "%x", &byte);
            this->push_back(byte);
        }
    }

    void reverse() {
        std::reverse(this->begin(), this->end());
    }

    const uchar_vector operator+(const std::vector<unsigned char>& rightOperand) const {
        return uchar_vector(*this) += rightOperand;
    }

    uchar_vector& operator=(const std::string& hex) {
        this->setHex(hex);
        m_cur_it = begin();
        m_last_op_it = begin();
        return *this;
    }

    void copyToArray(unsigned char* array) {
        std::copy(this->begin(), this->end(), array);
    }

    uint8_t read_uint8() {
        if (end() - m_cur_it < 1) {
            std::string _err = "error in data index: ";
            _err += jub::to_string(m_last_op_it - begin());
            throw std::runtime_error(_err);
        }
        m_last_op_it = m_cur_it;
        return *(m_cur_it++);
    }

    uint16_t read_le_uint16() {
        if (end() - m_cur_it < 2) {
            std::string _err = "error in data index: ";
            _err += jub::to_string(m_last_op_it - begin());
            throw std::runtime_error(_err);
        }
        uint16_t d = ReadLE16(&(*m_cur_it));
        m_last_op_it = m_cur_it;
        m_cur_it += 2;
        return d;
    }

    uint32_t read_le_uint32() {
        if (end() - m_cur_it < 4) {
            std::string _err = "error in data index: ";
            _err += jub::to_string(m_last_op_it - begin());
            throw std::runtime_error(_err);
        }
        uint32_t d = ReadLE32(&(*m_cur_it));
        m_last_op_it = m_cur_it;
        m_cur_it += 4;
        return d;
    }

    uint64_t read_le_uint64() {
        if (end() - m_cur_it < 8) {
            std::string _err = "error in data index: ";
            _err += jub::to_string(m_last_op_it - begin());
            throw std::runtime_error(_err);
        }
        uint64_t d = ReadLE64(&(*m_cur_it));
        m_last_op_it = m_cur_it;
        m_cur_it += 8;
        return d;
    }

    uint16_t read_be_uint16() {
        if (end() - m_cur_it < 2) {
            std::string _err = "error in data index: ";
            _err += jub::to_string(m_last_op_it - begin());
            throw std::runtime_error(_err);
        }
        uint16_t d = ReadBE16(&(*m_cur_it));
        m_last_op_it = m_cur_it;
        m_cur_it += 2;
        return d;
    }

    uint32_t read_be_uint32() {
        if (end() - m_cur_it < 4) {
            std::string _err = "error in data index: ";
            _err += jub::to_string(m_last_op_it - begin());
            throw std::runtime_error(_err);
        }
        uint32_t d = ReadBE32(&(*m_cur_it));
        m_last_op_it = m_cur_it;
        m_cur_it += 4;
        return d;
    }

    uint64_t read_be_uint64() {
        if (end() - m_cur_it < 8) {
            std::string _err = "error in data index: ";
            _err += jub::to_string(m_last_op_it - begin());
            throw std::runtime_error(_err);
        }
        uint64_t d = ReadBE64(&(*m_cur_it));
        m_last_op_it = m_cur_it;
        m_cur_it += 8;
        return d;
    }

    uchar_vector read_vector(int len) {
        if (end() - m_cur_it < len) {
            std::string _err = "error in data index: ";
            _err += jub::to_string(m_last_op_it - begin());
            throw std::runtime_error(_err);
        }
        uchar_vector v;
        v.insert(v.begin(), m_cur_it, m_cur_it + len);
        v.reset_it();
        m_last_op_it = m_cur_it;
        m_cur_it = m_cur_it + len;
        return v;
    }

    uchar_vector& operator | (uchar_vector&  v) {

        if (m_cur_it == end()) {
            std::string _err = "error in data index: ";
            _err += jub::to_string(m_last_op_it - begin());
            throw std::runtime_error(_err);
        }

        uint32_t len = 0;
        if (*m_cur_it == (unsigned char)libbitcoin::machine::opcode::push_one_size) {
            m_last_op_it = m_cur_it;
            m_cur_it++;
            len = read_uint8();
        }
        else if (*m_cur_it == (unsigned char)libbitcoin::machine::opcode::push_two_size) {
            m_last_op_it = m_cur_it;
            m_cur_it++;
            len = read_le_uint16();
        }
        else if (*m_cur_it == (unsigned char)libbitcoin::machine::opcode::push_four_size) {
            m_last_op_it = m_cur_it;
            m_cur_it++;
            len = read_le_uint32();
        }
        else {
            len = read_uint8();
        }
        if (end() - m_cur_it < (int)len) {
            std::string _err = "error in data index: ";
            _err += jub::to_string(m_last_op_it - begin());
            throw std::runtime_error(_err);
        }
        v.clear();
        v.insert(v.end(), m_cur_it, m_cur_it + len);
        v.reset_it();
        m_last_op_it = m_cur_it;
        m_cur_it += len;
        return *this;
    }

    uchar_vector& operator || (uchar_vector&  v) {
        uint64_t len = read_compact_size();

        if ((uint64_t)(end() - m_cur_it) < len) {
            std::string _err = "error in data index: ";
            _err += jub::to_string(m_last_op_it - begin());
            throw std::runtime_error(_err);
        }
        v.clear();
        v.insert(v.end(), m_cur_it, m_cur_it + (int)len);
        v.reset_it();
        m_last_op_it = m_cur_it;
        m_cur_it += (int)len;
        return *this;
    }

    uint64_t read_compact_size() {
        if (m_cur_it == end()) {
            std::string _err = "error in data index: ";
            _err += jub::to_string(m_last_op_it - begin());
            throw std::runtime_error(_err);
        }
        uint64_t len = 0;
        if (*m_cur_it == 0xfd) {
            m_last_op_it = m_cur_it;
            m_cur_it++;
            len = read_le_uint16();
        }
        else if (*m_cur_it == 0xfe) {
            m_last_op_it = m_cur_it;
            m_cur_it++;
            len = read_le_uint32();
        }
        else if (*m_cur_it == 0xff) {
            m_last_op_it = m_cur_it;
            m_cur_it++;
            len = read_le_uint64();
        }
        else {
            len = read_uint8();
        }

        return len;
    }

    void reset_it() {
        m_cur_it = begin();
        m_last_op_it = begin();
    }
    void reset_it(int offset) {
        m_cur_it = m_cur_it + offset;
    };

private:
    std::vector<unsigned char>::iterator m_cur_it;
    std::vector<unsigned char>::iterator m_last_op_it;
};
}
}
typedef bitcoin::util::uchar_vector uchar_vector;
#endif
