#pragma once
#include "TFrame.h"

class TCandidate : public TFrame
{
public:
	TCandidate( TComponent *pParent, LP_FRAMEDESC pDesc)
		: TFrame( pParent, pDesc)
	{}

	virtual ~TCandidate()
	{}

	virtual void ResetCandidateChar()=0;
	virtual void SetCandidateChar( std::vector<CString>& vString )=0;
	virtual void SetSelection( INT nIndex )=0;
	virtual void MoveComponentAdjustText( CRect& )=0;
};
