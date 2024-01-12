#include <App.hpp>
#include <BinReloc.hpp>
#include <TempDefines.hpp>
#include <signal.h>
#include <string.h>

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
