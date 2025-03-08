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

	_p0 = ImVec2(center.x - _size.x / 2, center.y - _size.y / 2);
	_p1 = ImVec2(center.x + _size.x / 2, center.y + _size.y / 2);

	ImVec2 canvas_p0_screen = _camera.worldToScreen(_p0);
	ImVec2 canvas_p1_screen = _camera.worldToScreen(_p1);

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

	for (auto& item : _items) {

		ImVec2 p0_screen = _camera.worldToScreen(item.p0);
		ImVec2 p1_screen = _camera.worldToScreen(item.p1);
		draw_list->AddImage(item.texture.getId(), p0_screen, p1_screen);
	}

	handleInput(draw_list);

	ImGui::End();
}

void Canvas::handleInput(ImDrawList* draw_list) {

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
			if(ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
				ImVec2 mouseDelta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);

				p0_screen.x += mouseDelta.x;
				p0_screen.y += mouseDelta.y;
				p1_screen.x += mouseDelta.x;
				p1_screen.y += mouseDelta.y;

				snapToItems(draw_list, p0_screen, p1_screen);

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

void snapInside(ImDrawList* draw_list, ImVec2& p0, ImVec2& p1, const ImVec2& other_p0, const ImVec2& other_p1, float snapping) {
	// Left side
	float diff = p0.x - other_p0.x;
	if(std::abs(diff) < snapping) {
		p0.x -= diff;
		p1.x -= diff;

		draw_list->AddLine(p0, {p0.x, p1.y}, IM_COL32(255, 0, 0, 255), 3.0f);
	}

	// Right side
	diff = p1.x - other_p1.x;
	if(std::abs(diff) < snapping) {
		p0.x -= diff;
		p1.x -= diff;

		draw_list->AddLine({p1.x, p0.y}, p1, IM_COL32(255, 0, 0, 255), 3.0f);
	}

	// Top side
	diff = p0.y - other_p0.y;
	if(std::abs(diff) < snapping) {
		p0.y -= diff;
		p1.y -= diff;

		draw_list->AddLine(p0, {p1.x, p0.y}, IM_COL32(255, 0, 0, 255), 3.0f);
	}

	// Bottom side
	diff = p1.y - other_p1.y;
	if(std::abs(diff) < snapping) {
		p0.y -= diff;
		p1.y -= diff;

		draw_list->AddLine({p0.x, p1.y}, p1, IM_COL32(255, 0, 0, 255), 3.0f);
	}
}

void snapOutside(ImDrawList* draw_list, ImVec2& p0, ImVec2& p1, const ImVec2& other_p0, const ImVec2& other_p1, float snapping) {
	// Left side
	float diff = p0.x - other_p1.x;
	if(std::abs(diff) < snapping) {
		p0.x -= diff;
		p1.x -= diff;

		const ImVec2 start = { p0.x, std::min(p0.y, other_p0.y) };
		const ImVec2 end   = { p0.x, std::max(p1.y, other_p1.y) };
		draw_list->AddLine(start, end, IM_COL32(255, 0, 0, 255), 3.0f);
	}

	// Right side
	diff = p1.x - other_p0.x;
	if(std::abs(diff) < snapping) {
		p0.x -= diff;
		p1.x -= diff;

		const ImVec2 start = { p1.x, std::min(p0.y, other_p0.y) };
		const ImVec2 end   = { p1.x, std::max(p1.y, other_p1.y) };
		draw_list->AddLine(start, end, IM_COL32(255, 0, 0, 255), 3.0f);
	}

	// Top side
	diff = p0.y - other_p1.y;
	if(std::abs(diff) < snapping) {
		p0.y -= diff;
		p1.y -= diff;

		const ImVec2 start = { std::min(p0.x, other_p0.x), p0.y };
		const ImVec2 end   = { std::max(p1.x, other_p1.x), p0.y };
		draw_list->AddLine(start, end, IM_COL32(255, 0, 0, 255), 3.0f);
	}

	// Bottom side
	diff = p1.y - other_p0.y;
	if(std::abs(diff) < snapping) {
		p0.y -= diff;
		p1.y -= diff;

		const ImVec2 start = { std::min(p0.x, other_p0.x), p1.y };
		const ImVec2 end   = { std::max(p1.x, other_p1.x), p1.y };
		draw_list->AddLine(start, end, IM_COL32(255, 0, 0, 255), 3.0f);
	}
}

void Canvas::snapToItems(ImDrawList* draw_list, ImVec2& p0_screen, ImVec2& p1_screen) {
	float snapping = 5.0f;

	// Snap to canvas
	ImVec2 canvas_p0 = _camera.worldToScreen(_p0);
	ImVec2 canvas_p1 = _camera.worldToScreen(_p1);

	snapInside(draw_list, p0_screen, p1_screen, canvas_p0, canvas_p1, snapping);

	// Todo: for efficiency (if needed), pre-compute the screen positions of other items whenever a new drag is started
	// Snap to other items
	for(auto& item : _items) {
		if(item.dragging) continue; // skip current item

		ImVec2 other_p0 = _camera.worldToScreen(item.p0);
		ImVec2 other_p1 = _camera.worldToScreen(item.p1);

		snapOutside(draw_list, p0_screen, p1_screen, other_p0, other_p1, snapping);
	}
}

