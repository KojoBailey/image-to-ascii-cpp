#include <array>
#include <cmath>
#include <algorithm>
#include <expected>

#include <iostream>

class ASCII {
public:
	static auto rgb_to_symbol(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a)
	-> std::expected<char, std::string>
	{
		auto rgb_byte_to_float = [](const unsigned char channel) {
			return channel / 255.0f;
		};
		const float rn = rgb_byte_to_float(r);
		const float gn = rgb_byte_to_float(g);
		const float bn = rgb_byte_to_float(b);
		const float an = rgb_byte_to_float(a);

		// Using HSL formula.
		const float lightness = (std::min({rn, gn, bn}) + std::max({rn, gn, bn})) / 2 * an;
		const size_t index = std::round(lightness * (symbols.size() - 1));
		if (index < 0 || index > 9) {
			return std::unexpected{"RGB out of range."};
		}
		return symbols[index];
	}

	static constexpr std::array<char, 10> symbols{' ', '.', ':', '-', '=', '+', '/', '%', '#', '@'};
};