#include "StdAfx.h"


DWORD CTachyonSFX::m_dwParticleNum = 0;
DWORD CTachyonSFX::m_dwNextID = 0;

LPMESHVB CSFX::m_pTEXVB = NULL;
MAPTSFX CTachyonSFX::m_mapINST;

BYTE CTachyonSFX::m_bOptionLodLevel = LODLEVEL_1;
FLOAT CTachyonSFX::m_fDistMAX = 0.0f;


void CSFX::InitTEXVB()
{
	PVERTEX vPoint[4] = {
		{ -1.0f / 2.0f, -1.0f / 2.0f, 0.0f, 0.0f, 1.0f},
		{ -1.0f / 2.0f,  1.0f / 2.0f, 0.0f, 0.0f, 0.0f},
		{  1.0f / 2.0f, -1.0f / 2.0f, 0.0f, 1.0f, 1.0f},
		{  1.0f / 2.0f,  1.0f / 2.0f, 0.0f, 1.0f, 0.0f}};

	CSFX::m_pTEXVB = new MESHVB();
	CSFX::m_pTEXVB->m_dwCount = 2;

	CSFX::m_pTEXVB->m_vVB.LoadT3DVB(
		4 * sizeof(PVERTEX),
		0, T3DFVF_PVERTEX);

	CSFX::m_pTEXVB->m_vVB.LoadT3DVBDATA(
		(LPBYTE) vPoint,
		4 * sizeof(PVERTEX), 0);
}

void CSFX::ReleaseTEXVB()
{
	if(CSFX::m_pTEXVB)
		delete CSFX::m_pTEXVB;

	CSFX::m_pTEXVB = NULL;
}


//////////////////////////////////////////////////////////////////
// CSFX Function...
//////////////////////////////////////////////////////////////////

CSFX::CSFX()
{
	m_vPFactor = D3DXVECTOR3( 1.0f, 1.0f, 1.0f);
	m_vLocalP = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_vCurP = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_vCurR = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_vCurS = D3DXVECTOR3( 1.0f, 1.0f, 1.0f);

	m_vSCALE = D3DXVECTOR3( 1.0f, 1.0f, 1.0f);
	m_vROT = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_vPOS = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);

	m_vERRScale = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_vERRPos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_vERRRot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&m_vGlobal);

	m_SRC.m_pSFX = NULL; //m_SRC.m_pVB = NULL;
	m_pSFX = NULL;
	m_fTime = 0.0f;

	m_dwERRTime = 0;
	m_dwTexTick = 0;
	m_dwLife = 0;

	m_SRC_VB_X = 1.0f;
	m_SRC_VB_Y = 1.0f;
}

CSFX::~CSFX()
{
	Release();
}

void CSFX::Release()
{
	if( m_pSFX && m_SRC.m_pSFX ) //if( m_pSFX && m_SRC.m_pVB )
	{
		switch(m_pSFX->m_bSRCType)
		{
		case SFXSRC_OBJ	: delete m_SRC.m_pOBJ; break;
		case SFXSRC_SFX	: delete m_SRC.m_pSFX; break;
		//case SFXSRC_TEX	: delete m_SRC.m_pVB; break;
		}

		m_SRC.m_pSFX = NULL; //m_SRC.m_pVB = NULL;
	}

	m_vCurP = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_vCurR = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_vCurS = D3DXVECTOR3( 1.0f, 1.0f, 1.0f);

	m_vSCALE = D3DXVECTOR3( 1.0f, 1.0f, 1.0f);
	m_vROT = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_vPOS = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);

	m_dwTexTick = 0;
	m_pSFX = NULL;
	m_fTime = 0.0f;
}

BOOL CSFX::InitSFX( CD3DDevice *pDevice, LPSFX pSFX)
{
	Release();
	m_pSFX = pSFX;

	switch(pSFX->m_bSRCType)
	{
	case SFXSRC_OBJ	:
		{
			MAPCLKINST::iterator it;

			m_SRC.m_pOBJ = new CTachyonObject();
			m_SRC.m_pOBJ->InitOBJ(pSFX->m_SRC.m_pOBJ);

			for( it = pSFX->m_mapCLK.begin(); it != pSFX->m_mapCLK.end(); it++)
			{
				LPCLKINST pCLK = (*it).second;
				if(pCLK)
					m_SRC.m_pOBJ->SetCloth( pDevice, (*it).first, pCLK->m_dwCurCL, pCLK->m_dwCurMESH);
			}
			m_SRC.m_pOBJ->ResetOBJPart(pDevice);

			m_SRC.m_pOBJ->SetAction( pSFX->m_dwACT, pSFX->m_dwANI);
			m_SRC.m_pOBJ->m_dwOBJSNDFuncID = pSFX->m_dwOBJFuncID;
			m_SRC.m_pOBJ->m_dwACTSNDFuncID = pSFX->m_dwACTFuncID;
			m_SRC.m_pOBJ->m_dwANISNDFuncID = pSFX->m_dwANIFuncID;

			m_SRC.m_pOBJ->m_dwOBJSFXFuncID = 0;
			m_SRC.m_pOBJ->m_dwACTSFXFuncID = 0;
			m_SRC.m_pOBJ->m_dwANISFXFuncID = 0;
		}

		break;

	case SFXSRC_SFX	:
		{
			m_SRC.m_pSFX = new CTachyonSFX();

			m_SRC.m_pSFX->m_vPFactor = m_vPFactor;
			m_SRC.m_pSFX->m_vGlobal = m_vGlobal;

			m_SRC.m_pSFX->InitSFX(
				pDevice,
				pSFX->m_SRC.m_pSFX);
		}

		break;

	case SFXSRC_TEX	:
		{
			m_SRC_VB_X = pSFX->m_fSizeX;
			m_SRC_VB_Y = pSFX->m_fSizeY;

			/*FLOAT vPoint[4][4] = {
				{ - pSFX->m_fSizeX / 2.0f, - pSFX->m_fSizeY / 2.0f, 0.0f, 1.0f},
				{ - pSFX->m_fSizeX / 2.0f,	 pSFX->m_fSizeY / 2.0f, 0.0f, 0.0f},
				{	pSFX->m_fSizeX / 2.0f, - pSFX->m_fSizeY / 2.0f, 1.0f, 1.0f},
				{	pSFX->m_fSizeX / 2.0f,	 pSFX->m_fSizeY / 2.0f, 1.0f, 0.0f}};

			m_SRC.m_pVB = new MESHVB();
			m_SRC.m_pVB->m_dwCount = 2;

			if( FAILED(pDevice->m_pDevice->CreateVertexBuffer(
					4 * sizeof(PVERTEX),
					0, T3DFVF_PVERTEX,
					D3DPOOL_MANAGED,
					&m_SRC.m_pVB->m_pVB, NULL)) )
			{
				Release();
				return FALSE;
			}

			LPPVERTEX pBUF = NULL;
			m_SRC.m_pVB->m_pVB->Lock( 0, 4 * sizeof(PVERTEX), (LPVOID *) &pBUF, 0);

			for( int i=0; i<4; i++)
			{
				pBUF[i].m_fPosX = vPoint[i][0];
				pBUF[i].m_fPosY = vPoint[i][1];
				pBUF[i].m_fPosZ = 0.0f;
				pBUF[i].m_fU = vPoint[i][2];
				pBUF[i].m_fV = vPoint[i][3];
			}
			m_SRC.m_pVB->m_pVB->Unlock();*/
		}

		break;
	}

	return TRUE;
}

