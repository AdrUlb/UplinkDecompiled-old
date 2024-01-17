#include <MainMenu.hpp>

#include <TempDefines.hpp>

MainMenu::MainMenu() : screenCode(0), screen(nullptr) {}

MainMenu::~MainMenu()
{
	if (screen)
		delete screen;
}

void MainMenu::Update()
{
	MainMenu__Update(this);
}

const char* MainMenu::GetID()
{
	return "MMI";
}
