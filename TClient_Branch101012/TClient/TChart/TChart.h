#pragma once

class CTChart
{
public:
	// Utility Method
	static CString ReadString( CFile *pFILE);

	// Game Logic Method
	static BOOL IsHideDetectSkill( LPTSKILL );

public:
	static CString m_strNationCode;

	static BOOL InitTString( CString strPath );
	static void ReleaseTString();
	static CString LoadString( TSTRING );
	static CString Format( TSTRING, ... );

	static void InitTSKILLTEMP( CString strPath );
	static void ReleaseTSKILLTEMP();
	static LPTSKILL FindTSKILLTEMP( WORD wSkillID);
	static BOOL IsNegativeSkill( LPTSKILL );

	static void InitTSKILLFUNCTION( CString strPath );
	static void InitTSKILLPOINT( CString strPath );
	static void InitTARROWDIR( CString strPath );

	static void InitTCOUNTRYINFO( CString strPath );
	static void ReleaseTCOUNTRYINFO();
	static CString GetTCOUNTRYINFO( DWORD dwID );

	static void InitTRACEINFO( CString strPath );
	static void ReleaseTRACEINFO();
	static CString GetTRACEINFO( DWORD dwID );

	static void InitTCLASSINFO( CString strPath );
	static void ReleaseTCLASSINFO();
	static CString GetTCLASSINFO( DWORD dwID );

	static void InitTDETAILINFO( CString strPath );
	static void ReleaseTDETAILINFO();
	static LPTDETAILINFO FindTDETAILINFO( DWORD dwInfoID );

	static void InitTREGIONINFO( CString strPath );
	static void ReleaseTREGIONINFO();
	static LPTREGIONINFO FindTREGIONINFO( DWORD dwRegionID);

	static void InitTBGM( CString strPath );
	static void ReleaseTBGM();

	static void InitTENV( CString strPath );
	static void ReleaseTENV();

	static void InitTACTIONTEMP( CString strPath );
	static void ReleaseTACTIONTEMP();
	static LPTACTION FindTACTIONTEMP( BYTE bActionID, BYTE bWeaponID );

	static void InitTACTIONDATA( CString strPath );
	static void ReleaseTACTIONDATA();
	static LPTACTIONDATA GetTACTION( INT nAction );

	static void InitTFACETEMP( CString strPath );
	static void InitTPANTSTEMP( CString strPath );
	static void InitTBODYTEMP( CString strPath );
	static void InitTHANDTEMP( CString strPath );
	static void InitTFOOTTEMP( CString strPath );
	
	static void InitTSKILLTREE( CString strPath );
	static void ReleaseTSKILLTREE();
	static WORD GetTSKILLID( BYTE bContryID, BYTE bClassID, BYTE bTabID, BYTE bTreeIndex );

	static void InitTQUESTTEMP( CString strPath );
	static void ReleaseTQUESTTEMP();
	static LPTQCLASS FindTQCLASS( DWORD dwClassID );
	static LPTQUEST FindTQUEST( DWORD dwQuestID );
	static LPTQUEST FindTMISSION( LPTQUEST pTQUEST);

	static void InitTPOPUPITEM( CString strPath );
	static void ReleaseTPOPUPITEM();
	static LPTPOPUPITEM FindTPOPUPITEM( DWORD dwMenuID);

	static void InitTPOPUPMENU( CString strPath );
	static void ReleaseTPOPUPMENU();
	static LPTPOPUPMENU FindTPOPUPMENU( DWORD dwPopupID);

	static void InitTMAPJOINT( CString strPath );
	static void ReleaseTMAPJOINT();
	static LPVTMAPJOINT GetTMAPJOINT( WORD wMapID, BYTE bUnitX, BYTE bUnitZ);

	static void InitTMAPINFO( CString strNodeChartPath, CString strMapCharPath );
	static void ReleaseTMAPINFO();
	static LPTMAPINFO GetTMAPINFO( DWORD dwMapID);

