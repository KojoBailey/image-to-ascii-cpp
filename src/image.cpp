#define STB_IMAGE_IMPLEMENTATION
#include "image.hpp"

#include <algorithm>
#include <cmath>

template <typename T>
float float_div(const T a, const T b)
{
	return static_cast<float>(a) / static_cast<float>(b);
}

void Pixel::set(const std::uint8_t _r, const std::uint8_t _g, const std::uint8_t _b, const std::uint8_t _a)
{
	r = _r;
	g = _g;
	b = _b;
	a = _a;
}

auto Image::load(unsigned char* _data, const int _width, const int _height)
-> std::expected<void, std::string>
{
	if (!_data) {
		return std::unexpected{"Image data is invalid."};
	}

	m_width = _width;
	m_height = _height;

	size_t pixel_count = m_width * m_height;
	auto pixel_data = reinterpret_cast<Pixel*>(_data);
	std::vector<Pixel> buffer{pixel_data, pixel_data + pixel_count};
	m_data = std::move(buffer);

	return {};
}

auto Image::load(const std::filesystem::path& _path)
-> std::expected<void, std::string>
{
	if (!std::filesystem::exists(_path)) {
		return std::unexpected(std::format("File at \"{}\" does not exist.", _path.string()));
	}

	int width, height, channels;
	auto str_path = _path.string();
	unsigned char* data = stbi_load(str_path.c_str(), &width, &height, &channels, 4);
	load(data, width, height);

	clean_up = [&] { stbi_image_free(data); }; // Called by destructor.

	return {};
}

void Image::clamp(size_t max_width)
{
	const size_t new_width = std::min(max_width, m_width);
	if (new_width == m_width) {
		m_height /= 2;
		return;
	}
	const size_t new_height = std::round(static_cast<float>(m_height) * float_div<size_t>(new_width, m_width) / 2.0f);
	const size_t new_size = new_height * new_width;
	const float x_step_size = float_div<size_t>(m_width, new_width);
	const float y_step_size = float_div<size_t>(m_height, new_height);

	for (size_t i = 0; i < new_size; i++) {
		const size_t column = i % new_width;
		const size_t row = i / new_width;
		const size_t x_index = (column+1) == new_width
			? m_width-1
			: std::floor(column * x_step_size);
		const size_t y_index = (row+1) == new_height
			? m_height-1
			: std::floor(row * y_step_size);
		const size_t target_i = y_index * m_width + x_index;

		/* OPTIMISED: Overwrites instead of copies,
		   since clamped size is always smaller. */
		m_data[i] = m_data[target_i];
	}
	m_data.resize(new_size);

	m_width = new_width;
	m_height = new_height;
}