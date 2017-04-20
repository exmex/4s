// TComponent.cpp: implementation of the TComponent class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

extern CTachyonRes*	g_pImagePool;
CD3DFont		g_Text;
BOOL TComponent::m_bUseWordBreak = TRUE;

// Arab Ver. CHAR_EXCEPTION, CHAR_RIGHT, CHAR_LEFT
BYTE TComponent::vCHAR_DIR[256] = {	// 8 X 32
		CHAR_EXCEPTION,	CHAR_EXCEPTION, CHAR_EXCEPTION, CHAR_EXCEPTION, CHAR_EXCEPTION, CHAR_EXCEPTION, CHAR_EXCEPTION, CHAR_EXCEPTION,		//1
		CHAR_EXCEPTION,	CHAR_EXCEPTION, CHAR_EXCEPTION, CHAR_EXCEPTION, CHAR_EXCEPTION, CHAR_EXCEPTION, CHAR_EXCEPTION, CHAR_EXCEPTION,		//2
		CHAR_EXCEPTION,	CHAR_EXCEPTION, CHAR_EXCEPTION, CHAR_EXCEPTION, CHAR_EXCEPTION, CHAR_EXCEPTION, CHAR_EXCEPTION, CHAR_EXCEPTION,		//3
		CHAR_EXCEPTION,	CHAR_EXCEPTION, CHAR_EXCEPTION, CHAR_EXCEPTION, CHAR_EXCEPTION, CHAR_EXCEPTION, CHAR_EXCEPTION, CHAR_EXCEPTION,		//4
		CHAR_SPECIAL,	CHAR_SPECIAL,	CHAR_SPECIAL,	CHAR_SPECIAL,	CHAR_SPECIAL,	CHAR_SPECIAL,	CHAR_SPECIAL,	CHAR_SPECIAL,		//5
		CHAR_SPECIAL,	CHAR_SPECIAL,	CHAR_SPECIAL,	CHAR_SPECIAL,	CHAR_SPECIAL,	CHAR_SPECIAL,	CHAR_SPECIAL,	CHAR_SPECIAL,		//6
		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_RIGHT,			//7
		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_SPECIAL,	CHAR_SPECIAL,	CHAR_SPECIAL,	CHAR_SPECIAL,	CHAR_SPECIAL,	CHAR_SPECIAL,		//8
		CHAR_SPECIAL,	CHAR_RIGHT,		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_RIGHT,			//9
		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_RIGHT,			//10
		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_RIGHT,			//11
		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_SPECIAL,	CHAR_SPECIAL,	CHAR_SPECIAL,	CHAR_SPECIAL,	CHAR_SPECIAL,		//12
		CHAR_SPECIAL,	CHAR_RIGHT,		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_RIGHT,			//13
		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_RIGHT,			//14
		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_RIGHT,			//15
		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_RIGHT,		CHAR_SPECIAL,	CHAR_SPECIAL,	CHAR_SPECIAL,	CHAR_SPECIAL,	CHAR_EXCEPTION,		//16
		CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,		//17
		CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,		//18
		CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,		//19
		CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,		//20
		CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,		//21
		CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,		//22
		CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,		//23
		CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,		//24
		CHAR_EXCEPTION,	CHAR_LEFT,		CHAR_LEFT,		CHAR_LEFT,		CHAR_LEFT,		CHAR_LEFT,		CHAR_LEFT,		CHAR_LEFT,			//25
		CHAR_LEFT,		CHAR_LEFT,		CHAR_LEFT,		CHAR_LEFT,		CHAR_LEFT,		CHAR_LEFT,		CHAR_LEFT,		CHAR_LEFT,			//26
		CHAR_LEFT,		CHAR_LEFT,		CHAR_LEFT,		CHAR_LEFT,		CHAR_LEFT,		CHAR_LEFT,		CHAR_LEFT,		CHAR_LEFT,			//27
		CHAR_LEFT,		CHAR_LEFT,		CHAR_LEFT,		CHAR_LEFT,		CHAR_LEFT,		CHAR_LEFT,		CHAR_LEFT,		CHAR_LEFT,			//28
		CHAR_EXCEPTION,	CHAR_LEFT,		CHAR_EXCEPTION,	CHAR_LEFT,		CHAR_LEFT,		CHAR_LEFT,		CHAR_LEFT,		CHAR_LEFT,			//29
		CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_LEFT,		CHAR_LEFT,		CHAR_EXCEPTION,	CHAR_EXCEPTION,		//30
		CHAR_LEFT,		CHAR_LEFT,		CHAR_LEFT,		CHAR_LEFT,		CHAR_EXCEPTION,	CHAR_LEFT,		CHAR_LEFT,		CHAR_EXCEPTION,		//31
		CHAR_LEFT,		CHAR_EXCEPTION,	CHAR_LEFT,		CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION,	CHAR_EXCEPTION		//32
		};

//////////////////////////////////////////////////////////////////////
// TextLine Implemetation
//////////////////////////////////////////////////////////////////////
TComponent::TextLine::TextLine()
: m_pVB(NULL),
m_nVertexCount(0),
m_nStartX(0),
m_nStartY(0),
m_nEndX(0),
m_nEndY(0),
m_dwColor(0),
m_nDotLength(0),
m_nSpaceLength(0)
{
}

TComponent::TextLine::~TextLine()
{
	if( m_pVB )
		m_pVB->Release();
}

void TComponent::TextLine::MakeLine( LPDIRECT3DDEVICE9 pDevice, INT StartX, INT StartY, INT EndX, INT EndY, DWORD dwColor )
{
	if( m_nStartX != StartX ||
		m_nStartY != StartY ||
		m_nEndX != EndX ||
		m_nEndY != EndY ||
		m_dwColor != dwColor )
	{
		m_nStartX = StartX;
		m_nStartY = StartY;
		m_nEndX = EndX;
		m_nEndY = EndY;
		m_dwColor = dwColor;

		std::vector<TNLPOINT> vOutput;

		TNLPOINT pt;
		pt.m_fPosZ = 0.0f;
		pt.m_fRHW = 1.0f;
		pt.m_dwColor = dwColor;

		pt.m_fPosX = (FLOAT) m_nStartX;
		pt.m_fPosY = (FLOAT) m_nStartY;
		vOutput.push_back( pt );

		pt.m_fPosX = (FLOAT) m_nEndX;
		pt.m_fPosY = (FLOAT) m_nEndY;
		vOutput.push_back( pt );

		m_nVertexCount = vOutput.size();
		_CreateVB( pDevice, m_nVertexCount);

		if( m_pVB )
		{
			TNLPOINT *ptrVB;
			if( D3D_OK == m_pVB->Lock( 0, 0, (LPVOID*) &ptrVB, 0 ) )
			{
				memcpy( ptrVB, &vOutput[0], sizeof(TNLPOINT)*m_nVertexCount);
				m_pVB->Unlock();
			}
			else
			{
				m_pVB->Release();
				m_pVB = NULL;
			}
		}
	}
}

