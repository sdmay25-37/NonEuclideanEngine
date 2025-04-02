#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <memory>
#include <string>
#include <utility>

#include <stb/stb_image.h>

#include "utils.hpp"


struct Image {
	std::string _filepath;
    unsigned char* _data;
	int _width, _height, _channels;

	Image() = delete;
	~Image() { stbi_image_free(_data); }

	// Delete copy constructor and copy assignment
	Image(const Image&) = delete;
	Image& operator=(const Image&) = delete;

	// Move constructor
	Image(Image&& other) noexcept
		: _filepath(std::move(other._filepath)), _data(other._data), _width(other._width), _height(other._height), _channels(other._channels) {
		other._data = nullptr;  // Prevent the other destructor from freeing the data
	}

	// Move assignment operator
	Image& operator=(Image&& other) noexcept {
		if (this != &other) {
			_filepath = std::move(other._filepath);
			_data = other._data;
			_width = other._width;
			_height = other._height;
			_channels = other._channels;

			other._data = nullptr;
		}
		return *this;
	}

	[[nodiscard]] const std::string& filepath() const { return _filepath; }
	[[nodiscard]] int width() const { return _width; }
	[[nodiscard]] int height() const { return _height; }
	[[nodiscard]] int channels() const { return _channels; }
	[[nodiscard]] const unsigned char* data() const { return _data; }

	enum class CreateError { IMAGE_LOAD_FAILURE };
	static Result<Image, CreateError> create(const std::string& filepath, int desired_channels = 4);

private:
	Image(std::string filepath, unsigned char* data, const int width, const int height, const int channels)
		: _filepath(std::move(filepath)), _data(data), _width(width), _height(height), _channels(channels) {};
};

#endif //IMAGE_HPP
