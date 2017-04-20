#include "StdAfx.h"
#include ".\tchart.h"

#define MODIFY_ITEM_EFFECT

CString CTChart::m_strNationCode;
CString* CTChart::m_vTSTRING = NULL;
MAPTSKILLTEMP CTChart::m_mapTSKILLTEMP;
MAPTSTRING CTChart::m_mapTCOUNTRYINFO;
MAPTSTRING CTChart::m_mapTRACEINFO;
MAPTSTRING CTChart::m_mapTCLASSINFO;
MAPTDETAILINFO CTChart::m_mapTDETAILINFO;	
MAPTREGIONINFO CTChart::m_mapTREGIONINFO;
MAPTBGM CTChart::m_mapTBGMINFO;
MAPTENV CTChart::m_mapTENVINFO;
MAPTACTION CTChart::m_mapTACTIONTEMP;
VTACTIONDATA CTChart::m_vTACTION;
TCLOTH CTChart::m_vTFACE[TFACE_COUNT][THAIR_COUNT];
TCLOTH CTChart::m_vTPANTS[TPANTS_COUNT];
TCLOTH CTChart::m_vTBODY[TBODY_COUNT];
TCLOTH CTChart::m_vTHAND[THAND_COUNT];
TCLOTH CTChart::m_vTFOOT[TFOOT_COUNT];
FLOAT CTChart::m_vTRACESCALE[TMAINRACE_COUNT][TSEX_COUNT];
DWORD CTChart::m_vTRACE[TMAINRACE_COUNT][TSEX_COUNT];
SETDWORD CTChart::m_setTRACEOBJID;							// Race object id set pool
MAPWORD CTChart::m_mapTSKILLTREE[TCONTRY_COUNT][TCLASS_COUNT][TSKILLTAB_COUNT];		// Skill tree data
MAPTQUESTTEMP CTChart::m_mapTQUESTTEMP;
MAPTQCLASSTEMP CTChart::m_mapTQCLASSTEMP;
TQCLASS CTChart::m_vNULLCLASS = { CString("NULL"), 0};
MAPTPOPUPITEM CTChart::m_mapTMENUITEM;
MAPTPOPUPMENU CTChart::m_mapTPOPUP;
MAPVTMAPJOINT CTChart::m_mapTMAPJOINT;
MAPTMAPINFO CTChart::m_mapTMAPINFO;
MAPTMINIMAP CTChart::m_mapTMINIMAP;
MAPTPORTAL CTChart::m_mapTPORTALINFO;
MAPDWORD CTChart::m_mapTBTNID2WORLD;
MAPDWORD CTChart::m_mapTBTNID2UNIT;
MAPTITEMTEMP CTChart::m_mapTITEMTEMP;
MAPTITEMTEMP CTChart::m_mapTCANTRADETEMP;
MAPTITEMVISUAL CTChart::m_mapTITEMVISUAL;
MAPTITEMGRADESFX CTChart::m_mapTITEMGRADESFX;
MAPTITEMMAGICSFX CTChart::m_mapTITEMMAGICSFX;
MAPTSFXTEMP CTChart::m_mapTSFXTEMP;
MAPMONTEMP CTChart::m_mapMONTEMP;		// Monster template data
MAPNPCTEMP CTChart::m_mapTNPCTEMP;		// NPC template data
MAPNPC CTChart::m_mapGlobalNPC;
MAPDWORD CTChart::m_mapTSTEPSND;
FLOAT CTChart::m_vLEVELRATE[TMAX_LEVEL];						// Level Ratio
MAPTFORMULA CTChart::m_mapTFORMULA;							// TFORMULACHART
MAPTSWITCHDATA CTChart::m_mapTSWITCH;
MAPTSKYBOX CTChart::m_mapTSKYBOX;								// Sky box pool
MAPTLIGHT CTChart::m_mapTLIGHT;								// Light setting pool
MAPTLEVEL CTChart::m_mapTLEVEL;								// Level price pool
MAPTUNITFOG CTChart::m_mapTFOG;								// Fog setting pool
MAPTGATEDATA CTChart::m_mapTGATE;
MAPTITEMMAGIC CTChart::m_mapITEMMAGIC;
MAPTPET CTChart::m_mapTPET;
MAPTITEMATTR CTChart::m_mapTITEMATTR;
VTITEMGRADE CTChart::m_vTITEMGRADE;
MAPTQUESTITEM CTChart::m_mapTQUESTMAGICITEM;
MAPTITEMGRADEVISUAL CTChart::m_mapTITEMGRADEVISUAL;
MAPTHELP CTChart::m_mapTHELP;
MAPTHELPLINK CTChart::m_mapTHELPLINK[ THELPLINK_TRIGGER_COUNT ];
MAPTGODTOWER CTChart::m_mapTGODTOWER;
MAPTGODBALL CTChart::m_mapTGODBALL;
MAPTMONSHOP  CTChart::m_mapTMONSHOP;
MAPTPORTALREGION CTChart::m_mapTPORTALREGION;
INT CTChart::m_TNATIONOPTION[ TNATIONOPTION_COUNT ];
CString* CTChart::m_pTWORDCURSE = NULL;
VTPSTRING CTChart::m_vTNAMECURSE;
VTPSTRING CTChart::m_vTCHATCURSE;
VTAUCTIONTREE CTChart::m_vTAUCTIONTREE;
MAPTMANTLECOORD CTChart::m_mapTMANTLECOORD;
MAPTFAMETITLE	CTChart::m_mapTFAMETITLE;
MAPTEQUIPCREATECHAR CTChart::m_mapTEquipCreateChar;
MAPTDYNAMICHELP CTChart::m_mapTDynamicHelp;
MAPTRPS CTChart::m_mapTRPS;

DWORD dwTCOLOR[TCOLOR_COUNT] =
{
	0, // TCOLOR_NONE
	D3DCOLOR_XRGB(0,0,0),
	D3DCOLOR_XRGB(51,51,51),
	D3DCOLOR_XRGB(102,102,102),
	D3DCOLOR_XRGB(153,153,153),
	D3DCOLOR_XRGB(204,204,204),
	D3DCOLOR_XRGB(255,255,255),
	D3DCOLOR_XRGB(255,248,163),
	D3DCOLOR_XRGB(255,255,81),
	D3DCOLOR_XRGB(255,219,0),
	D3DCOLOR_XRGB(233,140,56),
	D3DCOLOR_XRGB(255,131,0),
	D3DCOLOR_XRGB(135,85,70),
	D3DCOLOR_XRGB(107,57,0),
	D3DCOLOR_XRGB(141,95,43),
	D3DCOLOR_XRGB(30,35,31),
	D3DCOLOR_XRGB(0,48,64),
	D3DCOLOR_XRGB(36,76,41),
	D3DCOLOR_XRGB(0,91,76),
	D3DCOLOR_XRGB(0,136,2),
	D3DCOLOR_XRGB(87,184,70),
	D3DCOLOR_XRGB(194,226,84),
	D3DCOLOR_XRGB(199,234,251),
	D3DCOLOR_XRGB(134,165,255),
	D3DCOLOR_XRGB(0,181,233),
	D3DCOLOR_XRGB(44,110,213),
	D3DCOLOR_XRGB(58,71,196),
	D3DCOLOR_XRGB(108,122,224),
	D3DCOLOR_XRGB(99,85,246),
	D3DCOLOR_XRGB(0,74,128),
	D3DCOLOR_XRGB(0,54,99),
	D3DCOLOR_XRGB(0,33,87),
	D3DCOLOR_XRGB(13,0,76),
	D3DCOLOR_XRGB(75,0,73),
	D3DCOLOR_XRGB(82,39,121),
	D3DCOLOR_XRGB(144,23,128),
	D3DCOLOR_XRGB(199,21,133),
	D3DCOLOR_XRGB(220,1,131),
	D3DCOLOR_XRGB(224,62,156),
	D3DCOLOR_XRGB(250,66,81),
	D3DCOLOR_XRGB(255,102,102),
	D3DCOLOR_XRGB(253,71,21),
	D3DCOLOR_XRGB(242,21,28),
	D3DCOLOR_XRGB(214,35,70),
	D3DCOLOR_XRGB(121,0,0),
	D3DCOLOR_XRGB(100,35,29),
	D3DCOLOR_XRGB(51,0,0),
	D3DCOLOR_XRGB(59,53,50),
	D3DCOLOR_XRGB(183,178,109),
	D3DCOLOR_XRGB(118,211,193),
};

TMANTLECOORD TMANTLECOORD::m_vDefaultUV1_OLD( D3DXVECTOR2(0,0), // m_vLeftTop
		D3DXVECTOR2(127,0), // m_vRightTop
		D3DXVECTOR2(0,127), // m_vLeftBottom
		D3DXVECTOR2(127,127) ); // m_vRightBottom

TMANTLECOORD TMANTLECOORD::m_vDefaultUV2_OLD( D3DXVECTOR2(0, -58), // m_vLeftTop
		D3DXVECTOR2(127,-58), // m_vRightTop
		D3DXVECTOR2(0,-16), // m_vLeftBottom
		D3DXVECTOR2(127,-16) ); // m_vRightBottom

TMANTLECOORD TMANTLECOORD::m_vDefaultUV3_OLD( D3DXVECTOR2(0, 49), // m_vLeftTop
		D3DXVECTOR2(127,49), // m_vRightTop
		D3DXVECTOR2(0,95), // m_vLeftBottom
		D3DXVECTOR2(127,95) ); // m_vRightBottom

TMANTLECOORD TMANTLECOORD::m_vDefaultUV1( D3DXVECTOR2(0,0), // m_vLeftTop
		D3DXVECTOR2(255,0), // m_vRightTop
		D3DXVECTOR2(0,255), // m_vLeftBottom
		D3DXVECTOR2(255,255) ); // m_vRightBottom

TMANTLECOORD TMANTLECOORD::m_vDefaultUV2( D3DXVECTOR2(0, -116), // m_vLeftTop
		D3DXVECTOR2(255,-116), // m_vRightTop
		D3DXVECTOR2(0,-32), // m_vLeftBottom
		D3DXVECTOR2(255,-32) ); // m_vRightBottom

TMANTLECOORD TMANTLECOORD::m_vDefaultUV3( D3DXVECTOR2(0, 98), // m_vLeftTop
		D3DXVECTOR2(255,98), // m_vRightTop
		D3DXVECTOR2(0,190), // m_vLeftBottom
		D3DXVECTOR2(255,190) ); // m_vRightBottom

BOOL TMANTLECOORD::IsDefaultCoordUV1_OLD( struct tagTMANTLECOORD& uv1 )
{
	return uv1.m_vLeftTop == m_vDefaultUV1_OLD.m_vLeftTop &&
		uv1.m_vRightTop == m_vDefaultUV1_OLD.m_vRightTop &&
		uv1.m_vLeftBottom == m_vDefaultUV1_OLD.m_vLeftBottom &&
		uv1.m_vRightBottom == m_vDefaultUV1_OLD.m_vRightBottom;
}

BOOL TMANTLECOORD::IsDefaultCoordUV2_OLD( struct tagTMANTLECOORD& uv2 )
{
	return uv2.m_vLeftTop == m_vDefaultUV2_OLD.m_vLeftTop &&
		uv2.m_vRightTop == m_vDefaultUV2_OLD.m_vRightTop &&
		uv2.m_vLeftBottom == m_vDefaultUV2_OLD.m_vLeftBottom &&
		uv2.m_vRightBottom == m_vDefaultUV2_OLD.m_vRightBottom;
}

BOOL TMANTLECOORD::IsDefaultCoordUV3_OLD( struct tagTMANTLECOORD& uv3 )
{
	return uv3.m_vLeftTop == m_vDefaultUV3_OLD.m_vLeftTop &&
		uv3.m_vRightTop == m_vDefaultUV3_OLD.m_vRightTop &&
		uv3.m_vLeftBottom == m_vDefaultUV3_OLD.m_vLeftBottom &&
		uv3.m_vRightBottom == m_vDefaultUV3_OLD.m_vRightBottom;
}

BOOL TMANTLECOORD::IsDefaultCoordUV1( struct tagTMANTLECOORD& uv1 )
{
	return uv1.m_vLeftTop == m_vDefaultUV1.m_vLeftTop &&
		uv1.m_vRightTop == m_vDefaultUV1.m_vRightTop &&
		uv1.m_vLeftBottom == m_vDefaultUV1.m_vLeftBottom &&
		uv1.m_vRightBottom == m_vDefaultUV1.m_vRightBottom;
}

BOOL TMANTLECOORD::IsDefaultCoordUV2( struct tagTMANTLECOORD& uv2 )
{
	return uv2.m_vLeftTop == m_vDefaultUV2.m_vLeftTop &&
		uv2.m_vRightTop == m_vDefaultUV2.m_vRightTop &&
		uv2.m_vLeftBottom == m_vDefaultUV2.m_vLeftBottom &&
		uv2.m_vRightBottom == m_vDefaultUV2.m_vRightBottom;
}

BOOL TMANTLECOORD::IsDefaultCoordUV3( struct tagTMANTLECOORD& uv3 )
{
	return uv3.m_vLeftTop == m_vDefaultUV3.m_vLeftTop &&
		uv3.m_vRightTop == m_vDefaultUV3.m_vRightTop &&
		uv3.m_vLeftBottom == m_vDefaultUV3.m_vLeftBottom &&
		uv3.m_vRightBottom == m_vDefaultUV3.m_vRightBottom;
}

CString CTChart::ReadString( CFile *pFILE)
{
	TCHAR szRESULT[ 1024 + 1 ];
	int nLength;

	pFILE->Read( &nLength, sizeof(int));
	pFILE->Read( szRESULT, nLength);
	szRESULT[nLength] = NULL;

	return CString(szRESULT);
}

BOOL CTChart::IsHideDetectSkill( LPTSKILL pTSKILL )
{
	VTSKILLFUNCTION::iterator it, end;
	it = pTSKILL->m_vTFUNCTION.begin();
	end = pTSKILL->m_vTFUNCTION.end();

	for(; it != end ; ++it )
	{
		if( (*it)->m_bType == SDT_STATUS &&
			(*it)->m_bFunctionID == SDT_STATUS_HIDEDETECT )
			return TRUE;
	}

	return FALSE;
}

BOOL CTChart::InitTString( CString strPath )
{
	CFile file;
	if( file.Open( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary ) )
	{
		CTChart::ReleaseTString();

		CTChart::m_vTSTRING = new CString[ TSTR_COUNT ];
		CTChart::m_vTSTRING[ 0 ].Empty();

		CArchive ar( &file, CArchive::load);

		WORD wCount;
		ar >> wCount;

		for( WORD i=0; i<wCount; i++)
		{
			WORD wStringID;
			CString strString;

			ar	>> wStringID
				>> strString;

			strString.Replace( "\\n", "\n");

			if( wStringID == 0 )
			{
				m_strNationCode = strString;
				CTChart::m_vTSTRING[ 0 ].Empty();
			}
			else
			{
				if( wStringID > TSTR_NONE && wStringID < TSTR_COUNT )
					CTChart::m_vTSTRING[ wStringID ] = strString;
			}
		}

		ar.Close();
		return TRUE;
	}

	return FALSE;
}

void CTChart::ReleaseTString()
{
	if( CTChart::m_vTSTRING )
		delete [] CTChart::m_vTSTRING;
	CTChart::m_vTSTRING = NULL;
}

CString CTChart::LoadString( TSTRING v )
{
	if( TSTR_NONE > v || v >= TSTR_COUNT )
		return CString("");

	return CTChart::m_vTSTRING[ v ];
}

CString CTChart::Format( TSTRING v, ... )
{
	if( TSTR_NONE > v || v >= TSTR_COUNT )
		return CString("");

	CString strRESULT;
	
	va_list argList;
	va_start( argList, v );
	strRESULT.FormatV( CTChart::LoadString( v ), argList );
	va_end( argList );

	return strRESULT;
}

