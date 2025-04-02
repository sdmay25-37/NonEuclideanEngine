#ifndef TOOLMANAGER_HPP
#define TOOLMANAGER_HPP


#include <functional>
#include <memory>
#include <unordered_map>

#include <ui/UIComponent.hpp>

// Todo: add output logging window
class Tool : public UIComponent {
public:
	virtual const char* getName() = 0;
};

class ToolManager : public UIComponent {
public:
	inline static const char* GUI_ID = "ToolManager";

	void render() override;
	void open(const char* id, const std::function<std::unique_ptr<Tool>()>& constructor);
	void close(const char* id);

	[[nodiscard]] bool empty() const { return _tools.empty(); }

private:
	std::unordered_map<const char*, std::unique_ptr<Tool>> _tools;
};

#endif //TOOLMANAGER_HPP
