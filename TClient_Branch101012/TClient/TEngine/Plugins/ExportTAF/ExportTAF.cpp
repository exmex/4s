/**********************************************************************
 *<
	FILE: ExportTAF.cpp

	DESCRIPTION: Tachyon Engine Animation Data Export Class Cpp File

	CREATED BY: Hyunho, Shin

	HISTORY: Created At 2002/2/20

 *>	Copyright (c) Tachyon Entertainment, All Rights Reserved.
 **********************************************************************/

#pragma warning( disable : 4786 4530 )

#include "ExportTAF.h"
#include "BipedApi.h"
#include "Tracks.h"
#include "ExportComm.h"

#define CEXPORTTAF_CLASS_ID						Class_ID( 0x1f7fdcae, 0xac7d1b7)
#define TAF_VERSION								301


class CExportTAF : public SceneExport
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

	CExportTAF();
	~CExportTAF();

protected:
	// Utility member function
	LPINODEDATA FindINODEDATA( INode *pNODE);
	D3DXMATRIX GetD3DMatrix( Matrix3 vMAT);

	D3DXQUATERNION InverseQUAT( D3DXQUATERNION vQUAT);
	D3DXMATRIX InverseMAT( D3DXMATRIX vMAT);
	D3DXMATRIX NoTrans( D3DXMATRIX vMAT);

	BYTE IsHelperBONE( INode *pNODE);
	BYTE IsBONE( INode *pNODE);

	IBipedKey GetBipedKey(
		IKeyControl *pIKEY,
		int nID,
		int nIndex);

	IKey GetControlKey(
		IKeyControl *pIKEY,
		int nIndex);

	D3DXMATRIX GetPosParentTM(
		LPINODEDATA pINODE,
		DWORD dwTIME);

	D3DXMATRIX GetRotParentTM(
		LPINODEDATA pINODE,
		DWORD dwTIME);

	D3DXMATRIX GetScaleParentTM(
		LPINODEDATA pINODE,
		DWORD dwTIME);

	D3DXVECTOR3 GetLocalPOS(
		LPINODEDATA pINODE,
		DWORD dwTIME);

	D3DXQUATERNION GetLocalROT(
		LPINODEDATA pINODE,
		DWORD dwTIME);

	D3DXVECTOR3 GetLocalSCALE(
		LPINODEDATA pINODE,
		DWORD dwTIME);

	void TransformInverse(
		LPTMESHBUFFER pPOINT,
		D3DXMATRIX vMAT);

	void InitBONESNODE(
		LPINODEDATA pINODE,
		LPBONESNODE pNODE);

	void BuildBONESKEY(
		LPMAPIKEY pKEY,
		IKeyControl *pIKEY);

	void BuildBIPEDKEY(
		LPINODEDATA pINODE,
		Control *pControl);

	void BuildBIPEDKEY(
		LPINODEDATA pINODE,
		Control *pControl,
		IKeyControl *pIKEY,
		int nID);
	void CheckBONES( HWND hWnd);

protected:
	// Initialize member function
	void InitDATA( INode *pNODE);
	void InitINDEX();
	void ClearDATA();

protected:
	// Export member function
	void ExportBONES(
		LPINODEDATA pINODE,
		FILE *pFILE);

	void ExportMESH(
		LPINODEDATA pINODE,
		FILE *pFILE);

	void ExportBONESNODE(
		LPINODEDATA pINODE,
		FILE *pFILE);

	void ExportFRAME(
		LPINODEDATA pINODE,
		FILE *pFILE);

	void ExportPOS(
		LPINODEDATA pINODE,
		FILE *pFILE);

	void ExportROT(
		LPINODEDATA pINODE,
		FILE *pFILE);

	void ExportSCALE(
		LPINODEDATA pINODE,
		FILE *pFILE);

public:
	FLOAT m_fSCALE;
	BYTE m_bSAMPLE;

	int m_nRATE;

protected:
	LISTINODEDATA m_vINODE;
	MAPIKEY m_mapKEY;

	DWORD m_dwSTART;
	DWORD m_dwEND;
};


class CExportTAFClassDesc : public ClassDesc2
{
public:
	int IsPublic() { return TRUE;}
	void *Create( BOOL loading = FALSE) { return new CExportTAF();}
	const TCHAR *ClassName() { return GetString(IDS_CLASS_NAME);}
	SClass_ID SuperClassID() { return SCENE_EXPORT_CLASS_ID;}
	Class_ID ClassID() { return CEXPORTTAF_CLASS_ID;}
	const TCHAR *Category() { return GetString(IDS_CATEGORY);}
	const TCHAR *InternalName() { return _T("CExportTAF");}
	HINSTANCE HInstance() { return hInstance;}
};


static CExportTAFClassDesc CExportTAFDesc;
ClassDesc2* GetCExportTAFDesc() { return &CExportTAFDesc;}

