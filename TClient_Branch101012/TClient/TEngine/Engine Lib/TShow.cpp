// TShow.cpp: implementation of the CTShow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

BYTE CTShow::m_bMasterVolume = VOLUME_MAX;
BYTE CTShow::m_bON = TRUE;


CTShow::CTShow()
{
	m_strFILE.Empty();
	m_dwMessage = 0;
	m_pHost = NULL;
	m_pWnd = NULL;

	m_bVolume = VOLUME_MAX;
	m_pGB = NULL;
	m_pMS = NULL;
	m_pMC = NULL;
	m_pME = NULL;
	m_pVW = NULL;
}

CTShow::~CTShow()
{
	Release();
}

void CTShow::InitDShow( CWnd *pHost,
					    CWnd *pWnd,
						LPTSTR szFile,
						DWORD dwMessage)
{
	if(!szFile)
		return;

	if(pWnd)
		pWnd->ModifyStyle( 0, WS_CLIPCHILDREN);

	m_strFILE.Format( _T("%s"), szFile);
	m_dwMessage = dwMessage;
	m_pHost = pHost;
	m_pWnd = pWnd;
}

BYTE CTShow::ResetGB()
{
	Release();

	if( FAILED(CoCreateInstance(
		CLSID_FilterGraph,
		NULL,
		CLSCTX_INPROC,
		IID_IGraphBuilder,
		(void **) &m_pGB)))
		return FALSE;

	if( FAILED(m_pGB->QueryInterface(
		IID_IMediaControl,
		(void **) &m_pMC)))
		return FALSE;

	if( FAILED(m_pGB->QueryInterface(
		IID_IMediaSeeking,
		(void **) &m_pMS)))
		return FALSE;

	if( FAILED(m_pGB->QueryInterface(
		IID_IVideoWindow,
		(void **) &m_pVW)))
		return FALSE;

	if( FAILED(m_pGB->QueryInterface(
		IID_IMediaEventEx,
		(void **) &m_pME)))
		return FALSE;

	return TRUE;
}

void CTShow::Release()
{
	if(m_pME)
		m_pME->SetNotifyWindow( (OAHWND) NULL, 0, 0);

	if(m_pVW)
	{
		m_pVW->put_Visible(OAFALSE);
		m_pVW->put_Owner((OAHWND) NULL);
	}

	if(m_pMC)
	{
		m_pMC->Stop();

		m_pMC->Release();
		m_pMC = NULL;
	}

	if(m_pMS)
	{
		m_pMS->Release();
		m_pMS = NULL;
	}

	if(m_pVW)
	{
		m_pVW->Release();
		m_pVW = NULL;
	}

	if(m_pME)
	{
		m_pME->Release();
		m_pME = NULL;
	}

	if(m_pGB)
	{
		m_pGB->Release();
		m_pGB = NULL;
	}
}

void CTShow::OnWindowPosition()
{
	if( m_pVW && m_pWnd )
	{
		CRect rect;

		m_pWnd->GetClientRect(&rect);
		m_pVW->SetWindowPosition(
			rect.left,
			rect.top,
			rect.Width(),
			rect.Height());
	}
}

BYTE CTShow::ResetVolume()
{
	IBasicAudio *pBA = NULL;
	LONG nCurrVolume;

	if(!m_pGB)
		return FALSE;

	if( FAILED(m_pGB->QueryInterface(
		IID_IBasicAudio,
		(void **) &pBA)))
		return FALSE;

	if(FAILED(pBA->get_Volume(&nCurrVolume)))
	{
		pBA->Release();
		return FALSE;
	}

	FLOAT fVolume = CTachyonMedia::m_bBACK ? 0.0f : FLOAT(CTachyonMedia::m_bMasterVolume) * FLOAT(m_bMasterVolume) * FLOAT(m_bVolume) / FLOAT(VOLUME_MAX * VOLUME_MAX * VOLUME_MAX);
	LONG nVolume = DSBVOLUME_MIN + LONG(fVolume * FLOAT(DSBVOLUME_MAX - DSBVOLUME_MIN));

	pBA->put_Volume(nVolume);
	pBA->Release();

	return TRUE;
}

