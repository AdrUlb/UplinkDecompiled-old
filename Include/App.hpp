#pragma once

#include <MainMenu.hpp>
#include <Network.hpp>
#include <Options.hpp>
#include <PhoneDialler.hpp>
#include <UplinkObject.hpp>

class App : UplinkObject
{
public:
	static constexpr size_t PathMax = 0x100;
	static constexpr size_t VersionMax = 0x20;
	static constexpr size_t TypeMax = 0x20;
	static constexpr size_t DateMax = 0x20;
	static constexpr size_t TitleMax = 0x40;
	static constexpr size_t BuildMax = 0x100;

	char Path[PathMax];
	char UsersPath[PathMax];
	char UsersTempPath[PathMax];
	char UsersOldPath[PathMax];
	char Version[VersionMax];
	char Type[TypeMax];
	char Date[DateMax];
	char Title[TitleMax];
	char Build[BuildMax];

private:
	int initTime;
	bool closed;
	Options* options;
	Network* network;
	MainMenu* mainMenu;
	PhoneDialler* phoneDialler;
	char* nextLoadGame;
	bool requireCodeCard;

public:
	App();
	void Set(const char* path, const char* version, const char* type, const char* date, const char* title);
	void Initialise();
	void Close();
	Options* GetOptions();

	virtual ~App() override;
	virtual void Print() override;
	virtual void Update() override;
	virtual const char* GetID() override;
};
