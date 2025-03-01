#ifndef TEXTURE_H
#define TEXTURE_H

#include "utils.hpp"

class Texture {
public:

    // Constructor to load texture from file
    enum class CreateError { IMAGE_LOAD_FAILURE };
   	static Result<Texture, CreateError> create(const char *filepath);

    // Destructor to free GPU texture memory
	~Texture();

    unsigned int getTextureId() const { return _textureId; }

private:
	unsigned int _textureId{};

     Texture(unsigned int textureId) : _textureId(textureId) {};
};



#endif //TEXTURE_H