void CTChart::InitTSKILLTEMP( CString strPath )
{
	ReleaseTSKILLTEMP();
	
	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		LPTSKILL pTSKILL = new TSKILL();

		ar	>> pTSKILL->m_wSkillID									///스킬ID
			>> pTSKILL->m_strNAME									///스킬이름
			>> pTSKILL->m_wActSkillID								///이전성공스킬
			>> pTSKILL->m_wNeedItemID								///소모아이템
			>> pTSKILL->m_wDefSkillID								///대상이걸린버프
			>> pTSKILL->m_dwClassID									///직업코드
			>> pTSKILL->m_bKind										///특성구분
			>> pTSKILL->m_bPositive									///이로움 해로움
			>> pTSKILL->m_bLevel									///요구레벨
			>> pTSKILL->m_bLevelInc									///요구레벨증가
			>> pTSKILL->m_bMaxLevel									///최대스킬레벨
			>> pTSKILL->m_fPrice									///가격비율
			>> pTSKILL->m_bDuraSlot									///내구감소슬롯(추가)
			>> pTSKILL->m_dwWeaponID								///사용무기
			>> pTSKILL->m_wUseHP									///HP소모비율
			>> pTSKILL->m_wUseMP									///MP소모비율
			>> pTSKILL->m_bHitInit									///성공율초기값(추가)
			>> pTSKILL->m_bHitInc									///성공율증가값(추가)
			>> pTSKILL->m_dwSpellTick								///발동시간
			>> pTSKILL->m_bIsRide									///버프중 탈 수 없는 스킬
			>> pTSKILL->m_dwDelay									///재사용대기
			>> pTSKILL->m_nDelayInc									///재사용 대기 증감비율
			>> pTSKILL->m_dwGroupTick								///계열대기
			>> pTSKILL->m_dwInterval								///반복시간
			>> pTSKILL->m_bDelayType								///공속적용
			>> pTSKILL->m_wModeID									///태세
			>> pTSKILL->m_bTargetType								///사용대상 피아
			>> pTSKILL->m_bRangeType								///대상판별
			>> pTSKILL->m_fMinRange									///최소사정거리
			>> pTSKILL->m_fMaxRange									///최대사정기리
			>> pTSKILL->m_fAtkRange									///인식 폭,반경
			>> pTSKILL->m_fBufRange									///오라범위
			>> pTSKILL->m_dwDuration								///적용시간초기값
			>> pTSKILL->m_dwDurationInc								///적용시간증가비율	
			>> pTSKILL->m_bCanCancel								///사용자 취소 가능 여부(추가)
			>> pTSKILL->m_bContinue									///동작지속여부
			>> pTSKILL->m_wIconID									///아이콘ID
			>> pTSKILL->m_wItemID[0]								///발사아이템1
			>> pTSKILL->m_wItemID[1]								///발사아이템2
			>> pTSKILL->m_bActive									///패시브=0 액티브=1
			>> pTSKILL->m_bLoop										///자동반복여부
			>> pTSKILL->m_bActionID[TSKILLACTION_SPELL]				///주문동작
			>> pTSKILL->m_bActionID[TSKILLACTION_ACTIVATE]			///발동동작
			>> pTSKILL->m_bActionID[TSKILLACTION_MOVEACTIVATE]		///이동발동동작
			>> pTSKILL->m_bActionID[TSKILLACTION_DEFEND]			///방어동작
			>> pTSKILL->m_dwInfoID									///툴팁ID
			>> pTSKILL->m_dwSFX[TSKILLSFX_SPELL]					///시전이펙트
			>> pTSKILL->m_dwSFX[TSKILLSFX_ACT]						///발동이펙트
			>> pTSKILL->m_dwSFX[TSKILLSFX_DEFEND]					///타겟이펙트
			>> pTSKILL->m_dwSFX[TSKILLSFX_DEFENDACT]				///타겟발동이펙트
			>> pTSKILL->m_dwSFX[TSKILLSFX_LOOP]						///유지이펙트
			>> pTSKILL->m_dwSFX[TSKILLSFX_END]						///해제이펙트
			>> pTSKILL->m_dwSFX[TSKILLSFX_ARROW]					///발사체이펙트
			>> pTSKILL->m_bShowIcon									///아이콘보임
			>> pTSKILL->m_bShowTime									///남은시간보임
			>> pTSKILL->m_bShowCritical								///크리티컬적용
			>> pTSKILL->m_bUseInHold								///갇힘 상태에서 사용 가능여부
			>> pTSKILL->m_bStaticWhenDie							///사망시 버프 유지
			>> pTSKILL->m_fMoveDist;								///순간이동 거리

		CTChart::m_mapTSKILLTEMP.insert( make_pair( pTSKILL->m_wSkillID, pTSKILL) );
	}
}

void CTChart::ReleaseTSKILLTEMP()
{
	MAPTSKILLTEMP::iterator itTSKILL;

	for( itTSKILL = CTChart::m_mapTSKILLTEMP.begin(); itTSKILL != CTChart::m_mapTSKILLTEMP.end(); itTSKILL++)
		delete itTSKILL->second;

	CTChart::m_mapTSKILLTEMP.clear();
}

LPTSKILL CTChart::FindTSKILLTEMP( WORD wSkillID)
{
	MAPTSKILLTEMP::iterator finder = CTChart::m_mapTSKILLTEMP.find(wSkillID);

	if( finder != CTChart::m_mapTSKILLTEMP.end() )
		return finder->second;

	return NULL;
}

BOOL CTChart::IsNegativeSkill( LPTSKILL pTSKILL )
{
	return pTSKILL && !( pTSKILL->m_bPositive % 2 );
}

void CTChart::InitTSKILLFUNCTION( CString strPath )
{
	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		LPTSKILLFUNCTION pTFUNCTION = new TSKILLFUNCTION();
		WORD wSkillID;

		ar	>> wSkillID						///일련번호
			>> pTFUNCTION->m_bMethodID		///적용방식
			>> pTFUNCTION->m_bType			///효과종류
			>> pTFUNCTION->m_bFunctionID	///효과내용
			>> pTFUNCTION->m_bOPCode		///수치적용
			>> pTFUNCTION->m_bCalc			///성능계산(추가)
			>> pTFUNCTION->m_wValue			///초기값
			>> pTFUNCTION->m_wInc;			///증가값

		LPTSKILL pTSKILL = CTChart::FindTSKILLTEMP(wSkillID);
		if(pTSKILL)
		{
			if( pTFUNCTION->m_bType == SDT_STATUS && pTFUNCTION->m_bFunctionID == SDT_STATUS_MARK )
				pTSKILL->m_bPursuit = TRUE;

			pTSKILL->m_vTFUNCTION.push_back(pTFUNCTION);
		}
		else
			delete pTFUNCTION;
	}
}

void CTChart::InitTSKILLPOINT( CString strPath )
{
	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		WORD wSkillID;

		BYTE bGroupPoint;
		BYTE bSkillPoint;
		BYTE bPrevLevel;
		BYTE bLevel;

		ar	>> wSkillID
			>> bLevel
			>> bSkillPoint
			>> bGroupPoint
			>> bPrevLevel;

		LPTSKILL pTSKILL = CTChart::FindTSKILLTEMP(wSkillID);
		if( pTSKILL &&
			pTSKILL->m_bMaxLevel > 0 &&
			bLevel > 0 &&
			pTSKILL->m_bMaxLevel + 1 > bLevel )
		{
			if(!pTSKILL->m_vTUPGRADEINFO)
				pTSKILL->m_vTUPGRADEINFO = new TSKILLUPGRADEINFO[pTSKILL->m_bMaxLevel];

			pTSKILL->m_vTUPGRADEINFO[bLevel - 1].m_bGroupPoint = bGroupPoint;
			pTSKILL->m_vTUPGRADEINFO[bLevel - 1].m_bSkillPoint = bSkillPoint;
			pTSKILL->m_vTUPGRADEINFO[bLevel - 1].m_bPrevLevel = bPrevLevel;
		}
	}
}

void CTChart::InitTARROWDIR( CString strPath )
{
	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		WORD wSkillID;
		FLOAT fDIR;

		ar	>> wSkillID
			>> fDIR;

		LPTSKILL pTSKILL = CTChart::FindTSKILLTEMP(wSkillID);
		if( pTSKILL )
			pTSKILL->m_vTARROWDIR.push_back(fDIR);
	}
}

void CTChart::InitTCOUNTRYINFO( CString strPath )
{
	ReleaseTCOUNTRYINFO();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		CString *pTEXT = new CString();
		DWORD dwID;

		ar	>> (*pTEXT)
			>> dwID;

		CTChart::m_mapTCOUNTRYINFO.insert( MAPTSTRING::value_type( dwID, pTEXT));
	}
}

void CTChart::ReleaseTCOUNTRYINFO()
{
	MAPTSTRING::iterator itINFO;

	for( itINFO = CTChart::m_mapTCOUNTRYINFO.begin(); itINFO != CTChart::m_mapTCOUNTRYINFO.end(); itINFO++)
		delete itINFO->second;

	CTChart::m_mapTCOUNTRYINFO.clear();
}

CString CTChart::GetTCOUNTRYINFO( DWORD dwID )
{
	MAPTSTRING::iterator finder = CTChart::m_mapTCOUNTRYINFO.find( dwID );

	if( finder != CTChart::m_mapTCOUNTRYINFO.end() )
		return *(finder->second);

	return CString("");
}

void CTChart::InitTRACEINFO( CString strPath )
{
	ReleaseTRACEINFO();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		CString *pTEXT = new CString();
		DWORD dwID;

		ar	>> (*pTEXT)
			>> dwID;

		CTChart::m_mapTRACEINFO.insert( MAPTSTRING::value_type( dwID, pTEXT));
	}
}

void CTChart::ReleaseTRACEINFO()
{
	MAPTSTRING::iterator itINFO;

	for( itINFO = CTChart::m_mapTRACEINFO.begin(); itINFO != CTChart::m_mapTRACEINFO.end(); itINFO++)
		delete (*itINFO).second;

	CTChart::m_mapTRACEINFO.clear();
}

CString CTChart::GetTRACEINFO( DWORD dwID )
{
	MAPTSTRING::iterator finder = CTChart::m_mapTRACEINFO.find( dwID );
	
	if( finder != CTChart::m_mapTRACEINFO.end() )
		return *(finder->second);
	
	return CString("");
}

void CTChart::InitTCLASSINFO( CString strPath )
{
	ReleaseTCLASSINFO();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		CString *pTEXT = new CString();
		DWORD dwID;

		ar	>> (*pTEXT)
			>> dwID;

		CTChart::m_mapTCLASSINFO.insert( MAPTSTRING::value_type( dwID, pTEXT));
	}
}

void CTChart::ReleaseTCLASSINFO()
{
	MAPTSTRING::iterator itINFO;

	for( itINFO = CTChart::m_mapTCLASSINFO.begin(); itINFO != CTChart::m_mapTCLASSINFO.end(); itINFO++)
		delete (*itINFO).second;

	CTChart::m_mapTCLASSINFO.clear();
}

CString CTChart::GetTCLASSINFO( DWORD dwID )
{
	MAPTSTRING::iterator finder = CTChart::m_mapTCLASSINFO.find( dwID );

	if( finder != CTChart::m_mapTCLASSINFO.end() )
		return *(finder->second);

	return CString("");
}

void CTChart::InitTDETAILINFO( CString strPath )
{
	ReleaseTDETAILINFO();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		LPTDETAILINFO pINFO = new TDETAILINFO();
		DWORD dwID;

		ar	>> dwID
			>> pINFO->m_bCount;

		if(pINFO->m_bCount)
		{
			pINFO->m_pTEXT = new CString[pINFO->m_bCount];
			for( BYTE j=0; j<pINFO->m_bCount; ++j )
				ar >> pINFO->m_pTEXT[j];
		}

	#ifdef DEBUG
		if( CTChart::FindTDETAILINFO( dwID ) )
		{
			TRACE("경고 : 인포 아이디 중복(%u)\n", dwID);
			delete pINFO;
			return;
		}
	#endif

		CTChart::m_mapTDETAILINFO.insert( std::make_pair( dwID, pINFO) );
	}
}

void CTChart::ReleaseTDETAILINFO()
{
	MAPTDETAILINFO::iterator itr,end;
	itr = CTChart::m_mapTDETAILINFO.begin();
	end = CTChart::m_mapTDETAILINFO.end();

	for(; itr!=end; ++itr)
		delete itr->second;

	CTChart::m_mapTDETAILINFO.empty();
}

LPTDETAILINFO CTChart::FindTDETAILINFO( DWORD dwInfoID )
{
	MAPTDETAILINFO::iterator finder = CTChart::m_mapTDETAILINFO.find( dwInfoID );

	if( finder != CTChart::m_mapTDETAILINFO.end() )
		return finder->second;

	return NULL;
}

void CTChart::InitTREGIONINFO( CString strPath )
{
	ReleaseTREGIONINFO();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		LPTREGIONINFO pTREGION = new TREGIONINFO();

		ar	>> pTREGION->m_dwID
			>> pTREGION->m_strNAME
			>> pTREGION->m_bContryID
			>> pTREGION->m_bCanFly
			>> pTREGION->m_wLocalID
			>> pTREGION->m_vTHUMB[TCONTRY_D].x
			>> pTREGION->m_vTHUMB[TCONTRY_D].y
			>> pTREGION->m_vTHUMB[TCONTRY_D].z
			>> pTREGION->m_vTHUMB[TCONTRY_C].x
			>> pTREGION->m_vTHUMB[TCONTRY_C].y
			>> pTREGION->m_vTHUMB[TCONTRY_C].z
			>> pTREGION->m_vTHUMB[TCONTRY_B].x
			>> pTREGION->m_vTHUMB[TCONTRY_B].y
			>> pTREGION->m_vTHUMB[TCONTRY_B].z
			>> pTREGION->m_bCanMail
			>> pTREGION->m_dwTENVSND_LOOP
			>> pTREGION->m_dwBGMID
			>> pTREGION->m_dwENVID
			>> pTREGION->m_bInfo;

		MAPTBGM::iterator itBGM = CTChart::m_mapTBGMINFO.find(pTREGION->m_dwBGMID);
		if( itBGM != CTChart::m_mapTBGMINFO.end() )
			pTREGION->m_pVTBGM = (*itBGM).second;
		else
			pTREGION->m_pVTBGM = NULL;

		MAPTENV::iterator itENV = CTChart::m_mapTENVINFO.find(pTREGION->m_dwENVID);
		if( itENV != CTChart::m_mapTENVINFO.end() )
			pTREGION->m_pVTENV = (*itENV).second;
		else
			pTREGION->m_pVTENV = NULL;

#ifdef _DEBUG
		if( CTChart::m_mapTREGIONINFO.find(pTREGION->m_dwID) != CTChart::m_mapTREGIONINFO.end() )
		{
			TRACE("WARNING : Duplicated ID[%u] In CTChart::InitTREGIONINFO\n", pTREGION->m_dwID);
			delete pTREGION;
			continue;
		}
#endif

		CTChart::m_mapTREGIONINFO.insert( MAPTREGIONINFO::value_type( pTREGION->m_dwID, pTREGION));
	}
}

void CTChart::ReleaseTREGIONINFO()
{
	MAPTREGIONINFO::iterator itTREGION;

	for( itTREGION = CTChart::m_mapTREGIONINFO.begin(); itTREGION != CTChart::m_mapTREGIONINFO.end(); itTREGION++)
		delete (*itTREGION).second;

	CTChart::m_mapTREGIONINFO.clear();
}

LPTREGIONINFO CTChart::FindTREGIONINFO( DWORD dwRegionID)
{
	MAPTREGIONINFO::iterator finder = CTChart::m_mapTREGIONINFO.find(dwRegionID);

	if( finder != CTChart::m_mapTREGIONINFO.end() )
		return (*finder).second;

	return NULL;
}

void CTChart::InitTBGM( CString strPath )
{
	CTChart::ReleaseTBGM();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		DWORD dwID;
		DWORD dwBGMID;

		ar	>> dwID
			>> dwBGMID;

		MAPTBGM::iterator itr;
		itr = CTChart::m_mapTBGMINFO.find( dwID );
		if( itr != CTChart::m_mapTBGMINFO.end() )
		{
			(itr->second)->push_back( dwBGMID);
		}
		else
		{
			std::pair<MAPTBGM::iterator,bool> result =
				CTChart::m_mapTBGMINFO.insert( MAPTBGM::value_type( dwID, new VECTORDWORD ) );

			if( result.second )
				(result.first->second)->push_back( dwBGMID);
		}
	}
}

void CTChart::ReleaseTBGM()
{
	MAPTBGM::iterator itr, end;
	itr = CTChart::m_mapTBGMINFO.begin();
	end = CTChart::m_mapTBGMINFO.end();
	
	for(;itr!=end; ++itr)
		delete itr->second;

	CTChart::m_mapTBGMINFO.clear();
}

