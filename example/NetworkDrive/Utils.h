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
