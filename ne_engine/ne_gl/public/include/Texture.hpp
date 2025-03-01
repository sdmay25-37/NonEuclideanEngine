#ifndef TEXTURE_H
#define TEXTURE_H



class Texture {
public:
    Texture(int width, int height, int channels, unsigned char *image_data);
	~Texture();

    unsigned int getTextureId() const { return _textureId; }

private:
	unsigned int _textureId{};
};



#endif //TEXTURE_H
