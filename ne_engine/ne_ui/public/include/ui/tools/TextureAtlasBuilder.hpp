#ifndef TEXTUREATLASBUILDER_HPP
#define TEXTUREATLASBUILDER_HPP

#include <IconsFontAwesome5.h>

#include "ui/UIComponent.hpp"
#include "ui/components/Canvas.hpp"
#include "ui/components/FileSelector.hpp"
#include "ui/components/ToolManager.hpp"


class TextureAtlasBuilder : public Tool {
public:
	GEN_GUI_NAME("TextureAtlasBuilder", ICON_FA_IMAGE " Texture Atlas Builder")

	TextureAtlasBuilder()
	: _canvas(std::make_unique<Canvas>(ImVec2(2048, 2048))), _atlasFileSelector("AtlasFileSelector"), _textureFolderSelector("TextureFolderSelector") {}

	void render() override;
	const char* getName() override { return GUI_NAME; }

private:
	std::unique_ptr<Canvas> _canvas;
	FileSelector _atlasFileSelector;
	FileSelector _textureFolderSelector;

	void loadTextures(const char* path);
	void saveAtlas(const char* json_path, const char* texture_path);
};

#endif //TEXTUREATLASBUILDER_HPP
