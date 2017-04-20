// MainFrm.h : CMainFrame 클래스의 인터페이스
//
#pragma once



class CMainFrame : public CFrameWnd
{
	
protected: // serialization에서만 만들어집니다.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

	// 특성
public:
	CSplitterWnd		m_SplitWnd[1];
	CTFontDialogView*	m_pFontDialog;
	CTFontTestView*		m_pFontView;

public:
	static LPDIRECT3DSWAPCHAIN9 m_pSWAPCHAIN;
	static CD3DDevice m_3DDevice;
	static BYTE m_bRender;
	static FLOAT m_fFOV;	// Perspective 창의 FOV 값

// 작업
public:
	void				ResizeSplitWnd();		// 리사이즈 SplitWnd

public:
	static void RestoreDevice();
	static void ReleaseDevice();
	static void InitDevice();

	static BOOL LoadImageFile(const CString& strFILE, CD3DImage* pIMG);


	void	SetFont( LOGFONT* of, BOOL bOutline, BOOL bShadow, DWORD dwColor, INT nFontRender, CRect rt );
	void	SetText( CString str );

	void	LoadImage( CString str );
	//static void	SetFont();
	//static void	SetFont();

	



// 재정의
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 컨트롤 모음이 포함된 멤버입니다.
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// 메시지 맵 함수를 생성했습니다.
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
};


