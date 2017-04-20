#include "stdafx.h"
#include "TClientWnd.h"
#include "Resource.h"

static FLOAT RealignRot( FLOAT fRadian )
{
	if( fRadian < 0 )
	{
		while(fRadian < 0)
			fRadian += D3DX_PI*2.0f;
	}

	if( fRadian >= D3DX_PI*2.0f )
	{
		while(fRadian >= D3DX_PI*2.0f)
			fRadian -= D3DX_PI*2.0f;
	}

	return fRadian;
}

CTClientNET::CTClientNET()
{
	m_pDevice = NULL;
	m_pLIGHT = NULL;
	m_pHost = NULL;
	m_pRES = NULL;
	m_nCamZOOM = 0;
	m_bSelServerNum = (BYTE)T_INVALID;
	m_pCandidate = NULL;
	m_bClickedIDBox = FALSE;

	for( int i=0 ; i < 6 ; ++i )
	{
		m_vMODEL[ i ] = NULL;
		D3DXMatrixIdentity(&m_matMODEL[ i ]);
	}

	m_nModelCount = 0;
	m_dwBackID = 0;
	m_bWndAni = FALSE;
	m_bWndAniType = 0;
	m_dwWndAni = 0;
	m_dwWndAniMax = 0;
	m_dwWndAniGM = 0;
	m_bTickReset = FALSE;
	m_pChannelDLG = NULL;

	m_fTargetROT = 0.0f;
	m_fRotDIR = 0.0f;
	m_bROT = FALSE;
	m_bCharROT = FALSE;
	m_fROT = 0.0f;
	m_fCharROT = 0.0f;
	m_bAniCharRot = FALSE;
	m_fAniCharRot = 0.0f;
	m_bTargetROT = FALSE;
}

CTClientNET::~CTClientNET()
{
	if( m_pChannelDLG )
		delete m_pChannelDLG;
	m_pChannelDLG = NULL;
}

void CTClientNET::ResetSlot()
{
	m_fTargetROT = 0.0f;
	m_fRotDIR = 0.0f;
	m_bROT = FALSE;
	m_bCharROT = FALSE;
	m_fROT = 0.0f;
	m_fCharROT = 0.0f;
	m_bAniCharRot = FALSE;
	m_fAniCharRot = 0.0f;
	m_bTargetROT = FALSE;
}

void CTClientNET::InitCameraLOGIN()
{
	m_vCamera.InitCamera(
		m_pDevice->m_pDevice,
		1.0f,
		1000.0f,
		0.907571f,
		m_pDevice->m_option.m_dwScreenX,
		m_pDevice->m_option.m_dwScreenY);

	CTClientObjBase::m_fCamDist = 0.0f;
	m_vCamera.m_bCamRotSPEED = TNETCAM_ROTSPEED;
	m_vCamera.m_fZoomSPEED = TNETCAM_ZOOMSPEED;
	m_vCamera.m_wCamPITCH = 0;
	m_nCamZOOM = 0;

	m_vCamera.m_fZoomMIN = 1.0f;
	m_vCamera.m_fZoomMAX = 1.0f;

	m_vCamera.m_fCamHEIGHT = 0.0f;
	m_vCamera.m_fCamDIST = 1.0f;	

	m_vCamera.SetPosition(
		D3DXVECTOR3( 0.0f, 23.900620f, -42.803051f ),
		D3DXVECTOR3( 0.0f, 18.979271f, -4.81124f ),
		D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) );

	m_vPanPosition = D3DXVECTOR3( -0.13f, 0.1f, 3.33f );
	m_fPanRot =  -16.2f;
	m_fPanDist = -3.85f;
}

void CTClientNET::InitCameraCHAR()
{
	m_vCamera.InitCamera(
		m_pDevice->m_pDevice,
		0.1f,
		1000.0f,
		83.0f * D3DX_PI / 180.0f,
		m_pDevice->m_option.m_dwScreenX,
		m_pDevice->m_option.m_dwScreenY);

	CTClientObjBase::m_fCamDist = 0.0f;
	m_vCamera.m_fZoomMIN = 1.0f;
	m_vCamera.m_fZoomMAX = 1.0f;

	m_vCamera.m_fCamHEIGHT = 0.0f;
	m_vCamera.m_fCamDIST = 1.0f;

	m_vCamera.m_bCamRotSPEED = TNETCAM_ROTSPEED;
	m_vCamera.m_fZoomSPEED = TNETCAM_ZOOMSPEED;
	m_vCamera.m_wCamPITCH = 0;
	m_nCamZOOM = 0;

	m_vCamera.SetPosition(
		D3DXVECTOR3( 1.6f, 1.471f, -2.786f ),
		D3DXVECTOR3( 1.385f, 1.447f, -1.917f ),
		D3DXVECTOR3(0.0f, 1.0f, 0.0f) );

	m_vPanPosition = D3DXVECTOR3( -0.130f, 0.1f, 3.330f );
	m_fPanRot =  -16.2f;
	m_fPanDist = -3.850f;
}

void CTClientNET::InitMODEL( LPTCHARINFO pINFO, INT nIndex )
{
	if( m_vMODEL[ nIndex ] == NULL )
		m_vMODEL[ nIndex ] = new CTClientChar;

	CTClientChar* pMODEL = m_vMODEL[ nIndex ];

	LPOBJECT pPREV = pMODEL->m_OBJ.m_pOBJ;
	LPOBJECT pNEXT = NULL;
	BYTE bContryID = pMODEL->m_bContryID;

	pMODEL->ReleaseData();

	pMODEL->m_bContryID = pINFO->m_bContry;
	pMODEL->m_strNAME = pINFO->m_strNAME;
	pMODEL->m_bClassID = pINFO->m_bClass;
	pMODEL->m_bRaceID_ = pINFO->m_bRace;
	pMODEL->m_bLevel = pINFO->m_bLevel;
	pMODEL->m_bPants = pINFO->m_bPants;
	pMODEL->m_bHair = pINFO->m_bHair;
	pMODEL->m_bFace = pINFO->m_bFace;
	pMODEL->m_bBody = pINFO->m_bBody;
	pMODEL->m_bHand = pINFO->m_bHand;
	pMODEL->m_bFoot = pINFO->m_bFoot;
	pMODEL->m_bSex = pINFO->m_bSex;
	pMODEL->m_dwID = pINFO->m_dwID;

	if( pMODEL->m_pGuildMark )
		delete pMODEL->m_pGuildMark;

	pMODEL->m_pGuildMark = new CTClientGuildMark(
		TGMARK_MARK(pINFO->m_dwFame),
		TGMARK_BACK(pINFO->m_dwFame),
		TGMCOLOR_MARK(pINFO->m_dwFameColor),
		TGMCOLOR_BACK1(pINFO->m_dwFameColor),
		TGMCOLOR_BACK2(pINFO->m_dwFameColor) );

	pNEXT = m_pRES->GetOBJ( CTChart::m_vTRACE[pINFO->m_bRace][pINFO->m_bSex]);
	if( pPREV != pNEXT )
	{
		pMODEL->InitOBJ(pNEXT);

		TACTION vActionID = pMODEL->FindActionID(
			TA_STAND,
			WT_NORMAL);

		pMODEL->SetAction(
			vActionID.m_dwActID,
			vActionID.m_dwAniID);

		pMODEL->InitSIZE(
			pMODEL->GetAttrFLOAT(ID_SIZE_X),
			pMODEL->GetAttrFLOAT(ID_SIZE_Y),
			pMODEL->GetAttrFLOAT(ID_SIZE_Z),
			0.0f, 0.0f);

		D3DXMatrixIdentity(&pMODEL->m_vWorld);
	}

	pMODEL->m_mapTINVEN.insert( MAPTINVEN::value_type( INVEN_EQUIP, &pINFO->m_vEQUIP));
	pMODEL->ClearEquip();
	pMODEL->ResetEQUIP( m_pDevice, m_pRES);
	pMODEL->m_mapTINVEN.clear();
}

void CTClientNET::CalcModelLocal( INT nCount )
{
	FLOAT fRad = 0.0f;
	for( int i=0 ; i < nCount ; ++i )
	{
		D3DXMATRIX vPOS, vROT;

		D3DXMatrixTranslation(
			&vPOS,
			0.0f, 0.0f, m_fPanDist );

		D3DXMatrixRotationY(
			&vROT,
			fRad );

		fRad -= D3DX_PI * 2.0f / FLOAT(nCount);
		m_matMODEL[ i ] = vPOS * vROT;
	}
}

void CTClientNET::InitMODEL( VTCHARINFO& vCHARINFO )
{
	m_nModelCount = vCHARINFO.size();

	CalcModelLocal( m_nModelCount );

	for( int i=0 ; i < m_nModelCount ; ++i )
		InitMODEL( vCHARINFO[ i ], i );
}

void CTClientNET::ReleaseMODEL( INT nIndex )
{
	if( m_vMODEL[ nIndex ] )
		delete m_vMODEL[ nIndex ];
	m_vMODEL[ nIndex ] = NULL;
}

void CTClientNET::InitLIGHT( BYTE bCountryID)
{
	D3DMATERIAL9 material;
	material.Ambient.r = 1.0f;
	material.Ambient.g = 1.0f;
	material.Ambient.b = 1.0f;
	material.Ambient.b = 1.0f;
	material.Diffuse.r = 0.0f;
	material.Diffuse.g = 0.0f;
	material.Diffuse.b = 0.0f;
	material.Diffuse.a = 0.0f;
	material.Emissive = material.Specular = material.Diffuse;
	material.Power = 0.0f;

	m_pDevice->m_pDevice->SetMaterial( &material );

	for( int i=0 ; i < TLIGHT_COUNT ; ++i )
		m_pDevice->m_pDevice->LightEnable( i, FALSE);

	m_pDevice->m_pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
}

void CTClientNET::InitEffectLIGHT()
{
	D3DMATERIAL9 material;
	material.Ambient.r = 1.0f;
	material.Ambient.g = 1.0f;
	material.Ambient.b = 1.0f;
	material.Ambient.b = 1.0f;
	material.Diffuse.r = 0.0f;
	material.Diffuse.g = 0.0f;
	material.Diffuse.b = 0.0f;
	material.Diffuse.a = 0.0f;
	material.Emissive = material.Specular = material.Diffuse;
	material.Power = 0.0f;

	m_pDevice->m_pDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

	m_pDevice->m_pDevice->SetMaterial( &material );

	for( int i=0 ; i < TLIGHT_COUNT ; ++i )
		m_pDevice->m_pDevice->LightEnable( i, FALSE);

	D3DLIGHT9 light;
	::ZeroMemory( &light, sizeof(D3DLIGHT9) );
	light.Ambient.r = 1.0f;
	light.Ambient.g = 1.0f;
	light.Ambient.b = 1.0f;
	light.Ambient.a = 1.0f;
	light.Type = D3DLIGHT_DIRECTIONAL;
	m_pDevice->m_pDevice->SetLight( 0, &light );
	m_pDevice->m_pDevice->LightEnable( 0, TRUE);

	DWORD dwAmbient = D3DCOLOR_XRGB(128, 128, 128);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_AMBIENT, dwAmbient );
}

