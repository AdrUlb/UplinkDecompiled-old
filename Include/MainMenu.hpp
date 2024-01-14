#pragma once

#include <UplinkObject.hpp>

typedef void MainMenuScreen;

class MainMenu : UplinkObject
{
	int screenCode;
	MainMenuScreen* screen;

public:
	MainMenu();
	virtual ~MainMenu() override;

private:
	virtual void Update() override;
	virtual const char* GetID() override;
};
