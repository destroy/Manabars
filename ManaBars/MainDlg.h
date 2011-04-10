// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "fun.h"
#include <shellapi.h>
#include <atlmisc.h>
#include <atlctrlx.h>

class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
		public CMessageFilter, public CIdleHandler
{
public:
	enum { IDD = IDD_MAINDLG };

    ////////////////////////////////////////////////////////////////////////// tray icon
    NOTIFYICONDATA m_nid;
    const UINT WM_TASKBARCREATED;   // for explorer recreate
    static const UINT WM_TRAY_MSG = 0x443;
    bool m_bFirstShow;
    CHyperLink m_link;
    CMainDlg()
     : WM_TASKBARCREATED(::RegisterWindowMessage(_T("TaskbarCreated")))
     , m_bFirstShow(false)
     , m_link()
    {
         m_link.m_dwExtendedStyle = HLINK_UNDERLINEHOVER ;

    }

    LRESULT OnTrayMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) 
    {
        switch (lParam)
        {
        case WM_LBUTTONDOWN:
        
            m_bFirstShow = true;
            SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
            ShowWindow(SW_SHOW);
            SetForegroundWindow(m_hWnd);
            break;
        case WM_RBUTTONDOWN:
            CMenu menu;
            menu.LoadMenu(IDR_MENU1);
            CMenuHandle ppopup = menu.GetSubMenu(0);
            SetForegroundWindow(m_hWnd);
            CPoint pt;
            GetCursorPos(&pt);
            ppopup.TrackPopupMenu(TPM_RIGHTBUTTON, pt.x, pt.y, m_hWnd, 0);
            menu.DestroyMenu();
            break;
        }

        return 0;
    }
    LRESULT OnTaskbarCreated(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) 
    {
        Shell_NotifyIcon(NIM_ADD, &m_nid);
        return 0;
    }
    ////////////////////////////////////////////////////////////////////////// tray icon

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CWindow::IsDialogMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		return FALSE;
	}

    LRESULT OnSysCommand(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
    {
        if (!m_bFirstShow)
        {
            bHandled = FALSE;
            return 0;
        }
        if((wParam & 0xFFF0) == SC_MINIMIZE)
        {

            ShowWindow(SW_HIDE);
            bHandled = TRUE;
        }
        else
            bHandled = FALSE;
        return 0;
    }
    
    LRESULT OnWindowPosChanging(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        WINDOWPOS* pWP = (WINDOWPOS*)lParam;
        if (!m_bFirstShow)
        {
            if (pWP->flags & SWP_SHOWWINDOW)   
            {   
                pWP->flags &= ~SWP_SHOWWINDOW;  
                PostMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
                bHandled = true;
            }
        }
        else
        {
            pWP->flags |= SWP_SHOWWINDOW;
            bHandled = false;
        }
        return 0;
    }
	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        MESSAGE_HANDLER(WM_TASKBARCREATED, OnTaskbarCreated)
        MESSAGE_HANDLER(WM_TRAY_MSG, OnTrayMsg)
        MESSAGE_HANDLER(WM_SYSCOMMAND, OnSysCommand)
        MESSAGE_HANDLER(WM_WINDOWPOSCHANGING, OnWindowPosChanging)
        COMMAND_ID_HANDLER(ID_SS_EXIT, OnCancel)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

    static DWORD WINAPI ThreadProc(
                                             LPVOID lpParameter
                               )
    {
        CWar war;
        war.MoniterAndDowork();
        return 0;
    }

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
        __try
        {
            TCHAR szTmp[MAX_PATH];
            memset(szTmp, '_', sizeof(szTmp));
            this->GetWindowText(szTmp, MAX_PATH);
            int iLen = lstrlen(szTmp);
            *(szTmp + iLen) = '_';
            *(szTmp + iLen + GetTickCount() % (MAX_PATH - iLen)) = 0;
            SetWindowText(szTmp);
        }
        __except(1){}

		// center the dialog on the screen
		CenterWindow();
        m_link.SubclassWindow(GetDlgItem(IDC_STATIC_LINK));
        m_link.m_clrLink = RGB(0, 128, 0);
        //m_link.m_clrVisited = RGB(255, 255,0);
		// set icons
		HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
		SetIcon(hIcon, TRUE);
		HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
		SetIcon(hIconSmall , FALSE);

		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		UIAddChildWindowContainer(m_hWnd);

        // tray icon
        memset(&m_nid, 0, sizeof(m_nid));
        m_nid.cbSize=sizeof(NOTIFYICONDATA);
        m_nid.hWnd  = m_hWnd;
        m_nid.uFlags = NIF_ICON | NIF_MESSAGE;
        m_nid.uCallbackMessage = WM_TRAY_MSG;
        m_nid.hIcon = hIconSmall;
        Shell_NotifyIcon(NIM_ADD, &m_nid);
        DWORD dwThread;
        CloseHandle(CreateThread(NULL, 0, ThreadProc, this, 0, &dwThread));
		return TRUE;
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// unregister message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->RemoveMessageFilter(this);
		pLoop->RemoveIdleHandler(this);

        Shell_NotifyIcon(NIM_DELETE, &m_nid);

		return 0;
	}



	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: Add validation code 
        
        try
        {
            CWar war;
            war.Find();
            war.DoWork();
        }
        catch (std::exception& e)
        {
            MessageBox(e.what());
            return 0;
        }

        MessageBox(_T("¿ªÆô³É¹¦£¡"), _T("¹§Ï²"), MB_OK | MB_ICONINFORMATION);
		CloseDialog(wID);
		return 0;
	}

	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CloseDialog(wID);
		return 0;
	}

	void CloseDialog(int nVal)
	{
		DestroyWindow();
		::PostQuitMessage(nVal);
	}
};
