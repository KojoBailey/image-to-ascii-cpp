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

	std::cout << "Enter output width (recommended max 140) or nothing for default (80):\n";
	std::string buf_input;
	std::getline(std::cin, buf_input);
	const size_t max_width = buf_input.empty() ? 80 : std::stoul(buf_input);

	std::cout << std::format("Converting image \"{}\"...\n", image_path.string());

	Image image;
	image.load(image_path);
	auto image_check = image.load(image_path);
	if (!image_check) {
		std::cerr << image_check.error();;
		return -1;
	}
	std::cout << std::format("Size: {}, Width: {}, Height: {}\n", image.size(), image.width(), image.height());

	image.clamp(max_width);
	std::cout << std::format("Compressed Size {}, Width: {}, Height: {}\n", image.size(), image.width(), image.height());

	std::cout << "Symbols -> {";
	for (char symbol : ASCII::symbols) {
		std::cout << symbol;
	}
	std::cout << "}\n";

	std::cout << std::format("Expected {} | Actual: {}\n", image.height() * image.width() * 3, image.data().size());

	size_t i = 0;
	std::string output{};
	for (const Pixel& pixel : image.data()) {
		const auto buffer = ASCII::rgb_to_symbol(pixel.r, pixel.g, pixel.b, pixel.a);
		if (!buffer) {
			std::cerr << "RGB out of range.\n";
			std::cout << std::format("[{:03}, {:03}, {:03}, {:03}]", pixel.r, pixel.g, pixel.b, pixel.a);
			return -1;
		}
		output += *buffer;
		
		if (Image::is_row_end(i, image.width())) {
			output += '\n';
		}

		i++;
	}
	std::cout << output;
}