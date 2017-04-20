#include "Stdafx.h"
#include "TPetDlg.h"
#include "TClientGame.h"
#include "Resource.h"

// ======================================================================
#define TPETDLG_CAM_DIST		3.0f
// ======================================================================

// ======================================================================
CTPetDlg::CTPetDlg(TComponent* pParent, LP_FRAMEDESC pDesc, CD3DDevice* pDevice)
	: CTClientUIBase(pParent, pDesc), m_pDisplayPet(NULL), m_bNeedUpdate(TRUE), 
	  m_pCDevice(pDevice)
{
	m_pDxDevice		= m_pCDevice->m_pDevice;

	m_pImage3D		= FindKid(ID_CTRLINST_IMAGE3D);
	m_pKindNameTxt	= FindKid(ID_CTRLINST_TXT_KIND);
	m_pKindNameTxt->m_strText.Empty();

	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));

	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	light.Diffuse.a = 1.0f;

	light.Ambient.r = 0.0f;
	light.Ambient.g = 0.0f;
	light.Ambient.b = 0.0f;
	light.Ambient.a = 1.0f;

	light.Direction = D3DXVECTOR3( -0.1f, -0.1f, 1.0f);
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Range = T3DLIGHT_RANGE_MAX;

	m_vLights[TLIGHT_CENTER].SetLight(&light);
	m_vLights[TLIGHT_CENTER].EnableLight(FALSE);

	light.Direction = D3DXVECTOR3( -1.0f, -0.1f, -0.1f);
	light.Diffuse.r = 0.4f;
	light.Diffuse.g = 0.4f;
	light.Diffuse.b = 0.4f;
	light.Diffuse.a = 1.0f;

	m_vLights[TLIGHT_SIDE].SetLight(&light);
	m_vLights[TLIGHT_SIDE].EnableLight(FALSE);

	light.Direction = D3DXVECTOR3( 0.1f, 0.1f, -1.0f);
	light.Diffuse.r = 0.4f;
	light.Diffuse.g = 0.4f;
	light.Diffuse.b = 0.4f;
	light.Diffuse.a = 1.0f;

	m_vLights[TLIGHT_BACK].SetLight(&light);
	m_vLights[TLIGHT_BACK].EnableLight(FALSE);
}
// ----------------------------------------------------------------------
CTPetDlg::~CTPetDlg()
{
	if( m_pDisplayPet )
		delete m_pDisplayPet;
}
// ======================================================================

// ======================================================================
CString CTPetDlg::KindToString(BYTE bKind, WORD wKindValue)
{
	CString strResult;
	
	switch( bKind )
	{
	case PETKIND_ONE:	strResult = CTChart::Format( TSTR_PETKIND_ONE, wKindValue); break;
	case PETKIND_TWO:	strResult = CTChart::Format( TSTR_PETKIND_TWO, wKindValue); break;
	case PETKIND_TELE:	strResult = CTChart::Format( TSTR_PETKIND_TELE, wKindValue); break;
	case PETKIND_INVEN:	strResult = CTChart::Format( TSTR_PETKIND_INVEN, wKindValue); break;
	case PETKIND_EXP:	strResult = CTChart::Format( TSTR_PETKIND_EXP, wKindValue); break;
	}

	return strResult;
}
// ======================================================================
CString CTPetDlg::KindToStringContract(BYTE bKind, WORD wKindValue)
{
	CString strResult;
	
	switch( bKind )
	{
	case PETKIND_ONE:	
		strResult = CTChart::Format( TSTR_PETKIND_ONE_CONTRACT, wKindValue+100); break;

	case PETKIND_TWO:
		strResult = CTChart::Format( TSTR_PETKIND_TWO_CONTRACT, wKindValue); break;

	case PETKIND_TELE:
		strResult = CTChart::Format( TSTR_PETKIND_TELE_CONTRACT, wKindValue); break;

	case PETKIND_INVEN:
		strResult = CTChart::Format( TSTR_PETKIND_INVEN_CONTRACT, wKindValue); break;

	case PETKIND_EXP:
		strResult = CTChart::Format( TSTR_PETKIND_EXP_CONTRACT, wKindValue); break;
	}

	return strResult;
}
// ----------------------------------------------------------------------
CString CTPetDlg::CondToString(BYTE bCond, DWORD dwCondValue)
{
	CString strResult;
	
	switch(bCond)
	{
	case PETCON_NONE:	strResult = CTChart::LoadString( TSTR_PETCON_NO); break;
	case PETCON_LEVEL:	strResult = CTChart::Format( TSTR_PETCON_LEVEL, dwCondValue); break;
	}

	return strResult;
}
// ======================================================================
CString CTPetDlg::CondToStringContract(BYTE bCond, DWORD dwCondValue)
{
	CString strResult;
	
	switch(bCond)
	{
	case PETCON_LEVEL:	strResult = CTChart::Format( TSTR_PETCON_LEVEL_CONTRACT, dwCondValue); break;
	}

	return strResult;
}

