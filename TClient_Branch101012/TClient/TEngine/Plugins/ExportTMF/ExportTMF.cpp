/**********************************************************************
 *<
	FILE: ExportTMF.cpp

	DESCRIPTION: Tachyon Engine Mesh Data Export Class Cpp File

	CREATED BY: Hyunho, Shin

	HISTORY: Created At 2002/2/19

 *>	Copyright (c) Tachyon Entertainment, All Rights Reserved.
 **********************************************************************/
#pragma warning( disable : 4786 4530 )

#include "ExportTMF.h"
#include "Modstack.h"
#include "StdMat.h"
#include "BipedApi.h"
#include "Tracks.h"
#include "PHYEXP.H"
#include "ExportComm.h"


#define TachyonShader_CLASS_ID		Class_ID( 0x6cb664ff, 0x995ac191)
#define CLUSTNODEOSM_CLASS_ID		Class_ID( 0x000c4d33, 0x00000000)
#define CEXPORTTMF_CLASS_ID			Class_ID( 0x41694b47, 0x2319def3)
#define TMF_VERSION					301


class CExportTMF : public SceneExport
{
public:
	static HWND hParams;

	int ExtCount();
	const TCHAR *Ext( int n);
	const TCHAR *LongDesc();
	const TCHAR *ShortDesc();
	const TCHAR *AuthorName();
	const TCHAR *CopyrightMessage();
	const TCHAR *OtherMessage1();
	const TCHAR *OtherMessage2();
	unsigned int Version();
	void ShowAbout( HWND hWnd);

	BOOL SupportsOptions( int ext, DWORD options);

	int DoExport(
		const TCHAR *name,
		ExpInterface *ei,
		Interface *i,
		BOOL suppressPrompts = FALSE,
		DWORD options = 0);

	//Constructor/Destructor

	CExportTMF();
	~CExportTMF();

protected:
	// Utility member function
	D3DXMATRIX GetD3DMatrix( Matrix3 vMAT);

	LPINODEDATA FindIBONESDATA( INode *pNODE);
	LPMESHNODE FindMESHNODE( Mtl *pMTRL);
	WORD FindINDEX( LPTMESHBUFFER pDATA);

	LPWEIGHT FindWEIGHT(
		LPLISTWEIGHT pWEIGHT,
		BYTE bINDEX);

	D3DXVECTOR3 GetNormal(
		LPMESHDATA pINODE,
		Face *pFACE,
		int nCount,
		DWORD dwINDEX,
		DWORD dwGROUP,
		int *pVTX,
		int nINDEX);

	BYTE IsHelperBONE( INode *pNODE);
	BYTE IsBONE( INode *pNODE);
	BYTE IsMESH( INode *pNODE);

	void BuildMESHDATA( LPMESHDATA pINODE);
	void BuildWEIGHT( LPMESHDATA pINODE);
	void BuildMESH( LPMESHDATA pINODE);

	void CheckBONES( HWND hWnd);
	void CheckMTRL( HWND hWnd);

protected:
	// Initialize member function
	FLOAT InitMESH( LPD3DXVECTOR3 pCENTER);
	void InitDATA( INode *pNODE);
	void InitDATA();
	void ClearDATA();

protected:
	// Export member function
	void ExportBONES(
		LPINODEDATA pINODE,
		FILE *pFILE);

public:
	BYTE m_bExportAllFaces;
	BYTE m_bExportAll;

	FLOAT m_fSCALE;

protected:
	LISTINODEDATA m_vIBONES;
	LISTMESHNODE m_vIMESH;

	VTMESHBUFFER m_vVB;
	VMESHDATA m_vIDATA;

	DWORD m_dwSTART;
};


class CExportTMFClassDesc:public ClassDesc2
{
public:
	int IsPublic() { return TRUE; }
	void *Create( BOOL loading = FALSE) { return new CExportTMF(); }
	const TCHAR *ClassName() { return GetString(IDS_CLASS_NAME); }
	SClass_ID SuperClassID() { return SCENE_EXPORT_CLASS_ID; }
	Class_ID ClassID() { return CEXPORTTMF_CLASS_ID; }

	const TCHAR* Category() { return GetString(IDS_CATEGORY); }

	const TCHAR *InternalName() { return _T("CExportTMF"); }
	HINSTANCE HInstance() { return hInstance; }
};


static CExportTMFClassDesc CExportTMFDesc;
ClassDesc2* GetCExportTMFDesc() { return &CExportTMFDesc; }

