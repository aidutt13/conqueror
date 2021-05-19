#include "Level.h"

bool Level::_IsBorderingAnotherColor(olc::vi2d& center)
{
	bool isBordering = false;
	auto* pbs = rdProvinceBitmap.Sprite();
	olc::Pixel centerPixel = pbs->GetPixel(center);

	for (int32_t i = -1; i <= 1; i++)
		for (int32_t j = -1; j <= 1; j++)
			if (centerPixel != pbs->GetPixel(center.x - j, center.y - i))
				return true;
	return false;
}

Level::Level(olc::vf2d _screenSize)
	: v2ScreenSize(_screenSize)
{}

bool Level::InitializeProvincesFromBitmap(std::string_view sPathName) {
	rdProvinceBitmap.Load(sPathName.data());
	const auto* pbs = rdProvinceBitmap.Sprite();
	v2SpriteSize = { (float)pbs->width, (float)pbs->height };
	mProvinces.clear();

	for (uint32_t i{ 0 }; i < pbs->height; i++) {
		for (uint32_t j{ 0 }; j < pbs->width; j++) {
			int32_t ii = i, ij = j;
			olc::vi2d vs{ ij, ii };

			const olc::Pixel& px = pbs->GetPixel(vs);

			mProvinces[px.n].vPositions.push_back(vs);

			// Checks if province is at the edge of the world
			// Uses the fact that 0 - 1 = MAX_UINT32 - this reduces comparisions from 4 to 2
			if (j - 1 >= pbs->width - 1 || i - 1 >= pbs->height - 1) {
				mProvinces[px.n].vBorderPos.push_back(vs);
			}
			else if (_IsBorderingAnotherColor(vs)) {
				mProvinces[px.n].vBorderPos.push_back(vs);
			}
		}
	}
	
	float
		minx = v2ScreenSize.x / v2SpriteSize.x,
		miny = v2ScreenSize.y / v2SpriteSize.y,
		maxx = v2SpriteSize.x / v2ScreenSize.x,
		maxy = v2SpriteSize.y / v2ScreenSize.y;

	fMinScale = std::max(minx, miny);
	fMaxScale = std::min(maxx, maxy);
	std::cout << v2ScreenSize << std::endl
		<< v2SpriteSize << std::endl
		<< olc::vf2d{fMinScale, fMaxScale} << std::endl
		<< olc::vf2d{ v2ScreenSize.x / v2SpriteSize.x, v2ScreenSize.y / v2SpriteSize.y } << std::endl
		<< olc::vf2d{ v2SpriteSize.x / v2ScreenSize.x, v2SpriteSize.y / v2ScreenSize.y } << std::endl;
	return true;
}

bool Level::CreateBorderRenderable()
{
	rdBorders.Create(v2SpriteSize.x, v2SpriteSize.y);
	auto* sprite = rdBorders.Sprite();
	for (auto i = 0; i < v2SpriteSize.y; ++i)
		for (auto j = 0; j < v2SpriteSize.x; ++j)
			sprite->SetPixel(j, i, olc::BLANK);

	for (auto i = mProvinces.begin(); i != mProvinces.end(); ++i) {
		for (auto j = i->second.vBorderPos.begin(); j != i->second.vBorderPos.end(); ++j) {
			sprite->SetPixel(*j, olc::BLACK);
		}
	}

	rdBorders.Decal()->Update();
	return true;
}


Level::ProvinceMap& Level::GetProvinces()
{
	return this->mProvinces;
}


void Level::MoveTowards(Direction dir, float fSpeed, float fDT)
{
	v2LocalPosition += Level::DirToVector(dir) / fLocalScale * fSpeed * fDT;
}

void Level::FixPosition() {
	olc::vf2d normalizedSpriteSize{ Normalize(v2SpriteSize) };
	olc::vf2d normalizedLocalPos{ Normalize(v2LocalPosition) };

	if (normalizedLocalPos.x > 0)
		v2LocalPosition.x -= v2SpriteSize.x;
	if (normalizedLocalPos.x < -normalizedSpriteSize.x)
		v2LocalPosition.x += v2SpriteSize.x;

	float minY = v2ScreenSize.y - normalizedSpriteSize.y;
	if (v2LocalPosition.y < minY) {
		v2LocalPosition.y = minY;
	}
	if (v2LocalPosition.y > 0.0f)
		v2LocalPosition.y = 0.0f;
}

olc::vf2d Level::DirToVector(Direction dir) {

	const auto& test = [&dir](Direction _rhs) -> float {
		return static_cast<float>(static_cast<bool>(dir & _rhs));
	};

	return olc::vf2d{
		-1.0f * test(Direction::EAST) + 1.0f * test(Direction::WEST),
		-1.0f * test(Direction::SOUTH) + 1.0f * test(Direction::NORTH)
	};
}