	static void InitTPORTAL( CString strPath );
	static void ReleaseTPORTAL();
	static LPTPORTAL GetTPORTAL( WORD wPortalID);

	static void InitTMINIMAP( CString strPath );
	static void ReleaseTMINIMAP();

	static void InitTITEMTEMP( CString strPath );
	static void ReleaseTITEMTEMP();
	static LPTITEM FindTITEMTEMP( WORD wItemID);
	static LPTITEM FindTCANTRADETEMP( WORD wItemID );

	static void InitTITEMVISUAL( CString strPath );
	static void ReleaseTITEMVISUAL();
	static LPTITEMVISUAL FindTITEMVISUAL( WORD wVisualID);

	static void InitTQUESTMAGICITEM( CString strPath );
	static void ReleaseTQUESTMAGICITEM();
	static LPTQUESTITEM FindTQUESTMAGICITEM( WORD wRewardID );

	static void InitTITEMGRADESFX( CString strPath );
	static void ReleaseTITEMGRADESFX();
	static LPTITEMGRADESFX FindTITEMGRADESFX( WORD wGradeSFX );

	static void InitTITEMMAGICSFX( CString strPath );
	static void ReleaseTITEMMAGICSFX();
	static LPTITEMMAGICSFX FindTITEMMAGICSFX( WORD wSFX );

	static void InitTSFXTEMP( CString strPath );
	static void ReleaseTSFXTEMP();
	static LPTSFXTEMP FindTSFXTEMP( DWORD dwFuncID);

	static void InitTMONTEMP( CString strPath );
	static void ReleaseTMONTEMP();
	static LPTMONTEMP FindTMONTEMP( WORD wMonID );

	static void InitTNPCTEMP( CString strPath );
	static void ReleaseTNPCTEMP();
	static LPTNPCTEMP FindTNPCTEMP( WORD wTempID);

	static void InitTNPCGlobal( CString strPath );
	static void ReleaseTNPCGlobal();
	static LPTNPC FindTNPCGlobal( WORD wID );
	static void ReadTNPC( CArchive& ar, LPTNPC );

	static void InitTSTEPSND( CString strPath );
	static void ReleaseTSTEPSND();

	static void InitTFORMULA( CString strPath );
	static void ReleaseTFORMULA();
	static LPTFORMULA FindTFORMULA( BYTE bID);

	static void InitTSKYBOX( CString strPath );
	static void ReleaseTSKYBOX();
	static LPTSKYBOX FindTSKYBOX( BYTE bUnitCountX, BYTE bIndexID, WORD wMapID, BYTE bUnitX, BYTE bUnitZ);

	static void InitTSWITCH( CString strPath );
	static void ReleaseTSWITCH();
	static LPTSWITCH FindTSWITCH( DWORD dwID );

	static void InitTLIGHT( CString strPath );
	static void ReleaseTLIGHT();
	static LPTLIGHT FindTLIGHT( BYTE bUnitCountX, BYTE bSetID, BYTE bIndexID, WORD wMapID, BYTE bUnitX, BYTE bUnitZ );

	static void InitTLEVEL( CString strPath );
	static void ReleaseTLEVEL();
	static DWORD GetLevelPrice( BYTE bLevel);
	static DWORD GetSoulmateSearchPrice( BYTE bLevel);
	static DWORD GetSoulmateRegPrice( BYTE bLevel);

	static void InitTFOG( CString strPath );
	static void ReleaseTFOG();
	static LPTUNITFOG FindTFOG( DWORD dwFogID);
	static LPTUNITFOG FindTFOG( WORD wMapID, BYTE bUnitX, BYTE bUnitZ);

	static void InitTGATE( CString strPath );
	static void ReleaseTGATE();
	static LPTGATE FindTGATE( DWORD dwID );

