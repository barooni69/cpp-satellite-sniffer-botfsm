#include "stdafx.h"
#include "Config.h"
#include "Program.h"
#include "Popup.h"
#include "Resources.h"
#include "Satellites.h"
#include "Menu.h"
#include "Map.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_syswm.h>
#include <imgui.h>
#include <imgui_impl_dx9.h>

Program::Program() :
	quit(false),
	loaded(false),
	timePassed(0),
	timestep(16), // frame time length 1000 / 60
	lastCalculationTime(0),
	calculationTimeStep(5000), // 1 sec
	state(PState::RUNNING)
{}


Program::~Program()
{
	if (loaded) unload();
}

void Program::init()
{
	// init SDL
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();

	// init resources
	Resources::getInstance();
	Satellites::getInstance();

	UIElements.emplace_back(new Map(Config::getRect("MAP"), PState::RUNNING));
	UIElements.emplace_back(new Menu(Config::getRect("MENU"), PState::RUNNING));
	UIElements.emplace_back(new Popup(Config::getRect("POPUP"), PState::PAUSED));

	// init imgui

	SDL_SysWMinfo info;
	SDL_VERSION(&info.version);
	SDL_GetWindowWMInfo(Resources::getInstance()->getWindow(), &info);

	ImGui_ImplDX9_Init(info.info.win.window, SDL_RenderGetD3D9Device(Resources::getInstance()->getRenderer()));
	ImGui::StyleColorsDark();

	loaded = true;
}

void Program::run()
{
	if (!loaded) init();
	while (!quit) {
		timePassed = SDL_GetTicks();
		quit = handleEvents();
		updatePositions();

		render();

		/* wait for next frame */
		while (timePassed + timestep > SDL_GetTicks()) {
			SDL_Delay(0);
		}
	}
}

void Program::unload()
{
	ImGui_ImplDX9_Shutdown();
	Resources::releaseResources();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	loaded = false;
}

void Program::updatePositions()
{
	if (state != PState::PAUSED && timePassed > lastCalculationTime + calculationTimeStep) {
		Satellites::getInstance()->updatePosition();
		lastCalculationTime = timePassed;
	}
}

void Program::render()
{
	ImGui_ImplDX9SDL_NewFrame(Resources::getInstance()->getWindow());
	SDL_SetRenderDrawColor(Resources::getInstance()->getRenderer(), 50, 50, 50, 255);
	for (auto& elem : UIElements) {
		if (elem->isActive(state)) {
			elem->render(&state);
		}
	}

	//ImGui::ShowMetricsWindow();
	//ImGui::ShowDemoWindow();
	ImGui::Render();
	SDL_RenderPresent(Resources::getInstance()->getRenderer());
}


bool Program::handleEvents()
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0) {
		ImGui_Sdl_ProcessEvent(&e);
		if (e.type == SDL_QUIT) {
			return true;
		}
		auto& io = ImGui::GetIO();
		if (!io.WantCaptureMouse) {
			if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
				for (auto& elem : UIElements) {
					if (elem->isActive(state) && elem->isClicked(e.button.x, e.button.y, state)) {
						break;
					}
				}
			}
		}
		if (PState::QUIT == state) {
			return true;
		}
	}
	return false;
}
