/**********************************************************************
 *<
	FILE: TachyonShader.cpp

	DESCRIPTION:	Appwizard generated plugin

	CREATED BY: 

	HISTORY: 

 *>	Copyright (c) 2003, All Rights Reserved.
 **********************************************************************/

#include "TachyonShader.h"

#define TachyonShader_CLASS_ID	Class_ID(0x6cb664ff, 0x995ac191)

#define PBLOCK_REF	0


inline float Bound( float x, float min = 0.0f, float max = 1.0f) { return x < min ? min : (x > max ? max : x);}
inline Color Bound( Color& c) { return Color( Bound(c.r), Bound(c.g), Bound(c.b));}

/*===========================================================================*\
 |	Definition of our UI and map parameters for the shader
\*===========================================================================*/

// Number of Map Buttons on our UI and number of texmaps
#define NMBUTS				2
#define SHADER_NTEXMAPS		8

// Channels used by this shader
#define S_DI				0
#define S_DK				1

// Texture Channel number --> IDC resource ID
static int texMButtonsIDC[] = { IDC_MAPON_DI, IDC_MAPON_DK};

// Map Button --> Texture Map number
static int texmapFromMBut[] = { 0, 1};

// Channel Name array
static int texNameIDS[STD2_NMAX_TEXMAPS] = {
	IDS_MN_DIFFUSE,		IDS_MN_DARK,		IDS_MN_SPECULAR,	IDS_MN_SPEC_LEVEL,
	IDS_MN_GLOSSINESS,	IDS_MN_SELFILLUM,	IDS_MN_TRANS,		IDS_MN_FILTER,
	IDS_MN_NONE,		IDS_MN_NONE,		IDS_MN_NONE,		IDS_MN_NONE,
	IDS_MN_NONE,		IDS_MN_NONE,		IDS_MN_NONE,		IDS_MN_NONE,
	IDS_MN_NONE,		IDS_MN_NONE,		IDS_MN_NONE,		IDS_MN_NONE,
	IDS_MN_NONE,		IDS_MN_NONE,		IDS_MN_NONE,		IDS_MN_NONE
};

// Channel Name array (INTERNAL NAMES)
static int intertexNameIDS[STD2_NMAX_TEXMAPS] = {
	IDS_MN_DIFFUSE_I,		IDS_MN_DARK_I,		IDS_MN_SPECULAR_I,	IDS_MN_SPEC_LEVEL_I,
	IDS_MN_GLOSSINESS_I,	IDS_MN_SELFILLUM_I,	IDS_MN_TRANS_I,		IDS_MN_FILTER_I,
	IDS_MN_NONE,			IDS_MN_NONE,		IDS_MN_NONE,		IDS_MN_NONE,
	IDS_MN_NONE,			IDS_MN_NONE,		IDS_MN_NONE,		IDS_MN_NONE,
	IDS_MN_NONE,			IDS_MN_NONE,		IDS_MN_NONE,		IDS_MN_NONE,
	IDS_MN_NONE,			IDS_MN_NONE,		IDS_MN_NONE,		IDS_MN_NONE
};


// Type of map channels supported
static int chanType[STD2_NMAX_TEXMAPS] = {
	CLR_CHANNEL,
	MONO_CHANNEL,
	CLR_CHANNEL,
	MONO_CHANNEL,
	MONO_CHANNEL,
	CLR_CHANNEL,
	MONO_CHANNEL,
	CLR_CHANNEL,
	UNSUPPORTED_CHANNEL,
	UNSUPPORTED_CHANNEL,
	UNSUPPORTED_CHANNEL,
	UNSUPPORTED_CHANNEL,
	UNSUPPORTED_CHANNEL,
	UNSUPPORTED_CHANNEL,
	UNSUPPORTED_CHANNEL,
	UNSUPPORTED_CHANNEL,
	UNSUPPORTED_CHANNEL,
	UNSUPPORTED_CHANNEL,
	UNSUPPORTED_CHANNEL
};