void CTChart::InitTENV( CString strPath )
{
	CTChart::ReleaseTENV();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		DWORD dwID;
		DWORD dwENVID;

		ar	>> dwID
			>> dwENVID;
		
		MAPTENV::iterator itr;
		itr = CTChart::m_mapTENVINFO.find( dwID );
		if( itr != CTChart::m_mapTENVINFO.end() )
		{
			(itr->second)->push_back( dwENVID);
		}
		else
		{
			std::pair<MAPTENV::iterator,bool> result =
				CTChart::m_mapTENVINFO.insert( MAPTENV::value_type( dwID, new VECTORDWORD ) );

			if( result.second )
				(result.first->second)->push_back( dwENVID);
		}
	}
}

void CTChart::ReleaseTENV()
{
	MAPTENV::iterator itr, end;
	itr = CTChart::m_mapTENVINFO.begin();
	end = CTChart::m_mapTENVINFO.end();
	
	for(;itr!=end; ++itr)
		delete itr->second;

	CTChart::m_mapTENVINFO.clear();
}

void CTChart::InitTACTIONTEMP( CString strPath )
{
	ReleaseTACTIONTEMP();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		LPTACTION pTACTION = new TACTION();
		DWORD dwActionID;

		ar	>> dwActionID
			>> pTACTION->m_dwActID
			>> pTACTION->m_bEquipMode
			>> pTACTION->m_dwTSLASH;

	#ifdef _DEBUG
		if( CTChart::m_mapTACTIONTEMP.find(dwActionID) != CTChart::m_mapTACTIONTEMP.end() )
		{
			TRACE("Duplicated ID[%u] In CTChart::InitTACTIONTEMP\n", dwActionID);
			continue;
		}
	#endif

		CTChart::m_mapTACTIONTEMP.insert( MAPTACTION::value_type( dwActionID, pTACTION));
	}
}

void CTChart::ReleaseTACTIONTEMP()
{
	MAPTACTION::iterator itTACTION;

	for( itTACTION = CTChart::m_mapTACTIONTEMP.begin(); itTACTION != CTChart::m_mapTACTIONTEMP.end(); itTACTION++)
		delete (*itTACTION).second;

	CTChart::m_mapTACTIONTEMP.clear();
}

LPTACTION CTChart::FindTACTIONTEMP( BYTE bActionID, BYTE bWeaponID )
{
	MAPTACTION::iterator finder = CTChart::m_mapTACTIONTEMP.find( MAKEWORD( bWeaponID, bActionID) );

	if( finder != CTChart::m_mapTACTIONTEMP.end() )
		return finder->second;

	return NULL;
}

void CTChart::InitTACTIONDATA( CString strPath )
{
	ReleaseTACTIONDATA();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		LPTACTIONDATA pTDATA = new TACTIONDATA();

		ar	>> pTDATA->m_strActCmd
			>> pTDATA->m_bLevel
			>> pTDATA->m_bSubAction
			>> pTDATA->m_bLoopAction
			>> pTDATA->m_bContinue
			>> pTDATA->m_bCancel
			>> pTDATA->m_bNavAction
			>> pTDATA->m_bSkipMain
			>> pTDATA->m_bHideOnCapeMode;

		pTDATA->m_strActCmd.TrimLeft();
		pTDATA->m_strActCmd.TrimRight();
		pTDATA->m_strActCmd.MakeUpper();

		CTChart::m_vTACTION.push_back(pTDATA);
	}
}

void CTChart::ReleaseTACTIONDATA()
{
	while(!CTChart::m_vTACTION.empty())
	{
		delete CTChart::m_vTACTION.back();
		CTChart::m_vTACTION.pop_back();
	}
}

LPTACTIONDATA CTChart::GetTACTION( INT nAction )
{
	if( m_vTACTION.size() <= nAction || nAction < 0 )
		return m_vTACTION[ TA_STAND ];

	return m_vTACTION[ nAction ];
}

void CTChart::InitTFACETEMP( CString strPath )
{
	for( int i=0; i<TFACE_COUNT; i++)
		for( int j=0; j<THAIR_COUNT; j++)
		{
			CTChart::m_vTFACE[i][j].m_bHideSlotID = 0xFF;
			CTChart::m_vTFACE[i][j].m_bHidePartID = 0xFF;
			CTChart::m_vTFACE[i][j].m_bHideRaceID = 0;

			CTChart::m_vTFACE[i][j].m_dwCLK = 0;
			CTChart::m_vTFACE[i][j].m_dwCL = 0;
			CTChart::m_vTFACE[i][j].m_dwMESH = 0;
		}

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( i=0; i<wCount; i++)
	{
		BYTE bFACE;
		BYTE bHAIR;

		ar	>> bFACE
			>> bHAIR;

		ar	>> CTChart::m_vTFACE[bFACE][bHAIR].m_dwCLK
			>> CTChart::m_vTFACE[bFACE][bHAIR].m_dwCL
			>> CTChart::m_vTFACE[bFACE][bHAIR].m_dwMESH
			>> CTChart::m_vTFACE[bFACE][bHAIR].m_bHideSlotID
			>> CTChart::m_vTFACE[bFACE][bHAIR].m_bHidePartID
			>> CTChart::m_vTFACE[bFACE][bHAIR].m_bHideRaceID;
	}
}

void CTChart::InitTPANTSTEMP( CString strPath )
{
	for( int i=0; i<TPANTS_COUNT; i++)
	{
		CTChart::m_vTPANTS[i].m_bHideSlotID = 0xFF;
		CTChart::m_vTPANTS[i].m_bHidePartID = 0xFF;
		CTChart::m_vTPANTS[i].m_bHideRaceID = 0;

		CTChart::m_vTPANTS[i].m_dwCLK = 0;
		CTChart::m_vTPANTS[i].m_dwCL = 0;
		CTChart::m_vTPANTS[i].m_dwMESH = 0;
	}

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( i=0; i<wCount; i++)
	{
		BYTE bPantsID;
		ar >> bPantsID;

		ar	>> CTChart::m_vTPANTS[bPantsID].m_dwCLK
			>> CTChart::m_vTPANTS[bPantsID].m_dwCL
			>> CTChart::m_vTPANTS[bPantsID].m_dwMESH
			>> CTChart::m_vTPANTS[bPantsID].m_bHideSlotID
			>> CTChart::m_vTPANTS[bPantsID].m_bHidePartID
			>> CTChart::m_vTPANTS[bPantsID].m_bHideRaceID;
	}
}

void CTChart::InitTBODYTEMP( CString strPath )
{
	for( int i=0; i<TBODY_COUNT; i++)
	{
		CTChart::m_vTBODY[i].m_bHideSlotID = 0xFF;
		CTChart::m_vTBODY[i].m_bHidePartID = 0xFF;
		CTChart::m_vTBODY[i].m_bHideRaceID = 0;

		CTChart::m_vTBODY[i].m_dwCLK = 0;
		CTChart::m_vTBODY[i].m_dwCL = 0;
		CTChart::m_vTBODY[i].m_dwMESH = 0;
	}

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( i=0; i<wCount; i++)
	{
		BYTE bBODY;
		ar >> bBODY;

		ar	>> CTChart::m_vTBODY[bBODY].m_dwCLK
			>> CTChart::m_vTBODY[bBODY].m_dwCL
			>> CTChart::m_vTBODY[bBODY].m_dwMESH
			>> CTChart::m_vTBODY[bBODY].m_bHideSlotID
			>> CTChart::m_vTBODY[bBODY].m_bHidePartID
			>> CTChart::m_vTBODY[bBODY].m_bHideRaceID;
	}
}

void CTChart::InitTHANDTEMP( CString strPath )
{
	for( int i=0; i<THAND_COUNT; i++)
	{
		CTChart::m_vTHAND[i].m_bHideSlotID = 0xFF;
		CTChart::m_vTHAND[i].m_bHidePartID = 0xFF;
		CTChart::m_vTHAND[i].m_bHideRaceID = 0;

		CTChart::m_vTHAND[i].m_dwCLK = 0;
		CTChart::m_vTHAND[i].m_dwCL = 0;
		CTChart::m_vTHAND[i].m_dwMESH = 0;
	}

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( i=0; i<wCount; i++)
	{
		BYTE bHAND;
		ar >> bHAND;

		ar	>> CTChart::m_vTHAND[bHAND].m_dwCLK
			>> CTChart::m_vTHAND[bHAND].m_dwCL
			>> CTChart::m_vTHAND[bHAND].m_dwMESH
			>> CTChart::m_vTHAND[bHAND].m_bHideSlotID
			>> CTChart::m_vTHAND[bHAND].m_bHidePartID
			>> CTChart::m_vTHAND[bHAND].m_bHideRaceID;
	}
}

void CTChart::InitTFOOTTEMP( CString strPath )
{
	for( int i=0; i<TFOOT_COUNT; i++)
	{
		CTChart::m_vTFOOT[i].m_bHideSlotID = 0xFF;
		CTChart::m_vTFOOT[i].m_bHidePartID = 0xFF;
		CTChart::m_vTFOOT[i].m_bHideRaceID = 0;

		CTChart::m_vTFOOT[i].m_dwCLK = 0;
		CTChart::m_vTFOOT[i].m_dwCL = 0;
		CTChart::m_vTFOOT[i].m_dwMESH = 0;
	}

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( i=0; i<wCount; i++)
	{
		BYTE bFOOT;
		ar >> bFOOT;

		ar	>> CTChart::m_vTFOOT[bFOOT].m_dwCLK
			>> CTChart::m_vTFOOT[bFOOT].m_dwCL
			>> CTChart::m_vTFOOT[bFOOT].m_dwMESH
			>> CTChart::m_vTFOOT[bFOOT].m_bHideSlotID
			>> CTChart::m_vTFOOT[bFOOT].m_bHidePartID
			>> CTChart::m_vTFOOT[bFOOT].m_bHideRaceID;
	}
}

void CTChart::InitTSKILLTREE( CString strPath )
{
	ReleaseTSKILLTREE();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		BYTE bContryID;
		BYTE bClassID;
		BYTE bTabID;
		BYTE bTreeID;
		WORD wSkillID;

		ar	>> bContryID
			>> bClassID
			>> bTabID
			>> bTreeID
			>> wSkillID;

		CTChart::m_mapTSKILLTREE[bContryID][bClassID][bTabID].insert( MAPWORD::value_type(
			bTreeID,
			wSkillID));
	}
}

void CTChart::ReleaseTSKILLTREE()
{
	for( BYTE i=0; i<TCONTRY_COUNT; i++)
		for( BYTE j=0; j<TCLASS_COUNT; j++)
			for( BYTE k=0; k<TSKILLTAB_COUNT; k++)
				CTChart::m_mapTSKILLTREE[i][j][k].clear();
}

WORD CTChart::GetTSKILLID( BYTE bContryID, BYTE bClassID, BYTE bTabID, BYTE bTreeIndex )
{
	MAPWORD::iterator finder = CTChart::m_mapTSKILLTREE[bContryID][bClassID][bTabID].find( bTreeIndex );

	if( finder != CTChart::m_mapTSKILLTREE[bContryID][bClassID][bTabID].end() )
		return (*finder).second;

	return 0;
}

void CTChart::InitTPOPUPMENU( CString strPath )
{
	ReleaseTPOPUPMENU();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		DWORD dwPopupID;
		DWORD dwItemID;

		ar	>> dwPopupID
			>> dwItemID;

		LPTPOPUPMENU pTMENU = CTChart::FindTPOPUPMENU(dwPopupID);
		LPTPOPUPITEM pTITEM = CTChart::FindTPOPUPITEM(dwItemID);

		if(pTITEM)
		{
			if(!pTMENU)
			{
				pTMENU = new TPOPUPMENU();
				pTMENU->m_dwID = dwPopupID;

				CTChart::m_mapTPOPUP.insert( MAPTPOPUPMENU::value_type( dwPopupID, pTMENU));
			}

			pTMENU->m_vTPOPUP.push_back(pTITEM);
		}
	}
}

void CTChart::ReleaseTPOPUPMENU()
{
	MAPTPOPUPMENU::iterator itTMENU;

	for( itTMENU = CTChart::m_mapTPOPUP.begin(); itTMENU != CTChart::m_mapTPOPUP.end(); itTMENU++)
		delete (*itTMENU).second;

	CTChart::m_mapTPOPUP.clear();
}

LPTPOPUPMENU CTChart::FindTPOPUPMENU( DWORD dwPopupID)
{
	MAPTPOPUPMENU::iterator finder = CTChart::m_mapTPOPUP.find(dwPopupID);

	if( finder != CTChart::m_mapTPOPUP.end() )
		return (*finder).second;

	return NULL;
}

void CTChart::InitTPOPUPITEM( CString strPath )
{
	ReleaseTPOPUPITEM();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		LPTPOPUPITEM pTITEM = new TPOPUPITEM();
		DWORD dwID;

		ar	>> pTITEM->m_strTITLE
			>> dwID
			>> pTITEM->m_dwMenuID
			>> pTITEM->m_bGhost;

		if( CTChart::m_mapTMENUITEM.find( dwID ) == CTChart::m_mapTMENUITEM.end() )
		{
			CTChart::m_mapTMENUITEM.insert( MAPTPOPUPITEM::value_type( dwID, pTITEM));
		}
		else
		{
			delete pTITEM;
			TRACE("경고 : %s에 중복된 아이디가 존재합니다. (ID:%u)\n", strPath, dwID);
		}
	}
}

void CTChart::ReleaseTPOPUPITEM()
{
	MAPTPOPUPITEM::iterator itTITEM;

	for( itTITEM = CTChart::m_mapTMENUITEM.begin(); itTITEM != CTChart::m_mapTMENUITEM.end(); itTITEM++)
		delete (*itTITEM).second;

	CTChart::m_mapTMENUITEM.clear();
}

LPTPOPUPITEM CTChart::FindTPOPUPITEM( DWORD dwMenuID)
{
	MAPTPOPUPITEM::iterator finder = CTChart::m_mapTMENUITEM.find(dwMenuID);

	if( finder != CTChart::m_mapTMENUITEM.end() )
		return (*finder).second;

	return NULL;
}

void CTChart::InitTMAPJOINT( CString strPath )
{
	ReleaseTMAPJOINT();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		LPTMAPJOINT pTJOINT = new TMAPJOINT();
		LPVTMAPJOINT pVTMAPJOINT = NULL;

		DWORD dwID;
		ar >> dwID;

		MAPVTMAPJOINT::iterator finder = CTChart::m_mapTMAPJOINT.find(dwID);
		if( finder == CTChart::m_mapTMAPJOINT.end() )
		{
			pVTMAPJOINT = new VTMAPJOINT();
			CTChart::m_mapTMAPJOINT.insert( MAPVTMAPJOINT::value_type( dwID, pVTMAPJOINT));
		}
		else
			pVTMAPJOINT = (*finder).second;

		ar	>> pTJOINT->m_vRECT.left
			>> pTJOINT->m_vRECT.top
			>> pTJOINT->m_vRECT.right
			>> pTJOINT->m_vRECT.bottom
			>> pTJOINT->m_bJOINT;

		pVTMAPJOINT->push_back(pTJOINT);
	}
}

void CTChart::ReleaseTMAPJOINT()
{
	MAPVTMAPJOINT::iterator itTJOINT;

	for( itTJOINT = CTChart::m_mapTMAPJOINT.begin(); itTJOINT != CTChart::m_mapTMAPJOINT.end(); itTJOINT++)
	{
		LPVTMAPJOINT pVTMAPJOINT = (*itTJOINT).second;

		while(!pVTMAPJOINT->empty())
		{
			delete pVTMAPJOINT->back();
			pVTMAPJOINT->pop_back();
		}

		delete pVTMAPJOINT;
	}

	CTChart::m_mapTMAPJOINT.clear();
}

LPVTMAPJOINT CTChart::GetTMAPJOINT( WORD wMapID,
										   BYTE bUnitX,
										   BYTE bUnitZ)
{
	MAPVTMAPJOINT::iterator finder = CTChart::m_mapTMAPJOINT.find(MAKELONG( MAKEWORD( bUnitX, bUnitZ), wMapID));

	if( finder != CTChart::m_mapTMAPJOINT.end() )
		return (*finder).second;

	return NULL;
}

