/**
 * Copyright (c) 2019 Anatolii Kurotych
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __SCHNORR_H__
#define __SCHNORR_H__

#include <TrezorCrypto/ecdsa.h>

#if defined(__cplusplus)
extern "C"
{
#endif

// result of sign operation
typedef struct {
  uint8_t r[32];
  uint8_t s[32];
} schnorr_sign_pair;

// sign/verify returns 0 if operation succeeded

// k is a random from [1, ..., order-1]
int schnorr_sign(const ecdsa_curve *curve, const uint8_t *sk, const bignum256 *k, const uint8_t *m, schnorr_sign_pair *sig);
int schnorr_verify(const ecdsa_curve *curve, const uint8_t *pk, const uint8_t *m, const schnorr_sign_pair *sig);
#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
