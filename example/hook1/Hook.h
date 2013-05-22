#ifdef HOOKEXPORTS
#define DLLSPEC extern "C" __declspec(dllexport)
#else
#define DLLSPEC extern "C" __declspec(dllimport)
#endif

//This function installs the Keyboard hook:
DLLSPEC void InstallHook(DWORD dwThreadId);

//This function removes the previously installed hook.
DLLSPEC void RemoveHook();

//hook procedure:
DLLSPEC LRESULT CALLBACK KeyboardProc(int ncode, WPARAM wparam, LPARAM lparam);
