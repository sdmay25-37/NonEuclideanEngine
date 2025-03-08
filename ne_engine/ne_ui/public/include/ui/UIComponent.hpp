#ifndef UICOMPONENT_HPP
#define UICOMPONENT_HPP


#include <string>

#include <imgui.h>


#define GEN_GUI_NAME(id, display_name)  \
	inline static const char* GUI_ID = "###" id;  \
	inline static const char* GUI_NAME = display_name; \
	inline static const char* GUI_REFERENCE = display_name "###" id;


class UIComponent {
  public:
	virtual void render() = 0;
    virtual ~UIComponent() = default;
};

#endif //UICOMPONENT_HPP
