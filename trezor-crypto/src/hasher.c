/**
 * Copyright (c) 2017 Saleem Rashid
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

#include <TrezorCrypto/hasher.h>
#include <TrezorCrypto/ripemd160.h>

// JuBiter-modified
int hasher_InitParam(Hasher *hasher, HasherType type, const void *param, uint32_t param_size) {
    int ret = 0;

	hasher->type = type;
	hasher->param = param;
	hasher->param_size = param_size;

	switch (hasher->type) {
	case HASHER_SHA2:
	case HASHER_SHA2D:
	case HASHER_SHA2_RIPEMD:
		sha256_Init(&hasher->ctx.sha2);
		break;
	case HASHER_SHA3:
	case HASHER_SHA3K:
		sha3_256_Init(&hasher->ctx.sha3);
		break;
	case HASHER_BLAKE:
	case HASHER_BLAKED:
	case HASHER_BLAKE_RIPEMD:
		blake256_Init(&hasher->ctx.blake);
		break;
	case HASHER_GROESTLD_TRUNC:
		groestl512_Init(&hasher->ctx.groestl);
		break;
	case HASHER_BLAKE2B:
		ret = blake2b_Init(&hasher->ctx.blake2b, 32);
		break;
	case HASHER_BLAKE2B_PERSONAL:
        ret = blake2b_InitPersonal(&hasher->ctx.blake2b, 32, hasher->param, hasher->param_size);
		break;
    case HASHER_SHA2_KECCAK:
        keccak_256_Init(&hasher->ctx.sha3);
        break;
    case HASHER_SHA3_KECCAK:
        keccak_512_Init(&hasher->ctx.sha3);
        break;
	}
    return ret;
}

// JuBiter-modified
int hasher_Init(Hasher *hasher, HasherType type) {
	return hasher_InitParam(hasher, type, NULL, 0);
}

// JuBiter-modified
int hasher_Reset(Hasher *hasher) {
	return hasher_InitParam(hasher, hasher->type, hasher->param, hasher->param_size);
}

// JuBiter-modified
int hasher_Update(Hasher *hasher, const uint8_t *data, size_t length) {
    int ret = 0;
	switch (hasher->type) {
	case HASHER_SHA2:
	case HASHER_SHA2D:
	case HASHER_SHA2_RIPEMD:
		sha256_Update(&hasher->ctx.sha2, data, length);
		break;
	case HASHER_SHA3:
	case HASHER_SHA3K:
		sha3_Update(&hasher->ctx.sha3, data, length);
		break;
	case HASHER_BLAKE:
	case HASHER_BLAKED:
	case HASHER_BLAKE_RIPEMD:
		blake256_Update(&hasher->ctx.blake, data, length);
		break;
	case HASHER_GROESTLD_TRUNC:
		groestl512_Update(&hasher->ctx.groestl, data, length);
		break;
	case HASHER_BLAKE2B:
	case HASHER_BLAKE2B_PERSONAL:
		ret = blake2b_Update(&hasher->ctx.blake2b, data, length);
		break;
	case HASHER_SHA2_KECCAK:
	case HASHER_SHA3_KECCAK:
		keccak_Update(&hasher->ctx.sha3, data, length);
		break;
	}
    return ret;
}

// JuBiter-modified
int hasher_Final(Hasher *hasher, uint8_t hash[HASHER_DIGEST_LENGTH]) {
    int ret = 0;
	switch (hasher->type) {
	case HASHER_SHA2:
		sha256_Final(&hasher->ctx.sha2, hash);
		break;
	case HASHER_SHA2D:
		sha256_Final(&hasher->ctx.sha2, hash);
		hasher_Raw(HASHER_SHA2, hash, HASHER_DIGEST_LENGTH, hash);
		break;
	case HASHER_SHA2_RIPEMD:
		sha256_Final(&hasher->ctx.sha2, hash);
		ripemd160(hash, HASHER_DIGEST_LENGTH, hash);
		break;
	case HASHER_SHA3:
		sha3_Final(&hasher->ctx.sha3, hash);
		break;
	case HASHER_SHA3K:
		keccak_Final(&hasher->ctx.sha3, hash);
		break;
	case HASHER_BLAKE:
		blake256_Final(&hasher->ctx.blake, hash);
		break;
	case HASHER_BLAKED:
		blake256_Final(&hasher->ctx.blake, hash);
		hasher_Raw(HASHER_BLAKE, hash, HASHER_DIGEST_LENGTH, hash);
		break;
	case HASHER_BLAKE_RIPEMD:
		blake256_Final(&hasher->ctx.blake, hash);
		ripemd160(hash, HASHER_DIGEST_LENGTH, hash);
		break;
	case HASHER_GROESTLD_TRUNC:
		groestl512_DoubleTrunc(&hasher->ctx.groestl, hash);
		break;
	case HASHER_BLAKE2B:
	case HASHER_BLAKE2B_PERSONAL:
		ret = blake2b_Final(&hasher->ctx.blake2b, hash, 32);
		break;
	case HASHER_SHA2_KECCAK:
	case HASHER_SHA3_KECCAK:
        keccak_Final(&hasher->ctx.sha3, hash);
		break;
	}
    return ret;
}

// JuBiter-modified
int hasher_Raw(HasherType type, const uint8_t *data, size_t length, uint8_t hash[HASHER_DIGEST_LENGTH]) {
    int ret = 0;
	Hasher hasher;
	ret = hasher_Init(&hasher, type);
    if (0 != ret) {
        return ret;
    }
    ret = hasher_Update(&hasher, data, length);
    if (0 != ret) {
        return ret;
    }
    return hasher_Final(&hasher, hash);
}

// JuBiter-modified
int hasher_InitParam_Raw(Hasher *phasher, const uint8_t *data, size_t length, uint8_t hash[HASHER_DIGEST_LENGTH]) {
    int ret = 0;
    Hasher hasher;
    ret = hasher_InitParam(&hasher, phasher->type, phasher->param, phasher->param_size);
    if (0 != ret) {
        return ret;
    }
    ret = hasher_Update(&hasher, data, length);
    if (0 != ret) {
        return ret;
    }
    return hasher_Final(&hasher, hash);
}
