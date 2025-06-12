// Copyright 2025 Erigon Technologies AG.
// SPDX-License-Identifier: Apache-2.0

// Package evmone wraps the evmone precompiles.
package evmone

/*
#cgo CXXFLAGS: -std=c++20
#cgo CXXFLAGS: -I./intx/include
#cgo CXXFLAGS: -I./evmone/include
#cgo CXXFLAGS: -I./evmone/lib/evmone_precompiles

#cgo noescape evmone_capi_ec_add
#cgo nocallback evmone_capi_ec_add
#cgo noescape evmone_capi_ec_mul
#cgo nocallback evmone_capi_ec_mul

#include "capi.h"
*/
import "C"

import "unsafe"

// EcAdd performs point addition on the elliptic curve alt_bn128.
// See EIP-196: Precompiled contracts for addition and scalar multiplication on the elliptic curve alt_bn128.
// out is set to x + y.
// Return true if the addition was successful an false otherwise (an input point was invalid).
func EcAdd(out *[64]byte, x *[64]byte, y *[64]byte) bool {
	outData := (*C.uchar)(unsafe.Pointer(&out[0]))
	xData := (*C.uchar)(unsafe.Pointer(&x[0]))
	yData := (*C.uchar)(unsafe.Pointer(&y[0]))

	return C.evmone_capi_ec_add(outData, xData, yData) != 0
}

// EcMul performs scalar multiplication on the elliptic curve alt_bn128.
// See EIP-196: Precompiled contracts for addition and scalar multiplication on the elliptic curve alt_bn128.
// out is set to s * x.
// Return true if the multiplication was successful an false otherwise (the input point was invalid).
func EcMul(out *[64]byte, x *[64]byte, s *[32]byte) bool {
	outData := (*C.uchar)(unsafe.Pointer(&out[0]))
	xData := (*C.uchar)(unsafe.Pointer(&x[0]))
	sData := (*C.uchar)(unsafe.Pointer(&s[0]))

	return C.evmone_capi_ec_mul(outData, xData, sData) != 0
}