void TComponent::TextLine::MakeDotLine( LPDIRECT3DDEVICE9 pDevice, INT StartX, INT StartY, INT EndX, INT EndY, DWORD dwColor, INT DotLength, INT SpaceLength)
{
	if( m_nStartX != StartX ||
		m_nStartY != StartY ||
		m_nEndX != EndX ||
		m_nEndY != EndY ||
		m_dwColor != dwColor ||
		m_nDotLength != DotLength ||
		m_nSpaceLength != SpaceLength )
	{
		m_nStartX = StartX;
		m_nStartY = StartY;
		m_nEndX = EndX;
		m_nEndY = EndY;
		m_nDotLength = DotLength;
		m_nSpaceLength = SpaceLength;
		m_dwColor = dwColor;

		std::vector<TNLPOINT> vOutput;
	    
		D3DXVECTOR2 vStart( (FLOAT) (StartX), (FLOAT) (StartY));
		D3DXVECTOR2 vEnd( (FLOAT) (EndX), (FLOAT) (EndY));

		FLOAT fLength = D3DXVec2Length( &D3DXVECTOR2(vEnd-vStart) );
		INT nCount = (INT) (fLength / (DotLength+SpaceLength) );

		D3DXVECTOR2 vDir(vEnd-vStart);
		D3DXVec2Normalize( &vDir, &vDir);

		D3DXVECTOR2 vDot(vDir), vSpace(vDir);
		vDot *= (FLOAT) DotLength;
		vSpace *= (FLOAT) SpaceLength;

		D3DXVECTOR2 vCur = vStart;
		for( INT i=0 ; i < nCount ; ++i )
		{
			//vCur에서부터 vCur+vDot까지 선
			D3DXVECTOR2 vDotEnd = vCur + vDot;

			TNLPOINT pt;
			pt.m_fPosZ = 0.0f;
			pt.m_fRHW = 1.0f;
			pt.m_dwColor = dwColor;

			pt.m_fPosX = vCur.x;
			pt.m_fPosY = vCur.y;
			vOutput.push_back( pt );

			pt.m_fPosX = vDotEnd.x;
			pt.m_fPosY = vDotEnd.y;
			vOutput.push_back( pt );

			vCur += (vDot+vSpace);
		}

		// vCur가 vEnd와 같지 않고
		if( fabsf( vCur.x > vEnd.x ? vCur.x-vEnd.x : vEnd.x-vCur.x ) > 0.0001f ||
			fabsf( vCur.y > vEnd.y ? vCur.y-vEnd.y : vEnd.y-vCur.y ) )
		{
			// vCur가 vEnd를 넘어가지 않았다면 나머지 공간도 그려준다.	
			D3DXVECTOR2 vDir1, vDir2;
			D3DXVec2Normalize( &vDir1, &(vCur-vStart));
			D3DXVec2Normalize( &vDir2, &(vEnd-vCur));

			if( D3DXVec2Length( &(vDir1-vDir2)) < 0.00001f )
			{
				D3DXVECTOR2 vEndDot = D3DXVec2Length( &(vEnd-vCur) ) < D3DXVec2Length( &(vDot) ) ? vEnd : vCur + vDot;

				TNLPOINT pt;
				pt.m_fPosZ = 0.0f;
				pt.m_fRHW = 1.0f;
				pt.m_dwColor = dwColor;

				pt.m_fPosX = vCur.x;
				pt.m_fPosY = vCur.y;
				vOutput.push_back( pt );

				pt.m_fPosX = vEndDot.x;
				pt.m_fPosY = vEndDot.y;
				vOutput.push_back( pt );
			}
		}

		m_nVertexCount = vOutput.size();
		_CreateVB( pDevice, m_nVertexCount);

		if( m_pVB )
		{
			TNLPOINT *ptrVB;
			if( D3D_OK == m_pVB->Lock( 0, 0, (LPVOID*) &ptrVB, 0 ) )
			{
				memcpy( ptrVB, &vOutput[0], sizeof(TNLPOINT)*m_nVertexCount);
				m_pVB->Unlock();
			}
			else
			{
				m_pVB->Release();
				m_pVB = NULL;
			}
		}
	}
}

void TComponent::TextLine::Draw( LPDIRECT3DDEVICE9 pDevice )
{
	if( m_pVB )
	{
		pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
		
		pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);

		pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
		pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_DISABLE);

		pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE);
		pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);
		pDevice->SetRenderState( D3DRS_ZENABLE, FALSE);

		pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
		pDevice->SetStreamSource( 0, m_pVB, 0, sizeof(TNLPOINT));
		pDevice->SetFVF( T3DFVF_TNLPOINT );
		pDevice->DrawPrimitive( D3DPT_LINELIST, 0, m_nVertexCount/2);
		pDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	}
}

void TComponent::TextLine::_CreateVB( LPDIRECT3DDEVICE9 pDevice, INT nCount )
{
	if( m_pVB )
		m_pVB->Release();

	m_pVB = NULL;

	pDevice->CreateVertexBuffer(
			nCount * sizeof(TNLPOINT),
			0,
			T3DFVF_TNLPOINT,
			D3DPOOL_MANAGED,
			&m_pVB, NULL);
}

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void TComponent::Init()
{	
	m_style = TS_NONE;

	m_pTipGDIFont = NULL;
	m_pGDIFont = NULL;
	m_pTipFont = NULL;
	m_pFont = NULL;
	m_pDESC = NULL;
	m_clrFg = RGB(0, 0, 255);
	m_clrBg = RGB(0, 0, 0);

	m_pParent = NULL;
	m_pParent = NULL;

	m_pEnable = m_pDisable = NULL;

	m_rc = CRect( -1, -1, -1, -1 );
	m_rcSrc = CRect( -1, -1, -1, -1 );

	for( int i=0; i<TCML_MENU_COUNT; i++)
		m_menu[i] = TCML_ID_NULL;

	m_id = TCML_ID_NULL;
	m_bType = TCML_TYPE_COMPONENT;
	m_flagAlign = m_flagBlt = TCML_FLAG_NULL;

	m_strText.Empty();
	m_strTooltip.Empty();

	m_bUseImageClrForText = FALSE;
	m_blIsCaret = FALSE;
	m_bNoHIT = FALSE;
	m_bFocus = FALSE;

	m_bWordBreak = TRUE;
	m_bVCenter = TRUE;
	m_bVisible = TRUE;
	m_bEnable = TRUE;

	m_nTextExtent = 0;
	m_nVMargine = 0;
	m_nHMargine = 0;

	m_dwMediaID = 0;
	m_dwColor = 0;

	m_dwTotalTick = 0;
	m_blDoSprite = TRUE;

	m_bNeedUpdateTextSetting = TRUE;
	m_PrvFlagAlign = TCML_FLAG_NULL;
	m_rcPrv = CRect(-1,-1,-1,-1);

	m_bShrink = TRUE;
	m_bDidShrink = FALSE;

	m_fScaleX = 1.0f;
	m_fScaleY = 1.0f;

	m_bRecalcKidRect = TRUE;
	m_bHitTestType = HITTEST_TYPE_IMAGEMASK;
	m_pNextTab = NULL;
}

TComponent::TComponent(TComponent *pParent, LP_FRAMEDESC pDesc)
{
	Init();

	m_pParent = pParent;

	CreateFrom(pDesc);
}

TComponent::~TComponent()
{
	Clear();	
}

void TComponent::CreateFrom(LP_FRAMEDESC pDesc)
{
	if( pDesc )
	{
		BuildMe(pDesc);
		BuildFont(pDesc);
		BuildKid(pDesc);
	}

	m_bRecalcKidRect = TRUE;
}

void TComponent::BuildMe( LP_FRAMEDESC pDesc )
{
	m_pDESC = pDesc;

	m_id = pDesc->m_vCOMP.m_dwID;
	m_dwMediaID = pDesc->m_vCOMP.m_dwSND;
	m_dwColor = pDesc->m_vCOMP.m_dwCOLOR;
	m_bType = pDesc->m_vCOMP.m_bType;
	m_strText = pDesc->m_vCOMP.m_strText;
	m_flagBlt = pDesc->m_vCOMP.m_bDisplay;
	m_flagAlign = pDesc->m_vCOMP.m_bAlign;	
	m_strTooltip = pDesc->m_vCOMP.m_strTooltip;
	m_style |= pDesc->m_vCOMP.m_dwStyle;

	for( int i=0; i<TCML_MENU_COUNT; i++)
		m_menu[i] = pDesc->m_vCOMP.m_vMENU[i];

	if(g_pImagePool)
	{
		m_pEnable = FindSprite(pDesc->m_vCOMP.m_dwImageID[TCML_IDX_ENABLE]);
		m_pDisable = FindSprite(pDesc->m_vCOMP.m_dwImageID[TCML_IDX_DISABLE]);
	}

	int nWidth = pDesc->m_vCOMP.m_nWidth;
	int nHeight = pDesc->m_vCOMP.m_nHeight;

	if( m_pEnable == NULL &&
		m_pDisable == NULL &&
		nWidth != 0 &&
		nHeight != 0 )
	{
		m_bHitTestType = HITTEST_TYPE_RECT;
	}

	if( nWidth == 0 && m_pEnable )
		nWidth = m_pEnable->m_vImage[0]->GetWidth();

	if( nHeight == 0 && m_pEnable )
		nHeight = m_pEnable->m_vImage[0]->GetHeight();

	m_nVMargine = pDesc->m_vCOMP.m_nMargineV;
	m_nHMargine = pDesc->m_vCOMP.m_nMargineH;

	m_rc = CRect(
		pDesc->m_vCOMP.m_nPosX,
		pDesc->m_vCOMP.m_nPosY,
		pDesc->m_vCOMP.m_nPosX + nWidth,
		pDesc->m_vCOMP.m_nPosY + nHeight);
}

void TComponent::BuildFont( LP_FRAMEDESC pDesc )
{
	//Font
	//지정한 폰트가 없다면 부모의 폰트를 상속 받는다.
	CGDIFont *pTipFont = FindFont(pDesc->m_vCOMP.m_dwTooltipID);
	CGDIFont *pFont = FindFont(pDesc->m_vCOMP.m_dwFontID);

	if( !pTipFont && m_pParent &&
		m_pParent->m_pTipGDIFont )
	{
		pTipFont = m_pParent->m_pTipGDIFont;
	}

	if( !pFont && m_pParent &&
		m_pParent->m_pGDIFont )
		pFont = m_pParent->m_pGDIFont;
	{
		SetFont( pFont, pTipFont);
	}
}

