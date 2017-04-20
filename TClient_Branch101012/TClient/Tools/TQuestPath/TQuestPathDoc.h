// TQuestPathDoc.h : CTQuestPathDoc 클래스의 인터페이스
//


#pragma once

class CTQuestPathDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CTQuestPathDoc();
	DECLARE_DYNCREATE(CTQuestPathDoc)

// 특성
public:
	MAPTQITEM m_mapTMISSION;
	MAPTQITEM m_mapTQCLASS;

	CTGameDB m_vTGAMEDB;
	LPTQITEM m_pTQITEM;

public:
	void ReleaseBUF();

// 작업
public:
	CTreeCtrl *GetQuestTreeCtrl();

	LPTQITEM FindTMISSION( DWORD dwID);
	LPTQITEM FindTQCLASS( DWORD dwID);
	CString GetTNPCName( WORD wID);

	LPTTITEM PickTTITEM(
		LPVTTITEM pVTTITEM,
		BYTE bType,
		DWORD dwID);
	BYTE LoadTQUEST();

	BYTE GetAutosetData(
		LPTTITEM pTRESULT,
		LPTQITEM pTQITEM,
		LPTTITEM pTTITEM);

	void UpdateDATA( LPTQITEM pTQITEM);
	void ResetTERM( LPTQITEM pTQITEM);

// 재정의
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 구현
public:
	virtual ~CTQuestPathDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 메시지 맵 함수를 생성했습니다.
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDbLoadQuest();
	afx_msg void OnDbLoadGame();
};
