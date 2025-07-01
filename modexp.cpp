#include <cstdint>
#include <span>
#include <algorithm>
#include <gmp.h>
#include <cassert>
#include <intx/intx.hpp>


static void expmod_gmp(std::span<const uint8_t> base, std::span<const uint8_t> exp,
           std::span<const uint8_t> mod, uint8_t* output) noexcept
       {
           mpz_t b, e, m, r;  // NOLINT(*-isolate-declaration)
           mpz_inits(b, e, m, r, nullptr);
           mpz_import(b, base.size(), 1, 1, 0, 0, base.data());
           mpz_import(e, exp.size(), 1, 1, 0, 0, exp.data());
           mpz_import(m, mod.size(), 1, 1, 0, 0, mod.data());
           assert(mpz_sgn(m) != 0);

           mpz_powm(r, b, e, m);

           size_t export_size = 0;
           mpz_export(output, &export_size, 1, 1, 0, 0, r);
           assert(export_size <= mod.size());
           mpz_clears(b, e, m, r, nullptr);

           std::copy_backward(output, output + export_size, output + mod.size());
           std::fill_n(output, mod.size() - export_size, 0);
       }

void modexp(
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

    expmod_gmp(base, exp, mod, output);
}