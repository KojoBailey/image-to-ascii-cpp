#define STB_IMAGE_IMPLEMENTATION
#include "image.hpp"

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