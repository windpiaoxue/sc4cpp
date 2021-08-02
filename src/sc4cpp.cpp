#include "sc4cpp.h"

// *** Place function definition here! ***

SC_NOINLINE
SC_CODESEG_REORDERING
DWORD WINAPI WorkerThread(PVOID lpAnsiMsg) {
    SC_IMPORT_API_BATCH_BEGIN();
    SC_IMPORT_API_BATCH("User32.dll", MessageBoxA);
    SC_IMPORT_API_BATCH("User32.dll", MessageBoxW);
    SC_IMPORT_API_BATCH_END();

    MessageBoxA(NULL, (PCSTR)lpAnsiMsg, SC_PISTRINGA("Hello!"), MB_OK);
    MessageBoxW(NULL, SC_PISTRINGW(L"Hello Unicode!"), SC_PISTRINGW(L"Hello!"), MB_OK);

    return 0;
}

// clang-format off
SC_MAIN_BEGIN()
{
    // clang-format on
    SC_IMPORT_API_BATCH_BEGIN();
    SC_IMPORT_API_BATCH("Kernel32.dll", CreateThread);
    SC_IMPORT_API_BATCH("Kernel32.dll", WaitForSingleObject);
    SC_IMPORT_API_BATCH("Kernel32.dll", ExitProcess);
    SC_IMPORT_API_BATCH_END();

    // *** Place code here! ***

    HANDLE hWorker =
        CreateThread(NULL, 0, SC_PIFUNCTION(WorkerThread), SC_PISTRINGA("Hello Ansi!"), 0, NULL);
    WaitForSingleObject(hWorker, INFINITE);

    ExitProcess(0);
    // clang-format off
}
SC_MAIN_END()
// clang-format on
