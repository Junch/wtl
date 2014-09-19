#include "stdafx.h"
#include "Utils.h"
#include <Winnetwk.h>

#pragma comment(lib, "mpr.lib")

NetworkDrive::NetworkDrive(TCHAR* szDrive) :m_szDrive(szDrive){

}

CString NetworkDrive::GetConnection(){
    TCHAR buffer[MAX_PATH];
    DWORD len = MAX_PATH;
    DWORD ret = WNetGetConnection(m_szDrive.GetBuffer(), buffer, &len);

    CString sz;
    if (ret == NO_ERROR)
        sz = buffer;

    return sz;
}

CString GetLastErrorAsString(){
    //Get the error message, if any.
    DWORD errorMessageID = ::GetLastError();
    if (errorMessageID == 0)
        return "No error message has been recorded";

    LPTSTR messageBuffer = nullptr;
    size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&messageBuffer, 0, NULL);

    CString message(messageBuffer, size);

    //Free the buffer.
    LocalFree(messageBuffer);

    return message;
}

void NetworkDrive::AddConnection(TCHAR* folder){
    NETRESOURCE nrs;
    memset(&nrs, 0, sizeof(NETRESOURCE));
    nrs.lpLocalName = m_szDrive.GetBuffer();
    nrs.lpRemoteName = folder;

    CString conn = GetConnection();
    if (!conn.IsEmpty()) {
        CString fmt;
        fmt.Format(L"The %s has been mapped to %s. You need unmap it first.", m_szDrive.GetBuffer(), conn.GetBuffer());
        ::MessageBox(NULL, fmt.GetBuffer(), L"Warning", MB_OK | MB_ICONERROR);
        return;
    }

    DWORD dwd = WNetAddConnection2(&nrs, NULL, NULL, CONNECT_UPDATE_PROFILE);
    if (dwd != NO_ERROR){
        CString fmt = GetLastErrorAsString();
        ::MessageBox(NULL, fmt.GetBuffer(), L"Warning", MB_OK | MB_ICONWARNING);
    }
}

void NetworkDrive::CancelConnection(){
    WNetCancelConnection2(m_szDrive, CONNECT_UPDATE_PROFILE, TRUE);
}
