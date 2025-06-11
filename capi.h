// Copyright 2025 Erigon Technologies AG.
// SPDX-License-Identifier: Apache-2.0

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

#ifdef __cplusplus
}
#endif
