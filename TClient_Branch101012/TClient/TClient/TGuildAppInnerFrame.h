#pragma once

class CTGuildAppInnerFrame : public ITInnerFrame
{
public:
	class CSelecter
	{
	public:
		TButton*		m_pLB;
		TButton*		m_pRB;
		TComponent*		m_pTXT;
		
		INT				m_nCUR;
		VECTORSTRING	m_vTXTS;
		VECTORDWORD		m_vDATAS;

		BOOL			m_bEnable;

	public:
		CSelecter(TButton* pLB, TButton* pRB, TComponent* pTXT);

	public:
		void AddTxt(DWORD dwID, DWORD dwDATA=0);
		void ClearTxt();
		
		void Move(BOOL bLeft);
		void SetCur(INT nIndex);
		INT  GetCur();
		DWORD GetData(INT nIndex);
		
		void SetEnable(BOOL bEnable);
		BOOL IsEnable();
		BOOL HitTest(CPoint pt);
	};

public:
	// !! 이하의 변수들은 Init() 에서 초기화 해주어야 함 !!
	//
	BOOL			m_bInit;		///< 초기화 여부

	UINT			m_nModeCnt;		///< 전체 모드의 수
	UINT			m_nCurMode;		///< 현재 모드

	TList*			m_pList;		///< 메인 리스트
	int				m_nPrvSel;		///< 이전 프레임의 선택 인덱스

	TMultiLineEdit*	m_pContentEdit;	///< 내용 입력 에디터
	TList*			m_pContentList; ///< 내용 출력 리스트

	CSelecter*		m_pSelecter;	///< 분야 선택

public:
	/// 클래스 초기화를 진행한다. 이 함수에서 주요 멤버들이 설정된다.
	virtual void Init() = 0;

	/// 현재 설정을 기본 모드로 초기화 한다.
	virtual void ResetMode() = 0;
	/// 현재 설정된 모드에 따라 컴포넌트를 갱신한다.
	virtual void UpdateCompByMode() = 0;
	/// 현재 리스트에서 선택된 항목을 다른 컨트롤에 반영한다.
	virtual void UpdateByListSel() = 0;
	/// 에디트 모드 여부를 얻는다.
	virtual BOOL IsEditMode() = 0;
	/// 현재 포커스된 에디터를 얻는다.
	virtual TEdit* GetCurEdit() = 0;

	/// 요청된 정보로 컨트롤을 갱신한다.
	virtual void ResetInfo() = 0;
	/// 컨트롤 갱신을 위한 정보를 요청한다.
	virtual void RequestInfo() = 0;

protected:
	/// 내용에 주어진 문자열을 설정한다.
	void SetContent(const CString& strText);

public:
	virtual void OnKeyDown(UINT nChar, int nRepCnt, UINT nFlags);
	virtual void OnLButtonDown( UINT nFlags, CPoint pt );
	virtual void OnRButtonDown( UINT nFlags, CPoint pt );
	virtual void SwitchFocus( TComponent *pCandidate );
	virtual void ShowComponent( BOOL bVisible = TRUE );
	
	virtual HRESULT Render( DWORD dwTickCount );

public:
	CTGuildAppInnerFrame(TComponent *pParent, LP_FRAMEDESC pDesc, UINT nFrameID);
	virtual ~CTGuildAppInnerFrame();
}; 