BOOL CSFX::IsDead()
{
	if( m_pSFX && m_pSFX->m_bSRCType == SFXSRC_SFX )
	{
		if( m_dwLife == 0 )
			m_SRC.m_pSFX->m_bDeleted = TRUE;

		return m_dwLife == 0 && m_SRC.m_pSFX->IsDead();
	}

	return m_dwLife == 0;
}

void CSFX::CalcTick( LPDIRECT3DDEVICE9 pDevice, DWORD dwTick)
{
	m_fTime += CTMath::GetTimeValue(dwTick + m_dwERRTime);
	m_dwTexTick += dwTick;

	if( m_dwLife > dwTick )
		m_dwLife -= dwTick;
	else
		m_dwLife = 0;

	if(m_pSFX)
	{
		switch(m_pSFX->m_bSRCType)
		{
		case SFXSRC_SFX	: m_SRC.m_pSFX->CalcTick( pDevice, dwTick); break;
		case SFXSRC_OBJ	: m_SRC.m_pOBJ->CalcTick( pDevice, dwTick); break;
		}

		if(m_pSFX->m_pANISRC)
		{
			FLOAT fTotal = m_pSFX->m_pANISRC->GetTotalTime();
			FLOAT fTime = m_fTime;

			while( fTime >= fTotal )
			{
				m_fTime = fTotal;
				CalcFrame(TRUE);

				fTime -= fTotal;
			}

			m_fTime = fTime;
			CalcFrame(FALSE);
		}
	}
}

void CSFX::CalcFrame( BOOL bUpdate)
{
	if( m_pSFX && m_pSFX->m_pANISRC )
	{
		FLOAT fRate = m_fTime / m_pSFX->m_pANISRC->GetTotalTime();

		m_pSFX->m_pANISRC->m_fCurTime = m_fTime;
		ANIKEY vKey = m_pSFX->m_pANISRC->GetAniKey();

		m_vCurP = fRate * m_vERRPos + D3DXVECTOR3(
			vKey.m_fPosX + m_vPOS.x,
			vKey.m_fPosY + m_vPOS.y,
			vKey.m_fPosZ + m_vPOS.z);

		m_vCurR = fRate * m_vERRRot + D3DXVECTOR3(
			vKey.m_fRotX + m_vROT.x,
			vKey.m_fRotY + m_vROT.y,
			vKey.m_fRotZ + m_vROT.z);

		m_vCurS = fRate * m_vERRScale + D3DXVECTOR3(
			vKey.m_fScaleX * m_vSCALE.x,
			vKey.m_fScaleY * m_vSCALE.y,
			vKey.m_fScaleZ * m_vSCALE.z);

		if(bUpdate)
		{
			m_vSCALE = m_vCurS;
			m_vROT = m_vCurR;
			m_vPOS = m_vCurP;
		}
	}
}

void CSFX::ApplyTexture( CD3DDevice *pDevice)
{
	static D3DXMATRIX vUV;

	if(m_pSFX->m_SRC.m_pTEX)
	{
		m_dwTexTick %= m_pSFX->m_SRC.m_pTEX->m_dwTotalTick;
		m_pSFX->m_SRC.m_pTEX->m_dwCurTick = m_dwTexTick;

		UVKEY vKey = m_pSFX->m_SRC.m_pTEX->GetUV();
		CTMath::GetUVMatrix( &vUV, &vKey);

		pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MIPMAPLODBIAS, *((LPDWORD) &m_pSFX->m_SRC.m_pTEX->m_fMipBias));
		pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, m_pSFX->m_SRC.m_pTEX->m_dwMipFilter);
		pDevice->m_pDevice->SetTexture( 0, m_pSFX->m_SRC.m_pTEX->GetTexture());
		pDevice->m_pDevice->SetTransform( D3DTS_TEXTURE0, &vUV);
	}
	else
	{
		D3DXMatrixIdentity(&vUV);

		pDevice->m_pDevice->SetTransform( D3DTS_TEXTURE0, &vUV);
		pDevice->m_pDevice->SetTexture( 0, NULL);
	}
}

void CSFX::RenderSINGLE( CD3DDevice *pDevice, CD3DCamera *pCamera, BYTE bMaxAlpha)
{
	if(m_pSFX)
	{
		DWORD dwCLR = m_pSFX->GetColor(CTMath::GetTimeTick(m_fTime));

		D3DXVECTOR3 vPosition = m_vCurP;
		D3DXMATRIX vScale;
		D3DXMATRIX vRotX;
		D3DXMATRIX vRotY;
		D3DXMATRIX vRotZ;

		if( m_vCurS.x <= 0.01f ||
			m_vCurS.y <= 0.01f ||
			m_vCurS.z <= 0.01f )
			return;

		D3DXMatrixScaling(
			&vScale,
			m_vCurS.x,
			m_vCurS.y,
			m_vCurS.z);

		vPosition.x *= m_vPFactor.x;
		vPosition.y *= m_vPFactor.y;
		vPosition.z *= m_vPFactor.z;

		switch(m_pSFX->m_bSRCType)
		{
		case SFXSRC_OBJ	:
			{
				D3DXMATRIX vBACK = m_SRC.m_pOBJ->m_vPosition;
				D3DXMATRIX vCP;

				FLOAT fValueA = ((FLOAT) GETVALUE_A(dwCLR)) / 255.0f;
				FLOAT fValueR = ((FLOAT) GETVALUE_R(dwCLR)) / 255.0f;
				FLOAT fValueG = ((FLOAT) GETVALUE_G(dwCLR)) / 255.0f;
				FLOAT fValueB = ((FLOAT) GETVALUE_B(dwCLR)) / 255.0f;

				D3DMATERIAL9 vOLD;
				D3DMATERIAL9 vNEW;

				ZeroMemory( &vOLD, sizeof(D3DMATERIAL9));
				ZeroMemory( &vNEW, sizeof(D3DMATERIAL9));

				pDevice->m_pDevice->GetMaterial(&vOLD);
				vNEW.Diffuse.a = vNEW.Ambient.a = min( fValueA, bMaxAlpha / 255.0f );
				vNEW.Diffuse.r = vNEW.Ambient.r = fValueR;
				vNEW.Diffuse.g = vNEW.Ambient.g = fValueG;
				vNEW.Diffuse.b = vNEW.Ambient.b = fValueB;
				pDevice->m_pDevice->SetMaterial(&vNEW);

				D3DXMatrixTranslation(
					&vCP,
					vPosition.x,
					vPosition.y,
					vPosition.z);

				D3DXMatrixRotationX(
					&vRotX,
					m_vCurR.x);

				D3DXMatrixRotationY(
					&vRotY,
					m_vCurR.y);

				D3DXMatrixRotationZ(
					&vRotZ,
					m_vCurR.z);

				m_SRC.m_pOBJ->m_vPosition = vScale * vRotX * vRotY * vRotZ * vCP * m_vGlobal * m_SRC.m_pOBJ->m_vPosition;
				m_SRC.m_pOBJ->m_bAlpha = GETVALUE_A(dwCLR);

				CTachyonMesh::BeginGlobalDraw(pDevice->m_pDevice);

				m_SRC.m_pOBJ->Render(
					pDevice,
					pCamera);

				CTachyonMesh::EndGlobalDraw(pDevice->m_pDevice);

				m_SRC.m_pOBJ->m_vPosition = vBACK;
				pDevice->m_pDevice->SetMaterial(&vOLD);
			}

			break;

		case SFXSRC_SFX	:
			{
				D3DXMatrixRotationX(
					&vRotX,
					m_vCurR.x);

				D3DXMatrixRotationY(
					&vRotY,
					m_vCurR.y);

				D3DXMatrixRotationZ(
					&vRotZ,
					m_vCurR.z);

				m_SRC.m_pSFX->m_vGlobal = vRotX * vRotY * vRotZ;
				m_SRC.m_pSFX->m_vGlobal._41 += vPosition.x;
				m_SRC.m_pSFX->m_vGlobal._42 += vPosition.y;
				m_SRC.m_pSFX->m_vGlobal._43 += vPosition.z;
				m_SRC.m_pSFX->m_vGlobal *= m_vGlobal;
				m_SRC.m_pSFX->m_vPFactor = m_vPFactor;
				m_SRC.m_pSFX->Render( pDevice, pCamera, bMaxAlpha );
			}

			break;

		case SFXSRC_TEX	:
			{
				D3DXMATRIX vSize;
				D3DXMatrixScaling( &vSize, m_SRC_VB_X, m_SRC_VB_Y, 1.0f );

				D3DXMATRIX vSize2;
				D3DXVECTOR3 vX( m_vGlobal._11, m_vGlobal._21, m_vGlobal._31 );
				D3DXVECTOR3 vY( m_vGlobal._12, m_vGlobal._22, m_vGlobal._32 );
				D3DXVECTOR3 vZ( m_vGlobal._13, m_vGlobal._23, m_vGlobal._33 );

				D3DXMatrixScaling( &vSize2,
					D3DXVec3Length( &vX ),
					D3DXVec3Length( &vY ),
					D3DXVec3Length( &vZ ) );

				CTMath::Transform(
					&m_vGlobal,
					&vPosition);

				D3DXMATRIX vBill = vSize * vScale * vSize2 * GetBillMatrix(
					pCamera,
					&vPosition);

				vBill._41 = vPosition.x;
				vBill._42 = vPosition.y;
				vBill._43 = vPosition.z;

				BYTE bAlpha = min( (BYTE) (dwCLR>>24), bMaxAlpha);
				dwCLR &= 0x00FFFFFF;
				dwCLR |= bAlpha << 24;

				if(CSFX::m_pTEXVB)
				{
					pDevice->m_pDevice->SetStreamSource( 0, CSFX::m_pTEXVB->m_vVB.GetVB(), 0, sizeof(PVERTEX));
					pDevice->m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, dwCLR);
					pDevice->m_pDevice->SetTransform( D3DTS_WORLD, &vBill);

					pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
					pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);

					ApplyTexture(pDevice);
					pDevice->m_pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, CSFX::m_pTEXVB->m_dwCount);

					pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
					pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
				}
			}

			break;
		}
	}
}

