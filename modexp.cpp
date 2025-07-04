#include <cstdint>
#include <span>
#include <algorithm>
#include <cassert>
#include <intx/intx.hpp>

#include <evmone_precompiles/modexp.cpp>

extern "C" void evmone_modexp(
    const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) noexcept
{
    static constexpr auto LEN_SIZE = sizeof(intx::uint256);
    static constexpr auto HEADER_SIZE = 3 * LEN_SIZE;
    static constexpr auto LEN32_OFF = LEN_SIZE - sizeof(uint32_t);

    // The output size equal to the modulus size.
    const auto mod_len = output_size;

    // Handle short incomplete input up front. The answer is 0 of the length of the modulus.
    if (input_size <= HEADER_SIZE) [[unlikely]]
    {
        std::fill_n(output, output_size, 0);
        return;
    }

    const auto base_len = intx::be::unsafe::load<uint32_t>(&input[LEN32_OFF]);
    const auto exp_len = intx::be::unsafe::load<uint32_t>(&input[LEN_SIZE + LEN32_OFF]);
    assert(intx::be::unsafe::load<uint32_t>(&input[2 * LEN_SIZE + LEN32_OFF]) == mod_len);

    const std::span payload{input + HEADER_SIZE, input_size - HEADER_SIZE};
    const size_t mod_off = base_len + exp_len;  // Cannot overflow if gas cost computed before.
    const auto mod_explicit = payload.subspan(std::min(mod_off, payload.size()), mod_len);

    // Early handle the mod being zero.
    // This serves two purposes:
    // - bigint libraries don't like such a modulus because division by 0 is not well-defined,
    // - having non-zero modulus guarantees that base and exp aren't out-of-bounds.
    if (std::ranges::all_of(mod_explicit, [](uint8_t b) { return b == 0; })) [[unlikely]]
    {
        // The modulus is zero, so the result is zero.
        std::fill_n(output, output_size, 0);
        return;
    }

    const auto mod_requires_padding = mod_explicit.size() != mod_len;
    if (mod_requires_padding) [[unlikely]]
    {
        // The modulus is the last argument, and some of its bytes may be missing and be implicitly
        // zero. In this case, copy the explicit modulus bytes to the output buffer and pad the rest
        // with zeroes. The output buffer is guaranteed to have exactly the modulus size.
        const auto [_, output_p] = std::ranges::copy(mod_explicit, output);
        std::fill(output_p, output + output_size, 0);
    }

    const auto base = payload.subspan(0, base_len);
    const auto exp = payload.subspan(base_len, exp_len);
    const auto mod = mod_requires_padding ? std::span{output, mod_len} : mod_explicit;

    evmone::crypto::modexp(base, exp, mod, output);
}