#include <array>
#include <cmath>
#include <algorithm>
#include <expected>

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
		const size_t max_index = symbols.size() - 1;
		const float lightness = (std::min({rn, gn, bn}) + std::max({rn, gn, bn})) / 2 * an;
		const size_t index = std::round(lightness * max_index);
		if (index < 0 || index > max_index) {
			return std::unexpected{"RGB out of range."};
		}
		return symbols[index];
	}

	static constexpr std::array<char, 10> symbols{' ', '.', ':', '-', '=', '+', '/', '%', '#', '@'};
};