void CTClientNET::InitModelLIGHT(D3DXMATRIX& matWorld, BYTE bSelected)
{
	m_pDevice->m_pDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

	D3DMATERIAL9 material;
	::ZeroMemory( &material, sizeof(material) );
	material.Ambient.r = 0.0f;
	material.Ambient.g = 0.0f;
	material.Ambient.b = 0.0f;
	material.Ambient.b = 0.0f;
	material.Emissive = material.Specular = material.Ambient;
	material.Diffuse.r = 1.0f;
	material.Diffuse.g = 1.0f;
	material.Diffuse.b = 1.0f;
	material.Diffuse.a = 1.0f;
	material.Power = 0.0f;

	for( int i=0 ; i < TLIGHT_COUNT ; ++i )
		m_pDevice->m_pDevice->LightEnable( i, FALSE);

	if( bSelected )
	{
		{
			D3DLIGHT9 light;
			::ZeroMemory( &light, sizeof(D3DLIGHT9) );
			D3DXVECTOR3 vDir( m_vCamera.m_vTarget - m_vCamera.m_vPosition );
			D3DXVec3Normalize(&vDir, &vDir);
			light.Direction.x = vDir.x;
			light.Direction.y = vDir.y;
			light.Direction.z = vDir.z;
			light.Type = D3DLIGHT_DIRECTIONAL;
			light.Diffuse.r = 0.6f;
			light.Diffuse.g = 0.6f;
			light.Diffuse.b = 0.6f;
			light.Diffuse.a = 1.0f;
			m_pDevice->m_pDevice->SetLight( 0, &light );
			m_pDevice->m_pDevice->LightEnable( 0, TRUE);
		}
		{
			D3DLIGHT9 light;
			::ZeroMemory( &light, sizeof(D3DLIGHT9) );
			D3DXVECTOR3 vDir( m_vCamera.m_vTarget - m_vCamera.m_vPosition );
			D3DXVec3Normalize(&vDir, &vDir);
			light.Direction.x = vDir.x;
			light.Direction.y = vDir.y;
			light.Direction.z = vDir.z;
			light.Type = D3DLIGHT_DIRECTIONAL;
			light.Diffuse.r = 0.6f;
			light.Diffuse.g = 0.6f;
			light.Diffuse.b = 0.6f;
			light.Diffuse.a = 1.0f;
			light.Ambient.r = 0.4f;
			light.Ambient.g = 0.4f;
			light.Ambient.b = 0.4f;
			light.Ambient.a = 1.0f;

			m_pDevice->m_pDevice->SetLight( 1, &light );
			m_pDevice->m_pDevice->LightEnable( 1, TRUE);

			material.Ambient.r = 1.0f;
			material.Ambient.g = 1.0f;
			material.Ambient.b = 1.0f;
			material.Ambient.a = 1.0f;		
		}
	}
	else
	{
		{
			D3DLIGHT9 light;
			::ZeroMemory( &light, sizeof(D3DLIGHT9) );
			D3DXVECTOR3 vDir( m_vCamera.m_vTarget - m_vCamera.m_vPosition );
			D3DXVec3Normalize(&vDir, &vDir);
			light.Direction.x = vDir.x;
			light.Direction.y = vDir.y;
			light.Direction.z = vDir.z;
			light.Type = D3DLIGHT_DIRECTIONAL;
			light.Diffuse.r = MODEL_LIGHT1_DIFFUSE_R / 255.0f;
			light.Diffuse.g = MODEL_LIGHT1_DIFFUSE_G / 255.0f;
			light.Diffuse.b = MODEL_LIGHT1_DIFFUSE_B / 255.0f;
			light.Diffuse.a = 1.0f;
			m_pDevice->m_pDevice->SetLight( 0, &light );
			m_pDevice->m_pDevice->LightEnable( 0, TRUE);
		}

		{
			D3DLIGHT9 light;
			::ZeroMemory( &light, sizeof(D3DLIGHT9) );
			light.Position.x = m_vPanPosition.x;
			light.Position.y = m_vPanPosition.y;
			light.Position.z = m_vPanPosition.z;
			light.Ambient.r = 0.0f;
			light.Ambient.g = 0.0f;
			light.Ambient.b = 0.0f;
			light.Ambient.a = 0.0f;
			light.Diffuse.r = MODEL_LIGHT2_DIFFUSE_R / 255.0f;
			light.Diffuse.g = MODEL_LIGHT2_DIFFUSE_G / 255.0f;
			light.Diffuse.b = MODEL_LIGHT2_DIFFUSE_B / 255.0f;
			light.Diffuse.a = 0.0f;
			light.Attenuation0 = 1.0f;
			light.Attenuation1 = 0.0f;
			light.Attenuation2 = 0.0f;
			light.Type = D3DLIGHT_POINT;
			light.Range = 5.0f;

			m_pDevice->m_pDevice->SetLight( 1, &light );
			m_pDevice->m_pDevice->LightEnable( 1, TRUE);
		}
	}

	m_pDevice->m_pDevice->SetMaterial( &material );
}

void CTClientNET::CalcMODEL( DWORD dwTick)
{
	D3DXMATRIX matWorld;
	D3DXMatrixRotationY(&matWorld, m_fROT + (D3DX_PI / 180.0f * m_fPanRot) );
	matWorld._41 = m_vPanPosition.x;
	matWorld._42 = m_vPanPosition.y;
	matWorld._43 = m_vPanPosition.z;

	D3DXMATRIX matCharRot;
	D3DXMatrixRotationY(&matCharRot, m_fCharROT);
	INT nSlotID = (m_dwLevel == ID_FRAME_CHAR_NEW) ? m_pHost->m_bSlotID : m_pHost->m_vNEWTCHAR.m_bRace;

	for( int i=0 ; i < 6 ; ++i )
	{
		if( m_vMODEL[ i ] ) 
		{
			CTachyonObject *pTOBJ = (CTachyonObject *) m_vMODEL[ i ];
			pTOBJ->CalcTick( m_pDevice->m_pDevice, dwTick );

			if( i != nSlotID )
				m_vMODEL[ i ]->SetPosition( m_matMODEL[ i ] * m_vMODEL[ i ]->GetPositionMat() * matWorld );
			else
				m_vMODEL[ i ]->SetPosition( matCharRot * m_matMODEL[ i ] * m_vMODEL[ i ]->GetPositionMat() * matWorld );
		}
	}
}

void CTClientNET::ResetLight()
{
	D3DLIGHT9 light;
	::ZeroMemory( &light, sizeof(light) );
	for( int i=0 ; i < TLIGHT_COUNT ; ++i )
	{
		m_pDevice->m_pDevice->SetLight( i, &light );
		m_pDevice->m_pDevice->LightEnable( i, TRUE );
	}

	D3DMATERIAL9 mtrl = {
		{ 1.0f, 1.0f, 1.0f, 1.0f},
		{ 1.0f, 1.0f, 1.0f, 1.0f},
		{ 0.0f, 0.0f, 0.0f, 0.0f},
		{ 0.0f, 0.0f, 0.0f, 0.0f},
		0.0f};
	m_pDevice->m_pDevice->SetMaterial(&mtrl);

	if(!m_pDevice->m_bEnableSHADER)
		return;

	static int vLightCount[4] = { TLIGHT_COUNT, 0, 1, 0};
	FLOAT vCONST[16];

	m_pDevice->m_pDevice->SetVertexShaderConstantF(
		m_pDevice->m_vConstantVS[VC_MTRLAMBIENT],
		(FLOAT *) &D3DXVECTOR4(
		mtrl.Ambient.r,
		mtrl.Ambient.g,
		mtrl.Ambient.b,
		mtrl.Ambient.a), 1);

	m_pDevice->m_pDevice->SetVertexShaderConstantF(
		m_pDevice->m_vConstantVS[VC_MTRLDIFFUSE],
		(FLOAT *) &D3DXVECTOR4(
		mtrl.Diffuse.r,
		mtrl.Diffuse.g,
		mtrl.Diffuse.b,
		mtrl.Diffuse.a), 1);

	m_pDevice->m_pDevice->SetVertexShaderConstantF(
		m_pDevice->m_vConstantVS[VC_AMBIENT],
		(FLOAT *) &D3DXVECTOR4(
		1.0f,
		1.0f,
		1.0f,
		1.0f), 1);

	m_pDevice->m_pDevice->SetVertexShaderConstantF(
		m_pDevice->m_vConstantVS[VC_CAMPOS],
		(FLOAT *) &D3DXVECTOR4(
		m_vCamera.m_vPosition.x,
		m_vCamera.m_vPosition.y,
		m_vCamera.m_vPosition.z,
		0.0f), 1);

	D3DXMatrixTranspose( (LPD3DXMATRIX) vCONST, &(m_vCamera.m_matView * m_vCamera.m_matProjection));
	m_pDevice->m_pDevice->SetVertexShaderConstantF(
		m_pDevice->m_vConstantVS[VC_PROJ],
		vCONST, 4);

	m_pDevice->m_pDevice->SetVertexShaderConstantI(
		m_pDevice->m_vConstantVS[VC_LIGHTCOUNT],
		vLightCount, 1);

	for( int i=0; i<TLIGHT_COUNT; i++)
	{
		memcpy( &vCONST[i * 4], &D3DXVECTOR4(
			m_pLIGHT[i].m_Light.Ambient.r,
			m_pLIGHT[i].m_Light.Ambient.g,
			m_pLIGHT[i].m_Light.Ambient.b,
			m_pLIGHT[i].m_Light.Ambient.a),
			4 * sizeof(FLOAT));
	}

	m_pDevice->m_pDevice->SetVertexShaderConstantF(
		m_pDevice->m_vConstantVS[VC_LIGHTAMBIENT],
		vCONST, TLIGHT_COUNT);

	for(int i=0; i<TLIGHT_COUNT; i++)
	{
		memcpy( &vCONST[i * 4], &D3DXVECTOR4(
			m_pLIGHT[i].m_Light.Diffuse.r,
			m_pLIGHT[i].m_Light.Diffuse.g,
			m_pLIGHT[i].m_Light.Diffuse.b,
			m_pLIGHT[i].m_Light.Diffuse.a),
			4 * sizeof(FLOAT));
	}

	m_pDevice->m_pDevice->SetVertexShaderConstantF(
		m_pDevice->m_vConstantVS[VC_LIGHTDIFFUSE],
		vCONST, TLIGHT_COUNT);

	for(int i=0; i<TLIGHT_COUNT; i++)
	{
		memcpy( &vCONST[i * 4], &D3DXVECTOR4(
			m_pLIGHT[i].m_Light.Direction.x,
			m_pLIGHT[i].m_Light.Direction.y,
			m_pLIGHT[i].m_Light.Direction.z,
			1.0f),
			4 * sizeof(FLOAT));
	}

	m_pDevice->m_pDevice->SetVertexShaderConstantF(
		m_pDevice->m_vConstantVS[VC_LIGHTDIR],
		vCONST, TLIGHT_COUNT);
}

void CTClientNET::ResetDEFFOG()
{
	m_pDevice->m_pDevice->SetRenderState( D3DRS_FOGENABLE, FALSE);

	if(m_pDevice->m_bEnableSHADER)
	{
		m_pDevice->m_pDevice->SetVertexShaderConstantF(
			m_pDevice->m_vConstantVS[VC_AMBIENT],
			(FLOAT *) &D3DXVECTOR4(
			0.0f,
			0.0f,
			0.0f,
			1.0f), 1);
	}
}

void CTClientNET::ResetFOG()
{
	m_pDevice->m_pDevice->SetRenderState( D3DRS_FOGENABLE, FALSE);

	if(m_pDevice->m_bEnableSHADER)
	{
		m_pDevice->m_pDevice->SetVertexShaderConstantF(
			m_pDevice->m_vConstantVS[VC_AMBIENT],
			(FLOAT *) &D3DXVECTOR4(
			0.0f,
			0.0f,
			0.0f,
			1.0f), 1);
	}

	return ;
}