// What channel in our shader maps to one of the StdMat channel IDs?
//
//
// The following are the StdMat channel IDs:
//
// 0   --	ambient
// 1   --	diffuse
// 2   --	specular
// 3   --	shininesNs
// 4   --	shininess strength
// 5   --	self-illumination
// 6   --	opacity
// 7   --	filter color
// 8   --	bump 
// 9   --	reflection
// 10  --	refraction 
// 11  --	displacement

static int stdIDToChannel[N_ID_CHANNELS] = { -1, 0, 2, 4, 3, 5, 6, 7, -1, -1, -1, -1};


/*===========================================================================*\
 |	Class definition for the shader itself
\*===========================================================================*/

#define SHADER_PARAMS (STD_EXTRA)

class TachyonShaderDlg;


class TachyonShader : public Shader
{
	friend class TachyonShaderDlg;

public:
	IParamBlock2 *m_pPARAM;

protected:
	TachyonShaderDlg *m_pDlg;

	TimeValue m_nCurTime;
	Interval m_vIValid;

	Color m_vDIFFUSE;
	Color m_vDARK;

public:
	TSTR GetName() { return GetString(IDS_CLASS_NAME);};
	ULONG SupportStdParams() { return SHADER_PARAMS;};

	void ConvertParamBlk( ParamBlockDescID *oldPBDesc, int oldCount, IParamBlock *oldPB) {};
	void CopyStdParams( Shader *pFrom);

	// Texture map channel support
	TSTR GetTexChannelInternalName( long nTex) { return GetString(intertexNameIDS[nTex]);};
	TSTR GetTexChannelName( long nTex) { return GetString(texNameIDS[nTex]);};

	long StdIDToChannel( long stdID) { return stdIDToChannel[stdID];};
	long ChannelType( long nChan) { return chanType[nChan];};
	long nTexChannelsSupported() { return SHADER_NTEXMAPS;};

	// Find out if we have a key at time value t
	BOOL KeyAtTime( int id, TimeValue t) { return m_pPARAM->KeyFrameAtTime( id, t);};

	// Get the requirements for this material (supersampling, etc)
	ULONG GetRequirements( int subMtlNum) { return MTLREQ_PHONG;};

	// Support for the dialog UI
	void SetParamDlg( ShaderParamDlg* newDlg, int n=0);

	ShaderParamDlg* CreateParamDialog( HWND hOldRollup, HWND hwMtlEdit, IMtlParams *imp, StdMat2* theMtl, int rollupOpen, int);
	ShaderParamDlg* GetParamDlg(int) { return (ShaderParamDlg *) m_pDlg;};

	// Shader state methods
	void Update( TimeValue t, Interval& valid);
	void Reset();

	// Fill the IllumParams with our data
	void GetIllumParams( ShadeContext &sc, IllumParams &ip);

	// Shader specific implimentations
	void CombineComponents( ShadeContext &sc, IllumParams& ip);
	void Illum( ShadeContext &sc, IllumParams &ip);

	// Metal support
	BOOL IsMetal();

	// This plugin specific
	void SetDarkClr( Color c, TimeValue t);
	Color GetDarkClr( int mtlNum=0, BOOL backFace=FALSE);
	Color GetDarkClr( TimeValue t);

	// DIFFUSE
	void SetDiffuseClr( Color c, TimeValue t);
	Color GetDiffuseClr( int mtlNum=0, BOOL backFace=FALSE);
	Color GetDiffuseClr( TimeValue t);

	// AMBIENT
	void SetAmbientClr( Color c, TimeValue t);
	Color GetAmbientClr( int mtlNum=0, BOOL backFace=FALSE);
	Color GetAmbientClr( TimeValue t);

	// SPECULAR
	void SetSpecularClr( Color c, TimeValue t);
	void SetSpecularLevel( float v, TimeValue t);
	Color GetSpecularClr( int mtlNum=0, BOOL backFace=FALSE);
	float GetSpecularLevel( int mtlNum=0, BOOL backFace=FALSE);
	Color GetSpecularClr( TimeValue t);
	float GetSpecularLevel( TimeValue t);