void CSFX::Render( CD3DDevice *pDevice, CD3DCamera *pCamera, BYTE bMaxAlpha)
{
	if(m_pSFX)
	{
		DWORD dwCLR = m_pSFX->GetColor(CTMath::GetTimeTick(m_fTime));

		D3DXVECTOR3 vLocalP = m_vLocalP;
		D3DXMATRIX vRotX;
		D3DXMATRIX vRotY;
		D3DXMATRIX vRotZ;

		if( m_vCurS.x <= 0.01f ||
			m_vCurS.y <= 0.01f ||
			m_vCurS.z <= 0.01f )
			return;

		vLocalP.x *= m_vCurS.x;
		vLocalP.y *= m_vCurS.y;
		vLocalP.z *= m_vCurS.z;

		D3DXMatrixRotationX(
			&vRotX,
			m_vCurR.x);

		D3DXMatrixRotationY(
			&vRotY,
			m_vCurR.y);

		D3DXMatrixRotationZ(
			&vRotZ,
			m_vCurR.z);

		switch(m_pSFX->m_bSRCType)
		{
		case SFXSRC_OBJ	:
			{
				D3DXMATRIX vBACK = m_SRC.m_pOBJ->m_vPosition;
				D3DXMATRIX vLP;
				D3DXMATRIX vCP;

				FLOAT fValueA = ((FLOAT) GETVALUE_A(dwCLR)) / 255.0f;
				FLOAT fValueR = ((FLOAT) GETVALUE_R(dwCLR)) / 255.0f;
				FLOAT fValueG = ((FLOAT) GETVALUE_G(dwCLR)) / 255.0f;
				FLOAT fValueB = ((FLOAT) GETVALUE_B(dwCLR)) / 255.0f;

				D3DMATERIAL9 vOLD;
				D3DMATERIAL9 vNEW;

				ZeroMemory( &vOLD, sizeof(D3DMATERIAL9));
				ZeroMemory( &vNEW, sizeof(D3DMATERIAL9));

				pDevice->m_pDevice->GetMaterial(&vOLD);

				vNEW.Diffuse.a = vNEW.Ambient.a = min( fValueA, bMaxAlpha / 255.0f );
				vNEW.Diffuse.r = vNEW.Ambient.r = fValueR;
				vNEW.Diffuse.g = vNEW.Ambient.g = fValueG;
				vNEW.Diffuse.b = vNEW.Ambient.b = fValueB;
				pDevice->m_pDevice->SetMaterial(&vNEW);

				D3DXMatrixTranslation(
					&vLP,
					vLocalP.x,
					vLocalP.y,
					vLocalP.z);

				D3DXMatrixTranslation(
					&vCP,
					m_vCurP.x * m_vPFactor.x,
					m_vCurP.y * m_vPFactor.y,
					m_vCurP.z * m_vPFactor.z);

				m_SRC.m_pOBJ->m_vPosition = vLP * vRotX * vRotY * vRotZ * vCP * m_vGlobal * m_SRC.m_pOBJ->m_vPosition;
				m_SRC.m_pOBJ->m_bAlpha = GETVALUE_A(dwCLR);

				CTachyonMesh::BeginGlobalDraw(pDevice->m_pDevice);
				m_SRC.m_pOBJ->Render(
					pDevice,
					pCamera);
				CTachyonMesh::EndGlobalDraw(pDevice->m_pDevice);

				m_SRC.m_pOBJ->m_vPosition = vBACK;
				pDevice->m_pDevice->SetMaterial(&vOLD);
			}

			break;

		case SFXSRC_SFX	:
			{
				CTMath::Transform(
					&(vRotX * vRotY * vRotZ),
					&vLocalP);

				m_SRC.m_pSFX->m_vGlobal = vRotX * vRotY * vRotZ;
				m_SRC.m_pSFX->m_vGlobal._41 += vLocalP.x + m_vCurP.x * m_vPFactor.x;
				m_SRC.m_pSFX->m_vGlobal._42 += vLocalP.y + m_vCurP.y * m_vPFactor.y;
				m_SRC.m_pSFX->m_vGlobal._43 += vLocalP.z + m_vCurP.z * m_vPFactor.z;
				m_SRC.m_pSFX->m_vGlobal *= m_vGlobal;
				m_SRC.m_pSFX->m_vPFactor = m_vPFactor;

				m_SRC.m_pSFX->Render( pDevice, pCamera, bMaxAlpha );
			}

			break;

		case SFXSRC_TEX	:
			{
				D3DXMATRIX vSize;
				D3DXMatrixScaling( &vSize, m_SRC_VB_X, m_SRC_VB_Y, 1.0f);

				CTMath::Transform(
					&(vRotX * vRotY * vRotZ),
					&vLocalP);

				vLocalP.x += m_vCurP.x * m_vPFactor.x;
				vLocalP.y += m_vCurP.y * m_vPFactor.y;
				vLocalP.z += m_vCurP.z * m_vPFactor.z;

				CTMath::Transform(
					&m_vGlobal,
					&vLocalP);

				D3DXMATRIX vBill = vSize * GetBillMatrix(
					pCamera,
					&vLocalP);

				vBill._41 = vLocalP.x;
				vBill._42 = vLocalP.y;
				vBill._43 = vLocalP.z;

				BYTE bAlpha = min( (BYTE)(dwCLR>>24), bMaxAlpha);
				dwCLR &= 0x00FFFFFF;
				dwCLR |= bAlpha << 24;

				if(CSFX::m_pTEXVB)
				{
					pDevice->m_pDevice->SetStreamSource( 0, CSFX::m_pTEXVB->m_vVB.GetVB(), 0, sizeof(PVERTEX));
					pDevice->m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, dwCLR);
					pDevice->m_pDevice->SetTransform( D3DTS_WORLD, &vBill);
					ApplyTexture(pDevice);

					pDevice->m_pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, CSFX::m_pTEXVB->m_dwCount);
				}
			}

			break;
		}
	}
}

