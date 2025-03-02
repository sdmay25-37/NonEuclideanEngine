
#include <glad/glad.h>

#include "Texture.hpp"

#include <stb_image.h>

Result<Texture, Texture::CreateError> Texture::create(const char *filepath) {
	using Result = Result<Texture, CreateError>;

	unsigned int textureId;
	int width, height, channels;

	unsigned char *image_data = stbi_load(filepath, &width, &height, &channels, 0);
	if (!image_data) {
		return Result::Error(CreateError::IMAGE_LOAD_FAILURE);
	}

	// Create texture from image data
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	GLint format = channels == 3 ? GL_RGB : GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image_data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(image_data);

	return Result::Ok(textureId);
}

Texture::~Texture() {
	glDeleteTextures(1, &_textureId);
}