	// SELFILLUM
	void SetSelfIllum( float v, TimeValue t);
	float GetSelfIllum( int mtlNum=0, BOOL backFace=FALSE);
	void SetSelfIllumClrOn( BOOL on);
	BOOL IsSelfIllumClrOn();
	BOOL IsSelfIllumClrOn( int mtlNum, BOOL backFace);
	void SetSelfIllumClr( Color c, TimeValue t);
	Color GetSelfIllumClr( int mtlNum=0, BOOL backFace=FALSE);
	float GetSelfIllum( TimeValue t);
	Color GetSelfIllumClr( TimeValue t);

	// SOFTEN
	void SetSoftenLevel( float v, TimeValue t);
	float GetSoftenLevel( int mtlNum=0, BOOL backFace=FALSE);
	float GetSoftenLevel( TimeValue t);

	void SetGlossiness( float v, TimeValue t);
	float GetGlossiness( int mtlNum, BOOL backFace);
	float GetGlossiness( TimeValue t);

	// Standard locks not supported (Diffuse+Specular)/(Ambient+Diffuse)
	void SetLockDS( BOOL lock) {};
	BOOL GetLockDS() { return FALSE;};

	void SetLockAD( BOOL lock) {};
	BOOL GetLockAD() { return FALSE;};

	void SetLockADTex( BOOL lock) {};
	BOOL GetLockADTex() { return FALSE;};

	void NotifyChanged() { NotifyDependents( FOREVER, PART_ALL, REFMSG_CHANGE);};

	// Loading/Saving
	IOResult Load( ILoad *iload);
	IOResult Save( ISave *isave);

	//From Animatable
	Class_ID ClassID() { return TachyonShader_CLASS_ID;};
	SClass_ID SuperClassID() { return SHADER_CLASS_ID;};
	void GetClassName( TSTR& s) { s = GetString(IDS_CLASS_NAME);};

	RefResult NotifyRefChanged( Interval changeInt, RefTargetHandle hTarget, PartID& partID, RefMessage message);
	RefTargetHandle Clone( RemapDir &remap);

	int NumSubs() { return 1;};
	TSTR SubAnimName( int i) { return GetString(IDS_PARAMS);};
	Animatable* SubAnim( int i) { return m_pPARAM;};

	// TODO: Maintain the number or references here
	int NumRefs() { return 1;};
	RefTargetHandle GetReference( int i) { return m_pPARAM;};
	void SetReference( int i, RefTargetHandle rtarg) { m_pPARAM = (IParamBlock2 *) rtarg;};

	int	NumParamBlocks() { return 1;};
	IParamBlock2* GetParamBlock( int i) { return m_pPARAM;};
	IParamBlock2* GetParamBlockByID( BlockID id) { return m_pPARAM->ID() == id ? m_pPARAM : NULL;};

	void DeleteThis() { delete this;};

	//Constructor/Destructor
	TachyonShader();
	~TachyonShader();
};


class TachyonShaderClassDesc : public ClassDesc2
{
public:
	int 			IsPublic() { return TRUE;};
	void *			Create( BOOL loading = FALSE) { return new TachyonShader();};
	const TCHAR *	ClassName() { return GetString(IDS_CLASS_NAME);};
	SClass_ID		SuperClassID() { return SHADER_CLASS_ID;};
	Class_ID		ClassID() { return TachyonShader_CLASS_ID;};
	const TCHAR* 	Category() { return GetString(IDS_CATEGORY);};

	const TCHAR*	InternalName() { return _T("TachyonShader");};
	HINSTANCE		HInstance() { return hInstance;};
};


static TachyonShaderClassDesc TachyonShaderDesc;
ClassDesc2* GetTachyonShaderDesc() { return &TachyonShaderDesc;};


enum {tachyonshader_params};
enum { pb_diffuse, pb_dark};


static ParamBlockDesc2 tachyonshader_param_blk( tachyonshader_params, _T("params"), 0, &TachyonShaderDesc,
	P_AUTO_CONSTRUCT, PBLOCK_REF,
	// params
	pb_diffuse, _T("diffuse"), TYPE_RGBA, P_ANIMATABLE, IDS_MN_DIFFUSE,
		p_default, Color( 0.59f, 0.59f, 0.59f),
		end,
	pb_dark, _T("dark"), TYPE_RGBA, P_ANIMATABLE, IDS_MN_DARK,
		p_default, Color( 0.59f, 0.59f, 0.59f),
		end,
	end);