D3DXMATRIX CSFX::GetBillMatrix( CD3DCamera *pCamera, LPD3DXVECTOR3 pTarget)
{
	D3DXVECTOR3 vDir = pCamera->IsOrthoCamera() ? pCamera->m_vPosition - pCamera->m_vTarget : pCamera->m_vPosition - (*pTarget);
	D3DXMATRIX vRotY;
	vDir /= D3DXVec3Length(&vDir);

	FLOAT fHeight = vDir.y;
	FLOAT fTheta;

	if( fabs(vDir.z) > 0.001f )
		if( vDir.z > 0.0f )
			fTheta = D3DX_PI + atanf(vDir.x / vDir.z);
		else
			fTheta = atanf(vDir.x / vDir.z);
	else if( fabs(vDir.x) > 0.001f )
		if( vDir.x > 0.0f )
			fTheta = -D3DX_PI / 2.0f;
		else
			fTheta = D3DX_PI / 2.0f;
	else if( fabs(pCamera->m_vUp.z) > 0.001f )
		if( pCamera->m_vUp.z > 0.0f )
			fTheta = D3DX_PI + atanf(pCamera->m_vUp.x / pCamera->m_vUp.z);
		else
			fTheta = atanf(pCamera->m_vUp.x / pCamera->m_vUp.z);
	else if( pCamera->m_vUp.x > 0.0f )
		fTheta = -D3DX_PI / 2.0f;
	else
		fTheta = D3DX_PI / 2.0f;

	D3DXMatrixRotationY(
		&vRotY,
		fTheta);
	vDir.y = 0.0f;

	FLOAT fLength = D3DXVec3Length(&vDir);
	D3DXMATRIX vRotX;

	D3DXMatrixRotationX(
		&vRotX,
		atanf( fHeight / fLength));

	return vRotX * vRotY;
}


//////////////////////////////////////////////////////////////////
// CTachyonSFX Function...
//////////////////////////////////////////////////////////////////

CTachyonSFX::CTachyonSFX()
{
	m_bHasHost = FALSE;
	m_bDeleted = FALSE;

	m_pHost = NULL;
	m_pSFX = NULL;

	m_vGMOVE = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_vSCALE = D3DXVECTOR3( 1.0f, 1.0f, 1.0f);
	m_vROT = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_vPOS = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);

	m_vPFactor = D3DXVECTOR3( 1.0f, 1.0f, 1.0f);
	m_vCurP = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_vCurR = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_vCurS = D3DXVECTOR3( 1.0f, 1.0f, 1.0f);
	D3DXMatrixIdentity(&m_vGlobal);

	m_dwInitTick = INVALID_INIT_TICK;
	m_dwGTime = 0;
	m_dwLife = 0;
	m_fTime = 0.0f;

	m_SINGLE.Release();
	m_vPSFX.clear();
	m_vCSFX.clear();
	
	m_bMaxAlpha = 0xFF;

	m_dwGlobalLodLevel = (DWORD)m_bOptionLodLevel;
	m_bShow = TRUE;
}

CTachyonSFX::~CTachyonSFX()
{
	Release();
}

void CTachyonSFX::Release()
{
	if( m_pSFX && m_pSFX->m_pRES )
		m_pSFX->m_pRES->ReleaseSFX(m_pSFX);

	while(!m_vCSFX.empty())
	{
		delete m_vCSFX.back();
		m_vCSFX.pop_back();
	}

	while(!m_vPSFX.empty())
	{
		delete m_vPSFX.back();
		m_vPSFX.pop_back();
	}

	m_vGMOVE = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_vSCALE = D3DXVECTOR3( 1.0f, 1.0f, 1.0f);
	m_vROT = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_vPOS = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);

	m_vCurP = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_vCurR = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_vCurS = D3DXVECTOR3( 1.0f, 1.0f, 1.0f);

	m_SINGLE.Release();
	m_bDeleted = FALSE;
	m_pSFX = NULL;

	m_dwInitTick = INVALID_INIT_TICK;
	m_dwGTime = 0;
	m_dwLife = 0;
	m_fTime = 0.0f;
}

BOOL CTachyonSFX::InitSFX( CD3DDevice *pDevice, LPSFX pSFX)
{
	Release();
	m_pSFX = pSFX;

	//D3DXVECTOR3 vPOS(
	//	m_vGlobal._41,
	//	m_vGlobal._42,
	//	m_vGlobal._43);

	//FLOAT fCamDist = D3DXVec3Length(&(vPOS - pCamera->m_vTarget));

	//float fDiv = fDistMAX / LODLEVEL_6;
	//int nDistLevel = (int)(fCamDist / fDiv);
	//m_dwGlobalLodLevel = max( m_bOptionLodLevel, nDistLevel );
	//m_dwGlobalLodLevel = min( m_dwGlobalLodLevel, LODLEVEL_6 );

//	m_dwGlobalLodLevel = (DWORD)m_bOptionLodLevel;
	if( m_pSFX->m_dwLodLevel < m_dwGlobalLodLevel )
		return FALSE;

	if(pSFX)
	{
		int nCount = INT(pSFX->m_vSFX.size());

		if(pSFX->m_pRES)
			pSFX->m_pRES->LockSFX(pSFX);

		for( int i=0; i<nCount; i++)
		{
			if( pSFX->m_vSFX[i]->m_dwLodLevel >= m_dwGlobalLodLevel )
			{
				CTachyonSFX *pTSFX = new CTachyonSFX();

				pTSFX->m_vPFactor = m_vPFactor;
				pTSFX->m_vGlobal = m_vGlobal;

				pTSFX->InitSFX(
					pDevice,
					pSFX->m_vSFX[i]);

				m_vCSFX.push_back(pTSFX);
			}
		}
		m_dwLife = m_pSFX->m_dwLifeTime;

		if( m_pSFX->m_bSFXType == SFX_PARTICLE )
		{
			if( m_pSFX->m_bSRCType == SFXSRC_SFX &&
				m_pSFX->m_dwPLifeTime > 0 &&
				m_pSFX->m_SRC.m_pSFX->m_dwLifeTime > 0 )
				m_pSFX->m_dwPLifeTime = min( m_pSFX->m_dwPLifeTime, m_pSFX->m_SRC.m_pSFX->m_dwLifeTime);

			if(m_pSFX->m_bFollow)
			{
				m_vGMOVE = D3DXVECTOR3(
					m_vGlobal._41,
					m_vGlobal._42,
					m_vGlobal._43);
			}
		}

		switch(pSFX->m_bSFXType)
		{
		case SFX_PARTICLE	: Generate(pDevice, NULL); break;
		case SFX_SINGLE		:
			{
				m_SINGLE.m_vPFactor = m_vPFactor;
				m_SINGLE.m_vGlobal = m_vGlobal;
				m_SINGLE.InitSFX( pDevice, pSFX);
			}

			break;
		}
	}

	return TRUE;
}

