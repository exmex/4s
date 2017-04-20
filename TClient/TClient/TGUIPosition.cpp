#include "Stdafx.h"
#include "TGUIPosition.h"
#include "TClientWnd.h"

#define TO_INT(ptr)		*((int*)(ptr))
#define TO_FLOAT(ptr)	*((float*)(ptr))

// ===============================================================================
CTGUIPosition	CTGUIPosition::m_NetInitPos[TNETFRAME_COUNT];
CTGUIPosition	CTGUIPosition::m_ClientInitPos[TFRAME_COUNT];
// ===============================================================================

// ===============================================================================
CTGUIPosition::CTGUIPosition()
	: m_eType(TGUI_POS_ABSOLUTE), m_eBaseX(TGUI_XBASE_LEFT), 
	  m_eBaseY(TGUI_YBASE_TOP), m_pParamX(0), m_pParamY(0)
{
}
// -------------------------------------------------------------------------------
CTGUIPosition::~CTGUIPosition()
{
}
// ===============================================================================

// ===============================================================================
void CTGUIPosition::Apply_ABS(TComponent* pComp)
{
	CPoint ptLT;
	ptLT.x = ToAbsLeft(pComp, TO_INT(m_pParamX), m_eBaseX);
	ptLT.y = ToAbsTop(pComp, TO_INT(m_pParamY), m_eBaseY);

	pComp->MoveComponent(ptLT);
}
// -------------------------------------------------------------------------------
void CTGUIPosition::Apply_REL(TComponent* pComp)
{
	CPoint ptScreen = GetScreen();

	CPoint ptLT;
	ptLT.x = ToAbsLeft(pComp, int( FLOAT(ptScreen.x) * TO_FLOAT(m_pParamX) ), m_eBaseX);
	ptLT.y = ToAbsTop(pComp, int( FLOAT(ptScreen.x) * TO_FLOAT(m_pParamX) ), m_eBaseY);

	pComp->MoveComponent(ptLT);
}
// -------------------------------------------------------------------------------
void CTGUIPosition::Apply_EXT(TComponent* pComp)
{
	CPoint ptScreen = GetScreen();
	float fScaleX = FLOAT(ptScreen.x) / FLOAT(TBASE_SCREEN_X);
	float fScaleY = FLOAT(ptScreen.y) / FLOAT(TBASE_SCREEN_Y);

	pComp->SetScale(fScaleX, fScaleY, TRUE);
}
// -------------------------------------------------------------------------------
void CTGUIPosition::Apply_CET(TComponent* pComp)
{
	CPoint ptScreen = GetScreen();

	int nX = (ptScreen.x/2) + int( FLOAT(ptScreen.x) * TO_FLOAT(m_pParamX) );
	int nY = (ptScreen.y/2) + int( FLOAT(ptScreen.y) * TO_FLOAT(m_pParamY) );

	CPoint ptLT;
	ptLT.x = ToAbsLeft(pComp, nX, m_eBaseX);
	ptLT.y = ToAbsTop(pComp, nY, m_eBaseY);

	pComp->MoveComponent(ptLT);
}
// -------------------------------------------------------------------------------
void CTGUIPosition::Apply_LCT(TComponent* pComp)
{
	CPoint ptScreen = GetScreen();

	int nX = (ptScreen.x/4) + int( FLOAT(ptScreen.x) * TO_FLOAT(m_pParamX) );
	int nY = (ptScreen.y/2) + int( FLOAT(ptScreen.y) * TO_FLOAT(m_pParamY) );

	CPoint ptLT;
	ptLT.x = ToAbsLeft(pComp, nX, m_eBaseX);
	ptLT.y = ToAbsTop(pComp, nY, m_eBaseY);

	pComp->MoveComponent(ptLT);
}
// -------------------------------------------------------------------------------
void CTGUIPosition::Apply_RCT(TComponent* pComp)
{
	CPoint ptScreen = GetScreen();

	int nX = (ptScreen.y*3/4) + int( FLOAT(ptScreen.x) * TO_FLOAT(m_pParamX) );
	int nY = (ptScreen.y/2) + int( FLOAT(ptScreen.y) * TO_FLOAT(m_pParamY) );

	CPoint ptLT;
	ptLT.x = ToAbsLeft(pComp, nX, m_eBaseX);
	ptLT.y = ToAbsTop(pComp, nY, m_eBaseY);

	pComp->MoveComponent(ptLT);
}
// ===============================================================================
int CTGUIPosition::ToAbsLeft(TComponent* pComp, int nPos, TGUI_XBASE eBaseX)
{
	switch( eBaseX )
	{
	case TGUI_XBASE_CENTER	: 
		{
			CRect rc;
			pComp->GetComponentRect(&rc);		
			nPos -= rc.Width()/2;
		}
		break;

	case TGUI_XBASE_RIGHT	: 
		{
			CRect rc;
			pComp->GetComponentRect(&rc);		
			nPos -= rc.Width();
		}
		break;
	
	default					: break;
	}

	return nPos;
}
// -------------------------------------------------------------------------------
int CTGUIPosition::ToAbsTop(TComponent* pComp, int nPos, TGUI_YBASE eBaseY)
{
	switch( eBaseY )
	{
	case TGUI_YBASE_CENTER	: 
		{
			CRect rc;
			pComp->GetComponentRect(&rc);		
			nPos -= rc.Height()/2;
		}
		break;

	case TGUI_YBASE_BOTTOM	: 
		{
			CRect rc;
			pComp->GetComponentRect(&rc);		
			nPos -= rc.Height();
		}
		break;
	
	default					: break;
	}

	return nPos;
}
// ===============================================================================
CPoint CTGUIPosition::GetScreen()
{
	CTClientWnd* pMainWnd = CTClientWnd::GetInstance();
	
	CPoint pt;
	pt.x = pMainWnd->m_Device.m_option.m_dwScreenX;
	pt.y = pMainWnd->m_Device.m_option.m_dwScreenY;

	return pt;
}
// ===============================================================================

// ===============================================================================
void CTGUIPosition::Init( TGUI_POS_TYPE type,
						  TGUI_XBASE baseX, TGUI_YBASE baseY,
						  LPVOID pParamX, LPVOID pParamY )
{
	m_eType = type;

	m_eBaseX = baseX;
	m_eBaseY = baseY;

	m_pParamX = pParamX;
	m_pParamY = pParamY;
}
// ===============================================================================
void CTGUIPosition::Apply(TComponent* pComp)
{
	switch(m_eType)
	{
	case TGUI_POS_ABSOLUTE		: return Apply_ABS(pComp);
	case TGUI_POS_RELATIVE		: return Apply_REL(pComp);
	case TGUI_POS_EXTEND		: return Apply_EXT(pComp);
	case TGUI_POS_CENTER		: return Apply_CET(pComp);
	case TGUI_POS_LEFT_CENTER	: return Apply_LCT(pComp);	
	case TGUI_POS_RIGHT_CENTER	: return Apply_RCT(pComp);
	}
}
// ===============================================================================

// ===============================================================================
void CTGUIPosition::InitNetFrame()
{
	/*CTClientWnd* pMainWnd = CTClientWnd::GetInstance();
	
	CTGUIPosition pos;
	pos.Init(TGUI_POS_EXTEND, TGUI_XBASE_LEFT, TGUI_YBASE_TOP, 0,0);

	for(int i=0; i<TNETFRAME_COUNT; ++i)
		 pos.Apply( pMainWnd->m_pTNetFRAME[i] );*/
}
// ===============================================================================