void CTChart::InitTMAPINFO( CString strNodeChartPath, CString strMapCharPath )
{
	ReleaseTMAPINFO();

	MAPTMAPINFO mapTNODE;
	mapTNODE.clear();

	CFile vTNODE( LPCSTR(strNodeChartPath), CFile::modeRead|CFile::typeBinary);
	CArchive vTNODECHART( &vTNODE, CArchive::load);

	WORD wCount;

	vTNODECHART
		>> wCount;

	MAPTMAPINFO::iterator itTNODE;
	for( WORD i=0; i<wCount; i++)
	{
		LPD3DXVECTOR3 pPOS = new D3DXVECTOR3();
		LPTMAPINFO pTNODE = NULL;

		DWORD dwNodeID;
		DWORD dwMapID;

		vTNODECHART
			>> dwMapID
			>> dwNodeID
			>> pPOS->x
			>> pPOS->y
			>> pPOS->z;

		itTNODE = mapTNODE.find(dwMapID);
		if( itTNODE == mapTNODE.end() )
		{
			pTNODE = new TMAPINFO;
			mapTNODE.insert( MAPTMAPINFO::value_type( dwMapID, pTNODE));
		}
		else
			pTNODE = (*itTNODE).second;

		pTNODE->m_mapTNODE.insert( MAPD3DXVECTOR3::value_type( dwNodeID, pPOS));
	}

	CFile vTMAP( LPCSTR(strMapCharPath), CFile::modeRead|CFile::typeBinary);
	CArchive vTMAPCHART( &vTMAP, CArchive::load);

	vTMAPCHART
		>> wCount;

	for( i=0; i<wCount; i++)
	{
		LPTMAPINFO pTMAPINFO = new TMAPINFO();
		DWORD dwMapID;

		FLOAT fScaleX;
		FLOAT fScaleY;
		FLOAT fScaleZ;

		FLOAT fPosX;
		FLOAT fPosY;
		FLOAT fPosZ;

		FLOAT fRotX;
		FLOAT fRotY;
		FLOAT fRotZ;

		vTMAPCHART
			>> dwMapID
			>> pTMAPINFO->m_dwMapID
			>> fScaleX
			>> fScaleY
			>> fScaleZ
			>> fPosX
			>> fPosY
			>> fPosZ
			>> fRotX
			>> fRotY
			>> fRotZ
			>> pTMAPINFO->m_bDungeon
			>> pTMAPINFO->m_bNPCCALL;

		itTNODE = mapTNODE.find(pTMAPINFO->m_dwMapID);
		if( itTNODE != mapTNODE.end() )
		{
			MAPD3DXVECTOR3::iterator itTPOS;
			D3DXMATRIX vPOS;
			D3DXMATRIX vROT;

			D3DXMatrixScaling(
				&vPOS,
				fScaleX,
				fScaleY,
				fScaleZ);

			D3DXMatrixRotationX(
				&vROT,
				fRotX);
			vPOS *= vROT;

			D3DXMatrixRotationY(
				&vROT,
				fRotY);
			vPOS *= vROT;

			D3DXMatrixRotationZ(
				&vROT,
				fRotZ);
			vPOS *= vROT;

			vPOS._41 = fPosX;
			vPOS._42 = fPosY;
			vPOS._43 = fPosZ;

			for( itTPOS = (*itTNODE).second->m_mapTNODE.begin(); itTPOS != (*itTNODE).second->m_mapTNODE.end(); itTPOS++)
			{
				LPD3DXVECTOR3 pPOS = new D3DXVECTOR3(
					(*itTPOS).second->x,
					(*itTPOS).second->y,
					(*itTPOS).second->z);

				CTMath::Transform(
					&vPOS,
					pPOS);

				pTMAPINFO->m_mapTNODE.insert( MAPD3DXVECTOR3::value_type( (*itTPOS).first, pPOS));
			}
		}

		CTChart::m_mapTMAPINFO.insert( MAPTMAPINFO::value_type( dwMapID, pTMAPINFO));
	}

	for( itTNODE = mapTNODE.begin(); itTNODE != mapTNODE.end(); itTNODE++)
		delete (*itTNODE).second;
	mapTNODE.clear();
}

void CTChart::ReleaseTMAPINFO()
{
	MAPTMAPINFO::iterator itTMAP;

	for( itTMAP = CTChart::m_mapTMAPINFO.begin(); itTMAP != CTChart::m_mapTMAPINFO.end(); itTMAP++)
		delete (*itTMAP).second;

	CTChart::m_mapTMAPINFO.clear();
}

LPTMAPINFO CTChart::GetTMAPINFO( DWORD dwMapID)
{
	MAPTMAPINFO::iterator finder = CTChart::m_mapTMAPINFO.find(dwMapID);

	if( finder != CTChart::m_mapTMAPINFO.end() )
		return (*finder).second;

	return NULL;
}

void CTChart::InitTPORTAL( CString strPath )
{
	ReleaseTPORTAL();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		LPTPORTAL pTPORTAL = new TPORTAL();

		ar	>> pTPORTAL->m_wPortalID
			>> pTPORTAL->m_strNAME
			>> pTPORTAL->m_wPortalRegionID;

		CTChart::m_mapTPORTALINFO.insert( MAPTPORTAL::value_type( pTPORTAL->m_wPortalID, pTPORTAL));
	}
}

void CTChart::ReleaseTPORTAL()
{
	MAPTPORTAL::iterator itTPORTAL;

	for( itTPORTAL = CTChart::m_mapTPORTALINFO.begin(); itTPORTAL != CTChart::m_mapTPORTALINFO.end(); itTPORTAL++)
		delete (*itTPORTAL).second;

	CTChart::m_mapTPORTALINFO.clear();
}

LPTPORTAL CTChart::GetTPORTAL( WORD wPortalID)
{
	MAPTPORTAL::iterator finder = CTChart::m_mapTPORTALINFO.find(wPortalID);

	if( finder != CTChart::m_mapTPORTALINFO.end() )
		return (*finder).second;

	return NULL;
}

void CTChart::InitTITEMTEMP( CString strPath )
{
	ReleaseTITEMTEMP();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		LPTITEM pTITEM = new TITEM();

		ar	>> pTITEM->m_wItemID				///일련번호
			>> pTITEM->m_bType					///종류
			>> pTITEM->m_bKind					///구분
			>> pTITEM->m_wAttrID				///성능일련번호
			>> pTITEM->m_strNAME				///이름
			>> pTITEM->m_wUseValue				///사용효과값
			>> pTITEM->m_dwSlotID				///장착위치
			>> pTITEM->m_dwClassID				///사용직업
			>> pTITEM->m_bPrmSlotID				///주무기장착위치
			>> pTITEM->m_bSubSlotID				///보조무기장착위치
			>> pTITEM->m_bLevel					///필요레벨
			>> pTITEM->m_bCanRepair				///수리여부(추가)
			>> pTITEM->m_dwDuraMax				///최대내구(추가)
			>> pTITEM->m_bRefineMax				///제련횟수(추가)
			>> pTITEM->m_fPriceRate				///가격비율
			>> pTITEM->m_dwPrice				///기준가격
			>> pTITEM->m_bMinRange				///최소사정거리
			>> pTITEM->m_bMaxRange				///최대사정거리
			>> pTITEM->m_bStack					///최대수량
			>> pTITEM->m_bSlotCount				///슬롯갯수
			>> pTITEM->m_bCanGamble				///봉인 생성여부
			>> pTITEM->m_bGambleProb			///치환여부(추가)
			>> pTITEM->m_bDestoryProb			///소멸여부(추가)
			>> pTITEM->m_bCanGrade				///등급 가능여부
			>> pTITEM->m_bCanMagic				///마법 생성여부
			>> pTITEM->m_bCanRare				///희귀 생성여부
			>> pTITEM->m_wDelayGroupID			///재사용대기그룹
			>> pTITEM->m_dwDelay				///재사용대기시간
			>> pTITEM->m_bCanTrade				///거래,판매 가능 여부
			>> pTITEM->m_bIsSpecial				///캐쉬 아이템 여부
			>> pTITEM->m_wUseTime				///사용 기간(일/시간)
			>> pTITEM->m_bUseType				///사용 타입
			>> pTITEM->m_bWeaponID				///WEAPON ID
			>> pTITEM->m_fShotSpeed				///SHOT SPEED
			>> pTITEM->m_fGravity				///GRAVITY
			>> pTITEM->m_dwInfoID				///INFO ID
			>> pTITEM->m_bSkillItemType			///발사타입
			>> pTITEM->m_wVisual[0]				///0단계 비주얼(추가)
			>> pTITEM->m_wVisual[1]				///1단계 비주얼(추가)
			>> pTITEM->m_wVisual[2]				///2단계 비주얼(추가)
			>> pTITEM->m_wVisual[3]				///3단계 비주얼(추가)
			>> pTITEM->m_wVisual[4]				///4단계 비주얼(추가)
			>> pTITEM->m_wGradeSFX				///등급 이펙트(추가)
			>> pTITEM->m_wOptionSFX[0]			///0단계 옵션 이펙트(추가)
			>> pTITEM->m_wOptionSFX[1]			///1단계 옵션 이펙트(추가)
			>> pTITEM->m_wOptionSFX[2]			///2단계 옵션 이펙트(추가)
			>> pTITEM->m_bCanWrap				/// 밀랍 가능
			>> pTITEM->m_dwAuctionCode
			>> pTITEM->m_bCanColor;				///염색가능


#ifdef DEBUG
		MAPTITEMTEMP::iterator _itr = CTChart::m_mapTITEMTEMP.find(pTITEM->m_wItemID);
		if( _itr != CTChart::m_mapTITEMTEMP.end() )
		{
			TRACE("경고 : %s에 중복된 아이디가 존재합니다. (ID:%u)\n", strPath, pTITEM->m_wItemID);

			delete pTITEM;
			continue;
		}
#endif

		CTChart::m_mapTITEMTEMP.insert( MAPTITEMTEMP::value_type( pTITEM->m_wItemID, pTITEM));

		if( pTITEM->m_bCanTrade & 1 ||
			pTITEM->m_bCanWrap == 1 ) // 1 = 거래가능
			CTChart::m_mapTCANTRADETEMP.insert( MAPTITEMTEMP::value_type( pTITEM->m_wItemID, pTITEM ) );
	}
}

void CTChart::ReleaseTITEMTEMP()
{
	MAPTITEMTEMP::iterator itTITEM;

	for( itTITEM = CTChart::m_mapTITEMTEMP.begin(); itTITEM != CTChart::m_mapTITEMTEMP.end(); itTITEM++)
		delete (*itTITEM).second;

	CTChart::m_mapTITEMTEMP.clear();
}

LPTITEM CTChart::FindTITEMTEMP( WORD wItemID)
{
	MAPTITEMTEMP::iterator finder = CTChart::m_mapTITEMTEMP.find(wItemID);

	if( finder != CTChart::m_mapTITEMTEMP.end() )
		return (*finder).second;

	return NULL;
}

LPTITEM CTChart::FindTCANTRADETEMP( WORD wItemID )
{
	MAPTITEMTEMP::iterator finder = CTChart::m_mapTCANTRADETEMP.find( wItemID );

	if( finder != CTChart::m_mapTCANTRADETEMP.end() )
		return (*finder).second;

	return NULL;
}

void CTChart::InitTITEMVISUAL( CString strPath )
{
	ReleaseTITEMVISUAL();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		LPTITEMVISUAL pTITEM = new TITEMVISUAL();

		ar	>> pTITEM->m_wID			
			>> pTITEM->m_dwInvenID		
			>> pTITEM->m_dwObjectID	
			>> pTITEM->m_dwCLKID		
			>> pTITEM->m_dwCLIID		
			>> pTITEM->m_dwMesh[MT_NORMAL]	
			>> pTITEM->m_dwMesh[MT_BATTLE]		
			>> pTITEM->m_dwPivot[MT_NORMAL]		
			>> pTITEM->m_dwPivot[MT_BATTLE]
			>> pTITEM->m_wIcon			
			>> pTITEM->m_bHideSlot		
			>> pTITEM->m_bHidePart		
			>> pTITEM->m_bHideRace		
			>> pTITEM->m_dwSlashColor	
			>> pTITEM->m_dwSlashTex	
			>> pTITEM->m_fSlashLen
			>> pTITEM->m_dwEffectFuncID[0]
			>> pTITEM->m_dwEffectFuncID[1]
			>> pTITEM->m_dwCostumeHide;

#ifdef DEBUG
		MAPTITEMVISUAL::iterator _itr = CTChart::m_mapTITEMVISUAL.find(pTITEM->m_wID);
		if( _itr != CTChart::m_mapTITEMVISUAL.end() )
		{
			TRACE("경고 : %s에 중복된 아이디가 존재합니다. (ID:%u)\n", strPath, pTITEM->m_wID);

			delete pTITEM;
			continue;
		}
#endif

		CTChart::m_mapTITEMVISUAL.insert( MAPTITEMVISUAL::value_type( pTITEM->m_wID, pTITEM));
	}
}

void CTChart::ReleaseTITEMVISUAL()
{
	MAPTITEMVISUAL::iterator itTITEM;

	for( itTITEM = CTChart::m_mapTITEMVISUAL.begin(); itTITEM != CTChart::m_mapTITEMVISUAL.end(); itTITEM++)
	{
		delete (*itTITEM).second;
		(*itTITEM).second = NULL;
	}

	CTChart::m_mapTITEMVISUAL.clear();
}

LPTITEMVISUAL CTChart::FindTITEMVISUAL( WORD wVisualID)
{
	MAPTITEMVISUAL::iterator itr = CTChart::m_mapTITEMVISUAL.find(wVisualID);
	if( itr != CTChart::m_mapTITEMVISUAL.end() )
		return itr->second;

	return NULL;
}

void CTChart::InitTQUESTMAGICITEM( CString strPath )
{
	CTChart::ReleaseTQUESTMAGICITEM();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; ++i)
	{
		LPTQUESTITEM pQuestItem = new TQUESTITEM;

		DWORD dwRewardID;

		ar	>> dwRewardID
			>> pQuestItem->m_wItemID
			>> pQuestItem->m_bGrade
			>> pQuestItem->m_bGLevel
			>> pQuestItem->m_dwDuraMax
			>> pQuestItem->m_dwDuraCur
			>> pQuestItem->m_bRefineCur;

		for(INT j=0; j<TMAGIC_MAX; ++j)
		{
			ar	>> pQuestItem->m_bMagic[ j ]
				>> pQuestItem->m_bValue[ j ];
		}

		if( CTChart::FindTITEMTEMP( pQuestItem->m_wItemID ) )
			CTChart::m_mapTQUESTMAGICITEM.insert( make_pair( (WORD)dwRewardID, pQuestItem) );
		else
			delete pQuestItem;
	}
}

void CTChart::ReleaseTQUESTMAGICITEM()
{
	MAPTQUESTITEM::iterator itr,end;
	itr =  CTChart::m_mapTQUESTMAGICITEM.begin();
	end =  CTChart::m_mapTQUESTMAGICITEM.end();

	for(;itr!=end; ++itr)
		delete itr->second;

	CTChart::m_mapTQUESTMAGICITEM.clear();
}

LPTQUESTITEM CTChart::FindTQUESTMAGICITEM( WORD wRewardID)
{
	MAPTQUESTITEM::iterator finder = CTChart::m_mapTQUESTMAGICITEM.find(wRewardID);

	if( finder != CTChart::m_mapTQUESTMAGICITEM.end() )
		return (*finder).second;

	return NULL;
}

void CTChart::InitTITEMGRADESFX( CString strPath )
{
	ReleaseTITEMGRADESFX();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		LPTITEMGRADESFX pSFX = new TITEMGRADESFX;
		
		ar >> pSFX->m_wID;

		for( WORD j=0; j<TGRADE_LEVEL_COUNT-1; ++j)
			ar >> pSFX->m_wSFX[ j ];

		CTChart::m_mapTITEMGRADESFX.insert( make_pair( pSFX->m_wID, pSFX) );
	}
}

void CTChart::ReleaseTITEMGRADESFX()
{
	MAPTITEMGRADESFX::iterator it, end;
	it = CTChart::m_mapTITEMGRADESFX.begin();
	end = CTChart::m_mapTITEMGRADESFX.end();

	for(; it != end ;++it )
		delete it->second;

	CTChart::m_mapTITEMGRADESFX.clear();
}

LPTITEMGRADESFX CTChart::FindTITEMGRADESFX( WORD wGradeSFX )
{
	MAPTITEMGRADESFX::iterator finder = CTChart::m_mapTITEMGRADESFX.find( wGradeSFX );

	if( finder != CTChart::m_mapTITEMGRADESFX.end() )
		return finder->second;

	return NULL;
}

void CTChart::InitTITEMMAGICSFX( CString strPath )
{
	ReleaseTITEMMAGICSFX();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		LPTITEMMAGICSFX pSFX = new TITEMMAGICSFX;
	
		ar >> pSFX->m_wID;

		for( WORD j=0; j<TMAGIC_SFXKIND_COUNT; ++j)
			ar >> pSFX->m_wSFX[ j ];

		CTChart::m_mapTITEMMAGICSFX.insert( make_pair( pSFX->m_wID, pSFX) );
	}
}

