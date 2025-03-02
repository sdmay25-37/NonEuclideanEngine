#ifndef TEXTUREATLASBUILDER_HPP
#define TEXTUREATLASBUILDER_HPP

#include "ui/UIComponent.hpp"
#include "ui/components/DraggableCanvas.hpp"

class TextureAtlasBuilder : public UIComponent {
public:

  explicit TextureAtlasBuilder(const char* texture_path);

	void render() override;

private:
	DraggableCanvas _canvas;
};

#endif //TEXTUREATLASBUILDER_HPP
