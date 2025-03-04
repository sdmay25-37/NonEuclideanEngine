#ifndef KEYMAPPINGSMANAGER_HPP
#define KEYMAPPINGSMANAGER_HPP

#include "ui/UIComponent.hpp"
#include <IconsFontAwesome5.h>

class KeyMappingsManager : public UIComponent {
public:
	GEN_GUI_NAME("KeyMappingsManager", ICON_FA_KEYBOARD " Key Mappings Manager")

	void render() override;
};



#endif //KEYMAPPINGSMANAGER_HPP
