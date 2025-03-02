#include "DraggableCanvas.hpp"

#include <algorithm>
#include <iostream>

void DraggableCanvas::render() {
	ImGui::SetNextWindowPos(_position, ImGuiCond_Appearing);
	ImGui::SetNextWindowSize(_size, ImGuiCond_Appearing);
	ImGui::Begin("Canvas", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	for (const auto& component : _components) {
		ImGui::SetNextItemAllowOverlap();
		component->render();
		if(ImGui::IsItemActive() && ImGui::IsMouseDragging(0)) {
			ImVec2 mouseDelta = ImGui::GetMouseDragDelta(0);
			ImVec2 position = component->getPosition();
			position.x += mouseDelta.x;
			position.y += mouseDelta.y;

			ImVec2 component_size = component->getSize();

			if(_size.x - component_size.x < 0) {
				position.x += (_size.x - component_size.x);
			}

			if((_size.y - component_size.y) < 0) {
				position.y += (_size.y - component_size.y);
			}

			position.x = std::clamp(position.x, 0.0f, _size.x - component_size.x);
			position.y = std::clamp(position.y, 0.0f, _size.y - component_size.y);

			component->setPosition(position);
			ImGui::ResetMouseDragDelta();
		}
	}

	ImGui::End();
}