	static void InitTITEMMAGIC( CString strPath );
	static void ReleaseTITEMMAGIC();
	static LPTITEMMAGIC FindTITEMMAGIC( WORD wMagicID );

	static void InitTPET( CString strPath );
	static void ReleaseTPET();
	static LPTPET FindTPETTEMP(WORD wPetID);

	static void InitTITEMATTR( CString strPath );
	static void ReleaseTITEMATTR();
	static LPTITEMATTR FindTITEMATTR( WORD wAttrID );

	static void InitTITEMGRADE( CString strPath );
	static void InitTITEMGRADEVISUAL( CString strPath );

	static void InitTHelp( CString strPath );
	static void ReleaseTHelp();
	static LPTHELP FindTHELP( DWORD dwHelpID );

	static void InitTHelpLink( CString strPath );
	static void ReleaseTHelpLink();

	static void InitTDYNAMICHELP( CString strPath );
	static CString FindTDYNAMICHELP(DWORD dwID);
	static void ReleaseTDYNAMICHELP();

	static void InitTRACETEMP( CString strPath );

	static void InitGODTOWER( CString strPath );
	static void ReleaseGODTOWER();
	static LPTGODTOWER FindTGODTOWER( WORD wTowerID );

	static void InitGODBALL( CString strPath );
	static void ReleaseGODBALL();
	static LPTGODBALL FindTGODBALL( WORD wBallID );

	static void InitMonShop( CString strPath );
	static void ReleaseTMonShop();
	static LPTMONSHOP FindTMonShop( DWORD dwID );

	static void InitTPortalRegion( CString strPath );
	static void ReleaseTPortalRegion();
	static LPTPORTALREGION FindTPORTALREGION( WORD wRegionID );

	static void InitTNATION( CString strPath );

	static void InitTCURSE( CString strPath );
	static void ReleaseTCURSE();
	static BOOL CheckNameCurse( const CString& );
	static BOOL CheckChatCurse( const CString& );

	static void InitTAUCTIONTREE( CString strPath );
	static void ReleaseTAUCTIONTREE();

	static void InitTMANTLEINFO( CString strPath, CString strPath2 );
	static void ReleaseTMANTLEINFO();

	static void InitTFAMETITLE( CString strPath );
	static void ReleaseTFAMETITLE();
	static LPTFAMETITLE FindTFAMETITLE( BYTE bRank );
	
	static LPTMANTLECOORDINFO GetTMANTLECOORD( TMANTLECOORDKEY );
	static DWORD GetTMANTLEDETAIL( TMANTLEDETAILKEY );

	static void SaveTMANTLECOORDINFO( CString strPath2 );

	static void InitTEQUIPCREATECHAR( CString strPath );
	static void ReleaseTEQUIPCREATECHAR();
	static LPTEQUIPCREATECHAR FindTEquipCreateChar( DWORD dwKey );