BOOL CTachyonSFX::IsDead()
{
	int nCount = INT(m_vCSFX.size());

	for( int i=0; i<nCount; i++)
	{
		if(m_bDeleted)
			m_vCSFX[i]->m_bDeleted = TRUE;

		if(!m_vCSFX[i]->IsDead())
			return FALSE;
	}

	return !m_pSFX || (( m_bDeleted || m_pSFX->m_dwLifeTime != 0 ) && m_SINGLE.IsDead() && m_dwLife == 0 && m_vPSFX.empty());
}

void CTachyonSFX::ClearDead()
{
	VECTORTSFX::iterator itCSFX = m_vCSFX.begin();
	VECTORCSFX::iterator itPSFX = m_vPSFX.begin();

	while( itPSFX != m_vPSFX.end() )
		if((*itPSFX)->IsDead())
		{
			delete (*itPSFX);
			m_vPSFX.erase(itPSFX);
		}
		else
			itPSFX++;

	while( itCSFX != m_vCSFX.end() )
		if((*itCSFX)->IsDead())
		{
			delete (*itCSFX);
			m_vCSFX.erase(itCSFX);
		}
		else
			itCSFX++;
}

void CTachyonSFX::CalcTick( LPDIRECT3DDEVICE9 pDevice, DWORD dwTick)
{
	if( m_dwInitTick != INVALID_INIT_TICK )
	{
		dwTick = m_dwInitTick;
		m_dwInitTick = INVALID_INIT_TICK;
	}
	m_fTime += CTMath::GetTimeValue(dwTick);

	if( m_dwLife > dwTick )
		m_dwLife -= dwTick;
	else
		m_dwLife = 0;
	m_dwGTime += dwTick;

	if( m_pSFX && m_pSFX->m_pSprayANI )
	{
		FLOAT fTotal = m_pSFX->m_pSprayANI->GetTotalTime();
		FLOAT fTime = m_fTime;

		if( fTime >= fTotal )
		{
			while( fTime >= fTotal )
			{
				m_fTime = fTotal;
				CalcFrame(TRUE);

				fTime -= fTotal;
			}

			if( m_pSFX->m_dwLifeTime == 0 )
				m_dwLife = 0;
		}
		else if( m_pSFX->m_dwLifeTime == 0 )
			m_dwLife = CTMath::GetTimeTick(fTotal - fTime);

		m_fTime = fTime;
		CalcFrame(FALSE);
	}

	int nCount = INT(m_vCSFX.size());
	for( int i=0; i<nCount; i++)
	{
		m_vCSFX[i]->m_bDeleted = m_bDeleted;
		m_vCSFX[i]->CalcTick( pDevice, dwTick);
	}

	nCount = INT(m_vPSFX.size());
	for( i=0; i<nCount; i++)
		m_vPSFX[i]->CalcTick( pDevice, dwTick);
	ClearDead();

	if(m_pSFX)
	{
		if( m_pSFX->m_dwLifeTime != 0 && m_dwLife == 0 )
			m_bDeleted = TRUE;

		if( m_pSFX->m_bSFXType == SFX_SINGLE )
			m_SINGLE.CalcTick( pDevice, dwTick);
	}
}

void CTachyonSFX::CalcFrame( BOOL bUpdate)
{
	if( m_pSFX && m_pSFX->m_pSprayANI )
	{
		m_pSFX->m_pSprayANI->m_fCurTime = m_fTime;
		ANIKEY vKey = m_pSFX->m_pSprayANI->GetAniKey();

		m_vCurP = D3DXVECTOR3(
			vKey.m_fPosX + m_vPOS.x,
			vKey.m_fPosY + m_vPOS.y,
			vKey.m_fPosZ + m_vPOS.z);

		m_vCurR = D3DXVECTOR3(
			vKey.m_fRotX + m_vROT.x,
			vKey.m_fRotY + m_vROT.y,
			vKey.m_fRotZ + m_vROT.z);

		m_vCurS = D3DXVECTOR3(
			vKey.m_fScaleX * m_vSCALE.x,
			vKey.m_fScaleY * m_vSCALE.y,
			vKey.m_fScaleZ * m_vSCALE.z);

		if(bUpdate)
		{
			m_vSCALE = m_vCurS;
			m_vROT = m_vCurR;
			m_vPOS = m_vCurP;
		}
	}
}

