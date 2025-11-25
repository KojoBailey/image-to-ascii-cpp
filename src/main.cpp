#include "image.hpp"
#include "ascii.hpp"

#include <iostream>
#include <format>

int main(int argc, char* argv[])
{
	if (argc < 1) {
		std::cerr << "Missing input file.\n";
		return -1;
	}
	std::filesystem::path image_path = argv[1];

	const size_t max_width = 80; // [!] Could also offer 100, 120, and 140.

	std::cout << std::format("Converting image \"{}\"...\n", image_path.string());

	Image image;
	image.load(image_path);
	std::cout << std::format("Width: {}, Height: {}\n", image.width(), image.height());
	image.clamp(80);

	// auto buf_image_data = image.load(image_path);
	// if (!buf_image_data) {
	// 	std::cerr << "Failed to load image.\n";
	// 	return buf_image_data.error();
	// }
	// std::vector<unsigned char> image_data = *buf_image_data;

	for (size_t i = 0; i < 30; i++) {
		std::cout << ASCII::rgb_to_symbol(image[i], image[++i], image[++i]);
	}
}