#define SHADER_NCOLBOX		2

class TachyonShaderDlg : public ShaderParamDlg
{
public:
	TachyonShader*	pShader;
	Texmap*			pDARK;
	StdMat2*		pMtl;
	HPALETTE		hOldPal;
	HWND			hwmEdit;
	IMtlParams*		pMtlPar;
	HWND			hRollup;
	TimeValue		curTime;
	BOOL			valid;
	BOOL			isActive;

	IColorSwatch *cs[SHADER_NCOLBOX];
	ICustButton *texMBut[NMBUTS];
	TexDADMgr dadMgr;

	TachyonShaderDlg( HWND hwMtlEdit, IMtlParams *pParams);
	~TachyonShaderDlg();

	// DnD: Required for correctly operating map buttons
	int FindSubTexFromHWND( HWND hw)
	{
		for( long i=0; i<NMBUTS; i++)
		{
			if( hw == texMBut[i]->GetHwnd() )
				return texmapFromMBut[i];
		}

		return -1;
	};

	BOOL PanelProc( HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam);
	Class_ID ClassID() { return TachyonShader_CLASS_ID;};

	void SetThing( ReferenceTarget *m) { pMtl = (StdMat2 *) m;};
	void SetThings( StdMat2* theMtl, Shader* theShader)
	{
		pShader = (TachyonShader *) theShader;

		if(pShader)
			pShader->SetParamDlg( this, 0);

		pMtl = theMtl;
	};

	ReferenceTarget* GetThing() { return (ReferenceTarget *) pMtl;};
	Shader* GetShader() { return pShader;};

	void SetTime( TimeValue t)
	{
		if(!pShader->m_vIValid.InInterval(t))
		{
			Interval v;

			pShader->Update( t, v);
			LoadDialog(TRUE);
		}

		curTime = t;
	};

	BOOL KeyAtCurTime( int id) { return pShader->KeyAtTime( id, curTime);};

	void DeleteThis() { delete this;};
	void ActivateDlg( BOOL dlgOn) { isActive = dlgOn;};
	HWND GetHWnd() { return hRollup;};

	void NotifyChanged() { pShader->NotifyChanged();};
	void LoadDialog( BOOL draw);
	void ReloadDialog() { Interval v; pShader->Update(pMtlPar->GetTime(), v); LoadDialog(FALSE);};
	void UpdateDialog( ParamID paramId) { ReloadDialog();};

	void UpdateMtlDisplay() { pMtlPar->MtlChanged();};
	void UpdateColSwatches();
	void UpdateMapButtons();
	void UpdateOpacity();
	void ResetMapChannel();

	void SelectEditColor( int i) { if( i == pb_diffuse ) cs[i]->EditThis(FALSE);};
};


static BOOL CALLBACK TachyonShaderDlgProc( HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	TachyonShaderDlg *theDlg;

	if( msg == WM_INITDIALOG )
	{
		theDlg = (TachyonShaderDlg *) lParam;
		SetWindowLong( hwndDlg, GWL_USERDATA, lParam);
	}
	else
	{
		if( (theDlg = (TachyonShaderDlg *) GetWindowLong( hwndDlg, GWL_USERDATA)) == NULL )
			return FALSE;
	}

	theDlg->isActive = 1;
	BOOL res = theDlg->PanelProc( hwndDlg, msg, wParam, lParam);
	theDlg->isActive = 0;

	return res;
}

void TachyonShader::SetParamDlg( ShaderParamDlg* newDlg, int n)
{
	if(m_pDlg)
		m_pDlg->ResetMapChannel();

	m_pDlg = (TachyonShaderDlg *) newDlg;
}

