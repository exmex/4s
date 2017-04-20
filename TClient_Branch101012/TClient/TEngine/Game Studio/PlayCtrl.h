#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "CtrlDef.h"


#define MM_NONE				((BYTE) 0x00)
#define MM_MOVE				((BYTE) 0x01)
#define MM_COPY				((BYTE) 0x02)


typedef vector<CWnd *>		VECTORPLAYCTRL;
typedef vector<int>			VECTORMARKER;

class CMarkCtrl : public CStatic
{
protected:
	VECTORMARKER m_vMarker;
	BYTE m_bMode;

	int m_nIndex;
	int m_nFrom;

public:
	CSliderCtrl *m_pPlayCtrl;
	CWnd *m_pHandler;

	DWORD m_dwMoveID;
	DWORD m_dwCopyID;

public:
	void SetRange( int nMin, int nMax);
	void AddMark( int nPos);
	void DelMark( int nPos);
	void DelAll();

	int HitPosTest( CPoint point);
	int HitTest( CPoint point);

public:
	CMarkCtrl();
	virtual ~CMarkCtrl();

public:
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
};


// CPlayCtrl 대화 상자입니다.

class CPlayCtrl : public CDialog
{
	DECLARE_REGISTER_WINDOW(CPlayCtrl)

public:
	CPlayCtrl(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPlayCtrl();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_PLAYCTRL };

public:
	static void ReleasePlayCtrl();
	static void InitPlayCtrl();
	static void DrawProc();

	static CFont *GetFont() { return &m_Font;};

public:
	static VECTORPLAYCTRL m_vPlayCtrl;

protected:
	static CFont m_Font;

public:
	void SetRange( int nMin, int nMax);
	void SetPos( int nPos);

	void Play();
	void Stop();

	void EnableCtrl( BOOL bEnable);
	void InitHandler(
		CWnd *pHandler,
		DWORD dwProcID,
		DWORD dwPlayID,
		DWORD dwStopID,
		DWORD dwPosID);

protected:
	DWORD m_dwProcID;
	DWORD m_dwPlayID;
	DWORD m_dwStopID;
	DWORD m_dwPosID;

	CWnd *m_pHandler;

protected:
	void ResetPosition();
	void OnPosChanged();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_cPlayCtrl;
	CMarkCtrl m_cMarker;

	CButton m_cPlayBtn;
	CButton m_cStopBtn;
	CButton m_cLoop;
	CStatic m_cTic;

	DWORD m_dwBeginTick;
	DWORD m_dwTickCount;

	BYTE m_bPlay;
	BOOL m_bLoop;

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButtonPlay();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedCheckLoop();
	afx_msg void OnDestroy();
};
