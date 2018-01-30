#include "stdafx.h"
#include "Menu.h"
#include "Config.h"

Menu::Menu(SDL_Rect rect, PState state) : UIElement(rect, state)
{
	rect.h = Config::getIntOption("Dimensions", "MENU_BUTTON_HEIGHT");
	rect.w = Config::getIntOption("Dimensions", "MENU_BUTTON_WIDTH");
	rect.x += Config::getIntOption("Dimensions", "MENU_BUTTON_SPACING");
	rect.y += Config::getIntOption("Dimensions", "MENU_BUTTON_VERTICAL_SPACING");
	menuButtons.emplace_back(new Button(rect, state, Config::getStringOption("ButtonName", "SATELLITES")));
	rect.x += Config::getRect("MENU").w - Config::getIntOption("Dimensions", "MENU_BUTTON_WIDTH") - 2 * Config::getIntOption("Dimensions", "MENU_BUTTON_SPACING");
	menuButtons.emplace_back(new Button(rect, state, Config::getStringOption("ButtonName", "EXIT")));
}

bool Menu::isClicked(const int x, const int y, PState & state)
{
	if (UIElement::isClicked(x, y, state)) {
		for (auto& button : menuButtons) {
			if (button->isClicked(x, y, state)) {
				break;
			}
		}
		return true;
	}
	return false;
}

void Menu::render()
{
	for (auto& button : menuButtons) {
		button->render();
	}
}

