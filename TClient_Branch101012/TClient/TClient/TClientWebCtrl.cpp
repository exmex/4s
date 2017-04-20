// TClientWebCtrl.cpp : 구현 파일입니다.
//

#include "stdafx.h"


// CTClientWebCtrl

IMPLEMENT_DYNAMIC(CTClientWebCtrl, CWnd)
CTClientWebCtrl::CTClientWebCtrl()
{
	m_pActHost = NULL;
	m_pWebCtrl = NULL;
}

CTClientWebCtrl::~CTClientWebCtrl()
{
	if(m_pActHost)
	{
		m_pActHost->SetForegroundWindow();
		m_pActHost->ShowWindow(SW_NORMAL);
	}

	if(m_pWebCtrl)
	{
		m_pWebCtrl->Release();
		m_pWebCtrl = NULL;
	}
}


BEGIN_MESSAGE_MAP(CTClientWebCtrl, CWnd)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


BYTE CTClientWebCtrl::Navigate( CString strURL,
							    int nLeft,
								int nTop,
								int nWidth,
								int nHeight,
								BYTE bToolBar,
								BYTE bMenuBar,
								BYTE bStatusBar,
								BYTE bResizable,
								BYTE bMinimizeHost,
								HWND hPOS)
{
	if(!m_pWebCtrl)
	{
		HRESULT hr = CoCreateInstance(
			CLSID_InternetExplorer,
			NULL,
			CLSCTX_LOCAL_SERVER,
			IID_IWebBrowser2,
			(LPVOID *) &m_pWebCtrl);

		if(FAILED(hr))
		{
			CWnd *pActHost = m_pActHost;

			if(GetSafeHwnd())
				DestroyWindow();
			m_pActHost = pActHost;

			return FALSE;
		}
	}

	if( nLeft || nTop || nWidth || nHeight )
	{
		m_pWebCtrl->put_Left(nLeft);
		m_pWebCtrl->put_Top(nTop);
		m_pWebCtrl->put_Width(nWidth);
		m_pWebCtrl->put_Height(nHeight);
	}

	m_pWebCtrl->put_ToolBar(bToolBar);
	m_pWebCtrl->put_MenuBar(bMenuBar);
	m_pWebCtrl->put_StatusBar(bStatusBar);
	m_pWebCtrl->put_Resizable(bResizable);

	m_pWebCtrl->put_Visible(TRUE);
	if( m_pActHost && bMinimizeHost )
		m_pActHost->ShowWindow(SW_MINIMIZE);

	HWND hWebCtrl;
	m_pWebCtrl->get_HWND((SHANDLE_PTR *) &hWebCtrl);

	if( !hWebCtrl || !Create() )
	{
		CWnd *pActHost = m_pActHost;

		m_pWebCtrl->Release();
		m_pWebCtrl = NULL;
		m_pActHost = NULL;

		if(GetSafeHwnd())
			DestroyWindow();
		m_pActHost = pActHost;

		return FALSE;
	}

	::SetWindowPos( hWebCtrl, hPOS, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	::SetForegroundWindow(hWebCtrl);

	CComVariant vARG, vURL(strURL.GetBuffer());
	m_pWebCtrl->Navigate2( &vURL, &vARG, &vARG, &vARG, &vARG);

	return TRUE;
}

// CTClientWebCtrl 메시지 처리기입니다.

BOOL CTClientWebCtrl::Create()
{
	if( !m_pWebCtrl || !m_pActHost )
		return FALSE;

	HWND hParent;
	m_pWebCtrl->get_HWND((SHANDLE_PTR *) &hParent);

	if(!hParent)
		return FALSE;

	if(GetSafeHwnd())
	{
		if( ::GetParent(GetSafeHwnd()) != hParent )
			::SetParent( GetSafeHwnd(), hParent);

		return TRUE;
	}

	return CWnd::CreateEx( 0, NULL, NULL, WS_CHILD, -20, -20, 10, 10, hParent, NULL);
}

void CTClientWebCtrl::OnDestroy()
{
	if(m_pActHost)
	{
		m_pActHost->SetForegroundWindow();
		m_pActHost->ShowWindow(SW_NORMAL);
	}

	if(m_pWebCtrl)
	{
		m_pWebCtrl->Release();
		m_pWebCtrl = NULL;
	}

	CWnd::OnDestroy();
}
