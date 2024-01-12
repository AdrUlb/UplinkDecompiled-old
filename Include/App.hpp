#pragma once

#include <UplinkObject.hpp>
#include <TempDefines.hpp>

class App : UplinkObject
{
public:
	static constexpr size_t PathMax = 0x100;
	static constexpr size_t VersionMax = 0x20;
	static constexpr size_t TypeMax = 0x20;
	static constexpr size_t DateMax = 0x20;
	static constexpr size_t TitleMax = 0x40;
	static constexpr size_t BuildMax = 0x100;

private:
	char Path[PathMax];
	char UsersPath[PathMax];
	char UsersTempPath[PathMax];
	char UsersOldPath[PathMax];
	char Version[VersionMax];
	char Type[TypeMax];
	char Date[DateMax];
	char Title[TitleMax];
	char Build[BuildMax];
	int InitTime;
	bool Closed;
	Options* TheOptions;
	Network* TheNetwork;
	MainMenu* TheMainMenu;
	PhoneDialler* ThePhoneDialler;
	char* NextLoadGame;
	bool RequireCodeCard;

	virtual ~App() override;
	virtual bool Load() override;
	virtual void Save() override;
	virtual void Print() override;
	virtual void Update() override;
	virtual char* GetID() override;
};
