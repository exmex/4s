#pragma once

class TOleControlSite;


class TOccManager : public COccManager
{
public:
	TOccManager() { m_pTHOST = NULL;};
	virtual ~TOccManager() {};

public:
	COleControlSite *CreateSite( COleControlContainer *pCtrlCont);

public:
	THttpCtrl *m_pTHOST;
};
