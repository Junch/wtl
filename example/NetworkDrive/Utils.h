#pragma once
#include <vector>
#include <tchar.h>

class NetworkDrive{
public:
    NetworkDrive(TCHAR* szDrive);

    CString GetConnection();
    void AddConnection(TCHAR* folder);
    void CancelConnection();

private:
    CString m_szDrive;
};

class Setting{
public:
    void addDir(CString dir);
    void removeDir(CString dir);
    void getDirs(std::vector<CString>& dirs);
    void load();
    void save();
private:
    std::vector<CString> m_Dirs;
};
