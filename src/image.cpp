#define STB_IMAGE_IMPLEMENTATION
#include "image.hpp"

#include <cmath>

void Image::load(unsigned char* _data, const int _width, const int _height)
{
	m_width = _width;
	m_height = _height;
	std::vector<unsigned char> buffer{_data, _data + m_width * m_height * 3};
	m_data = std::move(buffer);
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
	const float step_size = static_cast<float>(m_width) / static_cast<float>(max_width);

	std::vector<unsigned char> compressed_data{};
	double step = 0;
	// Prints first row only
	for (size_t i = 0; i < max_width; i++) {
		const size_t index = (i == max_width)
			? max_width
			: std::floor(step);
		compressed_data.push_back(m_data[index * 3]);		// R
		compressed_data.push_back(m_data[index * 3 + 1]);	// G
		compressed_data.push_back(m_data[index * 3 + 2]);	// B
		step += step_size;
	}

	m_data.clear();
	m_data = std::move(compressed_data);
	m_height = std::round(static_cast<float>(m_height) / step_size);
	m_width = max_width;
}