void TComponent::BuildKid( LP_FRAMEDESC pDesc )
{
	LP_FRAMEDESC pTemplateKid = pDesc->m_pCHILD;
	while(pTemplateKid)
	{
		TComponent* pKid = AddKid(pTemplateKid);

		if( pKid )
		{
			CPoint ptKid;
			CSize szKid;
			pKid->GetComponentPos( &ptKid );
			pKid->GetComponentSize( &szKid );

			if( szKid.cx + ptKid.x > m_rc.Width() )
				m_rc.right = m_rc.left + szKid.cx + ptKid.x;

			if( szKid.cy + ptKid.y > m_rc.Height() )
				m_rc.bottom = m_rc.top + szKid.cy + ptKid.y;
		}

		pTemplateKid = pTemplateKid->m_pNEXT;
	}

	m_nTextExtent = m_rc.Width() - 2 * m_nHMargine;
}

void TComponent::Clear()
{
	ClearKids();

	SAFE_DELETE(m_pFont);
	SAFE_DELETE(m_pTipFont);

	ClearTextOutputData();
}

void TComponent::ClearKids()
{
	for(TCOMP_LIST::iterator it = m_kids.begin(); it != m_kids.end(); it++)
	{
		TCOMP_LIST::iterator del = it;
		TComponent *pDel =(*it);

		delete pDel;
	}

	m_kids.clear();
}

TComponent* TComponent::AddKid( LP_FRAMEDESC pDesc)
{
	TComponent *pKid = ComponentNew(pDesc);
	if(!pKid)
		return NULL;

	return AddKid(pKid);
}

TComponent* TComponent::AddKid( TComponent *pKid )
{
	if(!pKid)
		return NULL;

	pKid->m_index = m_kids.size();
	pKid->m_pParent = this;
	m_kids.push_back(pKid);

	NotifyComponentUpdate();
	return pKid;
}

void TComponent::RemoveKid( TComponent *pKid)
{
	TCOMP_LIST::iterator it;
	int nIndex = 0;

	for( it = m_kids.begin(); it != m_kids.end(); it++)
		if( (*it) == pKid )
		{
			pKid->m_pParent = NULL;
			pKid->NotifyComponentUpdate();

			m_kids.erase(it);
			break;
		}

	for( it = m_kids.begin(); it != m_kids.end(); it++)
	{
		(*it)->m_index = nIndex;
		nIndex++;
	}

	NotifyComponentUpdate();
}

void TComponent::RemoveKidWithoutRearrage( TComponent* pKid )
{
	TCOMP_LIST::iterator it;

	for( it = m_kids.begin(); it != m_kids.end(); it++)
		if( (*it) == pKid )
		{
			pKid->m_pParent = NULL;
			pKid->NotifyComponentUpdate();

			m_kids.erase(it);
			break;
		}

	NotifyComponentUpdate();
}

TComponent* TComponent::ComponentNew(LP_FRAMEDESC pDesc)
{
	TComponent *pComponent = NULL;

	switch(pDesc->m_vCOMP.m_bType)
	{
	case TCML_TYPE_COMPONENT	:	pComponent = new TComponent(this, pDesc);		break;
	case TCML_TYPE_IMAGELIST	:	pComponent = new TImageList(this, pDesc);		break;
	case TCML_TYPE_TABCTRL		:	pComponent = new TTabCtrl(this, pDesc);			break;
	case TCML_TYPE_BUTTON		:	pComponent = new TButton(this, pDesc);			break;
	case TCML_TYPE_SCROLL		:	pComponent = new TScroll(this, pDesc);			break;
	case TCML_TYPE_METER		:   pComponent = new TMeter(this, pDesc);			break;
	case TCML_TYPE_FRAME		:	pComponent = new TFrame(this, pDesc);			break;
	case TCML_TYPE_COMBO		:	pComponent = new TCombo(this, pDesc);			break;
	case TCML_TYPE_EDIT			:	pComponent = new TEdit(this, pDesc);			break;	
	case TCML_TYPE_LIST			:	pComponent = new TList(this, pDesc);			break;
	case TCML_TYPE_GAUGE		:	pComponent = new TGauge(this, pDesc);			break;
	default						:	return FALSE;
	}

	return pComponent;
}

TComponent * TComponent::FindKid(DWORD id)
{
	TCOMP_LIST::iterator it;

	for( it = m_kids.begin(); it != m_kids.end(); it ++)
	{
		TComponent *pKid = *it;
		if( pKid &&
			pKid->m_id == id)
			return pKid;
	}

	return NULL;
}

BOOL TComponent::HitRect( CPoint pt)
{
	CRect rc;
	GetComponentRectWithKid(&rc);
	ComponentToScreen(&rc);
	if( rc.PtInRect(pt) )
	{
		rc = m_rc;
		ComponentToScreen(&rc);
		if( rc.PtInRect(pt) )
			return TRUE;

		TCOMP_LIST::iterator it;
		for( it=m_kids.begin(); it!=m_kids.end(); it++)
		{
			TComponent *pKid = *it;

			if( pKid->HitRect(pt) )
				return TRUE;
		}
	}

	return FALSE;
}

