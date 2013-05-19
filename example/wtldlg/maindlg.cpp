#include "stdafx.h"
#include "resource.h"
#include "MainDlg.h"


//
// http://forums.codeguru.com/showthread.php?377394-Windows-SDK-User-Interface-How-can-I-emulate-mouse-events-in-an-application
//

void LeftClick ( )
{  
    INPUT    Input={0};
    // left down 
    Input.type       = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    ::SendInput(1, &Input, sizeof(INPUT));

    // left up
    ::ZeroMemory(&Input,sizeof(INPUT));
    Input.type       = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    ::SendInput(1, &Input, sizeof(INPUT));
}

void RightClick ( )
{  
    INPUT    Input={0};
    // right down 
    Input.type       = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
    ::SendInput(1, &Input, sizeof(INPUT));

    // right up
    ::ZeroMemory(&Input,sizeof(INPUT));
    Input.type       = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
    ::SendInput(1, &Input, sizeof(INPUT));
}

void MouseMove (int x, int y)
{  
    double fScreenWidth  = ::GetSystemMetrics(SM_CXSCREEN)-1; 
    double fScreenHeight = ::GetSystemMetrics(SM_CYSCREEN)-1; 
    double fx = x*(65535.0f/fScreenWidth);
    double fy = y*(65535.0f/fScreenHeight);
    INPUT Input={0};
    Input.type      = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_MOVE|MOUSEEVENTF_ABSOLUTE;
    Input.mi.dx = fx;
    Input.mi.dy = fy;
    ::SendInput(1,&Input,sizeof(INPUT));
}

LRESULT CMainDlg::OnStart(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    POINT pts[]= {{30, 20}, {200, 20}, {200, 300}, {30,300}};

    int i = 0;
    while(i < 8)
    {
        int j = i % 4;
        POINT pt = pts[j];
        ClientToScreen(&pt);
        MouseMove(pt.x, pt.y);

        Sleep(1000);
        ++i;
    }

    return 0;
}
