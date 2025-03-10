
#include "ui/components/FileSelector.hpp"

#include <IconsFontAwesome5.h>

FileSelector::FileSelector(const char* id) {
	std::stringstream button_sstream;
	button_sstream << ICON_FA_FOLDER "###" << id << "Button";
	_buttonId = button_sstream.str();

	std::stringstream text_sstream;
	text_sstream << "###" << id << "Text";
	_textId = text_sstream.str();
}

void FileSelector::render() {
	if(ImGui::Button(_buttonId.c_str())) {
		// TODO: actual file dialog
	}

	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() - ImGui::GetStyle().ItemSpacing.x);

	ImGui::InputText(_textId.c_str(), _buffer, sizeof(_buffer));

}
