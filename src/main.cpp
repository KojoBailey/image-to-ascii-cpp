#include "image.hpp"

#include <iostream>
#include <array>
#include <expected>
#include <format>

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