void CTChart::ReleaseTITEMMAGICSFX()
{
	MAPTITEMMAGICSFX::iterator it, end;
	it = CTChart::m_mapTITEMMAGICSFX.begin();
	end = CTChart::m_mapTITEMMAGICSFX.end();

	for(; it != end ; ++it )
		delete it->second;

	CTChart::m_mapTITEMMAGICSFX.clear();
}

LPTITEMMAGICSFX CTChart::FindTITEMMAGICSFX( WORD wSFX )
{
	MAPTITEMMAGICSFX::iterator finder = CTChart::m_mapTITEMMAGICSFX.find( wSFX );

	if( finder != CTChart::m_mapTITEMMAGICSFX.end() )
		return finder->second;

	return NULL;
}

void CTChart::InitTSFXTEMP( CString strPath )
{
	ReleaseTSFXTEMP();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		LPTSFXTEMP pTSFX = new TSFXTEMP();
		DWORD dwSFX = 0;

		ar	>> pTSFX->m_dwID
			>> dwSFX
			>> pTSFX->m_dwPosID;

		pTSFX->m_pTSFX = (LPSFX)( dwSFX );

#ifdef DEBUG
		MAPTSFXTEMP::iterator _itr = CTChart::m_mapTSFXTEMP.find(pTSFX->m_dwID);
		if( _itr != CTChart::m_mapTSFXTEMP.end() )
		{
			TRACE("경고 : %s에 중복된 아이디가 존재합니다. (ID:%u)\n", strPath, pTSFX->m_dwID);

			delete pTSFX;
			continue;
		}
#endif

		CTChart::m_mapTSFXTEMP.insert( MAPTSFXTEMP::value_type( pTSFX->m_dwID, pTSFX));
	}
}

void CTChart::ReleaseTSFXTEMP()
{
	MAPTSFXTEMP::iterator itTSFX;

	for( itTSFX = CTChart::m_mapTSFXTEMP.begin(); itTSFX != CTChart::m_mapTSFXTEMP.end(); itTSFX++)
		delete (*itTSFX).second;

	CTChart::m_mapTSFXTEMP.clear();
}

LPTSFXTEMP CTChart::FindTSFXTEMP( DWORD dwFuncID)
{
	MAPTSFXTEMP::iterator finder = CTChart::m_mapTSFXTEMP.find(dwFuncID);

	if( finder != CTChart::m_mapTSFXTEMP.end() )
		return (*finder).second;

	return NULL;
}

void CTChart::InitTMONTEMP( CString strPath )
{
	ReleaseTMONTEMP();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		LPTMONTEMP pTEMP = new TMONTEMP();
		DWORD dwMenuID;
		WORD wMonID;

		ar	>> wMonID
			>> pTEMP->m_wKind
			>> pTEMP->m_strTITLE
			>> pTEMP->m_strNAME
			>> pTEMP->m_fLB
			>> pTEMP->m_fLOST
			>> pTEMP->m_fAB
			>> pTEMP->m_dwOBJ;

		for( BYTE j=0; j<ES_COUNT; j++)
		{
			WORD wItemID;

			ar >> wItemID;
			pTEMP->m_pTITEM[j] = CTChart::FindTITEMTEMP(wItemID);
		}

		for( BYTE j=0; j<TMONSKILL_COUNT; j++)
			ar >> pTEMP->m_wTSKILL[j];

		BYTE bSlidingWhenDie;
		BYTE bDrawNameWhenDie;

		ar	>> pTEMP->m_bNotNockBack
			>> pTEMP->m_bCanSelected
			>> pTEMP->m_bCanFly
			>> pTEMP->m_bCanAttack
			>> pTEMP->m_bDrawName
			>> pTEMP->m_bCanTame
			>> pTEMP->m_bVisible
			>> pTEMP->m_bApplyAI
			>> dwMenuID
			>> pTEMP->m_fSize
			>> pTEMP->m_fScaleX
			>> pTEMP->m_fScaleY
			>> pTEMP->m_fScaleZ
			>> pTEMP->m_wSpawnSFX
			>> pTEMP->m_dwSpawnSND
			>> pTEMP->m_wFaceIcon
			>> pTEMP->m_bCanDetectHidingPC
			>> bSlidingWhenDie
			>> bDrawNameWhenDie;

		pTEMP->m_bSlidingWhenDie = !bSlidingWhenDie;
		pTEMP->m_bDrawNameWhenDie = !bDrawNameWhenDie;

#ifdef DEBUG
		MAPMONTEMP::iterator _itr = CTChart::m_mapMONTEMP.find(wMonID);
		if( _itr != CTChart::m_mapMONTEMP.end() )
		{
			TRACE("경고 : %s에 중복된 아이디가 존재합니다. (ID:%u)\n", strPath, wMonID);

			delete pTEMP;
			continue;
		}
#endif

		pTEMP->m_pTPOPUP = CTChart::FindTPOPUPMENU(dwMenuID);
		CTChart::m_mapMONTEMP.insert( MAPMONTEMP::value_type( wMonID, pTEMP));
	}
}

void CTChart::ReleaseTMONTEMP()
{
	MAPMONTEMP::iterator itMON;

	for( itMON = CTChart::m_mapMONTEMP.begin(); itMON != CTChart::m_mapMONTEMP.end(); itMON++)
		delete (*itMON).second;

	CTChart::m_mapMONTEMP.clear();
}

LPTMONTEMP CTChart::FindTMONTEMP( WORD wMonID)
{
	MAPMONTEMP::iterator finder = CTChart::m_mapMONTEMP.find(wMonID);

	if( finder != CTChart::m_mapMONTEMP.end() )
		return (*finder).second;

	return NULL;
}

void CTChart::InitTNPCTEMP( CString strPath )
{
	ReleaseTNPCTEMP();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		LPTNPCTEMP pTNPC = new TNPCTEMP();

		ar	>> pTNPC->m_wID
			>> pTNPC->m_dwOBJ;

		for( BYTE j=0; j<ES_COUNT; j++)
		{
			WORD wItemID;

			ar >> wItemID;
			pTNPC->m_pTITEM[j] = CTChart::FindTITEMTEMP(wItemID);
		}

		ar	>> pTNPC->m_bActionID
			>> pTNPC->m_bRndActID[0]
			>> pTNPC->m_bRndActID[1]
			>> pTNPC->m_dwSND
			>> pTNPC->m_wFaceIcon
			>> pTNPC->m_bBoxAnim;

#ifdef DEBUG
		MAPNPCTEMP::iterator _itr = CTChart::m_mapTNPCTEMP.find(pTNPC->m_wID);
		if( _itr != CTChart::m_mapTNPCTEMP.end() )
		{
			TRACE("경고 : %s에 중복된 아이디가 존재합니다. (ID:%u)\n", strPath, pTNPC->m_wID);

			delete pTNPC;
			continue;
		}
#endif

		CTChart::m_mapTNPCTEMP.insert( MAPNPCTEMP::value_type( pTNPC->m_wID, pTNPC));
	}
}

void CTChart::ReleaseTNPCTEMP()
{
	MAPNPCTEMP::iterator itNPC;

	for( itNPC = CTChart::m_mapTNPCTEMP.begin(); itNPC != CTChart::m_mapTNPCTEMP.end(); itNPC++)
		delete (*itNPC).second;

	CTChart::m_mapTNPCTEMP.clear();
}

LPTNPCTEMP CTChart::FindTNPCTEMP( WORD wTempID)
{
	MAPNPCTEMP::iterator finder = CTChart::m_mapTNPCTEMP.find(wTempID);

	if( finder == CTChart::m_mapTNPCTEMP.end() )
		return NULL;

	return (*finder).second;
}

void CTChart::InitTNPCGlobal( CString strPath )
{
	ReleaseTNPCGlobal();

	CFile file;
	if( file.Open( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary) )
	{
		CArchive ar( &file, CArchive::load);

		WORD wCount;
			ar >> wCount;

		for( WORD i=0; i<wCount; i++)
		{
			LPTNPC pTNPC = new TNPC();

			CTChart::ReadTNPC( ar, pTNPC);
			CTChart::m_mapGlobalNPC.insert( std::make_pair( (WORD) pTNPC->m_dwID, pTNPC));
		}
	}
}

void CTChart::ReleaseTNPCGlobal()
{
	MAPNPC::iterator it, end;
	it = CTChart::m_mapGlobalNPC.begin();
	end = CTChart::m_mapGlobalNPC.end();

	for(; it != end ; ++it )
		delete it->second;

	CTChart::m_mapGlobalNPC.clear();
}

LPTNPC CTChart::FindTNPCGlobal( WORD wID )
{
	MAPNPC::iterator finder = CTChart::m_mapGlobalNPC.find(wID);

	if( finder == CTChart::m_mapGlobalNPC.end() )
		return NULL;

	return (*finder).second;
}

void CTChart::ReadTNPC( CArchive& ar, LPTNPC pTNPC)
{
	static TNPC vDummy;

	if(!pTNPC)
		pTNPC = &vDummy;

	DWORD dwMenuID = 0;
	WORD wTempID = 0;

	ar	>> pTNPC->m_dwID
		>> wTempID
		>> pTNPC->m_strNAME
		>> pTNPC->m_strTITLE
		>> pTNPC->m_bNPCType
		>> pTNPC->m_bClassID
		>> pTNPC->m_bLevel
		>> pTNPC->m_bCountryID
		>> pTNPC->m_bCollisionType
		>> pTNPC->m_bCanSelected
		>> pTNPC->m_bLand
		>> pTNPC->m_bMode
		>> pTNPC->m_bDrawGhost
		>> pTNPC->m_bDrawMark
		>> pTNPC->m_bDrawName
		>> pTNPC->m_bHouseMesh
		>> pTNPC->m_dwHouseID
		>> dwMenuID
		>> pTNPC->m_dwExecID
		>> pTNPC->m_dwQuestID
		>> pTNPC->m_wItemID
		>> pTNPC->m_dwMaxHP
		>> pTNPC->m_fDIR
		>> pTNPC->m_fPosX
		>> pTNPC->m_fPosY
		>> pTNPC->m_fPosZ
		>> pTNPC->m_fSizeX
		>> pTNPC->m_fSizeY
		>> pTNPC->m_fSizeZ
		>> pTNPC->m_fScaleX
		>> pTNPC->m_fScaleY
		>> pTNPC->m_fScaleZ
		>> pTNPC->m_bCamp;

	pTNPC->m_pTPOPUP = CTChart::FindTPOPUPMENU(dwMenuID);
	pTNPC->m_pTNPC = CTChart::FindTNPCTEMP(wTempID);
}

void CTChart::InitTSTEPSND( CString strPath )
{
	ReleaseTSTEPSND();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		DWORD dwTileID;
		DWORD dwFuncID;

		ar	>> dwTileID
			>> dwFuncID;

		CTChart::m_mapTSTEPSND.insert( MAPDWORD::value_type( dwTileID, dwFuncID));
	}
}

void CTChart::ReleaseTSTEPSND()
{
	CTChart::m_mapTSTEPSND.clear();
}

void CTChart::InitTFORMULA( CString strPath )
{
	ReleaseTFORMULA();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		LPTFORMULA pTFORMULA = new TFORMULA();
		BYTE bID;

		ar	>> bID
			>> pTFORMULA->m_dwInit
			>> pTFORMULA->m_fRateX
			>> pTFORMULA->m_fRateY;

#ifdef DEBUG
		MAPTFORMULA::iterator _itr = CTChart::m_mapTFORMULA.find(bID);
		if( _itr != CTChart::m_mapTFORMULA.end() )
		{
			TRACE("경고 : %s에 중복된 아이디가 존재합니다. (ID:%u)\n", strPath, bID);

			delete pTFORMULA;
			continue;
		}
#endif

		CTChart::m_mapTFORMULA.insert( MAPTFORMULA::value_type( bID, pTFORMULA));
	}

	LPTFORMULA pTFORMULA = CTChart::FindTFORMULA(FTYPE_1ST);
	for( i=0; i<TMAX_LEVEL; i++)
		CTChart::m_vLEVELRATE[i] = powf( pTFORMULA->m_fRateX, i);
}

void CTChart::ReleaseTFORMULA()
{
	MAPTFORMULA::iterator itTFORMULA;

	for( itTFORMULA = CTChart::m_mapTFORMULA.begin(); itTFORMULA != CTChart::m_mapTFORMULA.end(); itTFORMULA++)
		delete (*itTFORMULA).second;

	CTChart::m_mapTFORMULA.clear();
}

LPTFORMULA CTChart::FindTFORMULA( BYTE bID)
{
	MAPTFORMULA::iterator finder = CTChart::m_mapTFORMULA.find(bID);

	if( finder != CTChart::m_mapTFORMULA.end() )
		return (*finder).second;

	return NULL;
}

void CTChart::InitTSKYBOX( CString strPath )
{
	ReleaseTSKYBOX();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		LPTSKYBOX pTSKYBOX = new TSKYBOX();
		DWORD dwID;

		ar	>> dwID
			>> pTSKYBOX->m_dwOBJ
			>> pTSKYBOX->m_dwCLK
			>> pTSKYBOX->m_dwCL
			>> pTSKYBOX->m_dwMESH
			>> pTSKYBOX->m_dwACT
			>> pTSKYBOX->m_dwANI;

#ifdef DEBUG
		MAPTSKYBOX::iterator _itr = CTChart::m_mapTSKYBOX.find(dwID);
		if( _itr != CTChart::m_mapTSKYBOX.end() )
		{
			TRACE("경고 : %s에 중복된 아이디가 존재합니다. (ID:%u)\n", strPath, dwID);

			delete pTSKYBOX;
			continue;
		}
#endif

		CTChart::m_mapTSKYBOX.insert( MAPTSKYBOX::value_type( dwID, pTSKYBOX));
	}
}

void CTChart::ReleaseTSKYBOX()
{
	MAPTSKYBOX::iterator itTSKY;

	for( itTSKY = CTChart::m_mapTSKYBOX.begin(); itTSKY != CTChart::m_mapTSKYBOX.end(); itTSKY++)
		delete (*itTSKY).second;

	CTChart::m_mapTSKYBOX.clear();
}

LPTSKYBOX CTChart::FindTSKYBOX( BYTE bUnitCountX,
									 BYTE bIndexID,
									 WORD wMapID,
									 BYTE bUnitX,
									 BYTE bUnitZ)
{
	MAPTSKYBOX::iterator finder = CTChart::m_mapTSKYBOX.find(MAKELONG( MAKEWORD( bUnitZ * bUnitCountX + bUnitX, bIndexID), wMapID));

	if( finder != CTChart::m_mapTSKYBOX.end() )
		return (*finder).second;

	return NULL;
}

void CTChart::InitTSWITCH( CString strPath )
{
	ReleaseTSWITCH();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		LPTSWITCH pTSWITCH = new TSWITCH();

		ar	>> pTSWITCH->m_dwID
			>> pTSWITCH->m_wMapID
			>> pTSWITCH->m_fPosX
			>> pTSWITCH->m_fPosY
			>> pTSWITCH->m_fPosZ
			>> pTSWITCH->m_fDIR
			>> pTSWITCH->m_fScaleX
			>> pTSWITCH->m_fScaleY
			>> pTSWITCH->m_fScaleZ
			>> pTSWITCH->m_dwOBJ;

		for( BYTE j=0; j<ES_COUNT; j++)
		{
			WORD wItemID;

			ar >> wItemID;
			pTSWITCH->m_pTITEM[j] = CTChart::FindTITEMTEMP(wItemID);
		}

		ar	>> pTSWITCH->m_bCloseID
			>> pTSWITCH->m_bOpenID
			>> pTSWITCH->m_bLockOnOpen
			>> pTSWITCH->m_bLockOnClose
			>> pTSWITCH->m_bHouseMesh
			>> pTSWITCH->m_dwHouseID
			>> pTSWITCH->m_dwSoundID;

#ifdef DEBUG
		MAPTSWITCHDATA::iterator _itr = CTChart::m_mapTSWITCH.find(pTSWITCH->m_dwID);
		if( _itr != CTChart::m_mapTSWITCH.end() )
		{
			TRACE("경고 : %s에 중복된 아이디가 존재합니다. (ID:%u)\n", strPath, pTSWITCH->m_dwID);

			delete pTSWITCH;
			continue;
		}
#endif

		CTChart::m_mapTSWITCH.insert( MAPTSWITCHDATA::value_type( pTSWITCH->m_dwID, pTSWITCH));
	}
}

