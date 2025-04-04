#include <fstream>
#include <iostream>
#include <sstream>

#include <nlohmann/json.hpp>
#include <stb/stb_rect_pack.h>

#include "TextureManager.hpp"

#include <glad/glad.h>


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

Result<std::nullptr_t, std::string> TextureManager::LoadTextures(const char *path) {
	using Result = Result<std::nullptr_t, std::string>;

	std::vector<Image> images;

	for(const auto& entry : std::filesystem::directory_iterator(path)) {
		if(!is_regular_file(entry)) continue;

		auto image_result = Image::create(entry.path().string());
		if(image_result.is_error()) {
			std::stringstream err;
			err << "TextureManager::LoadTextures Error\n";
			err << "\t-> Failed to load texture at path [" << path << "]\n";
			err << "\t-> Image::Create Error";
			return Result::Error(err.str());
		}

		Image image = image_result.ok();
		images.emplace_back(std::move(image));
	}

	// TODO: query max texture size with OpenGL
	int atlas_size;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &atlas_size);

	std::cout << "Atlas size: " << atlas_size << std::endl;

	stbrp_context packing_context;
	std::vector<stbrp_rect> image_rects(images.size());
	std::vector<stbrp_node> rect_node_storage(images.size());

	// Setup packing context with temporary node storage
	stbrp_init_target(
		&packing_context,
		atlas_size,
		atlas_size,
		rect_node_storage.data(),
		rect_node_storage.size()
	);

	// Initialize stb rects with image sizes
	for(int i = 0; i < images.size(); i++) {
		stbrp_rect& rect = image_rects[i];

		rect.id = i;
		rect.w = images[i].width();
		rect.h = images[i].height();
	}

	// Pack
	stbrp_pack_rects(&packing_context, image_rects.data(), image_rects.size());

	// Temp: Print packed rects
	for(int i = 0; i < images.size(); i++) {
		std::cout << images[i].filepath() << "\n"
			<< "\tpacked?: " << ((image_rects[i].was_packed) ? "yes" : "no") << "\n"
			<< "\tsize: (" << image_rects[i].w << ", " << image_rects[i].h << ")\n"
			<< "\tpos: (" << image_rects[i].x << ", " << image_rects[i].y << ")\n";
	}

	auto atlas_result = Image::create_empty(images.back().width(), images.back().height());
	Image atlas = atlas_result.ok();

	atlas.CopySubImage(images.back(), 0, 0);

	auto texture_result = Texture::createFromImage(atlas);
	Texture atlas_texture = texture_result.ok();

	unsigned int atlas_id = atlas_texture.getId();

	_atlases.emplace(atlas_id, std::move(atlas_texture));
	_textures.emplace(
		std::string("test"),
		AtlasedTexture {
			atlas_id,
			{0.0, 0.0},
			{1.0, 1.0}
		}
	);

	return Result::Ok(nullptr);
}

