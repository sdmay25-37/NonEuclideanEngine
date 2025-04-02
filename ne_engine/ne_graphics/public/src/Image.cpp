#include "Image.hpp"


Result<Image, Image::CreateError> Image::create(const std::string& filepath, const int desired_channels) {
	using Result = Result<Image, CreateError>;

	int width, height, channels;

	unsigned char* image_data = stbi_load(filepath.c_str(), &width, &height, &channels, desired_channels);
	if(image_data == nullptr) {
		return Result::Error(CreateError::IMAGE_LOAD_FAILURE);
	}

	return Result::Ok(Image(filepath, image_data, width, height, desired_channels));
}

Result<Image, std::nullptr_t> Image::create_empty(int width, int height, int channels) {
	using Result = Result<Image, std::nullptr_t>;

	auto image_data = new unsigned char[width * height * channels];

	return Result::Ok(Image("", image_data, width, height, channels));
}
