#ifndef DRAGGABLECANVAS_HPP
#define DRAGGABLECANVAS_HPP

#include <memory>
#include <vector>

#include <IconsFontAwesome5.h>
#include <Texture.hpp>

#include "ui/UIComponent.hpp"

class ScreenCamera {
public:
	ScreenCamera() : _pos(ImVec2(0.0f, 0.0f)), _zoom(1) {}

	[[nodiscard]] ImVec2 worldToScreen(ImVec2 world_pos) const;

    [[nodiscard]] ImVec2 screenToWorld(ImVec2 screen_pos) const;

    void pan(ImVec2 delta);

    void zoom(float delta, ImVec2 cursor_pos);

private:
	ImVec2 _pos;
	float _zoom;
};

struct CanvasItem {
	Texture texture;
	ImVec2 p0, p1;
	bool dragging = false;

	explicit CanvasItem(Texture&& texture)
		: texture(std::move(texture)), p0({0, 0}), p1({0, 0}) {
		p1 = ImVec2(p0.x + texture.getWidth(), p0.y + texture.getHeight());
	}

};

class Canvas : public UIComponent {
  GEN_GUI_NAME("Canvas", ICON_FA_PALETTE " Canvas")

public:

	void render() override;
	~Canvas() override = default;

	explicit Canvas(const ImVec2 size) : _size(size) {}

	void addItem(Texture&& texture) {
		_items.emplace_back(std::move(texture));
	}

private:
    ScreenCamera _camera;
	std::vector<CanvasItem> _items;
	bool is_dragging = false;
	ImVec2 _size;
	ImVec2 _p0, _p1;

  void handleInput(ImDrawList* draw_list);
	void snapToItems(ImDrawList* draw_list, ImVec2& p0_screen, ImVec2& p1_screen);
};



#endif //DRAGGABLECANVAS_HPP
