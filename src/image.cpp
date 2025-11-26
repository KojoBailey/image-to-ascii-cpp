#define STB_IMAGE_IMPLEMENTATION
#include "image.hpp"

#include <cmath>
#include <iostream>

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
	/*const*/ size_t max_height =
		std::round(static_cast<float>(m_height) * static_cast<float>(max_width) / static_cast<float>(m_width));
	const float step_size = m_data.size() / (max_width * max_height);
	std::cout << std::format("Step size: {}\n", step_size);

	std::vector<Pixel> compressed_data{};
	// max_height = 20; //[!] remove
	double step = 0;
	for (size_t row = 0; row < max_height; row++) {
		// double step = m_width * row;
		for (size_t column = 0; column < max_width; column++) {
			const size_t index = (column == max_width)
				? max_width
				: std::floor(step);
			std::cout << std::format("{:03}:{:03} - {:03}\n", row+1, column+1, index);
			compressed_data.push_back(m_data[index]);
			step += step_size;
		}
	}

	m_data.clear();
	m_data = std::move(compressed_data);
	m_width = max_width;
	m_height = max_height;
}