static INT_PTR CALLBACK ExportTMFProc( HWND hWnd,
									   UINT msg,
									   WPARAM wParam,
									   LPARAM lParam)
{
	ISpinnerControl* pSpinCtrl;

	CExportTMF* pExpTMF = (CExportTMF*) GetWindowLongPtr( hWnd, GWLP_USERDATA);
	switch(msg)
	{
	case WM_INITDIALOG		:
		{
			pExpTMF = (CExportTMF*) lParam;
			SetWindowLongPtr( hWnd, GWLP_USERDATA, lParam); 
			CenterWindow( hWnd, GetParent(hWnd));

			pSpinCtrl = GetISpinner( GetDlgItem( hWnd, IDC_SPIN_SCALE));
			pSpinCtrl->LinkToEdit( GetDlgItem( hWnd, IDC_EDIT_SCALE), EDITTYPE_FLOAT);
			pSpinCtrl->SetLimits( 0.0f, 1000.0f, TRUE);
			pSpinCtrl->SetAutoScale(TRUE);
			pSpinCtrl->SetValue( pExpTMF->m_fSCALE, FALSE);
			ReleaseISpinner(pSpinCtrl);

			SendMessage( GetDlgItem( hWnd, IDC_CHECK_EXPORT_ALL_FACES), BM_SETCHECK, pExpTMF->m_bExportAllFaces ? BST_CHECKED : BST_UNCHECKED, 0);
		}

		break;

	case CC_SPINNER_CHANGE	: pSpinCtrl = (ISpinnerControl *) lParam; break;
	case WM_COMMAND			:
		switch(LOWORD(wParam))
		{
		case IDC_BTN_TMFEXPORT	:
			{
				pSpinCtrl = GetISpinner( GetDlgItem( hWnd, IDC_SPIN_SCALE));
				pExpTMF->m_fSCALE = pSpinCtrl->GetFVal();
				ReleaseISpinner(pSpinCtrl);

				switch(SendMessage( GetDlgItem( hWnd, IDC_CHECK_EXPORT_ALL_FACES), BM_GETCHECK, 0, 0))
				{
				case BST_CHECKED	: pExpTMF->m_bExportAllFaces = TRUE; break;
				default				: pExpTMF->m_bExportAllFaces = FALSE; break;
				}

				EndDialog( hWnd, 1);
			}

			break;

		case IDCANCEL			: EndDialog( hWnd, 0); break;
		}

		break;

	default					: return FALSE;
	}

	return TRUE;
}

Modifier* FindPhysiqueModifier( INode* pNode)
{
	if(!pNode)
		return NULL;

	Object *pRefObject = pNode->GetObjectRef();

	if( pRefObject && pRefObject->SuperClassID() == GEN_DERIVOB_CLASS_ID )
	{
		IDerivedObject* pDerived = static_cast <IDerivedObject*> (pRefObject);
		int nModStackIndex = 0;

		while( nModStackIndex < pDerived->NumModifiers() )
		{
			Modifier* pMod = pDerived->GetModifier(nModStackIndex);

			if( pMod->ClassID() == Class_ID( PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B))
				return pMod;

			nModStackIndex++;
		}
	}

	return NULL;
}

void ScaleBiped( INode* pNode, int scale)
{
	if(pNode->IsRootNode())
		return;

	Control *c = pNode->GetTMController();
	if( c->ClassID() == BIPSLAVE_CONTROL_CLASS_ID ||
		c->ClassID() == BIPBODY_CONTROL_CLASS_ID ||
		c->ClassID() == FOOTPRINT_CLASS_ID )
    {
		IBipedExport* BipIface = (IBipedExport*) c->GetInterface(I_BIPINTERFACE);
		if(BipIface)
			BipIface->RemoveNonUniformScale(scale);

		Control* iMaster = (Control*) c->GetInterface(I_MASTER);
		if(iMaster)
			iMaster->NotifyDependents( FOREVER, PART_TM, REFMSG_CHANGE);

		c->ReleaseInterface( I_MASTER, iMaster);
		c->ReleaseInterface( I_BIPINTERFACE, BipIface);
	}
}

BYTE IsEqual( LPTMESHBUFFER pPoint1, LPTMESHBUFFER pPoint2)
{
	return fabs(pPoint2->m_fPosX - pPoint1->m_fPosX) < ALMOST_ZERO &&
		fabs(pPoint2->m_fPosY - pPoint1->m_fPosY) < ALMOST_ZERO &&
		fabs(pPoint2->m_fPosZ - pPoint1->m_fPosZ) < ALMOST_ZERO &&
		fabs(pPoint2->m_fNormalX - pPoint1->m_fNormalX) < ALMOST_ZERO &&
		fabs(pPoint2->m_fNormalY - pPoint1->m_fNormalY) < ALMOST_ZERO &&
		fabs(pPoint2->m_fNormalZ - pPoint1->m_fNormalZ) < ALMOST_ZERO &&
		fabs(pPoint2->m_fU1 - pPoint1->m_fU1) < ALMOST_ZERO &&
		fabs(pPoint2->m_fV1 - pPoint1->m_fV1) < ALMOST_ZERO &&
		fabs(pPoint2->m_fU2 - pPoint1->m_fU2) < ALMOST_ZERO &&
		fabs(pPoint2->m_fV2 - pPoint1->m_fV2) < ALMOST_ZERO &&
		fabs(pPoint2->m_fWeight[0] - pPoint1->m_fWeight[0]) < ALMOST_ZERO &&
		fabs(pPoint2->m_fWeight[1] - pPoint1->m_fWeight[1]) < ALMOST_ZERO &&
		fabs(pPoint2->m_fWeight[2] - pPoint1->m_fWeight[2]) < ALMOST_ZERO &&
		pPoint2->m_dwMatIndex == pPoint1->m_dwMatIndex ? TRUE : FALSE;
}

