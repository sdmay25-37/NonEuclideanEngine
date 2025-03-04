
#include "ui/components/FileSelector.hpp"

#include <IconsFontAwesome5.h>

void FileSelector::render() {
	if(ImGui::Button(ICON_FA_FOLDER "##path")) {
		// TODO: actual file dialog
	}

	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() - ImGui::GetStyle().ItemSpacing.x);

	ImGui::InputText("##path", _buffer, sizeof(_buffer));

}
