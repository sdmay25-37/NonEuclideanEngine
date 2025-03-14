#include <filesystem>
#include <fstream>
#include <iostream>

#include <IconsFontAwesome5.h>
#include <imgui_internal.h>
#include <nlohmann/json.hpp>

#include "Texture.hpp"
#include "ui/components/Image.hpp"
#include "ui/tools/TextureAtlasBuilder.hpp"


void TextureAtlasBuilder::render() {
	ImGuiID dockspace_id = ImGui::GetID("TextureAssetBuilderDockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0, 0), ImGuiDockNodeFlags_NoWindowMenuButton);

	static bool init = true;
	if(init) {
		init = false;

		ImGui::DockBuilderRemoveNode(dockspace_id); // Clear previous layout
		ImGuiID dock_id_main = ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(dock_id_main, ImGui::GetMainViewport()->Size);

		auto dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.2f, nullptr, &dock_id_main);
		auto dock_id_down = ImGui::DockBuilderSplitNode(dock_id_main, ImGuiDir_Down, 0.25f, nullptr, &dock_id_main);

		ImGui::DockBuilderDockWindow("###Canvas", dock_id_main);
		ImGui::DockBuilderDockWindow("###Menu", dock_id_left);
		ImGui::DockBuilderDockWindow("###Output", dock_id_down);

		ImGui::DockBuilderFinish(dockspace_id);

	}

	if(_canvas != nullptr) {
		_canvas->render();
	}

	ImGui::Begin(ICON_FA_SLIDERS_H " Menu###Menu");

	ImGui::SeparatorText("Atlas");
	_atlasFileSelector.render();

	ImGui::SameLine();
	if(ImGui::Button("Save")) {
		saveAtlas(_atlasFileSelector.getBuffer(), "../res/textures/atlas.png");
		_atlasFileSelector.clear();
	}

	ImGui::SeparatorText("Textures");

	ImGui::TextUnformatted("Textures folder: ");
	_textureFolderSelector.render();

	ImGui::SameLine();
	if(ImGui::Button("Import")) {
		loadTextures(_textureFolderSelector.getBuffer());
		_textureFolderSelector.clear();
	}

	ImGui::End();

	ImGui::Begin(ICON_FA_TERMINAL " Output###Output", nullptr, ImGuiWindowFlags_NoTitleBar);
	ImGui::End();
}

// Todo: use result type
void TextureAtlasBuilder::loadTextures(const char* path) {
	if(_canvas == nullptr) return;

	try {
		for(const auto &entry : std::filesystem::directory_iterator(path)) {
			if(is_regular_file(entry.status())) {
				std::string filepath = entry.path().string();
				std::string filename = entry.path().filename().string();

				auto image_result = Image::create(filepath);

				if(image_result.is_ok()) {
					_canvas->addItem(std::move(image_result.ok()));
				} else {
					std::cerr << "Failed to load image file: " << filepath << std::endl;
				}
			}
		}
	} catch(const std::exception &e) {
		std::cerr << "Failed to load file: " << e.what() << std::endl;
	}
}

// Todo: use result type
void TextureAtlasBuilder::saveAtlas(const char* json_path, const char* texture_path) {
	nlohmann::json json_data;

	json_data["atlas"] = texture_path;
	json_data["textures"] = nlohmann::json::array();

	const std::vector<TextureUVs> items = _canvas->getItemUVs();
	for(auto& item : items) {
		json_data["textures"].push_back({
			item.filepath,
			{item.uv_min.x, item.uv_min.y},
			{item.uv_max.x, item.uv_max.y}
		});
	}


	// Write JSON to file
	std::ofstream json_file(json_path);
	if(json_file.is_open()) {
		json_file << json_data.dump();
		json_file.close();
	}
}
