#include "ToolManager.hpp"

void ToolManager::render() {
	if(empty()) return;

	ImGui::Begin(GUI_ID);
	ImGui::BeginTabBar("TabBar");

	for(auto& [id, tool] : _tools) {
		if(ImGui::BeginTabItem(tool->getName())) {
			tool->render();
			ImGui::EndTabItem();
		}

		if(ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
			close(id);
		}
	}

	ImGui::EndTabBar();
	ImGui::End();
}

void ToolManager::open(const char* id, const std::function<std::unique_ptr<Tool>()>& constructor) {
	auto it = _tools.find(id);
	if(it != _tools.end()) return;
	_tools[id] = std::move(constructor());
}

void ToolManager::close(const char* id) {
	auto it = _tools.find(id);
	if(it == _tools.end()) return;
	_tools.erase(it);
}