// Copyright 2025 Erigon Technologies AG.
// SPDX-License-Identifier: Apache-2.0

// Package evmone wraps the evmone precompiles.
package evmone

/*
#cgo CXXFLAGS: -std=c++20
#cgo CXXFLAGS: -I./intx/include
#cgo CXXFLAGS: -I./evmone/include
#cgo CXXFLAGS: -I./evmone/lib/evmone_precompiles

#include "capi.h"
*/
import "C"

import "unsafe"

// EcAdd performs point addition on the elliptic curve alt_bn128.
// See EIP-196: Precompiled contracts for addition and scalar multiplication on the elliptic curve alt_bn128.
// Return true if the addition was successful an false otherwise (an input point was invalid).
func EcAdd(out *[64]byte, p *[64]byte, q *[64]byte) bool {
	outData := (*C.uchar)(unsafe.Pointer(&out[0]))
	pData := (*C.uchar)(unsafe.Pointer(&p[0]))
	qData := (*C.uchar)(unsafe.Pointer(&q[0]))

	return C.evmone_capi_ec_add(outData, pData, qData) != 0
}
