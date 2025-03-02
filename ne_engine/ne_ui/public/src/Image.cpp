
#include "Image.hpp"

#include <iostream>

void Image::render() {
	DraggableComponent::render();
	ImGui::ImageButton(_id.c_str(), (ImTextureID)(intptr_t)_texture.getId(), ImVec2(_texture.getWidth(), _texture.getHeight()));
}
