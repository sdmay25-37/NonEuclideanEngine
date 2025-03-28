#ifndef KEYMAPPINGSMANAGER_HPP
#define KEYMAPPINGSMANAGER_HPP

#include "ui/UIComponent.hpp"
#include <IconsFontAwesome5.h>

#include "ui/components/ToolManager.hpp"

class KeyMappingsManager : public Tool {
public:
	GEN_GUI_NAME("KeyMappingsManager", ICON_FA_KEYBOARD " Key Mappings Manager")

	void render() override;
	const char* getName() override { return GUI_NAME; }
};



#endif //KEYMAPPINGSMANAGER_HPP
