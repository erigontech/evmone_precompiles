// Copyright 2025 Erigon Technologies AG.
// SPDX-License-Identifier: Apache-2.0

#include "capi.h"

#include "bn254.hpp"
#include "modexp.hpp"
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <span>

int evmone_capi_ec_add(
    unsigned char* out,
    const unsigned char* x_in,
    const unsigned char* y_in) {
    const auto x = evmmax::bn254::AffinePoint::from_bytes(
        std::span<const uint8_t, 64>{x_in, 64});
    const auto y = evmmax::bn254::AffinePoint::from_bytes(
        std::span<const uint8_t, 64>{y_in, 64});

    if (!x.has_value() || !y.has_value())
        return 0;

    if (!evmmax::bn254::validate(*x) || !evmmax::bn254::validate(*y))
        return 0;

    const auto res = evmmax::ecc::add_affine(*x, *y);
    res.to_bytes(std::span<uint8_t, 64>{out, 64});
    return 1;
}

void evmone_capi_modexp(
    unsigned char* out,
    const unsigned char* base, size_t base_size,
    const unsigned char* exp, size_t exp_size,
    const unsigned char* mod, size_t mod_size) {
    if (mod_size == 0)
        return;

    // Check if the modulus is zero; if so, the result is zero.
    if (std::all_of(mod, mod + mod_size, [](unsigned char b) { return b == 0; })) {
        std::memset(out, 0, mod_size);
        return;
    }

    // Provide a valid pointer for empty base/exp spans.
    const uint8_t dummy = 0;
    evmone::crypto::modexp(
        {base_size > 0 ? reinterpret_cast<const uint8_t*>(base) : &dummy, base_size},
        {exp_size > 0 ? reinterpret_cast<const uint8_t*>(exp) : &dummy, exp_size},
        {reinterpret_cast<const uint8_t*>(mod), mod_size},
        reinterpret_cast<uint8_t*>(out));
}
