#ifndef UICOMPONENT_HPP
#define UICOMPONENT_HPP

#include <string>
#include <imgui.h>

#include "Texture.hpp"

#define GEN_GUI_NAME(id, display_name)  \
	inline static const char* GUI_ID = "###" id;  \
	inline static const char* GUI_NAME = display_name "###" id;

class UIComponent {
  public:
	virtual void render() = 0;
    virtual ~UIComponent() = default;
};

class DraggableComponent : public UIComponent {
public:
	ImVec2 getPosition() { return _position; };
	ImVec2 getSize()  { return _size; };
	void setPosition(ImVec2 position) { _position = position; };

	DraggableComponent(std::string id, ImVec2 position, ImVec2 size)
		: _id(std::move(id)), _position(position), _size(size) {};

	void render() override { ImGui::SetCursorPos(_position); };
	 ~DraggableComponent() override = default;

protected:
	std::string _id;
	ImVec2 _position;
	ImVec2 _size;
};

#endif //UICOMPONENT_HPP
