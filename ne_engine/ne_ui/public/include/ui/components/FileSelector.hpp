#ifndef FILESELECTOR_HPP
#define FILESELECTOR_HPP

#include <sstream>

#include "ui/UIComponent.hpp"

class FileSelector final : public UIComponent {
public:
	static constexpr unsigned int BUFFER_SIZE = 128;

	explicit FileSelector(const char* id);

	void render() override;

	[[nodiscard]] const char* getBuffer() const { return _buffer; }
	void clear() { _buffer[0] = '\0'; }

private:
	char _buffer[BUFFER_SIZE] = "";

	std::string _buttonId;
	std::string _textId;
};

#endif //FILESELECTOR_HPP
