#ifndef TEXTUREATLASBUILDER_HPP
#define TEXTUREATLASBUILDER_HPP

#include <IconsFontAwesome5.h>

#include "ui/UIComponent.hpp"
#include "ui/components/DraggableCanvas.hpp"


class TextureAtlasBuilder : public UIComponent {
public:
	GEN_GUI_NAME("TextureAtlasBuilder", ICON_FA_LAYER_GROUP " Texture Atlas Builder")

  explicit TextureAtlasBuilder(const char* texture_path);

	void render() override;

private:
	DraggableCanvas _canvas;
};

#endif //TEXTUREATLASBUILDER_HPP