void CTChart::ReleaseTSWITCH()
{
	MAPTSWITCHDATA::iterator itTSWITCH;

	for( itTSWITCH = CTChart::m_mapTSWITCH.begin(); itTSWITCH != CTChart::m_mapTSWITCH.end(); itTSWITCH++)
		delete (*itTSWITCH).second;

	CTChart::m_mapTSWITCH.clear();
}

LPTSWITCH CTChart::FindTSWITCH( DWORD dwID)
{
	MAPTSWITCHDATA::iterator finder = CTChart::m_mapTSWITCH.find(dwID);

	if( finder != CTChart::m_mapTSWITCH.end() )
		return (*finder).second;

	return NULL;
}

void CTChart::InitTLIGHT( CString strPath )
{
	ReleaseTLIGHT();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		LPTLIGHT pTLIGHT = new TLIGHT();
		DWORD dwLightID;

		ar	>> dwLightID
			>> pTLIGHT->m_vDIR.x
			>> pTLIGHT->m_vDIR.y
			>> pTLIGHT->m_vDIR.z
			>> pTLIGHT->m_fAmbientR
			>> pTLIGHT->m_fAmbientG
			>> pTLIGHT->m_fAmbientB
			>> pTLIGHT->m_fDiffuseR
			>> pTLIGHT->m_fDiffuseG
			>> pTLIGHT->m_fDiffuseB;

#ifdef DEBUG
		MAPTLIGHT::iterator _itr = CTChart::m_mapTLIGHT.find(dwLightID);
		if( _itr != CTChart::m_mapTLIGHT.end() )
		{
			TRACE("경고 : %s에 중복된 아이디가 존재합니다. (ID:%u)\n", strPath, dwLightID);

			delete pTLIGHT;
			continue;
		}
#endif

		CTChart::m_mapTLIGHT.insert( MAPTLIGHT::value_type( dwLightID, pTLIGHT));
	}
}

void CTChart::ReleaseTLIGHT()
{
	MAPTLIGHT::iterator itTLIGHT;

	for( itTLIGHT = CTChart::m_mapTLIGHT.begin(); itTLIGHT != CTChart::m_mapTLIGHT.end(); itTLIGHT++)
		delete (*itTLIGHT).second;

	CTChart::m_mapTLIGHT.clear();
}

LPTLIGHT CTChart::FindTLIGHT( BYTE bUnitCountX,
								   BYTE bSetID,
								   BYTE bIndexID,
								   WORD wMapID,
								   BYTE bUnitX,
								   BYTE bUnitZ)
{
	MAPTLIGHT::iterator finder = CTChart::m_mapTLIGHT.find(MAKELONG( MAKEWORD( bUnitZ * bUnitCountX + bUnitX, bSetID * TLIGHT_COUNT + bIndexID), wMapID));

	if( finder != CTChart::m_mapTLIGHT.end() )
		return (*finder).second;

	return NULL;
}

void CTChart::InitTLEVEL( CString strPath )
{
	ReleaseTLEVEL();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		TLEVEL level;

		ar	>> level.m_bLevel
			>> level.m_dwCost
			>> level.m_dwRegCost
			>> level.m_dwSearchCost;

		CTChart::m_mapTLEVEL.insert( make_pair(level.m_bLevel, level) );
	}
}

void CTChart::ReleaseTLEVEL()
{
	CTChart::m_mapTLEVEL.clear();
}

DWORD CTChart::GetLevelPrice( BYTE bLevel)
{
	MAPTLEVEL::iterator finder = CTChart::m_mapTLEVEL.find(bLevel);

	if( finder != CTChart::m_mapTLEVEL.end() )
		return (*finder).second.m_dwCost;

	return 0;
}

DWORD CTChart::GetSoulmateSearchPrice( BYTE bLevel)
{
	MAPTLEVEL::iterator finder = CTChart::m_mapTLEVEL.find(bLevel);

	if( finder != CTChart::m_mapTLEVEL.end() )
		return (*finder).second.m_dwSearchCost;

	return 0;
}

DWORD CTChart::GetSoulmateRegPrice( BYTE bLevel)
{
	MAPTLEVEL::iterator finder = CTChart::m_mapTLEVEL.find(bLevel);

	if( finder != CTChart::m_mapTLEVEL.end() )
		return (*finder).second.m_dwRegCost;

	return 0;
}

void CTChart::InitTFOG( CString strPath )
{
	ReleaseTFOG();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		LPTFOG pTFOG = new TFOG();

		DWORD dwFogID;
		BYTE bTGLOBAL;

		ar	>> dwFogID
			>> bTGLOBAL
			>> pTFOG->m_bType
			>> pTFOG->m_bRValue
			>> pTFOG->m_bGValue
			>> pTFOG->m_bBValue
			>> pTFOG->m_fRADIUS
			>> pTFOG->m_fRANGE
			>> pTFOG->m_fPosX
			>> pTFOG->m_fPosZ
			>> pTFOG->m_fDENSITY
			>> pTFOG->m_fSTART
			>> pTFOG->m_fEND;

		LPTUNITFOG pTUNITFOG = CTChart::FindTFOG(dwFogID);
		if(!pTUNITFOG)
		{
			pTUNITFOG = new TUNITFOG();
			CTChart::m_mapTFOG.insert(MAPTUNITFOG::value_type( dwFogID, pTUNITFOG));
		}

		if(bTGLOBAL)
		{
			if(pTUNITFOG->m_pTGLOBAL)
				delete pTUNITFOG->m_pTGLOBAL;

			pTUNITFOG->m_pTGLOBAL = pTFOG;
		}
		else
			pTUNITFOG->m_vTLOCAL.push_back(pTFOG);
	}
}

void CTChart::ReleaseTFOG()
{
	MAPTUNITFOG::iterator itTFOG;

	for( itTFOG = CTChart::m_mapTFOG.begin(); itTFOG != CTChart::m_mapTFOG.end(); itTFOG++)
		delete (*itTFOG).second;

	CTChart::m_mapTFOG.clear();
}

LPTUNITFOG CTChart::FindTFOG( DWORD dwFogID)
{
	MAPTUNITFOG::iterator finder = CTChart::m_mapTFOG.find(dwFogID);

	if( finder != CTChart::m_mapTFOG.end() )
		return (*finder).second;

	return NULL;
}

LPTUNITFOG CTChart::FindTFOG( WORD wMapID, BYTE bUnitX, BYTE bUnitZ )
{
	return CTChart::FindTFOG( MAKELONG( MAKEWORD( bUnitX, bUnitZ), wMapID));
}

void CTChart::InitTGATE( CString strPath )
{
	ReleaseTGATE();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		LPTGATE pTGATE = new TGATE();

		ar	>> pTGATE->m_dwID
			>> pTGATE->m_dwSwitchID
			>> pTGATE->m_wMapID
			>> pTGATE->m_fPosX
			>> pTGATE->m_fPosY
			>> pTGATE->m_fPosZ
			>> pTGATE->m_fDIR
			>> pTGATE->m_fScaleX
			>> pTGATE->m_fScaleY
			>> pTGATE->m_fScaleZ
			>> pTGATE->m_dwOBJ;

		for( BYTE j=0; j<ES_COUNT; j++)
		{
			WORD wItemID;

			ar >> wItemID;
			pTGATE->m_pTITEM[j] = CTChart::FindTITEMTEMP(wItemID);
		}

		ar	>> pTGATE->m_bCloseID
			>> pTGATE->m_bCloseActID
			>> pTGATE->m_bOpenID
			>> pTGATE->m_bOpenActID
			>> pTGATE->m_bDeleteOnOpen
			>> pTGATE->m_bDeleteOnClose
			>> pTGATE->m_bHouseMesh
			>> pTGATE->m_dwHouseID
			>> pTGATE->m_dwOpenSFX
			>> pTGATE->m_dwCloseSFX
			>> pTGATE->m_bStair;

		if(! CTChart::FindTGATE(pTGATE->m_dwID))
			CTChart::m_mapTGATE.insert( MAPTGATEDATA::value_type( pTGATE->m_dwID, pTGATE));
		else
			delete pTGATE;
	}
}

void CTChart::ReleaseTGATE()
{
	MAPTGATEDATA::iterator itTGATE;

	for( itTGATE = CTChart::m_mapTGATE.begin(); itTGATE != CTChart::m_mapTGATE.end(); itTGATE++)
		delete (*itTGATE).second;

	CTChart::m_mapTGATE.clear();
}

LPTGATE CTChart::FindTGATE( DWORD dwID )
{
	MAPTGATEDATA::iterator finder = CTChart::m_mapTGATE.find( dwID );

	if( finder != CTChart::m_mapTGATE.end() )
		return (*finder).second;

	return NULL;
}

void CTChart::InitTITEMMAGIC( CString strPath )
{
	ReleaseTITEMMAGIC();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	CString strData;
	
	for( WORD i=0; i<wCount; i++)
	{
		LPTITEMMAGIC pTITEMMAGIC = new TITEMMAGIC;

		BYTE bOptionKind;

		ar	>> pTITEMMAGIC->m_strNAME
			>> pTITEMMAGIC->m_wID
			>> bOptionKind
			>> pTITEMMAGIC->m_fUtil
			>> pTITEMMAGIC->m_bSFX;

		pTITEMMAGIC->m_bSecOption = !bOptionKind;

		CTChart::m_mapITEMMAGIC.insert( make_pair( pTITEMMAGIC->m_wID, pTITEMMAGIC) );
	}
}

void CTChart::ReleaseTITEMMAGIC()
{
	MAPTITEMMAGIC::iterator it, end;
	it = CTChart::m_mapITEMMAGIC.begin();
	end = CTChart::m_mapITEMMAGIC.end();

	for(; it != end ; ++it )
		delete it->second;

	CTChart::m_mapITEMMAGIC.clear();
}

LPTITEMMAGIC CTChart::FindTITEMMAGIC( WORD wMagicID )
{
	MAPTITEMMAGIC::iterator finder = CTChart::m_mapITEMMAGIC.find( wMagicID );
	
	if( finder != CTChart::m_mapITEMMAGIC.end () )
		return finder->second;

	return NULL;
}

void CTChart::InitTPET( CString strPath )
{
	ReleaseTPET(); 

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; ++i)
	{
		LPTPET pPET = new TPET;

		ar  >> pPET->m_wID
			>> pPET->m_wMonID
			>> pPET->m_bRecallKind1
			>> pPET->m_bRecallKind2
			>> pPET->m_wRecallValue1
			>> pPET->m_wRecallValue2
			>> pPET->m_bConditionType
			>> pPET->m_dwConditionValue
			>> pPET->m_wIcon;

		CTChart::m_mapTPET.insert( make_pair(pPET->m_wID, pPET) );
	}
}

void CTChart::ReleaseTPET()
{
	MAPTPET::iterator itr,end;
	itr = CTChart::m_mapTPET.begin();
	end = CTChart::m_mapTPET.end();

	for(;itr!=end; ++itr)
		delete itr->second;

	CTChart::m_mapTPET.clear();
}

LPTPET CTChart::FindTPETTEMP(WORD wPetID)
{
	MAPTPET::iterator finder = CTChart::m_mapTPET.find(wPetID);
	if( finder == CTChart::m_mapTPET.end() )
		return NULL;

	return finder->second;
}

void CTChart::InitTITEMATTR( CString strPath )
{
	ReleaseTITEMATTR();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; ++i)
	{
		LPTITEMATTR pTITEMATTR = new TITEMATTR;

		ar 	>> pTITEMATTR->m_wID
			>> pTITEMATTR->m_bGrade
			>> pTITEMATTR->m_wMinAP
			>> pTITEMATTR->m_wMaxAP
			>> pTITEMATTR->m_wDP
			>> pTITEMATTR->m_wMinMAP
			>> pTITEMATTR->m_wMaxMAP
			>> pTITEMATTR->m_wMDP
			>> pTITEMATTR->m_bBlockProb
			>> pTITEMATTR->m_bSpeedInc;

		CTChart::m_mapTITEMATTR.insert( make_pair( pTITEMATTR->m_wID, pTITEMATTR) );
	}
}

void CTChart::ReleaseTITEMATTR()
{
	MAPTITEMATTR::iterator it, end;
	it = CTChart::m_mapTITEMATTR.begin();
	end = CTChart::m_mapTITEMATTR.end();

	for(; it != end ; ++it )
		delete it->second;

	CTChart::m_mapTITEMATTR.clear();
}

LPTITEMATTR CTChart::FindTITEMATTR( WORD wAttrID )
{
	MAPTITEMATTR::iterator finder = CTChart::m_mapTITEMATTR.find( wAttrID );
	if( finder != CTChart::m_mapTITEMATTR.end() )
		return finder->second;

	return NULL;
}

void CTChart::InitTITEMGRADE( CString strPath )
{
	CTChart::m_vTITEMGRADE.clear();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	TITEMGRADE grd;
	for( WORD i=0; i<wCount; ++i)
	{
		ar 	>> grd.m_bLevel
			>> grd.m_bGrade;
			//>> grd.m_dwCost;

		CTChart::m_vTITEMGRADE.push_back(grd);
	}
}

void CTChart::InitTITEMGRADEVISUAL( CString strPath )
{
	CTChart::m_mapTITEMGRADEVISUAL.clear();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; ++i)
	{
		BYTE bKind;
		WORD wGrade;
		DWORD dwTextureID;
		BYTE bOPCode;

		ar >> bKind
			>> wGrade
			>> dwTextureID
			>> bOPCode;

		TITEMGRADEVISUALKEY Key(
			bKind,
			wGrade);
		
		TITEMGRADEVISUAL Data;
		Data.m_bPSTYPE = (INT) bOPCode;
		Data.m_pSkinTex = (LPTEXTURESET) dwTextureID;

		CTChart::m_mapTITEMGRADEVISUAL.insert(
			make_pair( Key, Data ) );
	}
}

void CTChart::InitTHelp( CString strPath )
{
	ReleaseTHelp();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		LPTHELPPAGE pHELPPAGE = new THELPPAGE;
		DWORD dwCategoryID;

		ar >> dwCategoryID
			>> pHELPPAGE->m_wPage
			>> pHELPPAGE->m_wImage
			>> pHELPPAGE->m_strTitle
			>> pHELPPAGE->m_strText;

		MAPTHELP::iterator it = CTChart::m_mapTHELP.find( dwCategoryID );
		if( it != CTChart::m_mapTHELP.end() )
		{
			it->second->m_vPage.push_back( pHELPPAGE );
		}
		else
		{
			LPTHELP pHELP = new THELP;
			pHELP->m_dwCategoryID = dwCategoryID;
			pHELP->m_vPage.push_back( pHELPPAGE );
			CTChart::m_mapTHELP.insert( MAPTHELP::value_type( dwCategoryID, pHELP));
		}
	}	
}

void CTChart::ReleaseTHelp()
{
	MAPTHELP::iterator itHELP;

	for( itHELP = CTChart::m_mapTHELP.begin(); itHELP != CTChart::m_mapTHELP.end(); ++itHELP)
	{
		VTHELPPAGE::iterator itPAGE, endPAGE;
		itPAGE = (*itHELP).second->m_vPage.begin();
		endPAGE = (*itHELP).second->m_vPage.end();
		for(; itPAGE != endPAGE ; ++itPAGE )
		{
			if( (*itPAGE) )
				delete (*itPAGE);
		}
		(*itHELP).second->m_vPage.clear();
		delete (*itHELP).second;
	}

	CTChart::m_mapTHELP.clear();
}

LPTHELP CTChart::FindTHELP( DWORD dwHelpID )
{
	MAPTHELP::iterator finder = CTChart::m_mapTHELP.find( dwHelpID );
	
	if( finder != CTChart::m_mapTHELP.end () )
		return finder->second;

	return NULL;
}

void CTChart::InitTHelpLink( CString strPath )
{
	ReleaseTHelpLink();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		DWORD dwQuestID;
		BYTE bTrigger;
		DWORD dwHelpID;

		ar >> dwQuestID
			>> bTrigger
			>> dwHelpID;

		if( bTrigger < 0 || bTrigger >= THELPLINK_TRIGGER_COUNT )
		{
			TRACE("경고 : THelpLink 차트 - 잘못된 트리거 값 dwQuestID(%d) bTrigger(%d) dwHelpID(%d)\n", dwQuestID, (DWORD) bTrigger, dwHelpID );
			continue;
		}

		CTChart::m_mapTHELPLINK[ bTrigger ].insert( MAPTHELPLINK::value_type( dwQuestID, dwHelpID ) );
	}
}

