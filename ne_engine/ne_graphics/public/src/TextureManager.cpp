#include <fstream>
#include <sstream>

#include <nlohmann/json.hpp>

#include "TextureManager.hpp"

#include <iostream>


Result<std::nullptr_t, std::string> TextureManager::loadAtlas(const char* path) {
	using Result = Result<std::nullptr_t, std::string>;

	std::ifstream json_file(path);
	nlohmann::json json_data = nlohmann::json::parse(json_file);

	if(json_data.empty()) {
		std::stringstream err;
		err << "JSON file [" << path << "] empty";
		return Result::Error(err.str());
	}

	auto atlas = json_data.at("atlas").get<std::string>();

	auto texture_result = Texture::create(atlas);
	if(texture_result.is_error()) {
		std::stringstream err;
		err << "Failed to load texture [" << atlas << "]";
		return Result::Error(err.str());
	}

	Texture atlas_texture = texture_result.ok();
	unsigned int atlas_id = atlas_texture.getId();
	_atlases.emplace(atlas_id, std::move(atlas_texture));

	auto texture_jsons = json_data.at("textures");
	for (auto& texture_json: texture_jsons) {
		auto name = texture_json[0].get<std::string>();

		auto uv_min_x = texture_json[1][0].get<float>();
		auto uv_min_y = texture_json[1][1].get<float>();
		glm::vec2 uv_min(uv_min_x, uv_min_y);

		auto uv_max_x = texture_json[2][0].get<float>();
		auto uv_max_y = texture_json[2][1].get<float>();
		glm::vec2 uv_max(uv_max_x, uv_max_y);

		_textures.emplace(name, AtlasedTexture{atlas_id, uv_min, uv_max});
	}

	// Return empty value
	return Result::Ok(nullptr);
}

std::optional<AtlasedTexture> TextureManager::getTexture(const std::string& name) {
	if(const auto it = _textures.find(name); it != _textures.end()) {
		return it->second;
	}
	return std::nullopt;
}