BOOL TComponent::HitTest( CPoint pt)
{
	if( m_bNoHIT )
		return FALSE;

	if( m_bVisible )
	{
		CRect rc;
		GetComponentRectWithKid(&rc);
		ComponentToScreen(&rc);

		if( rc.PtInRect(pt) )
		{
			switch( m_bHitTestType )
			{
			case HITTEST_TYPE_RECT:
				{
					CRect rc = m_rc;
					ComponentToScreen( &rc );
					if( rc.PtInRect(pt) )
						return TRUE;
				}
				break;

			default:
			case HITTEST_TYPE_IMAGEMASK:
				{
					LPIMAGESET pImageset = m_bEnable || !m_pDisable ? m_pEnable: m_pDisable;

					if( pImageset )
					{
						pImageset->m_dwCurTick = m_dwTotalTick % pImageset->m_dwTotalTick;
						CD3DImage *pImage = pImageset->GetImage();

						if( pImage )
						{
							CPoint point = pt;

							InComponentScreenPt(&point);
							if(pImage->GetMask( point.x, point.y))
								return TRUE;
						}
					}
				}
				break;
			}

			TCOMP_LIST::iterator it;
			for( it=m_kids.begin(); it!=m_kids.end(); it++)
			{
				TComponent *pKid = *it;
				if( pKid->HitTest(pt) )
					return TRUE;
			}
		}
	}

	return FALSE;
}
// Arab Ver.
BOOL TComponent::CheckRightChar(CString strText, int nIndex, int &nNextCount)
{
	unsigned char chCurrent = strText[nIndex];
	BOOL bResult = FALSE;

	switch(vCHAR_DIR[chCurrent])
	{
	case CHAR_SPECIAL:
		{
			if(strText[nIndex+1] == NULL)
				return bResult;

			if(CheckRightChar(strText, nIndex+1, nNextCount) == TRUE)
			{
				nNextCount++;
				bResult = TRUE;
			}
		}
		break;
	case CHAR_RIGHT:
		{
			if(strText[nIndex+1] != NULL)
				CheckRightChar(strText, nIndex+1, nNextCount);
			nNextCount++;
			bResult = TRUE;
		}
		break;
	}

	return bResult;
}
// Arab Ver.
BOOL TComponent::CheckLeftChar(CString strText, int nIndex, int &nPrevCount)
{
	unsigned char chPrev = strText[nIndex];
	BOOL bResult = FALSE;

	switch(vCHAR_DIR[chPrev])
	{
	case CHAR_SPECIAL:
		{
			if(nIndex-1 < 0)
				return bResult;

			if(CheckLeftChar(strText, nIndex-1, nPrevCount) == TRUE)
			{
				nPrevCount++;
				bResult = TRUE;
			}
		}
		break;
	case CHAR_RIGHT:
		{
			if(nIndex-1 >= 0)
				CheckLeftChar(strText, nIndex-1, nPrevCount);
			nPrevCount++;
			bResult = TRUE;
		}
		break;
	}

	return bResult;
}
// Arab Ver.
CString TComponent::GetCaretPoint(LPCTSTR lpstrText, int nCaret)
{
	CString strText = lpstrText;

	if(nCaret <= 0){
		return strText;
	}

	unsigned char chPrev, chCurrent;
	DWORD dwTextLength = strText.GetLength();
	
	chPrev = strText[nCaret-1];
	chCurrent = strText[nCaret];

	switch(vCHAR_DIR[chPrev])
	{
	case CHAR_LEFT:
		{
			// |아랍어||빈칸|, |아랍어||아랍어|, |아랍어||영어or숫자|, |아랍어||특수기호|
			return strText.Right(dwTextLength - nCaret);
		}
		break;
	case CHAR_RIGHT:
		{
			int nPrevCount = 0;
			CheckLeftChar(strText, nCaret-1, nPrevCount);

			// |영어or숫자||빈칸|
			if(nCaret == dwTextLength)
			{
				return strText.Right(nPrevCount);
			}
			// |영어or숫자||영어or숫자|
			else if(vCHAR_DIR[chCurrent] == CHAR_RIGHT)
			{
				int nNextCount = 0;
				CheckRightChar(strText, nCaret, nNextCount);

				CString strResult = strText.Mid(nCaret - nPrevCount, nPrevCount);
				strResult += strText.Right(dwTextLength - (nCaret + nNextCount));
				return strResult;
			}
			// |영어or숫자||아랍어|
			else if(vCHAR_DIR[chCurrent] == CHAR_LEFT)
			{
				return strText.Right(dwTextLength - nCaret + nPrevCount);
			}
			// |영어or숫자||특수기호|
			else if(vCHAR_DIR[chCurrent] == CHAR_SPECIAL)
			{
				int nNextCount = 0;
				CheckRightChar(strText, nCaret, nNextCount);

				if(nNextCount>0)
				{
					CString strResult = strText.Mid(nCaret - nPrevCount, nPrevCount);
					strResult += strText.Right(dwTextLength - (nCaret + nNextCount));
					return strResult;
				}
				else
					return strText.Right(dwTextLength - nCaret + nPrevCount);
			}
		}
		break;
	case CHAR_SPECIAL:
		{
			// |특수기호||빈칸|, |특수기호||아랍어|
			if(nCaret == dwTextLength || vCHAR_DIR[chCurrent] == CHAR_LEFT)
				return strText.Right(dwTextLength - nCaret);

			int nPrevCount = 0;
			int nNextCount = 0;
			// |특수기호||영어or숫자|, |특수기호||특수기호|
			CheckRightChar(strText, nCaret, nNextCount);
			CheckLeftChar(strText, nCaret-1, nPrevCount);

			if(nPrevCount>0 && nNextCount>0)		// 이 case는 특수기호를 영어or숫자처럼 취급
			{
				CString strResult = strText.Mid(nCaret - nPrevCount, nPrevCount);
				strResult += strText.Right(dwTextLength - (nCaret + nNextCount));
				return strResult;
			}
			else
				return strText.Right(dwTextLength - nCaret);
		}
		break;
	};

	return strText.Right(0);
}
// Arab Ver.
void TComponent::TextToPoint( LPCTSTR strText, int nPos, CPoint &pt, BOOL bFLIP_SCREEN, int nLine)
{
	if(!m_pGDIFont) return;

	CSize sizeText;
	CSize sizeComp = m_rc.Size();
	GetTextExtentPoint(strText, sizeText);

	pt = CPoint(m_rc.left, m_rc.top);
	
	if(m_flagAlign == ALIGN_CENTER)
		pt.x += (sizeComp.cx - sizeText.cx)/2;
	else if(m_flagAlign == ALIGN_RIGHT)
		pt.x += sizeComp.cx - sizeText.cx - m_nHMargine;

	if(nLine == 0xFF)
		pt.y += (sizeComp.cy - sizeText.cy)/2;
	else
	{
		int nLineSize = INT(sizeText.cy * 1.4f);
		pt.y += INT(sizeText.cy * 1.4f / 2 + nLine*nLineSize);
	}

	if(nPos >= 0 && nPos <= strlen(strText))
	{
		CString strTextLeft = strText;
		// Arab Ver.
		strTextLeft = bFLIP_SCREEN ? GetCaretPoint(strTextLeft, nPos) : strTextLeft.Left(nPos);

		GetTextExtentPoint(strTextLeft, sizeText);
		pt.x += sizeText.cx;
	}

	if(m_flagAlign != ALIGN_RIGHT)
		pt.x += m_nHMargine;
	pt.y += m_nVMargine;
}

void TComponent::ShowComponent(BOOL bVisible)
{
	m_bVisible = bVisible;

	TCOMP_LIST::iterator it = GetFirstKidsFinder();

	while( !EndOfKids( it ) )
	{
		TComponent* pKid = GetNextKid( it );
		if( pKid && !pKid->m_blIsCaret )
			pKid->ShowComponent( bVisible );
	}
}

void TComponent::EnableComponent(BOOL bEnable)
{
	m_bEnable = bEnable;
	TCOMP_LIST::iterator it = GetFirstKidsFinder();

	while( !EndOfKids( it ) )
	{
		TComponent* pKid = GetNextKid( it );
		if( pKid && !pKid->m_blIsCaret )
			pKid->EnableComponent( bEnable );
	}
}

int TComponent::GetKidsCount()
{
	return m_kids.size();
}

TCOMP_LIST::iterator TComponent::GetFirstKidsFinder()
{
	return m_kids.begin();
}

TComponent * TComponent::GetNextKid(TCOMP_LIST::iterator & it)
{
	TComponent *pKid = *it++;	
	return pKid;
}

BOOL TComponent::EndOfKids(TCOMP_LIST::iterator it)
{
	return (it == m_kids.end());
}

void TComponent::SetTextClr(DWORD clrText)
{
	if( m_pFont )
	{
		m_pFont->m_dwLineColor &= 0x00FFFFFF;
		m_pFont->m_dwLineColor |= (clrText & 0xFF000000);
		m_pFont->m_dwColor = clrText;
	}
}

void TComponent::SetTextClr( DWORD dwColor,
							 DWORD dwLineColor,
							 BYTE bOutLine)
{
	if(m_pFont)
	{
		m_pFont->m_dwLineColor = dwLineColor;
		m_pFont->m_dwColor = dwColor;
		m_pFont->m_bOutLine = bOutLine;
	}
}

void TComponent::InComponentScreenPt( CPoint* ppt )
{
	ppt->x -= m_rc.left;
	ppt->y -= m_rc.top;

	if( m_pParent )
		m_pParent->InComponentScreenPt( ppt );
}

void TComponent::ScreenToComponent(CPoint *ppt)
{
	if(m_pParent)
	{
		m_pParent->ScreenToComponent(ppt);

		ppt->x -= m_pParent->m_rc.left;
		ppt->y -= m_pParent->m_rc.top;
	}
}

void TComponent::ScreenToComponent(CRect *prc)
{
	if(m_pParent)
	{
		m_pParent->ScreenToComponent(prc);

		prc->left -= m_pParent->m_rc.left;
		prc->top -= m_pParent->m_rc.top;

		prc->right -= m_pParent->m_rc.left;
		prc->bottom -= m_pParent->m_rc.top;
	}	
}

void TComponent::ComponentToScreen(CPoint *ppt)
{
	if(m_pParent)
	{
		m_pParent->ComponentToScreen(ppt);

		ppt->x += m_pParent->m_rc.left;
		ppt->y += m_pParent->m_rc.top;
	}
}

void TComponent::ComponentToScreen(CRect *prc)
{
	if(m_pParent)
	{
		m_pParent->ComponentToScreen(prc);

		prc->left += m_pParent->m_rc.left;
		prc->top += m_pParent->m_rc.top;

		prc->right += m_pParent->m_rc.left;
		prc->bottom += m_pParent->m_rc.top;
	}
}

HRESULT TComponent::Render( DWORD dwTickCount )
{
	if( !m_bVisible ) return S_OK;
	if( !CTachyonRes::m_pDEVICE ) return E_FAIL;

	HRESULT hr = DrawDefault(
		dwTickCount,
		m_rcSrc != CRect( -1, -1, -1, -1 ) ? &m_rcSrc : NULL );

	if(FAILED(hr))
		return hr;

	hr = DrawKids(dwTickCount);
	if(FAILED(hr))
		return hr;

	return DrawText();
}

HRESULT TComponent::DrawKids( DWORD dwTickCount)
{
	if(!CTachyonRes::m_pDEVICE)
		return E_FAIL;

	TCOMP_LIST::iterator it = GetFirstKidsFinder();
	while(!EndOfKids(it))
	{
		TComponent *pKid = GetNextKid(it);

		if(m_style&TS_CUSTOM_COLOR)
		{
			pKid->m_style |= TS_CUSTOM_COLOR;
			pKid->m_dwColor = m_dwColor;
		}
		else
			pKid->m_style &= ~TS_CUSTOM_COLOR;

		if( pKid && ( m_bEnable || !pKid->m_blIsCaret ))
		{
			HRESULT hr = pKid->Render(dwTickCount);
			if(FAILED(hr)) return hr;
		}
	}

	return S_OK;
}