ShaderParamDlg* TachyonShader::CreateParamDialog( HWND hOldRollup, HWND hwMtlEdit, IMtlParams *imp, StdMat2* theMtl, int rollupOpen, int)
{
	Interval v;
	Update( imp->GetTime(), v);

	TachyonShaderDlg *pDlg = new TachyonShaderDlg( hwMtlEdit, imp);
	pDlg->SetThings( theMtl, this);

	if(hOldRollup)
	{
		pDlg->hRollup = imp->ReplaceRollupPage(
			hOldRollup,
			hInstance,
			MAKEINTRESOURCE(IDD_PANEL),
			TachyonShaderDlgProc,
			GetString(IDS_PARAMS),
			(LPARAM) pDlg,
			rollupOpen);
	}
	else
	{
		pDlg->hRollup = imp->AddRollupPage(
			hInstance,
			MAKEINTRESOURCE(IDD_PANEL),
			TachyonShaderDlgProc,
			GetString(IDS_PARAMS),
			(LPARAM) pDlg,
			rollupOpen);
	}

	return (ShaderParamDlg *) pDlg;
}

RefResult TachyonShader::NotifyRefChanged( Interval changeInt, RefTargetHandle hTarget, PartID& partID, RefMessage message)
{
	switch(message)
	{
	case REFMSG_CHANGE	:
		m_vIValid.SetEmpty();

		if( hTarget == m_pPARAM )
		{
			// update UI if paramblock changed, possibly from scripter
			ParamID changingParam = m_pPARAM->LastNotifyParamID();

			// reload the dialog if present
			if(m_pDlg)
				m_pDlg->UpdateDialog(changingParam);

			// notify our dependents that we've changed
			NotifyChanged();
		}

		break;
	}

	return REF_SUCCEED;
}

TachyonShaderDlg::TachyonShaderDlg( HWND hwMtlEdit, IMtlParams *pParams)
{
	hRollup = NULL;
	pShader = NULL;
	pDARK = NULL;
	pMtl = NULL;

	hwmEdit = hwMtlEdit;
	pMtlPar = pParams;
	dadMgr.Init(this);

	isActive = FALSE;
	valid = FALSE;
	curTime = 0;

	for( long i=0; i<SHADER_NCOLBOX; i++)
		cs[i] = NULL;

	for( i=0; i<NMBUTS; i++)
		texMBut[i] = NULL;
}

TachyonShaderDlg::~TachyonShaderDlg()
{
	HDC hdc = GetDC(hRollup);

	GetGPort()->RestorePalette( hdc, hOldPal);
	ReleaseDC( hRollup, hdc);

	if(pShader)
		pShader->SetParamDlg( NULL, 0);

	for( long i=0; i<NMBUTS; i++)
	{
		ReleaseICustButton(texMBut[i]);
		texMBut[i] = NULL;
	}

	SetWindowLong( hRollup, GWL_USERDATA, NULL);
	hRollup = NULL;
	pDARK = NULL;
}

void TachyonShaderDlg::LoadDialog( BOOL draw)
{
	if( pShader && hRollup )
	{
		UpdateColSwatches();
		UpdateMapButtons();
	}
}


static TCHAR* mapStates[] = { _T(" "), _T("m"), _T("M")};

void TachyonShaderDlg::UpdateMapButtons()
{
	for( long i=0; i<NMBUTS; i++)
	{
		int nMap = texmapFromMBut[i];
		int state = pMtl->GetMapState(nMap);

		texMBut[i]->SetText(mapStates[state]);

		TSTR nm = pMtl->GetMapName(nMap);
		texMBut[i]->SetTooltip( TRUE, nm);
	}
}

void TachyonShaderDlg::UpdateOpacity()
{
}

void TachyonShaderDlg::UpdateColSwatches()
{
	Color vCLR[SHADER_NCOLBOX] = { pShader->GetDiffuseClr(), pShader->GetDarkClr()};
	int vPB[SHADER_NCOLBOX] = { pb_diffuse, pb_dark};

	for( long i=0; i<SHADER_NCOLBOX; i++)
	{
		cs[i]->SetKeyBrackets(pShader->KeyAtTime( vPB[i], curTime));
		cs[i]->SetColor(vCLR[i]);
	}
}

