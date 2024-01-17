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
		Close();
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
	UplinkStrncpy(Path, path, PathMax);
	UplinkStrncpy(Version, version, VersionMax);
	UplinkStrncpy(Type, type, TypeMax);
	UplinkStrncpy(Date, date, DateMax);
	UplinkStrncpy(Title, title, TitleMax);
	// TODO: remove base game compile date
	UplinkSnprintf(Build, BuildMax,
				   "Version %s (%s)\n"
				   "Base compiled on %s at %s\n"
				   "Compiled on %s\n",
				   Version, Type, baseCompileDate, baseCompileTime, Date);

	auto homeDir = getenv("HOME");
	if (homeDir)
	{
		UplinkSnprintf(UsersPath, PathMax, "%s/.uplink/", homeDir);
		UplinkSnprintf(UsersTempPath, PathMax, "%s/.uplink/userstmp/", homeDir);
		UplinkSnprintf(UsersOldPath, PathMax, "%s/.uplink/usersold/", homeDir);
	}
	else
	{
		UplinkSnprintf(UsersPath, PathMax, "%susers/", Path);
		UplinkSnprintf(UsersTempPath, PathMax, "%suserstmp/", Path);
		UplinkSnprintf(UsersOldPath, PathMax, "%susersold/", Path);
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

void App::Close()
{
	App__Close(this);
}

Options* App::GetOptions()
{
	UplinkAssert(options);
	return options;
}
