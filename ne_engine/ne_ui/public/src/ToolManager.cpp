#include "../include/ui/components/ToolManager.hpp"

void ToolManager::render() {
	if(empty()) return;

	ImGui::Begin(GUI_ID);
	ImGui::BeginTabBar("TabBar", ImGuiTabBarFlags_AutoSelectNewTabs);

	for(auto it = _tools.begin(); it != _tools.end(); ) {
		auto& [id, tool] = *it;

		bool open = true;
		if(ImGui::BeginTabItem(tool->getName(), &open)) {
			tool->render();
			ImGui::EndTabItem();
		}

		if(!open) {
			it = _tools.erase(it);
		} else {
			++it;
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