void TachyonShaderDlg::ResetMapChannel()
{
	if(!pMtl)
		return;

	Texmap *pNEW = pMtl->GetSubTexmap(S_DK);
	if( pNEW != pDARK )
	{
		if(pNEW)
		{
			UVGen *pUV = pNEW->GetTheUVGen();

			if(pUV)
				pUV->SetMapChannel(3);
		}

		pDARK = pNEW;
	}
}


static int ColorIDCToIndex( int idc)
{
	switch(idc)
	{
	case IDC_DIFFUSE	: return pb_diffuse;
	case IDC_DARK		: return pb_dark;
	}

	return 0;
}


BOOL TachyonShaderDlg::PanelProc( HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int code = HIWORD(wParam);
	int id = LOWORD(wParam);

	switch(msg)
	{
	case WM_INITDIALOG			:
		{
			HDC theHDC = GetDC(hwndDlg);

			hOldPal = GetGPort()->PlugPalette(theHDC);
			ReleaseDC( hwndDlg, theHDC);

			Color vCLR[SHADER_NCOLBOX] = { pShader->GetDiffuseClr(), pShader->GetDarkClr()};
			DWORD vIDC[SHADER_NCOLBOX] = { IDC_DIFFUSE, IDC_DARK};
			DWORD vIDS[SHADER_NCOLBOX] = { IDS_DIFFUSE_COLOR, IDS_DARK_COLOR};

			for( long i=0; i<SHADER_NCOLBOX; i++)
			{
				HWND hwndCS = GetDlgItem( hwndDlg, vIDC[i]);

				cs[i] = GetIColorSwatch(
					hwndCS,
					vCLR[i],
					GetString(vIDS[i]));
			}

			for( i=0; i<NMBUTS; i++)
			{
				texMBut[i] = GetICustButton(GetDlgItem( hwndDlg, texMButtonsIDC[i]));
				assert(texMBut[i]);

				texMBut[i]->SetRightClickNotify(TRUE);
				texMBut[i]->SetDADMgr(&dadMgr);
			}

			pDARK = pMtl->GetSubTexmap(S_DK);
			LoadDialog(TRUE);
		}

		break;

	case WM_COMMAND				:
		for( int i=0; i<NMBUTS; i++)
			if( id == texMButtonsIDC[i] )
			{
				PostMessage( hwmEdit, WM_TEXMAP_BUTTON, texmapFromMBut[i], (LPARAM) pMtl);
				UpdateMapButtons();

				return FALSE;
			}

		break;

	case CC_COLOR_DROP			:
	case CC_COLOR_SEL			: SelectEditColor(ColorIDCToIndex(id)); break;

	case CC_COLOR_BUTTONDOWN	:
		if( ColorIDCToIndex(id) == pb_diffuse )
			theHold.Begin();

		break;

	case CC_COLOR_BUTTONUP		:
		if( ColorIDCToIndex(id) == pb_diffuse )
			if(code)
				theHold.Accept(GetString(IDS_PARAMCHG));
			else
				theHold.Cancel();

		break;

	case CC_COLOR_CHANGE		:
		if( ColorIDCToIndex(id) == pb_diffuse )
		{
			if(code)
				theHold.Begin();
			DWORD curRGB = cs[pb_diffuse]->GetColor();

			pShader->SetDiffuseClr( (Color) curRGB, curTime);
			if(code)
			{
				theHold.Accept(GetString(IDS_PARAMCHG));
				UpdateMtlDisplay();
			}
		}

		break;

	case WM_PAINT				:
		if(!valid)
		{
			valid = TRUE;
			ReloadDialog();
		}

		break;
	}

	return FALSE;
}


/*===========================================================================*\
 |	Constructor
\*===========================================================================*/

TachyonShader::TachyonShader()
{
	m_pPARAM = NULL;
	m_pDlg = NULL;

	m_vIValid.SetEmpty();
	m_nCurTime = 0;
}

TachyonShader::~TachyonShader()
{
}


/*===========================================================================*\
 |	Cloning and coping standard parameters
\*===========================================================================*/