// ======================================================================

// ======================================================================
void CTPetDlg::SetDisplayPet(const CString& strName, WORD wPetID, const CTime& tEndTime)
{
	if( m_pDisplayPet )
	{
		delete m_pDisplayPet;
		m_pDisplayPet = NULL;
	}

	m_pDisplayPet = NewDisplayPet(strName,wPetID,tEndTime);
	NotifyUpdate();
}
// ======================================================================
void CTPetDlg::NotifyUpdate()
{
	m_bNeedUpdate = TRUE;
}
// ----------------------------------------------------------------------
void CTPetDlg::Update(DWORD dwTickCount)
{
	if( !m_pDisplayPet )
		return;

	LPTPET pPetTemp = m_pDisplayPet->GetPetTemp();
	if( !pPetTemp )
		return;

	const CString& strPetName = m_pDisplayPet->GetPetName();
	if( strPetName.IsEmpty() )
		m_pKindNameTxt->m_strText = m_pDisplayPet->GetPetKindName();
	else
		m_pKindNameTxt->m_strText = CTChart::Format( TSTR_PETMAKE_KINDNAME, m_pDisplayPet->GetPetKindName(), strPetName);

	m_bNeedUpdate = FALSE;
}
// ======================================================================
HRESULT CTPetDlg::Render(DWORD dwTickCount)
{
	if( !IsVisible() )
		return S_OK;

	if( m_bNeedUpdate )
		Update(dwTickCount);

	HRESULT res = CTClientUIBase::Render(dwTickCount);
	
	if( !m_pDisplayPet )
		return res;

	CRect rect;
	m_pImage3D->GetComponentRect(&rect);
	m_pImage3D->ComponentToScreen(&rect);

	D3DVIEWPORT9 vOldVPort, vNewVPort;
	m_pDxDevice->GetViewport(&vOldVPort);

	vNewVPort.Height = rect.Height() + min( 0, rect.top);
	vNewVPort.Width = rect.Width() + min( 0, rect.left);
	vNewVPort.MinZ = 0.0f;
	vNewVPort.MaxZ = 1.0f;
	vNewVPort.X = max(0, rect.left);
	vNewVPort.Y = max(0, rect.top);
	m_pDxDevice->SetViewport(&vNewVPort);

	FLOAT fHeight = m_pDisplayPet->m_fSizeY * FLOAT(vNewVPort.Height) / FLOAT(rect.Height()) * 1.1f;
	FLOAT fWidth = m_pDisplayPet->m_fSizeY * FLOAT(vNewVPort.Width) / FLOAT(rect.Height()) * 1.1f;

	m_vCamera.InitOrthoCamera( m_pDxDevice, 
		-TPETDLG_CAM_DIST, TPETDLG_CAM_DIST, fWidth,fHeight);

	fWidth = (m_pDisplayPet->m_fSizeY * FLOAT(rect.Width()) / FLOAT(rect.Height()) - fWidth) / 2.0f;
	fHeight /= 2.0f;

	m_vCamera.SetPosition(
		D3DXVECTOR3( fWidth+1.0f, fHeight,  0.0f),
		D3DXVECTOR3( fWidth, fHeight,  0.0f),
		D3DXVECTOR3(   0.0f,	1.0f,  0.0f),
		FALSE);

	m_vCamera.Rotate( 0.0f, -D3DX_PI / 18.0f, 0.0f);
	m_vCamera.Move(
		fWidth - m_vCamera.m_vTarget.x,
		fHeight - m_vCamera.m_vTarget.y,
		-m_vCamera.m_vTarget.z, TRUE);
	m_vCamera.Activate(TRUE);

	m_pDxDevice->Clear(
		0, NULL,
		D3DCLEAR_ZBUFFER,
		0, 1.0f, 0);
	
	EnableTLIGHT( &m_vCamera, TRUE);

	m_pDisplayPet->CTachyonObject::CalcTick(m_pDxDevice,dwTickCount);

	CTachyonMesh::BeginGlobalDraw(m_pDxDevice);
	m_pDisplayPet->Render(m_pCDevice,&m_vCamera);
	CTachyonMesh::EndGlobalDraw(m_pDxDevice);

	m_pDxDevice->SetViewport(&vOldVPort);
	
	EnableTLIGHT( &m_vCamera, FALSE);

	return res;
}
// ======================================================================