bool IsSameFloat(FLOAT a, FLOAT b)
{
	return (a > b ? a-b : b-a) < 0.0001f;
}

INT GetPeriod( FLOAT fRadian )
{
	INT n=0;
	if( fRadian < 0 )
	{
		while(fRadian < 0)
		{
			fRadian += D3DX_PI*2.0f;
			++n;
		}
	}
	else if( fRadian >= 0 )
		++n;

	if( fRadian > D3DX_PI*2.0f )
	{
		while(fRadian > D3DX_PI*2.0f)
		{
			fRadian -= D3DX_PI*2.0f;
			++n;
		}
	}

	return n;
}

void CTClientNET::SetTargetSlot( INT nTargetSlot, INT nDir )
{
	m_bTargetROT = TRUE;
	m_fROT = RealignRot(m_fROT);
	m_fTargetROT = GetSlotROT( nTargetSlot ); // ´ë»ó ¹æÇâ

	if( nDir > 0 )
	{
		m_fRotDIR = 1.0f;
		if( m_fROT > m_fTargetROT )
			m_fTargetROT += D3DX_PI * 2.0f * GetPeriod(m_fROT);
	}
	else if( nDir < 0 )
	{
		m_fRotDIR = -1.0f;
		if( m_fROT < m_fTargetROT )
			m_fTargetROT -= D3DX_PI * 2.0f * GetPeriod(m_fROT);
	}

	if( m_dwLevel == ID_FRAME_CHAR_NEW )
	{
		m_pHost->m_bSlotID = nTargetSlot;
		m_pHost->SelectCHAR();
	}
	else if( m_dwLevel == ID_FRAME_NEWCHAR_NEW )
	{
		m_pHost->m_vNEWTCHAR.m_bRace = nTargetSlot;
		UpdateRACE();
	}
}

int GetStateIconIndex( BYTE bStatus )
{
	switch( bStatus )
	{
	case TSTATUS_NORMAL: return 0;
	case TSTATUS_BUSY: return 2;
	case TSTATUS_FULL: return 1;
	}
	return 3;
}

HRESULT CTClientNET::Render( DWORD dwTickCount )
{
	static DWORD dwPrevTick = GetTickCount();
	INT nHighlightedSlot = -1;

	m_dwLevelTime += dwTickCount;
	dwTickCount = GetTickCount();

	if( m_bTickReset )
	{
		dwPrevTick = dwTickCount;
		m_bTickReset = FALSE;
	}
	
	if( dwTickCount > dwPrevTick )
	{
		DWORD dwPrev = dwPrevTick;
		dwPrevTick = dwTickCount;
		dwTickCount -= dwPrev;
	}
	else
		dwTickCount = 0;

	// ·Îºñ »óÅÂ Ã³¸® & UI Ã³¸®
	{
		TFrame *pFRAME = FindFrame(m_dwLevel);
		TComponent* pPCROOM = pFRAME->FindKid(ID_CTRLINST_PCROOM);
		if( pPCROOM )
			pPCROOM->ShowComponent(m_pHost->m_bInPCROOM);

		if( CTNationOption::JAPAN_IME || CTNationOption::TAIWAN_IME )
		{
			if( m_pCandidate && m_pCandidate->IsVisible())
			{
				RemoveKid(m_pCandidate);
				AddKid(m_pCandidate);
			}
		}
	}

	switch( m_dwLevel )
	{
	case ID_FRAME_LOGIN_NEW:
		{
			if( CTNationOption::MODIFY_DIRECTLOGIN && m_pHost->m_bDirectLogin )
			{
				static const INT nCount = 10;
				static DWORD dwHIDE[ nCount ] =
				{
					ID_CTRLINST_BACK,
					ID_CTRLINST_TITLE,
					ID_CTRLINST_LOGIN,
					ID_CTRLINST_HOMEPAGE,
					ID_CTRLINST_ID_TEXT,
					ID_CTRLINST_PW_TEXT,
					ID_CTRLINST_ID,
					ID_CTRLINST_PW,
					ID_CTRLINST_ID_BACK,
					ID_CTRLINST_PW_BACK
				};

				for( INT i=0 ; i < nCount ; ++i )
				{
					TFrame* pFRAME = GetCurrentFrame();
					TComponent* pCOMP = pFRAME->FindKid( dwHIDE[ i ] );
					if( pCOMP )
						pCOMP->ShowComponent( FALSE );
				}
			}
		}
		break;

	case ID_FRAME_SERVER_NEW:
		{
			static DWORD dwSTATE_ICON[3] =
			{
				ID_CTRLINST_SERVER_STATE_1,
				ID_CTRLINST_SERVER_STATE_2,
				ID_CTRLINST_SERVER_STATE_3
			};

			static DWORD dwCHANNEL[3] =
			{
				ID_CTRLINST_CHANNEL_1,
				ID_CTRLINST_CHANNEL_2,
				ID_CTRLINST_CHANNEL_3
			};

			static DWORD dwCHANNEL_STATE_ICON[3] =
			{
				ID_CTRLINST_CANNEL_STATE_1,
				ID_CTRLINST_CANNEL_STATE_2,
				ID_CTRLINST_CANNEL_STATE_3
			};

			TFrame *pFRAME = FindFrame( ID_FRAME_SERVER_NEW );
			TList* pLIST = (TList*) pFRAME->FindKid( ID_CTRLINST_LIST );

			int nTop = pLIST->GetTop();
			int nMax = pLIST->GetItemCount();
			for( int i=0 ; i < 3 ; ++i )
			{
				int nCur = nTop + i;
				if( nCur < nMax )
				{
					LPTGROUP pTGROUP = (LPTGROUP) pLIST->GetItemData( nCur, 0 );

					TImageList* pSTATE = (TImageList*) pFRAME->FindKid( dwSTATE_ICON[ i ] );
					pSTATE->SetCurImage( GetStateIconIndex( pTGROUP->m_bStatus ) );
					pSTATE->ShowComponent(TRUE);

					TComponent* pCHANNEL = pFRAME->FindKid( dwCHANNEL[ i ] );
					pCHANNEL->ShowComponent(TRUE);

					TImageList* pCHANNEL_STATE = (TImageList*) pFRAME->FindKid( dwCHANNEL_STATE_ICON[ i ] );
					if( pTGROUP->m_bSelectedChannelIndex == 0xFF )
					{
						pCHANNEL->m_strText = CTChart::LoadString( TSTR_LOBBY_SELECTING_AUTO );
						pCHANNEL_STATE->ShowComponent(FALSE);
					}
					else
					{
						if( pTGROUP->m_bSelectedChannelIndex < pTGROUP->m_vChannel.size() )
						{
							LPTCHANNEL_INFO pChannel = pTGROUP->m_vChannel[ pTGROUP->m_bSelectedChannelIndex ];
							pCHANNEL->m_strText = pChannel->strNAME;
							pCHANNEL_STATE->SetCurImage( GetStateIconIndex( pChannel->bSTATUS ) );
							pCHANNEL_STATE->ShowComponent(TRUE);
						}
						else
						{
							pTGROUP->m_bSelectedChannelIndex = 0xFF;
							pCHANNEL->m_strText = CTChart::LoadString( TSTR_LOBBY_SELECTING_AUTO );
							pCHANNEL_STATE->ShowComponent(FALSE);
						}
					}
				}
				else
				{
					TImageList* pSTATE = (TImageList*) pFRAME->FindKid( dwSTATE_ICON[ i ] );
					TComponent* pCHANNEL = pFRAME->FindKid( dwCHANNEL[ i ] );
					TImageList* pCHANNEL_STATE = (TImageList*) pFRAME->FindKid( dwCHANNEL_STATE_ICON[ i ] );
					pSTATE->ShowComponent(FALSE);
					pCHANNEL->ShowComponent(FALSE);
					pCHANNEL_STATE->ShowComponent(FALSE);
				}
			}
		}
		break;

	case ID_FRAME_AGREEMENT_NEW:
		{
			TFrame* pFRAME = GetCurrentFrame();
			TList* pList = static_cast<TList*>( pFRAME->FindKid(ID_CTRLINST_LIST) );
			TComponent* pAgreeBtn = pFRAME->FindKid(ID_CTRLINST_AGREE);
			TComponent* pQuitBtn  = pFRAME->FindKid(ID_CTRLINST_QUIT);
			
			if( pList->GetTop() >= (pList->GetItemCount() - pList->GetItemPerPage()) &&
				(!pAgreeBtn->m_bEnable || !pQuitBtn->m_bEnable) )
			{
				pAgreeBtn->EnableComponent(TRUE);
				pQuitBtn->EnableComponent(TRUE);

				pAgreeBtn->Get3DFont()->m_dwColor = 0xFFFFFFFF;
				pQuitBtn->Get3DFont()->m_dwColor  = 0xFFFFFFFF;
			}
		}
		break;

	case ID_FRAME_CHAR_NEW:
	case ID_FRAME_NEWCHAR_NEW	:
		{
			if( m_nModelCount != 0 )
			{
				if( m_bTargetROT && !IsSameFloat(m_fROT, m_fTargetROT) )
				{
					m_fROT += FLOAT(D3DX_PI / 1000.0f * dwTickCount) * m_fRotDIR;

					if( ( m_fRotDIR > 0.0f && m_fROT > m_fTargetROT ) ||
						( m_fRotDIR < 0.0f && m_fROT < m_fTargetROT ) )
					{
						m_fROT = RealignRot(m_fTargetROT);
						m_fTargetROT = m_fROT;
						m_bTargetROT = FALSE;
					}
				}

				for( int i=0 ; i < m_nModelCount ; ++i )
				{
					D3DXVECTOR3 vSlotDIR = GetSlotDIR( i );
					D3DXVECTOR3 vCurDIR = GetRotDIR( m_fROT );

					FLOAT fD = D3DXVec3Dot(&vCurDIR, &vSlotDIR);
					if( acos(fD) < MODEL_ROT_RANGE || ((fD >= 1.0f && fD-1.0f < 0.001f) || (fD <= 1.0f && 1.0f-fD < 0.001f)) )
					{
						nHighlightedSlot = i;
						break;
					}
				}
			}

			if( m_bAniCharRot )
				m_fCharROT += m_fAniCharRot / 1000.0f * dwTickCount;
		}
		break;

	case ID_FRAME_LOADING	:
		{
			static const DWORD dwPCSTRS[MAX_LOADING_PCROOM_TEXT] = 
			{
				TSTR_FMT_PCROOM_1,
				TSTR_FMT_PCROOM_2,
				TSTR_FMT_PCROOM_3,
				TSTR_FMT_PCROOM_4,
				0,
				0
			};

			TFrame *pFRAME = FindFrame(m_dwLevel);
			TComponent* pPCROOM[MAX_LOADING_PCROOM_TEXT] = 
			{
				pFRAME->FindKid(ID_CTRLINST_PCROOM_1),
				pFRAME->FindKid(ID_CTRLINST_PCROOM_2),
				pFRAME->FindKid(ID_CTRLINST_PCROOM_3),
				pFRAME->FindKid(ID_CTRLINST_PCROOM_4),
				pFRAME->FindKid(ID_CTRLINST_PCROOM_5),
				pFRAME->FindKid(ID_CTRLINST_PCROOM_6),
			};

			if( CTNationOption::POLAND ||
				CTNationOption::GERMANY ||
				CTNationOption::GREECE ||
				CTNationOption::FRANCE ||
				CTNationOption::ENGLISH ||
				CTNationOption::CZECH ||
				CTNationOption::ITALY ||
				CTNationOption::SPAIN ||
				CTNationOption::UNITEDKINGDOM ||
				CTNationOption::TAIWAN ||
				CTNationOption::RUSSIA ||
				CTNationOption::ROMANIA ||
				CTNationOption::UNITEDSTATE ||
				CTNationOption::HUNGARY ||
				CTNationOption::TURKEY ||
				CTNationOption::PORTUGAL ||
				CTNationOption::JAPAN )
			{
				for(INT i=0; i<MAX_LOADING_PCROOM_TEXT; ++i)
					pPCROOM[i]->ShowComponent(FALSE);
			}
			else
			{
				if( (m_pHost->m_bInPCROOM & PCBANG_REAL) == PCBANG_REAL )
				{
					for(INT i=0; i<MAX_LOADING_PCROOM_TEXT; ++i)
					{
						if( pPCROOM[i] )
						{
							if( dwPCSTRS[i] )
							{
								pPCROOM[i]->m_strText = CTChart::LoadString( (TSTRING) dwPCSTRS[i]);
								pPCROOM[i]->ShowComponent(TRUE);
							}
							else
								pPCROOM[i]->ShowComponent(FALSE);
						}
					}
				}
				else
				{
					for(INT i=0; i<MAX_LOADING_PCROOM_TEXT; ++i)
						pPCROOM[i]->ShowComponent(FALSE);
				}
			}
		}
		break;
	}

	// ·»´õ¸µ
	if( m_dwLevel != ID_FRAME_LOADING )
	{
		// RenderState ¼¼ÆÃ
		m_pDevice->m_pDevice->SetRenderState( D3DRS_AMBIENT, 0xFF000000);
		m_pDevice->m_pDevice->SetRenderState( D3DRS_FOGENABLE, FALSE);
		m_vCamera.Activate(TRUE);

		D3DMATERIAL9 mtrl = {
			{ 1.0f, 1.0f, 1.0f, 1.0f},
			{ 1.0f, 1.0f, 1.0f, 1.0f},
			{ 0.0f, 0.0f, 0.0f, 0.0f},
			{ 0.0f, 0.0f, 0.0f, 0.0f},
			0.0f};
		m_pDevice->m_pDevice->SetMaterial(&mtrl);

		m_vBACK.CalcTick(
			m_pDevice->m_pDevice,
			dwTickCount);

		m_vBACK.Render(
			m_pDevice,
			&m_vCamera);

		// ¸ðµ¨ ·»´õ¸µ
		if( m_dwLevel == ID_FRAME_CHAR_NEW || m_dwLevel == ID_FRAME_NEWCHAR_NEW )
		{
			CalcMODEL(dwTickCount);

			D3DXMATRIX matW;
			D3DXMatrixRotationY(&matW, D3DX_PI / 180.0f * m_fPanRot);
			matW._41 = m_vPanPosition.x;
			matW._42 = m_vPanPosition.y;
			matW._43 = m_vPanPosition.z;

			InitModelLIGHT( matW, FALSE );

			for( int i=0 ; i < 6 ; ++i )
			{
				if( i != nHighlightedSlot && m_vMODEL[ i ] )
				{
					m_vMODEL[ i ]->Render(
						m_pDevice,
						&m_vCamera);
				}
			}

			InitModelLIGHT( matW, TRUE );

			if( nHighlightedSlot != -1 && m_vMODEL[ nHighlightedSlot ] )
			{
				m_vMODEL[ nHighlightedSlot ]->RenderSILHOUETTE(
					m_pDevice,
					&m_vCamera,
					D3DCOLOR_XRGB(180,244,253) );

				m_vMODEL[ nHighlightedSlot ]->Render(
					m_pDevice,
					&m_vCamera);
			}

			DrawModelShadow();
		}

		// ÀÌÆåÆ® ·»´õ¸µ
		CTachyonSFX::CalcBUF(
			m_pDevice->m_pDevice,
			dwTickCount);

		InitEffectLIGHT();

		CTachyonSFX::RenderBUF(
			m_pDevice,
			&m_vCamera, 0);

		for( int i=0 ; i < 6 ; ++i )
		{
			if( m_vMODEL[ i ] )
			{
				m_vMODEL[ i ]->RenderWeaponEffect(
					m_pDevice,
					&m_vCamera);
			}
		}

		if( m_dwLevel == ID_FRAME_CONTRY_NEW )
			DrawBack( D3DCOLOR_ARGB(150,0,0,0) );
	}

	CFrameGroup::Render(dwTickCount);

	switch( m_dwLevel )
	{
	case ID_FRAME_CHAR_NEW:
		{
			CPoint pt;
			::GetCursorPos(&pt);
			::ScreenToClient( AfxGetMainWnd()->GetSafeHwnd(), &pt);

			TImageList* pList = (TImageList*) GetCurrentFrame()->FindKid( ID_CTRLINST_CLASS0 );
			if( pList->HitTest( pt ) )
			{
				CTDetailNameDlg* pDLG = static_cast<CTDetailNameDlg*>( m_pHost->m_MainGame.GetFrame( TFRAME_DETAIL_NAME ) );
				pDLG->ResetText( CTChart::LoadString( (TSTRING) CTClientGame::m_vTCLASSSTR[ pList->GetCurImage() ] ) );
				pDLG->SetPos( pt );
				pDLG->ShowComponent(TRUE);
				pDLG->Render( dwTickCount );
			}
		}
		break;
	case ID_FRAME_SERVER_NEW:
		{
			if( m_pChannelDLG && m_pChannelDLG->CanProcess() )
			{
				m_pChannelDLG->Render( dwTickCount );
			}
		}
		break;
	}

	// ÆäÀÌµå ÀÎ/¾Æ¿ô ·»´õ¸µ
	if( m_bWndAni )
	{
		if( m_bWndAni != 3 )
			m_dwWndAni -= min(m_dwWndAni, dwTickCount);

		if( m_dwWndAni == 0 && m_dwWndAniGM != 0 )
		{
			m_pHost->m_vCOMMAND.push_back( m_dwWndAniGM );
			m_bWndAniType = 3;
		}
		else if( m_dwWndAni == 0 && m_dwWndAniGM == 0 )
		{
			m_bWndAni = FALSE;
		}

		FLOAT fAni = 0.0f;

		if( m_bWndAniType )
			fAni = 1.0f - (FLOAT)m_dwWndAni / (FLOAT)m_dwWndAniMax;
		else
			fAni = (FLOAT)m_dwWndAni / (FLOAT)m_dwWndAniMax;

		if( fAni < 0.0f )
			fAni = 0.0f;
		if( fAni > 255.0f )
			fAni = 255.0f;

		BYTE bAlpha = (BYTE)( fAni * 255.0f );
		DrawBack( D3DCOLOR_ARGB(bAlpha,0,0,0) );
	}

	return S_OK;
}

