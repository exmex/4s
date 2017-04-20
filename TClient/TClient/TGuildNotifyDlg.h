#pragma once

class CTGuildNotifyDlg : public CTClientUIBase
{
public:
	enum { BUTTON_COUNT = 3 };

	enum COLUMN
	{
		COL_TITLE,			///< 공지 제목
		COL_DATE,			///< 공지 작성일
		COL_SELECTION,		///< 공지 전체 선택
		COL_DELETE,			///< 공지 삭제 버튼
		COL_COUNT
	};

	enum MODE
	{
		MODE_NORMAL,		///< 일반 모드 (공지를 보는것만 가능함)
		MODE_MASTER,		///< 길드장 모드 (공지 수정 가능)
		MODE_NEW,			///< 새공지 작성
		MODE_EDIT,			///< 공지 수정
		MODE_COUNT
	};

	typedef CTGuildCommander::GuildNotify		GuildNotify;
	typedef CTGuildCommander::GuildNotifyVec	GuildNotifyVec;

public:
	static const BUTTON_STATE	MODE_BTN_VIEW[MODE_COUNT][BUTTON_COUNT];

protected:
	MODE			m_eMode;
	INT				m_nPrvSel;
	INT				m_nModIdx;

	TList*			m_pList;
	TComponent*		m_pViewAuthor;
	
	TComponent*		m_pViewTitle;
	TEdit*			m_pEditTitle;
	
	TMultiLineEdit*	m_pEditContents;
	TList*			m_pViewContents;

	TComponent*		m_pBTN[BUTTON_COUNT];

public:
	/// 공지 목록을 최신의 정보로 갱신한다.
	void UpdateInfo();

	/// 현재 설정을 기본 모드로 초기화 한다. 기본 모드는 길드장 여부에 따라 다르게 결정된다.
	void ResetMode();
	/// 모드에 따라 컴포넌트를 다시 갱신한다.
	void UpdateCompByMode();

	/// 새공지 작성을 시작한다.
	void StartNew();
	/// 새공지 작성을 끝낸다.
	void EndNew(BOOL bOK);

	/// 공지 수정을 시작한다.
	void StartModify();
	/// 공지 수정을 끝낸다.
	void EndModify(BOOL bOK);

	/// 공지를 제거한다.
	void Delete();

	/// 편집 모드인지 여부를 얻는다.
	BOOL IsEditMode();
	/// 현재 포커스된 에디트 콘트롤을 얻는다.
	TEdit* GetCurEdit();

protected:
	/// 현재 선택된 공지사항을 반영한다.
	void UpdateByListSel();

public:
	virtual void OnLButtonDown(UINT nFlags, CPoint pt);
	virtual void OnKeyDown(UINT nChar, int nRepCnt, UINT nFlags);
	virtual void ShowComponent( BOOL bVisible = TRUE);
	virtual HRESULT Render( DWORD dwTickCount );

public:
	CTGuildNotifyDlg( TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTGuildNotifyDlg();
};