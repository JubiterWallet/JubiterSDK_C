//
//  tlvparse.hpp
//  JubSDK
//
//  Created by Pan Min on 2020/7/15.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#ifndef GPC_TLV_PARSE_H
#define GPC_TLV_PARSE_H

#include <stdio.h>
#include <string.h>

/*
 * TLV decoding routines
 */
int gpc_tlv_get_tag( unsigned char **p,
               const unsigned char *end,
               unsigned short *tag,
               size_t *len );


#endif /* tlvparse_hpp */
