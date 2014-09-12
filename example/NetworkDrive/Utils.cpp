#include "stdafx.h"
#include "Utils.h"
#include <Winnetwk.h>

#pragma comment(lib, "mpr.lib")

void getSubDirs(TCHAR* rootDir, std::vector<CString>& subDirs){
	WIN32_FIND_DATA fd;
    ZeroMemory(&fd, sizeof(WIN32_FIND_DATA));

    HANDLE hFile = FindFirstFile(rootDir, &fd);
    if (hFile == INVALID_HANDLE_VALUE)
        return;

    do {
        if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
			if (lstrcmp(fd.cFileName, TEXT(".")) == 0 ||
				lstrcmp(fd.cFileName, TEXT("..")) == 0)
			{
				continue;
			}

            subDirs.push_back(fd.cFileName);
        }
    }
    while(FindNextFile(hFile, &fd) != 0);

    FindClose(hFile);
}

bool isValidDir(const TCHAR* folder)
{
	WIN32_FIND_DATA fd;
	ZeroMemory(&fd, sizeof(WIN32_FIND_DATA));

	HANDLE hFile = FindFirstFile(folder, &fd);
	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	FindClose(hFile);
	bool bRet = (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
	return bRet;
}

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

void NetworkDrive::AddConnection(TCHAR* folder){
	NETRESOURCE nrs;
	memset(&nrs, 0, sizeof(NETRESOURCE));
	nrs.lpLocalName = m_szDrive.GetBuffer();
	nrs.lpRemoteName = folder;

	CString conn = GetConnection();
	if (!conn.IsEmpty()) {
		CString fmt;
		fmt.Format(L"The %s has already mapped to %s. You need unmap it first.", m_szDrive.GetBuffer(), conn.GetBuffer());
		::MessageBox(NULL, fmt.GetBuffer(), L"Warning", MB_OK);
		return;
	}

	WNetAddConnection2(&nrs, NULL, NULL, CONNECT_UPDATE_PROFILE);
}

void NetworkDrive::CancelConnection(){
	WNetCancelConnection2(m_szDrive, CONNECT_UPDATE_PROFILE, TRUE);
}
