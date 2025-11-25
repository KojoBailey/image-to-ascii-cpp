#include <iostream>
#include <array>
#include <fstream>
#include <filesystem>
#include <expected>
#include <vector>
#include <format>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

struct Image {
public:
	void load(unsigned char* _data, const int _width, const int _height)
	{
		m_width = _width;
		m_height = _height;
		std::vector<unsigned char> buffer{_data, _data + m_width * m_height * 3};
		m_data = std::move(buffer);
	}

	void load(const std::filesystem::path& _path)
	{
		int width, height, channels;
		std::string str_path = _path.string();
		unsigned char* data = stbi_load(str_path.c_str(), &width, &height, &channels, 3);
		// if (!data) {
		// 	return std::unexpected{-1};
		// }
		load(data, width, height);
		stbi_image_free(data);
	}

	const std::vector<unsigned char>& data() const { return m_data; }

	const unsigned char operator[](size_t index) const { return m_data[index]; }

	int width() const { return m_width; }
	int height() const { return m_height; }

private:
	std::vector<unsigned char> m_data{};

	int m_width{};
	int m_height{};
};

constexpr int rgb_to_symbol(const int r, const int g, const int b)
{
	const size_t max = 255 * 3;
	return ((max - (r + g + b)) / max) * 10;
}

int main(int argc, char* argv[])
{
	if (argc < 1) {
		std::cerr << "Missing input file.\n";
		return -1;
	}
	std::filesystem::path image_path = argv[1];

	std::array<char, 10> symbols = {'.', ':', '-', '=', '+', '*', '/', '%', '#', '@'};

	std::cout << std::format("Converting image \"{}\"...\n", image_path.string());

	Image image;
	image.load(image_path);
	// auto buf_image_data = image.load(image_path);
	// if (!buf_image_data) {
	// 	std::cerr << "Failed to load image.\n";
	// 	return buf_image_data.error();
	// }
	// std::vector<unsigned char> image_data = *buf_image_data;

	for (size_t i = 0; i < 30; i++) {
		std::cout << symbols[rgb_to_symbol(image[i], image[++i], image[++i])];
	}
}