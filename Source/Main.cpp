#include <App.hpp>
#include <BinReloc.hpp>
#include <TempDefines.hpp>
#include <Util.hpp>
#include <ctime>
#include <dirent.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>

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

static bool DoesFileExist(const char* path)
{
	return access(path, 0) == 0;
}

static char* GetFilePath(const char* path)
{
	UplinkAssert(path);

	const auto bufferSize = strlen(path) + 1;
	auto buffer = new char[bufferSize];
	if (bufferSize != 0)
		UplinkStrncpy(buffer, path, bufferSize);

	auto dirSepStart = strrchr(buffer, '/');
	if (dirSepStart)
	{
		dirSepStart[1] = 0;
		return buffer;
	}

	dirSepStart = strrchr(buffer, '\\');
	if (dirSepStart)
	{
		dirSepStart[1] = 0;
		return buffer;
	}

	if (buffer)
		delete[] buffer;

	return strdup("./");
}

static void MakeDirectory(char* path)
{
	mkdir(path, 0700);
}

static void EmptyDirectory(char* path)
{
	char* pcVar1;
	DIR* __dirp;
	dirent* pdVar3;
	int iVar5;
	char* pcVar6;
	const char* pcVar7;
	bool bVar8;
	char local_210[256];
	char local_110[256];

	UplinkStrncpy(local_110, path, 0x100);
	local_110[255] = '\0';
	__dirp = opendir(local_110);
	if (__dirp != (DIR*)0x0)
	{
		pdVar3 = readdir(__dirp);
		while (bVar8 = pdVar3 == (dirent*)0x0, !bVar8)
		{
			pcVar1 = pdVar3->d_name;
			iVar5 = 2;
			pcVar6 = pcVar1;
			pcVar7 = ".";
			do
			{
				if (iVar5 == 0)
					break;
				iVar5 = iVar5 + -1;
				bVar8 = *pcVar6 == *pcVar7;
				pcVar6 = pcVar6 + 1;
				pcVar7 = pcVar7 + 1;
			} while (bVar8);
			if (!bVar8)
			{
				iVar5 = 3;
				pcVar6 = pcVar1;
				pcVar7 = "..";
				do
				{
					if (iVar5 == 0)
						break;
					iVar5 = iVar5 + -1;
					bVar8 = *pcVar6 == *pcVar7;
					pcVar6 = pcVar6 + 1;
					pcVar7 = pcVar7 + 1;
				} while (bVar8);
				if (!bVar8)
				{
					UplinkSnprintf(local_210, 0x100, "%s%s", path, pcVar1);
					unlink(local_210);
				}
			}
			pdVar3 = readdir(__dirp);
		}
		closedir(__dirp);
	}
	return;
}

static void Init_App(const char* path)
{
	char buildTime[0x20];
	char debugLogFileName[0x100];

	auto dir = GetFilePath(path);

	gApp = new App();

	UplinkSnprintf(buildTime, sizeof(buildTime), "%s at %s", __DATE__, __TIME__);

	gApp->Set(dir, "1.55", "RELEASE", buildTime, "Uplink");

	if (dir)
		delete[] dir;

	puts("=============================");
	puts("=                           =");
	puts("=        U P L I N K        =");
	puts("=                           =");
	printf("=        Version %-10s =\n", gApp->Version);
	puts("=     - R E L E A S E -     =");
	puts("=                           =");
	puts("=============================");
	putchar(L'\n');
	puts(gApp->Build);

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
	puts("\n");
	puts("===============================================");
	printf("NEW GAME     %d:%d, %d/%d/%d\n", localTime->tm_hour, localTime->tm_min, localTime->tm_mday, localTime->tm_mon + 1,
		   localTime->tm_year + 1900);
	puts("===============================================");
	printf("Version : %s\n", gApp->Version);
	puts("RELEASE");
	puts("Linux Build");
	puts(gApp->Build);
	printf("Path : %s\n", gApp->Path);
	RsInitialise(gApp->Path);
	gApp->Initialise();
	return;
}

static void RunUplink(const int argc, char* argv[])
{
	if (argc >= 2 && argv[1][0] == '-' && argv[1][1] == 'v')
	{
		puts("1.55");
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
	Init_Sound();
	Init_Music();
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