struct name_less : public binary_function< LPINODEDATA, LPINODEDATA, bool>
{
	bool operator ()( const LPINODEDATA& _Left, const LPINODEDATA& _Right) const
	{
		return strcmp( _Left->m_pINODE->GetName(), _Right->m_pINODE->GetName()) < 0 ? true : false;
	};
};

struct weight_greater : public binary_function< LPWEIGHT, LPWEIGHT, bool>
{
	bool operator ()( const LPWEIGHT& _Left, const LPWEIGHT& _Right) const
	{
		return _Left->m_fWeight > _Right->m_fWeight ? true : false;
	};
};

struct mtrl_less : public binary_function< LPMESHNODE, LPMESHNODE, bool>
{
	bool operator ()( const LPMESHNODE& _Left, const LPMESHNODE& _Right) const
	{
		if(!_Left->m_pMTRL)
			return true;

		if(!_Right->m_pMTRL)
			return false;

		return strcmp( _Left->m_pMTRL->GetName(), _Right->m_pMTRL->GetName()) < 0 ? true : false;
	};
};

//--- CExportTMF -------------------------------------------------------
CExportTMF::CExportTMF()
{
	m_vIBONES.clear();
	m_vIMESH.clear();
	m_vIDATA.clear();
	m_vVB.clear();

	m_bExportAllFaces = TRUE;
	m_bExportAll = TRUE;
	m_dwSTART = 0;
	m_fSCALE = 0.01f;
}

CExportTMF::~CExportTMF() 
{
	ClearDATA();
}

int CExportTMF::ExtCount()
{
	return 1;
}

const TCHAR *CExportTMF::Ext( int n)
{
	return _T("TMF");
}

const TCHAR *CExportTMF::LongDesc()
{
	return _T("Tachyon Engine Mesh Data File");
}

const TCHAR *CExportTMF::ShortDesc()
{
	return _T("Tachyon Engine Mesh Data File");
}

const TCHAR *CExportTMF::AuthorName()
{
	return _T("Hyunho, Shin");
}

const TCHAR *CExportTMF::CopyrightMessage() 
{
	return _T("");
}

const TCHAR *CExportTMF::OtherMessage1()
{
	return _T("");
}

const TCHAR *CExportTMF::OtherMessage2()
{
	return _T("");
}

unsigned int CExportTMF::Version()
{
	return 100;
}

void CExportTMF::ShowAbout( HWND hWnd)
{
}

BOOL CExportTMF::SupportsOptions( int ext, DWORD options)
{
	return TRUE;
}

D3DXMATRIX CExportTMF::GetD3DMatrix( Matrix3 vMAT)
{
	D3DXMATRIX vRESULT;

	vRESULT._11 = vMAT.GetRow(0).x;
	vRESULT._12 = vMAT.GetRow(0).z;
	vRESULT._13 = vMAT.GetRow(0).y;
	vRESULT._14 = 0.0f;

	vRESULT._21 = vMAT.GetRow(2).x;
	vRESULT._22 = vMAT.GetRow(2).z;
	vRESULT._23 = vMAT.GetRow(2).y;
	vRESULT._24 = 0.0f;

	vRESULT._31 = vMAT.GetRow(1).x;
	vRESULT._32 = vMAT.GetRow(1).z;
	vRESULT._33 = vMAT.GetRow(1).y;
	vRESULT._34 = 0.0f;

	vRESULT._41 = vMAT.GetRow(3).x;
	vRESULT._42 = vMAT.GetRow(3).z;
	vRESULT._43 = vMAT.GetRow(3).y;
	vRESULT._44 = 1.0f;

	return vRESULT;
}

