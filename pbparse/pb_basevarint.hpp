//
//  pb_basevarint.hpp
//  JubSDK
//
//  Created by Pan Min on 2020/10/22.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#ifndef pb_basevarint_hpp
#define pb_basevarint_hpp

#include <stdio.h>
#include <stdint.h>
#include <vector>


// Base 128 Varints
// https://developers.google.com/protocol-buffers/docs/encoding#varints
typedef struct pb_basevarint {

public:
    pb_basevarint() {}

    static size_t size(const  int64_t v);
    static size_t size(const uint64_t v);
    static size_t size(const  int32_t v);
    static size_t size(const uint32_t v);

    static bool encode(const  int64_t v, std::vector<uint8_t>& enc);
    static bool encode(const uint64_t v, std::vector<uint8_t>& enc);
    static bool encode(const  int32_t v, std::vector<uint8_t>& enc);
    static bool encode(const uint32_t v, std::vector<uint8_t>& enc);
} pb_basevarint;


#endif /* pb_basevarint_hpp */
