// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Utils.h"

class CMainDlg : public CDialogImpl<CMainDlg>
{
public:
	enum { IDD = IDD_MAINDLG };

	CComboBox m_SubDirs;
	CEdit     m_RootDir;
	CEdit     m_mappedDrive;
	CString   m_szDrive;

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_HANDLER(IDC_COMBO_SUBDIRS, CBN_SELCHANGE, OnComboSelChange)
		COMMAND_ID_HANDLER(IDC_BTN_UNMAP, OnUnMap)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// center the dialog on the screen
		CenterWindow();

		// set icons
		HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
		SetIcon(hIcon, TRUE);
		HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
		SetIcon(hIconSmall, FALSE);
		
		m_szDrive = L"Z:";

		m_RootDir.Attach(GetDlgItem(IDC_EDIT_ROOTDIR));
		m_RootDir.SetWindowText(L"\\\\localhost\\D$\\Temp");
		m_SubDirs.Attach(GetDlgItem(IDC_COMBO_SUBDIRS));

		m_mappedDrive.Attach(GetDlgItem(IDC_EDIT_MAPPED_DRIVE));
		CString conn = GetConnection(m_szDrive.GetBuffer());
		if (!conn.IsEmpty())
			m_mappedDrive.SetWindowText(conn.GetBuffer());
		
		return TRUE;
	}

	void clearSubDirs()
	{
		for (int i = m_SubDirs.GetCount() - 1; i >= 0; i--) {
			m_SubDirs.DeleteString(i);
		}
	}

	CString getRootDir()
	{
		CString szRootDir;
		m_RootDir.GetWindowText(szRootDir);
		szRootDir.Trim();
		int length = szRootDir.GetLength();
		if (length == 0)
			return L"";

		if (szRootDir[length - 1] == L'\\'){
			szRootDir.Delete(length-1, 1);
		}

		return szRootDir;
	}

	void populateSubDirs(CString& szRootDir)
	{
		if (isValidDir(szRootDir.GetBuffer()))
		{
			std::vector<CString> subdirs;
			CString rootDir = szRootDir + "\\*";

			getSubDirs(rootDir.GetBuffer(), subdirs);

			for (size_t i = 0, nCount = subdirs.size(); i < nCount; ++i){
				const CString& dir = subdirs[i];
				m_SubDirs.AddString(dir);
				m_SubDirs.SetItemDataPtr(i, new CString(szRootDir + "\\" + dir));
			}
		}
	}

	LRESULT OnComboSelChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CString sz;
		CString *p = (CString *)m_SubDirs.GetItemDataPtr(m_SubDirs.GetCurSel());
		sz.Format(L"Current seletion is %d string=%s", m_SubDirs.GetCurSel(), p->GetBuffer());

		AddConnection(p->GetBuffer(), m_szDrive.GetBuffer());
		CString conn = GetConnection(m_szDrive.GetBuffer());
		m_mappedDrive.SetWindowText(conn);

		return 0;
	}

	LRESULT OnUnMap(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CString conn = GetConnection(m_szDrive.GetBuffer());
		if (!conn.IsEmpty()){
			CancelConnection(m_szDrive.GetBuffer());
		}

		conn = GetConnection(m_szDrive.GetBuffer());
		m_mappedDrive.SetWindowText(conn);

		return 0;
	}

	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		clearSubDirs();
		
		CString szRootDir = getRootDir();
		if (szRootDir.IsEmpty())
			return 0;

		populateSubDirs(szRootDir);

		return 0;
	}

	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		EndDialog(wID);
		return 0;
	}
};
