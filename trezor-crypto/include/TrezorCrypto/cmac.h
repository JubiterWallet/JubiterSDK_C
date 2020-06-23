/**
 * Copyright (c) 2020 tools.ietf.org
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT HMAC_SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
// code from: https://tools.ietf.org/html/rfc4493
// check: https://artjomb.github.io/cryptojs-extension/
#ifndef __CMAC_H__
#define __CMAC_H__

#include <stdint.h>
#include <TrezorCrypto/aes.h>

#ifdef __cplusplus
extern "C" {
#endif

extern unsigned char const_Zero[16];

void AES_CMAC(unsigned char *key, unsigned char *input, int length,
              unsigned char *mac);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
