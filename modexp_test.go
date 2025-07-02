// Copyright 2025 Erigon Technologies AG.
// SPDX-License-Identifier: Apache-2.0

// Package evmone wraps the evmone precompiles.
package evmone

import (
	"encoding/hex"
	"testing"
)

func TestModExp(t *testing.T) {
	inputHex := "000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000000200000000000000000000000000000000000000000000000000000000000000020ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
	input, err := hex.DecodeString(inputHex)
	if err != nil {
		t.Fatalf("failed to decode hex string: %v", err)
	}
	var output [32]byte
	ModExp(output[:], input)
	outputHex := hex.EncodeToString(output[:])
	if outputHex != "1a3e144c4d88691245905e7de147bd63a9397d752467c4596345e59e052325ff" {
		t.Fatalf("unexpected output: %s", outputHex)
	}
}

func BenchmarkModExp(b *testing.B) {
	inputHex := "0000000000000000000000000000000000000000000000000000000000000020000000000000000000000000000000000000000000000000000000000000000c0000000000000000000000000000000000000000000000000000000000000020ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
	input, err := hex.DecodeString(inputHex)
	if err != nil {
		b.Fatalf("failed to decode hex string: %v", err)
	}
	var output [32]byte
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		ModExp(output[:], input)
	}
}
