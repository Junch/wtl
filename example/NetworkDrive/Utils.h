#pragma once
#include <vector>
#include <tchar.h>

void getSubDirs(TCHAR* folder, std::vector<CString>& subFolders);
bool isValidDir(const TCHAR* folder);

void AddConnection(TCHAR* folder, TCHAR* driveName);
CString GetConnection(TCHAR* driveName);
void CancelConnection(TCHAR* driveName);