void CTChart::ReleaseTHelpLink()
{
	for( INT i=0 ; i < THELPLINK_TRIGGER_COUNT ; ++i )
		CTChart::m_mapTHELPLINK[ i ].clear();
}

void CTChart::InitTQUESTTEMP( CString strPath)
{
	ReleaseTQUESTTEMP();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	DWORD dwLength = DWORD(file.GetLength());

	int nClassCount = 0;
	file.Read( &nClassCount, sizeof(int));

	for( int i=0; i<nClassCount; i++)
	{
		LPTQCLASS pTQCLASS = new TQCLASS();

		file.Read( &pTQCLASS->m_dwClassID, sizeof(DWORD));
		pTQCLASS->m_strNAME = ReadString(&file);
		file.Read( &pTQCLASS->m_bMAIN, sizeof(BYTE));

		CTChart::m_mapTQCLASSTEMP.insert( MAPTQCLASSTEMP::value_type( pTQCLASS->m_dwClassID, pTQCLASS));
	}

	while(file.GetPosition() < dwLength)
	{
		LPTQUEST pTQUEST = new TQUEST();

		int nRewardCount;
		int nTermCount;

		file.Read( &pTQUEST->m_dwID, sizeof(DWORD));
		file.Read( &pTQUEST->m_bType, sizeof(BYTE));
		file.Read( &pTQUEST->m_dwClassID, sizeof(DWORD));
		file.Read( &pTQUEST->m_bMAIN, sizeof(BYTE));

		pTQUEST->m_strTITLE = ReadString(&file);
		pTQUEST->m_strTriggerMSG = ReadString(&file);
		pTQUEST->m_strCompleteMSG = ReadString(&file);
		pTQUEST->m_strAcceptMSG = ReadString(&file);
		pTQUEST->m_strRejectMSG = ReadString(&file);
		pTQUEST->m_strSummaryMSG = ReadString(&file);
		pTQUEST->m_strNPCName = ReadString(&file);
		pTQUEST->m_strReply = ReadString(&file);

		file.Read( &pTQUEST->m_bLevel, sizeof(BYTE));
		file.Read( &nTermCount, sizeof(int));

		for( int i=0; i<nTermCount; i++)
		{
			LPTTERM pTTERM = new TTERM();

			file.Read( &pTTERM->m_bType, sizeof(BYTE));
			file.Read( &pTTERM->m_dwID, sizeof(DWORD));
			file.Read( &pTTERM->m_bCount, sizeof(BYTE));

			pTTERM->m_strTermMSG = ReadString(&file);
			pTTERM->m_strFailMSG = ReadString(&file);

			pTQUEST->m_vTTERM.push_back(pTTERM);
		}

		file.Read( &nRewardCount, sizeof(int));
		for( i=0; i<nRewardCount; i++)
		{
			LPTREWARD pTREWARD = new TREWARD();

			file.Read( &pTREWARD->m_bType, sizeof(BYTE));
			file.Read( &pTREWARD->m_dwID, sizeof(DWORD));
			file.Read( &pTREWARD->m_bCount, sizeof(BYTE));
			file.Read( &pTREWARD->m_bMethod, sizeof(BYTE));
			file.Read( &pTREWARD->m_bProb, sizeof(BYTE));

			pTQUEST->m_vTREWARD.push_back(pTREWARD);
		}

		file.Read( &pTQUEST->m_dwMinLevel, sizeof(DWORD));
		file.Read( &pTQUEST->m_dwMaxLevel, sizeof(DWORD));

		CTChart::m_mapTQUESTTEMP.insert( MAPTQUESTTEMP::value_type( pTQUEST->m_dwID, pTQUEST));
	}
}

void CTChart::ReleaseTQUESTTEMP()
{
	MAPTQCLASSTEMP::iterator itTQClass;
	MAPTQUESTTEMP::iterator it;

	for( itTQClass = CTChart::m_mapTQCLASSTEMP.begin(); itTQClass != CTChart::m_mapTQCLASSTEMP.end(); itTQClass++)
		delete (*itTQClass).second;
	CTChart::m_mapTQCLASSTEMP.clear();

	for( it = CTChart::m_mapTQUESTTEMP.begin(); it != CTChart::m_mapTQUESTTEMP.end(); it++)
		delete (*it).second;
	CTChart::m_mapTQUESTTEMP.clear();
}

LPTQCLASS CTChart::FindTQCLASS( DWORD dwClassID)
{
	MAPTQCLASSTEMP::iterator finder = CTChart::m_mapTQCLASSTEMP.find(dwClassID);

	if( finder != CTChart::m_mapTQCLASSTEMP.end() )
		return (*finder).second;

	return &m_vNULLCLASS;
}

LPTQUEST CTChart::FindTQUEST( DWORD dwQuestID)
{
	MAPTQUESTTEMP::iterator finder = CTChart::m_mapTQUESTTEMP.find(dwQuestID);

	if( finder != CTChart::m_mapTQUESTTEMP.end() )
		return (*finder).second;

	return NULL;
}

LPTQUEST CTChart::FindTMISSION( LPTQUEST pTQUEST)
{
	for( BYTE i=0; i<INT(pTQUEST->m_vTTERM.size()); i++)
		if( pTQUEST->m_vTTERM[i]->m_bType == QTT_COMPQUEST )
			return FindTQUEST(pTQUEST->m_vTTERM[i]->m_dwID);

	return NULL;
}

void CTChart::InitTMINIMAP( CString strPath )
{
	ReleaseTMINIMAP();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		LPTMINIMAP pTMINIMAP = new TMINIMAP();
		DWORD dwTexID;

		ar	>> pTMINIMAP->m_dwUnitID
			>> dwTexID
			>> pTMINIMAP->m_fSCALE
			>> pTMINIMAP->m_fPosX
			>> pTMINIMAP->m_fPosZ
			>> pTMINIMAP->m_bWorldID
			>> pTMINIMAP->m_dwWorldButtonID
			>> pTMINIMAP->m_bWorldMapLevel;
		pTMINIMAP->m_pTEX = (LPTEXTURESET) dwTexID;

		CTChart::m_mapTMINIMAP.insert( std::make_pair( pTMINIMAP->m_dwUnitID, pTMINIMAP));
	}
}

void CTChart::ReleaseTMINIMAP()
{
	MAPTMINIMAP::iterator itTMINIMAP;

	for( itTMINIMAP = CTChart::m_mapTMINIMAP.begin(); itTMINIMAP != CTChart::m_mapTMINIMAP.end(); itTMINIMAP++)
		delete (*itTMINIMAP).second;

	CTChart::m_mapTMINIMAP.clear();
}

void CTChart::InitTDYNAMICHELP( CString strPath )
{
	CString strTEXT;

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		CString strPART;
		DWORD dwID;

		ar	>> strPART
			>> dwID;

		strPART.Remove('\n');

		CTChart::m_mapTDynamicHelp.insert( MAPTDYNAMICHELP::value_type(dwID, strPART) );
	}
}

void CTChart::ReleaseTDYNAMICHELP()
{
	CTChart::m_mapTDynamicHelp.clear();
}

CString CTChart::FindTDYNAMICHELP(DWORD dwID)
{
	MAPTDYNAMICHELP::iterator it = CTChart::m_mapTDynamicHelp.find(dwID);
	if( it != CTChart::m_mapTDynamicHelp.end() )
		return it->second;
	return _T("");
}

void CTChart::InitTRACETEMP( CString strPath )
{
	for( int i=0; i<TMAINRACE_COUNT; i++)
		for( int j=0; j<TSEX_COUNT; j++)
		{
			CTChart::m_vTRACESCALE[i][j] = 1.0f;
			CTChart::m_vTRACE[i][j] = 0;
		}

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( i=0; i<wCount; i++)
	{
		BYTE bRACE;
		BYTE bSEX;
		DWORD dwOBJID;
		FLOAT fSCALE;

		ar	>> bRACE
			>> bSEX
			>> dwOBJID
			>> fSCALE;

		CTChart::m_vTRACESCALE[bRACE][bSEX] = fSCALE;
		CTChart::m_vTRACE[bRACE][bSEX] = dwOBJID;
		CTChart::m_setTRACEOBJID.insert(dwOBJID);
	}
}

void CTChart::InitGODTOWER( CString strPath )
{
	ReleaseGODTOWER();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary );
	CArchive ar( &file, CArchive::load );

	WORD wCount;
	ar >> wCount;

	for( int i=0; i<wCount; ++i )
	{
		LPTGODTOWER pTower = new TGODTOWER();

		ar	>> pTower->m_strName
			>> pTower->m_dwID
			>> pTower->m_fRot
			>> pTower->m_dwOBJID
			>> pTower->m_dwIconID
			>> pTower->m_dwSfxAtkID
			>> pTower->m_dwSfxDefID
			>> pTower->m_dwSfxNorID;

		m_mapTGODTOWER.insert( MAPTGODTOWER::value_type( (WORD)pTower->m_dwID, pTower ) );
	}
}

void CTChart::ReleaseGODTOWER()
{
	MAPTGODTOWER::iterator itor = m_mapTGODTOWER.begin();

	while( itor != m_mapTGODTOWER.end() )
	{
		if( (*itor).second != NULL )
		{
			delete (*itor).second;
			(*itor).second = NULL;
		}
		
		++itor;
	}

	m_mapTGODTOWER.clear();
}

LPTGODTOWER CTChart::FindTGODTOWER( WORD wTowerID )
{
	MAPTGODTOWER::iterator find = m_mapTGODTOWER.find( wTowerID );

	if( find == m_mapTGODTOWER.end() )
		return NULL;

	return (*find).second;
}

void CTChart::InitGODBALL( CString strPath )
{
	ReleaseGODBALL();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary );
	CArchive ar( &file, CArchive::load );
	
	WORD wCount;
	ar	>> wCount;

	for( int i=0; i<wCount; ++i )
	{
		LPTGODBALL pBall = new TGODBALL();

		ar	>> pBall->m_strName
			>> pBall->m_dwID
			>> pBall->m_fRot
			>> pBall->m_dwOBJID
			>> pBall->m_dwIconID
			>> pBall->m_dwSfxID;

		m_mapTGODBALL.insert( MAPTGODBALL::value_type( (WORD)pBall->m_dwID, pBall ) );
	}
}

void CTChart::ReleaseGODBALL()
{
	MAPTGODBALL::iterator itor = m_mapTGODBALL.begin();

	while( itor != m_mapTGODBALL.end() )
	{
		if( (*itor).second != NULL )
		{
			delete (*itor).second;
			(*itor).second = NULL;
		}
		
		++itor;
	}

	m_mapTGODBALL.clear();
}

LPTGODBALL CTChart::FindTGODBALL( WORD wBallID )
{
	MAPTGODBALL::iterator find = m_mapTGODBALL.find( wBallID );

	if( find == m_mapTGODBALL.end() )
		return NULL;

	return (*find).second;
}

void CTChart::InitMonShop( CString strPath )
{
	ReleaseTMonShop();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary );
	CArchive ar( &file, CArchive::load );
	
	WORD wCount;
	ar	>> wCount;

	for( int i=0; i<wCount; ++i )
	{
		LPTMONSHOP pMonShop = new TMONSHOP();

		ar	>> pMonShop->m_strName
			>> pMonShop->m_dwID
			>> pMonShop->m_dwSpawnID
			>> pMonShop->m_dwIcon;

		m_mapTMONSHOP.insert( MAPTMONSHOP::value_type( (DWORD)pMonShop->m_dwID, pMonShop ) );
	}
}

void CTChart::ReleaseTMonShop()
{
	MAPTMONSHOP::iterator itor = m_mapTMONSHOP.begin();

	while( itor != m_mapTMONSHOP.end() )
	{
		if( (*itor).second != NULL )
		{
			delete (*itor).second;
			(*itor).second = NULL;
		}
		
		++itor;
	}

	m_mapTMONSHOP.clear();
}

LPTMONSHOP CTChart::FindTMonShop( DWORD dwID )
{
	MAPTMONSHOP::iterator find = m_mapTMONSHOP.find( dwID );

	if( find == m_mapTMONSHOP.end() )
		return NULL;

	return (*find).second;
}

void CTChart::InitTPortalRegion( CString strPath )
{
	ReleaseTPortalRegion();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary );
	CArchive ar( &file, CArchive::load );

	WORD wCount;
	ar	>> wCount;

	for( int i=0; i<wCount; ++i )
	{
		LPTPORTALREGION pPortalRegion = new TPORTALREGION();

		ar	>> pPortalRegion->m_wID
			>> pPortalRegion->m_strName;

		m_mapTPORTALREGION.insert( MAPTPORTALREGION::value_type( (WORD)pPortalRegion->m_wID, pPortalRegion ) );
	}
}

void CTChart::ReleaseTPortalRegion()
{
	MAPTPORTALREGION::iterator itor = m_mapTPORTALREGION.begin();

	while( itor != m_mapTPORTALREGION.end() )
	{
		if( (*itor).second != NULL )
		{
			delete (*itor).second;
			(*itor).second = NULL;
		}

		++itor;
	}

	m_mapTPORTALREGION.clear();
}

LPTPORTALREGION CTChart::FindTPORTALREGION( WORD dwID )
{
	MAPTPORTALREGION::iterator find = m_mapTPORTALREGION.find( (WORD)dwID );

	if( find == m_mapTPORTALREGION.end() )
		return NULL;

	return (*find).second;
}


void CTChart::InitTNATION( CString strPath )
{
	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary );
	CArchive ar( &file, CArchive::load );

	for( INT i=0 ; i < TNATIONOPTION_COUNT ; ++i )
	{
		CString strCHAR;
		ar	>> strCHAR;
		CTChart::m_TNATIONOPTION[ i ] = atoi( strCHAR.GetBuffer() );
	}
}

void CTChart::InitTCURSE( CString strPath )
{
	ReleaseTCURSE();

	CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary );
	CArchive ar( &file, CArchive::load );
	
	WORD wCount;
	ar	>> wCount;

	CTChart::m_pTWORDCURSE = new CString[ wCount ];

	for( int i=0; i<wCount; ++i )
		ar >> CTChart::m_pTWORDCURSE[ i ];

	for( int i=0; i<wCount; ++i )
	{
		BOOL bNameCurse, bChatCurse;

		ar >> bNameCurse
			>> bChatCurse;

		if( bNameCurse )
			CTChart::m_vTNAMECURSE.push_back( &CTChart::m_pTWORDCURSE[ i ] );
		if( bChatCurse )
			CTChart::m_vTCHATCURSE.push_back( &CTChart::m_pTWORDCURSE[ i ] );
	}
}

void CTChart::ReleaseTCURSE()
{
	if( CTChart::m_pTWORDCURSE )
		delete [] CTChart::m_pTWORDCURSE;

	CTChart::m_pTWORDCURSE = NULL;

	CTChart::m_vTNAMECURSE.clear();
	CTChart::m_vTCHATCURSE.clear();
}

BOOL CTChart::CheckNameCurse( const CString& )
{
	return FALSE;
}

BOOL CTChart::CheckChatCurse( const CString& )
{
	return FALSE;
}

void CTChart::InitTAUCTIONTREE( CString strPath )
{
	ReleaseTAUCTIONTREE();

	CFile file( LPCSTR( strPath ), CFile::modeRead | CFile::typeBinary );
	CArchive ar( &file, CArchive::load );

	WORD wCount = 0;
	ar >> (WORD) wCount;

	size_t i = 0;
	for( ; i < wCount; ++i )
	{
		LPTAUCTIONTREE pAucTree = new TAUCTIONTREE;
		if( pAucTree )
		{
			ar
				>> pAucTree->m_strName
				>> (DWORD)pAucTree->m_ID.dwValue;

			m_vTAUCTIONTREE.push_back( pAucTree );
		}
	}
}

void CTChart::ReleaseTAUCTIONTREE()
{
	size_t size = m_vTAUCTIONTREE.size();
	size_t i = 0;
	
	for( ; i < size; ++i )
	{
		delete m_vTAUCTIONTREE[i];
		m_vTAUCTIONTREE[i] = NULL;
	}

	m_vTAUCTIONTREE.clear();
}