HRESULT TComponent::DrawText()
{
	if(!CTachyonRes::m_pDEVICE)
		return E_FAIL;

	if( m_strText.IsEmpty() || !m_pGDIFont || !m_pFont )
		return S_OK;

	CSize szCTRL(
		m_rc.Width() - (m_flagAlign == ALIGN_CENTER ? 2 * m_nHMargine : m_nHMargine),
		m_rc.Height() - m_nVMargine);

	if( m_strText != m_strPrvText )
	{
		m_vTextLine.clear();

		m_strPrvText = m_strText;
		m_bNeedUpdateTextSetting = TRUE;

		SeparateTextFitSize( m_strText, szCTRL, m_vTextLine );

		m_bDidShrink = FALSE; // 밑에 UpdateTextOutputData에서 m_bDidShrink의 TRUE를 다시 결정한다.
	}

	if( m_bNeedUpdateTextSetting || m_PrvFlagAlign != m_flagAlign || m_rcPrv != m_rc )
	{
		m_PrvFlagAlign = m_flagAlign;
		m_rcPrv = m_rc;

		ClearTextOutputData();

		UpdateTextOutputData( m_vTextLine, szCTRL, m_TextOutputDataSet );

		m_bNeedUpdateTextSetting = FALSE;
	}

	DoTextOut( m_TextOutputDataSet );
	return S_OK;
}

HRESULT TComponent::DrawDefault( DWORD dwTickCount, LPRECT lpRect )
{
	if(!CTachyonRes::m_pDEVICE)
		return E_FAIL;

	LPIMAGESET pImageset = m_bEnable || !m_pDisable ? m_pEnable: m_pDisable;

	if(pImageset)
	{
		CPoint pt = CPoint( m_rc.left, m_rc.top );
		ComponentToScreen( &pt );
		
		CD3DImage *pImage = NULL;

		if( m_blDoSprite )
		{
			m_dwTotalTick += dwTickCount;

			pImageset->m_dwCurTick = m_dwTotalTick % pImageset->m_dwTotalTick;
			pImage = pImageset->GetImage();
		}
		else
		{
			if( m_nSpriteIndex >= pImageset->m_vImage.size() )
				m_nSpriteIndex = 0;

			pImage = pImageset->m_vImage[m_nSpriteIndex];
		}

		if(pImage)
		{
			pImage->SetScaleX(m_fScaleX);
			pImage->SetScaleY(m_fScaleY);

			pImage->Render(
				CTachyonRes::m_pDEVICE->m_pDevice,
				m_style&TS_CUSTOM_COLOR ?
				m_dwColor :
				pImageset->GetColor(),
				pt.x,
				pt.y,
				lpRect);
		}

		if(m_bUseImageClrForText)
			SetTextClr(pImageset->GetColor());
	}

	return S_OK;
}

DWORD TComponent::GetRepeatCount()
{
	LPIMAGESET pImageset = m_bEnable || !m_pDisable ? m_pEnable: m_pDisable;

	if(!pImageset)
		return 0;

	return m_dwTotalTick / pImageset->m_dwTotalTick;
}

void TComponent::ResetAnimation()
{
	m_dwTotalTick = 0;
}

void TComponent::ResetTotalTick( DWORD dwTick)
{
	m_dwTotalTick = dwTick;
}

TComponent* TComponent::FindShrinkInChilds( POINT pt )
{
	if( m_bDidShrink &&
		m_bVisible &&
		HitRect(pt) )
		return this;

	TCOMP_LIST::iterator it;
	for( it=m_kids.begin(); it!=m_kids.end(); it++)
	{
		TComponent *pShrinked = (*it)->FindShrinkInChilds( pt );
		if( pShrinked )
			return pShrinked;
	}

	return NULL;
}
/*
	Message handlers
*/
void TComponent::OnLButtonDown(UINT nFlags, CPoint pt)
{
}

void TComponent::OnLButtonUp(UINT nFlags, CPoint pt)
{
}

void TComponent::OnLButtonDblClk(UINT nFlags, CPoint pt)
{
}

void TComponent::OnMButtonDown(UINT nFlags, CPoint pt)
{
}

void TComponent::OnMButtonUp(UINT nFlags, CPoint pt)
{
}

void TComponent::OnMButtonDblClk(UINT nFlags, CPoint pt)
{
}

void TComponent::OnRButtonDown(UINT nFlags, CPoint pt)
{
}

void TComponent::OnRButtonUp(UINT nFlags, CPoint pt)
{
}

void TComponent::OnRButtonDblClk(UINT nFlags, CPoint pt)
{
}

void TComponent::OnMouseMove(UINT nFlags, CPoint pt)
{
}

BOOL TComponent::DoMouseWheel( UINT nFlags, short zDelta, CPoint pt)
{
	return FALSE;
}

BOOL TComponent::OnSetCursor( CWnd* pWnd, UINT nHitTest, UINT message, CPoint pt)
{
	return FALSE;
}

void TComponent::OnChar( UINT nChar, int nRepCnt, UINT nFlags)
{
	switch(nChar)
	{
	case VK_RETURN	: SAFE_NOTIFY( m_id, TNM_ENTER, 0); break;
	case VK_ESCAPE	: SAFE_NOTIFY( m_id, TNM_ESC, 0); break;
	}
}

void TComponent::OnKeyDown(UINT nChar, int nRepCnt, UINT nFlags)
{
}

void TComponent::OnKeyUp(UINT nChar, int nRepCnt, UINT nFlags)
{
}

void TComponent::OnNotify(DWORD from, WORD msg, LPVOID param)
{
}

void TComponent::OnImeComposition(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
}

void TComponent::OnImeNotify( HWND hWnd, WPARAM wParam, LPARAM lParam )
{
}

BOOL TComponent::OnInputLangChangeRequest( HWND hWnd, WPARAM wParam, LPARAM lParam )
{
	return TRUE;
}

void TComponent::OnSetFocus(TComponent *pLoseFocus)
{
}

void TComponent::OnLoseFocus(TComponent *pSetFocus)
{
}

BOOL TComponent::CanFocus()
{
	return ((m_style & TS_FOCUSABLE) && CanProcess());
}

BOOL TComponent::GetFocus()
{
	return m_bFocus;
}

void TComponent::SetFocus(BOOL bFocus)
{
	m_bFocus = bFocus;
}

DWORD TComponent::SetFocus( DWORD dwID)
{
	return 0;
}

void TComponent::ResetFocus()
{
}

void TComponent::SetStyle(DWORD style)
{
	m_style = style;
}

LP_FRAMEDESC TComponent::RetrieveKidDesc(DWORD id, LP_FRAMEDESC pParent)
{
	LP_FRAMEDESC ptr = pParent->m_pCHILD;
	while(ptr)
	{
		if(ptr->m_vCOMP.m_dwID == id)
			return ptr;
		ptr = ptr->m_pNEXT;	
	}

	return NULL;
}

DWORD TComponent::GetStyle()
{
	return m_style;
}

void TComponent::SetScale(float fScaleX, float fScaleY, BOOL bApplyChildren)
{
	m_fScaleX = fScaleX;
	m_fScaleY = fScaleY;

	if( bApplyChildren )
	{
		TCOMP_LIST::iterator itrComp, endComp;
		itrComp = m_kids.begin();
		endComp = m_kids.end();

		for(; itrComp != endComp; ++itrComp)
		{
			TComponent* pKid = *itrComp;
			pKid->SetScale(fScaleX, fScaleY, TRUE);			
		}
	}
}

void TComponent::MoveComponent( CPoint pt )
{
	CSize size = m_rc.Size();
	m_rc = CRect( pt.x, pt.y, pt.x + size.cx, pt.y + size.cy );

	NotifyComponentUpdate();
}

void TComponent::SetComponentRect( CRect rc )
{
	m_rc = rc;

	NotifyComponentUpdate();
}

void TComponent::SetTextAlign( BYTE bAlign)
{
	m_flagAlign = bAlign;
}

HRESULT TComponent::TextOut(CString &strText, int x, int y)
{
	if(m_pFont)
	{
		m_pFont->SetScaleX(m_fScaleX);
		m_pFont->SetScaleY(m_fScaleY);

		m_pFont->TextOut(
			CTachyonRes::m_pDEVICE->m_pDevice,
			strText, x, y);
	}

	return S_OK;
}