// ======================================================================
void CTPetDlg::EnableTLIGHT(CD3DCamera* pCamera, BYTE bENABLE)
{
	for(int i=0; i<TLIGHT_COUNT; ++i)
		m_vLights[i].EnableLight(bENABLE);

	if(!m_pCDevice->m_bEnableSHADER)
		return;

	if(bENABLE)
	{
		static int vLightCount[4] = { TLIGHT_COUNT, 0, 1, 0};
		FLOAT vCONST[16];

		m_pDxDevice->SetVertexShaderConstantI(
			m_pCDevice->m_vConstantVS[VC_LIGHTCOUNT],
			vLightCount, 1);

		m_pDxDevice->SetVertexShaderConstantF(
			m_pCDevice->m_vConstantVS[VC_CAMPOS],
			(FLOAT *) &D3DXVECTOR4(
			pCamera->m_vPosition.x,
			pCamera->m_vPosition.y,
			pCamera->m_vPosition.z,
			0.0f), 1);

		D3DXMatrixTranspose( (LPD3DXMATRIX) vCONST, &(pCamera->m_matView * pCamera->m_matProjection));
		m_pDxDevice->SetVertexShaderConstantF(
			m_pCDevice->m_vConstantVS[VC_PROJ],
			vCONST, 4);

		for( i=0; i<TLIGHT_COUNT; ++i)
		{
			memcpy( &vCONST[i * 4], &D3DXVECTOR4(
				m_vLights[i].m_Light.Ambient.r,
				m_vLights[i].m_Light.Ambient.g,
				m_vLights[i].m_Light.Ambient.b,
				m_vLights[i].m_Light.Ambient.a),
				4 * sizeof(FLOAT));
		}

		m_pDxDevice->SetVertexShaderConstantF(
			m_pCDevice->m_vConstantVS[VC_LIGHTAMBIENT],
			vCONST, TLIGHT_COUNT);

		for( i=0; i<TLIGHT_COUNT; ++i)
		{
			memcpy( &vCONST[i * 4], &D3DXVECTOR4(
				m_vLights[i].m_Light.Diffuse.r,
				m_vLights[i].m_Light.Diffuse.g,
				m_vLights[i].m_Light.Diffuse.b,
				m_vLights[i].m_Light.Diffuse.a),
				4 * sizeof(FLOAT));
		}

		m_pDxDevice->SetVertexShaderConstantF(
			m_pCDevice->m_vConstantVS[VC_LIGHTDIFFUSE],
			vCONST, TLIGHT_COUNT);

		for( i=0; i<TLIGHT_COUNT; ++i)
		{
			memcpy( &vCONST[i * 4], &D3DXVECTOR4(
				m_vLights[i].m_Light.Direction.x,
				m_vLights[i].m_Light.Direction.y,
				m_vLights[i].m_Light.Direction.z,
				1.0f),
				4 * sizeof(FLOAT));
		}

		m_pDxDevice->SetVertexShaderConstantF(
			m_pCDevice->m_vConstantVS[VC_LIGHTDIR],
			vCONST, TLIGHT_COUNT);
	}
	else
	{
		static const int vLightCount[4] = {0, 0, 1, 0};

		m_pDxDevice->SetVertexShaderConstantI(
			m_pCDevice->m_vConstantVS[VC_LIGHTCOUNT],
			vLightCount, 1);
	}
}
// ======================================================================
CTClientPet* CTPetDlg::NewDisplayPet(const CString& strName, WORD wPetID, const CTime& tEndTime)
{
	CTClientPet* pPet = new CTClientPet;
	
	pPet->SetPetInfo(strName, wPetID, tEndTime);
	LPTMONTEMP pMonTemp = pPet->m_pTEMP;

	D3DXMATRIX matINIT;
	D3DXMatrixIdentity(&matINIT);

	pPet->SetPosition(matINIT);
	pPet->m_vWorld = matINIT;

	CTClientInven* pTEquip = new CTClientInven();
	pPet->ClearInven();

	pTEquip->m_bInvenID = INVEN_EQUIP;
	pTEquip->m_wItemID = 0;
	pPet->m_mapTINVEN.insert( MAPTINVEN::value_type(pTEquip->m_bInvenID, pTEquip));

	if( pMonTemp && pMonTemp->m_dwOBJ )
	{
		CTachyonRes* pRes = CTClientGame::GetInstance()->GetResource();
		LPOBJECT pObj = pRes->GetOBJ( pMonTemp->m_dwOBJ );
	
		pPet->InitOBJ(pObj);
	
		for( BYTE i=0; i<ES_COUNT; ++i )
		{
			if(pMonTemp->m_pTITEM[i])
			{
				CTClientItem* pTItem = new CTClientItem();

				pTItem->SetItemID( pMonTemp->m_pTITEM[i]->m_wItemID );
				pTItem->SetItemSlot(i);
				pTItem->SetCount(1);

				pTEquip->m_mapTITEM.insert( make_pair(pTItem->GetItemSlot(), pTItem) );
			}
		}
		
		pPet->ResetEQUIP(m_pCDevice, pRes);

		pPet->m_fSizeY = pPet->GetAttrFLOAT(ID_SIZE_Y);

		TACTION vActionID = pPet->FindActionID(TA_STAND,WT_NORMAL);
		pPet->SetAction(vActionID.m_dwActID, vActionID.m_dwAniID);
		((CTachyonObject *) pPet)->CalcTick( m_pCDevice->m_pDevice, 0);
	}

	return pPet;
}
// ======================================================================























