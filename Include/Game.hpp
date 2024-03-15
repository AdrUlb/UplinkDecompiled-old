#pragma once

#include <GameObituary.hpp>
#include <Interface.hpp>
#include <UplinkObject.hpp>
#include <Util.hpp>
#include <View.hpp>
#include <World.hpp>
#include <ctime>

class Game : UplinkObject
{
	Interface* interface;
	View* view;
	World* world;
	int speed;
	GameObituary* obituary;
	time_t lastUpdateTime;
	char* loadedSavefileVer;
	char* UNKNOWN;
	char* UNKNOWN2;
	char* winningCodeDesc;
	char* winningCodeName;
	int UNKNOWN3;
	int worldmapType;

public:
	Game();
	virtual ~Game() override;
	virtual bool Load(FILE* file) override;
	virtual void Save(FILE* file) override;
	virtual void Print() override;
	virtual void Update() override;
	virtual const char* GetID() override;

	inline World* GetWorld()
	{
		UplinkAssert(world);
		return world;
	}

	inline Interface* GetInterface()
	{
		UplinkAssert(interface);
		return interface;
	}

	inline View* GetView()
	{
		UplinkAssert(view);
		return view;
	}
};
