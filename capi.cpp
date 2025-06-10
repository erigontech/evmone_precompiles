// Copyright 2025 Erigon Technologies AG.
// SPDX-License-Identifier: Apache-2.0

#include "capi.h"

#include "bn254.hpp"

int evmone_capi_ec_add(
    unsigned char* out,
    const unsigned char* p_in,
    const unsigned char* q_in) {
    const evmmax::bn254::Point p{intx::be::unsafe::load<intx::uint256>(p_in),
                                 intx::be::unsafe::load<intx::uint256>(p_in + 32)};
    const evmmax::bn254::Point q = {intx::be::unsafe::load<intx::uint256>(q_in),
                                    intx::be::unsafe::load<intx::uint256>(q_in + 32)};

    if (!evmmax::bn254::validate(p) || !evmmax::bn254::validate(q)) {
        return 0;
    }

    const auto res = evmmax::bn254::add(p, q);
    intx::be::unsafe::store(out, res.x);
    intx::be::unsafe::store(out + 32, res.y);
    return 1;
}
