//
//  scp03.hpp
//  JubSDK
//
//  Created by Pan Min on 2020/6/21.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#ifndef gpc_scp03_hpp
#define gpc_scp03_hpp

#include <stdio.h>
#include <string.h>
#include <memory>
#include <vector>
#include <algorithm>

typedef struct gpc_scp03 {

public:
     gpc_scp03() {}
    ~gpc_scp03() {}

    void reset() {
        resetCounter();
        mac_chain.clear();
    }

    void resetCounter() {
        gpc_scp03::counter = 1;
    }
    int incCounter() {
        return ++gpc_scp03::counter;
    }
    int getCounter() {
        return gpc_scp03::counter;
    }
    // initial Chaining Vector
    bool icv(const unsigned char *key, const int keyLen,
             unsigned char *icv, int* icvLen,
             bool forWrap = true);
    bool encrypt(const unsigned char *key, const int keyLen,
                 const unsigned char *icv, const int icvLen,
                 const unsigned char *cmd, const int cmdLen,
                 unsigned char *enc, int* encLen);
    bool decrypt(const unsigned char *key, const int keyLen,
                 const unsigned char *icv, const int icvLen,
                 const unsigned char *enc, const int encLen,
                 unsigned char *dec, int* decLen);
    bool cmac(const unsigned char *key, const int keyLen,
              const unsigned char *data, const int dataLen,
              unsigned char *mac, int* macLen);

    void macChaining(const std::vector<unsigned char>& chain);
    virtual std::vector<unsigned char> getMacChain() {
        return mac_chain;
    }

private:
    bool   pad80(const unsigned char* text, int offset, int length, int blocksize,
                 unsigned char* pad, int* ppadlength);
    bool unpad80(unsigned char* pad, int padlength,
                 unsigned char* unpad, int* punpadlength);
public:
    static int counter;

private:
    std::vector<unsigned char> mac_chain;
} scp03;


#endif /* scp03_hpp */
