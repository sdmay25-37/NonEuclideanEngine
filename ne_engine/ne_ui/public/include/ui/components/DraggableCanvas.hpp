#ifndef DRAGGABLECANVAS_HPP
#define DRAGGABLECANVAS_HPP

#include <memory>
#include <vector>

#include "ui/UIComponent.hpp"


class DraggableCanvas : public UIComponent {
public:

	void render() override;
	~DraggableCanvas() override = default;

	void addComponent(std::unique_ptr<DraggableComponent> component) {
		_components.push_back(std::move(component));
	}

	void setSize(const ImVec2 size) { this->_size = size; }
	void setPosition(const ImVec2 position) { this->_position = position; }

private:
	std::vector<std::unique_ptr<DraggableComponent>> _components;
	ImVec2 _size;
	ImVec2 _position;
};



#endif //DRAGGABLECANVAS_HPP
