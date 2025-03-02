#ifndef UICOMPONENT_HPP
#define UICOMPONENT_HPP

class UIComponent {
  public:
	virtual void Render() = 0;
    virtual ~UIComponent() = default;
};

#endif //UICOMPONENT_HPP