D3DXVECTOR3 CExportTMF::GetNormal( LPMESHDATA pINODE,
								   Face *pFACE,
								   int nCount,
								   DWORD dwINDEX,
								   DWORD dwGROUP,
								   int *pVTX,
								   int nINDEX)
{
	D3DXVECTOR3 vNORMAL( 0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vSUM( 0.0f, 0.0f, 0.0f);

	for( int i=0; i<nCount; i++)
		if( i == nINDEX || ((pFACE[i].smGroup & dwGROUP) && (
			pFACE[i].v[0] == dwINDEX ||
			pFACE[i].v[1] == dwINDEX ||
			pFACE[i].v[2] == dwINDEX)) )
		{
			D3DXVECTOR3 vPosition[3];
			D3DXVECTOR3 vFaceNormal;
			D3DXVECTOR3 vCross;

			for( BYTE j=0; j<3; j++)
			{
				vPosition[j] = D3DXVECTOR3(
					pINODE->m_vMESH[pFACE[i].v[pVTX[j]]]->m_fPosX,
					pINODE->m_vMESH[pFACE[i].v[pVTX[j]]]->m_fPosY,
					pINODE->m_vMESH[pFACE[i].v[pVTX[j]]]->m_fPosZ);
			}

			vPosition[1] -= vPosition[0];
			vPosition[2] -= vPosition[0];

			D3DXVec3Cross(
				&vCross,
				&vPosition[1],
				&vPosition[2]);

			D3DXVec3Normalize(
				&vFaceNormal,
				&vCross);

			vSUM += vFaceNormal;
		}

	D3DXVec3Normalize(
		&vNORMAL,
		&vSUM);

	return vNORMAL;
}

LPINODEDATA CExportTMF::FindIBONESDATA( INode *pNODE)
{
	LISTINODEDATA::iterator itINODE;

	for( itINODE = m_vIBONES.begin(); itINODE != m_vIBONES.end(); itINODE++)
		if( (*itINODE)->m_pINODE == pNODE )
			return (*itINODE);

	return NULL;
}

LPMESHNODE CExportTMF::FindMESHNODE( Mtl *pMTRL)
{
	LISTMESHNODE::iterator itINODE;

	for( itINODE = m_vIMESH.begin(); itINODE != m_vIMESH.end(); itINODE++)
		if( (*itINODE)->m_pMTRL == pMTRL )
			return (*itINODE);
	LPMESHNODE pNODE = new MESHNODE();

	m_vIMESH.push_back(pNODE);
	pNODE->m_pMTRL = pMTRL;

	return pNODE;
}

LPWEIGHT CExportTMF::FindWEIGHT( LPLISTWEIGHT pWEIGHT,
								 BYTE bINDEX)
{
	LISTWEIGHT::iterator itWEIGHT;

	for( itWEIGHT = pWEIGHT->begin(); itWEIGHT != pWEIGHT->end(); itWEIGHT++)
		if( (*itWEIGHT)->m_bINDEX == bINDEX )
			return (*itWEIGHT);

	return NULL;
}

WORD CExportTMF::FindINDEX( LPTMESHBUFFER pDATA)
{
	for( WORD i=0; i<WORD(m_vVB.size()); i++)
		if(IsEqual( pDATA, m_vVB[i]))
			return i;

	return WORD(m_vVB.size());
}

void CExportTMF::ClearDATA()
{
	while(!m_vIBONES.empty())
	{
		delete m_vIBONES.back();
		m_vIBONES.pop_back();
	}

	while(!m_vIMESH.empty())
	{
		delete m_vIMESH.back();
		m_vIMESH.pop_back();
	}

	while(!m_vIDATA.empty())
	{
		delete m_vIDATA.back();
		m_vIDATA.pop_back();
	}

	while(!m_vVB.empty())
	{
		delete m_vVB.back();
		m_vVB.pop_back();
	}
}

BYTE CExportTMF::IsBONE( INode *pNODE)
{
	if(!pNODE)
		return FALSE;

	ObjectState vSTATE = pNODE->EvalWorldState(m_dwSTART);
	if(vSTATE.obj)
		if( vSTATE.obj->SuperClassID() == GEOMOBJECT_CLASS_ID )
		{
			if( vSTATE.obj->ClassID() == BONE_OBJ_CLASSID )
				return TRUE;

			Control* pTMCtrl = pNODE->GetTMController();
			if( pTMCtrl && (
				pTMCtrl->ClassID() == BIPSLAVE_CONTROL_CLASS_ID ||
				pTMCtrl->ClassID() == BIPBODY_CONTROL_CLASS_ID ||
				pTMCtrl->ClassID() == FOOTPRINT_CLASS_ID ) )
				return TRUE;
		}
		else if( vSTATE.obj->SuperClassID() == HELPER_CLASS_ID )
			if( vSTATE.obj->ClassID() == Class_ID( DUMMY_CLASS_ID, 0) )
				return IsHelperBONE(pNODE);
			else if( vSTATE.obj->ClassID() == Class_ID( POINTHELP_CLASS_ID, 0) )
				return TRUE;

	return FALSE;
}

BYTE CExportTMF::IsHelperBONE( INode *pNODE)
{
	if(!pNODE)
		return FALSE;

	ObjectState vSTATE = pNODE->EvalWorldState(m_dwSTART);
	if( vSTATE.obj && vSTATE.obj->SuperClassID() == GEOMOBJECT_CLASS_ID )
	{
		if( vSTATE.obj->ClassID() == BONE_OBJ_CLASSID )
			return TRUE;

		Control* pTMCtrl = pNODE->GetTMController();
		if( pTMCtrl && (
			pTMCtrl->ClassID() == BIPSLAVE_CONTROL_CLASS_ID ||
			pTMCtrl->ClassID() == BIPBODY_CONTROL_CLASS_ID ||
			pTMCtrl->ClassID() == FOOTPRINT_CLASS_ID ) )
			return TRUE;
	}

	return IsHelperBONE(pNODE->GetParentNode());
}

BYTE CExportTMF::IsMESH( INode *pNODE)
{
	if(!pNODE)
		return FALSE;

	ObjectState vSTATE = pNODE->EvalWorldState(m_dwSTART);
	if( vSTATE.obj &&
		vSTATE.obj->SuperClassID() == GEOMOBJECT_CLASS_ID )
	{
		if( vSTATE.obj->ClassID() != BONE_OBJ_CLASSID )
			return TRUE;

		Control* pTMCtrl = pNODE->GetTMController();
		if( pTMCtrl &&
			pTMCtrl->ClassID() != BIPSLAVE_CONTROL_CLASS_ID &&
			pTMCtrl->ClassID() != BIPBODY_CONTROL_CLASS_ID &&
			pTMCtrl->ClassID() != FOOTPRINT_CLASS_ID )
			return TRUE;
	}

	return FALSE;
}

void CExportTMF::BuildMESHDATA( LPMESHDATA pINODE)
{
	ObjectState vSTATE = pINODE->m_pINODE->EvalWorldState(m_dwSTART);

	TriObject *pTOBJ = (TriObject *) vSTATE.obj->ConvertToType(
		0, Class_ID( TRIOBJ_CLASS_ID, 0));

	if(pTOBJ)
	{
		Mesh *pMESH = &(pTOBJ->GetMesh());

		if(pMESH)
		{
			LPINODEDATA pBONES = FindIBONESDATA(pINODE->m_pINODE->GetParentNode());

			Matrix3 vNodeTM = pINODE->m_pINODE->GetObjectTM(m_dwSTART);
			DWORD dwCount = pMESH->getNumVerts();

			for( DWORD i=0; i<dwCount; i++)
			{
				Point3& vPOINT = pMESH->getVert(i) * vNodeTM;
				LPTMESHBUFFER pPOINT = new TMESHBUFFER();

				pPOINT->m_fPosX = vPOINT.x;
				pPOINT->m_fPosY = vPOINT.z;
				pPOINT->m_fPosZ = vPOINT.y;

				for( BYTE j=0; j<WEIGHT_COUNT; j++)
					pPOINT->m_fWeight[j] = 0.0f;
				pPOINT->m_dwMatIndex = 0;

				if(pBONES)
				{
					pPOINT->m_dwMatIndex = pBONES->m_bINDEX + 1;
					pPOINT->m_fWeight[0] = 1.0f;
				}

				pINODE->m_vMESH.push_back(pPOINT);
			}
		}
	}
}

void CExportTMF::BuildWEIGHT( LPMESHDATA pINODE)
{
	Modifier *pMOD = FindPhysiqueModifier(pINODE->m_pINODE);

	if(pMOD)
	{
		IPhysiqueExport *pEXP = (IPhysiqueExport *) pMOD->GetInterface(I_PHYEXPORT);

		if(pEXP)
		{
			IPhyContextExport *pContextEXP = (IPhyContextExport *) pEXP->GetContextInterface(pINODE->m_pINODE);

			if(pContextEXP)
			{
				pContextEXP->ConvertToRigid(TRUE);
				pContextEXP->AllowBlending(TRUE);

				for( int i=0; i<pContextEXP->GetNumberVertices(); i++)
				{
					IPhyVertexExport *pVertexEXP = pContextEXP->GetVertexInterface(i);

					if(pVertexEXP)
						switch(pVertexEXP->GetVertexType())
						{
						case RIGID_BLENDED_TYPE	:
							{
								IPhyBlendedRigidVertex *pBlendedVTX = (IPhyBlendedRigidVertex *) pVertexEXP;
								int nCount = pBlendedVTX->GetNumberNodes();

								LISTWEIGHT vWEIGHT;
								vWEIGHT.clear();

								for( int j=0; j<nCount; j++)
								{
									LPINODEDATA pBONE = FindIBONESDATA(pBlendedVTX->GetNode(j));

									if(pBONE)
									{
										LPWEIGHT pWEIGHT = FindWEIGHT(
											&vWEIGHT,
											pBONE->m_bINDEX + 1);
										ScaleBiped( pBONE->m_pINODE, 0);

										if(!pWEIGHT)
										{
											pWEIGHT = new WEIGHT();

											pWEIGHT->m_bINDEX = pBONE->m_bINDEX + 1;
											vWEIGHT.push_back(pWEIGHT);
										}

										pWEIGHT->m_fWeight += pBlendedVTX->GetWeight(j);
									}
								}
								vWEIGHT.sort(weight_greater());

								while(INT(vWEIGHT.size()) > WEIGHT_COUNT + 1)
								{
									delete vWEIGHT.back();
									vWEIGHT.pop_back();
								}

								LISTWEIGHT::iterator itWEIGHT;
								FLOAT fSUM = 0.0f;

								for( itWEIGHT = vWEIGHT.begin(); itWEIGHT != vWEIGHT.end(); itWEIGHT++)
									fSUM += (*itWEIGHT)->m_fWeight;

								for( itWEIGHT = vWEIGHT.begin(); itWEIGHT != vWEIGHT.end(); itWEIGHT++)
									(*itWEIGHT)->m_fWeight /= fSUM;
								nCount = 0;

								for( itWEIGHT = vWEIGHT.begin(); itWEIGHT != vWEIGHT.end(); itWEIGHT++)
								{
									pINODE->m_vMESH[i]->m_dwMatIndex |= (*itWEIGHT)->m_bINDEX << (nCount * 8);

									if( nCount < WEIGHT_COUNT )
										pINODE->m_vMESH[i]->m_fWeight[nCount] = (*itWEIGHT)->m_fWeight;
									nCount++;
								}

								while(!vWEIGHT.empty())
								{
									delete vWEIGHT.back();
									vWEIGHT.pop_back();
								}
							}

							break;

						case RIGID_TYPE			:
							{
								IPhyRigidVertex *pRigidVTX = (IPhyRigidVertex *) pVertexEXP;
								LPINODEDATA pBONE = FindIBONESDATA(pRigidVTX->GetNode());

								if(pBONE)
								{
									pINODE->m_vMESH[i]->m_dwMatIndex = pBONE->m_bINDEX + 1;
									pINODE->m_vMESH[i]->m_fWeight[0] = 1.0f;
								}
							}

							break;

						default					: break;
						}
				}

				pEXP->ReleaseContextInterface(pContextEXP);
				pMOD->ReleaseInterface( I_PHYINTERFACE, pEXP);
			}
			else
				pMOD->ReleaseInterface( I_PHYINTERFACE, pEXP);
		}
	}
}

void CExportTMF::BuildMESH( LPMESHDATA pINODE)
{
	ObjectState vTOBJ = pINODE->m_pINODE->EvalWorldState(m_dwSTART);
	TriObject *pTOBJ = (TriObject *) vTOBJ.obj->ConvertToType(
		0, Class_ID( TRIOBJ_CLASS_ID, 0));

	if(pTOBJ)
	{
		Mesh *pMESH = &(pTOBJ->GetMesh());

		if(pMESH)
		{
			Matrix3 vNodeTM = pINODE->m_pINODE->GetObjectTM(m_dwSTART);
			Face *pFACE = pMESH->faces;

			int nVX[3];

			if( DotProd( CrossProd( vNodeTM.GetRow(0), vNodeTM.GetRow(1) ), vNodeTM.GetRow(2) ) < 0.0 ? TRUE : FALSE )
			{
				nVX[0] = 0;
				nVX[1] = 1;
				nVX[2] = 2;
			}
			else
			{
				nVX[0] = 2;
				nVX[1] = 1;
				nVX[2] = 0;
			}

			if(pFACE)
			{
				Mtl *pMtl = pINODE->m_pINODE->GetMtl();
				int nFaceCount = pMESH->getNumFaces();

				for( int i=0; i<nFaceCount; i++)
					if( m_bExportAllFaces || pMESH->faceSel[i] )
					{
						Mtl *pSubMtl = pMtl;

						while( pSubMtl && pSubMtl->ClassID() != Class_ID( DMTL_CLASS_ID, 0) )
							pSubMtl = pSubMtl->GetSubMtl(pFACE[i].getMatID());
						LPMESHNODE pDATA = FindMESHNODE(pSubMtl);

						Shader *pTSH = pSubMtl ? ((StdMat2 *) pSubMtl)->GetShader() : NULL;
						BYTE bUseDI = !pTSH || pTSH->ClassID() != TachyonShader_CLASS_ID ? TRUE : FALSE;

						for( BYTE j=0; j<3; j++)
						{
							LPTMESHBUFFER pPOINT = new TMESHBUFFER();
							FLOAT vTU[S_COUNT] = { 0.0f, 0.0f};
							FLOAT vTV[S_COUNT] = { 0.0f, 0.0f};

							if( pSubMtl && pMESH->getNumTVerts() > 0 )
								for( BYTE k=0; k<S_COUNT; k++)
								{
									BitmapTex *pTEX = (BitmapTex *) ((StdMat *) pSubMtl)->GetSubTexmap(bUseDI ? ID_DI : k);

									if(pTEX)
									{
										int nChannel = pTEX->GetMapChannel();

										UVVert *pVERT = pMESH->mapVerts(nChannel);
										TVFace *pFACE = pMESH->mapFaces(nChannel);

										if( pVERT && pFACE )
										{
											Matrix3 vUVTrans;
											Point3 vUVMap;

											pTEX->GetUVTransform(vUVTrans);
											vUVMap = pVERT[pFACE[i].t[nVX[j]]] * vUVTrans;

											vTV[k] = 1.0f - vUVMap.y;
											vTU[k] = vUVMap.x;
										}
									}
								}

							D3DXVECTOR3 vNORMAL = GetNormal(
								pINODE,
								pFACE,
								nFaceCount,
								pFACE[i].v[nVX[j]],
								pFACE[i].smGroup,
								nVX, i);

							for( BYTE k=0; k<WEIGHT_COUNT; k++)
								pPOINT->m_fWeight[k] = pINODE->m_vMESH[pFACE[i].v[nVX[j]]]->m_fWeight[k];

							pPOINT->m_dwMatIndex = pINODE->m_vMESH[pFACE[i].v[nVX[j]]]->m_dwMatIndex;
							pPOINT->m_fPosX = m_fSCALE * pINODE->m_vMESH[pFACE[i].v[nVX[j]]]->m_fPosX;
							pPOINT->m_fPosY = m_fSCALE * pINODE->m_vMESH[pFACE[i].v[nVX[j]]]->m_fPosY;
							pPOINT->m_fPosZ = m_fSCALE * pINODE->m_vMESH[pFACE[i].v[nVX[j]]]->m_fPosZ;

							pPOINT->m_fNormalX = vNORMAL.x;
							pPOINT->m_fNormalY = vNORMAL.y;
							pPOINT->m_fNormalZ = vNORMAL.z;

							pPOINT->m_fU1 = vTU[S_DI];
							pPOINT->m_fV1 = vTV[S_DI];
							pPOINT->m_fU2 = vTU[S_DK];
							pPOINT->m_fV2 = vTV[S_DK];

							WORD wINDEX = FindINDEX(pPOINT);
							if( wINDEX >= WORD(m_vVB.size()) )
							{
								wINDEX = WORD(m_vVB.size());
								m_vVB.push_back(pPOINT);
							}
							else
								delete pPOINT;

							pDATA->m_vMESH.push_back(wINDEX);
						}
					}
			}
		}
	}
}

void CExportTMF::InitDATA( INode *pNODE)
{
	if(IsBONE(pNODE))
	{
		LPINODEDATA pIDATA = new INODEDATA();

		pIDATA->m_pINODE = pNODE;
		pIDATA->m_bINDEX = 0;
		pIDATA->m_bNODE = INT(m_vIBONES.size());

		m_vIBONES.push_back(pIDATA);
	}
	else if( IsMESH(pNODE) && (m_bExportAll || pNODE->Selected()) )
	{
		LPMESHDATA pDATA = new MESHDATA();
		pDATA->m_pINODE = pNODE;

		m_vIDATA.push_back(pDATA);
	}

	int nCount = pNODE->NumberOfChildren();
	for( int i=0; i<nCount; i++)
		InitDATA(pNODE->GetChildNode(i));
}

void CExportTMF::InitDATA()
{
	LISTINODEDATA::iterator itINODE;

	m_vIBONES.sort(name_less());
	BYTE bCount = 0;

	for( itINODE = m_vIBONES.begin(); itINODE != m_vIBONES.end(); itINODE++)
	{
		(*itINODE)->m_bINDEX = bCount;
		bCount++;
	}

	for( int i=0; i<INT(m_vIDATA.size()); i++)
	{
		BuildMESHDATA(m_vIDATA[i]);
		BuildWEIGHT(m_vIDATA[i]);
	}
}

FLOAT CExportTMF::InitMESH( LPD3DXVECTOR3 pCENTER)
{
	FLOAT fRESULT = 0.0f;

	for( int i=0; i<INT(m_vIDATA.size()); i++)
		BuildMESH(m_vIDATA[i]);
	m_vIMESH.sort(mtrl_less());

	pCENTER->x = 0.0f;
	pCENTER->y = 0.0f;
	pCENTER->z = 0.0f;

	if(!m_vVB.empty())
	{
		int nCount = INT(m_vVB.size());

		for( i=0; i<nCount; i++)
		{
			pCENTER->x += m_vVB[i]->m_fPosX;
			pCENTER->y += m_vVB[i]->m_fPosY;
			pCENTER->z += m_vVB[i]->m_fPosZ;
		}
		(*pCENTER) /= nCount;

		for( i=0; i<nCount; i++)
		{
			FLOAT fLocal = D3DXVec3Length(&D3DXVECTOR3(
				m_vVB[i]->m_fPosX - pCENTER->x,
				m_vVB[i]->m_fPosY - pCENTER->y,
				m_vVB[i]->m_fPosZ - pCENTER->z));

			if( fRESULT < fLocal )
				fRESULT = fLocal;
		}
	}

	return fRESULT;
}

void CExportTMF::ExportBONES( LPINODEDATA pINODE,
							  FILE *pFILE)
{
	D3DXMATRIX vMAT = GetD3DMatrix(pINODE->m_pINODE->GetObjectTM(m_dwSTART));

	vMAT._41 *= m_fSCALE;
	vMAT._42 *= m_fSCALE;
	vMAT._43 *= m_fSCALE;

	fwrite( &vMAT, sizeof(D3DXMATRIX), 1, pFILE);
}

void CExportTMF::CheckBONES( HWND hWnd)
{
	LISTINODEDATA::iterator itINODE;

	MAPSTRING mapSTRING;
	mapSTRING.clear();

	for( itINODE = m_vIBONES.begin(); itINODE != m_vIBONES.end(); itINODE++)
		if((*itINODE)->m_pINODE)
		{
			CString strNAME((*itINODE)->m_pINODE->GetName());

			if( mapSTRING.find(strNAME) != mapSTRING.end() )
			{
				CString strMSG;

				strMSG.Format( IDS_FMT_DUP_BONES_NAME, strNAME);
				MessageBox( hWnd, strMSG, _T("ExportTMF"), MB_OK);

				break;
			}

			mapSTRING.insert( MAPSTRING::value_type( strNAME, 0));
		}

	mapSTRING.clear();
}

void CExportTMF::CheckMTRL( HWND hWnd)
{
	LISTMESHNODE::iterator itIMESH;

	MAPSTRING mapSTRING;
	mapSTRING.clear();

	for( itIMESH = m_vIMESH.begin(); itIMESH != m_vIMESH.end(); itIMESH++)
		if((*itIMESH)->m_pMTRL)
		{
			CString strNAME((*itIMESH)->m_pMTRL->GetName());

			if( mapSTRING.find(strNAME) != mapSTRING.end() )
			{
				CString strMSG;

				strMSG.Format( IDS_FMT_DUP_MTRL_NAME, strNAME);
				MessageBox( hWnd, strMSG, _T("ExportTMF"), MB_OK);

				break;
			}

			mapSTRING.insert( MAPSTRING::value_type( strNAME, 0));
		}

	for( itIMESH = m_vIMESH.begin(); itIMESH != m_vIMESH.end(); itIMESH++)
		if((*itIMESH)->m_pMTRL)
		{
			Shader *pTSH = ((StdMat2 *) (*itIMESH)->m_pMTRL)->GetShader();

			if( !pTSH || pTSH->ClassID() != TachyonShader_CLASS_ID )
			{
				CString strMSG;

				strMSG.Format( IDS_FMT_INVALID_SHADER, (*itIMESH)->m_pMTRL->GetName());
				MessageBox( hWnd, strMSG, _T("ExportTMF"), MB_OK);

				break;
			}
		}

	mapSTRING.clear();
}

int	CExportTMF::DoExport( const TCHAR *name,
						  ExpInterface *ei,
						  Interface *i,
						  BOOL suppressPrompts,
						  DWORD options)
{
	HWND hWnd = i->GetMAXHWnd();

	if( !DialogBoxParam(
		hInstance,
		MAKEINTRESOURCE(IDD_PANEL_TMF),
		hWnd,
		ExportTMFProc,
		(LPARAM) this))
		return 1;

	Interval vAniRange = i->GetAnimRange();
	INode *pROOT = i->GetRootNode();
	int nRESULT = FALSE;

	m_dwSTART = vAniRange.Start();
	ClearDATA();

	m_bExportAll = (options != SCENE_EXPORT_SELECTED);
	if(pROOT)
	{
		LISTINODEDATA::iterator itINODE;
		LISTMESHNODE::iterator itIMESH;
		FILE *pFILE = fopen( name, "wb");

		InitDATA(pROOT);
		InitDATA();

		D3DXVECTOR3 vCENTER( 0.0f, 0.0f, 0.0f);
		FLOAT fRADIUS = InitMESH(&vCENTER);

		DWORD dwCount = DWORD(m_vIBONES.size());
		int nTMFVersion = TMF_VERSION;

		fwrite( &nTMFVersion, sizeof(int), 1, pFILE);
		fwrite( &dwCount, sizeof(DWORD), 1, pFILE);
		fwrite( &fRADIUS, sizeof(FLOAT), 1, pFILE);
		fwrite( &vCENTER, sizeof(D3DXVECTOR3), 1, pFILE);

		for( itINODE = m_vIBONES.begin(); itINODE != m_vIBONES.end(); itINODE++)
			ExportBONES( *itINODE, pFILE);

		dwCount = DWORD(m_vVB.size());
		fwrite( &dwCount, sizeof(DWORD), 1, pFILE);

		for( DWORD i=0; i<dwCount; i++)
			fwrite( m_vVB[i], sizeof(TMESHBUFFER), 1, pFILE);

		dwCount = DWORD(m_vIMESH.size());
		fwrite( &dwCount, sizeof(DWORD), 1, pFILE);

		for( itIMESH = m_vIMESH.begin(); itIMESH != m_vIMESH.end(); itIMESH++)
		{
			dwCount = DWORD((*itIMESH)->m_vMESH.size());
			fwrite( &dwCount, sizeof(DWORD), 1, pFILE);

			for( DWORD i=0; i<dwCount; i++)
				fwrite( &(*itIMESH)->m_vMESH[i], sizeof(WORD), 1, pFILE);
		}

		nRESULT = TRUE;
		fclose(pFILE);
	}

	CheckBONES(hWnd);
	CheckMTRL(hWnd);
	ClearDATA();

	return nRESULT;
}
