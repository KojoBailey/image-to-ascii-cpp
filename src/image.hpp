#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <cstdint>
#include <expected>
#include <filesystem>
#include <vector>

#include "stb_image.h"

struct Pixel {
	std::uint8_t r{0};
	std::uint8_t g{0};
	std::uint8_t b{0};
	std::uint8_t a{255};

	Pixel() = default;
	Pixel(const std::uint8_t _r, const std::uint8_t _g, const std::uint8_t _b, const std::uint8_t _a) { set(_r, _g, _b, _a); }

	void set(const std::uint8_t _r, const std::uint8_t _g, const std::uint8_t _b, const std::uint8_t _a = 255);
};

class Image {
public:
	void load(unsigned char* _data, const int _width, const int _height);

	auto load(const std::filesystem::path&) -> std::expected<void, std::string>;

	void clamp(const size_t max_width);

	const std::vector<Pixel>& data() const { return m_data; }

	const Pixel operator[](size_t index) const { return m_data[index]; }

	int width() const { return m_width; }
	int height() const { return m_height; }
	int size() const { return m_data.size(); }

	static bool is_row_end(const size_t index, const size_t width)
	{
		return (index > 0 && (index+1) % width == 0);
	}

private:
	std::vector<Pixel> m_data{};

	int m_width{};
	int m_height{};
};

#endif