void CTachyonSFX::Generate( CD3DDevice *pDevice, CD3DCamera *pCamera)
{
	D3DXMATRIX vGlobal;
	D3DXMATRIX vRotX;
	D3DXMATRIX vRotY;
	D3DXMATRIX vRotZ;

	D3DXMatrixRotationX(
		&vRotX,
		m_vCurR.x);

	D3DXMatrixRotationY(
		&vRotY,
		m_vCurR.y);

	D3DXMatrixRotationZ(
		&vRotZ,
		m_vCurR.z);

	vGlobal = vRotX * vRotY * vRotZ * m_vGlobal;
	vGlobal._41 += m_vCurP.x;
	vGlobal._42 += m_vCurP.y;
	vGlobal._43 += m_vCurP.z;


	//
	D3DXVECTOR3 vPOS(
		vGlobal._41,
		vGlobal._42,
		vGlobal._43);

	if( pCamera )
	{
		FLOAT fCamDist = D3DXVec3Length(&(vPOS - pCamera->m_vTarget));

		float fDiv = m_fDistMAX / LODLEVEL_6;
		int nDistLevel = (int)(fCamDist / fDiv);
		m_dwGlobalLodLevel = max( m_bOptionLodLevel, nDistLevel );
		m_dwGlobalLodLevel = min( m_dwGlobalLodLevel, LODLEVEL_6 );

	}
	if( m_pSFX->m_dwLodLevel < m_dwGlobalLodLevel )
		return ;

	int count = 0;
	if( m_pSFX->m_dwGCnt > 0 )
		count = max( 1, (int)((float)m_pSFX->m_dwGCnt / (float)m_dwGlobalLodLevel) );
//	m_dwParticleNum += count;
	for( int i=0; i < count; i++)
	{
		CSFX *pSFX = new CSFX();

		pSFX->m_dwLife = m_pSFX->m_dwPLifeTime;
		switch(m_pSFX->m_bSprayType)
		{
		case SPRAY_CYLINDER	:
			{
				int nBound = INT((m_pSFX->m_vOutBound.z - m_pSFX->m_vInBound.z) * 50.0f);
				int nValueZ = (rand() % 2 ? 1 : -1) * (nBound ? rand() % nBound : 0);

				if( nValueZ < 0 )
					nValueZ = nValueZ - INT(m_pSFX->m_vInBound.z * 50.0f);
				else if( nValueZ > 0 )
					nValueZ = nValueZ + INT(m_pSFX->m_vInBound.z * 50.0f);
				else
					nValueZ = (rand() % 2 ? 1 : -1) * (nValueZ + INT(m_pSFX->m_vInBound.z * 50.0f));

				nBound = INT((m_pSFX->m_vOutBound.x - m_pSFX->m_vInBound.x) * 50.0f);
				int nValueR = INT(m_pSFX->m_vInBound.x * 50.0f) + (nBound ? rand() % nBound : 0);

				FLOAT fValueT = ((FLOAT) (rand() % 360)) * D3DX_PI / 180.0f;
				pSFX->m_vLocalP.x = ((FLOAT) nValueR) * cosf(fValueT) / 100.0f;
				pSFX->m_vLocalP.y = ((FLOAT) nValueZ) / 100.0f;
				pSFX->m_vLocalP.z = ((FLOAT) nValueR) * sinf(fValueT) / 100.0f;
			}

			break;

		case SPRAY_SPHERE	:
			{
				int nBound = INT((m_pSFX->m_vOutBound.x - m_pSFX->m_vInBound.x) * 50.0f);
				int nValueR = INT(m_pSFX->m_vInBound.x * 50.0f) + (nBound ? rand() % nBound : 0);

				FLOAT fValueT1 = ((FLOAT) (rand() % 360)) * D3DX_PI / 180.0f;
				FLOAT fValueT2 = ((FLOAT) (rand() % 360)) * D3DX_PI / 180.0f;

				pSFX->m_vLocalP.x = ((FLOAT) nValueR) * cosf(fValueT1) * cosf(fValueT2) / 100.0f;
				pSFX->m_vLocalP.y = ((FLOAT) nValueR) * sinf(fValueT1) / 100.0f;
				pSFX->m_vLocalP.z = ((FLOAT) nValueR) * cosf(fValueT1) * sinf(fValueT2) / 100.0f;
			}

			break;

		case SPRAY_BOX		:
			{
				int nFace = rand() % 6;
				int nValueX;
				int nValueY;
				int nValueZ;

				switch(nFace)
				{
				case 0	:
					{
						int nBound = INT((m_pSFX->m_vOutBound.z - m_pSFX->m_vInBound.z) * 50.0f);
						nValueZ = nBound ? rand() % nBound : 0;
						nValueZ = -INT(m_pSFX->m_vInBound.z * 50.0f) - nValueZ;

						nBound = INT(m_pSFX->m_vOutBound.x * 100.0f);
						nValueX = nBound ? rand() % nBound : 0;
						nValueX = -INT(m_pSFX->m_vOutBound.x * 50.0f) + nValueX;

						nBound = INT(m_pSFX->m_vOutBound.y * 100.0f);
						nValueY = nBound ? rand() % nBound : 0;
						nValueY = -INT(m_pSFX->m_vOutBound.y * 50.0f) + nValueY;
					}

					break;

				case 1	:
					{
						int nBound = INT((m_pSFX->m_vOutBound.z - m_pSFX->m_vInBound.z) * 50.0f);
						nValueZ = nBound ? rand() % nBound : 0;
						nValueZ = INT(m_pSFX->m_vInBound.z * 50.0f) + nValueZ;

						nBound = INT(m_pSFX->m_vOutBound.x * 100.0f);
						nValueX = nBound ? rand() % nBound : 0;
						nValueX = -INT(m_pSFX->m_vOutBound.x * 50.0f) + nValueX;

						nBound = INT(m_pSFX->m_vOutBound.y * 100.0f);
						nValueY = nBound ? rand() % nBound : 0;
						nValueY = -INT(m_pSFX->m_vOutBound.y * 50.0f) + nValueY;
					}

					break;

				case 2	:
					{
						int nBound = INT((m_pSFX->m_vOutBound.x - m_pSFX->m_vInBound.x) * 50.0f);
						nValueX = nBound ? rand() % nBound : 0;
						nValueX = -INT(m_pSFX->m_vInBound.x * 50.0f) - nValueX;

						nBound = INT(m_pSFX->m_vOutBound.z * 100.0f);
						nValueZ = nBound ? rand() % nBound : 0;
						nValueZ = -INT(m_pSFX->m_vOutBound.z * 50.0f) + nValueZ;

						nBound = INT(m_pSFX->m_vOutBound.y * 100.0f);
						nValueY = nBound ? rand() % nBound : 0;
						nValueY = -INT(m_pSFX->m_vOutBound.y * 50.0f) + nValueY;
					}

					break;

				case 3	:
					{
						int nBound = INT((m_pSFX->m_vOutBound.x - m_pSFX->m_vInBound.x) * 50.0f);
						nValueX = nBound ? rand() % nBound : 0;
						nValueX = INT(m_pSFX->m_vInBound.x * 50.0f) + nValueX;

						nBound = INT(m_pSFX->m_vOutBound.z * 100.0f);
						nValueZ = nBound ? rand() % nBound : 0;
						nValueZ = -INT(m_pSFX->m_vOutBound.z * 50.0f) + nValueZ;

						nBound = INT(m_pSFX->m_vOutBound.y * 100.0f);
						nValueY = nBound ? rand() % nBound : 0;
						nValueY = -INT(m_pSFX->m_vOutBound.y * 50.0f) + nValueY;
					}

					break;

				case 4	:
					{
						int nBound = INT((m_pSFX->m_vOutBound.y - m_pSFX->m_vInBound.y) * 50.0f);
						nValueY = nBound ? rand() % nBound : 0;
						nValueY = -INT(m_pSFX->m_vInBound.y * 50.0f) - nValueY;

						nBound = INT(m_pSFX->m_vOutBound.z * 100.0f);
						nValueZ = nBound ? rand() % nBound : 0;
						nValueZ = -INT(m_pSFX->m_vOutBound.z * 50.0f) + nValueZ;

						nBound = INT(m_pSFX->m_vOutBound.x * 100.0f);
						nValueX = nBound ? rand() % nBound : 0;
						nValueX = -INT(m_pSFX->m_vOutBound.x * 50.0f) + nValueX;
					}

					break;

				case 5	:
					{
						int nBound = INT((m_pSFX->m_vOutBound.y - m_pSFX->m_vInBound.y) * 50.0f);
						nValueY = nBound ? rand() % nBound : 0;
						nValueY = INT(m_pSFX->m_vInBound.y * 50.0f) + nValueY;

						nBound = INT(m_pSFX->m_vOutBound.z * 100.0f);
						nValueZ = nBound ? rand() % nBound : 0;
						nValueZ = -INT(m_pSFX->m_vOutBound.z * 50.0f) + nValueZ;

						nBound = INT(m_pSFX->m_vOutBound.x * 100.0f);
						nValueX = nBound ? rand() % nBound : 0;
						nValueX = -INT(m_pSFX->m_vOutBound.x * 50.0f) + nValueX;
					}

					break;
				}

				pSFX->m_vLocalP.x = ((FLOAT) nValueX) / 100.0f;
				pSFX->m_vLocalP.y = ((FLOAT) nValueY) / 100.0f;
				pSFX->m_vLocalP.z = ((FLOAT) nValueZ) / 100.0f;
			}

			break;
		}

		if( m_pSFX->m_dwERRTime != 0 )
			pSFX->m_dwERRTime = rand() % m_pSFX->m_dwERRTime;

		if( m_pSFX->m_vERRPos.x != 0.0f )
			pSFX->m_vERRPos.x = (rand() % 2 ? 1.0f : -1.0f) * ((FLOAT) (rand() % 100)) * m_pSFX->m_vERRPos.x / 100.0f;

		if( m_pSFX->m_vERRPos.y != 0.0f )
			pSFX->m_vERRPos.y = (rand() % 2 ? 1.0f : -1.0f) * ((FLOAT) (rand() % 100)) * m_pSFX->m_vERRPos.y / 100.0f;

		if( m_pSFX->m_vERRPos.z != 0.0f )
			pSFX->m_vERRPos.z = (rand() % 2 ? 1.0f : -1.0f) * ((FLOAT) (rand() % 100)) * m_pSFX->m_vERRPos.z / 100.0f;

		if( m_pSFX->m_vERRRot.x != 0.0f )
			pSFX->m_vERRRot.x = (rand() % 2 ? 1.0f : -1.0f) * ((FLOAT) (rand() % 100)) * m_pSFX->m_vERRRot.x / 100.0f;

		if( m_pSFX->m_vERRRot.y != 0.0f )
			pSFX->m_vERRRot.y = (rand() % 2 ? 1.0f : -1.0f) * ((FLOAT) (rand() % 100)) * m_pSFX->m_vERRRot.y / 100.0f;

		if( m_pSFX->m_vERRRot.z != 0.0f )
			pSFX->m_vERRRot.z = (rand() % 2 ? 1.0f : -1.0f) * ((FLOAT) (rand() % 100)) * m_pSFX->m_vERRRot.z / 100.0f;

		if( m_pSFX->m_vERRScale.x != 0.0f )
			pSFX->m_vERRScale.x = (rand() % 2 ? 1.0f : -1.0f) * ((FLOAT) (rand() % 100)) * m_pSFX->m_vERRScale.x / 100.0f;

		if( m_pSFX->m_vERRScale.y != 0.0f )
			pSFX->m_vERRScale.y = (rand() % 2 ? 1.0f : -1.0f) * ((FLOAT) (rand() % 100)) * m_pSFX->m_vERRScale.y / 100.0f;

		if( m_pSFX->m_vERRScale.z != 0.0f )
			pSFX->m_vERRScale.z = (rand() % 2 ? 1.0f : -1.0f) * ((FLOAT) (rand() % 100)) * m_pSFX->m_vERRScale.z / 100.0f;

		pSFX->m_vLocalP.x *= m_vCurS.x;
		pSFX->m_vLocalP.y *= m_vCurS.y;
		pSFX->m_vLocalP.z *= m_vCurS.z;
		pSFX->m_vGlobal = vGlobal;
		pSFX->m_vPFactor = m_vPFactor;

		if( pSFX->InitSFX( pDevice, m_pSFX) )
		{
			pSFX->CalcFrame(FALSE);
			m_vPSFX.push_back(pSFX);
		}
	}
}

