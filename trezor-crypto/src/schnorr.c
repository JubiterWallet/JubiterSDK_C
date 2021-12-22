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

#include "TrezorCrypto/bignum.h"
#include "TrezorCrypto/ecdsa.h"
#include "TrezorCrypto/sha2.h"
#include <TrezorCrypto/schnorr.h>
#include <stdbool.h>
#include <stdint.h>

bool has_even_y(const curve_point *P) { return bn_is_even(&P->y); }

bool lift_x(const ecdsa_curve *curve, const uint8_t *pk, curve_point *P) {
    if (!ecdsa_read_pubkey(curve, pk, P)) {
        return false;
    }
    if (!has_even_y(P)) {
        bn_subtract(&curve->prime, &P->y, &P->y);
    }
    return true;
}

// r = hash-BIP0340/nonce
static void calc_r(bignum256 *t, curve_point *P, const uint8_t *m, bignum256 *r) {
    SHA256_CTX ctx;
    uint8_t x[32];
    uint8_t digest[SHA256_DIGEST_LENGTH];

    // tag
    sha256_Raw((uint8_t *)"BIP0340/nonce", 13, digest);

    sha256_Init(&ctx);
    sha256_Update(&ctx, digest, SHA256_DIGEST_LENGTH);
    sha256_Update(&ctx, digest, SHA256_DIGEST_LENGTH);
    bn_write_be(t, x);
    sha256_Update(&ctx, x, 32);
    bn_write_be(&P->x, x);
    sha256_Update(&ctx, x, 32);
    sha256_Update(&ctx, m, 32);
    sha256_Final(&ctx, digest);

    // Convert the raw bigendian 256 bit value to a normalized, partly reduced bignum
    bn_read_be(digest, r);
}

// e = hash-BIP0340/challenge(R||P||m)
static void calc_e(const bignum256 *r, const curve_point *P, const uint8_t *m, bignum256 *e) {
    SHA256_CTX ctx;
    uint8_t digest[SHA256_DIGEST_LENGTH];
    uint8_t x[32];
    // tag
    sha256_Raw((uint8_t *)"BIP0340/challenge", 17, digest);

    sha256_Init(&ctx);
    sha256_Update(&ctx, digest, SHA256_DIGEST_LENGTH);
    sha256_Update(&ctx, digest, SHA256_DIGEST_LENGTH);
    bn_write_be(r, x);
    sha256_Update(&ctx, x, 32);
    bn_write_be(&P->x, x);
    sha256_Update(&ctx, x, 32);
    sha256_Update(&ctx, m, 32);
    sha256_Final(&ctx, digest);
    bn_read_be(digest, e);
}

// t = d ^ hash-BIP0340/aux(k)
static void calc_t(const bignum256 *d, const bignum256 *k, bignum256 *t) {
    SHA256_CTX ctx;
    uint8_t digest[SHA256_DIGEST_LENGTH];
    uint8_t kbs[32];

    bn_write_be(k, kbs);
    // tag
    sha256_Raw((uint8_t *)"BIP0340/aux", 11, digest);

    // hash-BIP0340/aux(k)
    sha256_Init(&ctx);
    sha256_Update(&ctx, digest, SHA256_DIGEST_LENGTH);
    sha256_Update(&ctx, digest, SHA256_DIGEST_LENGTH);
    sha256_Update(&ctx, kbs, 32);
    sha256_Final(&ctx, digest);

    bn_read_be(digest, t);
    // d ^ t
    bn_xor(t, d, t);
}

// Returns 0 if signing succeeded
int schnorr_sign(const ecdsa_curve *curve, const uint8_t *sk, const bignum256 *k, const uint8_t *m,
                 schnorr_sign_pair *result) {
    curve_point P;
    curve_point R;
    bignum256 d;
    bignum256 t;
    bignum256 s;
    bignum256 k2; /* k' */

    // d = int(sk)
    bn_read_be(sk, &d);

    // P = d*G
    scalar_multiply(curve, &d, &P);

    // d = d if has_even_y(P) else d = n - d
    if (!has_even_y(&P))
        bn_subtract(&curve->order, &d, &d);

    // t = d ^ hash-BIP0340/aux(k)
    calc_t(&d, k, &t);

    // rand = hash-BIP0340/nonce(t, P, m)
    calc_r(&t, &P, m, &k2);

    // k' = rand % n
    bn_fast_mod(&k2, &curve->order);

    if (bn_is_zero(&k2))
        return 1;

    // R = k'*G
    scalar_multiply(curve, &k2, &R);

    // k = k' if has_even_y(R) else n - k'
    if (!has_even_y(&R))
        bn_subtract(&curve->order, &k2, &k2);

    // e = hash-BIP0340/challenge(R,P,m)
    calc_e(&R.x, &P, m, &s);
    // e %= n
    bn_fast_mod(&s, &curve->order);

    // s = (k+ed)mod n
    bn_multiply(&d, &s, &curve->order);
    bn_addmod(&s, &k2, &curve->order);

    if (bn_is_zero(&R.x) || bn_is_zero(&s))
        return 1;
    bn_write_be(&R.x, result->r);
    bn_write_be(&s, result->s);
    return 0;
}

// Returns 0 if verification succeeded
int schnorr_verify(const ecdsa_curve *curve, const uint8_t *pk, const uint8_t *m, const schnorr_sign_pair *sig) {
    curve_point P;
    curve_point R;
    bignum256 r;
    bignum256 s;
    bignum256 e;

    bn_read_be(sig->r, &r);
    bn_read_be(sig->s, &s);

    if (bn_is_zero(&r))
        return 1;
    if (bn_is_zero(&s))
        return 2;
    if (!bn_is_less(&r, &curve->prime))
        return 3;
    if (!bn_is_less(&s, &curve->order))
        return 4;
    if (!lift_x(curve, pk, &P)) {
        return 5;
    }

    // e = hash-BIP0340/challenge(r, P, m)
    calc_e(&r, &P, m, &e);
    // e %= n
    bn_fast_mod(&e, &curve->order);

    // R = s*G - e*P
    bn_subtract(&curve->order, &e, &e);
    scalar_multiply(curve, &s, &R);
    point_multiply(curve, &e, &P, &P);
    point_add(curve, &P, &R);

    if (point_is_infinity(&R)) {
        return 6;
    }
    if (!has_even_y(&R)) {
        return 7;
    }
    if (!bn_is_equal(&r, &R.x)) {
        return 8; // success
    }

    // success
    return 0;
}
