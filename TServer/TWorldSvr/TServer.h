#pragma once


class CTServer : public CTWorldSession
{
public:
	MAPBYTE m_mapCHANNEL;
	WORD m_wID;
	BYTE m_bCashSale;  

public:
	void WrapItem(CPacket * pPacket, LPTITEM pItem);

	void RelayCorpsMsg(
		DWORD dwCharID,
		DWORD dwKey,
		CPacket *pPacket,
		WORD wMsgID);

	// Control message sender
	void SendCT_SERVICEMONITOR_REQ(
		DWORD dwTick,
		DWORD dwSession,
		DWORD dwUser,
		DWORD dwActiveUser);

	void SendCT_USERMOVE_ACK(
		DWORD dwCharID,
		DWORD dwKEY,
		BYTE bChannel,
		WORD wMapID,
		FLOAT fPosX,
		FLOAT fPosY,
		FLOAT fPosZ,
		WORD wPartyID = 0);

	void SendCT_CHATBAN_ACK(
		BYTE bRet,
		DWORD dwBanSeq,
		DWORD dwManagerID);

	void SendCT_ITEMSTATE_ACK(CPacket* pPacket);

	void SendCT_CASTLEGUILDCHG_ACK(
		DWORD dwManagerID,
		BYTE bRet,
		WORD wCastleID = 0,
		DWORD dwDefGuildID=0,
		CString strDefGuild=NAME_NULL,
		DWORD dwAtkGuildID=0,
		CString strAtkGuildID=NAME_NULL,
		__time64_t tTime = 0);

	 
	void SendCT_CASHITEMSALE_ACK(DWORD dwIndex, WORD wValue);
	void SendCT_RPSGAMEDATA_ACK(BYTE bChange, BYTE bGroup, LPMAPRPSGAME pRPS);

	void SendCT_CMGIFTLIST_ACK(DWORD dwManager, LPMAPCMGIFT pMap);

	// World message sender
	void SendMW_MONTHRANKRESETCHAR_REQ(DWORD dwCharID);

