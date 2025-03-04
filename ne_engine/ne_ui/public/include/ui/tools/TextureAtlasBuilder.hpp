#ifndef TEXTUREATLASBUILDER_HPP
#define TEXTUREATLASBUILDER_HPP

#include <IconsFontAwesome5.h>

#include "ui/UIComponent.hpp"
#include "ui/components/DraggableCanvas.hpp"
#include "ui/components/FileSelector.hpp"


class TextureAtlasBuilder : public UIComponent {
public:
	GEN_GUI_NAME("TextureAtlasBuilder", ICON_FA_LAYER_GROUP " Texture Atlas Builder")

	void render() override;

private:
	DraggableCanvas _canvas;
	FileSelector _textureFolderSelector;

	void loadTextures(const char* path);
};

#endif //TEXTUREATLASBUILDER_HPP
