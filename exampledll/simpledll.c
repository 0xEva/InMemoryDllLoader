#include <windows.h>

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpvReserved )  // reserved
{
    // Perform actions based on the reason for calling.
    switch( fdwReason ) 
    { 
        case DLL_PROCESS_ATTACH:
         // Initialize once for each new process.
         // Return FALSE to fail DLL load.
			MessageBoxW(
                    NULL,
                    L"Hello, World!",
                    L"Message Box Title",
                    MB_OK | MB_ICONINFORMATION
                    );
            break;

        case DLL_THREAD_ATTACH:
         // Do thread-specific initialization.
            break;

        case DLL_THREAD_DETACH:
         // Do thread-specific cleanup.
            break;

        case DLL_PROCESS_DETACH:
        
            if (lpvReserved != NULL)
            {
                break; // do not do cleanup if process termination scenario
            }
         // Perform any necessary cleanup.
            break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}

// Exported function
__declspec(dllexport) void TestExportedFunc()
{
    MessageBoxW(
            NULL,
            L"TestExportedFunc called",
            L"Exported Function",
            MB_OK | MB_ICONINFORMATION
            );
}
