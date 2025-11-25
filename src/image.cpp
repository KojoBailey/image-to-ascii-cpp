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
	const size_t max_height = std::round(static_cast<float>(m_height) / step_size);

	std::vector<unsigned char> compressed_data{};
	double step = 0;
	for (size_t row = 0; row < 5; row++) {
		for (size_t j = 0; j < max_width; j++) {
			const size_t index = (j == max_width)
				? max_width
				: std::floor(step);
			const size_t start = index * row * 3;
			compressed_data.push_back(m_data[start]);	// R
			compressed_data.push_back(m_data[start+1]);	// G
			compressed_data.push_back(m_data[start+2]);	// B
			step += step_size;
		}
		step = max_width * row * 3;
	}

	m_data.clear();
	m_data = std::move(compressed_data);
	m_width = max_width;
	m_height = max_height;
}