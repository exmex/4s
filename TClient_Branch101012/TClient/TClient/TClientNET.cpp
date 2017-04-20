#include "stdafx.h"
#include "TClientWnd.h"
#include "Resource.h"

CTClientNET::CTClientNET()
{
	m_pDevice = NULL;
	m_pCamera = NULL;
	m_pLIGHT = NULL;
	m_pHost = NULL;
	m_pRES = NULL;

	m_bROT = FALSE;
	m_wROT = 0;

	m_dwLevelTime = 0;
	m_nCamZOOM = 0;

	m_pChannelDLG = NULL;
	m_bSelChannelNum = (BYTE)T_INVALID;
	m_bSelServerNum = (BYTE)T_INVALID;
	m_bSelServerGropNum = (BYTE)T_INVALID;

	m_pCandidate = NULL;
	m_bInvertCountrySel = 0;
	m_bClickedIDBox = FALSE;
}

CTClientNET::~CTClientNET()
{
}

void CTClientNET::InitDEFCAM()
{
	m_pCamera->InitCamera(
		m_pDevice->m_pDevice,
		TNETCAM_DEFNEAR,
		TNETCAM_DEFFAR,
		TNETCAM_FOV * D3DX_PI / 180.0f,
		m_pDevice->m_option.m_dwScreenX,
		m_pDevice->m_option.m_dwScreenY);
	CTClientObjBase::m_fCamDist = TNETCAM_DEFFAR;

	m_pCamera->m_bCamRotSPEED = TNETCAM_ROTSPEED;
	m_pCamera->m_fZoomSPEED = TNETCAM_ZOOMSPEED;
	m_pCamera->m_wCamPITCH = TNETCAM_PITCH % 1800;
	m_nCamZOOM = 0;

	m_pCamera->m_fZoomMIN = 1.0f;
	m_pCamera->m_fZoomMAX = 1.0f;

	m_pCamera->m_fCamHEIGHT = 1.0f;
	m_pCamera->m_fCamDIST = 1.0f;

	m_bROT = FALSE;
	m_wROT = 0;
}

void CTClientNET::InitCamera()
{
	m_pCamera->InitCamera(
		m_pDevice->m_pDevice,
		TNETCAM_NEAR,
		TNETCAM_FAR,
		TNETCAM_FOV * D3DX_PI / 180.0f,
		m_pDevice->m_option.m_dwScreenX,
		m_pDevice->m_option.m_dwScreenY);
	CTClientObjBase::m_fCamDist = TNETCAM_CULL;

	m_pCamera->m_fZoomMIN = m_vMODEL.GetAttrFLOAT(ID_LCAM_DIST);
	m_pCamera->m_fZoomMAX = m_vMODEL.GetAttrFLOAT(ID_LCAM_DIST);

	m_pCamera->m_fCamHEIGHT = m_vMODEL.GetAttrFLOAT(ID_LCAM_HEIGHT);
	m_pCamera->m_fCamDIST = m_vMODEL.GetAttrFLOAT(ID_LCAM_DIST);

	m_pCamera->m_bCamRotSPEED = TNETCAM_ROTSPEED;
	m_pCamera->m_fZoomSPEED = TNETCAM_ZOOMSPEED;
	m_pCamera->m_wCamPITCH = TNETCAM_PITCH % 1800;
	m_nCamZOOM = 0;

	m_wROT = m_vMODEL.GetAttrINT(ID_CREATE_DIR);
	m_bROT = FALSE;
}

void CTClientNET::InitMODEL( LPTCHARINFO pINFO)
{
	LPOBJECT pPREV = m_vMODEL.m_OBJ.m_pOBJ;
	LPOBJECT pNEXT = NULL;
	BYTE bContryID = m_vMODEL.m_bContryID;

	m_vMODEL.ReleaseData();

	m_vMODEL.m_bContryID = pINFO->m_bContry;
	m_vMODEL.m_strNAME = pINFO->m_strNAME;
	m_vMODEL.m_bClassID = pINFO->m_bClass;
	m_vMODEL.m_bRaceID_ = pINFO->m_bRace;
	m_vMODEL.m_bLevel = pINFO->m_bLevel;
	m_vMODEL.m_bPants = pINFO->m_bPants;
	m_vMODEL.m_bHair = pINFO->m_bHair;
	m_vMODEL.m_bFace = pINFO->m_bFace;
	m_vMODEL.m_bBody = pINFO->m_bBody;
	m_vMODEL.m_bHand = pINFO->m_bHand;
	m_vMODEL.m_bFoot = pINFO->m_bFoot;
	m_vMODEL.m_bSex = pINFO->m_bSex;
	m_vMODEL.m_dwID = pINFO->m_dwID;

	if( m_vMODEL.m_pGuildMark )
		delete m_vMODEL.m_pGuildMark;

	m_vMODEL.m_pGuildMark = new CTClientGuildMark( TGMARK_MARK(pINFO->m_dwFame), TGMARK_BACK(pINFO->m_dwFame),
				TGMCOLOR_MARK(pINFO->m_dwFameColor), TGMCOLOR_BACK1(pINFO->m_dwFameColor), TGMCOLOR_BACK2(pINFO->m_dwFameColor) );

	pNEXT = m_pRES->GetOBJ( CTChart::m_vTRACE[pINFO->m_bRace][pINFO->m_bSex]);
	if( pPREV != pNEXT )
	{
		m_vMODEL.InitOBJ(pNEXT);
		InitBACK();

		TACTION vActionID = m_vMODEL.FindActionID(
			TA_STAND,
			WT_NORMAL);

		m_vMODEL.SetAction(
			vActionID.m_dwActID,
			vActionID.m_dwAniID);

		m_vMODEL.InitSIZE(
			m_vMODEL.GetAttrFLOAT(ID_SIZE_X),
			m_vMODEL.GetAttrFLOAT(ID_SIZE_Y),
			m_vMODEL.GetAttrFLOAT(ID_SIZE_Z),
			0.0f, 0.0f);
		D3DXMatrixIdentity(&m_vMODEL.m_vWorld);
	}
	else if( m_vMODEL.m_bContryID != bContryID )
		InitBACK();

	m_vMODEL.m_mapTINVEN.insert( MAPTINVEN::value_type( INVEN_EQUIP, &pINFO->m_vEQUIP));
	m_vMODEL.ClearEquip();
	m_vMODEL.ResetEQUIP( m_pDevice, m_pRES);
	m_vMODEL.m_mapTINVEN.clear();
}

void CTClientNET::InitDEFBACK()
{
	m_vDEFBACK.ReleaseData();
	m_vDEFBACK.Release();

	m_vDEFBACK.InitOBJ(m_pRES->GetOBJ(ID_BACK_00));
	m_vDEFBACK.SetCloth(
		m_pDevice,
		ID_CLK_MAPOBJ,
		ID_CL_MAPOBJ,
		ID_MESH_0);
	m_vDEFBACK.ResetOBJPart(m_pDevice);

	m_vDEFBACK.SetAction(
		ID_ACT_DEFAULT,
		ID_ANI_0);

	D3DXMatrixIdentity(&m_vDEFBACK.m_vWorld);
	InitLIGHT(TCONTRY_N);
	InitDEFCAM();
}

void CTClientNET::InitBACK()
{
	static DWORD dwID[TCONTRY_COUNT][TMAINRACE_COUNT] = {{
		ID_BACK_01,
		ID_BACK_01,
		ID_BACK_01}, {
		ID_BACK_02,
		ID_BACK_02,
		ID_BACK_02}, {
		ID_BACK_01,
		ID_BACK_01,
		ID_BACK_01}};
	m_vBACK.ReleaseData();

	m_vBACK.InitOBJ(m_pRES->GetOBJ(dwID[m_vMODEL.m_bContryID][m_vMODEL.GetRaceID()]));
	m_vBACK.SetCloth(
		m_pDevice,
		ID_CLK_MAPOBJ,
		ID_CL_MAPOBJ,
		ID_MESH_0);
	m_vBACK.ResetOBJPart(m_pDevice);

	D3DXMatrixIdentity(&m_vBACK.m_vWorld);
	InitLIGHT(m_vMODEL.m_bContryID);
	InitCamera();
}

