// satellite_sniffer_BoTFSM.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <list>
#include <iostream>

#include "Satellite.h"
#include "Resources.h"
#include "SatelliteLoader.h"

namespace {
	bool handleEvents() {
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				return true;
			}
		}
		return false;
	}
}


int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	// init resources
	Resources::getInstance();
	std::list<Satellite> sats;
	SatelliteLoader::loadSatellites(sats);
	// main loop

	bool quit(false);
	Uint32 timePassed(0); 
	Uint32 timestep(16); // frame time length 1000 / 60
	Uint32 lastCalculationTime(0);
	Uint32 calculationTimeStep(1000); // 1 sec

	while (!quit) {
		timePassed = SDL_GetTicks();
		quit = handleEvents();

#pragma region calculate_satellite_positions
		if (timePassed > lastCalculationTime + 1000) {
			for (auto& sat : sats) {
				sat.setPosition();
			}
			lastCalculationTime = timePassed;
		}
#pragma endregion

#pragma region render
		SDL_RenderClear(Resources::getInstance()->getRenderer());
		Resources::getInstance()->clearMap();
		Resources::getInstance()->getMap()->setAsRenderTarget();
		for (auto& sat : sats) {
			sat.render();
		}

		Resources::getInstance()->resetRenderer();
		SDL_Rect pos = { 0,0,1280,640 };
		Resources::getInstance()->getMap()->render(&pos);

		SDL_RenderPresent(Resources::getInstance()->getRenderer());
#pragma endregion
		/* wait for next frame */
		while (timePassed + timestep > SDL_GetTicks()) {
			SDL_Delay(0);
		}

	}

	Resources::releaseResources();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	return 0;
}
