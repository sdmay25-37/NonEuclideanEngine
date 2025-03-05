#include "ToolManager.hpp"

void ToolManager::render() {
	if(empty()) return;

	ImGui::Begin(GUI_ID);
	ImGui::BeginTabBar("TabBar");

	for(auto& [_, tool] : _tools) {
		if(ImGui::BeginTabItem(tool->getName())) {
			tool->render();
			ImGui::EndTabItem();
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