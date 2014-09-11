#pragma once
#include <vector>
#include <tchar.h>

void getSubDirs(TCHAR* folder, std::vector<CString>& subFolders);
bool isValidDir(const TCHAR* folder);