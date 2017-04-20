#include "stdafx.h"
#include "TClientMsgBox.h"

CTClientMsgBox::CTClientMsgBox(void)
{
	m_pCandidate = NULL;
}

CTClientMsgBox::~CTClientMsgBox(void)
{
	// m_mapFrame.clear();
}

HRESULT CTClientMsgBox::Render( DWORD dwTickCount )
{
	if( CTNationOption::JAPAN_IME || CTNationOption::TAIWAN_IME )
	{
		if( m_pCandidate && m_pCandidate->IsVisible())
		{
			RemoveKid(m_pCandidate);
			AddKid(m_pCandidate);
		}
	}

	return CFrameGroup::Render( dwTickCount );
}