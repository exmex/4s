#pragma once

class CTCandidateFrame : public TCandidate
{
public:
	CTCandidateFrame( TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTCandidateFrame();

public:
	void ResetCandidateChar();
	void SetCandidateChar( std::vector<CString>& vString );
	void SetSelection( INT nIndex );

public:
	virtual void MoveComponentAdjustText( CRect& );
	virtual HRESULT Render( DWORD dwTickCount );

protected:
	VTSTRING m_vCandidateChar;
	INT m_nSelection;
	TComponent* m_pBK[9];
	TComponent* m_pSEL[9];
	TComponent* m_pNUM[9];
	TComponent* m_pTEXT[9];
	TComponent* m_pUP;
	TComponent* m_pDOWN;
};