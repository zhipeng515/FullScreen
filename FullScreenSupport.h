#pragma once

#include "FullScreenTip.h"

template <class T, class C = CWindow>
class FullScreenSupport
{
public:
	BEGIN_MSG_MAP(FullScreenSupport)
		MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	END_MSG_MAP()

	FullScreenSupport()
	{
		m_bFullScreen = false;
		m_pFullScreenTip = NULL;
		m_pClient = NULL;
	}
	~FullScreenSupport()
	{
	}

	void SetFullScreenClient(C* client)
	{
		_ASSERT(client != NULL);
		m_pClient = client;
	}
	void SetFullScreenTip(HWND hTipWnd)
	{
		m_hTipContentWnd = hTipWnd;
	}

	void ExitFullScreen()
	{
		if (m_bFullScreen)
		{
			HideFullScreenTip();
			//CWindow ShellTrayWnd = ::FindWindow(_T("Shell_TrayWnd"), NULL);
			//ShellTrayWnd.ShowWindow(SW_SHOW);
			m_bFullScreen = FALSE;

			T* pT = static_cast<T*>(this);
			// 推出全屏显示,恢复原始窗口显示
			pT->ShowWindow(SW_HIDE);
			pT->SetWindowPlacement(&m_OldWndPlacement);
			pT->SetActiveWindow();

		}
	}

	void EnterFullScreen()
	{
		if (m_pClient == NULL)
			return;
		// TODO: Add your command handler code here
		m_bFullScreen = TRUE;       // 设置全屏显示标志为TURE`

		T* pT = static_cast<T*>(this);
		pT->GetWindowPlacement(&m_OldWndPlacement);
		RECT WindowRect;
		pT->GetWindowRect(&WindowRect);
		RECT ClientRect;
		m_pClient->GetClientRect(&ClientRect);
		m_pClient->ClientToScreen(&ClientRect);

		// 获取屏幕的分辨率
		int nFullWidth = GetSystemMetrics(SM_CXSCREEN);
		int nFullHeight = GetSystemMetrics(SM_CYSCREEN);
		// 将出控制条外的客户区全屏显示到从(0,0)到(nFullWidth,nFullHeight)区域
		// 将(0,0)和(nFullWidth,nFullHeight)两个点外扩充原窗口和控制条之外的
		// 客户区位置间的差值，就得到全屏显示的窗口位置
		m_FullScreenRect.left = WindowRect.left - ClientRect.left;
		m_FullScreenRect.top = WindowRect.top - ClientRect.top;
		m_FullScreenRect.right = WindowRect.right - ClientRect.right + nFullWidth;
		m_FullScreenRect.bottom = WindowRect.bottom - ClientRect.bottom + nFullHeight;

		WINDOWPLACEMENT wndpl;
		wndpl.length = sizeof(WINDOWPLACEMENT);
		wndpl.flags = 0;
		wndpl.showCmd = SW_SHOWNORMAL;
		wndpl.rcNormalPosition = m_FullScreenRect;
		pT->SetWindowPlacement(&wndpl);

		//CWindow ShellTrayWnd = ::FindWindow(_T("Shell_TrayWnd"), NULL);
		//ShellTrayWnd.ShowWindow(SW_HIDE);
		ShowFullScreenTip();

		pT->SetActiveWindow();
	}

	bool IsFullScreent()
	{
		return m_bFullScreen;
	}

	void SwitchFullScreen()
	{
		if (m_bFullScreen)
			ExitFullScreen();
		else
			EnterFullScreen();
	}

	void ShowFullScreenTip()
	{
		T* pT = static_cast<T*>(this);

		RECT rect;
		SetRect(&rect, 0, 0, 10, 10);
		m_pFullScreenTip = new CFullScreenTip(m_hTipContentWnd);
		m_pFullScreenTip->Create(pT->m_hWnd, rect, _T("FullScreenTip"), WS_POPUP );
	}

	void HideFullScreenTip()
	{
		if (m_pFullScreenTip)
		{
			m_pFullScreenTip->DestroyWindow();
			delete m_pFullScreenTip;
			m_pFullScreenTip = NULL;
		}
	}

	LRESULT OnGetMinMaxInfo(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
	{
		// TODO: Add your message handler code here and/or call default
		bHandled = FALSE;
		MINMAXINFO FAR* lpMMI = (MINMAXINFO FAR*)lParam;
		if (m_bFullScreen)
		{
			int nWidth = m_FullScreenRect.right - m_FullScreenRect.left;
			int nHeight = m_FullScreenRect.bottom - m_FullScreenRect.top;
			lpMMI->ptMaxSize.x = nWidth;
			lpMMI->ptMaxSize.y = nHeight;
			lpMMI->ptMaxPosition.x = m_FullScreenRect.left;
			lpMMI->ptMaxPosition.y = m_FullScreenRect.top;
			//最大的Track尺寸也要改变
			lpMMI->ptMaxTrackSize.x = nWidth;
			lpMMI->ptMaxTrackSize.y = nHeight;

			bHandled = TRUE;
		}
		return 0;
	}

private:
	bool					m_bFullScreen;		// 全屏显示标志
	WINDOWPLACEMENT			m_OldWndPlacement;	// 用来保存原窗口的位置
	RECT					m_FullScreenRect;	// 表示全屏显示时的窗口位置
	CFullScreenTip *		m_pFullScreenTip;
	C *						m_pClient;
	HWND					m_hTipContentWnd;
};

