#include "stdafx.h"
#include "Utils.h"

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