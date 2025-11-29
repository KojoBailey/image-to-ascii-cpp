#define STB_IMAGE_IMPLEMENTATION
#include "image.hpp"

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
	clean_up = [&] { stbi_image_free(data); };
	return {};
}

void Image::clamp(size_t max_width)
{
	if (max_width > m_width) {
		max_width = m_width;
	}
	size_t max_height = std::round(static_cast<float>(m_height) * float_div<size_t>(max_width, m_width) / 2.0f);
	const float x_step_size = float_div<size_t>(m_width, max_width);
	const float y_step_size = float_div<size_t>(m_height, max_height);

	std::vector<Pixel> compressed_data{};
	compressed_data.reserve(max_height * max_width);
	double y_step = 0;
	for (size_t row = 0; row < max_height; row++) {
		const size_t y_index = (row+1) == max_height
			? m_height-1
			: std::floor(y_step);

		double x_step = 0;
		for (size_t column = 0; column < max_width; column++) {
			const size_t x_index = (column+1) == max_width
				? m_width-1
				: std::floor(x_step);
			compressed_data.push_back(m_data[y_index * m_width + x_index]);
			x_step += x_step_size;
		}

		y_step += y_step_size;
	}

	m_data.clear();
	m_data = std::move(compressed_data);
	m_width = max_width;
	m_height = max_height;
}