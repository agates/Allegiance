//////////////////////////////////////////////////////////////////////////////
//
// Main function to include in a win32app
//
//////////////////////////////////////////////////////////////////////////////

#include "..\Lib\steam\steam_api.h"

// BT - STEAM - Integrated Steam Exception at the highest level.
void MiniDumpFunction(unsigned int nExceptionCode, EXCEPTION_POINTERS *pException)
{
	// You can build and set an arbitrary comment to embed in the minidump here,
	// maybe you want to put what level the user was playing, how many players on the server,
	// how much memory is free, etc...

	char miniDumpComment[1024] = "";

	if (SteamUser() != nullptr && SteamUser()->BLoggedOn() == true)
	{
		char steamID[64];
		sprintf(steamID, "%lld", SteamUser()->GetSteamID().ConvertToUint64());
		sprintf(miniDumpComment, "%s - %s", steamID, SteamFriends()->GetPersonaName());
	}

	SteamAPI_SetMiniDumpComment(miniDumpComment);

	// The 0 here is a build ID, we don't set it
	SteamAPI_WriteMiniDump(nExceptionCode, pException, 0);
}
int WINAPI Win32Main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow);

#ifdef DREAMCAST
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpszCmdLine, int nCmdShow)
{
    InitAllocs();
    int result = Win32Main(hInstance, hPrevInstance, "", nCmdShow);
    return result;
}
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_WNDW);

    InitAllocs();

    // OleInitialize() is needed for Ole Drag and Drop

    OleInitialize(NULL);
    //CoInitialize(NULL);

    int result = Win32Main(hInstance, hPrevInstance, lpszCmdLine, nCmdShow);

    OleUninitialize();
    //CoUninitialize();

    return result;
}
#endif

int main( int argc, char *argv[ ])
{
    ZString strCommandLine;

    for(int index = 1; index < argc; index++) {
        ZString str = argv[index];

        if (str.Find(' ') == -1) {
            strCommandLine += ZString(argv[index]);
        } else {
            strCommandLine += "\"" + ZString(argv[index]) + "\"";
        }

        if (index < argc - 1) {
            strCommandLine += " ";
        }
    }

#ifdef DREAMCAST
    return WinMain(GetModuleHandle(NULL), NULL, NULL, SW_SHOW);
#else
    return WinMain(GetModuleHandle(NULL), NULL, (char*)(PCC)strCommandLine, SW_SHOW);
#endif
}
