#pragma once

class CTDetailNameDlg : public CTClientUIBase
{
public:
	CTDetailNameDlg( TComponent*pParent, LP_FRAMEDESC pDesc);
	virtual ~CTDetailNameDlg();

	void ResetText( CString& );
	void ClearText();
	void SetPos( POINT );

	virtual BOOL CanWithItemUI();

protected:
	INT					m_nDefaultWidth;
	INT					m_nHeight;

	TComponent*		m_pMID;
	TComponent*		m_pHEAD;
	TComponent*		m_pTAIL;
	TComponent*		m_pTEXT;

	TCOMP_LIST			m_vExtBLOCK;
	INT					m_nBlockWidth;

	INT					m_nTextWidth;

	void AddBlock( INT nCount );
	void RemoveBlock( INT nCount );
	void ReAlignBlock();
};