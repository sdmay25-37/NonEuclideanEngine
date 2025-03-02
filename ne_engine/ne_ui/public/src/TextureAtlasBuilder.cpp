#include <filesystem>
#include <iostream>

#include "Texture.hpp"
#include "ui/components/Image.hpp"
#include "ui/tools/TextureAtlasBuilder.hpp"

#include <imgui_internal.h>


TextureAtlasBuilder::TextureAtlasBuilder(const char* texture_path) {
	try {
		for(const auto &entry : std::filesystem::directory_iterator(texture_path)) {
			if(is_regular_file(entry.status())) {
				const char *filepath = entry.path().c_str();
				std::string filename = entry.path().filename().string();

				auto texture_result = Texture::create(filepath);

				if(texture_result.is_ok()) {
					int image_x = Random::integer(0, 500);
					int image_y = Random::integer(0, 500);

					Texture texture = std::move(texture_result.ok());
					int width = texture.getWidth();
					int height = texture.getHeight();

					_canvas.addComponent(std::make_unique<Image>(filename, std::move(texture), ImVec2(image_x, image_y), ImVec2(width, height)));
				} else {
					std::cerr << "Failed to load image file: " << filepath << std::endl;
				}
			}
		}
	} catch(const std::exception &e) {
		std::cerr << "Failed to load file: " << e.what() << std::endl;
	}
}

void TextureAtlasBuilder::render() {
	ImGui::Begin("Texture Atlas Builder", nullptr);

	ImGuiID dockspace_id = ImGui::GetID("TextureAssetBuilderDockSpace");
	ImGui::DockSpace(dockspace_id);

	static bool init = true;
	if(init) {
		init = false;

		ImGui::DockBuilderRemoveNode(dockspace_id); // Clear previous layout
		ImGuiID dock_id_main = ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(dock_id_main, ImGui::GetMainViewport()->Size);

		auto dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.2f, nullptr, &dock_id_main);
		auto dock_id_down = ImGui::DockBuilderSplitNode(dock_id_main, ImGuiDir_Down, 0.25f, nullptr, &dock_id_main);

		ImGui::DockBuilderDockWindow("Canvas", dock_id_main);
		ImGui::DockBuilderDockWindow("Menu", dock_id_left);
		ImGui::DockBuilderDockWindow("Output", dock_id_down);

		ImGui::DockBuilderFinish(dockspace_id);
	}
	ImGui::End();

	ImGui::Begin("Canvas");
	ImVec2 window_size = ImGui::GetContentRegionAvail();

	ImVec2 canvasSize = window_size;
	canvasSize.x *= 0.8;
	canvasSize.y *= 0.8;

	_canvas.setSize(canvasSize);
	_canvas.setPosition(ImVec2((window_size.x - canvasSize.x) / 2, (window_size.y - canvasSize.y) / 2));
	_canvas.render();

	ImGui::End();

	ImGui::Begin("Menu");
	ImGui::End();

	ImGui::Begin("Output");
	ImGui::End();
}