void TComponent::SetTextExtent( int nExtent)
{
	m_nTextExtent = nExtent;
	m_nHMargine = (m_rc.Width() - m_nTextExtent) / 2;

	if( m_nHMargine < 0 )
		m_nHMargine = 0;	
}

int TComponent::GetTextExtent()
{
	return m_nTextExtent;
}

void TComponent::GetTextExtentPoint( LPCTSTR strText, CSize &size)
{
	size = CSize( 0, 0);
	if(!m_pGDIFont)
		return;

	HDC hDC = CreateCompatibleDC(NULL);
	RectF vRECT;

	SelectObject( hDC, m_pGDIFont->m_pFont->m_hObject);
	if( _tcslen(strText) > 0 )
	{
		WCHAR pBUF[TGDIBUFFER_SIZE];

		Graphics vGDI(hDC);
		Font vFONT(hDC);
		// Arab Ver.
		CString strMBCS(strText);
		strMBCS.TrimRight(_T("\n\r"));

		MultiByteToWideChar(
			CP_THREAD_ACP, 0,	
			LPCSTR(strMBCS),
			strMBCS.GetLength() + 1,
			pBUF, TGDIBUFFER_SIZE);

		vGDI.SetTextRenderingHint(TextRenderingHintAntiAlias);
		// Arab Ver.
		vGDI.MeasureString(
			pBUF, -1,
			&vFONT,
			PointF(
			0.0f,
			0.0f),
			&StringFormat(StringFormatFlagsMeasureTrailingSpaces),
			&vRECT);
		vRECT.Width -= 3.0f;
	}
	else
	{
		Graphics vGDI(hDC);
		Font vFONT(hDC);

		vGDI.SetTextRenderingHint(TextRenderingHintAntiAlias);
		vGDI.MeasureString(
			L"X", -1,
			&vFONT,
			PointF(
			0.0f,
			0.0f),
			&vRECT);
		vRECT.Width = 0.0f;
	}

	size = CSize( INT(vRECT.Width), INT(vRECT.Height - 3.0f));
	DeleteDC(hDC);
}

void TComponent::ShrinkText( CString& text)
{
	CString strMBCS = BuildMBCSInfo(text);

	int index = text.GetLength() - 1;
	int nDot2Replace = 2;

	text.Delete(index);
	index--;

	if( index >= 0 && IS_MBCS_LEAD(strMBCS.GetAt(index)) )
	{
		text.SetAt( index--, '.');
		nDot2Replace--;
	}

	while( nDot2Replace > 0 && index >= 0 )
	{
		text.SetAt( index--, '.');
		nDot2Replace--;

		if( index >= 0 && IS_MBCS_LEAD(strMBCS.GetAt(index)) )
		{
			text.SetAt( index--, '.');
			nDot2Replace--;
		}
	}

	m_bDidShrink = TRUE; // 글자가 짤리면 마우스커서를 올려놨을때 풀텍스트가 나오는 기능을 활성화시킴
}

DWORD TComponent::GetID()
{
	return m_id;
}

BOOL TComponent::AreYou(DWORD id)
{
	return (m_id == id);
}

void TComponent::SetSrcRect(CRect rc)
{
	m_rcSrc = rc;
}

void TComponent::GetComponentRectWithKid(LPRECT lpRect)
{
	if(m_bRecalcKidRect)
	{
		*lpRect = m_rc;

		TCOMP_LIST::iterator itr1,end1;
		itr1 = m_kids.begin();
		end1 = m_kids.end();

		for(; itr1 != end1; ++itr1)
		{
			TComponent* pKid = *itr1;

			CRect rcKid;
			pKid->GetComponentRectWithKid(&rcKid);

			rcKid.left += m_rc.left;
			rcKid.right += m_rc.left;
			rcKid.top += m_rc.top;
			rcKid.bottom += m_rc.top;

			lpRect->left = min( lpRect->left, rcKid.left );
			lpRect->top = min( lpRect->top, rcKid.top );
			lpRect->right = max( lpRect->right, rcKid.right );
			lpRect->bottom = max( lpRect->bottom, rcKid.bottom );
		}

		m_rcWithKid = *lpRect;
		m_bRecalcKidRect = FALSE;
	}
	else
		*lpRect = m_rcWithKid;
}

void TComponent::GetComponentRect(LPRECT lpRect)
{
	*lpRect = m_rc;
}

void TComponent::GetComponentSize(LPSIZE lpSize)
{
	CSize size = m_rc.Size();

	if(lpSize)
	{
		lpSize->cx = size.cx;
		lpSize->cy = size.cy;
	}
}

void TComponent::GetComponentPos(LPPOINT lpPt)
{
	if(lpPt)
	{
		lpPt->x = m_rc.left;
		lpPt->y = m_rc.top;
	}
}

void TComponent::GetComponentRealRect( LPRECT lpRect)
{
	GetComponentRect(lpRect);

	lpRect->left	= int( FLOAT(lpRect->left) * m_fScaleX );
	lpRect->right	= int( FLOAT(lpRect->right) * m_fScaleX );
	lpRect->top		= int( FLOAT(lpRect->top) * m_fScaleX );
	lpRect->bottom	= int( FLOAT(lpRect->bottom) * m_fScaleX );
}

void TComponent::GetComponentRealPos( LPPOINT lpPt )
{
	GetComponentPos(lpPt);

	lpPt->x	= int( FLOAT(lpPt->x) * m_fScaleX );
	lpPt->y	= int( FLOAT(lpPt->y) * m_fScaleX );
}

void TComponent::GetComponentRealSize( LPSIZE lpSize )
{
	GetComponentSize(lpSize);

	lpSize->cx	= int( FLOAT(lpSize->cx) * m_fScaleX );
	lpSize->cy	= int( FLOAT(lpSize->cy) * m_fScaleX );
}

BOOL TComponent::IsTypeOf(BYTE bType)
{
	return (m_bType == bType);
}

void TComponent::SetComponentText(LPCTSTR strText)
{
	m_strText = strText;
}

void TComponent::SetComponentINT( int n )
{
	m_strText.Empty();
	m_strText.Format( "%d", n );
}

CString TComponent::GetComponentText()
{
	return m_strText;
}

void TComponent::ClearText()
{
	m_strText.Empty();
}

void TComponent::SetComponentSize(CSize size)
{
	CPoint lt = CPoint(m_rc.left, m_rc.top);

	m_rc.right = m_rc.left + size.cx;
	m_rc.bottom = m_rc.top + size.cy;

	NotifyComponentUpdate();
}

BOOL TComponent::IsVisible()
{
	return m_bVisible;
}

BOOL TComponent::IsEnable()
{
	return m_bEnable;
}

void TComponent::NotifyComponentUpdate()
{
	m_bRecalcKidRect = TRUE;
	
	if( m_pParent )
		m_pParent->NotifyComponentUpdate();
}

LPIMAGESET TComponent::GetDefaultImage()
{
	return (m_bEnable)? m_pEnable: m_pDisable;
}

void TComponent::Create3DFont( CGDIFont *pGDIFont, CGDIFont* pTipGDIFont)
{
	if(!CTachyonRes::m_pDEVICE)
		return;

	if(m_pTipFont)
	{
		delete m_pTipFont;
		m_pTipFont = NULL;
	}

	if(m_pFont)
	{
		delete m_pFont;
		m_pFont = NULL;
	}

	if(pTipGDIFont)
	{
		m_pTipFont = new CD3DFont();
		m_pTipFont->Release();

		m_pTipFont->SetFont(pTipGDIFont->m_pFont);
		m_pTipFont->m_dwColor = pTipGDIFont->m_clr;
	}

	if(pGDIFont)
	{
		m_pFont = new CD3DFont();
		m_pFont->Release();

		m_pFont->SetFont(pGDIFont->m_pFont);
		m_pFont->m_dwColor = pGDIFont->m_clr;
	}
}

void TComponent::SetFont( CGDIFont *pGDIFont, CGDIFont* pTipGDIFont)
{
	m_pTipGDIFont = pTipGDIFont;
	m_pGDIFont = pGDIFont;

	Create3DFont( m_pGDIFont, m_pTipGDIFont);
}

CD3DFont* TComponent::Get3DFont()
{
	return m_pFont;
}

CD3DFont* TComponent::Get3DTipFont()
{
	return m_pTipFont;
}

CGDIFont* TComponent::GetFont()
{
	return m_pGDIFont;
}

CGDIFont* TComponent::GetTipFont()
{
	return m_pTipGDIFont;
}

