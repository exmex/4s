// TEdit.h: interface for the TEdit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEDIT_H__B581C2D6_C499_4588_A482_AFAFDB23AEDC__INCLUDED_)
#define AFX_TEDIT_H__B581C2D6_C499_4588_A482_AFAFDB23AEDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class TEdit : public TComponent  
{
public:
	enum CountryFlag
	{
		COUNTRY_FLAG_KOREA,
		COUNTRY_FLAG_POLAND,
		COUNTRY_FLAG_FRANCE,
		COUNTRY_FLAG_CZECH,
		COUNTRY_FLAG_ITALY,
		COUNTRY_FLAG_SPAIN,
		COUNTRY_FLAG_GREECE,
		COUNTRY_FLAG_RUSSIA,
		COUNTRY_FLAG_ROMANIA,
		COUNTRY_FLAG_HUNGARY,
		COUNTRY_FLAG_TURKEY,
		COUNTRY_FLAG_PORTUGAL,
	};

public:

	TEdit(TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~TEdit();

public:
	void SetPassword( BOOL bPassword );
	void SetLock( BOOL blLock );
	
	const CString& GetCompositionStr() const;

	virtual void ClearText();
	virtual void SetText(const CString& txt);
	
	virtual void MoveCaretToFront();
	virtual void MoveCaretToBack();

public:
	static CHAR SCodeToAscii(UINT nScanCode);
	static TCandidate* m_pCandidate;
	static TEdit* m_pCandidateOwner;
	static CRect m_rtCandidateTarget;
	static HWND g_hWnd;
	static UINT m_nCountryFlag;
	static BOOL g_bPassImeNotify;
	static HKL m_hKL;

protected:
	TComponent *m_pCaret;

	int			m_nCaret;
	int			m_nDisplayPos;
	int			m_nCompCaretOffset;
    BOOL		m_bConverting;

	BOOL		m_blIsLocked;
	BOOL		m_bProcDBCS;
	BOOL		m_bProcPASS;
	
	CString		m_strComposition;
	BOOL			m_bNeedUpdateComposition;
	BYTE			m_pCompAttr[512];
	DWORD		m_pCompClause[512];
	INT			m_nCompAttrLength;
	INT			m_nCompClauseLength;

	CString     m_strDisplayText;

	CString		m_strPrvOuputText;
	CPoint		m_ptPrvOuputText;
	INT			m_nPrvStartIndex;

public:
	virtual void InsertString(const CString& strText);
	virtual void InsertString(const CString& strText, DWORD dwColor, DWORD dwID);

	virtual void PushFrontString(const CString& strText);

protected:
	virtual void LeftCaret(BOOL bDouble);
	virtual void RightCaret(BOOL bDouble);
	virtual void RightCaretCount(UINT uCount);

	int HandleChar(TCHAR chCharCode, int nRepCnt, CString & strResult);

	int HandleImeComposition(
		HWND hWnd,
		WPARAM wParam,
		LPARAM lParam,
		CString & strComp,
		CString & strResult,
		BYTE* pbAttr,
		INT* pbAttrLength,
		DWORD* pbClause,
		INT* pbClauseLength);

	BOOL CheckCountryCharCode( TCHAR chCharCode );

	void RemoveCompTextSetting();
	void MoveTextSetting(INT iFrom, INT iCount);

	void ResetCompTextSetting(
		INT nCompStart,
		INT nCompEnd );

	void ResetTextSettingLine(
		const TextSetting& data,
		const CRect& rectScreenText,
		TextOutputData& sTextOutputData);

public://Render
	virtual HRESULT DrawText();

public://Message Handler
	virtual void OnSetFocus(TComponent *pLoseFocus);
	virtual void OnLoseFocus(TComponent *pSetFocus);
	virtual void OnChar(UINT nChar, int nRepCnt, UINT nFlags);
	virtual void OnKeyDown(UINT nChar, int nRepCnt, UINT nFlags);
	virtual void OnImeComposition(HWND hWnd, WPARAM wParam, LPARAM lParam);	
	virtual void OnImeNotify(HWND hWnd, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInputLangChangeRequest( HWND hWnd, WPARAM wParam, LPARAM lParam );
	virtual void ShowComponent( BOOL bVisible = TRUE);
};

#endif // !defined(AFX_TEDIT_H__B581C2D6_C499_4588_A482_AFAFDB23AEDC__INCLUDED_)
