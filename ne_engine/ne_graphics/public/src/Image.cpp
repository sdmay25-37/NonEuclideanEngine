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

	try {
		auto image_data = new unsigned char[static_cast<size_t>(width) *
										   static_cast<size_t>(height) *
										   static_cast<size_t>(channels)];
		// ...
	} catch (const std::bad_alloc&) {
		std::cerr << "Allocation of 4GB atlas failed!" << std::endl;
		return Result::Error(nullptr);
	}

	return Result::Ok(Image("", image_data, width, height, channels));
}

void Image::CopySubImage(const Image& other, std::size_t sub_x, std::size_t sub_y) {
	// Copy image line by line
	for(std::size_t other_y = 0; other_y < other.height(); other_y++) {

		// Calculate starting destination
		unsigned char* image_row = _data + ((sub_y + other_y) * _width + sub_x) * _channels;
		const unsigned char* other_row = other.data() + (other_y * other.width() * _channels);

		// Copy row
		memcpy(image_row, other_row, other.width() * _channels);
	}
}