BYTE CTShow::SetPos( LONGLONG nPos)
{
	if( !m_pMS || !m_pMC )
		return FALSE;
	m_pMC->StopWhenReady();

	if( FAILED(m_pMS->SetPositions(
		&nPos,
		AM_SEEKING_AbsolutePositioning,
		NULL,
		AM_SEEKING_NoPositioning)))
		return FALSE;

	if(m_pWnd)
		UpdateWindow(m_pWnd->GetSafeHwnd());

	return TRUE;
}

LONGLONG CTShow::GetLength()
{
	if(!m_pMS)
		return 0;

	LONGLONG nResult;
	if(FAILED(m_pMS->GetStopPosition(&nResult)))
		return 0;

	return nResult;
}

LONGLONG CTShow::GetPos()
{
	if(!m_pMS)
		return 0;

	LONGLONG nResult;
	if(FAILED(m_pMS->GetCurrentPosition(&nResult)))
		return 0;

	return nResult;
}

BYTE CTShow::Toggle()
{
	static HWND hDrain = 0;
	LONG nMode;

	if( !m_pVW || !m_pWnd )
		return FALSE;

	if(FAILED(m_pVW->get_FullScreenMode(&nMode)))
		return FALSE;

	if( nMode == OAFALSE )
	{
		m_pVW->get_MessageDrain((OAHWND *) &hDrain);
		m_pVW->put_MessageDrain((OAHWND) m_pWnd->GetSafeHwnd());

		nMode = OATRUE;
		if(FAILED(m_pVW->put_FullScreenMode(nMode)))
			return FALSE;
	}
	else
	{
		nMode = OAFALSE;

		if(FAILED(m_pVW->put_FullScreenMode(nMode)))
			return FALSE;

		m_pVW->put_MessageDrain((OAHWND) hDrain);
		m_pVW->SetWindowForeground(-1);

		UpdateWindow(m_pWnd->GetSafeHwnd());
	}

	return TRUE;
}

BYTE CTShow::Pause()
{
	if(!m_pMC)
		return FALSE;

	if(FAILED(m_pMC->Pause()))
		return FALSE;

	return TRUE;
}

BYTE CTShow::Play()
{
	if( !CTachyonMedia::m_bON || !m_bON )
		return FALSE;

	USES_CONVERSION;
	ResetGB();

	if(!m_pMC)
		return FALSE;

	if(FAILED(m_pGB->RenderFile( T2W(LPCSTR(m_strFILE)), NULL)))
		return FALSE;

	if(m_pWnd)
	{
		m_pVW->put_MessageDrain((OAHWND) m_pWnd->GetSafeHwnd());
		m_pVW->put_WindowStyle(WS_CHILD);

		m_pVW->put_Owner((OAHWND) m_pWnd->GetSafeHwnd());
		m_pVW->put_Visible(OATRUE);

		UpdateWindow(m_pWnd->GetSafeHwnd());
	}

	if(m_pHost)
		m_pME->SetNotifyWindow( (OAHWND) m_pHost->GetSafeHwnd(), m_dwMessage, (LONG_PTR) m_pME);
	ResetVolume();

	if(FAILED(m_pMC->Run()))
		return FALSE;

	return TRUE;
}

BYTE CTShow::Stop()
{
	if(!m_pMC)
		return FALSE;

	if(FAILED(m_pMC->Stop()))
		return FALSE;

	LONGLONG nPos = 0;
	OAFilterState fs;

	m_pMC->GetState( 500, &fs);
	m_pMS->SetPositions(
		&nPos,
		AM_SEEKING_AbsolutePositioning,
		NULL,
		AM_SEEKING_NoPositioning);
	m_pMC->StopWhenReady();

	if(m_pWnd)
		UpdateWindow(m_pWnd->GetSafeHwnd());

	return TRUE;
}

BYTE CTShow::IsPlay()
{
	OAFilterState state;

	if(!m_pGB)
		return FALSE;

	m_pMC->GetState( INFINITE, &state);
	return state == State_Running && GetPos() < GetLength() ? TRUE : FALSE;
}