void CTClientNET::InitLIGHT( BYTE bCountryID)
{
	static WORD vMapID[TCONTRY_COUNT] = {
		0xFFFE,		// TCONTRY_D
		0xFFFD,		// TCONTRY_C
		0xFFFC,		// TCONTRY_B
		0xFFFF};	// TCONTRY_N

	for( BYTE i=0; i<TLIGHT_COUNT; i++)
	{
		LPTLIGHT pTLIGHT = CTChart::FindTLIGHT(
			1, TLIGHTSET_MAIN, i,
			vMapID[bCountryID],
			0, 0);

		if(pTLIGHT)
		{
			memset( &m_pLIGHT[i].m_Light, 0, sizeof(D3DLIGHT9));

			m_pLIGHT[i].m_Light.Type = D3DLIGHT_DIRECTIONAL;
			m_pLIGHT[i].m_Light.Direction.x = pTLIGHT->m_vDIR.x;
			m_pLIGHT[i].m_Light.Direction.y = pTLIGHT->m_vDIR.y;
			m_pLIGHT[i].m_Light.Direction.z = pTLIGHT->m_vDIR.z;

			m_pLIGHT[i].m_Light.Ambient.r = pTLIGHT->m_fAmbientR;
			m_pLIGHT[i].m_Light.Ambient.g = pTLIGHT->m_fAmbientG;
			m_pLIGHT[i].m_Light.Ambient.b = pTLIGHT->m_fAmbientB;
			m_pLIGHT[i].m_Light.Ambient.a = 1.0f;

			m_pLIGHT[i].m_Light.Diffuse.r = pTLIGHT->m_fDiffuseR;
			m_pLIGHT[i].m_Light.Diffuse.g = pTLIGHT->m_fDiffuseG;
			m_pLIGHT[i].m_Light.Diffuse.b = pTLIGHT->m_fDiffuseB;
			m_pLIGHT[i].m_Light.Diffuse.a = 1.0f;

			m_pLIGHT[i].SetLight(&m_pLIGHT[i].m_Light);
			m_pLIGHT[i].EnableLight();
		}
	}
}

void CTClientNET::CalcDEFCamera( DWORD dwTick)
{
	m_pCamera->SetPosition(
		D3DXVECTOR3( 0.0f, 0.0f, 0.0f),
		D3DXVECTOR3( 0.0f, 0.0f, 1.0f),
		D3DXVECTOR3( 0.0f, 1.0f, 0.0f));

	m_pCamera->Rotate( 0.0f, TNETCAM_ROT_X * D3DX_PI / 180.0f, 0.0f, TRUE);
	m_pCamera->Rotate( TNETCAM_ROT_Y * D3DX_PI / 180.0f, 0.0f, 0.0f, TRUE);
	m_pCamera->Move(
		TNETCAM_POS_X - m_pCamera->m_vPosition.x,
		TNETCAM_POS_Y - m_pCamera->m_vPosition.y,
		TNETCAM_POS_Z - m_pCamera->m_vPosition.z, TRUE);
	m_pCamera->Activate(TRUE);
}

void CTClientNET::CalcCamera( DWORD dwTick)
{
	TComponent *pCENTER = GetCurrentFrame()->FindKid(ID_CTRLINST_CENTER);
	CPoint vCENTER;
	int nScreenX = TBASE_SCREEN_X * m_pDevice->m_option.m_dwScreenY / TBASE_SCREEN_Y;

	pCENTER->GetComponentPos(&vCENTER);
	pCENTER->ShowComponent(FALSE);

	vCENTER.y = m_pDevice->m_option.m_dwScreenY * vCENTER.y / TBASE_SCREEN_Y;
	if( m_pDevice->m_option.m_dwScreenX > nScreenX )
		vCENTER.x = (m_pDevice->m_option.m_dwScreenX - nScreenX) / 2 + nScreenX * vCENTER.x / TBASE_SCREEN_X;
	else
		vCENTER.x = m_pDevice->m_option.m_dwScreenX * vCENTER.x / TBASE_SCREEN_X;

	if(m_nCamZOOM)
	{
		DWORD dwZoomTick = abs(m_nCamZOOM);

		if( dwZoomTick > dwTick )
			dwZoomTick -= dwTick;

		int nCamZoom = m_nCamZOOM < 0 ? -1 : 1;
		m_pCamera->ZoomCamera(
			dwZoomTick,
			nCamZoom);

		m_nCamZOOM -= nCamZoom * INT(dwZoomTick);
	}

	m_pCamera->SetPosition(
		D3DXVECTOR3( 0.0f, 0.0f, -m_pCamera->m_fCamDIST),
		D3DXVECTOR3( 0.0f, 0.0f, 0.0f),
		D3DXVECTOR3( 0.0f, 1.0f, 0.0f));
	m_pCamera->Rotate( 0.0f, FLOAT(m_pCamera->m_wCamPITCH) * D3DX_PI / 900.0f, 0.0f, TRUE);

	D3DVIEWPORT9 vVIEW;
	D3DXMATRIX vWORLD;
	D3DXPLANE vPLANE;

	m_pDevice->m_pDevice->GetViewport(&vVIEW);
	D3DXMatrixIdentity(&vWORLD);
	D3DXPlaneFromPointNormal(
		&vPLANE,
		&m_pCamera->m_vTarget,
		&m_pCamera->m_vAxisZ);

	m_pCamera->Move(
		0.0f,
		0.0f,
		D3DXPlaneDotCoord(
		&vPLANE,
		&D3DXVECTOR3(
		0.0f,
		m_pCamera->m_fCamHEIGHT,
		0.0f)));

	D3DXVECTOR3 vSCREEN[2] = {
		D3DXVECTOR3( FLOAT(vCENTER.x), FLOAT(vCENTER.y), vVIEW.MinZ),
		D3DXVECTOR3( FLOAT(vCENTER.x), FLOAT(vCENTER.y), vVIEW.MaxZ)};
	D3DXVECTOR3 vLINE[2];
	D3DXVECTOR3 vMOVE;

	D3DXVec3UnprojectArray(
		vLINE,
		sizeof(D3DXVECTOR3),
		vSCREEN,
		sizeof(D3DXVECTOR3),
		&vVIEW,
		&m_pCamera->m_matProjection,
		&m_pCamera->m_matView,
		&vWORLD, 2);

	D3DXPlaneIntersectLine(
		&vMOVE,
		&vPLANE,
		&vLINE[0],
		&vLINE[1]);

	m_pCamera->Move(
		-vMOVE.x,
		-vMOVE.y,
		-vMOVE.z,
		TRUE);

	vMOVE = D3DXVECTOR3(
		0.0f,
		m_pCamera->m_fCamHEIGHT,
		0.0f);

	D3DXVec3Project(
		vLINE,
		&vMOVE,
		&vVIEW,
		&m_pCamera->m_matProjection,
		&m_pCamera->m_matView,
		&vWORLD);

	if( vLINE[0].y < m_pCamera->m_fHeight / 6.0f )
	{
		vSCREEN[0].y = vSCREEN[1].y = min( m_pCamera->m_fHeight / 2.0f, 2.0f * m_pCamera->m_fHeight / 6.0f - vLINE[0].y);
		vSCREEN[0].x = vSCREEN[1].x = vLINE[0].x;

		D3DXVec3UnprojectArray(
			vLINE,
			sizeof(D3DXVECTOR3),
			vSCREEN,
			sizeof(D3DXVECTOR3),
			&vVIEW,
			&m_pCamera->m_matProjection,
			&m_pCamera->m_matView,
			&vWORLD, 2);

		D3DXPlaneFromPointNormal(
			&vPLANE,
			&vMOVE,
			&m_pCamera->m_vAxisZ);

		D3DXPlaneIntersectLine(
			&vMOVE,
			&vPLANE,
			&vLINE[0],
			&vLINE[1]);

		m_pCamera->Move(
			-vMOVE.x,
			m_pCamera->m_fCamHEIGHT - vMOVE.y,
			-vMOVE.z,
			TRUE);
	}

	m_pCamera->Activate(TRUE);
}