void CTClientNET::StartFadeIn( DWORD dwSec, DWORD dwGM )
{
	m_bWndAni = TRUE;
	m_dwWndAni = dwSec;
	m_dwWndAniMax = dwSec;
	m_bWndAniType = 1;
	m_dwWndAniGM = dwGM;
}

void CTClientNET::StartFadeOut( DWORD dwSec, DWORD dwGM )
{
	m_bWndAni = TRUE;
	m_dwWndAni = dwSec;
	m_dwWndAniMax = dwSec;
	m_bWndAniType = 0;
	m_dwWndAniGM = dwGM;
}

void CTClientNET::DrawModelShadow()
{
	for( int i=0 ; i < 6 ; ++i )
	{
		if( m_vMODEL[ i ] )
		{
			CTClientObjBase::BeginDrawSquare(m_pDevice);
			m_pHost->m_MainGame.SetMaterial( NULL );

			UVKEY vKEY = CTClientObjBase::m_pTSHADOW->GetUV();
			FLOAT fDepthBias = TOBJSHADOW_BIAS;
			D3DXMATRIX vUV;
			m_pDevice->m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, *((LPDWORD) &fDepthBias));
			m_pDevice->m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, 0xC8000000);
			m_pDevice->m_pDevice->SetTexture( 0, CTClientObjBase::m_pTSHADOW->GetTexture());
			CTMath::GetUVMatrix( &vUV, &vKEY);

			m_pDevice->m_pDevice->SetTransform( D3DTS_TEXTURE0, &vUV);

			D3DXMATRIX vMAT;
			D3DXMatrixScaling(
				&vMAT,
				min( 2.0f, TOBJSHADOW_SCALE * m_vMODEL[ i ]->m_fSizeX ),
				1.0f,
				min( 2.0f, TOBJSHADOW_SCALE * m_vMODEL[ i ]->m_fSizeZ ));

			vMAT._41 = m_vMODEL[ i ]->GetPositionX();
			vMAT._42 = m_vMODEL[ i ]->GetPositionY();
			vMAT._43 = m_vMODEL[ i ]->GetPositionZ();

			m_pDevice->m_pDevice->SetStreamSource( 0, CTClientObjBase::m_vTSQUARE.GetVB(), 0, sizeof(PVERTEX));
			m_pDevice->m_pDevice->SetTransform( D3DTS_WORLD, &vMAT);
			m_pDevice->m_pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
		}
	}
}

void CTClientNET::UpdateRACE()
{
	TFrame *pFRAME = FindFrame(ID_FRAME_NEWCHAR_NEW);

	TComponent *pRACENAME = pFRAME->FindKid(ID_CTRLINST_TEXT_RACE_NAME);
	pRACENAME->m_strText = CTChart::LoadString( (TSTRING) CTClientGame::m_vTRACESTR[m_pHost->m_vNEWTCHAR.m_bRace]);

	TImageList *pRACEIMG = (TImageList*)pFRAME->FindKid(ID_CTRLINST_ICON_RACE);
	pRACEIMG->SetCurImage( m_pHost->m_vNEWTCHAR.m_bRace );

	if( m_pRACETEXTPOPUP )
		m_pRACETEXTPOPUP->SetText( CTChart::GetTRACEINFO( m_pHost->m_vNEWTCHAR.m_bRace ) );
}

void CTClientNET::SwitchFocus(TComponent *pCandidate)
{
	if( m_dwLevel == ID_FRAME_AGREEMENT_NEW )
	{
		m_pFocus = pCandidate;
		return;
	}

	CFrameGroup::SwitchFocus(pCandidate);
}

void CTClientNET::OnKeyDown( UINT nChar, int nRepCnt, UINT nFlags)
{
	if( m_bWndAni )
		return ;

	switch(m_dwLevel)
	{
	case ID_FRAME_LOGIN_NEW:
			if( !m_bClickedIDBox )
			{
				TFrame* pFrame = FindFrame( ID_FRAME_LOGIN_NEW );
				TEdit* pEDIT = (TEdit*) pFrame->FindKid( ID_CTRLINST_ID );

				if( pEDIT->GetFocus() )
				{
					pEDIT->ClearText();
					m_bClickedIDBox = TRUE;
					pEDIT->Get3DFont()->m_dwColor &= 0x00FFFFFF;
					pEDIT->Get3DFont()->m_dwColor |= 0xFFFFFFFF;
				}
			}
			break;
	}

	CFrameGroup::OnKeyDown( nChar, nRepCnt, nFlags);
}

void CTClientNET::OnKeyUp( UINT nChar, int nRepCnt, UINT nFlags)
{
	if( m_bWndAni )
		return ;

	if( nChar == VK_SNAPSHOT )
	{
		m_pHost->CaptureIMG();
	}

	if( m_dwLevel == ID_FRAME_CHAR_NEW )
	{
		if( nChar == VK_LEFT )
		{
			m_pHost->m_vCOMMAND.push_back( GM_NEXT_CHAR );
		}
		else if( nChar == VK_RIGHT )
		{
			m_pHost->m_vCOMMAND.push_back( GM_PREV_CHAR );
		}
	}

	CFrameGroup::OnKeyUp( nChar, nRepCnt, nFlags);
}

