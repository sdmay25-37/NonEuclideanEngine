#include "ui/components/DraggableCanvas.hpp"

#include <algorithm>

void DraggableCanvas::render() {
	ImGui::SetCursorPos(_position);
	ImGui::BeginChild("Canvas", _size, ImGuiChildFlags_Borders, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
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

	ImGui::PopStyleVar();
	ImGui::EndChild();
}
