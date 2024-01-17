#include <App.hpp>
#include <BinReloc.hpp>
#include <RedShirt.hpp>
#include <TempDefines.hpp>
#include <Util.hpp>
#include <ctime>
#include <signal.h>
#include <string.h>

#define buildVersionString "1.55"
#define buildTypeString "RELEASE"

static void SignalHandler(int signum)
{
	const auto sigdescr = sigdescr_np(signum);
	const auto sigabbrev = sigabbrev_np(signum);

	printf("\nAn Uplink Internal Error has occured: %s (%s)\n", sigdescr, sigabbrev);
	if (file_stdout)
	{
		fprintf(file_stdout, "\nAn Uplink Internal Error has occured: %s (%s)\n", sigdescr, sigabbrev);
		fflush(file_stdout);
	}
	RunUplinkExceptionHandling();
}

static void SetWindowScaleFactor(float x, float y)
{
	gWindowScaleX = x;
	gWindowScaleY = y;
	return;
}

static void Init_App(const char* path)
{
	char buildTime[0x20];
	char debugLogFileName[0x100];

	auto dir = GetFilePath(path);

	gApp = new App();

	UplinkSnprintf(buildTime, sizeof(buildTime), "%s at %s", __DATE__, __TIME__);

	gApp->Set(dir, buildVersionString, "RELEASE", buildTime, "Uplink");

	if (dir)
		delete[] dir;

	printf("=============================\n"
		   "=                           =\n"
		   "=        U P L I N K        =\n"
		   "=                           =\n"
		   "=        Version %-10s =\n"
		   "=     - R E L E A S E -     =\n"
		   "=                           =\n"
		   "=============================\n"
		   "\n"
		   "%s\n",
		   gApp->Version, gApp->Build);

	MakeDirectory(gApp->UsersPath);

	MakeDirectory(gApp->UsersTempPath);
	EmptyDirectory(gApp->UsersTempPath);

	MakeDirectory(gApp->UsersOldPath);

	UplinkSnprintf(debugLogFileName, sizeof(debugLogFileName), "%sdebug.log", gApp->UsersPath);

	file_stdout = nullptr;
	const auto fdstdout = dup(fileno(stdout));
	if (fdstdout != -1)
		file_stdout = fdopen(fdstdout, "a");

	if (!freopen(debugLogFileName, "a", stdout))
		printf("WARNING : Failed to open %s for writing stdout\n", debugLogFileName);

	if (!freopen(debugLogFileName, "a", stderr))
		printf("WARNING : Failed to open %s for writing stderr\n", debugLogFileName);

	const auto currentTime = time((time_t*)0x0);
	const auto localTime = localtime(&currentTime);

	printf("\n\n"
		   "===============================================\n"
		   "NEW GAME     %d:%d, %d/%d/%d\n"
		   "===============================================\n"
		   "Version : %s\n" buildTypeString "\n"
		   "Linux Build\n"
		   "%s\n"
		   "Path : %s\n",
		   localTime->tm_hour, localTime->tm_min, localTime->tm_mday, localTime->tm_mon + 1, localTime->tm_year + 1900, gApp->Version,
		   gApp->Build, gApp->Path);

	RedShirt::Initialise(gApp->Path);
	gApp->Initialise();
	return;
}

static void Init_Options(const int argc, char* argv[])
{
	const auto options = gApp->GetOptions();

		for (auto i = 1; i < argc; i++)
		{
			const auto arg = argv[i];
			const auto prefix = arg[0];

			const auto optionName = arg + 1;
			switch (prefix)
			{
				case '+':
					options->SetOptionValue(optionName, 1);
					break;
				case '-':
					options->SetOptionValue(optionName, 0);
					break;
				case '!':
					i++;

					if (i >= argc)
					{
						printf("Error parsing command line option : %s\n", arg);
						break;
					}

					int value;
					sscanf(argv[i], "%d", &value);
					options->SetOptionValue(optionName, value);
					break;
				default:
					printf("Error parsing command line option : %s\n", arg);
					continue;
			}
		}

	if (options->GetOptionValue("graphics_safemode"))
	{
		options->SetOptionValue("graphics_fullscreen", 0);
		options->SetOptionValue("graphics_screenrefresh", -1);
		options->SetOptionValue("graphics_screendepth", -1);
		options->SetOptionValue("graphics_softwaremouse", 1);
	}
	putchar('\n');

	const auto width = options->GetOptionValue("graphics_screenwidth");
	const auto height = options->GetOptionValue("graphics_screenheight");
	SetWindowScaleFactor(width / 640.0f, height / 480.0f);

	if (options->IsOptionEqualTo("game_debugstart", 1))
		puts("=====DEBUGGING INFORMATION ENABLED=====");
}

static void Cleanup_Uplink()
{
	if (gApp)
	{
		delete gApp;
		gApp = nullptr;
	}
}

static void RunUplink(const int argc, char* argv[])
{
	if (argc >= 2 && argv[1][0] == '-' && argv[1][1] == 'v')
	{
		puts(buildVersionString);
		return;
	}

	const char* path = "/opt/uk.co.introversion.uplink-full/data.dat";

	if (!DoesFileExist(path))
		path = BinReloc::FindExe(nullptr);

	Init_App(path);
	Init_Options(argc, argv);

	if (!VerifyLegitAndCodeCardCheck() || !Load_Data())
	{
		Cleanup_Uplink();
		return;
	}
	Init_Game();
	Init_Graphics();
	Init_OpenGL();
	Init_Fonts();
	// TODO: reenable sound
	// Init_Sound();
	// Init_Music();
	Run_MainMenu();
	Run_Game();
	Cleanup_Uplink();
	fflush(nullptr);
}

int main(int argc, char* argv[])
{
	signal(SIGSEGV, SignalHandler);
	signal(SIGFPE, SignalHandler);
	signal(SIGPIPE, SignalHandler);

	RunUplink(argc, argv);

	return 0;
}
