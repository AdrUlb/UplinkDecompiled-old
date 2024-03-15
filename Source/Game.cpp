#include <Game.hpp>
#include <TempDefines.hpp>

Game::Game()
	: interface(nullptr), view(nullptr), world(nullptr), speed(0), obituary(nullptr), loadedSavefileVer(nullptr), UNKNOWN(nullptr),
	  UNKNOWN2(0), winningCodeDesc(nullptr), winningCodeName(nullptr), UNKNOWN3(0), worldmapType(1)
{
}

Game::~Game()
{
	if (interface)
		delete interface;

	if (view)
		delete view;

	if (world)
		delete world;

	if (obituary)
		delete obituary;

	if (loadedSavefileVer)
		delete[] loadedSavefileVer;

	if (UNKNOWN)
		delete[] UNKNOWN;

	if (winningCodeDesc)
		delete[] winningCodeDesc;

	if (winningCodeName)
		delete[] winningCodeName;
}

bool Game::Load(FILE* file)
{
	return Game__Load(this, file);
}

void Game::Save(FILE* file)
{
	if (speed == 0)
		return;

	SaveID(file);

	UplinkAssert(interface);
	UplinkAssert(view);
	UplinkAssert(world);
	fwrite(gCurrentSaveVersion, 6, 1, file);
	fwrite(&worldmapType, sizeof(worldmapType), 1, file);
	fwrite(&speed, sizeof(speed), 1, file);
	if (speed == -1)
	{
		UplinkAssert(obituary);
		GameObituary__Load(obituary, file);
	}
	else
	{
		World__Load(GetWorld(), file);
		Interface__Load(GetInterface(), file);
		View__Load(GetView(), file);
		// GetWorld()->Load(file);
		// GetInterface()->Load(file);
		// GetView()->Load(file);
	}

	SaveDynamicString(UNKNOWN, file);
	fwrite(&UNKNOWN2, 4, 1, file);
	SaveDynamicString(winningCodeDesc, file);
	SaveDynamicString(winningCodeName, file);
	fwrite(&UNKNOWN3, 4, 1, file);

	SaveID_END(file);
}

void Game::Print()
{
	Game__Print(this);
}

void Game::Update()
{
	Game__Update(this);
}

const char* Game::GetID()
{
	return "GAME";
}