void CTachyonSFX::Render( CD3DDevice *pDevice, CD3DCamera *pCamera, BYTE bMaxAlpha )
{
	if( m_pSFX->m_dwLodLevel < m_dwGlobalLodLevel )
		return ;

	if( m_pSFX && m_pSFX->m_bSRCType == SFXSRC_TEX )
	{
		pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
		pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

		pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
		pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
		pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);

		pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

		pDevice->m_pDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		pDevice->m_pDevice->SetRenderState( D3DRS_ALPHAREF, 0x00000000);
		pDevice->m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE);

		pDevice->m_pDevice->SetRenderState( D3DRS_DESTBLEND, m_pSFX->m_dwDESTBlend);
		pDevice->m_pDevice->SetRenderState( D3DRS_SRCBLEND, m_pSFX->m_dwSRCBlend);
		pDevice->m_pDevice->SetRenderState( D3DRS_BLENDOP, m_pSFX->m_dwBlendOP);
		pDevice->m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);
		pDevice->m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
		pDevice->m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, m_pSFX->m_bZWrite);
		pDevice->m_pDevice->SetRenderState( D3DRS_ZENABLE, m_pSFX->m_bZEnable);

		pDevice->m_pDevice->SetFVF(T3DFVF_PVERTEX);
	}

	if( m_pSFX->m_bSFXType == SFX_PARTICLE && m_pSFX->m_bFollow )
	{
		int nCount = INT(m_vPSFX.size());

		m_vGMOVE = D3DXVECTOR3(
			m_vGlobal._41,
			m_vGlobal._42,
			m_vGlobal._43) - m_vGMOVE;

		for( int i=0; i<nCount; i++)
		{
			if(m_pSFX->m_bStrict)
			{
				m_vPSFX[i]->m_vGlobal._11 = m_vGlobal._11;
				m_vPSFX[i]->m_vGlobal._12 = m_vGlobal._12;
				m_vPSFX[i]->m_vGlobal._13 = m_vGlobal._13;
				m_vPSFX[i]->m_vGlobal._14 = m_vGlobal._14;
				m_vPSFX[i]->m_vGlobal._21 = m_vGlobal._21;
				m_vPSFX[i]->m_vGlobal._22 = m_vGlobal._22;
				m_vPSFX[i]->m_vGlobal._23 = m_vGlobal._23;
				m_vPSFX[i]->m_vGlobal._24 = m_vGlobal._24;
				m_vPSFX[i]->m_vGlobal._31 = m_vGlobal._31;
				m_vPSFX[i]->m_vGlobal._32 = m_vGlobal._32;
				m_vPSFX[i]->m_vGlobal._33 = m_vGlobal._33;
				m_vPSFX[i]->m_vGlobal._34 = m_vGlobal._34;
			}

			m_vPSFX[i]->m_vGlobal._41 += m_pSFX->m_fFollow * m_vGMOVE.x;
			m_vPSFX[i]->m_vGlobal._42 += m_pSFX->m_fFollow * m_vGMOVE.y;
			m_vPSFX[i]->m_vGlobal._43 += m_pSFX->m_fFollow * m_vGMOVE.z;
		}

		m_vGMOVE = D3DXVECTOR3(
			m_vGlobal._41,
			m_vGlobal._42,
			m_vGlobal._43);
	}

	if( !m_bDeleted && m_pSFX && m_pSFX->m_bSFXType == SFX_PARTICLE )
		if( m_dwGTime >= m_pSFX->m_dwGTime * m_bOptionLodLevel )
		{
			while( m_dwGTime >= m_pSFX->m_dwGTime * m_bOptionLodLevel )
				m_dwGTime -= m_pSFX->m_dwGTime;
			Generate(pDevice, pCamera);
		}

	if( m_pSFX &&
		m_pSFX->m_bSFXType == SFX_SINGLE &&
		(m_SINGLE.m_SRC.m_pOBJ || m_pSFX->m_bSRCType == SFXSRC_TEX) )
	{
		m_SINGLE.m_vPFactor = m_vPFactor;
		m_SINGLE.m_vGlobal = m_vGlobal;
		m_SINGLE.RenderSINGLE( pDevice, pCamera, bMaxAlpha );
	}

	int nCount = INT(m_vPSFX.size());
	m_dwParticleNum += nCount;
	for( int i=0; i<nCount; i++)
	{
		m_vPSFX[i]->m_vPFactor = m_vPFactor;
		m_vPSFX[i]->Render( pDevice, pCamera, bMaxAlpha );
	}
	pDevice->m_pDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD);

	nCount = INT(m_vCSFX.size());
	m_dwParticleNum += nCount;
	for( i=0; i<nCount; i++)
	{
		m_vCSFX[i]->m_vPFactor = m_vPFactor;
		m_vCSFX[i]->m_vGlobal = m_vGlobal;
		m_vCSFX[i]->Render( pDevice, pCamera, bMaxAlpha );
	}
}

