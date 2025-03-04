#ifndef FILESELECTOR_HPP
#define FILESELECTOR_HPP

#include "ui/UIComponent.hpp"

class FileSelector final : public UIComponent {
public:
	static constexpr unsigned int BUFFER_SIZE = 128;

	void render() override;

	[[nodiscard]] const char* getBuffer() const { return _buffer; }
	void clear() { _buffer[0] = '\0'; }

private:
	char _buffer[BUFFER_SIZE] = "";
};

#endif //FILESELECTOR_HPP
