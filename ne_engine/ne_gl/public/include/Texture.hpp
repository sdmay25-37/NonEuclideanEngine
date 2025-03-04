#ifndef TEXTURE_H
#define TEXTURE_H

#include <imgui.h>
#include <stb_image.h>

#include "utils.hpp"
#include "Resource.hpp"

class Texture : public Resource {
public:

    // Constructor to load texture from file
    enum class CreateError { IMAGE_LOAD_FAILURE };
   	static Result<Texture, CreateError> create(const std::string& filepath);

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
