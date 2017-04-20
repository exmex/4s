#pragma once


// CSetIDSeedDlg 대화 상자입니다.

class CSetIDSeedDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetIDSeedDlg)

public:
	CSetIDSeedDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetIDSeedDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_ITEM_ID_SEED };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

	DWORD m_dwID_Animation;
	DWORD m_dwID_ObjectType;
	DWORD m_dwID_Texture;
	DWORD m_dwID_Image;
	DWORD m_dwID_Media;
	DWORD m_dwID_Frame;
	DWORD m_dwID_Component;
	DWORD m_dwID_Mesh;
	DWORD m_dwID_Map;
	DWORD m_dwID_Object;
	DWORD m_dwID_SFX;
	DWORD m_dwSeed;
};
