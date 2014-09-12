#pragma once
#include <vector>
#include <tchar.h>

void getSubDirs(TCHAR* folder, std::vector<CString>& subFolders);
bool isValidDir(const TCHAR* folder);

class NetworkDrive{
public:
	NetworkDrive(TCHAR* szDrive);

	CString GetConnection();
	void AddConnection(TCHAR* folder);
	void CancelConnection();

private:
	CString m_szDrive;
};
