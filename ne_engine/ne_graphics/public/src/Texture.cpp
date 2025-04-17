#include <glad/glad.h>
#include <stb/stb_rect_pack.h>

#include "Texture.hpp"
#include <iostream>

Result<Texture, Texture::CreateError> Texture::create(const std::string &filepath)
{
	using Result = Result<Texture, CreateError>;

	// Load image
	auto image_result = Image::create(filepath);
	if (image_result.is_error())
	{
		return Result::Error(CreateError::IMAGE_LOAD_FAILURE);
	}

	const Image image = image_result.ok();

	// Create texture from image
	return createFromImage(image);
}

Result<Texture, Texture::CreateError> Texture::createFromImage(const Image &image)
{
	using Result = Result<Texture, CreateError>;

	unsigned int textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	stbi_set_flip_vertically_on_load(true);
	int texWidth, texHeight, texChannels;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *data = stbi_load("../res/textures/cy.jpg", &texWidth, &texHeight, &texChannels, 0);
	if (data)
	{
		GLenum format = (texChannels == 4) ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_2D, 0, format, texWidth, texHeight, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cerr << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	std::cout << "Loaded texture: " << image.width() << " x " << image.height() << "\n";
	return Result::Ok(Texture(textureId, image.width(), image.height()));
}

Texture::~Texture()
{
	if (_textureId != 0)
	{
		glDeleteTextures(1, &_textureId);
	}
}