static int CALLBACK ExportTAFProc( HWND hWnd,
								   UINT msg,
								   WPARAM wParam,
								   LPARAM lParam)
{
	ISpinnerControl* pSpinCtrl;

	CExportTAF* pExpTAF = (CExportTAF*) GetWindowLongPtr( hWnd, GWLP_USERDATA);
	switch(msg)
	{
	case WM_INITDIALOG		:
		{
			pExpTAF = (CExportTAF*) lParam;
			SetWindowLongPtr( hWnd, GWLP_USERDATA, lParam); 
			CenterWindow( hWnd, GetParent(hWnd));

			pSpinCtrl = GetISpinner( GetDlgItem( hWnd, IDC_SPIN_FRAME));
			pSpinCtrl->LinkToEdit( GetDlgItem( hWnd, IDC_EDIT_FRAME), EDITTYPE_INT);
			pSpinCtrl->SetLimits( 1, 1000, TRUE);
			pSpinCtrl->SetAutoScale(TRUE);
			pSpinCtrl->SetValue( pExpTAF->m_nRATE, FALSE);
			ReleaseISpinner(pSpinCtrl);

			pSpinCtrl = GetISpinner( GetDlgItem( hWnd, IDC_SPIN_SCALE));
			pSpinCtrl->LinkToEdit( GetDlgItem( hWnd, IDC_EDIT_SCALE), EDITTYPE_FLOAT);
			pSpinCtrl->SetLimits( 0.0f, 1000.0f, TRUE);
			pSpinCtrl->SetAutoScale(TRUE);
			pSpinCtrl->SetValue( pExpTAF->m_fSCALE, FALSE);
			ReleaseISpinner(pSpinCtrl);

			SendMessage(
				GetDlgItem( hWnd, IDC_CHECK_SAMPLING),
				BM_SETCHECK,
				pExpTAF->m_bSAMPLE ? BST_CHECKED : BST_UNCHECKED, 0);
		}

		break;

	case CC_SPINNER_CHANGE	: pSpinCtrl = (ISpinnerControl*) lParam; break;
	case WM_COMMAND			:
		switch(LOWORD(wParam))
		{
		case IDC_BTN_TAFEXPORT	:
			{
				BOOL bResult = FALSE;

				pSpinCtrl = GetISpinner( GetDlgItem( hWnd, IDC_SPIN_FRAME));
				pExpTAF->m_nRATE = pSpinCtrl->GetFVal();
				ReleaseISpinner(pSpinCtrl);

				pSpinCtrl = GetISpinner( GetDlgItem( hWnd, IDC_SPIN_SCALE));
				pExpTAF->m_fSCALE = pSpinCtrl->GetFVal();
				ReleaseISpinner(pSpinCtrl);

				pExpTAF->m_bSAMPLE = SendMessage(
					GetDlgItem( hWnd, IDC_CHECK_SAMPLING),
					BM_GETCHECK,
					0, 0) == BST_CHECKED ? TRUE : FALSE;
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

BOOL EqualQuat( D3DXQUATERNION q1, D3DXQUATERNION q2)
{
	Quat vResult = Quat( q1.x, q1.z, q1.y, q1.w) / Quat( q2.x, q2.z, q2.y, q2.w);

	if( fabs(vResult.x) > ALMOST_ZERO )
		return FALSE;

	if( fabs(vResult.y) > ALMOST_ZERO )
		return FALSE;

	if( fabs(vResult.z) > ALMOST_ZERO )
		return FALSE;

	if( fabs(vResult.w - 1.0f) > ALMOST_ZERO )
		return FALSE;

	return TRUE;
}

BOOL EqualPoint3( D3DXVECTOR3 p1, D3DXVECTOR3 p2)
{
	if( fabs( p1.x - p2.x ) > ALMOST_ZERO )
		return FALSE;

	if( fabs( p1.y - p2.y ) > ALMOST_ZERO )
		return FALSE;

	if( fabs( p1.z - p2.z ) > ALMOST_ZERO )
		return FALSE;

	return TRUE;
}

struct name_less : public binary_function< LPINODEDATA, LPINODEDATA, bool>
{
	bool operator ()( const LPINODEDATA& _Left, const LPINODEDATA& _Right) const
	{
		return strcmp( _Left->m_pINODE->GetName(), _Right->m_pINODE->GetName()) < 0 ? true : false;
	};
};

struct node_less : public binary_function< LPINODEDATA, LPINODEDATA, bool>
{
	bool operator ()( const LPINODEDATA& _Left, const LPINODEDATA& _Right) const
	{
		return _Left->m_bNODE < _Right->m_bNODE ? true : false;
	};
};

//--- CExportTAF -------------------------------------------------------
CExportTAF::CExportTAF()
{
	m_mapKEY.clear();
	m_vINODE.clear();

	m_bSAMPLE = FALSE;
	m_fSCALE = 0.01f;
	m_nRATE = 1;

	m_dwSTART = 0;
	m_dwEND = 0;
}

CExportTAF::~CExportTAF()
{
	ClearDATA();
}

int CExportTAF::ExtCount()
{
	return 1;
}

const TCHAR *CExportTAF::Ext( int n)
{
	return _T("TAF");
}

const TCHAR *CExportTAF::LongDesc()
{
	return _T("Tachyon Engine Animation Data File");
}

const TCHAR *CExportTAF::ShortDesc()
{
	return _T("Tachyon Engine Animation Data File");
}

const TCHAR *CExportTAF::AuthorName()
{
	return _T("Hyunho, Shin");
}

const TCHAR *CExportTAF::CopyrightMessage()
{
	return _T("");
}

const TCHAR *CExportTAF::OtherMessage1()
{
	return _T("");
}

const TCHAR *CExportTAF::OtherMessage2()
{
	return _T("");
}

unsigned int CExportTAF::Version()
{
	return 100;
}

void CExportTAF::ShowAbout(HWND hWnd)
{
}

BOOL CExportTAF::SupportsOptions( int ext, DWORD options)
{
	return TRUE;
}

D3DXMATRIX CExportTAF::GetD3DMatrix( Matrix3 vMAT)
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

D3DXMATRIX CExportTAF::NoTrans( D3DXMATRIX vMAT)
{
	vMAT._41 = 0.0f;
	vMAT._42 = 0.0f;
	vMAT._43 = 0.0f;

	return vMAT;
}

D3DXMATRIX CExportTAF::InverseMAT( D3DXMATRIX vMAT)
{
	D3DXMATRIX vRESULT;

	D3DXMatrixInverse(
		&vRESULT,
		NULL,
		&vMAT);

	return vRESULT;
}

D3DXQUATERNION CExportTAF::InverseQUAT( D3DXQUATERNION vQUAT)
{
	D3DXQUATERNION vRESULT;

	D3DXQuaternionInverse(
		&vRESULT,
		&vQUAT);

	return vRESULT;
}

void CExportTAF::ClearDATA()
{
	MAPIKEY::iterator itKEY;

	for( itKEY = m_mapKEY.begin(); itKEY != m_mapKEY.end(); itKEY++)
		delete (*itKEY).second;
	m_mapKEY.clear();

	while(!m_vINODE.empty())
	{
		delete m_vINODE.back();
		m_vINODE.pop_back();
	}
}

BYTE CExportTAF::IsBONE( INode *pNODE)
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

BYTE CExportTAF::IsHelperBONE( INode *pNODE)
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

void CExportTAF::TransformInverse( LPTMESHBUFFER pPOINT,
								   D3DXMATRIX vMAT)
{
	D3DXVECTOR3 vPOS(
		pPOINT->m_fPosX,
		pPOINT->m_fPosY,
		pPOINT->m_fPosZ);

	D3DXVECTOR3 vNORMAL(
		pPOINT->m_fNormalX,
		pPOINT->m_fNormalY,
		pPOINT->m_fNormalZ);
	D3DXVECTOR4 vRESULT;

	D3DXVec3Transform(
		&vRESULT,
		&vPOS,
		&InverseMAT(vMAT));

	pPOINT->m_fPosX = vRESULT.x;
	pPOINT->m_fPosY = vRESULT.y;
	pPOINT->m_fPosZ = vRESULT.z;

	vMAT._41 = 0.0f;
	vMAT._42 = 0.0f;
	vMAT._43 = 0.0f;
	vMAT._44 = 1.0f;

	D3DXVec3Transform(
		&vRESULT,
		&vNORMAL,
		&InverseMAT(vMAT));

	pPOINT->m_fNormalX = vRESULT.x;
	pPOINT->m_fNormalY = vRESULT.y;
	pPOINT->m_fNormalZ = vRESULT.z;
}

LPINODEDATA CExportTAF::FindINODEDATA( INode *pNODE)
{
	LISTINODEDATA::iterator itINODE;

	for( itINODE = m_vINODE.begin(); itINODE != m_vINODE.end(); itINODE++)
		if( (*itINODE)->m_pINODE == pNODE )
			return (*itINODE);

	return NULL;
}

void CExportTAF::BuildBONESKEY( LPMAPIKEY pKEY,
								IKeyControl *pIKEY)
{
	int nCount = pIKEY->GetNumKeys();

	for( int i=0; i<nCount; i++)
	{
		IKey vIKEY = GetControlKey( pIKEY, i);

		if( m_dwSTART <= vIKEY.time && vIKEY.time <= m_dwEND )
		{
			MAPIKEY::iterator finder = pKEY->find(DWORD(vIKEY.time));

			if( finder == pKEY->end() )
			{
				IKey *pVALUE = new IKey();

				pVALUE->flags = vIKEY.flags;
				pVALUE->time = vIKEY.time;

				pKEY->insert( MAPIKEY::value_type( DWORD(vIKEY.time), pVALUE));
			}
		}
	}
}

void CExportTAF::BuildBIPEDKEY( LPINODEDATA pINODE,
							    Control *pControl)
{
	if( pControl->ClassID() == BIPSLAVE_CONTROL_CLASS_ID )
	{
		IBipMaster *pMASTER = GetBipMasterInterface(pControl);

		int nLINK;
		int nID;

		if( !pMASTER || !pMASTER->GetIdLink( pINODE->m_pINODE, nID, nLINK) )
			return;

		IKeyControl *pIKEY = GetKeyControlInterface(pControl);
		if(!pIKEY)
			return;

		BuildBIPEDKEY(
			pINODE,
			pControl,
			pIKEY,
			nID);
	}
	else if( pControl->ClassID() == BIPBODY_CONTROL_CLASS_ID )
	{
		IBipMaster *pMASTER = GetBipMasterInterface(pControl);

		if(!pMASTER)
			return;

		pControl = pMASTER->GetHorizontalControl();
		if(pControl)
		{
			IKeyControl *pIKEY = GetKeyControlInterface(pControl);

			if(pIKEY)
			{
				BuildBIPEDKEY(
					pINODE,
					pControl,
					pIKEY,
					KEY_HORIZONTAL);
			}
		}

		pControl = pMASTER->GetVerticalControl();
		if(pControl)
		{
			IKeyControl *pIKEY = GetKeyControlInterface(pControl);

			if(pIKEY)
			{
				BuildBIPEDKEY(
					pINODE,
					pControl,
					pIKEY,
					KEY_VERTICAL);
			}
		}

		pControl = pMASTER->GetTurnControl();
		if(pControl)
		{
			IKeyControl *pIKEY = GetKeyControlInterface(pControl);

			if(pIKEY)
			{
				BuildBIPEDKEY(
					pINODE,
					pControl,
					pIKEY,
					KEY_TURN);
			}
		}
	}
}

void CExportTAF::BuildBIPEDKEY( LPINODEDATA pINODE,
								Control *pControl,
								IKeyControl *pIKEY,
								int nID)
{
	int nCount = pIKEY->GetNumKeys();

	for( int i=0; i<nCount; i++)
	{
		IBipedKey vTCB = GetBipedKey( pIKEY, nID, i);

		if( m_dwSTART <= vTCB.time && vTCB.time <= m_dwEND )
		{
			MAPIKEY::iterator finder = m_mapKEY.find(DWORD(vTCB.time));
			IKey *pTCB = NULL;

			if( finder == m_mapKEY.end() )
			{
				pTCB = new IBipedKey();

				m_mapKEY.insert( MAPIKEY::value_type(
					DWORD(vTCB.time),
					pTCB));
			}
			else
				pTCB = (*finder).second;
			(*pTCB) = vTCB;
		}
	}
}

IBipedKey CExportTAF::GetBipedKey( IKeyControl *pIKEY,
								   int nID,
								   int nIndex)
{
	IBipedKey *pKEY = NULL;
	IBipedKey vKEY;

	IBipedVertKey vKeyV;
	IBipedHorzKey vKeyH;
	IBipedTurnKey vKeyR;
	IBipedBodyKey vKeyB;
	IBipedHeadKey vKeyD;
	IBipedPropKey vKeyP;

	vKEY.easeOut = 0.0f;
	vKEY.easeIn = 0.0f;
	vKEY.tens = 0.0f;
	vKEY.cont = 0.0f;
	vKEY.bias = 0.0f;

	switch(nID)
	{
	case KEY_VERTICAL	: pKEY = &vKeyV; break;
	case KEY_HORIZONTAL	: pKEY = &vKeyH; break;
	case KEY_TURN		: pKEY = &vKeyR; break;
	case KEY_LARM		:
	case KEY_RARM		:
	case KEY_LHAND		:
	case KEY_RHAND		:
	case KEY_LLEG		:
	case KEY_RLEG		:
	case KEY_LFOOT		:
	case KEY_RFOOT		: pKEY = &vKeyB; break;
	case KEY_HEAD		: pKEY = &vKeyD; break;
	case KEY_PROP1		:
	case KEY_PROP2		:
	case KEY_PROP3		: pKEY = &vKeyP; break;
	}

	if(pKEY)
	{
		pIKEY->GetKey( nIndex, pKEY);

		vKEY.easeOut = pKEY->easeOut;
		vKEY.easeIn = pKEY->easeIn;

		vKEY.time = pKEY->time;
		vKEY.type = pKEY->type;

		vKEY.tens = (pKEY->tens - 25.0f) / 25.0f;
		vKEY.cont = (pKEY->cont - 25.0f) / 25.0f;
		vKEY.bias = (pKEY->bias - 25.0f) / 25.0f;
	}

	return vKEY;
}

IKey CExportTAF::GetControlKey( IKeyControl *pIKEY,
							    int nIndex)
{
	LPBYTE pBUF = new BYTE[pIKEY->GetKeySize()];
	IKey *pKEY = (IKey *) pBUF;
	IKey vKEY;

	pIKEY->GetKey( nIndex, pKEY);
	vKEY.flags = pKEY->flags;
	vKEY.time = pKEY->time;
	delete[] pBUF;

	return vKEY;
}

D3DXMATRIX CExportTAF::GetPosParentTM( LPINODEDATA pINODE,
									   DWORD dwTIME)
{
	Control *pControl = pINODE->m_pINODE->GetTMController();
	INode *pPARENT = pINODE->m_pINODE->GetParentNode();
	Matrix3 vRESULT;

	if( pControl && pControl->ClassID() == BIPSLAVE_CONTROL_CLASS_ID )
	{
		IBipMaster *pMASTER = GetBipMasterInterface(pControl);

		int nLINK;
		int nID;

		if( pMASTER && pMASTER->GetIdLink( pINODE->m_pINODE, nID, nLINK) )
			pPARENT = pMASTER->GetPosParentNode( nID, nLINK);
	}
	vRESULT.IdentityMatrix();

	if( pPARENT && IsBONE(pPARENT) )
		vRESULT = pPARENT->GetNodeTM(dwTIME);

	return GetD3DMatrix(vRESULT);
}

D3DXMATRIX CExportTAF::GetRotParentTM( LPINODEDATA pINODE,
									   DWORD dwTIME)
{
	Control *pControl = pINODE->m_pINODE->GetTMController();
	INode *pPARENT = pINODE->m_pINODE->GetParentNode();
	Matrix3 vRESULT;

	if( pControl && pControl->ClassID() == BIPSLAVE_CONTROL_CLASS_ID )
	{
		IBipMaster *pMASTER = GetBipMasterInterface(pControl);

		int nLINK;
		int nID;

		if( pMASTER && pMASTER->GetIdLink( pINODE->m_pINODE, nID, nLINK) )
			pPARENT = pMASTER->GetRotParentNode( nID, nLINK);
	}
	vRESULT.IdentityMatrix();

	if( pPARENT && IsBONE(pPARENT) )
		vRESULT = pPARENT->GetNodeTM(dwTIME);

	return GetD3DMatrix(vRESULT);
}

D3DXMATRIX CExportTAF::GetScaleParentTM( LPINODEDATA pINODE,
										 DWORD dwTIME)
{
	INode *pPARENT = pINODE->m_pINODE->GetParentNode();
	Matrix3 vRESULT;
	vRESULT.IdentityMatrix();

	if( pPARENT && IsBONE(pPARENT) )
		vRESULT = pPARENT->GetNodeTM(dwTIME);

	return GetD3DMatrix(vRESULT);
}

D3DXVECTOR3 CExportTAF::GetLocalPOS( LPINODEDATA pINODE,
									 DWORD dwTIME)
{
	D3DXMATRIX vMAT = NoTrans(InverseMAT(GetD3DMatrix(pINODE->m_pINODE->GetNodeTM(m_dwSTART))));

	vMAT *= GetD3DMatrix(pINODE->m_pINODE->GetNodeTM(dwTIME));
	vMAT *= InverseMAT(GetPosParentTM( pINODE, dwTIME));
	vMAT *= NoTrans(GetPosParentTM( pINODE, m_dwSTART));

	D3DXQUATERNION vROT;
	D3DXVECTOR3 vSCALE;
	D3DXVECTOR3 vPOS;

	D3DXMatrixDecompose(
		&vSCALE,
		&vROT,
		&vPOS,
		&vMAT);

	return vPOS;
}

D3DXQUATERNION CExportTAF::GetLocalROT( LPINODEDATA pINODE,
									    DWORD dwTIME)
{
	D3DXMATRIX vMAT = InverseMAT(GetD3DMatrix(pINODE->m_pINODE->GetNodeTM(m_dwSTART)));

	vMAT *= GetD3DMatrix(pINODE->m_pINODE->GetNodeTM(dwTIME));
	vMAT *= InverseMAT(GetRotParentTM( pINODE, dwTIME));
	vMAT *= GetRotParentTM( pINODE, m_dwSTART);

	D3DXQUATERNION vROT;
	D3DXVECTOR3 vSCALE;
	D3DXVECTOR3 vPOS;

	D3DXMatrixDecompose(
		&vSCALE,
		&vROT,
		&vPOS,
		&vMAT);

	return vROT;
}

D3DXVECTOR3 CExportTAF::GetLocalSCALE( LPINODEDATA pINODE,
									   DWORD dwTIME)
{
	D3DXMATRIX vMAT = InverseMAT(GetD3DMatrix(pINODE->m_pINODE->GetNodeTM(m_dwSTART)));

	vMAT *= GetD3DMatrix(pINODE->m_pINODE->GetNodeTM(dwTIME));
	vMAT *= InverseMAT(GetScaleParentTM( pINODE, dwTIME));
	vMAT *= GetScaleParentTM( pINODE, m_dwSTART);

	D3DXQUATERNION vROT;
	D3DXVECTOR3 vSCALE;
	D3DXVECTOR3 vPOS;

	D3DXMatrixDecompose(
		&vSCALE,
		&vROT,
		&vPOS,
		&vMAT);

	return vSCALE;
}

void CExportTAF::InitBONESNODE( LPINODEDATA pINODE,
							    LPBONESNODE pNODE)
{
	Control *pControl = pINODE->m_pINODE->GetTMController();
	INode *pParentROT = pINODE->m_pINODE->GetParentNode();
	INode *pParentPOS = pINODE->m_pINODE->GetParentNode();

	pNODE->m_bINDEX = pINODE->m_bINDEX;
	pNODE->m_bParentROT = 0;
	pNODE->m_bParentPOS = 0;

	if( pControl && pControl->ClassID() == BIPSLAVE_CONTROL_CLASS_ID )
	{
		IBipMaster *pMASTER = GetBipMasterInterface(pControl);

		int nLINK;
		int nID;

		if( pMASTER && pMASTER->GetIdLink( pINODE->m_pINODE, nID, nLINK) )
		{
			pParentROT = pMASTER->GetRotParentNode( nID, nLINK);
			pParentPOS = pMASTER->GetPosParentNode( nID, nLINK);
		}
	}

	if( pParentROT && IsBONE(pParentROT) )
	{
		LPINODEDATA pIROT = FindINODEDATA(pParentROT);

		if(pIROT)
			pNODE->m_bParentROT = pIROT->m_bINDEX + 1;
	}

	if( pParentPOS && IsBONE(pParentPOS) )
	{
		LPINODEDATA pIPOS = FindINODEDATA(pParentPOS);

		if(pIPOS)
			pNODE->m_bParentPOS = pIPOS->m_bINDEX + 1;
	}
}

void CExportTAF::InitDATA( INode *pNODE)
{
	if(IsBONE(pNODE))
	{
		LPINODEDATA pIDATA = new INODEDATA();

		pIDATA->m_pINODE = pNODE;
		pIDATA->m_bINDEX = 0;
		pIDATA->m_bNODE = INT(m_vINODE.size());

		m_vINODE.push_back(pIDATA);
	}

	int nCount = pNODE->NumberOfChildren();
	for( int i=0; i<nCount; i++)
		InitDATA(pNODE->GetChildNode(i));
}

void CExportTAF::InitINDEX()
{
	LISTINODEDATA::iterator itINODE;
	BYTE bCount = 0;
	m_vINODE.sort(name_less());

	for( itINODE = m_vINODE.begin(); itINODE != m_vINODE.end(); itINODE++)
	{
		(*itINODE)->m_bINDEX = bCount;
		bCount++;
	}
}

void CExportTAF::ExportBONES( LPINODEDATA pINODE,
							  FILE *pFILE)
{
	D3DXMATRIX vMAT = GetD3DMatrix(pINODE->m_pINODE->GetObjectTM(m_dwSTART));

	vMAT._41 *= m_fSCALE;
	vMAT._42 *= m_fSCALE;
	vMAT._43 *= m_fSCALE;

	fwrite( &vMAT, sizeof(D3DXMATRIX), 1, pFILE);
}

void CExportTAF::ExportMESH( LPINODEDATA pINODE,
							 FILE *pFILE)
{
	ObjectState vSTATE = pINODE->m_pINODE->EvalWorldState(m_dwSTART);
	DWORD dwVertexCount = 0;
	BYTE bEXPORT = FALSE;

	if( vSTATE.obj && vSTATE.obj->SuperClassID() != HELPER_CLASS_ID )
	{
		TriObject *pTOBJ = (TriObject *) vSTATE.obj->ConvertToType(
			0, Class_ID( TRIOBJ_CLASS_ID, 0));

		if(pTOBJ)
		{
			Matrix3 vNodeTM = pINODE->m_pINODE->GetObjectTM(m_dwSTART);
			Mesh *pMESH = &(pTOBJ->GetMesh());

			if(pMESH)
			{
				D3DXMATRIX vMAT = GetD3DMatrix(vNodeTM);
				Face *pFACE = pMESH->faces;

				vMAT._41 *= m_fSCALE;
				vMAT._42 *= m_fSCALE;
				vMAT._43 *= m_fSCALE;

				if(pFACE)
				{
					DWORD dwFaceCount = DWORD(pMESH->getNumFaces());

					dwVertexCount = 3 * dwFaceCount;
					bEXPORT = TRUE;
					fwrite( &dwVertexCount, sizeof(DWORD), 1, pFILE);

					for( DWORD i=0; i<dwFaceCount; i++)
					{
						D3DXVECTOR3 vPOLY[3];
						D3DXVECTOR3 vNORMAL;

						for( BYTE j=0; j<3; j++)
						{
							Point3& vPOINT = pMESH->getVert(pFACE[i].v[2 - j]) * vNodeTM;

							vPOLY[j].x = vPOINT.x;
							vPOLY[j].y = vPOINT.z;
							vPOLY[j].z = vPOINT.y;
						}

						D3DXVec3Cross(
							&vNORMAL,
							&(vPOLY[1] - vPOLY[0]),
							&(vPOLY[2] - vPOLY[0]));
						vNORMAL /= D3DXVec3Length(&vNORMAL);

						for( j=0; j<3; j++)
						{
							TMESHBUFFER vMESH = {
								m_fSCALE * vPOLY[j].x,
								m_fSCALE * vPOLY[j].y,
								m_fSCALE * vPOLY[j].z,
								1.0f,
								0.0f,
								0.0f,
								pINODE->m_bINDEX + 1,
								vNORMAL.x,
								vNORMAL.y,
								vNORMAL.z,
								0.0f, 0.0f,
								0.0f, 0.0f};
							TransformInverse( &vMESH, vMAT);

							fwrite( &vMESH, sizeof(TMESHBUFFER), 1, pFILE);
						}
					}
				}
			}
		}
	}

	if(!bEXPORT)
		fwrite( &dwVertexCount, sizeof(DWORD), 1, pFILE);
}

void CExportTAF::ExportBONESNODE( LPINODEDATA pINODE,
								  FILE *pFILE)
{
	BONESNODE vNODE;

	InitBONESNODE(
		pINODE,
		&vNODE);

	fwrite( &vNODE, sizeof(BONESNODE), 1, pFILE);
}

void CExportTAF::ExportFRAME( LPINODEDATA pINODE,
							  FILE *pFILE)
{
	ExportPOS( pINODE, pFILE);
	ExportROT( pINODE, pFILE);
	ExportSCALE( pINODE, pFILE);
}

void CExportTAF::ExportPOS( LPINODEDATA pINODE,
						    FILE *pFILE)
{
	if( m_dwSTART >= m_dwEND )
	{
		int nCount = 0;

		fwrite( &nCount, sizeof(int), 1, pFILE);
		return;
	}
	Control *pControl = pINODE->m_pINODE->GetTMController();

	if(!pControl)
	{
		int nCount = 0;

		fwrite( &nCount, sizeof(int), 1, pFILE);
		return;
	}

	if(m_bSAMPLE)
	{
		int nKeyCount = (m_dwEND - m_dwSTART) / (GetTicksPerFrame() * m_nRATE) + 1;
		int nCount = 1;

		if(!((m_dwEND - m_dwSTART) % (GetTicksPerFrame() * m_nRATE)))
			nKeyCount--;
		LPTPOINTKEY pDATA = new TPOINTKEY[nKeyCount + 1];

		pDATA[0].m_vKeyPoint = m_fSCALE * GetLocalPOS(
				pINODE,
				m_dwSTART);
		pDATA[0].m_dwTime = m_dwSTART;

		for( int i=0; i<nKeyCount - 1; i++)
		{
			DWORD dwTIME = m_dwSTART + (i + 1) * GetTicksPerFrame() * m_nRATE;

			D3DXVECTOR3 vPOS = m_fSCALE * GetLocalPOS(
				pINODE,
				dwTIME);

			if( nCount < 2 || !EqualPoint3( pDATA[nCount - 1].m_vKeyPoint, vPOS) || !EqualPoint3( pDATA[nCount - 2].m_vKeyPoint, vPOS) )
			{
				pDATA[nCount].m_vKeyPoint = vPOS;
				pDATA[nCount].m_dwTime = dwTIME;
				nCount++;
			}
			else
				pDATA[nCount - 1].m_dwTime = dwTIME;
		}

		D3DXVECTOR3 vPOS = m_fSCALE * GetLocalPOS(
			pINODE,
			m_dwEND);

		if( nCount < 2 || !EqualPoint3( pDATA[nCount - 1].m_vKeyPoint, vPOS) || !EqualPoint3( pDATA[nCount - 2].m_vKeyPoint, vPOS) )
		{
			pDATA[nCount].m_vKeyPoint = vPOS;
			pDATA[nCount].m_dwTime = m_dwEND;
			nCount++;
		}
		else
			pDATA[nCount - 1].m_dwTime = m_dwEND;

		fwrite( &nCount, sizeof(int), 1, pFILE);
		fwrite( pDATA, nCount * sizeof(TPOINTKEY), 1, pFILE);

		delete[] pDATA;
	}
	else if( pControl->ClassID() == BIPSLAVE_CONTROL_CLASS_ID || pControl->ClassID() == BIPBODY_CONTROL_CLASS_ID )
	{
		MAPIKEY::iterator itKEY;

		BuildBIPEDKEY(
			pINODE,
			pControl);

		int nCount = INT(m_mapKEY.size());
		fwrite( &nCount, sizeof(int), 1, pFILE);

		if( nCount > 0 )
			for( itKEY = m_mapKEY.begin(); itKEY != m_mapKEY.end(); itKEY++)
			{
				TPOINTKEY vKEY;

				vKEY.m_vKeyPoint = m_fSCALE * GetLocalPOS(
					pINODE,
					(*itKEY).first);
				vKEY.m_dwTime = (*itKEY).first;

				fwrite( &vKEY, sizeof(TPOINTKEY), 1, pFILE);
			}
	}
	else
	{
		MAPIKEY::iterator itKEY;
		MAPIKEY mapKEY;
		mapKEY.clear();

		pControl = pControl->GetPositionController();
		if(!pControl)
		{
			int nCount = 0;

			fwrite( &nCount, sizeof(int), 1, pFILE);
			return;
		}

		IKeyControl *pIKEY = GetKeyControlInterface(pControl);
		if(!pIKEY)
		{
			int nCount = 0;

			fwrite( &nCount, sizeof(int), 1, pFILE);
			return;
		}

		BuildBONESKEY(
			&mapKEY,
			pIKEY);

		int nCount = INT(mapKEY.size());
		fwrite( &nCount, sizeof(int), 1, pFILE);

		for( itKEY = mapKEY.begin(); itKEY != mapKEY.end(); itKEY++)
		{
			TPOINTKEY vKEY;

			vKEY.m_vKeyPoint = m_fSCALE * GetLocalPOS(
				pINODE,
				(*itKEY).first);
			vKEY.m_dwTime = (*itKEY).first;

			fwrite( &vKEY, sizeof(TPOINTKEY), 1, pFILE);
			delete (*itKEY).second;
		}

		mapKEY.clear();
	}
}

void CExportTAF::ExportROT( LPINODEDATA pINODE,
						    FILE *pFILE)
{
	D3DXQUATERNION vIDENTITY( 0.0f, 0.0f, 0.0f, 1.0f);

	if( m_dwSTART >= m_dwEND )
	{
		int nCount = 0;

		fwrite( &nCount, sizeof(int), 1, pFILE);
		return;
	}
	Control *pControl = pINODE->m_pINODE->GetTMController();

	if(!pControl)
	{
		int nCount = 0;

		fwrite( &nCount, sizeof(int), 1, pFILE);
		return;
	}

	if(m_bSAMPLE)
	{
		int nKeyCount = (m_dwEND - m_dwSTART) / (GetTicksPerFrame() * m_nRATE) + 1;
		int nCount = 1;

		if(!((m_dwEND - m_dwSTART) % (GetTicksPerFrame() * m_nRATE)))
			nKeyCount--;

		LPTROTKEY pDATA = new TROTKEY[nKeyCount + 1];
		D3DXQUATERNION vPREV = GetLocalROT(
			pINODE,
			m_dwSTART);

		pDATA[0].m_vKeyQuat = vIDENTITY;
		pDATA[0].m_dwTime = m_dwSTART;

		for( int i=0; i<nKeyCount - 1; i++)
		{
			DWORD dwTIME = m_dwSTART + (i + 1) * GetTicksPerFrame() * m_nRATE;

			D3DXQUATERNION vNEXT = GetLocalROT(
				pINODE,
				dwTIME);
			D3DXQUATERNION vQUAT = InverseQUAT(vPREV) * vNEXT;

			if( nCount < 2 || !EqualQuat( vQUAT, vIDENTITY) || !EqualQuat( pDATA[nCount - 1].m_vKeyQuat, vIDENTITY) )
			{
				pDATA[nCount].m_vKeyQuat = vQUAT;
				pDATA[nCount].m_dwTime = dwTIME;

				vPREV = vNEXT;
				nCount++;
			}
			else
				pDATA[nCount - 1].m_dwTime = dwTIME;
		}

		D3DXQUATERNION vNEXT = GetLocalROT(
			pINODE,
			m_dwEND);
		D3DXQUATERNION vQUAT = InverseQUAT(vPREV) * vNEXT;

		if( nCount < 2 || !EqualQuat( vQUAT, vIDENTITY) || !EqualQuat( pDATA[nCount - 1].m_vKeyQuat, vIDENTITY) )
		{
			pDATA[nCount].m_vKeyQuat = vQUAT;
			pDATA[nCount].m_dwTime = m_dwEND;
			nCount++;
		}
		else
			pDATA[nCount - 1].m_dwTime = m_dwEND;

		fwrite( &nCount, sizeof(int), 1, pFILE);
		fwrite( pDATA, nCount * sizeof(TROTKEY), 1, pFILE);

		delete[] pDATA;
	}
	else if( pControl->ClassID() == BIPSLAVE_CONTROL_CLASS_ID || pControl->ClassID() == BIPBODY_CONTROL_CLASS_ID )
	{
		MAPIKEY::iterator itKEY;

		BuildBIPEDKEY(
			pINODE,
			pControl);

		int nCount = INT(m_mapKEY.size());
		fwrite( &nCount, sizeof(int), 1, pFILE);

		if( nCount > 0 )
		{
			D3DXQUATERNION vPREV = GetLocalROT(
				pINODE,
				m_dwSTART);

			for( itKEY = m_mapKEY.begin(); itKEY != m_mapKEY.end(); itKEY++)
			{
				D3DXQUATERNION vNEXT = GetLocalROT(
					pINODE,
					(*itKEY).first);
				TROTKEY vKEY;

				vKEY.m_vKeyQuat = InverseQUAT(vPREV) * vNEXT;
				vKEY.m_dwTime = (*itKEY).first;

				fwrite( &vKEY, sizeof(TROTKEY), 1, pFILE);
				vPREV = vNEXT;
			}
		}
	}
	else
	{
		MAPIKEY::iterator itKEY;
		MAPIKEY mapKEY;
		mapKEY.clear();

		pControl = pControl->GetRotationController();
		if(!pControl)
		{
			int nCount = 0;

			fwrite( &nCount, sizeof(int), 1, pFILE);
			return;
		}

		IKeyControl *pIKEY = GetKeyControlInterface(pControl);
		if(!pIKEY)
		{
			int nCount = 0;

			fwrite( &nCount, sizeof(int), 1, pFILE);
			return;
		}

		BuildBONESKEY(
			&mapKEY,
			pIKEY);

		int nCount = INT(mapKEY.size());
		fwrite( &nCount, sizeof(int), 1, pFILE);

		D3DXQUATERNION vPREV = GetLocalROT(
			pINODE,
			m_dwSTART);

		for( itKEY = mapKEY.begin(); itKEY != mapKEY.end(); itKEY++)
		{
			TROTKEY vKEY;

			D3DXQUATERNION vNEXT = GetLocalROT(
				pINODE,
				(*itKEY).first);

			vKEY.m_vKeyQuat = InverseQUAT(vPREV) * vNEXT;
			vKEY.m_dwTime = (*itKEY).first;

			fwrite( &vKEY, sizeof(TROTKEY), 1, pFILE);
			vPREV = vNEXT;
			delete (*itKEY).second;
		}

		mapKEY.clear();
	}
}

void CExportTAF::ExportSCALE( LPINODEDATA pINODE,
							  FILE *pFILE)
{
	if( m_dwSTART >= m_dwEND )
	{
		int nCount = 0;

		fwrite( &nCount, sizeof(int), 1, pFILE);
		return;
	}
	Control *pControl = pINODE->m_pINODE->GetTMController();

	if(!pControl)
	{
		int nCount = 0;

		fwrite( &nCount, sizeof(int), 1, pFILE);
		return;
	}

	if(m_bSAMPLE)
	{
		int nKeyCount = (m_dwEND - m_dwSTART) / (GetTicksPerFrame() * m_nRATE) + 1;
		int nCount = 1;

		if(!((m_dwEND - m_dwSTART) % (GetTicksPerFrame() * m_nRATE)))
			nKeyCount--;
		LPTSCALEKEY pDATA = new TSCALEKEY[nKeyCount + 1];

		pDATA[0].m_vKeyScale = GetLocalSCALE(
			pINODE,
			m_dwSTART);
		pDATA[0].m_dwTime = m_dwSTART;

		for( int i=0; i<nKeyCount - 1; i++)
		{
			DWORD dwTIME = m_dwSTART + (i + 1) * GetTicksPerFrame() * m_nRATE;

			D3DXVECTOR3 vSCALE = GetLocalSCALE(
				pINODE,
				dwTIME);

			if( nCount < 2 || !EqualPoint3( pDATA[nCount - 1].m_vKeyScale, vSCALE) || !EqualPoint3( pDATA[nCount - 2].m_vKeyScale, vSCALE) )
			{
				pDATA[nCount].m_vKeyScale = vSCALE;
				pDATA[nCount].m_dwTime = dwTIME;
				nCount++;
			}
			else
				pDATA[nCount - 1].m_dwTime = dwTIME;
		}

		D3DXVECTOR3 vSCALE = GetLocalSCALE(
			pINODE,
			m_dwEND);

		if( nCount < 2 || !EqualPoint3( pDATA[nCount - 1].m_vKeyScale, vSCALE) || !EqualPoint3( pDATA[nCount - 2].m_vKeyScale, vSCALE) )
		{
			pDATA[nCount].m_vKeyScale = vSCALE;
			pDATA[nCount].m_dwTime = m_dwEND;
			nCount++;
		}
		else
			pDATA[nCount - 1].m_dwTime = m_dwEND;

		fwrite( &nCount, sizeof(int), 1, pFILE);
		fwrite( pDATA, nCount * sizeof(TSCALEKEY), 1, pFILE);

		delete[] pDATA;
	}
	else if( pControl->ClassID() == BIPSLAVE_CONTROL_CLASS_ID || pControl->ClassID() == BIPBODY_CONTROL_CLASS_ID )
	{
		MAPIKEY::iterator itKEY;

		BuildBIPEDKEY(
			pINODE,
			pControl);

		int nCount = INT(m_mapKEY.size());
		fwrite( &nCount, sizeof(int), 1, pFILE);

		if( nCount > 0 )
			for( itKEY = m_mapKEY.begin(); itKEY != m_mapKEY.end(); itKEY++)
			{
				TSCALEKEY vKEY;

				vKEY.m_vKeyScale = GetLocalSCALE(
					pINODE,
					(*itKEY).first);
				vKEY.m_dwTime = (*itKEY).first;

				fwrite( &vKEY, sizeof(TSCALEKEY), 1, pFILE);
			}
	}
	else
	{
		MAPIKEY::iterator itKEY;
		MAPIKEY mapKEY;
		mapKEY.clear();

		pControl = pControl->GetScaleController();
		if(!pControl)
		{
			int nCount = 0;

			fwrite( &nCount, sizeof(int), 1, pFILE);
			return;
		}

		IKeyControl *pIKEY = GetKeyControlInterface(pControl);
		if(!pIKEY)
		{
			int nCount = 0;

			fwrite( &nCount, sizeof(int), 1, pFILE);
			return;
		}

		BuildBONESKEY(
			&mapKEY,
			pIKEY);

		int nCount = INT(mapKEY.size());
		fwrite( &nCount, sizeof(int), 1, pFILE);

		for( itKEY = mapKEY.begin(); itKEY != mapKEY.end(); itKEY++)
		{
			TSCALEKEY vKEY;

			vKEY.m_vKeyScale = GetLocalSCALE(
				pINODE,
				(*itKEY).first);
			vKEY.m_dwTime = (*itKEY).first;

			fwrite( &vKEY, sizeof(TSCALEKEY), 1, pFILE);
			delete (*itKEY).second;
		}

		mapKEY.clear();
	}
}

void CExportTAF::CheckBONES( HWND hWnd)
{
	LISTINODEDATA::iterator itINODE;

	MAPSTRING mapSTRING;
	mapSTRING.clear();

	for( itINODE = m_vINODE.begin(); itINODE != m_vINODE.end(); itINODE++)
		if((*itINODE)->m_pINODE)
		{
			CString strNAME((*itINODE)->m_pINODE->GetName());

			if( mapSTRING.find(strNAME) != mapSTRING.end() )
			{
				CString strMSG;

				strMSG.Format( IDS_FMT_DUP_BONES_NAME, strNAME);
				MessageBox( hWnd, strMSG, _T("ExportTAF"), MB_OK);

				break;
			}

			mapSTRING.insert( MAPSTRING::value_type( strNAME, 0));
		}

	mapSTRING.clear();
}

int	CExportTAF::DoExport( const TCHAR *name,
						  ExpInterface *ei,
						  Interface *i,
						  BOOL suppressPrompts,
						  DWORD options)
{
	HWND hWnd = i->GetMAXHWnd();

	if( !DialogBoxParam(
		hInstance,
		MAKEINTRESOURCE(IDD_PANEL_TAF),
		hWnd,
		ExportTAFProc,
		(LPARAM) this ))
		return 1;

	Interval vAniRange = i->GetAnimRange();
	INode *pROOT = i->GetRootNode();
	int nRESULT = FALSE;

	m_dwSTART = vAniRange.Start();
	m_dwEND = vAniRange.End();
	ClearDATA();

	if(pROOT)
	{
		LISTINODEDATA::iterator itINODE;
		FILE *pFILE = fopen( name, "wb");

		InitDATA(pROOT);
		InitINDEX();

		DWORD dwCount = DWORD(m_vINODE.size());
		int nTAFVersion = TAF_VERSION;

		fwrite( &nTAFVersion, sizeof(int), 1, pFILE);
		fwrite( &dwCount, sizeof(DWORD), 1, pFILE);

		fwrite( &m_dwSTART, sizeof(DWORD), 1, pFILE);
		fwrite( &m_dwEND, sizeof(DWORD), 1, pFILE);

		for( itINODE = m_vINODE.begin(); itINODE != m_vINODE.end(); itINODE++)
			ExportBONES( *itINODE, pFILE);

		for( itINODE = m_vINODE.begin(); itINODE != m_vINODE.end(); itINODE++)
			ExportMESH( *itINODE, pFILE);
		m_vINODE.sort(node_less());

		for( itINODE = m_vINODE.begin(); itINODE != m_vINODE.end(); itINODE++)
			ExportBONESNODE( *itINODE, pFILE);

		for( itINODE = m_vINODE.begin(); itINODE != m_vINODE.end(); itINODE++)
			ExportFRAME( *itINODE, pFILE);

		nRESULT = TRUE;
		fclose(pFILE);
	}

	CheckBONES(hWnd);
	ClearDATA();

	return nRESULT;
}
