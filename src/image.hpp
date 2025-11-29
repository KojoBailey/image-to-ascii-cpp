#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <cstdint>
#include <expected>
#include <filesystem>
#include <functional>
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
	~Image() { clean_up(); }

	auto load(unsigned char* _data, const int _width, const int _height) -> std::expected<void, std::string>;

	auto load(const std::filesystem::path&) -> std::expected<void, std::string>;

	void clamp(const size_t max_width);

	[[nodiscard]] const std::vector<Pixel>& data() const { return m_data; }

	[[nodiscard]] const Pixel operator[](size_t index) const { return m_data[index]; }

	[[nodiscard]] int width() const { return m_width; }
	[[nodiscard]] int height() const { return m_height; }
	[[nodiscard]] int size() const { return m_data.size(); }

	[[nodiscard]] static bool is_row_end(const size_t index, const size_t width)
	{
		return (index > 0 && (index+1) % width == 0);
	}

private:
	std::vector<Pixel> m_data{};

	int m_width{};
	int m_height{};

	std::function<void()> clean_up;
};

#endif