BOOL TComponent::CanProcess()
{
	return (m_bEnable && m_bVisible );
}

void TComponent::SetSprite( BOOL blDoSprite, int nSpriteIndex )
{
	m_blDoSprite = blDoSprite;
	m_nSpriteIndex = nSpriteIndex;
}

TComponent* TComponent::GetParent()
{
	return m_pParent;
}

void TComponent::ResetParent( TComponent *pParent)
{
	m_pParent = pParent;
}

void TComponent::AddTextSetting(INT iStart, INT iEnd, DWORD dwColor, DWORD dwUser, INT iType)
{
	if( iStart > iEnd )
		iEnd = iStart;

	RemoveTextSetting(iStart, iEnd);

	TextSetting data;
	
	data.iStart = iStart;
	data.iEnd = iEnd;
	data.dwColor = dwColor;
	data.dwUser = dwUser;
	data.iType = iType;

	m_TextSettingSet.insert(data);

	m_bNeedUpdateTextSetting = TRUE;
}
void TComponent::RemoveTextSetting(INT iStart, INT iEnd)
{
	std::vector<TextSetting> vInsertData;

	if( iStart > iEnd )
		iEnd = iStart;

	TextSettingSet::iterator itr = m_TextSettingSet.begin();
	while( itr != m_TextSettingSet.end() )
	{
		TextSetting& data = *itr;

		if( (data.iStart > iEnd) || (data.iEnd < iStart) )
		{
			++itr;
			continue;
		}

		if( iStart > data.iStart )
		{
			if( iEnd < data.iEnd )
			{
				vInsertData.push_back(data);
				itr = m_TextSettingSet.erase(itr);
				continue;
			}
			else
			{
				data.iEnd = iStart - 1;
			}
		}
		else 
		{
			if( iEnd < data.iEnd )
			{
				data.iStart = iEnd + 1;
			}
			else
			{
				itr = m_TextSettingSet.erase(itr);
				continue;
			}
		}

		++itr;
	}

	TextSetting inData;
	for( size_t i=0; i<vInsertData.size(); ++i )
	{
		const TextSetting& data = vInsertData[i];
		inData.dwColor = data.dwColor;
		inData.dwUser = data.dwUser;

		inData.iStart = data.iStart;
		inData.iEnd = iStart - 1;
		m_TextSettingSet.insert( inData );

		inData.iStart = iEnd + 1;
		inData.iEnd = data.iEnd;
		m_TextSettingSet.insert( inData );
	}

	m_bNeedUpdateTextSetting = TRUE;
}
void TComponent::RemoveTextSetting(const TextSetting& data)
{
	TextSettingSet::iterator itr = m_TextSettingSet.find(data);
	if( itr != m_TextSettingSet.end() )
		m_TextSettingSet.erase(itr);

	m_bNeedUpdateTextSetting = TRUE;
}
void TComponent::ResetTextSetting()
{
	m_TextSettingSet.clear();
	m_bNeedUpdateTextSetting = TRUE;
}
BOOL TComponent::FindTextSetting(INT iIndex, TextSetting& outSetting)
{
	TextSettingSet::iterator itr, end;
	itr = m_TextSettingSet.begin();
	end = m_TextSettingSet.end();

	for( ; itr != end; ++itr )
	{
		const TextSetting& data = *itr;
		
		if( data.iStart > iIndex )
			break;

		if( data.iEnd >= iIndex )
		{
			outSetting = data;
			return TRUE;
		}
	}

	return FALSE;
}

TComponent::TextSettingSet::iterator TComponent::GetTextSettingItr(INT iIndex)
{
	TextSettingSet::iterator itr, end;
	itr = m_TextSettingSet.begin();
	end = m_TextSettingSet.end();

	for( ; itr != end; ++itr )
	{
		if( itr->iStart >= iIndex )
			return itr;
	}

	return end;
}

TComponent::TextSettingSet::iterator TComponent::GetTextSettingItrBegin()
{
	return m_TextSettingSet.begin();
}

TComponent::TextSettingSet::iterator TComponent::GetTextSettingItrEnd()
{
	return m_TextSettingSet.end();
}

BOOL TComponent::HitTestTextSetting(const CPoint& pt, DWORD* outUser)
{
	if(!m_bVisible)
		return FALSE;

	TCOMP_LIST::iterator itr1,end1;
	itr1 = m_kids.begin();
	end1 = m_kids.end();

	for(; itr1 != end1; ++itr1)
	{
		TComponent* pKid = *itr1;

		if( pKid->HitTestTextSetting(pt,outUser) )
			return TRUE;
	}

	if( m_TextOutputDataSet.empty() )
		return FALSE;

	CRect rc = m_rc;
	ComponentToScreen(&rc);
	if( !rc.PtInRect(pt) )
		return FALSE;

	TextOutputDataSet::iterator itr2,end2;
	itr2 = m_TextOutputDataSet.begin();
	end2 = m_TextOutputDataSet.end();

	for(; itr2 != end2; ++itr2)
	{
		const TextOutputData& data = *itr2;

		rc = CRect( data.ptPoint, data.szSize );
		ComponentToScreen(&rc);
		if( rc.PtInRect(pt) )
		{
			if( outUser )
				*outUser = data.dwUser;

			return TRUE;
		}
	}

	return FALSE;
}
HRESULT TComponent::TextOut(const TextOutputData& ptd)
{
	CD3DFont* pFont = ptd.pFont? ptd.pFont: m_pFont;

	if(pFont)
	{
		CPoint pt = ptd.ptPoint;
		ComponentToScreen(&pt);

		m_pFont->SetScaleX(m_fScaleX);
		m_pFont->SetScaleY(m_fScaleY);

		pFont->TextOut(
			CTachyonRes::m_pDEVICE->m_pDevice,
			ptd.strText, pt.x, pt.y);

		if( ptd.pTextLine)
			ptd.pTextLine->Draw( CTachyonRes::m_pDEVICE->m_pDevice);
	}

	return S_OK;
}

#ifdef DEBUG
void TComponent::_LOG_TEXT_SETTING(LPCSTR func, LPCSTR file, LONG line)
{
	static CString str, temp;

	str  = "----------------------------------------------------------------------\n";
	temp.Format("< %s [FILE:%s, LINE:%u] >\n\n" ,func,file,line);
	str += temp;

	TextSettingSet::iterator itr,end;
	itr = m_TextSettingSet.begin();
	end = m_TextSettingSet.end();

	for(INT i=0; itr!=end; ++itr, ++i)
	{
		temp.Format("#%d START(%d) END(%d) COLOR(0x%x)\n", i, itr->iStart, itr->iEnd, itr->dwColor);
		str += temp;
	}

	str += "----------------------------------------------------------------------\n";

	::OutputDebugString(str);
}
#endif

void TComponent::ClearTextOutputData()
{
	TextOutputDataSet::iterator itr,end;
	itr = m_TextOutputDataSet.begin();
	end = m_TextOutputDataSet.end();

	for(; itr!=end; ++itr)
	{
		if( itr->pFont )
			delete itr->pFont;

		if( itr->pTextLine )
			delete itr->pTextLine;
	}

	m_TextOutputDataSet.clear();
}

