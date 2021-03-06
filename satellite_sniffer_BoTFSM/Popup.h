#pragma once
#include <memory>
#include <list>

#include "UIElement.h"
#include "Button.h"

class Popup : public UIElement
{
public:
	Popup(SDL_Rect rect, PState state, PState& programState);
	bool isClicked(const SDL_MouseButtonEvent e, PState& state) override;
	void render() override;
private:
	PState & state;
	std::unique_ptr<Button> button;
};

