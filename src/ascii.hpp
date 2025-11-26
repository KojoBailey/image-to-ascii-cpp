#include <array>
#include <cmath>
#include <algorithm>
#include <optional>

#include <iostream>

class ASCII {
public:
	static std::optional<char> rgb_to_symbol(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a)
	{
		const float intensity = (r + g + b) / (255.0f * 3.0f) * a / 255.0f;
		const size_t index = std::round(intensity * (symbols.size() - 1));
		if (index < 0 || index > 9) {
			std::cout << std::format("Index: {}\n", index);
			return {};
		}
		return symbols[index];
	}

	static constexpr std::array<char, 10> symbols{' ', '.', ':', '-', '=', '+', '/', '%', '#', '@'};
};