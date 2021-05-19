#pragma once

#include "CommonMath.h"
#include "PixelGameEngine.h"
#include <iostream>

#include <map>
#include <string_view>
#include <thread>
#include <vector>

class Level {

public:
	struct Province {
		std::vector<olc::vi2d> vPositions;
		std::vector<olc::vi2d> vBorderPos;

		Province() {
			vPositions.clear();
		}
	};

	typedef std::map<uint32_t, Level::Province> ProvinceMap;

private:

private:
	bool _IsBorderingAnotherColor(olc::vi2d&);

public:
	Level() {}
	Level(olc::vf2d);
	bool InitializeProvincesFromBitmap(std::string_view);
	bool CreateBorderRenderable();

	Level::ProvinceMap& GetProvinces();

	template<typename T>
	T Normalize(T rhs) {
		return rhs * fLocalScale;
	};

public:
	olc::vf2d v2ScreenSize;
	olc::vf2d v2SpriteSize;
	olc::vf2d v2LocalPosition{ 0.0f, 0.0f };
	float fLocalScale{ 1.0f };

	float fMinScale, fMaxScale;

	olc::Renderable rdProvinceBitmap;
	olc::Renderable rdBorders;
	Level::ProvinceMap mProvinces;

public:
	enum Direction {
		NORTH = 0b1000, EAST = 0b0100, SOUTH = 0b0010, WEST = 0b0001
	};

	void MoveTowards(Direction, float, float);
	void FixPosition();

	static olc::vf2d DirToVector(Direction);
};