#include <glad/glad.h>
#include <stb/stb_rect_pack.h>

#include "Texture.hpp"


Result<Texture, Texture::CreateError> Texture::create(const std::string& filepath) {
	using Result = Result<Texture, CreateError>;

	// Load image
	auto image_result = Image::create(filepath);
	if(image_result.is_error()) {
		return Result::Error(CreateError::IMAGE_LOAD_FAILURE);
	}

	const Image image = image_result.ok();

	// Create texture from image
	return createFromImage(image);
}

Result<Texture, Texture::CreateError> Texture::createFromImage(const Image& image) {
	using Result = Result<Texture, CreateError>;

	unsigned int textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());

	return Result::Ok(Texture(textureId, image.width(), image.height()));
}

Texture::~Texture() {
	if(_textureId != 0) {
		glDeleteTextures(1, &_textureId);
	}
}
