#ifndef ANIMATIONEDITOR_HPP
#define ANIMATIONEDITOR_HPP

#include <IconsFontAwesome5.h>

#include "ui/UIComponent.hpp"
#include "ui/components/ToolManager.hpp"

class AnimationEditor : public Tool {
public:
	GEN_GUI_NAME("AnimationEditor", ICON_FA_FILM " Animation Editor")

	void render() override;
	const char* getName() override { return GUI_NAME; }
};


#endif //ANIMATIONEDITOR_HPP
