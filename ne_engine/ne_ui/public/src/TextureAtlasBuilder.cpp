#include <filesystem>
#include <iostream>

#include <IconsFontAwesome5.h>
#include <imgui_internal.h>

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
		//loadTextures(_textureFolderSelector.getBuffer());
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

void TextureAtlasBuilder::loadTextures(const char* path) {
	if(_canvas == nullptr) return;

	try {
		for(const auto &entry : std::filesystem::directory_iterator(path)) {
			if(is_regular_file(entry.status())) {
				std::string filepath = entry.path().string();
				std::string filename = entry.path().filename().string();

				auto texture_result = Texture::create(filepath);

				if(texture_result.is_ok()) {

					_canvas->addItem(std::move(texture_result.ok()));
				} else {
					std::cerr << "Failed to load image file: " << filepath << std::endl;
				}
			}
		}
	} catch(const std::exception &e) {
		std::cerr << "Failed to load file: " << e.what() << std::endl;
	}
}
