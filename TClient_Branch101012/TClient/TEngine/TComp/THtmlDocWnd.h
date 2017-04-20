#pragma once


#define TUSER32_BEGINPAINT			_T("BeginPaint")
#define TUSER32_ENDPAINT			_T("EndPaint")
#define TUSER32_DLL					_T("USER32.DLL")

#define TASM_JUMP					((BYTE) 0xE9)
#define TASM_BYTE					(5)

#define TCARET_HEIGHT				(13)
#define TCARET_WIDTH				(1)
#define TCARET_TICK					(100)


typedef enum TPAINT_API
{
	TPAINT_API_BEGINPAINT = 0,
	TPAINT_API_ENDPAIND,
	TPAINT_API_COUNT
} *LPTPAINT_API;

typedef struct tagTASM_CODE
{
	LPBYTE m_pTAPI;

	BYTE m_vTBUF[TASM_BYTE];
	BYTE m_bTSET;

	tagTASM_CODE()
	{
		ZeroMemory( m_vTBUF, TASM_BYTE);
		m_pTAPI = NULL;

		m_bTSET = FALSE;
	};
} TASM_CODE, *LPTASM_CODE;


// THtmlDocWnd

class THtmlDocWnd : public CWnd
{
	DECLARE_DYNAMIC(THtmlDocWnd)
	friend class THttpCtrl;

protected:
	static void UnhookTAPI( LPTASM_CODE pTCODE);
	static BYTE HookTAPI(
		LPTASM_CODE pTCODE,
		LPVOID pTAPI);

protected:
	static TASM_CODE m_vTAPI[TPAINT_API_COUNT];

public:
	static BOOL CALLBACK __HookDocWnd(
		HWND hWnd,
		LPARAM lParam);

	static BOOL CALLBACK __PutReady(
		HWND hWnd,
		LPARAM lParam);

	static BOOL CALLBACK __RedrawWnd(
		HWND hWnd,
		LPARAM lParam);

	static void ReleaseHook();
	static BYTE InitHook();

public:
	THtmlDocWnd();
	virtual ~THtmlDocWnd();

public:
	BYTE CanFocus();

protected:
	BYTE OnMouseMSG(
		UINT nMSG,
		UINT nFlags,
		CPoint point);

protected:
	DWORD_PTR m_pMAINPROC;
	DWORD_PTR m_pWINPROC;

	THttpCtrl *m_pTHOST;
	CWnd *m_pMainWnd;
	BYTE m_bReady;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};
