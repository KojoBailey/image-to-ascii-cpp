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

	size_t i = 0;
	for (const Pixel& pixel : image.data()) {
		const auto buffer = ASCII::rgb_to_symbol(pixel.r, pixel.g, pixel.b);
		if (!buffer) {
			std::cerr << "RGB out of range.\n";
			std::cout << std::format("[{:03}, {:03}, {:03}]", pixel.r, pixel.g, pixel.b);
			return -1;
		}
		std::cout << *buffer;
		
		if (Image::is_row_end(i, image.width())) {
			std::cout << '\n';
		}

		i++;
	}
}