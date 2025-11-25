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
	std::cout << std::format("Size: {}, Width: {}, Height: {}\n", image.size(), image.width(), image.height());
	image.clamp(80);
	std::cout << std::format("Compressed Size {}, Width: {}, Height: {}\n", image.size(), image.width(), image.height());

	// auto buf_image_data = image.load(image_path);
	// if (!buf_image_data) {
	// 	std::cerr << "Failed to load image.\n";
	// 	return buf_image_data.error();
	// }
	// std::vector<unsigned char> image_data = *buf_image_data;

	std::cout << std::format("Expected {} | Actual: {}\n", image.height() * image.width() * 3, image.data().size());
	for (size_t row = 0; row < image.height(); row++) {
		for (size_t i = 0; i < image.width(); i++) {
			const size_t start = i * 3 * row;
			const unsigned char r = image[start];
			const unsigned char g = image[start+1];
			const unsigned char b = image[start+2];
			const auto buffer = ASCII::rgb_to_symbol(r, g, b);
			if (!buffer) {
				std::cerr << "RGB out of range.\n";
				std::cout << std::format("[{:03}, {:03}, {:03}]", r, g, b);
				return -1;
			}
			std::cout << *buffer;
		}
		std::cout << '\n';
	}
}