void TachyonShader::CopyStdParams( Shader *pFrom)
{
	macroRecorder->Disable();

	SetAmbientClr( pFrom->GetAmbientClr( 0, 0), m_nCurTime);
	SetDiffuseClr( pFrom->GetDiffuseClr( 0, 0), m_nCurTime);

	macroRecorder->Enable();
	m_vIValid.SetEmpty();
}


RefTargetHandle TachyonShader::Clone( RemapDir &remap)
{
	TachyonShader *mnew = new TachyonShader();

	mnew->ReplaceReference( 0, remap.CloneRef(m_pPARAM));
	mnew->m_vIValid.SetEmpty();
	mnew->m_vDIFFUSE = m_vDIFFUSE;
	mnew->m_vDARK = m_vDARK;
	BaseClone( this, mnew, remap);

	return (RefTargetHandle) mnew;
}


/*===========================================================================*\
 |	Shader state
\*===========================================================================*/

void TachyonShader::Update( TimeValue t, Interval &valid)
{
	Point3 p;

	if(!m_vIValid.InInterval(t))
	{
		m_vIValid.SetInfinite();

		m_pPARAM->GetValue( pb_diffuse, t, p, m_vIValid);
		m_vDIFFUSE = Bound(Color( p.x, p.y, p.z));

		m_pPARAM->GetValue( pb_dark, t, p, m_vIValid);
		m_vDARK = Bound(Color( p.x, p.y, p.z));
		m_nCurTime = t;
	}

	valid &= m_vIValid;
}

void TachyonShader::Reset()
{
	TachyonShaderDesc.MakeAutoParamBlocks(this);

	m_vIValid.SetEmpty();
	SetDiffuseClr( Color( 0.59f, 0.59f, 0.59f), 0);
	SetDarkClr( Color( 0.59f, 0.59f, 0.59f), 0);
}


/*===========================================================================*\
 |	Shader load/save
\*===========================================================================*/

#define SHADER_VERS_CHUNK 0x6500


IOResult TachyonShader::Save( ISave *isave)
{
	ULONG nb;

	isave->BeginChunk(SHADER_VERS_CHUNK);
	int version = 1;

	isave->Write( &version, sizeof(version), &nb);
	isave->EndChunk();

	return IO_OK;
}


IOResult TachyonShader::Load( ILoad *iload)
{
	IOResult res;
	ULONG nb;

	int version = 0;
	int id;

	while( IO_OK == (res = iload->OpenChunk()) )
	{
		switch(id = iload->CurChunkID())
		{
		case SHADER_VERS_CHUNK	: res = iload->Read( &version, sizeof(version), &nb); break;
		}

		iload->CloseChunk();
		if( res != IO_OK )
			return res;
	}

	return IO_OK;
}


/*===========================================================================*\
 |	Actually shade the surface
\*===========================================================================*/

void TachyonShader::GetIllumParams( ShadeContext &sc, IllumParams &ip)
{
	ip.stdParams = SupportStdParams();
	ip.channels[S_DI] = m_vDIFFUSE;
}

void TachyonShader::Illum( ShadeContext &sc, IllumParams &ip)
{
	Color lightCol;
	LightDesc *l;

	Color Cd = ip.channels[S_DI];
	double phExp = 4.0;

	for( int i=0; i<sc.nLights; i++)
	{
		l = sc.Light(i);

		register float NL, diffCoef;
		Point3 L;

		if( l->Illuminate( sc, sc.Normal(), lightCol, L, NL, diffCoef) )
		{
			if(l->ambientOnly)
			{
				ip.ambIllumOut += lightCol;
				continue;
			}

			if( NL <= 0.0f )
				continue;

			if(l->affectDiffuse)
				ip.diffIllumOut += diffCoef * lightCol;
		}
	}

	// now we can multiply by the clrs
	ip.ambIllumOut *= Cd * 0.5f;
	ip.diffIllumOut *= Cd;
	ip.finalOpac = 1.0f;

	CombineComponents( sc, ip);
}

void TachyonShader::CombineComponents( ShadeContext &sc, IllumParams& ip)
{
	ip.finalC = ip.finalOpac * (ip.ambIllumOut + ip.diffIllumOut + ip.selfIllumOut) + ip.specIllumOut + ip.reflIllumOut + ip.transIllumOut;
}

