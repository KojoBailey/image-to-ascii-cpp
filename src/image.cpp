#define STB_IMAGE_IMPLEMENTATION
#include "image.hpp"

#include <cmath>
#include <iostream>

float size_t_div(const size_t a, const size_t b)
{
	return static_cast<float>(a) / static_cast<float>(b);
}

void Pixel::set(const std::uint8_t _r, const std::uint8_t _g, const std::uint8_t _b)
{
	r = _r;
	g = _g;
	b = _b;
}

void Image::load(unsigned char* _data, const int _width, const int _height)
{
	m_width = _width;
	m_height = _height;

	std::vector<unsigned char> buffer{_data, _data + m_width * m_height * 3};

	m_data.clear();
	m_data.resize(buffer.size() / 3);
	size_t i = 0;
	for (Pixel& pixel : m_data) {
		pixel.set(buffer[i], buffer[i+1], buffer[i+2]);
		i += 3;
	}
}

void Image::load(const std::filesystem::path& _path)
{
	int width, height, channels;
	auto str_path = _path.string();
	unsigned char* data = stbi_load(str_path.c_str(), &width, &height, &channels, 3);
	// if (!data) {
	// 	return std::unexpected{-1};
	// }
	load(data, width, height);
	stbi_image_free(data);
}

void Image::clamp(size_t max_width)
{
	if (max_width > m_width) {
		max_width = m_width;
	}
	size_t max_height = std::round(static_cast<float>(m_height) * size_t_div(max_width, m_width)) / 2.0f;
	const float x_step_size = size_t_div(m_width, max_width);
	const float y_step_size = size_t_div(m_height, max_height);
	std::cout << std::format("Row step size: {}, Column step size: {}\n", x_step_size, y_step_size);

	std::vector<Pixel> compressed_data{};
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
			// std::cout << std::format("{:03}:{:03} -> {:03}:{:03}\n", y_index, x_index, row, column);
			compressed_data.push_back(m_data[x_index * y_index]);
			x_step += x_step_size;
		}

		y_step += y_step_size;
	}

	m_data.clear();
	m_data = std::move(compressed_data);
	m_width = max_width;
	m_height = max_height;
}