// Copyright 2025 Erigon Technologies AG.
// SPDX-License-Identifier: Apache-2.0

#include "capi.h"

#include "bn254.hpp"
#include <cstdint>
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