void CTChart::InitTMANTLEINFO( CString strPath, CString strPath2 )
{
	ReleaseTMANTLEINFO();

	std::map<CString, DWORD> mapSTRtoID;

	// Load from strPath
	{
		CFile file( LPCSTR(strPath), CFile::modeRead|CFile::typeBinary );
		CArchive ar( &file, CArchive::load );
		
		WORD wCount;
		ar	>> wCount;

		for( int i=0; i<wCount; ++i )
		{
			BYTE bRaceID;
			BYTE bSexID;
			DWORD dwMesh;
			DWORD dwCond;
			CString strMeshName;

			ar >> bRaceID
				>> bSexID
				>> dwMesh
				>> dwCond
				>> strMeshName;

			MAPTMANTLECOORD::iterator finder2 = m_mapTMANTLECOORD.find( TMANTLECOORDKEY( bRaceID, bSexID, dwMesh ) );
			if( finder2 == m_mapTMANTLECOORD.end() )
			{
				LPTMANTLECOORDINFO p = new TMANTLECOORDINFO;
				p->m_strMeshName = strMeshName;
				m_mapTMANTLECOORD.insert( make_pair( TMANTLECOORDKEY( bRaceID, bSexID, dwMesh ), p ) );
				mapSTRtoID.insert( make_pair( strMeshName, dwMesh ) );
			}
			else
				TRACE("경고 : Mantle차트 dwDetailTextureID 중복(%u)\n", dwMesh);
		}
	}

	BOOL bSizeConvert = FALSE;

	// Load from strPath2
	CFile file2;
	if( file2.Open( LPCSTR(strPath2), CFile::modeRead|CFile::typeBinary ) )
	{
		CArchive ar( &file2, CArchive::load );

		DWORD dwVer = 0;
		ar >> dwVer;
		switch(dwVer)
		{
		case MANTLECOORD_VER:
			break;
		case MANTLECOORD_VER_OLD:
			bSizeConvert = TRUE;
			break;
		default:
			file2.Close();
			return ;
		}

		WORD wCount;
		ar	>> wCount;

		for( int i=0; i<wCount; ++i )
		{
			BYTE bRaceID;
			BYTE bSexID;
			CString strMeshName;

			ar >> bRaceID
				>> bSexID
				>> strMeshName;

			std::map<CString, DWORD>::iterator id = mapSTRtoID.find( strMeshName );
			BOOL bFind = (id != mapSTRtoID.end());

			LPTMANTLECOORDINFO pMantleCoord = NULL;
			if( bFind )
			{
				MAPTMANTLECOORD::iterator finder = m_mapTMANTLECOORD.find( TMANTLECOORDKEY( bRaceID, bSexID, id->second ) );
				if( finder != m_mapTMANTLECOORD.end() )
					pMantleCoord = finder->second;
				else
					bFind = FALSE;
			}

			DWORD dwUV1Count;
			ar >> dwUV1Count;
			for( DWORD u=0 ; u < dwUV1Count ; ++u )
			{
				TMANTLECOORD coord;
				ar >> coord.m_vLeftTop.x
					>> coord.m_vLeftTop.y
					>> coord.m_vRightTop.x
					>> coord.m_vRightTop.y
					>> coord.m_vLeftBottom.x
					>> coord.m_vLeftBottom.y
					>> coord.m_vRightBottom.x
					>> coord.m_vRightBottom.y;

				if(bSizeConvert)
				{
					if( TMANTLECOORD::IsDefaultCoordUV1_OLD( coord ) )
						coord = TMANTLECOORD::m_vDefaultUV1;
					else
					{
						coord.m_vLeftTop = coord.m_vLeftTop / 127.0f * 255.0f;
						coord.m_vRightTop = coord.m_vRightTop / 127.0f * 255.0f;
						coord.m_vLeftBottom = coord.m_vLeftBottom / 127.0f * 255.0f;
						coord.m_vRightBottom = coord.m_vRightBottom / 127.0f * 255.0f;
					}
				}

				if( bFind && pMantleCoord )
					pMantleCoord->m_vUV1.push_back( coord );
			}

			DWORD dwUV2Count;
			ar >> dwUV2Count;
			for( DWORD u=0 ; u < dwUV2Count ; ++u )
			{
				TMANTLECOORD coord;
				ar >> coord.m_vLeftTop.x
					>> coord.m_vLeftTop.y
					>> coord.m_vRightTop.x
					>> coord.m_vRightTop.y
					>> coord.m_vLeftBottom.x
					>> coord.m_vLeftBottom.y
					>> coord.m_vRightBottom.x
					>> coord.m_vRightBottom.y;

				if(bSizeConvert)
				{
					if( TMANTLECOORD::IsDefaultCoordUV2_OLD( coord ) )
						coord = TMANTLECOORD::m_vDefaultUV2;
					else
					{
						coord.m_vLeftTop = coord.m_vLeftTop / 127.0f * 255.0f;
						coord.m_vRightTop = coord.m_vRightTop / 127.0f * 255.0f;
						coord.m_vLeftBottom = coord.m_vLeftBottom / 127.0f * 255.0f;
						coord.m_vRightBottom = coord.m_vRightBottom / 127.0f * 255.0f;
					}
				}

				if( bFind && pMantleCoord )
					pMantleCoord->m_vUV2.push_back( coord );
			}

			DWORD dwUV3Count;
			ar >> dwUV3Count;
			for( DWORD u=0 ; u < dwUV3Count ; ++u )
			{
				TMANTLECOORD coord;
				ar >> coord.m_vLeftTop.x
					>> coord.m_vLeftTop.y
					>> coord.m_vRightTop.x
					>> coord.m_vRightTop.y
					>> coord.m_vLeftBottom.x
					>> coord.m_vLeftBottom.y
					>> coord.m_vRightBottom.x
					>> coord.m_vRightBottom.y;

				if(bSizeConvert)
				{
					if( TMANTLECOORD::IsDefaultCoordUV3_OLD( coord ) )
						coord = TMANTLECOORD::m_vDefaultUV3;
					else
					{
						coord.m_vLeftTop = coord.m_vLeftTop / 127.0f * 255.0f;
						coord.m_vRightTop = coord.m_vRightTop / 127.0f * 255.0f;
						coord.m_vLeftBottom = coord.m_vLeftBottom / 127.0f * 255.0f;
						coord.m_vRightBottom = coord.m_vRightBottom / 127.0f * 255.0f;
					}
				}

				if( bFind && pMantleCoord )
					pMantleCoord->m_vUV3.push_back( coord );
			}
		}
	}
}

void CTChart::SaveTMANTLECOORDINFO( CString strPath2 )
{
	CFile file;
	if( file.Open( strPath2, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary ) )
	{
		CArchive ar(&file, CArchive::store);

		DWORD dwVer = MANTLECOORD_VER;
		ar << dwVer;

		MAPTMANTLECOORD::iterator it, end;
		it = m_mapTMANTLECOORD.begin();
		end = m_mapTMANTLECOORD.end();

		ar << (WORD)( m_mapTMANTLECOORD.size() );

		for(; it != end ; ++it )
		{
			LPTMANTLECOORDINFO p = it->second;

			ar << (BYTE)( it->first.m_bRaceID )
				<< (BYTE)( it->first.m_bSexID )
				<< p->m_strMeshName;

			ar << (DWORD)( TMAXGUILD_MARKBACK );
			for( DWORD u=0 ; u < TMAXGUILD_MARKBACK ; ++u )
			{
				if( u < p->m_vUV1.size() )
				{
					TMANTLECOORD& v = p->m_vUV1[ u ];
					ar << v.m_vLeftTop.x
						<< v.m_vLeftTop.y
						<< v.m_vRightTop.x
						<< v.m_vRightTop.y
						<< v.m_vLeftBottom.x
						<< v.m_vLeftBottom.y
						<< v.m_vRightBottom.x
						<< v.m_vRightBottom.y;
				}
				else
				{
					ar << TMANTLECOORD::m_vDefaultUV1.m_vLeftTop.x
						<< TMANTLECOORD::m_vDefaultUV1.m_vLeftTop.y
						<< TMANTLECOORD::m_vDefaultUV1.m_vRightTop.x
						<< TMANTLECOORD::m_vDefaultUV1.m_vRightTop.y
						<< TMANTLECOORD::m_vDefaultUV1.m_vLeftBottom.x
						<< TMANTLECOORD::m_vDefaultUV1.m_vLeftBottom.y
						<< TMANTLECOORD::m_vDefaultUV1.m_vRightBottom.x
						<< TMANTLECOORD::m_vDefaultUV1.m_vRightBottom.y;
				}
			}

			ar << (DWORD)( TMAXGUILD_MARKBACK );
			for( DWORD u=0 ; u < TMAXGUILD_MARKBACK ; ++u )
			{
				if( u < p->m_vUV2.size() )
				{
					TMANTLECOORD& v = p->m_vUV2[ u ];
					ar << v.m_vLeftTop.x
						<< v.m_vLeftTop.y
						<< v.m_vRightTop.x
						<< v.m_vRightTop.y
						<< v.m_vLeftBottom.x
						<< v.m_vLeftBottom.y
						<< v.m_vRightBottom.x
						<< v.m_vRightBottom.y;
				}
				else
				{
					ar << TMANTLECOORD::m_vDefaultUV2.m_vLeftTop.x
						<< TMANTLECOORD::m_vDefaultUV2.m_vLeftTop.y
						<< TMANTLECOORD::m_vDefaultUV2.m_vRightTop.x
						<< TMANTLECOORD::m_vDefaultUV2.m_vRightTop.y
						<< TMANTLECOORD::m_vDefaultUV2.m_vLeftBottom.x
						<< TMANTLECOORD::m_vDefaultUV2.m_vLeftBottom.y
						<< TMANTLECOORD::m_vDefaultUV2.m_vRightBottom.x
						<< TMANTLECOORD::m_vDefaultUV2.m_vRightBottom.y;
				}
			}

			ar << (DWORD)( TMAXGUILD_MARK );
			for( DWORD u=0 ; u < TMAXGUILD_MARK ; ++u )
			{
				if( u < p->m_vUV3.size() )
				{
					TMANTLECOORD& v = p->m_vUV3[ u ];
					ar << v.m_vLeftTop.x
						<< v.m_vLeftTop.y
						<< v.m_vRightTop.x
						<< v.m_vRightTop.y
						<< v.m_vLeftBottom.x
						<< v.m_vLeftBottom.y
						<< v.m_vRightBottom.x
						<< v.m_vRightBottom.y;
				}
				else
				{
					ar << TMANTLECOORD::m_vDefaultUV3.m_vLeftTop.x
						<< TMANTLECOORD::m_vDefaultUV3.m_vLeftTop.y
						<< TMANTLECOORD::m_vDefaultUV3.m_vRightTop.x
						<< TMANTLECOORD::m_vDefaultUV3.m_vRightTop.y
						<< TMANTLECOORD::m_vDefaultUV3.m_vLeftBottom.x
						<< TMANTLECOORD::m_vDefaultUV3.m_vLeftBottom.y
						<< TMANTLECOORD::m_vDefaultUV3.m_vRightBottom.x
						<< TMANTLECOORD::m_vDefaultUV3.m_vRightBottom.y;
				}
			}
		}
	}
}

void CTChart::ReleaseTMANTLEINFO()
{
	MAPTMANTLECOORD::iterator it, end;
	it = m_mapTMANTLECOORD.begin();
	end = m_mapTMANTLECOORD.end();

	for(; it != end ; ++it )
	{
		if( it->second )
			delete it->second;
	}

	m_mapTMANTLECOORD.clear();
}

LPTMANTLECOORDINFO CTChart::GetTMANTLECOORD( TMANTLECOORDKEY key )
{
	MAPTMANTLECOORD::iterator it = m_mapTMANTLECOORD.find( key );
	if( it != m_mapTMANTLECOORD.end() )
		return it->second;
	else
		return NULL;
}

void CTChart::InitTFAMETITLE( CString strPath )
{
	ReleaseTFAMETITLE();

	CFile file( LPCSTR( strPath ), CFile::modeRead | CFile::typeBinary );
	CArchive ar( &file, CArchive::load );

	WORD wCount = 0;
	ar >> (WORD) wCount;

	size_t i = 0;
	for( ; i < wCount; ++i )
	{
		LPTFAMETITLE pTFAMERANK = new TFAMETITLE;

		if ( pTFAMERANK )
		{
			BYTE bRANK;
			ar 
				>> bRANK
				>> pTFAMERANK->m_strFameName
				>> pTFAMERANK->m_bSFX
				>> pTFAMERANK->m_bSFXID;

			m_mapTFAMETITLE.insert( MAPTFAMETITLE::value_type( bRANK, pTFAMERANK ) );
		}
	}
}

void CTChart::ReleaseTFAMETITLE()
{
	MAPTFAMETITLE::iterator itor = m_mapTFAMETITLE.begin();

	while( itor != m_mapTFAMETITLE.end() )
	{
		if ( (*itor).second != NULL )
		{
			delete (*itor).second;
			(*itor).second = NULL;
		}
		++itor;
	}

	m_mapTFAMETITLE.clear();
}

LPTFAMETITLE CTChart::FindTFAMETITLE( BYTE bRank )
{
	MAPTFAMETITLE::iterator find = m_mapTFAMETITLE.find( bRank );

	if ( find == m_mapTFAMETITLE.end() )
		return NULL;

	return (*find).second;
}


void CTChart::InitTEQUIPCREATECHAR( CString strPath )
{
	ReleaseTEQUIPCREATECHAR();

	CFile file( LPCSTR( strPath ), CFile::modeRead | CFile::typeBinary );
	CArchive ar( &file, CArchive::load );

	WORD wCount = 0;
	ar >> (WORD) wCount;

	WORD i = 0;
	for( ; i < wCount; ++i )
	{
		LPTEQUIPCREATECHAR pTEquip = new TEQUIPCREATECHAR;

		if( pTEquip )
		{
			ar 
				>> pTEquip->m_bCountry
				>> pTEquip->m_bClass
				>> pTEquip->m_bSex;

			size_t j = 0;
			for( ; j < TEQUIPCREATECHAR::TE_COUNT; ++j )
			{
				ar 
					>> pTEquip->m_wItemID[j]
					>> pTEquip->m_wGrade[j]
					>> pTEquip->m_bGradeEffect[j];
			}

			DWORD dwKey = pTEquip->m_bClass + ( pTEquip->m_bSex * 64 ) + ( pTEquip->m_bCountry * 128 );
			m_mapTEquipCreateChar.insert( make_pair( dwKey, pTEquip ) );
		}
	}
}

void CTChart::ReleaseTEQUIPCREATECHAR()
{
	MAPTEQUIPCREATECHAR::iterator itor = m_mapTEquipCreateChar.begin();

	while( itor != m_mapTEquipCreateChar.end() )
	{
		if ( (*itor).second != NULL )
		{
			delete (*itor).second;
			(*itor).second = NULL;
		}
		++itor;
	}

	m_mapTEquipCreateChar.clear();
}

LPTEQUIPCREATECHAR CTChart::FindTEquipCreateChar( DWORD dwKey )
{
	MAPTEQUIPCREATECHAR::iterator find = m_mapTEquipCreateChar.find( dwKey );

	if( find == m_mapTEquipCreateChar.end() )
		return NULL;

	return (*find).second;
}

void CTChart::InitTRPS( CString strPath )
{
	ReleaseTRPS();

	CFile file( LPCSTR( strPath ), CFile::modeRead | CFile::typeBinary );
	CArchive ar( &file, CArchive::load );

	WORD wCount = 0;
	ar >> (WORD) wCount;

	WORD i = 0;
	for( ; i < wCount; ++i )
	{
		LPTRPS pTRps = new TRPS;
		BYTE bType = 0;
		BYTE bWin = 0;

		ar >> bType
			>> bWin
			>> pTRps->wNeedItem
			>> pTRps->wRewardItem1
			>> pTRps->bRewardCount1
			>> pTRps->wRewardItem2
			>> pTRps->bRewardCount2
			>> pTRps->dwRewardMoney;

		m_mapTRPS.insert( MAPTRPS::value_type(MAKEWORD(bType, bWin), pTRps) );
	}
}

void CTChart::ReleaseTRPS()
{
	MAPTRPS::iterator it, end;
	it = m_mapTRPS.begin();
	end = m_mapTRPS.end();
	for(; it != end ; ++it )
	{
		if( it->second )
			delete it->second;
	}
	m_mapTRPS.clear();
}

LPTRPS CTChart::FindTRPS( BYTE bType, BYTE bWin )
{
	MAPTRPS::iterator find = m_mapTRPS.find( MAKEWORD(bType, bWin) );

	if ( find == m_mapTRPS.end() )
		return NULL;

	return (*find).second;
}
