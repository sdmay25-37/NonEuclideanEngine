#ifndef ANIMATIONEDITOR_HPP
#define ANIMATIONEDITOR_HPP

#include <IconsFontAwesome5.h>

#include "ui/UIComponent.hpp"

class AnimationEditor : public UIComponent {
public:
	GEN_GUI_NAME("AnimationEditor", ICON_FA_FILM " Animation Editor")

	void render() override;
};


#endif //ANIMATIONEDITOR_HPP