void CTClientNET::CalcMODEL( DWORD dwTick)
{
	D3DXMATRIX vROT;

	m_vBACK.CalcTick(
		m_pDevice->m_pDevice,
		dwTick);

	((CTachyonObject *) &m_vMODEL)->CalcTick(
		m_pDevice->m_pDevice,
		dwTick);

	FLOAT fROT = D3DX_PI + atan2f( m_pCamera->m_vPosition.x, m_pCamera->m_vPosition.z) + FLOAT(m_wROT) * D3DX_PI / 900.0f;
	D3DXMatrixRotationY(
		&vROT,
		fROT);

	m_vMODEL.SetPosition( vROT * m_vMODEL.GetPositionMat() );
}

void CTClientNET::ResetConstant()
{
	static int vLightCount[4] = { TLIGHT_COUNT, 0, 1, 0};

	D3DMATERIAL9 mtrl = {
		{ 1.0f, 1.0f, 1.0f, 1.0f},
		{ 1.0f, 1.0f, 1.0f, 1.0f},
		{ 0.0f, 0.0f, 0.0f, 0.0f},
		{ 0.0f, 0.0f, 0.0f, 0.0f},
		0.0f};
	m_pDevice->m_pDevice->SetMaterial(&mtrl);

	if(!m_pDevice->m_bEnableSHADER)
		return;
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
		m_pCamera->m_vPosition.x,
		m_pCamera->m_vPosition.y,
		m_pCamera->m_vPosition.z,
		0.0f), 1);

	D3DXMatrixTranspose( (LPD3DXMATRIX) vCONST, &(m_pCamera->m_matView * m_pCamera->m_matProjection));
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

	for( i=0; i<TLIGHT_COUNT; i++)
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

	for( i=0; i<TLIGHT_COUNT; i++)
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
	static WORD wMapID[TCONTRY_COUNT][TMAINRACE_COUNT] = {{
		TNETFOG_MAPID_D_H,
		TNETFOG_MAPID_D_B,
		TNETFOG_MAPID_D_N}, {
		TNETFOG_MAPID_C_H,
		TNETFOG_MAPID_C_B,
		TNETFOG_MAPID_C_N}, {
		TNETFOG_MAPID_B_H,
		TNETFOG_MAPID_B_B,
		TNETFOG_MAPID_B_N}};

	static BYTE bUNIT[TCONTRY_COUNT][TMAINRACE_COUNT][2] = {{{
		TNETFOG_UNITX_D_H,
		TNETFOG_UNITY_D_H}, {
		TNETFOG_UNITX_D_B,
		TNETFOG_UNITY_D_B}, {
		TNETFOG_UNITX_D_N,
		TNETFOG_UNITY_D_N}}, {{
		TNETFOG_UNITX_C_H,
		TNETFOG_UNITY_C_H}, {
		TNETFOG_UNITX_C_B,
		TNETFOG_UNITY_C_B}, {
		TNETFOG_UNITX_C_N,
		TNETFOG_UNITY_C_N}}, {{
		TNETFOG_UNITX_B_H,
		TNETFOG_UNITY_B_H}, {
		TNETFOG_UNITX_B_B,
		TNETFOG_UNITY_B_B}, {
		TNETFOG_UNITX_B_N,
		TNETFOG_UNITY_B_N}}};

	LPTUNITFOG pTFOG = CTChart::FindTFOG(
		wMapID[m_vMODEL.m_bContryID][m_vMODEL.GetRaceID()],
		bUNIT[m_vMODEL.m_bContryID][m_vMODEL.GetRaceID()][0],
		bUNIT[m_vMODEL.m_bContryID][m_vMODEL.GetRaceID()][1]);

	if(m_pDevice->m_vCAPS.RasterCaps & D3DPRASTERCAPS_WFOG)
	{
		m_pDevice->m_pDevice->SetRenderState( D3DRS_FOGTABLEMODE, pTFOG->m_pTGLOBAL->m_bType);
		m_pDevice->m_pDevice->SetRenderState( D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
	}
	else
	{
		m_pDevice->m_pDevice->SetRenderState( D3DRS_FOGVERTEXMODE, pTFOG->m_pTGLOBAL->m_bType);
		m_pDevice->m_pDevice->SetRenderState( D3DRS_FOGTABLEMODE, D3DFOG_NONE);
		m_pDevice->m_pDevice->SetRenderState( D3DRS_RANGEFOGENABLE, FALSE);
	}

	m_pDevice->m_pDevice->SetRenderState( D3DRS_FOGCOLOR, D3DCOLOR_ARGB( 0xFF,
		pTFOG->m_pTGLOBAL->m_bRValue,
		pTFOG->m_pTGLOBAL->m_bGValue,
		pTFOG->m_pTGLOBAL->m_bBValue));
	m_pDevice->m_pDevice->SetRenderState( D3DRS_FOGENABLE, TRUE);

	switch(pTFOG->m_pTGLOBAL->m_bType)
	{
	case D3DFOG_LINEAR	:
		{
			m_pDevice->m_pDevice->SetRenderState( D3DRS_FOGSTART, *(LPDWORD) (&pTFOG->m_pTGLOBAL->m_fSTART));
			m_pDevice->m_pDevice->SetRenderState( D3DRS_FOGEND, *(LPDWORD) (&pTFOG->m_pTGLOBAL->m_fEND));
		}

		break;

	default				: m_pDevice->m_pDevice->SetRenderState( D3DRS_FOGDENSITY, *(LPDWORD) (&pTFOG->m_pTGLOBAL->m_fDENSITY)); break;
	}

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

HRESULT CTClientNET::Render( DWORD dwTickCount)
{
	static DWORD dwLevel = ID_FRAME_CHAR;

	m_dwLevelTime += dwTickCount;

	switch(m_dwLevel)
	{
	case ID_FRAME_CHAR		:
		{
			TFrame *pFRAME = FindFrame(ID_FRAME_CHAR);

			TImageList *pFACE[MAX_SLOT] = {
				(TImageList *) pFRAME->FindKid(ID_CTRLINST_FACE0),
				(TImageList *) pFRAME->FindKid(ID_CTRLINST_FACE1),
				(TImageList *) pFRAME->FindKid(ID_CTRLINST_FACE2),
				(TImageList *) pFRAME->FindKid(ID_CTRLINST_FACE3),
				(TImageList *) pFRAME->FindKid(ID_CTRLINST_FACE4),
				(TImageList *) pFRAME->FindKid(ID_CTRLINST_FACE5)};

			TImageList *pCLASS[MAX_SLOT] = {
				(TImageList *) pFRAME->FindKid(ID_CTRLINST_CLASS0),
				(TImageList *) pFRAME->FindKid(ID_CTRLINST_CLASS1),
				(TImageList *) pFRAME->FindKid(ID_CTRLINST_CLASS2),
				(TImageList *) pFRAME->FindKid(ID_CTRLINST_CLASS3),
				(TImageList *) pFRAME->FindKid(ID_CTRLINST_CLASS4),
				(TImageList *) pFRAME->FindKid(ID_CTRLINST_CLASS5)};
			CPoint point;

			GetCursorPos(&point);
			m_pCommandHandler->ScreenToClient(&point);

			for( BYTE i=0; i<MAX_SLOT; i++)
			{
				if( i < BYTE(m_pHost->m_vTCHARINFO.size()) && pCLASS[i]->HitTest(point) )
					pCLASS[i]->m_strText = CTChart::LoadString( (TSTRING) CTClientGame::m_vTCLASSSTR[m_pHost->m_vTCHARINFO[i]->m_bClass]);
				else
					pCLASS[i]->m_strText.Empty();

				if( i < BYTE(m_pHost->m_vTCHARINFO.size()) && pFACE[i]->HitTest(point) )
					pFACE[i]->m_strText = CTChart::LoadString( (TSTRING) CTClientGame::m_vTRACESTR[m_pHost->m_vTCHARINFO[i]->m_bRace]);
				else
					pFACE[i]->m_strText.Empty();
			}
		}

	case ID_FRAME_NEWCHAR	:
		{
			if( m_dwLevel == ID_FRAME_NEWCHAR )
			{
				CPoint point;
				GetCursorPos(&point);
				m_pCommandHandler->ScreenToClient(&point);

				TFrame *pFRAME = FindFrame(ID_FRAME_NEWCHAR);

				TComponent *pSEX[2] = {
					pFRAME->FindKid(ID_CTRLINST_SEX_SEL_M),
					pFRAME->FindKid(ID_CTRLINST_SEX_SEL_W)};

				TComponent *pSEX_NAME[2] = {
					pFRAME->FindKid(ID_CTRLINST_TEXT_M),
						pFRAME->FindKid(ID_CTRLINST_TEXT_W)};

					if( pSEX[ 0 ] ->HitTest(point) )
					{
						pSEX_NAME[0]->ShowComponent(TRUE);
						pSEX_NAME[1]->ShowComponent(FALSE);
					}
					else if( pSEX[ 1 ] ->HitTest(point) )
					{
						pSEX_NAME[0]->ShowComponent(FALSE);
						pSEX_NAME[1]->ShowComponent(TRUE);
					}
					else
					{
						pSEX_NAME[0]->ShowComponent(FALSE);	
						pSEX_NAME[1]->ShowComponent(FALSE);
					}
			}

			D3DXMATRIX vMAT;

			if( dwLevel == ID_FRAME_LOGIN )
			{
				dwLevel = ID_FRAME_CHAR;
				m_vDEFBACK.ClearSFX();
				CTachyonSFX::ReleaseNoHost();

				InitBACK();
			}

			CalcCamera(dwTickCount);
			CalcMODEL(dwTickCount);
			ResetConstant();

			CTachyonSFX::CalcBUF(
				m_pDevice->m_pDevice,
				dwTickCount);

			m_pDevice->m_pDevice->SetRenderState( D3DRS_AMBIENT, 0xFF000000);
			CTachyonMesh::BeginGlobalDraw(m_pDevice->m_pDevice);
			ResetFOG();

			m_vBACK.Render(
				m_pDevice,
				m_pCamera);

			m_vMODEL.Render(
				m_pDevice,
				m_pCamera);

			CTachyonMesh::EndGlobalDraw(m_pDevice->m_pDevice);
			m_pDevice->m_pDevice->SetRenderState( D3DRS_FOGENABLE, FALSE);

			CTachyonSFX::RenderBUF(
				m_pDevice,
				m_pCamera, 0);

			m_vMODEL.RenderWeaponEffect(
				m_pDevice,
				m_pCamera);
		}

		break;

	case ID_FRAME_LOGIN		:
	case ID_FRAME_SERVER	:
	case ID_FRAME_AGREEMENT	:
		{
			if( dwLevel == ID_FRAME_CHAR )
			{
				dwLevel = ID_FRAME_LOGIN;
				m_vMODEL.ClearSFX();
				m_vBACK.ClearSFX();
				CTachyonSFX::ReleaseNoHost();

				InitDEFBACK();
			}

			if( m_dwLevel == ID_FRAME_SERVER )
			{
				TFrame *pFRAME = FindFrame(ID_FRAME_SERVER);
				TList* pLIST = (TList*) pFRAME->FindKid(ID_CTRLINST_LIST);
				BYTE bSEL = (BYTE)pLIST->GetSel();

				if( bSEL != m_bSelServerNum )
				{
					EnableComponent(FALSE);

					LPTGROUP pGROUP = (LPTGROUP) pLIST->GetItemData(bSEL, 0);
					m_pHost->SendCS_CHANNELLIST_REQ(pGROUP->m_bGroupID);

					m_bSelServerGropNum = pGROUP->m_bGroupID;
					m_bSelServerNum = bSEL;
				}
			}
			else if( m_dwLevel == ID_FRAME_AGREEMENT )
			{
				TFrame* pFRAME = GetCurrentFrame();
				TList* pList = static_cast<TList*>( pFRAME->FindKid(ID_CTRLINST_LIST) );
				if( pList->GetTop() >= pList->GetItemCount() - pList->GetItemPerPage() )
				{
					TComponent* pAgreeBtn = pFRAME->FindKid(ID_CTRLINST_AGREE);
					TComponent* pQuitBtn  = pFRAME->FindKid(ID_CTRLINST_QUIT);

					pAgreeBtn->EnableComponent(TRUE);
					pQuitBtn->EnableComponent(TRUE);

					pAgreeBtn->Get3DFont()->m_dwColor = 0xFFFFFFFF;
					pQuitBtn->Get3DFont()->m_dwColor  = 0xFFFFFFFF;
				}
			}
			else if( CTNationOption::MODIFY_DIRECTLOGIN )
			{
				if( m_dwLevel == ID_FRAME_LOGIN && m_pHost->m_bDirectLogin )
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

			m_vDEFBACK.CalcTick(
				m_pDevice->m_pDevice,
				dwTickCount);

			CTachyonSFX::CalcBUF(
				m_pDevice->m_pDevice,
				dwTickCount);

			CalcDEFCamera(dwTickCount);
			ResetConstant();

			m_pDevice->m_pDevice->SetRenderState( D3DRS_AMBIENT, 0xFF000000);
			CTachyonMesh::BeginGlobalDraw(m_pDevice->m_pDevice);
			ResetDEFFOG();

			m_vDEFBACK.Render(
				m_pDevice,
				m_pCamera);

			CTachyonMesh::EndGlobalDraw(m_pDevice->m_pDevice);
			m_pDevice->m_pDevice->SetRenderState( D3DRS_FOGENABLE, FALSE);

			CTachyonSFX::RenderBUF(
				m_pDevice,
				m_pCamera, 0);
		}

		break;

	default					:
		{
			m_vDEFBACK.ClearSFX();
			m_vBACK.ClearSFX();
			m_vMODEL.ClearSFX();

			CTachyonSFX::ReleaseNoHost();
		}

		break;
	}

	switch(m_dwLevel)
	{
	case ID_FRAME_CHAR		:
	case ID_FRAME_SERVER	:
	case ID_FRAME_NEWCHAR	:
	case ID_FRAME_CONTRY	:
		{
			TFrame *pFRAME = FindFrame(m_dwLevel);
			TComponent* pPCROOM = pFRAME->FindKid(ID_CTRLINST_PCROOM);
			pPCROOM->ShowComponent(m_pHost->m_bInPCROOM);

			if( m_dwLevel == ID_FRAME_CONTRY )
			{
				if( m_bInvertCountrySel )
				{
					ResetPosition();

					static const INT nCount = 6;
					static DWORD dwEXCHANGE[nCount][2] =
					{
						{ ID_CTRLINST_BD_C_UNSELECTED, ID_CTRLINST_BG_C_UNSELECTED },
						{ ID_CTRLINST_BG_D, ID_CTRLINST_BG_C },
						{ ID_CTRLINST_CON_D_BACK, ID_CTRLINST_CON_C_BACK },
						{ ID_CTRLINST_CON_D, ID_CTRLINST_CON_C },
						{ ID_CTRLINST_TXT1, ID_CTRLINST_TXT2 },
						{ ID_CTRLINST_CON_D_INFO, ID_CTRLINST_CON_C_INFO }
					};

					pFRAME = FindFrame(ID_FRAME_CONTRY);
					for( INT i=0 ; i < nCount ; ++i )
					{
						TComponent* pSRC = pFRAME->FindKid( dwEXCHANGE[ i ][0] );
						TComponent* pDST = pFRAME->FindKid( dwEXCHANGE[ i ][1] );
						if( pSRC && pDST )
						{
							CRect rectSRC, rectDST;
							pSRC->GetComponentRect( &rectSRC );
							pDST->GetComponentRect( &rectDST );

							pSRC->SetComponentRect( &rectDST );
							pDST->SetComponentRect( &rectSRC );
						}
					}
				}
				else
					ResetPosition();
			}

			if( m_dwLevel == ID_FRAME_NEWCHAR )
			{
				TComponent* pCOMP = pFRAME->FindKid( ID_CTRLINST_TXT_9);
				if( pCOMP ) pCOMP->ShowComponent(FALSE);
				pCOMP = pFRAME->FindKid( ID_CTRLINST_TXT_MAKESLOT);
				if( pCOMP ) pCOMP->ShowComponent(FALSE);
			}
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
				// Arab Ver.
				CTNationOption::EGYPT ||
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
	// New Arab Ver.
	if(CTNationOption::EGYPT)
	{
		TFrame *pFRAME = FindFrame(m_dwLevel);
		TComponent* p15ICON = pFRAME->FindKid(ID_CTRLINST_15_ICON);
		if(p15ICON)
			p15ICON->ShowComponent(FALSE);
	}

	if( CTNationOption::JAPAN_IME || CTNationOption::TAIWAN_IME )
	{
		if( m_pCandidate && m_pCandidate->IsVisible())
		{
			RemoveKid(m_pCandidate);
			AddKid(m_pCandidate);
		}
	}

	return CFrameGroup::Render(dwTickCount);
}

void CTClientNET::SwitchFocus(TComponent *pCandidate)
{
	if( m_dwLevel == ID_FRAME_AGREEMENT )
	{
		m_pFocus = pCandidate;
		return;
	}

	CFrameGroup::SwitchFocus(pCandidate);
}

void CTClientNET::OnKeyDown( UINT nChar, int nRepCnt, UINT nFlags)
{
	switch(m_dwLevel)
	{
	case ID_FRAME_CHAR		:
		if(!m_pHost->m_vTCHARINFO.empty())
		{
			BYTE bCount = BYTE(m_pHost->m_vTCHARINFO.size());

			switch(nChar)
			{
			case VK_UP		: m_pHost->m_bSlotID += bCount - 1; break;
			case VK_DOWN	: m_pHost->m_bSlotID++; break;
			}

			m_pHost->m_bSlotID %= bCount;
			m_pHost->SelectCHAR();
		}

		break;

	case ID_FRAME_LOGIN:
			if( !m_bClickedIDBox )
			{
				TFrame* pFrame = FindFrame( ID_FRAME_LOGIN );
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
	if( nChar == VK_SNAPSHOT )
		m_pHost->CaptureIMG();

	CFrameGroup::OnKeyUp( nChar, nRepCnt, nFlags);
}

void CTClientNET::OnImeNotify( HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch(m_dwLevel)
	{
	case ID_FRAME_LOGIN:
		{
			if( !m_bClickedIDBox )
			{
				TFrame* pFrame = FindFrame( ID_FRAME_LOGIN );
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
	case ID_FRAME_LOGIN:
		{
			if( !m_bClickedIDBox )
			{
				TFrame* pFrame = FindFrame( ID_FRAME_LOGIN );
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

	case ID_FRAME_NEWCHAR	:
	case ID_FRAME_CHAR		:
		if(!HitTest(pt))
		{
			CPoint point(
				m_pDevice->m_option.m_dwScreenX / 2,
				m_pDevice->m_option.m_dwScreenY / 2);

			ClientToScreen( AfxGetMainWnd()->GetSafeHwnd(), &point);
			SetCursorPos( point.x, point.y);
			m_bROT = TRUE;
		}

		break;

	case ID_FRAME_CONTRY	:
		{
			TFrame* pFrame = FindFrame(ID_FRAME_CONTRY);

			static DWORD dwEXCEP[] =
			{
				ID_CTRLINST_BACK,
				ID_CTRLINST_OK
			};

			BOOL bBreak = FALSE;
			for( INT i=0 ; i < 2 ; ++i )
			{
				TComponent* pCOMP = pFrame->FindKid( dwEXCEP[i] );
				if( pCOMP )
				{
					CRect rc;
					pCOMP->GetComponentRect(&rc);

					if( rc.PtInRect(pt) )
					{
						bBreak = TRUE;
						break;
					}
				}
			}

			if( bBreak ) 
				break;

			TComponent* pTarget, *pTarget2;
			if( m_pHost->m_vNEWTCHAR.m_bContry == TCONTRY_D )
			{
				pTarget = pFrame->FindKid(ID_CTRLINST_CON_C);
				pTarget2 = pFrame->FindKid(ID_CTRLINST_BG_C_UNSELECTED);				
			}
			else
			{
				pTarget = pFrame->FindKid(ID_CTRLINST_CON_D);
				pTarget2 = pFrame->FindKid( ID_CTRLINST_BD_C_UNSELECTED);
			}

			CRect rc, rc2;
			pTarget->GetComponentRect(&rc);
			pTarget2->GetComponentRect(&rc2);
			
			if( rc.PtInRect(pt) || rc2.PtInRect(pt) )
			{
				if( m_pHost->m_vNEWTCHAR.m_bContry == TCONTRY_D )
					m_pHost->m_vNEWTCHAR.m_bContry = TCONTRY_C;
				else
					m_pHost->m_vNEWTCHAR.m_bContry = TCONTRY_D;

				m_pHost->SelectCONTRY();
			}
		}

		break;
	}

	CFrameGroup::OnLButtonDown( nFlags, pt);
}

void CTClientNET::OnMouseMove( UINT nFlags, CPoint pt)
{
	if(m_bROT)
	{
		CPoint point(
			m_pDevice->m_option.m_dwScreenX / 2,
			m_pDevice->m_option.m_dwScreenY / 2);
		int nROT = INT(m_wROT) + point.x - pt.x;

		while(nROT < 0)
			nROT += 1800;
		m_wROT = nROT % 1800;

		ClientToScreen( AfxGetMainWnd()->GetSafeHwnd(), &point);
		SetCursorPos( point.x, point.y);
	}

	CFrameGroup::OnMouseMove( nFlags, pt);
}

void CTClientNET::OnLButtonUp( UINT nFlags, CPoint pt)
{
	if(m_bROT)
		m_bROT = FALSE;

	CFrameGroup::OnLButtonUp( nFlags, pt);
}

void CTClientNET::DefaultPosition( CPoint* vBASIS, BOOL bRestore )
{
	static const DWORD TLOGINFRAME_COMP_COUNT = 14;
	static const DWORD TSERVERFRAME_COMP_COUNT = 15;
	// Arab Ver.
	static const DWORD TCHARFRAME_COMP_COUNT = 54;
	// Arab Ver.
	static const DWORD TNEWCHARFRAME_COMP_COUNT = 37;
	static const DWORD TLOADINGCOMP_COMP_COUNT = 8;
	static const DWORD TCOUNTRY_COMP_COUNT = 20;
	static const DWORD TAGREEMENT_COMP_COUNT = 8;

	static TFRAMEOFFSET m_vLOGINCOMP[TLOGINFRAME_COMP_COUNT];
	static TFRAMEOFFSET m_vSERVERCOMP[TSERVERFRAME_COMP_COUNT];
	static TFRAMEOFFSET m_vCHARCOMP[TCHARFRAME_COMP_COUNT];
	static TFRAMEOFFSET m_vNEWCHARCOMP[TNEWCHARFRAME_COMP_COUNT];
	static TFRAMEOFFSET m_vLOADINGCOMP[TLOADINGCOMP_COMP_COUNT];
	static TFRAMEOFFSET m_vCONTRYCOMP[TCOUNTRY_COMP_COUNT];
	static TFRAMEOFFSET m_vAGREEMENTCOMP[TAGREEMENT_COMP_COUNT];

	if( bRestore )
	{
		TFrame *pFRAME = FindFrame(ID_FRAME_LOGIN);
		for( INT i=0 ; i < TLOGINFRAME_COMP_COUNT ; ++i )
		{
			TComponent* pCOMP = pFRAME->FindKid( m_vLOGINCOMP[i].m_dwFRAME );
			CPoint vCompOffset( m_vLOGINCOMP[i].m_vOffset );
			// Arab Ver.
			vCompOffset = CTClientUIBase::GetUIBasisPos( m_vLOGINCOMP[i].m_bBasisPoint, vCompOffset, pCOMP->m_rc );
			
			pCOMP->ScreenToComponent(&vCompOffset);
			pCOMP->MoveComponent(vCompOffset);
		}

		pFRAME = FindFrame(ID_FRAME_SERVER);
		for( INT i=0 ; i < TSERVERFRAME_COMP_COUNT ; ++i )
		{
			TComponent* pCOMP = pFRAME->FindKid( m_vSERVERCOMP[i].m_dwFRAME );
			CPoint vCompOffset( m_vSERVERCOMP[i].m_vOffset );
			// Arab Ver.
			vCompOffset = CTClientUIBase::GetUIBasisPos( m_vSERVERCOMP[i].m_bBasisPoint, vCompOffset, pCOMP->m_rc );

			pCOMP->ScreenToComponent(&vCompOffset);
			pCOMP->MoveComponent(vCompOffset);
		}

		pFRAME = FindFrame(ID_FRAME_CHAR);
		for( INT i=0 ; i < TCHARFRAME_COMP_COUNT ; ++i )
		{
			TComponent* pCOMP = pFRAME->FindKid( m_vCHARCOMP[i].m_dwFRAME );
			CPoint vCompOffset( m_vCHARCOMP[i].m_vOffset );
			// Arab Ver.
			vCompOffset = CTClientUIBase::GetUIBasisPos( m_vCHARCOMP[i].m_bBasisPoint, vCompOffset, pCOMP->m_rc );
			
			pCOMP->ScreenToComponent(&vCompOffset);
			pCOMP->MoveComponent(vCompOffset);
		}
		
		pFRAME = FindFrame(ID_FRAME_NEWCHAR);
		for( INT i=0 ; i < TNEWCHARFRAME_COMP_COUNT ; ++i )
		{
			TComponent* pCOMP = pFRAME->FindKid( m_vNEWCHARCOMP[i].m_dwFRAME );
			CPoint vCompOffset( m_vNEWCHARCOMP[i].m_vOffset );
			// Arab Ver.
			vCompOffset = CTClientUIBase::GetUIBasisPos( m_vNEWCHARCOMP[i].m_bBasisPoint, vCompOffset, pCOMP->m_rc );
			
			pCOMP->ScreenToComponent(&vCompOffset);
			pCOMP->MoveComponent(vCompOffset);
		}
		
		pFRAME = FindFrame(ID_FRAME_LOADING);
		for( INT i=0 ; i < TLOADINGCOMP_COMP_COUNT ; ++i )
		{
			TComponent* pCOMP = pFRAME->FindKid( m_vLOADINGCOMP[i].m_dwFRAME );
			CPoint vCompOffset( m_vLOADINGCOMP[i].m_vOffset );
			  // Arab Ver.
			vCompOffset = CTClientUIBase::GetUIBasisPos( m_vLOADINGCOMP[i].m_bBasisPoint, vCompOffset, pCOMP->m_rc );

			pCOMP->ScreenToComponent(&vCompOffset);
			pCOMP->MoveComponent(vCompOffset);
		}
		
		pFRAME = FindFrame(ID_FRAME_CONTRY);
		for( INT i=0 ; i < TCOUNTRY_COMP_COUNT ; ++i )
		{
			TComponent* pCOMP = pFRAME->FindKid( m_vCONTRYCOMP[i].m_dwFRAME );
			CPoint vCompOffset( m_vCONTRYCOMP[i].m_vOffset );
			// Arab Ver.
			vCompOffset = CTClientUIBase::GetUIBasisPos( m_vCONTRYCOMP[i].m_bBasisPoint, vCompOffset, pCOMP->m_rc );

			pCOMP->ScreenToComponent(&vCompOffset);
			pCOMP->MoveComponent(vCompOffset);
		}
		
		pFRAME = FindFrame(ID_FRAME_AGREEMENT);
		for( INT i=0 ; i < TAGREEMENT_COMP_COUNT ; ++i )
		{
			TComponent* pCOMP = pFRAME->FindKid( m_vAGREEMENTCOMP[i].m_dwFRAME );
			CPoint vCompOffset( m_vAGREEMENTCOMP[i].m_vOffset );
			// Arab Ver.
			vCompOffset = CTClientUIBase::GetUIBasisPos( m_vAGREEMENTCOMP[i].m_bBasisPoint, vCompOffset, pCOMP->m_rc );

			pCOMP->ScreenToComponent(&vCompOffset);
			pCOMP->MoveComponent(vCompOffset);
		}
	}
	else
	{
		// ID_FRAME_LOGIN
		{
			TFrame *pFRAME = FindFrame(ID_FRAME_LOGIN);
			// Arab Ver.
			static DWORD vCOMP[TLOGINFRAME_COMP_COUNT][3] =
			{
				{ ID_CTRLINST_LEFT_TOP_LOGO, TBASISPOINT_LEFT_TOP, TRUE },
				{ ID_CTRLINST_LEFT_BOTTOM_LOGO, TBASISPOINT_LEFT_BOTTOM, TRUE },
				{ ID_CTRLINST_15_ICON, TBASISPOINT_RIGHT_TOP, TRUE },
				{ ID_CTRLINST_BACK, TBASISPOINT_CENTER_MIDDLE, FALSE },
				{ ID_CTRLINST_ID_BACK, TBASISPOINT_CENTER_MIDDLE, FALSE },
				{ ID_CTRLINST_PW_BACK, TBASISPOINT_CENTER_MIDDLE, FALSE },
				{ ID_CTRLINST_ID, TBASISPOINT_CENTER_MIDDLE, FALSE },
				{ ID_CTRLINST_PW, TBASISPOINT_CENTER_MIDDLE, FALSE },
				{ ID_CTRLINST_LOGIN, TBASISPOINT_CENTER_MIDDLE, FALSE },
				{ ID_CTRLINST_HOMEPAGE, TBASISPOINT_CENTER_MIDDLE, FALSE },
				{ ID_CTRLINST_QUIT, TBASISPOINT_RIGHT_BOTTOM, TRUE },
				{ ID_CTRLINST_TITLE, TBASISPOINT_CENTER_MIDDLE, FALSE },
				{ ID_CTRLINST_ID_TEXT, TBASISPOINT_CENTER_MIDDLE, FALSE },
				{ ID_CTRLINST_PW_TEXT, TBASISPOINT_CENTER_MIDDLE, FALSE },
			};

			for( INT i=0 ; i < TLOGINFRAME_COMP_COUNT ; ++i )
			{
				TComponent* pCOMP = pFRAME->FindKid( vCOMP[ i ][0] );
				if( pCOMP )
				{
					CPoint pt;
					pCOMP->GetComponentPos(&pt);
					pt -= CTClientUIBase::m_vDefaultBASIS[ vCOMP[i][1] ];
					// Arab Ver.
					m_vLOGINCOMP[i].m_dwFRAME = vCOMP[i][0];
					m_vLOGINCOMP[i].m_bBasisPoint = vCOMP[i][2] ? CTClientUIBase::GetBasis((BYTE)vCOMP[i][1]) : (BYTE)vCOMP[i][1];
					m_vLOGINCOMP[i].m_vOffset = vCOMP[i][2] ? CTClientUIBase::GetOffset(pt, pCOMP->m_rc) : pt;
				}
			}
		}

		// ID_FRAME_SERVER
		{
			TFrame *pFRAME = FindFrame(ID_FRAME_SERVER);
			// Arab Ver.
			static DWORD vCOMP[TSERVERFRAME_COMP_COUNT][3] =
			{
				{ ID_CTRLINST_LEFT_TOP_LOGO, TBASISPOINT_LEFT_TOP, TRUE },
				{ ID_CTRLINST_LEFT_BOTTOM_LOGO, TBASISPOINT_LEFT_BOTTOM, TRUE },
				{ ID_CTRLINST_15_ICON, TBASISPOINT_RIGHT_TOP, TRUE },
				{ ID_CTRLINST_SERVER_BACK, TBASISPOINT_CENTER_MIDDLE, FALSE },
				{ ID_CTRLINST_COLMN_BACK, TBASISPOINT_CENTER_MIDDLE, FALSE },
				{ ID_CTRLINST_LIST, TBASISPOINT_CENTER_MIDDLE, FALSE },
				{ ID_CTRLINST_NAME, TBASISPOINT_CENTER_MIDDLE, FALSE },
				{ ID_CTRLINST_KIND, TBASISPOINT_CENTER_MIDDLE, FALSE },
				{ ID_CTRLINST_STATUS, TBASISPOINT_CENTER_MIDDLE, FALSE },
				{ ID_CTRLINST_CHAR, TBASISPOINT_CENTER_MIDDLE, FALSE },
				{ ID_CTRLINST_OK, TBASISPOINT_CENTER_MIDDLE, FALSE },
				{ ID_CTRLINST_BACK, TBASISPOINT_RIGHT_BOTTOM, TRUE },
				{ ID_CTRLINST_SERVER, TBASISPOINT_CENTER_MIDDLE, FALSE },
				{ ID_CTRLINST_POS_CHANNEL, TBASISPOINT_CENTER_MIDDLE, FALSE },
				{ ID_CTRLINST_PCROOM, TBASISPOINT_RIGHT_TOP, TRUE }
			};

			for( INT i=0 ; i < TSERVERFRAME_COMP_COUNT ; ++i )
			{
				TComponent* pCOMP = pFRAME->FindKid( vCOMP[ i ][0] );
				if( pCOMP )
				{
					CPoint pt;
					pCOMP->GetComponentPos(&pt);
					pt -= CTClientUIBase::m_vDefaultBASIS[ vCOMP[i][1] ];
					// Arab Ver.
					m_vSERVERCOMP[i].m_dwFRAME = vCOMP[i][0];
					m_vSERVERCOMP[i].m_bBasisPoint = vCOMP[i][2] ? CTClientUIBase::GetBasis((BYTE)vCOMP[i][1]) : (BYTE)vCOMP[i][1];
					m_vSERVERCOMP[i].m_vOffset = vCOMP[i][2] ? CTClientUIBase::GetOffset(pt, pCOMP->m_rc) : pt;
					// Arab Ver.
					if(vCOMP[ i ][0] == ID_CTRLINST_LIST)
						CTClientUIBase::FlipControl(pCOMP);
				}
			}
		}

		// ID_FRAME_CHAR
		{
			TFrame *pFRAME = FindFrame(ID_FRAME_CHAR);
			// Arab Ver.
			static DWORD vCOMP[TCHARFRAME_COMP_COUNT][3] =
			{
				{ ID_CTRLINST_LEFT_TOP_LOGO, TBASISPOINT_LEFT_TOP, FALSE },
				{ ID_CTRLINST_LEFT_BOTTOM_LOGO, TBASISPOINT_LEFT_BOTTOM, FALSE },
				{ ID_CTRLINST_15_ICON, TBASISPOINT_RIGHT_TOP, FALSE },
				{ ID_CTRLINST_BASE, TBASISPOINT_RIGHT_MIDDLE, FALSE },
				{ ID_CTRLINST_NEWCHAR, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_START, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_DELETE, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_BACK, TBASISPOINT_RIGHT_BOTTOM, FALSE },
				{ ID_CTRLINST_CHAR0, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_CHAR1, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_CHAR2, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_CHAR3, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_CHAR4, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_CHAR5, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_SEL0, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_SEL1, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_SEL2, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_SEL3, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_SEL4, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_SEL5, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_CLASS0, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_CLASS1, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_CLASS2, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_CLASS3, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_CLASS4, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_CLASS5, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_LOC0, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_LOC1, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_LOC2, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_LOC3, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_LOC4, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_LOC5, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_LVL0, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_LVL1, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_LVL2, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_LVL3, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_LVL4, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_LVL5, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_NAME0, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_NAME1, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_NAME2, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_NAME3, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_NAME4, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_NAME5, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_SERVER, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_FACE0, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_FACE1, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_FACE2, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_FACE3, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_FACE4, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_FACE5, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_PCROOM, TBASISPOINT_RIGHT_TOP, FALSE },
				{ ID_CTRLINST_NAME_SELECTED, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_TEXT_NAME, TBASISPOINT_RIGHT_MIDDLE, TRUE },
			};
			// Arab Ver.
			TComponent *pBASE = pFRAME->FindKid(ID_CTRLINST_BASE);

			for( INT i=0 ; i < TCHARFRAME_COMP_COUNT ; ++i )
			{
				TComponent* pCOMP = pFRAME->FindKid( vCOMP[ i ][0] );
				if( pCOMP )
				{
					CPoint pt;

					// Arab Ver.
					if( CTNationOption::FLIP_SCREEN && vCOMP[i][2] && pBASE )
						pt = CTClientUIBase::GetFlipPos( pCOMP, pBASE);
					else
						pCOMP->GetComponentPos(&pt);
					pt -= CTClientUIBase::m_vDefaultBASIS[ vCOMP[i][1] ];

					m_vCHARCOMP[i].m_dwFRAME = vCOMP[i][0];
					m_vCHARCOMP[i].m_bBasisPoint = CTClientUIBase::GetBasis((BYTE) vCOMP[i][1]);
					m_vCHARCOMP[i].m_vOffset = CTClientUIBase::GetOffset(pt, pCOMP->m_rc);
				}
			}
		}

		// ID_FRAME_NEWCHAR
		{
			TFrame *pFRAME = FindFrame(ID_FRAME_NEWCHAR);

			// Arab Ver.
			static DWORD vCOMP[TNEWCHARFRAME_COMP_COUNT][3] =
			{
				{ ID_CTRLINST_LEFT_TOP_LOGO, TBASISPOINT_LEFT_TOP, FALSE },
				{ ID_CTRLINST_LEFT_BOTTOM_LOGO, TBASISPOINT_LEFT_BOTTOM, FALSE },
				{ ID_CTRLINST_15_ICON, TBASISPOINT_RIGHT_TOP, FALSE },
				{ ID_CTRLINST_BASE, TBASISPOINT_RIGHT_MIDDLE, FALSE },
				{ ID_CTRLINST_NAME, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_OK, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_CANCEL, TBASISPOINT_RIGHT_BOTTOM, FALSE },
				{ ID_CTRLINST_FACE_L, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_FACE_R, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_HAIR_L, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_HAIR_R, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_TEXT_FACE, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_TEXT_HAIRCOLOR, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_TEXT_RACE_NAME, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_TEXT_CLASS_NAME, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_RACE_L, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_RACE_R, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_CLASS_L, TBASISPOINT_RIGHT_MIDDLE, TRUE},
				{ ID_CTRLINST_CLASS_R, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_SERVER, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_TXT_9, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_TXT_MAKESLOT, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_PCROOM, TBASISPOINT_RIGHT_TOP, FALSE },
				{ ID_CTRLINST_SEX_SEL_M, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_SEX_SEL_W, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_FACE_M, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_FACE_W, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_FACE_OUTLINE_M, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_FACE_OUTLINE_W, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_CLASS, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_SEX_SELMASK_M, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_SEX_SELMASK_W, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_TEXT_M, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_TEXT_W, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_TEXT_RACE_INFO, TBASISPOINT_RIGHT_MIDDLE, TRUE },		//List
				{ ID_CTRLINST_TEXT_CLASS_INFO, TBASISPOINT_RIGHT_MIDDLE, TRUE },	//List
				{ ID_CTRLINST_TEXT_NAME, TBASISPOINT_RIGHT_MIDDLE, TRUE },
			};
			// Arab Ver.
			TComponent *pBASE = pFRAME->FindKid(ID_CTRLINST_BASE);

			for( INT i=0 ; i < TNEWCHARFRAME_COMP_COUNT ; ++i )
			{
				TComponent* pCOMP = pFRAME->FindKid( vCOMP[ i ][0] );
				if( pCOMP )
				{
					CPoint pt;

					// Arab Ver.
					if( CTNationOption::FLIP_SCREEN && vCOMP[i][2] && pBASE )
						pt = CTClientUIBase::GetFlipPos( pCOMP, pBASE);
					else
						pCOMP->GetComponentPos(&pt);
					pt -= CTClientUIBase::m_vDefaultBASIS[ vCOMP[i][1] ];

					m_vNEWCHARCOMP[i].m_dwFRAME = vCOMP[i][0];
					m_vNEWCHARCOMP[i].m_bBasisPoint = CTClientUIBase::GetBasis((BYTE) vCOMP[i][1]);
					m_vNEWCHARCOMP[i].m_vOffset = CTClientUIBase::GetOffset(pt, pCOMP->m_rc);

					if(vCOMP[ i ][0] == ID_CTRLINST_TEXT_RACE_INFO || vCOMP[ i ][0] == ID_CTRLINST_TEXT_CLASS_INFO)
						CTClientUIBase::FlipControl(pCOMP);
				}
			}
		}

		// ID_FRAME_LOADING
		{
			TFrame *pFRAME = FindFrame(ID_FRAME_LOADING);
			// Arab Ver.
			static DWORD vCOMP[TLOADINGCOMP_COMP_COUNT][3] =
			{
				{ ID_CTRLINST_BACK_1, TBASISPOINT_CENTER_MIDDLE, FALSE },
				{ ID_CTRLINST_BACK_2, TBASISPOINT_CENTER_MIDDLE, FALSE },
				{ ID_CTRLINST_PCROOM_1, TBASISPOINT_CENTER_MIDDLE, FALSE },
				{ ID_CTRLINST_PCROOM_2, TBASISPOINT_CENTER_MIDDLE, FALSE },
				{ ID_CTRLINST_PCROOM_3, TBASISPOINT_CENTER_MIDDLE, FALSE },
				{ ID_CTRLINST_PCROOM_4, TBASISPOINT_CENTER_MIDDLE, FALSE },
				{ ID_CTRLINST_PCROOM_5, TBASISPOINT_CENTER_MIDDLE, FALSE },
				{ ID_CTRLINST_PCROOM_6, TBASISPOINT_CENTER_MIDDLE, FALSE },
			};

			for( INT i=0 ; i < TLOADINGCOMP_COMP_COUNT ; ++i )
			{
				TComponent* pCOMP = pFRAME->FindKid( vCOMP[ i ][0] );
				if( pCOMP )
				{
					CPoint pt;
					pCOMP->GetComponentPos(&pt);
					pt -= CTClientUIBase::m_vDefaultBASIS[ vCOMP[i][1] ];
					// Arab Ver.
					m_vLOADINGCOMP[i].m_dwFRAME = vCOMP[i][0];
					m_vLOADINGCOMP[i].m_bBasisPoint = vCOMP[i][2] ? CTClientUIBase::GetBasis((BYTE)vCOMP[i][1]) : (BYTE)vCOMP[i][1];
					m_vLOADINGCOMP[i].m_vOffset = vCOMP[i][2] ? CTClientUIBase::GetOffset(pt, pCOMP->m_rc) : pt;
				}
			}
		}

		// ID_FRAME_CONTRY
		{
			TFrame *pFRAME = FindFrame(ID_FRAME_CONTRY);
			// Arab Ver.
			static DWORD vCOMP[TCOUNTRY_COMP_COUNT][3] =
			{
				{ ID_CTRLINST_BD_C_UNSELECTED, TBASISPOINT_CENTER_MIDDLE, FALSE },
				{ ID_CTRLINST_BG_D, TBASISPOINT_CENTER_MIDDLE, FALSE },
				{ ID_CTRLINST_BG_C_UNSELECTED, TBASISPOINT_CENTER_MIDDLE, FALSE },
				{ ID_CTRLINST_BG_C, TBASISPOINT_CENTER_MIDDLE, FALSE },
				{ ID_CTRLINST_LEFT_TOP_LOGO, TBASISPOINT_LEFT_TOP, FALSE },
				{ ID_CTRLINST_LEFT_BOTTOM_LOGO, TBASISPOINT_LEFT_BOTTOM, FALSE },
				{ ID_CTRLINST_15_ICON, TBASISPOINT_RIGHT_TOP, FALSE },
				{ ID_CTRLINST_BACK, TBASISPOINT_RIGHT_BOTTOM, FALSE },
				{ ID_CTRLINST_CON_BACK, TBASISPOINT_RIGHT_MIDDLE, FALSE },
				{ ID_CTRLINST_CON_D_BACK, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_CON_C_BACK, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_CON_D, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_CON_C, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_TITLE, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_TXT1, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_TXT2, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_OK, TBASISPOINT_RIGHT_MIDDLE, TRUE },
				{ ID_CTRLINST_PCROOM, TBASISPOINT_RIGHT_TOP, FALSE },
				{ ID_CTRLINST_CON_D_INFO, TBASISPOINT_RIGHT_MIDDLE, TRUE },	//List
				{ ID_CTRLINST_CON_C_INFO, TBASISPOINT_RIGHT_MIDDLE, TRUE }	//List
			};
			// Arab Ver.
			TComponent *pBASE = pFRAME->FindKid(ID_CTRLINST_CON_BACK);

			for( INT i=0 ; i < TCOUNTRY_COMP_COUNT ; ++i )
			{
				TComponent* pCOMP = pFRAME->FindKid( vCOMP[ i ][0] );
				if( pCOMP )
				{
					CPoint pt;

					// Arab Ver.
					if( CTNationOption::FLIP_SCREEN && vCOMP[i][2] && pBASE )
						pt = CTClientUIBase::GetFlipPos( pCOMP, pBASE);
					else
						pCOMP->GetComponentPos(&pt);
					pt -= CTClientUIBase::m_vDefaultBASIS[ vCOMP[i][1] ];

					m_vCONTRYCOMP[i].m_dwFRAME = vCOMP[i][0];
					m_vCONTRYCOMP[i].m_bBasisPoint = CTClientUIBase::GetBasis((BYTE) vCOMP[i][1]);
					m_vCONTRYCOMP[i].m_vOffset = CTClientUIBase::GetOffset(pt, pCOMP->m_rc);

					if(vCOMP[ i ][0] == ID_CTRLINST_CON_D_INFO || vCOMP[ i ][0] == ID_CTRLINST_CON_C_INFO)
						CTClientUIBase::FlipControl(pCOMP);
				}
			}
		}

		// ID_FRAME_AGREEMENT
		{
			TFrame *pFRAME = FindFrame(ID_FRAME_AGREEMENT);
			// Arab Ver.
			static DWORD vCOMP[TAGREEMENT_COMP_COUNT][3] =
			{
				{ ID_CTRLINST_LEFT_TOP_LOGO, TBASISPOINT_LEFT_TOP, TRUE },
				{ ID_CTRLINST_LEFT_BOTTOM_LOGO, TBASISPOINT_LEFT_BOTTOM, TRUE },
				{ ID_CTRLINST_15_ICON, TBASISPOINT_RIGHT_TOP, TRUE },
				{ ID_CTRLINST_BASE, TBASISPOINT_CENTER_MIDDLE, FALSE },
				{ ID_CTRLINST_AGREE, TBASISPOINT_CENTER_MIDDLE, FALSE },
				{ ID_CTRLINST_QUIT, TBASISPOINT_CENTER_MIDDLE, FALSE },
				{ ID_CTRLINST_TITLE, TBASISPOINT_CENTER_MIDDLE, FALSE },
				{ ID_CTRLINST_LIST, TBASISPOINT_CENTER_MIDDLE, FALSE },	//List
			};

			for( INT i=0 ; i < TAGREEMENT_COMP_COUNT ; ++i )
			{
				TComponent* pCOMP = pFRAME->FindKid( vCOMP[ i ][0] );
				if( pCOMP )
				{
					CPoint pt;
					pCOMP->GetComponentPos(&pt);
					pt -= CTClientUIBase::m_vDefaultBASIS[ vCOMP[i][1] ];
					// Arab Ver.
					m_vAGREEMENTCOMP[i].m_dwFRAME = vCOMP[i][0];
					m_vAGREEMENTCOMP[i].m_bBasisPoint = vCOMP[i][2] ? CTClientUIBase::GetBasis((BYTE)vCOMP[i][1]) : (BYTE)vCOMP[i][1];
					m_vAGREEMENTCOMP[i].m_vOffset = vCOMP[i][2] ? CTClientUIBase::GetOffset(pt, pCOMP->m_rc) : pt;

					if(vCOMP[ i ][0] == ID_CTRLINST_LIST)
						CTClientUIBase::FlipControl(pCOMP);
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
			TFrame *pFRAME = FindFrame(ID_FRAME_LOGIN);
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
		// Arab Ver.
		else if ( CTNationOption::EGYPT )
			m_strCAURL = CString(_T("http://www.4story.eg/user/register"));
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

