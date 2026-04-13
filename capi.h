// Copyright 2025 Erigon Technologies AG.
// SPDX-License-Identifier: Apache-2.0

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// evmone_capi_ec_add performs point addition on the elliptic curve alt_bn128.
// See EIP-196: Precompiled contracts for addition and scalar multiplication on the elliptic curve alt_bn128.
//
// Returns: 1: addition was successful
//          0: addition was not successful (an input point was invalid)
// Args:
//  Out:    out:  the serialized 64-byte resulting curve point (cannot be NULL)
//  In:     x_in: pointer to the 1st serialized 64-byte curve point (cannot be NULL)
//          y_in: pointer to the 2nd serialized 64-byte curve point (cannot be NULL)
int evmone_capi_ec_add(
    unsigned char* out,
    const unsigned char* x_in,
    const unsigned char* y_in);

// evmone_capi_modexp computes modular exponentiation: (base^exp) % mod.
// See EIP-198: Precompiled contracts for bigint modular exponentiation.
//
// The output buffer (out) must be at least mod_size bytes.
// If mod_size is 0, this is a no-op.
// If the modulus value is zero, the output is filled with zeros.
// Args:
//  Out:    out:       the result buffer, mod_size bytes (cannot be NULL if mod_size > 0)
//  In:     base:      the base, big-endian (can be NULL if base_size is 0)
//          base_size: size of base in bytes
//          exp:       the exponent, big-endian (can be NULL if exp_size is 0)
//          exp_size:  size of exp in bytes
//          mod:       the modulus, big-endian (cannot be NULL if mod_size > 0)
//          mod_size:  size of mod in bytes
void evmone_capi_modexp(
    unsigned char* out,
    const unsigned char* base, size_t base_size,
    const unsigned char* exp, size_t exp_size,
    const unsigned char* mod, size_t mod_size);

#ifdef __cplusplus
}
#endif
