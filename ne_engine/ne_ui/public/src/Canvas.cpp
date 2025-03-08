#include <algorithm>
#include <cmath>
#include <iostream>

#include "ui/components/Canvas.hpp"


[[nodiscard]] ImVec2 ScreenCamera::worldToScreen(ImVec2 world_pos) const {
	return {
		(world_pos.x - _pos.x) * _zoom,
		(world_pos.y - _pos.y) * _zoom
	};
}

[[nodiscard]] ImVec2 ScreenCamera::screenToWorld(ImVec2 screen_pos) const {
	return {
		screen_pos.x / _zoom + _pos.x,
		screen_pos.y / _zoom + _pos.y
	};
}

void ScreenCamera::pan(ImVec2 delta) {
	_pos.x -= delta.x / _zoom;
	_pos.y -= delta.y / _zoom;
}

void ScreenCamera::zoom(float delta, ImVec2 cursor_pos) {
	ImVec2 prev_pos = screenToWorld(cursor_pos);

	_zoom *= delta;

	ImVec2 new_pos = screenToWorld(cursor_pos);
	_pos.x += prev_pos.x - new_pos.x;
	_pos.y += prev_pos.y - new_pos.y;
}

void Canvas::render() {

	ImGui::Begin(GUI_REFERENCE, nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	ImVec2 window_pos = ImGui::GetWindowPos();
	ImVec2 window_size = ImGui::GetContentRegionAvail();
	ImVec2 center = ImVec2(window_pos.x + window_size.x / 2, window_pos.y + window_size.y / 2);

	ImVec2 canvas_size = ImVec2(1000, 1000);
	ImVec2 canvas_p0 = ImVec2(center.x - canvas_size.x / 2, center.y - canvas_size.y / 2);
	ImVec2 canvas_p1 = ImVec2(center.x + canvas_size.x / 2, center.y + canvas_size.y / 2);

	ImVec2 canvas_p0_screen = _camera.worldToScreen(canvas_p0);
	ImVec2 canvas_p1_screen = _camera.worldToScreen(canvas_p1);

	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRectFilled(canvas_p0_screen, canvas_p1_screen, IM_COL32(50, 50, 50, 255));


	float scroll = ImGui::GetIO().MouseWheel;
	if(ImGui::IsWindowHovered() && scroll != 0) {
		float zoom_sensitivity = 1.15f;
		_camera.zoom( powf(zoom_sensitivity, scroll), ImGui::GetMousePos());
	}

	if(ImGui::IsMouseDown(ImGuiMouseButton_Middle)) {
		ImVec2 delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Middle);
		_camera.pan(delta);
		ImGui::ResetMouseDragDelta(ImGuiMouseButton_Middle);
	}

	handleInput();

	for (auto& item : _items) {

		ImVec2 p0_screen = _camera.worldToScreen(item.p0);
		ImVec2 p1_screen = _camera.worldToScreen(item.p1);
		draw_list->AddImage(item.texture.getId(), p0_screen, p1_screen);
	}

	ImGui::End();
}

void Canvas::handleInput() {

	for(auto item = _items.rbegin(); item != _items.rend(); ++item) {
		ImVec2 p0_screen = _camera.worldToScreen(item->p0);
		ImVec2 p1_screen = _camera.worldToScreen(item->p1);

		if(!is_dragging
			&& ImGui::IsMouseHoveringRect(p0_screen, p1_screen)
			&& ImGui::IsMouseDown(ImGuiMouseButton_Left)
		) {
			item->dragging = true;
			is_dragging = true;
		}

		if(item->dragging) {
			std::cerr << "Dragging" << std::endl;
			if(ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
				std::cout << "Hi" << std::endl;

				ImVec2 mouseDelta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);

				p0_screen.x += mouseDelta.x;
				p0_screen.y += mouseDelta.y;
				p1_screen.x += mouseDelta.x;
				p1_screen.y += mouseDelta.y;

				item->p0 = _camera.screenToWorld(p0_screen);
				item->p1 = _camera.screenToWorld(p1_screen);

				ImGui::ResetMouseDragDelta();
			} else {
				item->dragging = false;
				is_dragging = false;
			}
		}
	}
}

