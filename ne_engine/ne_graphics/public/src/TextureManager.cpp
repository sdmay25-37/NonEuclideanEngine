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

unsigned int QueryAtlasSize(std::vector<Image>& images) {
	// Set atlas size large enough to contain biggest texture
	unsigned int atlas_size = 0;
	for(auto& image : images) {
		if(image.width() > atlas_size) atlas_size = image.width();
		if(image.height() > atlas_size) atlas_size = image.height();
	}

	// Make atlas size a power of two for efficiency
	atlas_size = NextPowerOfTwo(atlas_size);

	int max_texture_size;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size);
	if(atlas_size > max_texture_size) std::cerr << "TEXTURE SIZE EXCEEDS MAXIMUM" << std::endl;

	return atlas_size;
}

void RemoveImage(std::vector<Image>& images, std::vector<stbrp_rect>& rects, const std::size_t index) {
	images[index] = std::move(images.back());
	rects[index] = std::move(rects.back());

	images.pop_back();
	rects.pop_back();
}

// TODO: actual error handling
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

	stbrp_context packing_context;
	std::vector<stbrp_rect> image_rects(images.size());
	std::vector<stbrp_node> rect_node_storage(images.size());

	// Initialize stb rects with image sizes
	for(int i = 0; i < images.size(); i++) {
		stbrp_rect& rect = image_rects[i];

		rect.id = i;
		rect.w = images[i].width();
		rect.h = images[i].height();
	}

	while(!images.empty()) {
		unsigned int atlas_size = QueryAtlasSize(images);

		auto atlas_result = Image::create_empty(atlas_size, atlas_size);
		Image atlas = atlas_result.ok();

		// Setup packing context with temporary node storage
		stbrp_init_target(
			&packing_context,
			atlas_size,
			atlas_size,
			rect_node_storage.data(),
			rect_node_storage.size()
		);

		// Pack
		stbrp_pack_rects(&packing_context, image_rects.data(), image_rects.size());

		// Copy images to atlas
		for(int i = 0; i < images.size(); i++) {
			if(!image_rects[i].was_packed) continue; // Only copy if successfully packed
			atlas.CopySubImage(images[i], image_rects[i].x, image_rects[i].y);
		}

		// Create OpenGL texture for atlas
		auto atlas_texture_result = Texture::createFromImage(atlas);
		Texture atlas_texture = atlas_texture_result.ok();

		// Add texture to manager
		unsigned int atlas_id = atlas_texture.getId();
		_atlases.emplace(atlas_id, std::move(atlas_texture));

		// Insert atlased textures into manager
		for(int i = 0; i < images.size(); i++) {
			if(!image_rects[i].was_packed) continue; // Only insert if successfully packed

			std::filesystem::path img_path(images[i].filepath());

			// Image pixel positions -> atlas UVs
			stbrp_rect& rect = image_rects[i];
			float min_x = (float)rect.x / atlas_size;
			float min_y = (float)rect.y / atlas_size;
			float max_x = (float)(rect.x + rect.w) / atlas_size;
			float max_y = (float)(rect.y + rect.h) / atlas_size;

			_textures.emplace(
				img_path.filename().string(),
				AtlasedTexture {
					atlas_id,
					{ min_x, min_y },
					{ max_x, max_y }
				}
			);
		}

		// Remove packed images from list
		for(int i = 0; i < images.size(); ) {
			if(image_rects[i].was_packed) {
				RemoveImage(images, image_rects, i);
			} else {
				i++;
			}
		}
	}

	return Result::Ok(nullptr);
}

