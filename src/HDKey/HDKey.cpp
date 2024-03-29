/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   HDKey.cpp
 * Author: zxc_work
 *
 * Created on 2019年7月19日, 下午2:02
 */

#include "HDKey/HDKey.hpp"

namespace jub {


std::vector<JUB_UINT32> parsePath(const std::string& path) {

    if (path.empty()) {
        throw "Path error";
    }

    std::vector<uint32_t> path_vector;

    size_t i = 0;
    uint64_t n = 0;
    while (i < path.size()) {
        char c = path[i];
        if (c == 'm') {
            i += 2;
        }
        else if (   c >= '0'
                 && c <= '9'
                 ) {
            n *= 10;
            n += (uint32_t)(c - '0');
            if (n >= 0x80000000) {
                throw "Path error";
            }
            i++;
            if (i >= path.size()) {
                path_vector.push_back((uint32_t)n);
            }
        }
        else if (c == '\'') {
            if (i + 1 < path.size()) {
                if (         (i + 2 >= path.size())
                    || (path[i + 1] != '/')
                    || (path[i + 2] <  '0')
                    || (path[i + 2] >  '9')
                    ) {
                    throw "Path error";
                }
            }
            n |= 0x80000000;
            path_vector.push_back((uint32_t)n);
            n = 0;
            i += 2;
        }
        else if (c == '/') {
            if (         i + 1 >= path.size()
                || path[i + 1] <  '0'
                || path[i + 1] >  '9'
                ) {
                throw "Path error";
            }
            path_vector.push_back((uint32_t)n);
            n = 0;
            i++;
        }
        else {
            throw "Path error";
        }
    }

    return path_vector;
}

bool chainCodeFromPolkadotPath(std::string path,JUB_ENUM_CURVES curve, std::vector<std::string>& pathSeveralVer, std::vector<bool>& isHardVer)
{
    if (path.empty()) {
        return true;
    }
    std::string::size_type idx = path.find("//");
    std::string::size_type idx1 = path.find("/");
    if('/' != path[0] || (idx == std::string::npos && idx1 == std::string::npos))
    {
        return false;
    }
    int start = 1;
    if ('/' == path[1]) {
        start = 2;
        isHardVer.push_back(true);
    } else {
        isHardVer.push_back(false);
    }
    int end = 0;
    
    path.push_back('/');
    for (int i = 2; i < path.size(); i++) {
        end = i;
        std::string temp;
        
        if (path[i] == '/' && path[i + 1] == '/') {
            
            temp = path.substr(start,end - start);
            if (!isString(temp) && !isNumber(temp)) return false;
            
            start = i + 2;
            pathSeveralVer.push_back(temp);
            isHardVer.push_back(true);
            
        } else if(path[i] == '/' && path[i + 1] != '/' && path[i - 1] != '/') {
            if (JUB_ENUM_CURVES::ED25519 == curve && i != path.size() -1) {
                return false;//ed25519 not support
            }
            temp = path.substr(start,end - start);
            if (!isString(temp) && !isNumber(temp)) return false;

            start = i + 1;
            pathSeveralVer.push_back(temp);
            isHardVer.push_back(false);
        } else {
            //noting code
        }
    }
    isHardVer.pop_back();
    
    return true;
}



JUB_RV hdnode_pub_ckd(std::string xpub, std::string path, std::string curve, JUB_UINT32 xpubPrefix ,JUB_UINT32 xprvPrefix,HDNode* node, JUB_UINT32* parentFingerprint) {
    HDNode hdkey;
    if (0 == hdnode_deserialize(xpub.c_str(),
                                xpubPrefix, xprvPrefix,
                                curve.c_str(), &hdkey, parentFingerprint)) {
        try {
            auto vPath = parsePath(path);
            uint32_t depth = hdkey.depth;
            for (uint32_t i=0; i<vPath.size(); ++i) {
                if (i < depth) {
                    continue;
                }
                *parentFingerprint = hdnode_fingerprint(&hdkey);
                if (1 != hdnode_public_ckd(&hdkey, vPath[i])) {
                    return JUBR_ARGUMENTS_BAD;
                }
            }
        }
        catch( ...) {
            return JUBR_ARGUMENTS_BAD;
        }
        *node = hdkey;
        return JUBR_OK;
    }

    return JUBR_ARGUMENTS_BAD;
}


JUB_RV hdnode_priv_ckd(std::string xprv, std::string path, std::string curve, JUB_UINT32 xpubPrefix, JUB_UINT32 xprvPrefix, HDNode* node, JUB_UINT32* parentFingerprint) {

    HDNode hdkey;
    if (0 == hdnode_deserialize(xprv.c_str(),
                                xpubPrefix, xprvPrefix,
                                curve.c_str(), &hdkey, parentFingerprint)) {
        try {
            auto vPath = parsePath(path);
            uint32_t depth = hdkey.depth;
            for (uint32_t i=0; i<vPath.size(); ++i) {
                if (i < depth) {
                    continue;
                }
                *parentFingerprint = hdnode_fingerprint(&hdkey);
                if (1 != hdnode_private_ckd(&hdkey, vPath[i])) {
                    return JUBR_ARGUMENTS_BAD;
                }
                hdnode_fill_public_key(&hdkey);
            }
        }
        catch( ...) {
            return JUBR_ARGUMENTS_BAD;
        }
        hdnode_fill_public_key(&hdkey);
        *node = hdkey;
        return JUBR_OK;
    }

    return JUBR_ARGUMENTS_BAD;
}


} // namespace jub end
