#pragma once

class ITInnerFrame : public CTClientUIBase
{
protected:
	UINT m_nFrameID;

public:
	UINT GetFrameID() const { return m_nFrameID; }

	UINT GetUniqueID()
	{
		UINT nResult;
		do { nResult = (UINT)::rand(); }
		while( FindKid(nResult) );
		return nResult;
	}

	virtual void RequestInfo() = 0;
	virtual void ResetInfo() = 0;

public:
	ITInnerFrame(TComponent* pParent, LP_FRAMEDESC pDesc, UINT nFrameID)
		: CTClientUIBase( pParent, pDesc), m_nFrameID(nFrameID)
	{}
	
	virtual ~ITInnerFrame() {}
};