#include <App.hpp>
#include <TempDefines.hpp>
#include <Util.hpp>
#include <cstdlib>

App::App()
{
	UplinkStrncpy(this->Path, "c:/", PathMax);
	UplinkStrncpy(this->UsersPath, this->Path, PathMax);
	UplinkStrncpy(this->Version, "1.31c", VersionMax);
	UplinkStrncpy(this->Type, "RELEASE", TypeMax);
	UplinkStrncpy(this->Date, "01/01/97", DateMax);
	UplinkStrncpy(this->Title, "NewApp", TitleMax);
	UplinkStrncpy(this->Build, "Version 1.0 (RELEASE), Compiled on 01/01/97", BuildMax);
	InitTime = 0;
	TheOptions = nullptr;
	TheNetwork = nullptr;
	TheMainMenu = nullptr;
	ThePhoneDialler = nullptr;
	NextLoadGame = nullptr;
	Closed = false;
	RequireCodeCard = false;
}

App::~App()
{
	if (!Closed)
	{
		App__Close(this);
	}
}

void App::Print()
{
	App__Print(this);
}

void App::Update()
{
	App__Update(this);
}

const char* App::GetID()
{
	return "APP";
}

void App::Set(const char* path, const char* version, const char* type, const char* date, const char* title)
{
	unsigned int uVar1;
	size_t uVar4;
	char* pcVar5;

	// TODO: remove redundant assertions, UplinkStrncpy checks this
	UplinkAssert(strlen(path) < PathMax);
	UplinkAssert(strlen(version) < VersionMax);
	UplinkAssert(strlen(type) < TypeMax);
	UplinkAssert(strlen(date) < DateMax);
	UplinkAssert(strlen(title) < TitleMax);
	UplinkStrncpy(this->Path, path, PathMax);
	UplinkStrncpy(this->Version, version, VersionMax);
	UplinkStrncpy(this->Type, type, TypeMax);
	UplinkStrncpy(this->Date, date, DateMax);
	UplinkStrncpy(this->Title, title, TitleMax);
	// TODO: remove base game compile date
	UplinkSnprintf(this->Build, BuildMax,
				   "Version %s (%s)\n"
				   "Base compiled on %s at %s\n"
				   "Compiled on %s\n",
				   this->Version, this->Type, baseCompileDate, baseCompileTime, this->Date);

	auto homeDir = getenv("HOME");
	if (homeDir)
	{
		UplinkSnprintf(this->UsersPath, PathMax, "%s/.uplink/", homeDir);
		UplinkSnprintf(this->UsersTempPath, PathMax, "%s/.uplink/userstmp/", homeDir);
		UplinkSnprintf(this->UsersOldPath, PathMax, "%s/.uplink/usersold/", homeDir);
	}
	else
	{
		UplinkSnprintf(this->UsersPath, PathMax, "%susers/", this->Path);
		UplinkSnprintf(this->UsersTempPath, PathMax, "%suserstmp/", this->Path);
		UplinkSnprintf(this->UsersOldPath, PathMax, "%susersold/", this->Path);
	}
}

void App::Initialise()
{
	App__Initialise(this);
}