	void SendMW_USERPOSITION_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		CString strGMName);

	void SendMW_HELPMESSAGE_REQ(
		BYTE bID,
		INT64 dlStart,
		INT64 dlEnd,
		CString strMessage);

	void SendMW_DEALITEMERROR_REQ(
		CString strTarget,
		CString strErrorChar,
		BYTE bError);

	void SendMW_MONTEMPT_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		WORD wMonID);

	void SendMW_ENTERSVR_REQ(
		BYTE bDBLoad,
		DWORD dwCharID,
		DWORD dwKEY);

	void SendMW_ENTERSVR_REQ(LPPACKETBUF pBUF);

	void SendMW_RELEASEMAIN_REQ(
		DWORD dwCharID,
		DWORD dwKEY,
		BYTE bChannel,
		WORD wMapID,
		FLOAT fPosX,
		FLOAT fPosY,
		FLOAT fPosZ);

	void SendMW_MAPSVRLIST_REQ(
		DWORD dwCharID,
		DWORD dwKEY,
		BYTE bChannel,
		WORD wMapID,
		FLOAT fPosX,
		FLOAT fPosY,
		FLOAT fPosZ);

	void SendMW_INVALIDCHAR_REQ(
		DWORD dwCharID,
		DWORD dwKEY,
		BYTE bReleaseMain = FALSE);

	void SendMW_CHARDATA_REQ(
		DWORD dwCharID,
		DWORD dwKEY);

	void SendMW_ENTERCHAR_REQ(
		DWORD dwCharID,
		DWORD dwKEY,
		BYTE bStartAct,
		CString strNAME,
		WORD wMapID,
		FLOAT fPosX,
		FLOAT fPosY,
		FLOAT fPosZ,
		DWORD dwGuildID,
		DWORD dwFame,
		DWORD dwFameColor,
		CString strGuildName,
		BYTE bGuildDuty,
		BYTE bGuildPeer,
		WORD wCastle,
		BYTE bCamp,
		DWORD dwTacticsID,
		CString strTacticsName,
		WORD wPartyID,
		BYTE bPartyType,
		DWORD dwPartyChiefID,
		WORD wCommander,
		BYTE bLevel,
		BYTE bHelmetHide,
		BYTE bCountry,
		BYTE bAidCountry,
		BYTE bMode,
		DWORD dwRiding,
		__int64 nBanTime,
		DWORD dwSoulMate,
		DWORD dwSoulSilence,
		CString strSoulmate,
		BYTE bClass,
		CPacket *pPacket);

	void SendMW_CONRESULT_REQ(
		DWORD dwCharID,
		DWORD dwKEY,
		BYTE bResult,
		MAPTCHARCON mapTCON);

	void SendMW_CLOSECHAR_REQ(
		DWORD dwCharID,
		DWORD dwKEY);

	void SendMW_CHECKMAIN_REQ(
		DWORD dwCharID,
		DWORD dwKEY,
		BYTE bChannel,
		WORD wMapID,
		FLOAT fPosX,
		FLOAT fPosY,
		FLOAT fPosZ);

	void SendMW_DELCHAR_REQ(
		DWORD dwCharID,
		DWORD dwKEY,
		BYTE bLogout,
		BYTE bSave);

	void SendMW_CHARINFO_REQ(
		DWORD dwCharID,
		DWORD dwKEY,
		CTGuild * pGuild,
		CTGuild * pTactics,
		CTParty *pParty,
		BYTE bDuty,
		BYTE bPeer,
		WORD wCastle,
		BYTE bCamp);

	void SendMW_TELEPORT_REQ(
		DWORD dwCharID,
		DWORD dwKEY,
		BYTE bChannel,
		WORD wMapID,
		FLOAT fPosX,
		FLOAT fPosY,
		FLOAT fPosZ,
		BYTE bResult);

	void SendMW_CONLIST_REQ(
		DWORD dwCharID,
		DWORD dwKEY,
		BYTE bChannel,
		WORD wMapID,
		FLOAT fPosX,
		FLOAT fPosY,
		FLOAT fPosZ);

	void SendMW_ROUTE_REQ(
		DWORD dwCharID,
		DWORD dwKEY,
		BYTE bChannel,
		WORD wMapID,
		FLOAT fPosX,
		FLOAT fPosY,
		FLOAT fPosZ);

	void SendMW_ENTERSOLOMAP_REQ(
		DWORD dwCharID,
		DWORD dwKEY,
		WORD wPartyID,
		BYTE bPartyType,
		DWORD dwChiefID);

	void SendMW_CHGPARTYTYPE_REQ(
		DWORD dwCharID,
		DWORD dwKEY,
		BYTE bRet,
		BYTE bPartyType);

	void SendMW_PARTYADD_REQ(
		DWORD dwID,
		DWORD dwKey,
		CString strRequest,
		CString strTarget,
		BYTE bObtainType,
		BYTE bResult,
		DWORD dwRequest = 0);

	void SendMW_PARTYJOIN_REQ(
		DWORD dwID,
		DWORD dwKey,
		WORD wPartyID,
		DWORD dwChiefID,
		WORD wCommanderID,
		LPTCHARACTER pNew,
		BYTE bObtainType);

	void SendMW_PARTYDEL_REQ(
		DWORD dwID,
		DWORD dwKey,
		DWORD dwTarget,
		DWORD dwChiefID,
		WORD wCommander,
		WORD wPartyID,
		BYTE bKick);

	void SendMW_PARTYMANSTAT_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		DWORD dwID,
		BYTE bType,
		BYTE bLevel,
		DWORD dwMaxHP,
		DWORD dwCurHP,
		DWORD dwMaxMP,
		DWORD dwCurMP);

	void SendMW_LEVELUP_REQ(
		DWORD dwCharID,
		DWORD dwKEY,
		BYTE bLevel);

	void SendMW_EVENTMSGLOTTERY_REQ(
		CString strTitle,
		vector<LOTPACKET>& pLotPacket);

	////////////////////////////////////////////////////////////////
	// ±æµå
	void SendMW_GUILDESTABLISH_REQ(
		DWORD dwCharID,
		DWORD dwKEY,
		BYTE bRet,
		DWORD dwGuildID,
		CString strName,
		BYTE bEstablish);
	void SendMW_GUILDDISORGANIZATION_REQ(
		DWORD dwCharID,
		DWORD dwKEY,
		BYTE bDisorg);
	void SendMW_GUILDINVITE_REQ(
		DWORD dwCharID,
		DWORD dwKEY,
		CString strName,
		DWORD dwInviterID,
		CString strInviterName);
	void SendMW_GUILDJOIN_REQ(
		DWORD dwCharID,
		DWORD dwKEY,
		BYTE bRet,
		DWORD dwGdID,
		DWORD dwFame,
		DWORD dwFameColor,
		CString strGdName,
		DWORD dwMemberID,
		CString strMemberName,
		BYTE bMaxGuildMember = 0);
	void SendMW_GUILDLEAVE_REQ(
		DWORD dwCharID,
		DWORD dwKEY,
		CString strTarget,
		BYTE bReason,
		DWORD dwTime);
	void SendMW_GUILDDUTY_REQ(
		DWORD dwCharID,
		DWORD dwKEY,
		CString strTarget,
		BYTE bDuty);
	void SendMW_GUILDPEER_REQ(
		DWORD dwCharID,
		DWORD dwKEY,
		BYTE bResult,
		CString strTarget,
		BYTE bPeer,
		BYTE bOldPeer);
	void SendMW_GUILDMEMBERLIST_REQ(
		DWORD dwCharID,
		DWORD dwKEY,
		BYTE bRet,
		CTGuild * pGuild);
	void SendMW_GUILDINFO_REQ(
		DWORD dwCharID,
		DWORD dwKEY,
		BYTE bRet,
		LPTGUILDMEMBER pChief = NULL,
		CTGuild * pGuild = NULL,
		BYTE bDuty = 0,
		BYTE bPeer = 0);

	void SendMW_GUILDCABINETLIST_REQ(
		DWORD dwCharID, 
		DWORD dwKEY,
		CTGuild *pGuild);
	void SendMW_GUILDCONTRIBUTION_REQ(
		DWORD dwCharID, 
		DWORD dwKEY,
		BYTE bResult,
		DWORD dwExp,
		DWORD dwGold,
		DWORD dwSilver,
		DWORD dwCooper,
		DWORD dwPvPoint);
	void SendMW_GUILDARTICLELIST_REQ(
		DWORD dwCharID, 
		DWORD dwKEY,
		CTGuild *pGuild);
	void SendMW_GUILDARTICLEADD_REQ(
		DWORD dwCharID, 
		DWORD dwKEY,
		BYTE bResult);
	void SendMW_GUILDARTICLEDEL_REQ(
		DWORD dwCharID, 
		DWORD dwKEY,
		BYTE bResult);
	void SendMW_GUILDARTICLEUPDATE_REQ(
		DWORD dwCharID,
		DWORD dwKEY,
		BYTE bResult);
	void SendMW_GUILDFAME_REQ(
		DWORD dwCharID, 
		DWORD dwKEY,
		BYTE bResult,
		DWORD dwID,
		DWORD dwFame,
		DWORD dwFameColor);
	void SendMW_GUILDWANTEDADD_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		BYTE bResult);
	void SendMW_GUILDWANTEDDEL_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		BYTE bResult);
	void SendMW_GUILDWANTEDLIST_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		BYTE bCountry,
		LPTGUILDWANTEDAPP pApp,
		LPMAPTGUILDWANTED mapWanted);
	void SendMW_GUILDVOLUNTEERING_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		BYTE bResult);
	void SendMW_GUILDVOLUNTEERINGDEL_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		BYTE bResult);
	void SendMW_GUILDVOLUNTEERLIST_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		LPMAPTGUILDWANTEDAPP pApp);
	void SendMW_GUILDVOLUNTEERREPLY_REQ(
		DWORD dwCharID,
        DWORD dwKey,
		BYTE bResult);
	void SendMW_GUILDTACTICSWANTEDADD_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		BYTE bResult);
	void SendMW_GUILDTACTICSWANTEDDEL_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		BYTE bResult);
	void SendMW_GUILDTACTICSWANTEDLIST_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		BYTE bCountry,
		LPTGUILDTACTICSWANTEDAPP pApp,
		LPMAPVTGUILDTACTICSWANTED mapWanted);
	void SendMW_GUILDTACTICSVOLUNTEERING_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		BYTE bResult);
	void SendMW_GUILDTACTICSVOLUNTEERINGDEL_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		BYTE bResult);
	void SendMW_GUILDTACTICSVOLUNTEERLIST_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		LPMAPTGUILDTACTICSWANTEDAPP pApp);
	void SendMW_GUILDTACTICSREPLY_REQ(
		DWORD dwCharID,
        DWORD dwKey,
		BYTE bResult,
		DWORD dwMemberID,
		DWORD dwGuildID=0,
		CString strGuildName=NAME_NULL,
		CString strMemberName=NAME_NULL,
		DWORD dwGold=0,
		DWORD dwSilver=0,
		DWORD dwCooper=0);
	void SendMW_GUILDTACTICSKICKOUT_REQ(
		DWORD dwCharID,
        DWORD dwKey,
		BYTE bResult,
		DWORD dwTarget,
		BYTE bKick);
	void SendMW_GUILDTACTICSINVITE_REQ(
		DWORD dwCharID,
		DWORD dwKEY,
		CString strGuildName,
		CString strNAME,
		BYTE bDay,
		DWORD dwPoint,
		DWORD dwGold,
		DWORD dwSilver,
		DWORD dwCooper);
	void SendMW_GUILDTACTICSANSWER_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		BYTE bResult,
		DWORD dwGuildID,
		CString strGuildName,
		DWORD dwMemberID,
		CString strMemberName,
		DWORD dwGold,
		DWORD dwSilver,
		DWORD dwCooper);
	void SendMW_GUILDTACTICSLIST_REQ(
		DWORD dwCharID, 
		DWORD dwKEY, 
		CTGuild * pGuild);

	/////////////////////////////////////////////////////////////////////////////
	void SendMW_CHAT_REQ(
		DWORD dwCharID,
		DWORD dwKEY,
		BYTE bChannel,
		DWORD dwSenderID,
		CString strSenderName,
		BYTE bCountry,
		BYTE bAidCountry,
		BYTE bType,
		BYTE bGroup,
		DWORD dwTargetID,
		CString strTalk);
	void SendMW_TAKEMONMONEY_REQ(LPPACKETBUF pBUF);
	void SendMW_MONSTERDIE_REQ(LPPACKETBUF pBUF);
	void SendMW_ADDITEM_REQ(CPacket * pPacket);
	void SendMW_PARTYORDERTAKEITEM_REQ(
		DWORD dwCharID,
		DWORD dwKEY, 
		BYTE bServerID,
		BYTE bChannel,
		WORD wMapID,
		DWORD dwMonID,
		WORD wTempMonID,  
		LPTITEM pITEM);
	void SendMW_FRIENDLIST_REQ(LPTCHARACTER pChar);
	void SendMW_FRIENDADD_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		BYTE bResult,
		DWORD dwFriendID = 0,
		CString strName = NAME_NULL,
		BYTE bLevel = 0,
		BYTE bGroupID = 0,
		BYTE bClass = 0,
		DWORD dwRegion = 0);
	void SendMW_FRIENDASK_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		CString strName,
		DWORD dwInviter);
	void SendMW_FRIENDERASE_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		BYTE bRet,
		DWORD dwTarget);
	void SendMW_CHGPARTYCHIEF_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		BYTE bRET);
	void SendMW_CHGSQUADCHIEF_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		WORD wSquadID,
		DWORD dwChiefID);
	void SendMW_PARTYATTR_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		WORD wPartyID,
		BYTE bPartyType,
		DWORD dwPartyChiefID,
		WORD wCommander);
	void SendMW_ADDITEMRESULT_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		BYTE bChannel,
		WORD wMapID,
		DWORD dwMonID,
		BYTE bItemID,
		BYTE bResult);
	void SendMW_CHARSTATINFO_REQ(
		CPacket * pPacket);
	void SendMW_CHARSTATINFOANS_REQ(
		DWORD dwReqCharID,
		DWORD dwCharID);
	void SendMW_LOCALENABLE_REQ(
		BYTE bStatus,
		DWORD dwSecond,
		DWORD dwLocalStart,
		BYTE bCastleDay,
		DWORD dwCastleStart);
	void SendMW_CHGCORPSCOMMANDER_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		BYTE bRET);
	void SendMW_CORPSASK_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		DWORD dwInviter,
		CString strName);
	void SendMW_CORPSREPLY_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		BYTE bResult,
		CString strName);
	void SendMW_CORPSJOIN_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		WORD wCorps,
		WORD wCommander);
	void SendMW_ADDSQUAD_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		CTParty * pParty);
	void SendMW_DELSQUAD_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		WORD wSquad);
	void SendMW_ADDCORPSUNIT_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		WORD wSquadID,
		DWORD dwAddID,
		CString strNAME,
		FLOAT fSPEED,
		DWORD dwTargetID,
		DWORD dwMaxHP,
		DWORD dwHP,
		WORD wTargetX,
		WORD wTargetZ,
		WORD wMapID,
		WORD wPosX,
		WORD wPosZ,
		WORD wDIR,
		BYTE bTargetType,
		BYTE bLevel,
		BYTE bClass,
		BYTE bRaceID,
		BYTE bSexID,
		BYTE bFaceID,
		BYTE bHairID,
		BYTE bTCMD);

	void SendMW_DELCORPSUNIT_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		WORD wSquad,
		DWORD dwTargetID);

	void SendMW_CORPSCMD_REQ(
		DWORD dwMemberID,
		DWORD dwKey,
		WORD wSquadID,
		DWORD dwCharID,
		WORD wMapID,
		BYTE bCMD,
		DWORD dwTargetID,
		BYTE bTargetType,
		WORD wPosX,
		WORD wPosZ);

	void SendMW_REPORTENEMYLIST_REQ(
		DWORD dwCharID, 
		DWORD dwKey, 
		WORD wSquadID, 
		DWORD dwChar);

	void SendMW_PARTYMOVE_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		BYTE bResult);

	void SendMW_TMSRECV_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		DWORD dwTMS,
		CString strSender,
		CString strMessage);
	void SendMW_TMSINVITEASK_REQ(
		DWORD dwCharID,
		DWORD dwKEY,
		DWORD dwTargetID,
		DWORD dwTargetKEY,
		DWORD dwTMS,
		CString strMessage);
	void SendMW_TMSINVITE_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		DWORD dwInviter,
		LPTMS pTMS);
	void SendMW_TMSOUT_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		DWORD dwTMS,
		CString strTarget);
	
	void SendMW_POSTRECV_REQ(
		CPacket * pPacket);
	void SendMW_LOCALOCCUPY_REQ(
		BYTE bType,
		WORD wLocalID,
		BYTE bCountry,
		DWORD dwGuildID,
		CString strName);
	void SendMW_CASTLEOCCUPY_REQ(
		BYTE bType,
		WORD wCastleID,
		DWORD dwGuildID,
		BYTE bCountry,
		CString strGuildName);
	void SendMW_MISSIONOCCUPY_REQ(
		BYTE bType,
		WORD wLocalID,
		BYTE bCountry);
	void SendMW_CASTLEENABLE_REQ(
		BYTE bStatus,
		DWORD dwSecond);
	void SendMW_CASTLEAPPLY_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		BYTE bResult,
		WORD wCastle,
		DWORD dwTarget,
		BYTE bCamp);
	void SendMW_CASTLEJOIN_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		DWORD dwCastleGuild,
		WORD wMapID);
	void SendMW_GETBLOOD_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		DWORD dwAtkID,
		BYTE bAtkType,
		BYTE bBloodType,
		DWORD dwBlood);
	void SendMW_FRIENDGROUPMAKE_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		BYTE bRet,
		BYTE bGroup,
		CString strName);
	void SendMW_FRIENDGROUPDELETE_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		BYTE bRet,
		BYTE bGroup);
	void SendMW_FRIENDGROUPCHANGE_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		BYTE bRet,
		BYTE bGroup,
		DWORD dwFriend);
	void SendMW_FRIENDGROUPNAME_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		BYTE bRet,
		BYTE bGroup,
		CString strName);
	void SendMW_FRIENDCONNECTION_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		BYTE bRet,
		CString strName,
		DWORD dwRegion);

	void SendMW_MAGICMIRROR_REQ( CPacket *pPacket);
	void SendMW_CREATERECALLMON_REQ(
		DWORD dwCharID,
		DWORD dwKEY,
		DWORD dwMonID,
		WORD wMon,
		DWORD dwATTR,
		WORD wPetID,
		CString strName,
		DWORD dwTime,
        BYTE bLevel,
		BYTE bClass,
		BYTE bRace,
		BYTE bAction,
		BYTE bStatus,
		BYTE bMode,
		DWORD dwMaxHP,
		DWORD dwMaxMP,
		DWORD dwHP,
		DWORD dwMP,
		BYTE bHit,
		BYTE bSkillLevel,
		FLOAT fPosX,
        FLOAT fPosY,
        FLOAT fPosZ,
		WORD wDir,
		VWORD vSkill);

	void SendMW_RECALLMONDEL_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		DWORD dwMonID);

	void SendMW_FRIENDREGION_REQ(
		DWORD dwCharID,
		DWORD dwKEY,
		CString strNAME,
		DWORD dwRegion);

	void SendMW_STARTTELEPORT_REQ(
        DWORD dwCharID,
        DWORD dwKEY,
        BYTE bChannel,
        WORD wMapID,
        FLOAT fPosX,
        FLOAT fPosY,
        FLOAT fPosZ);

	void SendMW_PETRIDING_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		DWORD dwRiding);

	void SendMW_EVENTQUARTER_REQ(
		BYTE bDay,
		BYTE bHour,
		BYTE bMinute,
		BYTE bSelect,
		CString strPresent);

	void SendMW_HELMETHIDE_REQ(
		DWORD dwCharID,
		DWORD dwKEY,
		BYTE bHide);

	void SendMW_PARTYMEMBERRECALL_REQ(
		DWORD dwCharID,
		DWORD dwKEY,
		BYTE bResult,
		CString strTarget,
		BYTE bType,
		BYTE bInvenID = 0,
		BYTE bItemID = 0,
		BYTE bChannel = 0,
		WORD wMapID = 0,
		FLOAT fPosX = 0,
		FLOAT fPosY = 0,
		FLOAT fPosZ = 0);

	void SendMW_PARTYMEMBERRECALLANS_REQ(
		DWORD dwCharID,
		DWORD dwKEY,
		CString strTarget,
		BYTE bType,
		BYTE bInvenID,
		BYTE bItemID);

	void SendMW_SOULMATE_REQ(
		DWORD dwCharID,
		DWORD dwKEY,
		DWORD dwSoulmate,
		CString strSoulmate,
		DWORD dwTime);

	void SendMW_SOULMATESEARCH_REQ(
		DWORD dwCharID,
		DWORD dwKEY,
		BYTE bResult,
		DWORD dwSoulID,
		CString strSoulmate,
		DWORD dwRegion,
		BYTE bNpcInvenID,
		BYTE bNpcItemID);

	void SendMW_SOULMATEREG_REQ(
		DWORD dwCharID,
		DWORD dwKEY,
		BYTE bResult,
		BYTE bReg,
		BYTE bNpcInvenID,  
		BYTE bNpcItemID,
		DWORD dwSoulmate=0,
		CString strSoulmate=NAME_NULL,
		DWORD dwRegion=0);

	void SendMW_SOULMATEEND_REQ(
		DWORD dwCharID,
		DWORD dwKEY,
		BYTE bResult,
		DWORD dwTime=0);

	void SendMW_CASTLEWARINFO_REQ(
		LPTCASTLEWARINFO pInfo,
		DWORD dwDefGuild,
		CString dwDefName,
		BYTE bDefCountry,
		DWORD dwAtkGuild,
		CString strAtkName);
	
	void SendMW_ENDWAR_REQ(
		WORD wCastle);

	void SendMW_CHATBAN_REQ(  
		CString strName,
		__int64 nChatBanTime,
		BYTE bResult,
		DWORD dwCharID,
		DWORD dwKEY);

	void SendMW_CHARMSG_REQ( 
		CString strName,
		CString strMsg);

	void SendMW_RECALLMONDATA_REQ(CPacket * pPacket);
	void SendMW_CHANGECHARBASE_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		BYTE bType,
		BYTE bValue,
		CString strName);

	void SendMW_HEROSELECT_REQ(  
		WORD wBattleZoneID,
		CString strHeroName,
		__time64_t timeHero);

	void SendMW_RELAYCONNECT_REQ(DWORD dwCharID, BYTE bRelayOn = TRUE);

