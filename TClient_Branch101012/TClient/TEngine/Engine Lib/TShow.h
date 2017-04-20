// TShow.h: interface for the CDShow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined __TSHOW_H
#define __TSHOW_H


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTShow
{
public:
	static BYTE m_bMasterVolume;
	static BYTE m_bON;

protected:
	IGraphBuilder* m_pGB;
	IMediaSeeking* m_pMS;
	IMediaControl* m_pMC;
	IMediaEventEx* m_pME;
	IVideoWindow* m_pVW;

	CWnd *m_pHost;
	CWnd *m_pWnd;

public:
	CString m_strFILE;
	DWORD m_dwMessage;
	BYTE m_bVolume;

public:
	BYTE SetPos( LONGLONG nPos);

	LONGLONG GetLength();
	LONGLONG GetPos();

	BYTE ResetVolume();
	BYTE ResetGB();
	BYTE IsPlay();
	BYTE Toggle();
	BYTE Pause();
	BYTE Play();
	BYTE Stop();

public:
	void InitDShow(
		CWnd *pHost,
		CWnd *pWnd,
		LPTSTR szFile,
		DWORD dwMessage);

	void OnWindowPosition();
	void Release();

public:
	CTShow();
	virtual ~CTShow();
};


#endif // !defined __TSHOW_H
