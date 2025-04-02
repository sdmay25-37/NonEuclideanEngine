#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP

#include <optional>
#include <unordered_map>

#include <glm/vec2.hpp>

#include "Texture.hpp"


struct AtlasedTexture {
	unsigned int atlas_id;
	glm::vec2 uv_min, uv_max;
};

class TextureManager {
public:

	Result<std::nullptr_t, std::string> loadAtlas(const char* path);
	std::optional<AtlasedTexture> getTexture(const std::string& name);

	Result<std::nullptr_t, std::string> LoadTextures(const char* path);

private:
	std::unordered_map<unsigned int, Texture> _atlases;
	std::unordered_map<std::string, AtlasedTexture> _textures;
};



#endif //TEXTUREMANAGER_HPP
