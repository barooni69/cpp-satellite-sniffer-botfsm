#pragma once
#include <list>
#include <memory>

#include "Satellite.h"
#include "UIElement.h"
#include "ProgramState.h"

class Program {
public:
	Program();
	~Program();
	void init();
	void run();
	void unload();
private:
	std::list<Satellite> sats;
	std::list<std::unique_ptr<UIElement>> UIElements;
	PState state;
	bool quit;
	bool loaded;
	Uint32 timePassed;
	Uint32 timestep;
	Uint32 lastCalculationTime;
	Uint32 calculationTimeStep;
	bool handleEvents();
	void updatePositions();
	void render();
	void renderMainScreen();
};

