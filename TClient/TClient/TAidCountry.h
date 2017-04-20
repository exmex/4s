#pragma once

class CTAidCountry : public CTClientUIBase
{
public:
	enum ALLOW
	{
		ALLOW_ALL,
		ALLOW_D,
		ALLOW_C
	};

	CTAidCountry( TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTAidCountry();
	void SetInfo(DWORD dwDefugel, DWORD dwCraxion);

public:
	ALLOW m_eAllow;
	TGauge* m_pGuageD;
	TGauge* m_pGuageC;
	TComponent* m_pPercentD;
	TComponent* m_pPercentC;
	TComponent* m_pMsg;
};