#include <App.hpp>
#include <TempDefines.hpp>
#include <Util.hpp>
#include <cstdlib>

App::App()
{
	UplinkStrncpy(Path, "c:/", PathMax);
	UplinkStrncpy(UsersPath, Path, PathMax);
	UplinkStrncpy(Version, "1.31c", VersionMax);
	UplinkStrncpy(Type, "RELEASE", TypeMax);
	UplinkStrncpy(Date, "01/01/97", DateMax);
	UplinkStrncpy(Title, "NewApp", TitleMax);
	UplinkStrncpy(Build, "Version 1.0 (RELEASE), Compiled on 01/01/97", BuildMax);
	initTime = 0;
	options = nullptr;
	network = nullptr;
	mainMenu = nullptr;
	phoneDialler = nullptr;
	nextLoadGame = nullptr;
	closed = false;
	requireCodeCard = false;
}

App::~App()
{
	if (!closed)
		App__Close(this);
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
	options = new Options();
	options->Load(nullptr);
	options->CreateDefaultOptions();
	initTime = EclGetAccurateTime();
	mainMenu = new MainMenu();
	network = new Network();
}