void CTClientNET::OnChar( UINT nChar, int nRepCnt, UINT nFlags )
{
	if( m_bWndAni )
		return ;

	CFrameGroup::OnChar( nChar, nRepCnt, nFlags );
}

void CTClientNET::OnImeComposition(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	CFrameGroup::OnImeComposition( hWnd, wParam, lParam);
}

void CTClientNET::OnImeNotify( HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    switch(m_dwLevel)
	{
	case ID_FRAME_LOGIN_NEW:
		{
			if( !m_bClickedIDBox )
			{
				TFrame* pFrame = FindFrame( ID_FRAME_LOGIN_NEW );
				TEdit* pEDIT = (TEdit*) pFrame->FindKid( ID_CTRLINST_ID );

				if( pEDIT->GetFocus() )
				{
					pEDIT->ClearText();
					m_bClickedIDBox = TRUE;
					pEDIT->Get3DFont()->m_dwColor &= 0x00FFFFFF;
					pEDIT->Get3DFont()->m_dwColor |= 0xFFFFFFFF;
				}
			}
		}
		break;
	}

	CFrameGroup::OnImeNotify( hWnd, wParam, lParam);
}

BOOL CTClientNET::DoMouseWheel( UINT nFlags, short zDelta, CPoint pt)
{
	m_nCamZOOM -= zDelta;
	return CFrameGroup::DoMouseWheel( nFlags, zDelta, pt);
}

void CTClientNET::OnLButtonDown( UINT nFlags, CPoint pt)
{
	switch(m_dwLevel)
	{
	case ID_FRAME_LOGIN_NEW:
		{
			if( !m_bClickedIDBox )
			{
				TFrame* pFrame = FindFrame( ID_FRAME_LOGIN_NEW );
				TEdit* pEDIT = (TEdit*) pFrame->FindKid( ID_CTRLINST_ID );

				if( pEDIT->HitTest( pt ) )
				{
					pEDIT->ClearText();
					m_bClickedIDBox = TRUE;
					pEDIT->Get3DFont()->m_dwColor &= 0x00FFFFFF;
					pEDIT->Get3DFont()->m_dwColor |= 0xFFFFFFFF;
				}
			}
		}
		break;

	case ID_FRAME_SERVER_NEW:
		{
			static DWORD dwCHANNEL[3] =
			{
				ID_CTRLINST_CHANNEL_1,
				ID_CTRLINST_CHANNEL_2,
				ID_CTRLINST_CHANNEL_3
			};

			TFrame* pFrame = FindFrame( ID_FRAME_SERVER_NEW );

			if( m_pChannelDLG->CanProcess() && m_pChannelDLG->GetButtonCount() > 0 )
			{
				if( m_pChannelDLG->HitTest(pt) )
				{
					m_pChannelDLG->OnLButtonDown( nFlags, pt );
					return ;
				}
				else
				{
					m_pChannelDLG->ShowComponent(FALSE);
					m_pChannelDLG->ClearButtons();
					return ;
				}
			}

			for( int i=0 ; i < 3 ; ++i )
			{
				TComponent* p = pFrame->FindKid( dwCHANNEL[ i ] );
				if( p->CanProcess() && p->HitTest(pt) )
				{
					p->OnLButtonDown(nFlags, pt);
					break;
				}
			}
		}
		break;

	case ID_FRAME_NEWCHAR_NEW	:
	case ID_FRAME_CHAR_NEW:
		if( HitTest(pt) )
		{
			TFrame* pFrame = FindFrame( m_dwLevel );
			TComponent* pRIGHT = pFrame->FindKid( ID_CTRLINST_CHAR_ROT_RIGHT );
			TComponent* pLEFT = pFrame->FindKid( ID_CTRLINST_CHAR_ROT_LEFT );

			if( pRIGHT && pRIGHT->HitTest(pt) )
			{
				m_bAniCharRot = TRUE;
				m_fAniCharRot = D3DX_PI / 180.0f * 60.0f;
			}
			else if( pLEFT && pLEFT->HitTest(pt) )
			{
				m_bAniCharRot = TRUE;
				m_fAniCharRot = -D3DX_PI / 180.0f * 60.0f;
			}
		}
		else
		{
			CPoint point(
				m_pDevice->m_option.m_dwScreenX / 2,
				m_pDevice->m_option.m_dwScreenY / 2);

			ClientToScreen( AfxGetMainWnd()->GetSafeHwnd(), &point);
			SetCursorPos( point.x, point.y);
			m_bROT = TRUE;
			m_fCharROT = 0.0f;
			m_bTargetROT = FALSE;
		}

		break;
	}

	CFrameGroup::OnLButtonDown( nFlags, pt);
}

void CTClientNET::OnLButtonUp( UINT nFlags, CPoint pt)
{
	if( m_bROT )
	{
		if( (m_dwLevel == ID_FRAME_CHAR_NEW || m_dwLevel == ID_FRAME_NEWCHAR_NEW) )
		{
			INT nSelected = (m_dwLevel == ID_FRAME_CHAR_NEW) ? m_pHost->m_bSlotID : m_pHost->m_vNEWTCHAR.m_bRace;
			if( nSelected != -1 )
			{
				D3DXVECTOR3 vSlotDIR = GetSlotDIR( nSelected );
				D3DXVECTOR3 vCurDIR = GetRotDIR( m_fROT );
				
				FLOAT fD = D3DXVec3Dot(&vCurDIR, &vSlotDIR);
				D3DXVECTOR3 vUP( 0.0f, 1.0f, 0.0f );
				D3DXVec3Cross(&vUP, &vCurDIR, &vSlotDIR);
				if( acos(fD) < MODEL_ROT_RANGE )
				{
					SetTargetSlot(
						nSelected,
						(vUP.y >= 0.0f) ? +1 : -1 );
				}
				else
				{
					if( vUP.y < 0.0f )
						nSelected = (nSelected + 1) % m_nModelCount;
					else if( vUP.y >= 0.0f )
						nSelected = (nSelected + m_nModelCount - 1) % m_nModelCount;

					SetTargetSlot(
						nSelected,
						(vUP.y < 0.0f) ? +1 : -1 );
				}
			}
		}

		m_bROT = FALSE;
	}

	if( m_bAniCharRot )
	{
		m_fAniCharRot = 0.0f;
		m_bAniCharRot = FALSE;
	}

	switch( m_dwLevel )
	{
	case ID_FRAME_SERVER_NEW:
		{
			static DWORD dwCHANNEL[3] =
			{
				ID_CTRLINST_CHANNEL_1,
				ID_CTRLINST_CHANNEL_2,
				ID_CTRLINST_CHANNEL_3
			};

			TFrame* pFrame = FindFrame( ID_FRAME_SERVER_NEW );
			
			if( m_pChannelDLG->CanProcess() && m_pChannelDLG->GetButtonCount() > 0 )
			{
				if( m_pChannelDLG->HitTest(pt) )
				{
					m_pChannelDLG->OnLButtonUp( nFlags, pt );
					return ;
				}
				else
				{
					m_pChannelDLG->ShowComponent(FALSE);
					m_pChannelDLG->ClearButtons();
					return ;
				}
			}

			for( int i=0 ; i < 3 ; ++i )
			{
				TComponent* p = pFrame->FindKid( dwCHANNEL[ i ] );
				if( p->IsVisible() && p->IsEnable() && p->HitTest(pt) )
				{
					p->OnLButtonUp(nFlags, pt);
					break;
				}
			}
		}
		break;
	}

	CFrameGroup::OnLButtonUp( nFlags, pt);
}

void CTClientNET::OnMouseMove( UINT nFlags, CPoint pt)
{
	if(m_bROT)
	{
		CPoint point(
			m_pDevice->m_option.m_dwScreenX / 2,
			m_pDevice->m_option.m_dwScreenY / 2);

		FLOAT fDelta = FLOAT(point.x - pt.x);
		m_fROT += (D3DX_PI / 180.0f * fDelta) / 3.5f;
		m_fROT = RealignRot(m_fROT);

		ClientToScreen( AfxGetMainWnd()->GetSafeHwnd(), &point);
		SetCursorPos( point.x, point.y);

		if( m_nModelCount != 0 )
		{
			for( int i=0 ; i < m_nModelCount ; ++i )
			{
				D3DXVECTOR3 vSlotDIR = GetSlotDIR( i );
				D3DXVECTOR3 vCurDIR = GetRotDIR( m_fROT );

				FLOAT fD = D3DXVec3Dot(&vCurDIR, &vSlotDIR);
				INT nSelected = (m_dwLevel == ID_FRAME_CHAR_NEW) ? m_pHost->m_bSlotID : m_pHost->m_vNEWTCHAR.m_bRace;
				if( acos(fD) < MODEL_ROT_RANGE &&
					nSelected != -1 &&
					nSelected != i )
				{
					if( m_dwLevel == ID_FRAME_CHAR_NEW )
					{
						m_pHost->m_bSlotID = i;
						m_pHost->SelectCHAR();
					}
					else if( m_dwLevel == ID_FRAME_NEWCHAR_NEW )
					{
						m_pHost->m_vNEWTCHAR.m_bRace = i;
						UpdateRACE();
					}
				}
			}
		}
	}

	if( m_bCharROT )
	{
		CPoint point(
			m_pDevice->m_option.m_dwScreenX / 2,
			m_pDevice->m_option.m_dwScreenY / 2);

		FLOAT fDelta = FLOAT(point.x - pt.x);
		m_fCharROT += (D3DX_PI / 180.0f * fDelta) / 10.0f;
		m_fCharROT = RealignRot(m_fCharROT);

		ClientToScreen( AfxGetMainWnd()->GetSafeHwnd(), &point);
		SetCursorPos( point.x, point.y);
	}

	switch( m_dwLevel )
	{
	case ID_FRAME_CONTRY_NEW:
		{
			TFrame* pFrame = FindFrame( ID_FRAME_CONTRY_NEW );
			TImageList* pCountryD = (TImageList*) pFrame->FindKid( ID_CTRLINST_CON_D );
			TImageList* pCountryC = (TImageList*) pFrame->FindKid( ID_CTRLINST_CON_C );

			if( pCountryD && pCountryD->HitTest( pt ) )
				pCountryD->SetCurImage( 1 );
			else
				pCountryD->SetCurImage( m_pHost->m_vNEWTCHAR.m_bContry == TCONTRY_D );
			
			if( pCountryC && pCountryC->HitTest( pt ) )
				pCountryC->SetCurImage( 1 );
			else
				pCountryC->SetCurImage( m_pHost->m_vNEWTCHAR.m_bContry == TCONTRY_C );

			pCountryD->UpdateLastImg();
			pCountryC->UpdateLastImg();
		}
		break;
	case ID_FRAME_SERVER_NEW:
		{
			if( m_pChannelDLG->CanProcess() && m_pChannelDLG->GetButtonCount() > 0 )
			{
				m_pChannelDLG->OnMouseMove( nFlags, pt );
				return ;
			}
		}
		break;
	}

	CFrameGroup::OnMouseMove( nFlags, pt);
}

