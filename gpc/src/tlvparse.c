/*
 *  Generic TLV parsing
 *
 *  Copyright (C) 2018-2020, JuBiter Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of JuBiter2.0 (http://www.jubiterwallet.com)
 */

#ifndef GPC_TLV_PARSE_C
#define GPC_TLV_PARSE_C

#include <stdio.h>
#include <string.h>
#include "tag.h"


/*
 * TLV decoding routines
 */
int gpc_tlv_get_tag( unsigned char **p,
               const unsigned char *end,
               unsigned short *tag,
               size_t *len )
{
    unsigned short _tag = **p;
    unsigned char subTag = (*(*p+1));
    size_t length = (*(*p+1));
    switch( _tag ) {
    case GPC_TLV_SCP11CRT:
    {
        length = (*(*p+2));
        _tag = (GPC_TLV_SCP11CRT <<8) + subTag;
        switch (subTag) {
        case (GPC_TLV_SCP11CRT_ENTITY & 0x00FF):   // '7F21' - Table 6-12: Certificate Format
        {
            switch (length) {
            case 0x82:
            {
                length  = (*(*p+3)) << 8;
                length |= (*(*p+4)) & 0xFF;
                break;
            }
            case 0x81:
            {
                length = (*(*p+3));
                break;
            }
            default:
                return( GPC_SCP11_ERR_TLV_OUT_OF_DATA );
            }

            *tag = _tag;
            break;
        }
        case (GPC_TLV_SCP11CRT_PUBKEY & 0x00FF):   // '7F49' - Public Key
        {
            *tag = _tag;
            break;
        }
        default:
            return( GPC_SCP11_ERR_TLV_OUT_OF_DATA );
        }
        break;
    }
    case GPC_TLV_MA_RECEIPT:
    case GPC_TLV_PK_Q:
    case GPC_TLV_PK_PARAM:
    case GPC_TLV_SCP11CRT_CAKLOCID: // '42' - CA-KLOC (or KA-KLOC) Identifier
    case GPC_TLV_SCP11CRT_DISC_53:  // '53' - Discretionary Data
    case GPC_TLV_SCP11CRT_DISC_73:  // '73' - Discretionary Data
    case GPC_TLV_SCP11CRT_SN:       // '93' - Certificate Serial Number
    case GPC_TLV_SCP11CRT_KEYUSAGE: // '95' - Key Usage:
                                    //        '82': Digital signature verification (CERT .KA-KLOC.ECDSA)
                                    //        '00 80': Key agreement (CERT .OCE.ECKA)
    {
        *tag = _tag;
        break;
    }
    case GPC_TLV_SCP11CRT_ITEM:
    {
        length = (*(*p+2));
        _tag = (GPC_TLV_SCP11CRT_ITEM <<8) + subTag;
        switch (subTag) {
        case (GPC_TLV_SCP11CRT_SUBJECTID & 0x00FF):     // '5F20' - Subject Identifier
        case (GPC_TLV_SCP11CRT_EXPEDATE & 0x00FF):      // '5F24' - Expiration Date (YYYYMMDD, BCD M format)
        case (GPC_TLV_SCP11CRT_EFFEDATE & 0x00FF):      // '5F25' - Effective  Date (YYYYMMDD, O BCD format)
        case (GPC_TLV_SCP11CRT_SIGNATURE & 0x00FF):     // '5F37' - Signature
        case (GPC_TLV_MA_PK & 0x00FF):                  // '5F49' - ePK.OCE.ECKA
        {
            *tag = _tag;
            break;
        }
        default:
            return( GPC_SCP11_ERR_TLV_OUT_OF_DATA );
        }
        break;
    }
    case GPC_TLV_SCP11CRT_RESTR:
    {
        length = (*(*p+2));
        _tag = (GPC_TLV_SCP11CRT_RESTR <<8) + subTag;
        switch (subTag) {
        case (GPC_TLV_SCP11CRT_RESTR_C & 0x00FF):  // 'BF20' - Restrictions under SCP11c
        {
            *tag = _tag;
            break;
        }
        default:
            return( GPC_SCP11_ERR_TLV_OUT_OF_DATA );
        }
        break;
    }
    default:
        return( GPC_SCP11_ERR_TLV_OUT_OF_DATA );
    }

    if( *len > (size_t) ( end - *p ) ) {
        return( GPC_SCP11_ERR_TLV_OUT_OF_DATA );
    }

    *len = length;

    return( 0 );
}


#endif /* GPC_TLV_PARSE_C */
