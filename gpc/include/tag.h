//
//  tag.hpp
//  JubSDK
//
//  Created by Pan Min on 2020/6/9.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#ifndef gpc_tag_h
#define gpc_tag_h


/**
 * \name TLV Error codes
 * These error codes are OR'ed to X509 error codes for
 * higher error granularity.
 * ASN1 is a standard to specify data structures.
 * \{
 */
#define GPC_SCP11_ERR_TLV_OUT_OF_DATA                      -0x0060  /**< Out of data when parsing an TLV data structure. */
#define GPC_SCP11_ERR_TLV_UNEXPECTED_TAG                   -0x0062  /**< ASN1 tag was of an unexpected value. */
#define GPC_SCP11_ERR_TLV_INVALID_LENGTH                   -0x0064  /**< Error when trying to determine the length or invalid length. */
#define GPC_SCP11_ERR_TLV_LENGTH_MISMATCH                  -0x0066  /**< Actual length differs from expected length. */
#define GPC_SCP11_ERR_TLV_INVALID_DATA                     -0x0068  /**< Data is invalid. (not used) */
#define GPC_SCP11_ERR_TLV_ALLOC_FAILED                     -0x006A  /**< Memory allocation failed */
#define GPC_SCP11_ERR_TLV_BUF_TOO_SMALL                    -0x006C  /**< Buffer too small when writing TLV data structure. */

#define GPC_TLV_BOOLEAN                 0x01

// SharedInfo
#define GPC_TLV_SHAREDINFO_SCP_ID_PARAM 0x90
#define GPC_TLV_SHAREDINFO_KEYUSAGE     0x95
#define GPC_TLV_SHAREDINFO_KEYTYPE      0x80
#define GPC_TLV_SHAREDINFO_KEYLENGTH    0x81
#define GPC_TLV_SHAREDINFO_HOSTID       0x84


// MUTUAL AUTHENTICATE Data Field
#define GPC_TLV_MA_CR                   0xA6
#define GPC_TLV_MA_PK                   0x5F49
#define GPC_TLV_MA_RECEIPT              0x86

// scp11 crt
#define GPC_TLV_SCP11CRT                0x7F
#define GPC_TLV_SCP11CRT_ENTITY         0x7F21
#define GPC_TLV_SCP11CRT_PUBKEY         0x7F49

#define GPC_TLV_SCP11CRT_CAKLOCID       0x42
#define GPC_TLV_SCP11CRT_DISC_53        0x53
#define GPC_TLV_SCP11CRT_DISC_73        0x73
#define GPC_TLV_SCP11CRT_SN             0x93
#define GPC_TLV_SCP11CRT_KEYUSAGE       0x95

#define GPC_TLV_SCP11CRT_ITEM           0x5F
#define GPC_TLV_SCP11CRT_SUBJECTID      0x5F20
#define GPC_TLV_SCP11CRT_EXPEDATE       0x5F24
#define GPC_TLV_SCP11CRT_EFFEDATE       0x5F25
#define GPC_TLV_SCP11CRT_SIGNATURE      0x5F37

#define GPC_TLV_SCP11CRT_RESTR          0xBF
#define GPC_TLV_SCP11CRT_RESTR_C        0xBF20

#define GPC_TLV_PK_Q                    0xB0
#define GPC_TLV_PK_PARAM                0xF0


#endif /* gpc_tag_h */
