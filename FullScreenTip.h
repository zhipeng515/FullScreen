#pragma once

#include "AppBar.h"

class CFullScreenTip : public ATL::CWindowImpl<CFullScreenTip>,
	public CAppBar<CFullScreenTip>
{
private:
	HWND contentWnd_;

public:

	CFullScreenTip(HWND hTipWnd) : contentWnd_(hTipWnd)
	{

	}

	BEGIN_MSG_MAP(CFullScreenTip)            //œ˚œ¢”≥…‰
		CHAIN_MSG_MAP(CAppBar<CFullScreenTip>)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		CRect rcWnd;
		if (::IsWindow(contentWnd_))
		{
			::GetWindowRect(contentWnd_, rcWnd);
			::SetParent(contentWnd_, m_hWnd);
			::SetWindowPos(contentWnd_, NULL, 0, 0, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
			::ShowWindow(contentWnd_, SW_SHOW);

			SetWindowPos(NULL, -1, -1, rcWnd.Width(), rcWnd.Height(), SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
		}

		CenterWindow();
		GetWindowRect(rcWnd);
		InitAppBar(APPBAR_DOCKING_TOP, true, true);
		DockAppBar(APPBAR_DOCKING_TOP, 5000);

		SetWindowPos(HWND_TOPMOST, rcWnd.left, 0, -1, -1, SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);

		bHandled = TRUE;

		return 1;
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		if (::IsWindow(contentWnd_))
		{
			::ShowWindow(contentWnd_, SW_HIDE);
			::SetParent(contentWnd_, NULL);
		}

		return 1;
	}
};