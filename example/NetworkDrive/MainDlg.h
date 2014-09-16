// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Utils.h"

class CMainDlg : public CDialogImpl < CMainDlg >
{
public:
    enum { IDD = IDD_MAINDLG };

    CEdit     m_mappedDrive;

    CListBox  m_lsDirectories;
    CEdit     m_editDirectory;
    NetworkDrive m_nwDrive;

    BEGIN_MSG_MAP(CMainDlg)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenu);
        COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
        COMMAND_ID_HANDLER(IDC_BTN_UNMAP, OnUnMap)
        COMMAND_ID_HANDLER(IDC_BUTTON_ADD, OnAdd)
        COMMAND_ID_HANDLER(ID_MAP_DELETE, OnDelete)
        COMMAND_ID_HANDLER(ID_MAP_MAP, OnMap)
    END_MSG_MAP()

    // Handler prototypes (uncomment arguments if needed):
    //	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    //	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    //	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

    CMainDlg() :m_nwDrive(L"U:"){

    }

    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    {
        // center the dialog on the screen
        CenterWindow();

        // set icons
        HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
        SetIcon(hIcon, TRUE);
        HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
        SetIcon(hIconSmall, FALSE);

        m_lsDirectories.Attach(GetDlgItem(IDC_LIST_DIRECTORY));
        m_lsDirectories.AddString(L"\\\\localhost\\d$\\Temp");
        m_lsDirectories.AddString(L"\\\\localhost\\c$\\Temp");
        m_lsDirectories.AddString(L"\\\\localhost\\e$\\Temp");

        m_mappedDrive.Attach(GetDlgItem(IDC_EDIT_MAPPED_DRIVE));
        CString conn = m_nwDrive.GetConnection();
        if (!conn.IsEmpty()){
            m_mappedDrive.SetWindowText(conn.GetBuffer());

            for (int i = 0, len = m_lsDirectories.GetCount(); i < len; ++i)
            {
                CString dir;
                m_lsDirectories.GetText(i, dir);

                if (0 == dir.CollateNoCase(conn)){
                    m_lsDirectories.SetCurSel(i);
                    break;
                }
            }
        }

        m_editDirectory.Attach(GetDlgItem(IDC_EDIT_DIRECTORY));

        return TRUE;
    }

    LRESULT OnUnMap(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
        unMap();

        return 0;
    }

    LRESULT OnAdd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
        CString szDirectory;
        m_editDirectory.GetWindowText(szDirectory);
        if (szDirectory.IsEmpty())
            return 0;

        m_lsDirectories.AddString(szDirectory);
        return 0;
    }

    LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
        EndDialog(wID);
        return 0;
    }

    bool isDirectoryMapped(CString szDir){
        CString szConn = m_nwDrive.GetConnection();
        return (0 == szConn.CollateNoCase(szDir));
    }

    CString getListBoxCurText()
    {
        CString szDir;
        int sel = m_lsDirectories.GetCurSel();
        m_lsDirectories.GetText(sel, szDir);
        return szDir;
    }

    void unMap()
    {
        CString conn = m_nwDrive.GetConnection();
        if (!conn.IsEmpty()){
            m_nwDrive.CancelConnection();
        }

        conn = m_nwDrive.GetConnection();
        m_mappedDrive.SetWindowText(conn);
    }

    void map(CString szDir)
    {
        m_nwDrive.AddConnection(szDir.GetBuffer());
        CString conn = m_nwDrive.GetConnection();
        m_mappedDrive.SetWindowText(conn);
    }

    LRESULT OnContextMenu(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
    {
        if (m_lsDirectories.m_hWnd != (HWND)wParam)
            return 0;

        CMenu menu;
        menu.LoadMenu(IDR_MENU_CONTEXT);
        CMenuHandle menuPopup = menu.GetSubMenu(0);

        CString szDir = getListBoxCurText();
        if (isDirectoryMapped(szDir))
            menuPopup.ModifyMenu(ID_MAP_MAP, MF_BYCOMMAND | MF_STRING, ID_MAP_MAP, L"UnMap");

        menuPopup.TrackPopupMenu(TPM_RIGHTBUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), m_hWnd, 0);

        return 0;
    }

    LRESULT OnDelete(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
        int sel = m_lsDirectories.GetCurSel();
        m_lsDirectories.DeleteString(sel);
        return 0;
    }

    LRESULT OnMap(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
        CString szDir = getListBoxCurText();
        if (isDirectoryMapped(szDir)){
            unMap();
        }
        else{
            map(szDir);
        }

        return 0;
    }
};