void CTClientNET::OnRButtonDown(UINT nFlags, CPoint pt)
{
	if( m_dwLevel == ID_FRAME_SERVER_NEW &&
		m_pChannelDLG->CanProcess() &&
		m_pChannelDLG->GetButtonCount() > 0 )
		return ;

	switch( m_dwLevel )
	{
	case ID_FRAME_NEWCHAR_NEW	:
	case ID_FRAME_CHAR_NEW:
		if( !HitTest(pt) )
		{
			CPoint point(
				m_pDevice->m_option.m_dwScreenX / 2,
				m_pDevice->m_option.m_dwScreenY / 2);

			ClientToScreen( AfxGetMainWnd()->GetSafeHwnd(), &point);
			SetCursorPos( point.x, point.y);
			m_bCharROT = TRUE;
		}
		break;
	}

	CFrameGroup::OnRButtonDown( nFlags, pt);
}

void CTClientNET::OnRButtonUp(UINT nFlags, CPoint pt)
{
	if( m_dwLevel == ID_FRAME_SERVER_NEW &&
		m_pChannelDLG->CanProcess() &&
		m_pChannelDLG->GetButtonCount() > 0 )
		return ;

	if(m_bCharROT)
	{
		m_bCharROT = FALSE;
	}

	CFrameGroup::OnRButtonUp( nFlags, pt);
}

void CTClientNET::OnLButtonDblClk(UINT nFlags, CPoint pt)
{
	if( m_dwLevel == ID_FRAME_SERVER_NEW &&
		m_pChannelDLG->CanProcess() &&
		m_pChannelDLG->GetButtonCount() > 0 )
	{
		if( m_pChannelDLG->HitTest(pt) )
		{
			m_pChannelDLG->OnLButtonDblClk(nFlags, pt);
			return ;
		}
		else
		{
			m_pChannelDLG->ShowComponent(FALSE);
			m_pChannelDLG->ClearButtons();
		}
	}

	CFrameGroup::OnLButtonDblClk( nFlags, pt);
}

void CTClientNET::DefaultPosition( CPoint* vBASIS, BOOL bRestore )
{
	static const DWORD TLOGINFRAME_COMP_COUNT = 11;
	static const DWORD TSERVERFRAME_COMP_COUNT = 16;
	static const DWORD TCHARFRAME_COMP_COUNT = 16;
	static const DWORD TNEWCHARFRAME_COMP_COUNT = 29;
	static const DWORD TLOADINGCOMP_COMP_COUNT = 8;
	//static const DWORD TCOUNTRY_COMP_COUNT = 10;
	static const DWORD TAGREEMENT_COMP_COUNT = 7;

	static TFRAMEOFFSET m_vLOGINCOMP[TLOGINFRAME_COMP_COUNT];
	static TFRAMEOFFSET m_vSERVERCOMP[TSERVERFRAME_COMP_COUNT];
	static TFRAMEOFFSET m_vCHARCOMP[TCHARFRAME_COMP_COUNT];
	static TFRAMEOFFSET m_vNEWCHARCOMP[TNEWCHARFRAME_COMP_COUNT];
	static TFRAMEOFFSET m_vLOADINGCOMP[TLOADINGCOMP_COMP_COUNT];
	//static TFRAMEOFFSET m_vCONTRYCOMP[TCOUNTRY_COMP_COUNT];
	static TFRAMEOFFSET m_vAGREEMENTCOMP[TAGREEMENT_COMP_COUNT];

	if( bRestore )
	{
		TFrame *pFRAME = FindFrame(ID_FRAME_LOGIN_NEW);
		for( INT i=0 ; i < TLOGINFRAME_COMP_COUNT ; ++i )
		{
			TComponent* pCOMP = pFRAME->FindKid( m_vLOGINCOMP[i].m_dwFRAME );
			CPoint vCompOffset( m_vLOGINCOMP[i].m_vOffset );
			pCOMP->MoveComponent( CTClientUIBase::m_vBasis[m_vLOGINCOMP[i].m_bBasisPoint] + vCompOffset );
		}

		pFRAME = FindFrame(ID_FRAME_SERVER_NEW);
		for( INT i=0 ; i < TSERVERFRAME_COMP_COUNT ; ++i )
		{
			TComponent* pCOMP = pFRAME->FindKid( m_vSERVERCOMP[i].m_dwFRAME );
			CPoint vCompOffset( m_vSERVERCOMP[i].m_vOffset );
			pCOMP->MoveComponent( CTClientUIBase::m_vBasis[m_vSERVERCOMP[i].m_bBasisPoint] + vCompOffset );
		}

		pFRAME = FindFrame(ID_FRAME_CHAR_NEW);
		for( INT i=0 ; i < TCHARFRAME_COMP_COUNT ; ++i )
		{
			TComponent* pCOMP = pFRAME->FindKid( m_vCHARCOMP[i].m_dwFRAME );
			CPoint vCompOffset( m_vCHARCOMP[i].m_vOffset );
			pCOMP->MoveComponent( CTClientUIBase::m_vBasis[m_vCHARCOMP[i].m_bBasisPoint] + vCompOffset );
		}
		
		pFRAME = FindFrame(ID_FRAME_NEWCHAR_NEW);
		for( INT i=0 ; i < TNEWCHARFRAME_COMP_COUNT ; ++i )
		{
			TComponent* pCOMP = pFRAME->FindKid( m_vNEWCHARCOMP[i].m_dwFRAME );
			CPoint vCompOffset( m_vNEWCHARCOMP[i].m_vOffset );
			pCOMP->MoveComponent( CTClientUIBase::m_vBasis[m_vNEWCHARCOMP[i].m_bBasisPoint] + vCompOffset );
		}
		
		pFRAME = FindFrame(ID_FRAME_LOADING);
		for( INT i=0 ; i < TLOADINGCOMP_COMP_COUNT ; ++i )
		{
			TComponent* pCOMP = pFRAME->FindKid( m_vLOADINGCOMP[i].m_dwFRAME );
			CPoint vCompOffset( m_vLOADINGCOMP[i].m_vOffset );
			pCOMP->MoveComponent( CTClientUIBase::m_vBasis[m_vLOADINGCOMP[i].m_bBasisPoint] + vCompOffset );
		}

		pFRAME = FindFrame(ID_FRAME_AGREEMENT_NEW);
		for( INT i=0 ; i < TAGREEMENT_COMP_COUNT ; ++i )
		{
			TComponent* pCOMP = pFRAME->FindKid( m_vAGREEMENTCOMP[i].m_dwFRAME );
			CPoint vCompOffset( m_vAGREEMENTCOMP[i].m_vOffset );
			pCOMP->MoveComponent( CTClientUIBase::m_vBasis[m_vAGREEMENTCOMP[i].m_bBasisPoint] + vCompOffset );
		}
	}
	else
	{
		// ID_FRAME_LOGIN_NEW
		{
			TFrame *pFRAME = FindFrame(ID_FRAME_LOGIN_NEW);

			static DWORD vCOMP[TLOGINFRAME_COMP_COUNT][2] = 
			{
				{ ID_CTRLINST_BACK, TBASISPOINT_CENTER_BOTTOM },
				{ ID_CTRLINST_TITLE, TBASISPOINT_CENTER_BOTTOM },
				{ ID_CTRLINST_ID_TEXT, TBASISPOINT_CENTER_BOTTOM },
				{ ID_CTRLINST_PW_TEXT, TBASISPOINT_CENTER_BOTTOM },
				{ ID_CTRLINST_ID, TBASISPOINT_CENTER_BOTTOM },
				{ ID_CTRLINST_PW, TBASISPOINT_CENTER_BOTTOM },
				{ ID_CTRLINST_LOGIN, TBASISPOINT_CENTER_BOTTOM },
				{ ID_CTRLINST_HOMEPAGE, TBASISPOINT_RIGHT_BOTTOM },
				{ ID_CTRLINST_QUIT, TBASISPOINT_LEFT_BOTTOM },
				{ ID_CTRLINST_LOGO, TBASISPOINT_CENTER_TOP },
				{ ID_CTRLINST_15_ICON, TBASISPOINT_RIGHT_TOP }
			};

			for( INT i=0 ; i < TLOGINFRAME_COMP_COUNT ; ++i )
			{
				TComponent* pCOMP = pFRAME->FindKid( vCOMP[ i ][0] );
				if( pCOMP )
				{
					CPoint pt;
					pCOMP->GetComponentPos(&pt);
					pt -= CTClientUIBase::m_vDefaultBASIS[ vCOMP[i][1] ];

					m_vLOGINCOMP[i].m_dwFRAME = vCOMP[i][0];
					m_vLOGINCOMP[i].m_bBasisPoint = (BYTE) vCOMP[i][1];
					m_vLOGINCOMP[i].m_vOffset = pt;
				}
			}
		}

		// ID_FRAME_SERVER_NEW
		{
			TFrame *pFRAME = FindFrame(ID_FRAME_SERVER_NEW);

			static DWORD vCOMP[TSERVERFRAME_COMP_COUNT][2] =
			{
				{ ID_CTRLINST_BG, TBASISPOINT_CENTER_BOTTOM },
				{ ID_CTRLINST_OK, TBASISPOINT_RIGHT_BOTTOM },
				{ ID_CTRLINST_BACK, TBASISPOINT_LEFT_BOTTOM },
				{ ID_CTRLINST_LIST, TBASISPOINT_CENTER_BOTTOM },
				{ ID_CTRLINST_15_ICON, TBASISPOINT_RIGHT_TOP },
				{ ID_CTRLINST_PCROOM, TBASISPOINT_RIGHT_TOP },
				{ ID_CTRLINST_LOGO, TBASISPOINT_CENTER_TOP },
				{ ID_CTRLINST_CHANNEL_1, TBASISPOINT_CENTER_BOTTOM },
				{ ID_CTRLINST_CHANNEL_2, TBASISPOINT_CENTER_BOTTOM },
				{ ID_CTRLINST_CHANNEL_3, TBASISPOINT_CENTER_BOTTOM },
				{ ID_CTRLINST_SERVER_STATE_1, TBASISPOINT_CENTER_BOTTOM },
				{ ID_CTRLINST_SERVER_STATE_2, TBASISPOINT_CENTER_BOTTOM },
				{ ID_CTRLINST_SERVER_STATE_3, TBASISPOINT_CENTER_BOTTOM },
				{ ID_CTRLINST_CANNEL_STATE_1, TBASISPOINT_CENTER_BOTTOM },
				{ ID_CTRLINST_CANNEL_STATE_2, TBASISPOINT_CENTER_BOTTOM },
				{ ID_CTRLINST_CANNEL_STATE_3, TBASISPOINT_CENTER_BOTTOM }
			};

			for( INT i=0 ; i < TSERVERFRAME_COMP_COUNT ; ++i )
			{
				TComponent* pCOMP = pFRAME->FindKid( vCOMP[ i ][0] );
				if( pCOMP )
				{
					CPoint pt;
					pCOMP->GetComponentPos(&pt);
					pt -= CTClientUIBase::m_vDefaultBASIS[ vCOMP[i][1] ];

					m_vSERVERCOMP[i].m_dwFRAME = vCOMP[i][0];
					m_vSERVERCOMP[i].m_bBasisPoint = (BYTE) vCOMP[i][1];
					m_vSERVERCOMP[i].m_vOffset = pt;
				}
			}
		}

		// ID_FRAME_CHAR_NEW
		{
			TFrame *pFRAME = FindFrame(ID_FRAME_CHAR_NEW);

			static DWORD vCOMP[TCHARFRAME_COMP_COUNT][2] =
			{
				{ ID_CTRLINST_DELETE, TBASISPOINT_LEFT_BOTTOM },
				{ ID_CTRLINST_START, TBASISPOINT_RIGHT_BOTTOM },
				{ ID_CTRLINST_NEWCHAR, TBASISPOINT_RIGHT_BOTTOM },
				{ ID_CTRLINST_BG, TBASISPOINT_CENTER_BOTTOM },
				{ ID_CTRLINST_CHAR_ROT_LEFT, TBASISPOINT_CENTER_BOTTOM },
				{ ID_CTRLINST_CHAR_ROT_RIGHT, TBASISPOINT_CENTER_BOTTOM },
				{ ID_CTRLINST_LEFT, TBASISPOINT_CENTER_BOTTOM },
				{ ID_CTRLINST_RIGHT, TBASISPOINT_CENTER_BOTTOM },
				{ ID_CTRLINST_CLASS0, TBASISPOINT_CENTER_BOTTOM },
				{ ID_CTRLINST_NAME0, TBASISPOINT_CENTER_BOTTOM },
				{ ID_CTRLINST_LVL0, TBASISPOINT_CENTER_BOTTOM },
				{ ID_CTRLINST_LOC0, TBASISPOINT_CENTER_BOTTOM },
				{ ID_CTRLINST_BACK, TBASISPOINT_LEFT_BOTTOM },
				{ ID_CTRLINST_COUNTRY, TBASISPOINT_LEFT_TOP },
				{ ID_CTRLINST_COUNTRY_NAME, TBASISPOINT_LEFT_TOP },
				{ ID_CTRLINST_SERVER_NAME, TBASISPOINT_LEFT_TOP }
			};

			for( INT i=0 ; i < TCHARFRAME_COMP_COUNT ; ++i )
			{
				TComponent* pCOMP = pFRAME->FindKid( vCOMP[ i ][0] );
				if( pCOMP )
				{
					CPoint pt;
					pCOMP->GetComponentPos(&pt);
					pt -= CTClientUIBase::m_vDefaultBASIS[ vCOMP[i][1] ];

					m_vCHARCOMP[i].m_dwFRAME = vCOMP[i][0];
					m_vCHARCOMP[i].m_bBasisPoint = (BYTE) vCOMP[i][1];
					m_vCHARCOMP[i].m_vOffset = pt;
				}
			}
		}

		// ID_FRAME_NEWCHAR_NEW
		{
			TFrame *pFRAME = FindFrame(ID_FRAME_NEWCHAR_NEW);

			static DWORD vCOMP[TNEWCHARFRAME_COMP_COUNT][2] =
			{
				{ ID_CTRLINST_40, TBASISPOINT_CENTER_TOP },
				{ ID_CTRLINST_45, TBASISPOINT_CENTER_TOP },
				{ ID_CTRLINST_48, TBASISPOINT_CENTER_TOP },
				{ ID_CTRLINST_SEX_SEL_M, TBASISPOINT_CENTER_TOP },
				{ ID_CTRLINST_SEX_SEL_W, TBASISPOINT_CENTER_TOP },
				{ ID_CTRLINST_50, TBASISPOINT_CENTER_BOTTOM },
				{ ID_CTRLINST_HAIR_L, TBASISPOINT_CENTER_TOP },
				{ ID_CTRLINST_HAIR_R, TBASISPOINT_CENTER_TOP },
				{ ID_CTRLINST_FACE_L, TBASISPOINT_CENTER_TOP },
				{ ID_CTRLINST_FACE_R, TBASISPOINT_CENTER_TOP },
				{ ID_CTRLINST_RACE_L, TBASISPOINT_CENTER_TOP },
				{ ID_CTRLINST_RACE_R, TBASISPOINT_CENTER_TOP },
				{ ID_CTRLINST_CLASS_L, TBASISPOINT_CENTER_TOP },
				{ ID_CTRLINST_CLASS_R, TBASISPOINT_CENTER_TOP },
				{ ID_CTRLINST_CANCEL, TBASISPOINT_LEFT_BOTTOM },
				{ ID_CTRLINST_OK, TBASISPOINT_RIGHT_BOTTOM },
				{ ID_CTRLINST_TEXT_RACE_NAME, TBASISPOINT_CENTER_TOP },
				{ ID_CTRLINST_TEXT_HAIRCOLOR, TBASISPOINT_CENTER_TOP },
				{ ID_CTRLINST_TEXT_FACE, TBASISPOINT_CENTER_TOP },
				{ ID_CTRLINST_TEXT_CLASS_NAME, TBASISPOINT_CENTER_TOP },
				{ ID_CTRLINST_NAME, TBASISPOINT_CENTER_BOTTOM },
				{ ID_CTRLINST_TEXT_NAME, TBASISPOINT_CENTER_BOTTOM },
				{ ID_CTRLINST_TEXT_RACE_INFO, TBASISPOINT_CENTER_TOP },
				{ ID_CTRLINST_TEXT_CLASS_INFO, TBASISPOINT_CENTER_TOP },
				{ ID_CTRLINST_ICON_RACE, TBASISPOINT_CENTER_TOP },
				{ ID_CTRLINST_ICON_CLASS, TBASISPOINT_CENTER_TOP },
				{ ID_CTRLINST_CHAR_ROT_RIGHT, TBASISPOINT_CENTER_BOTTOM },
				{ ID_CTRLINST_CHAR_ROT_LEFT, TBASISPOINT_CENTER_BOTTOM },
				{ ID_CTRLINST_WARNING, TBASISPOINT_CENTER_BOTTOM }
			};

			for( INT i=0 ; i < TNEWCHARFRAME_COMP_COUNT ; ++i )
			{
				TComponent* pCOMP = pFRAME->FindKid( vCOMP[ i ][0] );
				if( pCOMP )
				{
					CPoint pt;
					pCOMP->GetComponentPos(&pt);
					pt -= CTClientUIBase::m_vDefaultBASIS[ vCOMP[i][1] ];

					m_vNEWCHARCOMP[i].m_dwFRAME = vCOMP[i][0];
					m_vNEWCHARCOMP[i].m_bBasisPoint = (BYTE) vCOMP[i][1];
					m_vNEWCHARCOMP[i].m_vOffset = pt;
				}
			}
		}

		// ID_FRAME_LOADING
		{
			TFrame *pFRAME = FindFrame(ID_FRAME_LOADING);

			static DWORD vCOMP[TLOADINGCOMP_COMP_COUNT][2] =
			{
				{ ID_CTRLINST_BACK_1, TBASISPOINT_CENTER_MIDDLE },
				{ ID_CTRLINST_BACK_2, TBASISPOINT_CENTER_MIDDLE },
				{ ID_CTRLINST_PCROOM_1, TBASISPOINT_CENTER_MIDDLE },
				{ ID_CTRLINST_PCROOM_2, TBASISPOINT_CENTER_MIDDLE },
				{ ID_CTRLINST_PCROOM_3, TBASISPOINT_CENTER_MIDDLE },
				{ ID_CTRLINST_PCROOM_4, TBASISPOINT_CENTER_MIDDLE },
				{ ID_CTRLINST_PCROOM_5, TBASISPOINT_CENTER_MIDDLE },
				{ ID_CTRLINST_PCROOM_6, TBASISPOINT_CENTER_MIDDLE },
			};

			for( INT i=0 ; i < TLOADINGCOMP_COMP_COUNT ; ++i )
			{
				TComponent* pCOMP = pFRAME->FindKid( vCOMP[ i ][0] );
				if( pCOMP )
				{
					CPoint pt;
					pCOMP->GetComponentPos(&pt);
					pt -= CTClientUIBase::m_vDefaultBASIS[ vCOMP[i][1] ];

					m_vLOADINGCOMP[i].m_dwFRAME = vCOMP[i][0];
					m_vLOADINGCOMP[i].m_bBasisPoint = (BYTE) vCOMP[i][1];
					m_vLOADINGCOMP[i].m_vOffset = pt;
				}
			}
		}

		// ID_FRAME_AGREEMENT_NEW
		{
			TFrame *pFRAME = FindFrame(ID_FRAME_AGREEMENT_NEW);

			static DWORD vCOMP[TAGREEMENT_COMP_COUNT][2] =
			{
				{ ID_CTRLINST_LOGO, TBASISPOINT_CENTER_TOP },
				{ ID_CTRLINST_BG, TBASISPOINT_CENTER_MIDDLE },
				{ ID_CTRLINST_TITLE, TBASISPOINT_CENTER_MIDDLE },
				{ ID_CTRLINST_LIST, TBASISPOINT_CENTER_MIDDLE },
				{ ID_CTRLINST_AGREE, TBASISPOINT_CENTER_MIDDLE },
				{ ID_CTRLINST_QUIT, TBASISPOINT_CENTER_MIDDLE },
				{ ID_CTRLINST_15_ICON, TBASISPOINT_RIGHT_TOP },
			};

			for( INT i=0 ; i < TAGREEMENT_COMP_COUNT ; ++i )
			{
				TComponent* pCOMP = pFRAME->FindKid( vCOMP[ i ][0] );
				if( pCOMP )
				{
					CPoint pt;
					pCOMP->GetComponentPos(&pt);
					pt -= CTClientUIBase::m_vDefaultBASIS[ vCOMP[i][1] ];

					m_vAGREEMENTCOMP[i].m_dwFRAME = vCOMP[i][0];
					m_vAGREEMENTCOMP[i].m_bBasisPoint = (BYTE) vCOMP[i][1];
					m_vAGREEMENTCOMP[i].m_vOffset = pt;
				}
			}
		}
	}
}

