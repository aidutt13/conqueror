#pragma once

#include "PixelGameEngine.h"
#include "Level.h"
#include <iostream>

#include <functional>
#include <sstream>
#include <thread>

class Game;
class InputHandler;

class Game : public olc::PixelGameEngine {

public:
	typedef std::function<void(float)> updateFunction;

protected:
	void StartLoading(float);
	void LoadGame(float);
	void DoNothing(float) {
		ClearDefault();
	}

	void MainLoop(float);


protected:
	updateFunction m_fnCurrentUpdate;

public:
	Game() {
		sAppName = "coh";
	}

	bool OnUserCreate() override;
	bool OnUserUpdate(float) override;

	void ClearDefault() {
		Clear(olc::Pixel(0xff7f7f00));
	}

	void LevelInputHandler(float);

public:
	float fSensitivity;
	float fZoomSensitivity;
	Level level;

};
