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

// ModExp computes modular exponentiation: (base^exp) % mod.
// See EIP-198: Precompiled contracts for bigint modular exponentiation.
// The out slice must be len(mod) bytes. If mod is empty, this is a no-op.
func ModExp(out []byte, base []byte, exp []byte, mod []byte) {
	if len(mod) == 0 {
		return
	}

	var basePtr, expPtr *C.uchar
	if len(base) > 0 {
		basePtr = (*C.uchar)(unsafe.Pointer(&base[0]))
	}
	if len(exp) > 0 {
		expPtr = (*C.uchar)(unsafe.Pointer(&exp[0]))
	}

	C.evmone_capi_modexp(
		(*C.uchar)(unsafe.Pointer(&out[0])),
		basePtr, C.size_t(len(base)),
		expPtr, C.size_t(len(exp)),
		(*C.uchar)(unsafe.Pointer(&mod[0])), C.size_t(len(mod)),
	)
}
