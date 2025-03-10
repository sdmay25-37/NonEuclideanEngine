#include "Image.hpp"


Result<Image, Image::CreateError> Image::create(const std::string& filepath, const int desired_channels) {
	using Result = Result<Image, CreateError>;

	int width, height, channels;

	unsigned char *image_data = stbi_load(filepath.c_str(), &width, &height, &channels, desired_channels);
	if(image_data == nullptr) {
		return Result::Error(CreateError::IMAGE_LOAD_FAILURE);
	}

	return Result::Ok(Image(image_data, width, height, desired_channels));
}