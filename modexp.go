// Copyright 2025 Erigon Technologies AG.
// SPDX-License-Identifier: Apache-2.0

// Package evmone wraps the evmone precompiles.
package evmone

/*
#cgo CXXFLAGS: -std=c++20
#cgo CXXFLAGS: -I./intx/include
#cgo CXXFLAGS: -I./evmone/include
#cgo CXXFLAGS: -I./evmone/lib/evmone_precompiles
#cgo CXXFLAGS: -I./evmone/test/state
#cgo LDFLAGS: -lgmp

// These don't help with anything:
#cgo noescape evmone_modexp
#cgo nocallback evmone_modexp

#include "capi.h"
*/
import "C"

import "unsafe"

// ModExp performs modular exponentiation using input data and writes the result to out.
// The out parameter holds the output, while input contains the modular exponentiation inputs.
// The len(out) must match the expected output size (i.e. the modulus length).
func ModExp(out []byte, input []byte) {
	outData := (*C.uchar)(unsafe.Pointer(&out[0]))
	inData := (*C.uchar)(unsafe.Pointer(&input[0]))

	C.evmone_modexp(inData, (C.size_t)(len(input)), outData, (C.size_t)(len(out)))
}