void CTClientNET::ResetPosition()
{
	DefaultPosition( NULL, TRUE );
}

void CTClientNET::LoadCAURL()
{
	CStdioFile file;
	if( file.Open( "url.txt", CFile::modeRead | CFile::typeText ) )
	{
		CString strCAURL_For_Luancher;
		file.ReadString( strCAURL_For_Luancher );

		file.ReadString( m_strCAURL );
		
		CString strBTN;
		file.ReadString( strBTN );
		if( !strBTN.IsEmpty() )
		{
			TFrame *pFRAME = FindFrame(ID_FRAME_LOGIN_NEW);
			if( pFRAME )
			{
				TComponent* pCOMP = pFRAME->FindKid( ID_CTRLINST_HOMEPAGE );

				if( pCOMP )
					pCOMP->m_strText = strBTN;
			}
		}

		file.Close();
		return ;
	}
	else
	{
		if( CTNationOption::GERMANY )
			m_strCAURL = CString(_T("http://www.4story.de/user/register"));
		else if ( CTNationOption::CZECH )
			m_strCAURL = CString(_T("http://www.4story.cz/user/register"));
		else if ( CTNationOption::GREECE )
			m_strCAURL = CString(_T("http://www.4story.gr/user/register"));
		else if ( CTNationOption::POLAND )
			m_strCAURL = CString(_T("http://www.4story.pl/user/register"));
		else if ( CTNationOption::FRANCE )
			m_strCAURL = CString(_T("http://www.4story.fr/user/register"));
		else if ( CTNationOption::ITALY )
			m_strCAURL = CString(_T("http://www.4story.it/user/register"));
		else if ( CTNationOption::SPAIN )
			m_strCAURL = CString(_T("http://www.4story.es/user/register"));
		else if ( CTNationOption::UNITEDKINGDOM )
			m_strCAURL = CString(_T("http://www.4story.co.uk/user/register"));
		else if ( CTNationOption::ROMANIA )
			m_strCAURL = CString(_T("http://www.4story.ro/user/register"));
		else if ( CTNationOption::UNITEDSTATE )
			m_strCAURL = CString(_T("http://www.gatesofandaron.com/user/register"));
		else if ( CTNationOption::HUNGARY )
			m_strCAURL = CString(_T("http://www.4story.hu/user/register"));
		else if ( CTNationOption::TURKEY )
			m_strCAURL = CString(_T("http://www.4story.web.tr/user/register"));
		else if ( CTNationOption::PORTUGAL )
			m_strCAURL = CString(_T("http://www.4story.pt/user.register"));
		else if( CTNationOption::ENGLISH )
			m_strCAURL = CString(_T("https://www.gamengame.com/gng/member/member_form.asp?gamename=fourstory&ssoreturn=global.4story.com/login/login.asp"));
		else if( CTNationOption::TAIWAN )
			m_strCAURL = CString(_T("http://dcard.draco-games.com/join/joinDcard.aspx"));
		else if ( CTNationOption::RUSSIA )
			m_strCAURL = CString(_T("http://www.4sonline.ru"));
		else
			m_strCAURL = CString(_T("https://www.4story.co.kr/member/join/join_agreement.asp"));
	}
}

