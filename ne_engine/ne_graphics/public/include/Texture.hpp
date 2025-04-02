#ifndef TEXTURE_H
#define TEXTURE_H

#include "Image.hpp"
#include "utils.hpp"


class Texture {
public:

	// Class factory methods
    enum class CreateError { IMAGE_LOAD_FAILURE };
   	[[nodiscard]] static Result<Texture, CreateError> create(const std::string& filepath);
	[[nodiscard]] static Result<Texture, CreateError> createFromImage(const Image& image);

    // Destructor to free GPU texture memory
	~Texture();

	// Delete copy constructor and copy assignment
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	// Move constructor
	Texture(Texture&& other) noexcept
		: _textureId(other._textureId), _width(other._width), _height(other._height) {
		other._textureId = 0;  // Prevent the other destructor from freeing the texture
	}

	// Move assignment operator
	Texture& operator=(Texture&& other) noexcept {
		if (this != &other) {
			_textureId = other._textureId;
			_width = other._width;
			_height = other._height;

			other._textureId = 0;
		}
		return *this;
	}

    [[nodiscard]] unsigned int getId() const { return _textureId; }
	[[nodiscard]] int getWidth() const { return _width; }
	[[nodiscard]] int getHeight() const { return _height; }

private:
	unsigned int _textureId;
	int _width, _height;

     Texture(unsigned int textureId, int width, int height)
		: _textureId(textureId), _width(width), _height(height) {};
};

#endif //TEXTURE_H
