#include <array>
#include <cmath>

class ASCII {
public:
	static constexpr char rgb_to_symbol(const char r, const char g, const char b)
	{
		const float intensity = (r + g + b) / (255.0f * 3.0f);
		const size_t index = std::round((1.0f - intensity) * 9);
		return symbols[index];
	}

private:
	static constexpr std::array<char, 10> symbols{'.', ':', '-', '=', '+', '*', '/', '%', '#', '@'};
};