void CTClientNET::ClearModelSFX()
{
	for( int i=0 ; i < 6 ; ++i )
	{
		if( m_vMODEL[ i ] )
			m_vMODEL[ i ]->ClearSFX();
	}
}

D3DXVECTOR3 CTClientNET::GetSlotDIR( INT nSlotID )
{
	D3DXVECTOR3 vDIR(0.0f, 0.0f, m_fPanDist);
	D3DXMATRIX vROT;
	D3DXMatrixRotationY(
		&vROT,
		D3DX_PI * 2.0f / FLOAT(m_nModelCount) * nSlotID );
	D3DXVec3TransformCoord( &vDIR, &vDIR, &vROT );
	D3DXVec3Normalize(&vDIR, &vDIR);
	return vDIR;
}

FLOAT CTClientNET::GetSlotROT( INT nSlotID )
{
	return D3DX_PI * 2.0f / FLOAT(m_nModelCount) * nSlotID;	
}

D3DXVECTOR3 CTClientNET::GetRotDIR( FLOAT fRadian )
{
	D3DXVECTOR3 vDIR(0.0f, 0.0f, m_fPanDist);
	D3DXMATRIX vROT;
	D3DXMatrixRotationY(
		&vROT,
		fRadian );
	D3DXVec3TransformCoord( &vDIR, &vDIR, &vROT );
	D3DXVec3Normalize(&vDIR, &vDIR);
	return vDIR;
}

void CTClientNET::ChangeBACK( DWORD toState )
{
	static DWORD dwCHAR[ TCONTRY_COUNT ][ TMAINRACE_COUNT ] =
	{
		{ ID_OBJECT_33100,	ID_OBJECT_33100, ID_OBJECT_33100},
		{ ID_OBJECT_33100,	ID_OBJECT_33100, ID_OBJECT_33100},
		{ ID_OBJECT_33100, ID_OBJECT_33100, ID_OBJECT_33100}
	};

	DWORD dwBACK = 0;

	switch( toState )
	{
	case ID_FRAME_LOGIN_NEW:
	case ID_FRAME_AGREEMENT_NEW:
	case ID_FRAME_SERVER_NEW:
	case ID_FRAME_CONTRY_NEW:
		{
			ClearModelAll();
			dwBACK = ID_OBJECT_3328;
			InitCameraLOGIN();
			InitLIGHT(TCONTRY_N);
		}
		break;

	case ID_FRAME_CHAR_NEW:
		{
			if( m_pHost->m_bSlotID != 255 && m_vMODEL[ m_pHost->m_bSlotID ] )
			{
				BYTE bCountry = m_vMODEL[m_pHost->m_bSlotID]->m_bContryID;
				BYTE bRaceID = m_vMODEL[m_pHost->m_bSlotID]->GetRaceID();
				dwBACK = ID_OBJECT_33100;
				InitCameraCHAR();
				InitLIGHT( bCountry );
			}
			else
				return ;
		}
		break;

	case ID_FRAME_NEWCHAR_NEW:
		{
			dwBACK = ID_OBJECT_33100;
			InitCameraCHAR();
			InitLIGHT( m_pHost->m_vNEWTCHAR.m_bContry );
		}
		break;

	case ID_FRAME_LOADING:
		{
			m_vBACK.ClearSFX();
			m_vBACK.ReleaseData();
			m_vBACK.Release();
			ClearModelAll();		
			m_dwBackID = 0;
		}
		return ;
	}

	if( dwBACK != 0 && m_dwBackID != dwBACK )
	{
		m_dwBackID = dwBACK;

		m_vBACK.ClearSFX();
		m_vBACK.ReleaseData();
		m_vBACK.Release();
		CTachyonSFX::ReleaseNoHost();

		m_vBACK.InitOBJ( m_pRES->GetOBJ( m_dwBackID ) );
		m_vBACK.SetCloth(
			m_pDevice,
			ID_CLK_MAPOBJ,
			ID_CL_MAPOBJ,
			ID_MESH_0);

		m_vBACK.ResetOBJPart(m_pDevice);

		m_vBACK.SetAction(
			ID_ACT_DEFAULT,
			ID_ANI_0);

		D3DXMatrixIdentity( &m_vBACK.m_vWorld );
	}
}

void CTClientNET::ClearModelAll()
{
	bool bOnce = FALSE;
	for( int i=0 ; i < 6 ; ++i )
	{
		if( m_vMODEL[ i ] )
		{
			m_vMODEL[ i ]->ClearSFX();
			m_vMODEL[ i ]->ReleaseData();
			m_vMODEL[ i ]->Release();
			delete m_vMODEL[ i ];
			m_vMODEL[ i ] = NULL;
			bOnce = TRUE;
		}
	}

	if( bOnce )
		CTachyonSFX::ReleaseNoHost();
}

void CTClientNET::SetLevel( DWORD dwLevel)
{
	CFrameGroup::SetLevel( dwLevel );
	ChangeBACK( dwLevel );
}

void CTClientNET::DrawBack(DWORD dwColor)
{
	static D3DXVECTOR3 vDRAWBACK[] =
	{
		D3DXVECTOR3( -1.0f, 1.0f, 0.5f ),
		D3DXVECTOR3( 1.0f, 1.0f, 0.5f ),
		D3DXVECTOR3( -1.0f, -1.0f, 0.5f ),
		D3DXVECTOR3( 1.0f, -1.0f, 0.5f )
	};

	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
	m_pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE );
	m_pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE );

	m_pDevice->m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	m_pDevice->m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	m_pDevice->m_pDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
	m_pDevice->m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_ZENABLE, FALSE);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	m_pDevice->m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, dwColor );
	
	D3DXMATRIX m;
	D3DXMatrixIdentity(&m);
	m_pDevice->m_pDevice->SetTransform( D3DTS_WORLD, &m);
	m_pDevice->m_pDevice->SetTransform( D3DTS_VIEW, &m);
	m_pDevice->m_pDevice->SetTransform( D3DTS_PROJECTION, &m);

	m_pDevice->m_pDevice->SetFVF( D3DFVF_XYZ );

	m_pDevice->m_pDevice->SetTexture( 0, NULL );
	m_pDevice->m_pDevice->SetTexture( 1, NULL );

	HRESULT hr = m_pDevice->m_pDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, vDRAWBACK, sizeof(D3DXVECTOR3));

	m_vCamera.Activate(TRUE);
}

void CTClientNET::ProcGM_OPEN_CHANNEL_LIST()
{
	static DWORD dwGroupCLR[] =
	{
		D3DCOLOR_ARGB(255, 171,160,153),	//TSTATUS_SLEEP 
		TCOLOR_LIGHT,								//TSTATUS_NORMAL
		TCOLOR_MIDDLE,							//TSTATUS_BUSY,
		TCOLOR_HEAVY								//TSTATUS_FULL,
	};

	switch( m_dwLevel )
	{
	case ID_FRAME_SERVER_NEW:
		{
			TFrame *pFRAME = FindFrame( ID_FRAME_SERVER_NEW );
			TList *pLIST = (TList *) pFRAME->FindKid( ID_CTRLINST_LIST );
			
			if( pLIST->GetSel() == -1 )
				return ;

			LPTGROUP pGROUP = (LPTGROUP) pLIST->GetItemData( pLIST->GetSel(), 0 );
			if( !pGROUP )
				return ;

			m_pChannelDLG->ClearButtons();
			
			CString strNAME;
			strNAME = CTChart::LoadString( TSTR_AUTOCHANNEL );
			m_pChannelDLG->AddButton( strNAME, GM_CHANNEL_POPUP_CLICKED );
			m_pChannelDLG->SelectButton( 0, FALSE );
			m_pChannelDLG->SetButtonColor( 0, dwGroupCLR[ TSTATUS_NORMAL ] );

			for( int i=0 ; i < pGROUP->m_vChannel.size() ; ++i )
			{
				LPTCHANNEL_INFO pChannel = pGROUP->m_vChannel[ i ];
				m_pChannelDLG->AddButton( pChannel->strNAME, GM_CHANNEL_POPUP_CLICKED );
				m_pChannelDLG->SelectButton( i + 1, FALSE );
				m_pChannelDLG->SetButtonColor( i + 1, dwGroupCLR[ pChannel->bSTATUS ] );
			}

			static DWORD dwCHANNEL[3] =
			{
				ID_CTRLINST_CHANNEL_1,
				ID_CTRLINST_CHANNEL_2,
				ID_CTRLINST_CHANNEL_3
			};

			CPoint ptPOPUPPOS;
			int n = pLIST->GetSel() - pLIST->GetTop();
			n = (0 <= n && n < 3) ? n : 0;
			TComponent* pCHANNEL = pFRAME->FindKid( dwCHANNEL[ n ] );
			pCHANNEL->GetComponentPos(&ptPOPUPPOS);
			CSize ptSize;
			pCHANNEL->GetComponentSize(&ptSize);
			INT nHeight = (m_pChannelDLG->GetButtonHeight() * m_pChannelDLG->GetButtonCount() + m_pChannelDLG->GetUpperHeight() * 2) + 2;
			ptPOPUPPOS.y -= nHeight - ptSize.cy;
			m_pChannelDLG->MoveComponent(ptPOPUPPOS);
			m_pChannelDLG->ShowComponent(TRUE);
		}
		break;
	}
}

void CTClientNET::InitGroupUI()
{
	TFrame *pFRAME = FindFrame( ID_FRAME_SERVER_NEW );
	TList *pLIST = (TList*) pFRAME->FindKid( ID_CTRLINST_LIST );
	pLIST->ApplyUserColor(TRUE);
	pLIST->RemoveAll();

	for( size_t i=0 ; i < m_pHost->m_vTGroups.size() ; ++i )
	{
		LPTGROUP pGroup = m_pHost->m_vTGroups[ i ];
		if( !pGroup )
			continue;

		CString strFMT;
		strFMT.Format( "   %s [%d]",
			pGroup->m_strNAME,
			pGroup->m_bCharCnt );

		int nIndex = pLIST->AddItem( strFMT, (DWORD) pGroup);
		pLIST->SetUserColor( nIndex, 0, D3DCOLOR_ARGB(255,219,214,202) );
	}

	if( m_bSelServerNum != (BYTE)T_INVALID )
		pLIST->SetCurSelItem( m_bSelServerNum );

	pFRAME->SwitchFocus( pLIST );
}

void CTClientNET::ResetWndAni()
{
	m_bWndAni = FALSE;
	m_bWndAniType = 0;
	m_dwWndAni = 0;
	m_dwWndAniMax = 0;
	m_dwWndAniGM = 0;
}