void CTachyonSFX::ReleaseNoHost()
{
	MAPTSFX::iterator it = m_mapINST.begin();

	while(it != m_mapINST.end())
	{
		CTachyonSFX *pSFX = (*it).second;
		MAPTSFX::iterator itNEXT = it;
		itNEXT++;

		if( pSFX->m_pSFX && !pSFX->m_bHasHost )
			Release((*it).first);

		it = itNEXT;
	}
}

void CTachyonSFX::ReleaseCommon()
{
	MAPTSFX::iterator it;

	for( it = m_mapINST.begin(); it != m_mapINST.end(); it++)
		delete (*it).second;

	m_mapINST.clear();
	m_dwNextID = 0;
}

void CTachyonSFX::InitCommon()
{
	ReleaseCommon();
}

CTachyonSFX *CTachyonSFX::GetSFX( DWORD dwID)
{
	MAPTSFX::iterator finder = m_mapINST.find(dwID);

	if( finder != m_mapINST.end() )
		return (*finder).second;

	return NULL;
}

DWORD CTachyonSFX::RegSFX( CTachyonSFX *pSFX, BYTE bBias)
{
	DWORD dwID = (m_dwNextID & 0x00FFFFFF) | ((((DWORD) bBias) << 24) & 0xFF000000);

	while(m_mapINST.find(dwID) != m_mapINST.end())
	{
		m_dwNextID = (m_dwNextID + 1) % 0x00FFFFFF;
		dwID = (m_dwNextID & 0x00FFFFFF) | ((((DWORD) bBias) << 24) & 0xFF000000);
	}
	m_mapINST.insert( MAPTSFX::value_type( dwID, pSFX));

	return dwID;
}

BYTE CTachyonSFX::IsVisible( CD3DCamera *pCamera,
							 FLOAT fDistMAX)
{
	if(!m_pSFX)
		return FALSE;

	if( fDistMAX == 0.0f )
		return TRUE;

	if( !m_bShow )
		return FALSE;

	D3DXVECTOR3 vDIR = pCamera->m_vPosition - pCamera->m_vTarget;
	D3DXVECTOR3 vPOS(
		m_vGlobal._41,
		m_vGlobal._42,
		m_vGlobal._43);

	D3DXPLANE vPLANE;

	FLOAT fCamDist = D3DXVec3Length(&(vPOS - pCamera->m_vTarget));
	FLOAT fRadius = 10.0f;

	if( fCamDist - fRadius > fDistMAX )
		return FALSE;

	vDIR /= D3DXVec3Length(&vDIR);
	D3DXPlaneFromPointNormal(
		&vPLANE,
		&vPOS,
		&vDIR);

	FLOAT fDOT = D3DXPlaneDotCoord(
		&vPLANE,
		&pCamera->m_vPosition);

	if( fDOT + fRadius < 0.0f )
		return FALSE;

	float fDiv = fDistMAX / LODLEVEL_6;
	int nDistLevel = (int)(fCamDist / fDiv);
	m_dwGlobalLodLevel = max( m_bOptionLodLevel, nDistLevel );
	m_dwGlobalLodLevel = min( m_dwGlobalLodLevel, LODLEVEL_6 );

	if(pCamera->IsOrthoCamera())
	{
		FLOAT fWIDTH = D3DXVec3Length(&(vPOS - (pCamera->m_vPosition - fDOT * vDIR)));

		if( fWIDTH - fRadius > pCamera->m_fWidth )
			return FALSE;
	}
	else
	{
		FLOAT fWIDTH = D3DXVec3Length(&(vPOS - (pCamera->m_vPosition - fDOT * vDIR)));
		FLOAT fFOV = pCamera->m_fFOV * pCamera->m_fWidth / pCamera->m_fHeight;
		fFOV = max( fFOV, pCamera->m_fFOV);

		if( fWIDTH - fRadius > tanf(fFOV / 2.0f) * fDOT )
			return FALSE;
	}

	return TRUE;
}

void CTachyonSFX::Release( DWORD dwID)
{
	MAPTSFX::iterator finder = m_mapINST.find(dwID);

	if( finder != m_mapINST.end() )
	{
		delete (*finder).second;
		m_mapINST.erase(finder);
	}

	if(m_mapINST.empty())
		ReleaseCommon();
}

void CTachyonSFX::CalcBUF( LPDIRECT3DDEVICE9 pDevice, DWORD dwTick)
{
	MAPTSFX::iterator it = m_mapINST.begin();

	while(it != m_mapINST.end())
	{
		CTachyonSFX *pSFX = (*it).second;
		MAPTSFX::iterator itNEXT = it;
		itNEXT++;

		if(pSFX->m_pSFX)
			if(!pSFX->IsDead())
				pSFX->CalcTick( pDevice, dwTick);
			else if(!pSFX->m_bHasHost)
				Release((*it).first);

		it = itNEXT;
	}
}

void CTachyonSFX::RenderBUF( CD3DDevice *pDevice,
							 CD3DCamera *pCamera,
							 FLOAT fDistMAX)
{
	MAPTSFX::iterator it;
	m_fDistMAX = fDistMAX;
	m_dwParticleNum = 0;

	for( it = m_mapINST.begin(); it != m_mapINST.end(); it++)
		if( (*it).second->m_pSFX &&
			!(*it).second->IsDead() &&
			(*it).second->IsVisible( pCamera, fDistMAX) )
		{
			(*it).second->Render( pDevice, pCamera, (*it).second->m_bMaxAlpha );
		}
}


void CTachyonSFX::SetOptionLOD(
		BYTE bSystemOption)
{
	switch( bSystemOption )
	{
	case 2 : m_bOptionLodLevel = LODLEVEL_1; break;
	case 1 : m_bOptionLodLevel = LODLEVEL_3; break;
	case 0 : m_bOptionLodLevel = LODLEVEL_6; break;
	}
}

BYTE CTachyonSFX::GetSystemOptionLOD()
{
	BYTE bResult;

	switch( m_bOptionLodLevel )
	{
	case LODLEVEL_1 : bResult = 2; break;
	case LODLEVEL_2 : 
	case LODLEVEL_3 : bResult = 1; break;
	case LODLEVEL_4 :
	case LODLEVEL_5 : 
	case LODLEVEL_6 : bResult = 0; break;
	}

	return bResult;
}
