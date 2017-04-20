#pragma once

class CTClientMsgBox : public CFrameGroup
{
public:
	TComponent* m_pCandidate;

public:
	CTClientMsgBox(void);
	virtual ~CTClientMsgBox(void);

	virtual HRESULT Render( DWORD dwTickCount );
};