	static void InitTRPS( CString strPath );
	static void ReleaseTRPS();
	static LPTRPS FindTRPS( BYTE bType, BYTE bWin );

public:
	static CString* m_vTSTRING;
	static MAPTSKILLTEMP m_mapTSKILLTEMP;
	static MAPTSTRING m_mapTCOUNTRYINFO;
	static MAPTSTRING m_mapTRACEINFO;
	static MAPTSTRING m_mapTCLASSINFO;
	static MAPTDETAILINFO m_mapTDETAILINFO;	
	static MAPTREGIONINFO m_mapTREGIONINFO;
	static MAPTBGM m_mapTBGMINFO;
	static MAPTENV m_mapTENVINFO;
	static MAPTACTION m_mapTACTIONTEMP;
	static VTACTIONDATA m_vTACTION;
	static TCLOTH m_vTFACE[TFACE_COUNT][THAIR_COUNT];
	static TCLOTH m_vTPANTS[TPANTS_COUNT];
	static TCLOTH m_vTBODY[TBODY_COUNT];
	static TCLOTH m_vTHAND[THAND_COUNT];
	static TCLOTH m_vTFOOT[TFOOT_COUNT];
	static FLOAT m_vTRACESCALE[TMAINRACE_COUNT][TSEX_COUNT];
	static DWORD m_vTRACE[TMAINRACE_COUNT][TSEX_COUNT];
	static SETDWORD m_setTRACEOBJID;
	static MAPWORD m_mapTSKILLTREE[TCONTRY_COUNT][TCLASS_COUNT][TSKILLTAB_COUNT];
	static MAPTQUESTTEMP m_mapTQUESTTEMP;
	static MAPTQCLASSTEMP m_mapTQCLASSTEMP;
	static TQCLASS m_vNULLCLASS;
	static MAPTPOPUPITEM m_mapTMENUITEM;
	static MAPTPOPUPMENU m_mapTPOPUP;
	static MAPVTMAPJOINT m_mapTMAPJOINT;
	static MAPTMAPINFO m_mapTMAPINFO;
	static MAPTPORTAL m_mapTPORTALINFO;
	static MAPTMINIMAP m_mapTMINIMAP;
	static MAPDWORD m_mapTBTNID2WORLD;
	static MAPDWORD m_mapTBTNID2UNIT;
	static MAPTITEMTEMP m_mapTITEMTEMP;
	static MAPTITEMTEMP m_mapTCANTRADETEMP;
	static MAPTITEMVISUAL m_mapTITEMVISUAL;
	static MAPTQUESTITEM m_mapTQUESTMAGICITEM;
	static MAPTITEMGRADESFX m_mapTITEMGRADESFX;
	static MAPTITEMMAGICSFX m_mapTITEMMAGICSFX;
	static MAPTSFXTEMP m_mapTSFXTEMP;
	static MAPMONTEMP m_mapMONTEMP;
	static MAPNPCTEMP m_mapTNPCTEMP;
	static MAPNPC m_mapGlobalNPC;
	static MAPDWORD m_mapTSTEPSND;
	static MAPTFORMULA m_mapTFORMULA;
	static FLOAT m_vLEVELRATE[TMAX_LEVEL];
	static MAPTSWITCHDATA m_mapTSWITCH;
	static MAPTSKYBOX m_mapTSKYBOX;
	static MAPTLIGHT m_mapTLIGHT;
	static MAPTLEVEL m_mapTLEVEL;
	static MAPTUNITFOG m_mapTFOG;
	static MAPTGATEDATA m_mapTGATE;
	static MAPTITEMMAGIC m_mapITEMMAGIC;
	static MAPTPET m_mapTPET;
	static MAPTITEMATTR m_mapTITEMATTR;
	static VTITEMGRADE m_vTITEMGRADE;
	static MAPTITEMGRADEVISUAL m_mapTITEMGRADEVISUAL;
	static MAPTHELP m_mapTHELP;
	static MAPTHELPLINK m_mapTHELPLINK[ THELPLINK_TRIGGER_COUNT ];
	static MAPTGODTOWER m_mapTGODTOWER;
	static MAPTGODBALL m_mapTGODBALL;
	static MAPTMONSHOP m_mapTMONSHOP;
	static INT m_TNATIONOPTION[ TNATIONOPTION_COUNT ];
	static CString* m_pTWORDCURSE;
	static VTPSTRING m_vTNAMECURSE;
	static VTPSTRING m_vTCHATCURSE;
	static MAPTPORTALREGION m_mapTPORTALREGION;
	static VTAUCTIONTREE m_vTAUCTIONTREE;
	static MAPTMANTLECOORD m_mapTMANTLECOORD;
	static MAPTFAMETITLE	m_mapTFAMETITLE;
	static MAPTEQUIPCREATECHAR	m_mapTEquipCreateChar;
	static MAPTDYNAMICHELP m_mapTDynamicHelp;
	static MAPTRPS m_mapTRPS;
};
