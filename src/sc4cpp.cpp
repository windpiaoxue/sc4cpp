#include "sc4cpp.h"

// clang-format off
SC_MAIN_BEGIN()
{
    // clang-format on
    // *** Place function declaration here! ***
    UINT MyFunc(VOID);

    SC_IMPORT_API_BATCH_BEGIN();
    SC_IMPORT_API_BATCH("User32.dll", MessageBoxA);
    SC_IMPORT_API_BATCH("User32.dll", MessageBoxW);
    SC_IMPORT_API_BATCH("Kernel32.dll", ExitProcess);
    SC_IMPORT_API_BATCH_END();

    // *** Place code here! ***
    MessageBoxA(NULL, SC_PISTRINGA("Hello Ansi!"), SC_PISTRINGA("Hello!"), MB_OK);
    MessageBoxW(NULL, SC_PISTRINGW(L"Hello Unicode!"), SC_PISTRINGW(L"Hello!"), MB_OK);

    ExitProcess(MyFunc());
    // clang-format off
}
// clang-format on

// *** Place function definition here! ***
SC_NOINLINE UINT MyFunc(VOID) { return 0; }

SC_MAIN_END()