VOID TComponent::SeparateTextFitSize( CString& strORG, CSize szCTRL, VECTORSTRING& outRESULT, BOOL bAcc )
{
	// 문단나눔('\n')과 szCTRL.cy 길이에 맞게 strORG 문자를 잘라서
	// 첫줄부터 마지막줄까지 outResult에 순서대로 담는다.

	if( !bAcc )
		outRESULT.clear();

	CSize szSIZE;
	CString strLEFT = strORG;
	CString strTEMP, strLINE;

	if( m_bUseWordBreak &&
		m_bWordBreak)
	{
		while(!strLEFT.IsEmpty())
		{
			strTEMP.Empty();

			GetTextExtentPoint(strLEFT, szSIZE);
			if( szSIZE.cx > szCTRL.cx )
			{
				strLINE.Empty();
				CString strWORD;

				while(!strLEFT.IsEmpty())
				{
					CHAR c = strLEFT.GetAt(0);

					if( c == ' ' || c == '\n' )
					{
						if( !strWORD.IsEmpty() )
						{
							// 시험에 들게 하자.
							CString strTEST(strLINE);
							strTEST.Append( strWORD );
							GetTextExtentPoint( strTEST, szSIZE );
							if( szSIZE.cx > szCTRL.cx )
							{
								// 이 단어를 포함할순없다.
								strWORD.Append( strLEFT );
								strLEFT = strWORD;
								strWORD.Empty();
								break;
							}
							else
							{
								strLINE.Append( strWORD );
								strWORD.Empty();
							}
						}

						if( c == '\n' )
						{
							strLEFT.Delete(0);
							break;
						}

						CString strTEST(strLINE);
						strTEST.AppendChar( c );
						GetTextExtentPoint( strTEST, szSIZE );
						if( szSIZE.cx > szCTRL.cx )
							break;
						else
						{
							strLEFT.Delete(0);
							strLINE.AppendChar( c );
						}
					}
					else
					{
						strWORD.AppendChar(c);

						BYTE bMBCS = MBCS_LEAD_TEST(c);
						if(bMBCS)
							strWORD.AppendChar(strLEFT.GetAt(1));

						strLEFT.Delete(0);
						if(bMBCS)
							strLEFT.Delete(0);

						CSize sizeWORD;
						GetTextExtentPoint( strWORD, sizeWORD);

						if( sizeWORD.cx > szCTRL.cx ) // 한 단어 길이가 컴포넌트의 가로길이보다 길다. 이럴땐 무조건 자르기.
						{
							CSize sizeLINE;

							GetTextExtentPoint( strLINE, sizeLINE);
							INT nRemainSize = szCTRL.cx - sizeLINE.cx;

							CString strNewWord;
							for( INT i=0; i<strWORD.GetLength(); i++)
							{
								CString strCHAR;
								CHAR c = strWORD.GetAt(i);

								strCHAR.AppendChar(c);
								if(MBCS_LEAD_TEST(c))
									strCHAR.AppendChar(strWORD.GetAt(++i));

								CString strTEST(strNewWord);
								strTEST.Append(strCHAR);
								GetTextExtentPoint( strTEST, szSIZE);

								if( szSIZE.cx < nRemainSize )
									strNewWord.Append(strCHAR);
								else
									break;
							}

							// strNewWord만 strLINE에 포함, 나머진 strLEFT로 되돌아감.
							strWORD = strWORD.Right( strWORD.GetLength() - strNewWord.GetLength() );
							strWORD.Append( strLEFT );
							strLEFT = strWORD;
							strWORD.Empty();

							strLINE.Append( strNewWord );
							break;
						}
					}
				}

				if( !strWORD.IsEmpty() )
				{
					// 시험에 들게 하자.
					CString strTEST(strLINE);
					strTEST.Append( strWORD );
					GetTextExtentPoint( strTEST, szSIZE );
					if( szSIZE.cx > szCTRL.cx )
					{
						// 이 단어를 포함할순없다.
						strWORD.Append( strLEFT );
						strLEFT = strWORD;
						strWORD.Empty();
					}
					else
					{
						strLINE.Append( strWORD );
						strWORD.Empty();
					}
				}
			}
			else
			{
				strLINE = strLEFT;
				strLEFT.Empty();
			}
		
			outRESULT.push_back( strLINE );
		}
	}
	else
	{
		CSize szTEXT;
		CString strTEXT, strFORMAT;

		INT nPOS(0);
		strTEXT = strORG.Tokenize("\n", nPOS);

		while(!strTEXT.IsEmpty())
		{
			strTEXT.Remove('\r');

			while(!strTEXT.IsEmpty())
			{
				GetTextExtentPoint(strTEXT, szTEXT);
				CString strMBCS = BuildMBCSInfo(strTEXT);
				strFORMAT = strTEXT;

				while( szTEXT.cx > szCTRL.cx ) // 글자를 한개씩 빼면서 szCTRL.cy 안에 들어갈 때까지 자른다.
				{
					int nLength = strFORMAT.GetLength() - 1;

					strFORMAT.Delete(nLength);
					nLength--;

					if( nLength >= 0 && IS_MBCS_LEAD(strMBCS.GetAt(nLength)) )
						strFORMAT.Delete(nLength);

					GetTextExtentPoint(strFORMAT, szTEXT);
				}

				int nIndex = strTEXT.GetLength() - strFORMAT.GetLength();
				if( nIndex > 0 )
					strTEXT = strTEXT.Right(nIndex);
				else
					strTEXT.Empty();

				outRESULT.push_back(strFORMAT);
				strFORMAT.Empty();
			}

			strTEXT = strORG.Tokenize("\n", nPOS);
		}
	}
}

VOID TComponent::UpdateTextOutputData( VECTORSTRING& vTextLine, const CSize szCTRL, TextOutputDataSet& outRESULT )
{
	if( !vTextLine.empty() )
	{
		CString strTEXT;
		TextOutputData sTextOutputData;

		CSize szTEXT;
		GetTextExtentPoint(vTextLine.back(), szTEXT);

		int nLINE = INT(min( szCTRL.cy, szTEXT.cy) * 1.2f);			// 한 줄의 높이(픽셀)
		int nTEXT = INT(vTextLine.size());							// 출력해야될 전체 라인수
		nLINE = min( szCTRL.cy, nLINE);

		int nSLOT = min(szCTRL.cy/nLINE, nTEXT);					// 출력할 수 있는 최종 라인수
		int nSIZE = m_rc.top + m_nVMargine + (nLINE - szTEXT.cy) / 2 + (m_bVCenter ? (szCTRL.cy - nSLOT * nLINE) / 2 : 0);

		if( m_bShrink && nTEXT > nSLOT && nSLOT > 0 )
		{
			vTextLine.erase( vTextLine.begin()+nSLOT, vTextLine.end() );
			ShrinkText(vTextLine.back());
		}

		TextSettingSet::iterator itr,end;
		itr = m_TextSettingSet.begin();
		end = m_TextSettingSet.end();

		INT iSTART(0), iEND(0);
		for( INT i=0; i<nSLOT; ++i )
		{
			CPoint point;
			CSize size;

			GetTextExtentPoint(vTextLine[i], size);
			point.y = nSIZE + i * nLINE;

			switch(m_flagAlign)
			{
			case ALIGN_CENTER	: point.x = m_rc.left + m_nHMargine + (szCTRL.cx - size.cx) / 2; break;
			case ALIGN_RIGHT	: point.x = m_rc.right - size.cx - m_nHMargine; break;
			default				: point.x = m_rc.left + m_nHMargine; break;
			}

			sTextOutputData.iLine = i;
			sTextOutputData.iDepth = 0;
			sTextOutputData.dwUser = 0;
			sTextOutputData.ptPoint = point;
			sTextOutputData.ptPoint.x += 1;
			sTextOutputData.strText = vTextLine[i];
			sTextOutputData.pFont = NULL;

			outRESULT.insert(sTextOutputData);

			iEND += vTextLine[i].GetLength();

			sTextOutputData.iDepth = 1;

			while( itr != end )
			{
				const TextSetting& data = *itr;
				if( data.iStart > iEND )
					break;

				INT s = data.iStart - iSTART;
				if( s > 0 )
				{
					strTEXT = vTextLine[i].Left(s);
					GetTextExtentPoint(strTEXT, szTEXT);
				}
				else
					szTEXT.cx = 0;

				sTextOutputData.dwUser  = data.dwUser;
				sTextOutputData.ptPoint = point + CPoint(szTEXT.cx,0);

				sTextOutputData.pFont = new CD3DFont();
				sTextOutputData.pFont->Release();
				sTextOutputData.pFont->SetFont(m_pGDIFont->m_pFont);
				sTextOutputData.pFont->m_dwColor = data.dwColor; 

				if( iEND < data.iEnd )
				{
					if( s < 0 )
						s = 0;

					sTextOutputData.strText = vTextLine[i].Right(vTextLine[i].GetLength() - s);
					GetTextExtentPoint(sTextOutputData.strText, sTextOutputData.szSize);
					outRESULT.insert(sTextOutputData);
					break;
				}
				else
				{
					if( s > 0 )
						sTextOutputData.strText = vTextLine[i].Mid(s, data.iEnd-data.iStart+1);
					else
						sTextOutputData.strText = vTextLine[i].Left(data.iEnd-iSTART+1);
				}

				GetTextExtentPoint(sTextOutputData.strText, sTextOutputData.szSize);
				outRESULT.insert(sTextOutputData);

				++itr;
			}

			iSTART = iEND;
		}
	}
}

VOID TComponent::DoTextOut( TextOutputDataSet& vTextOutputDataSet )
{
	TextOutputDataSet::iterator itr,end;
	itr = vTextOutputDataSet.begin();
	end = vTextOutputDataSet.end();

	for( ; itr!=end; ++itr )
		TextOut(*itr);
}

void TComponent::OnLostDevice()
{
	TCOMP_LIST::iterator it = GetFirstKidsFinder();
	while(!EndOfKids(it))
	{
		TComponent *pKid = GetNextKid(it);
		if( pKid )
			pKid->OnLostDevice();
	}
}