#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <filesystem>
#include <vector>

#include "stb_image.h"

class Image {
public:
	void load(unsigned char* _data, const int _width, const int _height);

	void load(const std::filesystem::path&);

	void clamp(const size_t max_width);

	const std::vector<unsigned char>& data() const { return m_data; }

	const unsigned char operator[](size_t index) const { return m_data[index]; }

	int width() const { return m_width; }
	int height() const { return m_height; }

private:
	std::vector<unsigned char> m_data{};

	int m_width{};
	int m_height{};
};

#endif