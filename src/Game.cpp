#include "Game.h"

void Game::StartLoading(float)
{
	ClearDefault();
	DrawStringDecal({ 0, 0 }, "Creating level...", olc::WHITE, { 2.0f, 2.0f });
	m_fnCurrentUpdate = std::bind(&Game::LoadGame, this, std::placeholders::_1);
}

void Game::LoadGame(float)
{
	level.InitializeProvincesFromBitmap(
		"test"
	);
	level.CreateBorderRenderable();
	m_fnCurrentUpdate = std::bind(&Game::MainLoop, this, std::placeholders::_1);
}

void Game::MainLoop(float fDT)
{
	ClearDefault();

	olc::vf2d posFirst{ level.Normalize(level.v2LocalPosition) };
	olc::vf2d posSecond{ level.Normalize(olc::vf2d{level.v2SpriteSize.x + level.v2LocalPosition.x, level.v2LocalPosition.y }) };

	// First
	DrawDecal(
		posFirst,
		level.rdProvinceBitmap.Decal(),
		{ level.fLocalScale });
	// Second
	DrawDecal(
		posSecond,
		level.rdProvinceBitmap.Decal(),
		{ level.fLocalScale });
	DrawDecal(
		posFirst,
		level.rdBorders.Decal(),
		{ level.fLocalScale });
	// Second
	DrawDecal(
		posSecond,
		level.rdBorders.Decal(),
		{ level.fLocalScale });

	LevelInputHandler(fDT);
	std::stringstream ss;
	ss << "pos: " << level.v2LocalPosition << " scale: " << level.fLocalScale;
	DrawStringDecal({ 0, 0 }, ss.str(), olc::WHITE, { 2.0f, 2.0f });
	
}

bool Game::OnUserCreate() {
	SetPixelMode(olc::Pixel::Mode::ALPHA);
	m_fnCurrentUpdate = std::bind(&Game::StartLoading, this, std::placeholders::_1);
	fSensitivity = 1000.0f;
	fZoomSensitivity = 4.0f;
	level = Level(olc::vf2d{ static_cast<float>(ScreenWidth()), static_cast<float>(ScreenHeight()) });
	return true;
}

bool Game::OnUserUpdate(float fDT) {
	std::function<void(float)> f(m_fnCurrentUpdate);
	f(fDT);

	return true;
}

void Game::LevelInputHandler(float fDT) {
	if (GetKey(olc::W).bHeld)
		level.MoveTowards(Level::Direction::NORTH, fSensitivity, fDT);
	if (GetKey(olc::D).bHeld)
		level.MoveTowards(Level::Direction::EAST, fSensitivity, fDT);
	if (GetKey(olc::S).bHeld)
		level.MoveTowards(Level::Direction::SOUTH, fSensitivity, fDT);
	if (GetKey(olc::A).bHeld)
		level.MoveTowards(Level::Direction::WEST, fSensitivity, fDT);


	if (GetKey(olc::Q).bHeld) {
		level.fLocalScale += fZoomSensitivity * fDT;
		if (level.fLocalScale > level.fMaxScale)
			level.fLocalScale = level.fMaxScale;
	}
	if (GetKey(olc::E).bHeld) {
		level.fLocalScale -= fZoomSensitivity * fDT;
		if (level.fLocalScale < level.fMinScale)
			level.fLocalScale = level.fMinScale;
	}
	
	level.FixPosition();
}
