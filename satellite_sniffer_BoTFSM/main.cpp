// satellite_sniffer_BoTFSM.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SDL.h>
#include <SDL_image.h>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include <iostream>

#include "Satellite.h"
#include "Resources.h"
#include "MechStandards.h"

namespace {
	void loadSatellites(std::list<Satellite> &sats) {
		std::ifstream file("satellites/satellites.dat");
		std::string line;
		while (std::getline(file, line)) {
			std::stringstream sat(line);
			std::string name, noradId;
			std::getline(sat, name, ';');
			std::getline(sat, noradId, ';');
			sats.emplace_back(name, noradId);
		}
		file.close();
	}

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
	std::list<Satellite> sats;
	loadSatellites(sats);
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	// init resources
	Resources::getInstance();
	// main loop

	bool quit(false);

	Uint32 timePassed(0);
	Uint32 timestep(16);

	while (!quit) {
		timePassed = SDL_GetTicks();

		quit = handleEvents();

		SDL_RenderClear(Resources::getInstance()->getRenderer());

		auto map(Resources::getInstance()->getMap());
		SDL_SetRenderTarget(Resources::getInstance()->getRenderer(), map.get());

		auto satpos(sats.begin()->calculate());
		std::cout << satpos.first << " " << satpos.second << std::endl;
		satpos.first -= satelliteSniffer::PI;
		satpos.first = -satpos.first;
		satpos.second += satelliteSniffer::PI;
		// rendersats

		SDL_Rect satRect = { satpos.second / (satelliteSniffer::PI * 2) * 8192 - 32,
		satpos.first / (satelliteSniffer::PI) * 4096 - 32, 64, 64 };
		Resources::getInstance()->getSat()->render(&satRect);

		Resources::getInstance()->resetRenderer();
		SDL_Rect pos = { 0,260,960,480 };
		map.render(&pos);

		SDL_RenderPresent(Resources::getInstance()->getRenderer());

		while (timePassed + timestep > SDL_GetTicks()) {
			SDL_Delay(0);
		}

	}

	Resources::releaseResources();
	IMG_Quit();
	SDL_Quit();
	return 0;
}
