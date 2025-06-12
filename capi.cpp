// Copyright 2025 Erigon Technologies AG.
// SPDX-License-Identifier: Apache-2.0

#include "capi.h"

#include "bn254.hpp"

int evmone_capi_ec_add(
    unsigned char* out,
    const unsigned char* x_in,
    const unsigned char* y_in) {
    const evmmax::bn254::Point x{intx::be::unsafe::load<intx::uint256>(x_in),
                                 intx::be::unsafe::load<intx::uint256>(x_in + 32)};
    const evmmax::bn254::Point y{intx::be::unsafe::load<intx::uint256>(y_in),
                                 intx::be::unsafe::load<intx::uint256>(y_in + 32)};

    if (!evmmax::bn254::validate(x) || !evmmax::bn254::validate(y)) {
        return 0;
    }

    const auto res = evmmax::bn254::add(x, y);
    intx::be::unsafe::store(out, res.x);
    intx::be::unsafe::store(out + 32, res.y);
    return 1;
}

int evmone_capi_ec_mul(unsigned char* out,
                       const unsigned char* x_in,
                       const unsigned char* s_in) {
    const evmmax::bn254::Point x{intx::be::unsafe::load<intx::uint256>(x_in),
                                 intx::be::unsafe::load<intx::uint256>(x_in + 32)};
    const auto s = intx::be::unsafe::load<intx::uint256>(s_in);

    if (!evmmax::bn254::validate(x)) {
        return 0;
    }

    const auto res = evmmax::bn254::mul(x, s);
    intx::be::unsafe::store(out, res.x);
    intx::be::unsafe::store(out + 32, res.y);
    return 1;
}