//	Relay Message Sender
	void SendRW_ENTERCHAR_ACK(
		DWORD dwCharID,
		CString strName,
		BYTE bResult,
		BYTE bCountry = TCONTRY_N,
		BYTE bAidCountry = TCONTRY_N,
		DWORD dwGuildID = 0,
		DWORD dwGuildChief = 0,
		BYTE bDuty = 0,
		WORD wPartyID = 0,
		DWORD dwPartyChiefID = 0,
		WORD wCorpsID = 0,
		DWORD dwGeneralID = 0,
		DWORD dwTacticsID = 0,
		DWORD dwTacticsChief = 0,
		WORD wMapID = 0,
		WORD wUnitID = 0);
	void SendRW_PARTYADD_ACK(
		DWORD dwCharID,
		WORD wPartyID,
		DWORD dwChiefID);
	void SendRW_PARTYDEL_ACK(
		DWORD dwCharID,
		WORD wPartyID,
		DWORD dwChiefID);
	void SendRW_PARTYCHGCHIEF_ACK(
		WORD wPartyID,
		DWORD dwChiefID);
	void SendRW_GUILDADD_ACK(
		DWORD dwCharID,
		DWORD dwGuildID,
		DWORD dwMasterID);
	void SendRW_GUILDDEL_ACK(
		DWORD dwCharID,
		DWORD dwGuildID);
	void SendRW_GUILDCHGMASTER_ACK(
		DWORD dwGuildID,
		DWORD dwMasterID);
	void SendRW_CORPSJOIN_ACK(
		WORD wPartyID,
		WORD wCorpsID,
		WORD wCommander);
	void SendRW_RELAYSVR_ACK(
		BYTE bNation,
		LPVECTORDWORD vTOPERATOR,
		LPMAPTSTRING mapTSVRMSG);
	void SendRW_CHANGENAME_ACK(
		DWORD dwCharID,
		BYTE bType,
		BYTE bValue,
		CString strName);
	void SendRW_TACTICSADD_ACK(
		DWORD dwCharID,
		DWORD dwGuildID,
		DWORD dwGuildMaster);
	void SendRW_TACTICSDEL_ACK(
		DWORD dwCharID,
		DWORD dwGuildID);
	void SendRW_CHATBAN_ACK(
		CString strName,
		__int64 nChatBanTime);
	void SendRW_CHANGEMAP_ACK(
		DWORD dwCharID,
		WORD wMap,
		WORD wUnitID);

    void SendMW_ITEMSTATE_REQ(CPacket * pPacket);
	void SendMW_GAINPVPPOINT_REQ(
		DWORD dwOwnerID,
		DWORD dwPoint,
		BYTE bEvent,
		BYTE bType,
		BYTE bGain,
		CString strName = NAME_NULL,
		BYTE bClass=0,
		BYTE bLevel=0);
	void SendMW_GUILDPOINTLOG_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		CTGuild * pGuild);
	void SendMW_GUILDPOINTREWARD_REQ(
		BYTE bResult,
		DWORD dwCharID,
		DWORD dwKey,
		DWORD dwRemainPoint,
		DWORD dwPoint = 0,
		DWORD dwTargetID = 0,
		CString strTarget = NAME_NULL,
		CString strMessage = NAME_NULL);
	void SendMW_GUILDPVPRECORD_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		CTGuild * pGuild,
		DWORD dwRecentRecordDate);
	void SendMW_MONSTERBUY_REQ(
		BYTE bRet,
		DWORD dwCharID,
		DWORD dwKey,
		DWORD dwGuildID,
		WORD wNpcID,
		WORD wID,
		DWORD dwPrice);
	void SendMW_CASTLEGUILDCHG_REQ(
		WORD wCastleID,
		DWORD dwDefGuildID,
		CString strDefGuild,
		DWORD dwAtkGuildID,
		CString strAtkGuild,
		__time64_t tTime);
	 
	void SendMW_EVENTUPDATE_REQ(BYTE bEvnetID,WORD wValue,LPEVENTINFO pEvent);
	void SendMW_EVENTMSG_REQ(BYTE bEventID,BYTE bEventMsgType,CString strMsg);
	void SendMW_CASHSHOPSTOP_REQ(BYTE bType,BYTE bSendPlayer = TRUE);
	void SendMW_CASHITEMSALE_REQ(DWORD dwIndex, WORD wValue, LPVTCASHITEMSALE pvSaleItem);
	void SendMW_WORLDPOSTSEND_REQ(
		BYTE bType,
		DWORD dwValue,
		DWORD dwSenderID,
		CString strSender,
		DWORD dwRecvID,
		CString strRecver,
		INT64 dlValue);
	void SendMW_WORLDPOSTSEND_REQ(
		BYTE	bType,
		DWORD	dwRecvID,
		CString	strRecver,
		CString	strTitle,
		CString	strMessage,
		WORD	wItemID,
		BYTE	bItemNum,
		WORD	wUseTime);
	void SendMW_CASTLEAPPLICANTCOUNT_REQ(
		WORD wCastle,
		DWORD dwGuildID,
		BYTE bCamp,
		BYTE bCount);

	
	void SendMW_MONTHRANKLIST_REQ(MONTHRANKER arMonthRank[COUNTRY_COUNT][MONTHRANKCOUNT],BYTE bRankMonth); 
	void SendMW_MONTHRANKRESET_REQ(MONTHRANKER arMonthRank[],BYTE bRankMonth);
	void SendMW_MONTHRANKUPDATE_REQ(BYTE bMonth,BYTE bCountry,BYTE bStartRank,BYTE bEndRank,MONTHRANKER arMonthRank[COUNTRY_COUNT][MONTHRANKCOUNT], BYTE bNewWarlord);
	void SendMW_FAMERANKUPDATE_REQ(LPPACKETBUF pBUF);
	void SendMW_WARLORDSAY_REQ(BYTE bType,BYTE bRankMonth,DWORD dwCharID,CString strSay);
	void SendMW_FIRSTGRADEGROUP_REQ(MONTHRANKER arMonthRank[COUNTRY_COUNT][FIRSTGRADEGROUPCOUNT],BYTE bRankMonth);
	void SendMW_TERMINATE_REQ(DWORD dwKey);
	void SendMW_TOURNAMENTENABLE_REQ(BYTE bGroup, BYTE bStep, DWORD dwPeriod, INT64 dlNextStep);
	void SendMW_TOURNAMENT_REQ(CPacket* pMSG);
	void SendMW_TOURNAMENT_REQ(DWORD dwCharID, DWORD dwKey, WORD wProtocol, BYTE bResult);
	void SendMW_TOURNAMENTINFO_REQ(BYTE bFirstGroupCount, LPMAPTOURNAMENTENTRY pTour, BYTE bGroup, BYTE bStep, BYTE bMaxLevel);
	void SendMW_TOURNAMENTMATCH_REQ(LPMAPTNMTPLAYER mapPlayer);
	void SendMW_TOURNAMENTRESULT_REQ(WORD wTournamentID, BYTE bStep, BYTE bRet, DWORD dwWin, DWORD dwLose, DWORD dwBlueHideTick, DWORD dwRedHideTick, VECTORDWORD & vPlayer);
	void SendMW_TOURNAMENTBATPOINT_REQ(DWORD dwCharID, CString strName, DWORD dwAmount);

	void SendMW_MISSIONENABLE_REQ(BYTE bStatus, DWORD dwStart, DWORD dwSecond);

	void SendMW_RPSGAME_REQ(DWORD dwCharID, DWORD dwKey, BYTE bResult, BYTE  bPlayerRPS);
	void SendMW_RPSGAMECHANGE_REQ(LPPACKETBUF pBUF);
	void SendMW_WARCOUNTRYBALANCE_REQ(DWORD dwCharID, DWORD dwKey, DWORD dwCount_D, DWORD dwCount_C, BYTE bGap);
	void SendMW_MEETINGROOM_REQ(DWORD dwCharID, DWORD dwKey, BYTE bType, BYTE bResult, CString strName);

	//Apex
	void SendMW_APEXDATA_REQ(DWORD dwCharID, DWORD dwKey, char * pBuf,int nBufLen);
public:
	CTServer();
	virtual ~CTServer();
};
