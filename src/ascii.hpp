#include <array>
#include <cstdint>

class ASCII {
public:
	static constexpr char rgb_to_symbol(const char r, const char g, const char b)
	{
		const size_t max = 255 * 3;
		return symbols[((max - (r + g + b)) / max) * 10];
	}

private:
	static constexpr std::array<char, 10> symbols{'.', ':', '-', '=', '+', '*', '/', '%', '#', '@'};
};