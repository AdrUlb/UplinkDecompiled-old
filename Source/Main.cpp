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

/*static void Init_Options(const int argc, char* argv[])
{
	char* pcVar1;
	char cVar2;
	char* pcVar3;
	bool bVar4;
	Options* pOVar5;
	int iVar6;
	int iVar7;
	int local_14;

	if (1 < argc)
	{
		iVar7 = 1;
		do
		{
			while (true)
			{
				pcVar3 = argv[iVar7];
				cVar2 = *pcVar3;
				if (cVar2 != '\0')
					break;
			LAB_080fe4a8:
				iVar7 = iVar7 + 1;
				printf("Error parsing command line option : %s\n", pcVar3);
				if (argc <= iVar7)
					goto LAB_080fe4c0;
			}
			pcVar1 = pcVar3 + 1;
			if (cVar2 == '+')
			{
				pOVar5 = App::GetOptions(gApp);
				Options::SetOptionValue(pOVar5, pcVar1, 1);
			}
			else if (cVar2 == '-')
			{
				pOVar5 = App::GetOptions(gApp);
				Options::SetOptionValue(pOVar5, pcVar1, 0);
			}
			else
			{
				if (cVar2 != '!')
					goto LAB_080fe4a8;
				iVar6 = iVar7 + 1;
				if (iVar6 < argc)
				{
					sscanf(argv[iVar6], "%d", &local_14);
					iVar7 = local_14;
					pOVar5 = App::GetOptions(gApp);
					Options::SetOptionValue(pOVar5, pcVar1, iVar7);
					iVar7 = iVar6;
				}
				else
				{
					printf("Error parsing command line option : %s\n", argv[iVar7]);
					iVar7 = iVar6;
				}
			}
			iVar7 = iVar7 + 1;
		} while (iVar7 < argc);
	}
LAB_080fe4c0:
	pOVar5 = gApp->GetOptions();
	iVar7 = pOVar5->GetOptionValue("graphics_safemode");
	if (iVar7 == 1)
	{
		pOVar5 = App::GetOptions(gApp);
		Options::SetOptionValue(pOVar5, "graphics_fullscreen", 0);
		pOVar5 = App::GetOptions(gApp);
		Options::SetOptionValue(pOVar5, "graphics_screenrefresh", 0xffffffff);
		pOVar5 = App::GetOptions(gApp);
		Options::SetOptionValue(pOVar5, "graphics_screendepth", 0xffffffff);
		pOVar5 = App::GetOptions(gApp);
		Options::SetOptionValue(pOVar5, "graphics_softwaremouse", 1);
	}
	putchar(10);
	pOVar5 = App::GetOptions(gApp);
	iVar7 = Options::GetOptionValue(pOVar5, "graphics_screenheight");
	pOVar5 = App::GetOptions(gApp);
	iVar6 = Options::GetOptionValue(pOVar5, "graphics_screenwidth");
	SetWindowScaleFactor((float)iVar6 / 640.0, (float)iVar7 / 480.0);
	pOVar5 = App::GetOptions(gApp);
	bVar4 = Options::IsOptionEqualTo(pOVar5, "game_debugstart", 1);
	if (bVar4)
	{
		puts("=====DEBUGGING INFORMATION ENABLED=====");
	}
	return;
}*/

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
