#pragma once
#include <vector>

class Settings{
public:
    Settings();
    void addDir(CString dir);
    void removeDir(CString dir);
    void getDirs(std::vector<CString>& dirs);
    void load();
    void save();
private:
    std::vector<CString> m_Dirs;
    CString m_fileName;
    CString m_drive;
};