BOOL TachyonShader::IsMetal()
{
	return FALSE;
}


/*===========================================================================*\
 |	Shader specific transaction
\*===========================================================================*/

void TachyonShader::SetDarkClr( Color c, TimeValue t)
{
	m_vDARK = c;
	m_pPARAM->SetValue( pb_dark, t, c);
}

Color TachyonShader::GetDarkClr( int mtlNum, BOOL backFace)
{
	return m_vDARK;
}

Color TachyonShader::GetDarkClr( TimeValue t)
{
	return m_pPARAM->GetColor( pb_dark, t);
}

void TachyonShader::SetGlossiness( float v, TimeValue t)
{
}

float TachyonShader::GetGlossiness( int mtlNum, BOOL backFace)
{
	return 40.0f;
}

float TachyonShader::GetGlossiness( TimeValue t)
{
	return 40.0f;
}


/*===========================================================================*\
 |	Diffuse Transactions
\*===========================================================================*/

void TachyonShader::SetDiffuseClr( Color c, TimeValue t)
{
	m_vDIFFUSE = c;
	m_pPARAM->SetValue( pb_diffuse, t, c);
}

Color TachyonShader::GetDiffuseClr( int mtlNum, BOOL backFace)
{
	return m_vDIFFUSE;
}

Color TachyonShader::GetDiffuseClr( TimeValue t)
{
	return m_pPARAM->GetColor( pb_diffuse, t);
}


/*===========================================================================*\
 |	Ambient Transactions
\*===========================================================================*/

void TachyonShader::SetAmbientClr( Color c, TimeValue t)
{
}

Color TachyonShader::GetAmbientClr( int mtlNum, BOOL backFace)
{
	return m_vDIFFUSE * 0.5f;
}

Color TachyonShader::GetAmbientClr( TimeValue t)
{
	return m_vDIFFUSE * 0.5f;
}


/*===========================================================================*\
 |	Specular Transactions
\*===========================================================================*/

void TachyonShader::SetSpecularClr( Color c, TimeValue t)
{
}

void TachyonShader::SetSpecularLevel( float v, TimeValue t)
{
}

Color TachyonShader::GetSpecularClr( int mtlNum, BOOL backFace)
{
	return Color( 0.9f, 0.9f, 0.9f);
}

float TachyonShader::GetSpecularLevel( int mtlNum, BOOL backFace)
{
	return 1.0f;
}

Color TachyonShader::GetSpecularClr( TimeValue t)
{
	return Color( 0.9f, 0.9f, 0.9f);
}

float TachyonShader::GetSpecularLevel( TimeValue t)
{
	return 1.0f;
}


/*===========================================================================*\
 |	SelfIllum Transactions
\*===========================================================================*/

void TachyonShader::SetSelfIllum( float v, TimeValue t)
{
}

float TachyonShader::GetSelfIllum( int mtlNum, BOOL backFace)
{
	return 0.0f;
}

void TachyonShader::SetSelfIllumClrOn( BOOL on)
{
}

BOOL TachyonShader::IsSelfIllumClrOn()
{
	return FALSE;
}

BOOL TachyonShader::IsSelfIllumClrOn( int mtlNum, BOOL backFace)
{
	return FALSE;
}

void TachyonShader::SetSelfIllumClr( Color c, TimeValue t)
{
}

Color TachyonShader::GetSelfIllumClr( int mtlNum, BOOL backFace)
{
	return Color( 0, 0, 0);
}

float TachyonShader::GetSelfIllum( TimeValue t)
{
	return 0.0f;
}

Color TachyonShader::GetSelfIllumClr( TimeValue t)
{
	return Color( 0, 0, 0);
}


/*===========================================================================*\
 |	Soften Transactions
\*===========================================================================*/

void TachyonShader::SetSoftenLevel( float v, TimeValue t)
{
}

float TachyonShader::GetSoftenLevel( int mtlNum, BOOL backFace)
{
	return 0.0f;
}

float TachyonShader::GetSoftenLevel( TimeValue t)
{
	return 0.0f;
}
