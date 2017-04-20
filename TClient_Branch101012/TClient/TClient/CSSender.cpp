#include "stdafx.h"
#include "TClient.h"
#include "Curse.h"
#include "TClientGame.h"


void CTClientSession::SendCS_DISCONNECT_REQ()
{
	CPacket vMSG;

	vMSG.SetID(CS_DISCONNECT_REQ);
	Say(&vMSG);
}

void CTClientSession::SendCS_CONNECT_REQ( WORD wVersion,
										  BYTE bChannel,
										  DWORD dwUserID,
										  DWORD dwCharID,
										  DWORD dwKEY,
										  DWORD dwIPAddr,
										  WORD wPort)
{
	CPacket vMSG;

	vMSG.SetID(CS_CONNECT_REQ)
		<< wVersion
		<< bChannel
		<< dwUserID
		<< dwCharID
		<< dwKEY
		<< dwIPAddr
		<< wPort;

	Say(&vMSG);
}

void CTClientSession::SendCS_KICKOUT_REQ( DWORD dwCharID)
{
	CPacket vMSG;

	vMSG.SetID(CS_KICKOUT_REQ)
		<< dwCharID;

	Say(&vMSG);
}

void CTClientSession::SendCS_ATTACK_REQ( DWORD dwTargetID,
										 BYTE bTargetType,
										 DWORD dwActID,
										 DWORD dwAniID)
{
	CPacket vMSG;

	vMSG.SetID(CS_ATTACK_REQ)
		<< dwTargetID
		<< bTargetType
		<< dwActID
		<< dwAniID;

	Say(&vMSG);
}

void CTClientSession::SendCS_MOVEITEM_REQ( BYTE bInvenSRC,
										   BYTE bSRCItemID,
										   BYTE bInvenDEST,
										   BYTE bDESTItemID,
										   BYTE bCount)
{
	CPacket vMSG;

	vMSG.SetID(CS_MOVEITEM_REQ)
		<< bInvenSRC
		<< bSRCItemID
		<< bInvenDEST
		<< bDESTItemID
		<< bCount;

	Say(&vMSG);
}

void CTClientSession::SendCS_CHGMODE_REQ( BYTE bMode)
{
	CPacket vMSG;

	vMSG.SetID(CS_CHGMODE_REQ)
		<< bMode;

	Say(&vMSG);
}

void CTClientSession::SendCS_CORPSASK_REQ( CString strTargetID)
{
	CPacket vMSG;

	vMSG.SetID(CS_CORPSASK_REQ)
		<< strTargetID;

	Say(&vMSG);
}

void CTClientSession::SendCS_CORPSREPLY_REQ( CString strReqNAME,
											 BYTE bReply)
{
	CPacket vMSG;

	vMSG.SetID(CS_CORPSREPLY_REQ)
		<< strReqNAME
		<< bReply;

	Say(&vMSG);
}

void CTClientSession::SendCS_CORPSLEAVE_REQ( WORD wSquadID)
{
	CPacket vMSG;

	vMSG.SetID(CS_CORPSLEAVE_REQ)
		<< wSquadID;

	Say(&vMSG);
}

void CTClientSession::SendCS_PARTYADD_REQ( CString strTargetID, BYTE bObtainType)
{
	CPacket vMSG;

	vMSG.SetID(CS_PARTYADD_REQ)
		<< strTargetID
		<< bObtainType;

	Say(&vMSG);
}

void CTClientSession::SendCS_PARTYJOIN_REQ( CString strReqNAME,
										    BYTE bObtainType,
											BYTE bReply)
{
	CPacket vMSG;

	vMSG.SetID(CS_PARTYJOIN_REQ)
		<< strReqNAME
		<< bObtainType
		<< bReply;

	Say(&vMSG);
}

void CTClientSession::SendCS_PARTYDEL_REQ( DWORD dwMemberID)
{
	CPacket vMSG;

	vMSG.SetID(CS_PARTYDEL_REQ)
		<< dwMemberID;

	Say(&vMSG);
}

void CTClientSession::SendCS_PARTYMOVE_REQ( CString strMoveNAME,
										    CString strTargetNAME,
											WORD wPartyID)
{
	CPacket vMSG;

	vMSG.SetID(CS_PARTYMOVE_REQ)
		<< strMoveNAME
		<< strTargetNAME
		<< wPartyID;

	Say(&vMSG);
}

void CTClientSession::SendCS_CHGPARTYTYPE_REQ( BYTE bPartyItemType )
{
	CPacket vMSG;

	vMSG.SetID(CS_CHGPARTYTYPE_REQ)
		<< bPartyItemType;

	Say(&vMSG);
}

void CTClientSession::SendCS_NPCITEMLIST_REQ( WORD wNpcID)
{
	CPacket vMSG;

	vMSG.SetID(CS_NPCITEMLIST_REQ)
		<< wNpcID;

	Say(&vMSG);
}

void CTClientSession::SendCS_ITEMBUY_REQ( WORD wNpcID,
										 DWORD dwQuestID,
										 WORD wItemID,
										 BYTE bCount,
										 BYTE bInvenIDofOpenCash,
										 BYTE bSlotIDofOpenCash )
{
	CPacket vMSG;

	vMSG.SetID(CS_ITEMBUY_REQ)
		<< wNpcID
		<< dwQuestID
		<< wItemID
		<< bCount
		<< bInvenIDofOpenCash
		<< bSlotIDofOpenCash;

	Say(&vMSG);
}

void CTClientSession::SendCS_MONSTERBUY_REQ( WORD wNpcID,
											WORD wItemID )
{
	CPacket vMSG;

	vMSG.SetID(CS_MONSTERBUY_REQ)
		<< wNpcID
		<< wItemID;

	Say(&vMSG);
}

void CTClientSession::SendCS_ITEMSELL_REQ( BYTE bInvenID,
										   BYTE bSlotID,
										   BYTE bCount,
										   BYTE bInvenIDofOpenCash,
										   BYTE bSlotIDofOpenCash )
{
	CPacket vMSG;

	vMSG.SetID(CS_ITEMSELL_REQ)
		<< bInvenID
		<< bSlotID
		<< bCount
		<< bInvenIDofOpenCash
		<< bSlotIDofOpenCash;

	Say(&vMSG);
}

void CTClientSession::SendCS_HOTKEYADD_REQ( BYTE bType,
										    WORD wID,
											BYTE bSetID,
											BYTE bSlotID)
{
	CPacket vMSG;

	vMSG.SetID(CS_HOTKEYADD_REQ)
		<< bType
		<< wID
		<< bSetID
		<< bSlotID;

	Say(&vMSG);
}

void CTClientSession::SendCS_HOTKEYDEL_REQ( BYTE bSetID,
										    BYTE bSlotID)
{
	CPacket vMSG;

	vMSG.SetID(CS_HOTKEYDEL_REQ)
		<< bSetID
		<< bSlotID;

	Say(&vMSG);
}

void CTClientSession::SendCS_QUESTEXEC_REQ( DWORD dwQuestID,
										    BYTE bRewardType,
											DWORD dwRewardID)
{
	CPacket vMSG;

	vMSG.SetID(CS_QUESTEXEC_REQ)
		<< dwQuestID
		<< bRewardType
		<< dwRewardID;

	Say(&vMSG);
}

void CTClientSession::SendCS_QUESTDROP_REQ( DWORD dwQuestID)
{
	CPacket vMSG;

	vMSG.SetID(CS_QUESTDROP_REQ)
		<< dwQuestID;

	Say(&vMSG);
}

void CTClientSession::SendCS_NPCTALK_REQ( WORD wNpcID)
{
	CPacket vMSG;

	vMSG.SetID(CS_NPCTALK_REQ)
		<< wNpcID;

	Say(&vMSG);
}

void CTClientSession::SendCS_CHGMODERECALLMON_REQ( DWORD dwMonID, BYTE bMode)
{
	CPacket vMSG;

	vMSG.SetID(CS_CHGMODERECALLMON_REQ)
		<< dwMonID
		<< bMode;

	Say(&vMSG);
}

void CTClientSession::SendCS_DELRECALLMON_REQ( DWORD dwMonID, BYTE bMonType)
{
	CPacket vMSG;

	vMSG.SetID(CS_DELRECALLMON_REQ)
		<< dwMonID
		<< bMonType;

	Say(&vMSG);
}

void CTClientSession::SendCS_INVENADD_REQ( BYTE bSlotID,
										   BYTE bInvenID,
										   BYTE bItemID)
{
	CPacket vMSG;

	vMSG.SetID(CS_INVENADD_REQ)
		<< bSlotID
		<< bInvenID
		<< bItemID;

	Say(&vMSG);
}

void CTClientSession::SendCS_INVENDEL_REQ( BYTE bSlotID,
										   BYTE bInvenID,
										   BYTE bPosID)
{
	CPacket vMSG;

	vMSG.SetID(CS_INVENDEL_REQ)
		<< bSlotID
		<< bInvenID
		<< bPosID;

	Say(&vMSG);
}

void CTClientSession::SendCS_INVENMOVE_REQ( BYTE bSRCSlotID,
										    BYTE bDESTSlotID)
{
	CPacket vMSG;

	vMSG.SetID(CS_INVENMOVE_REQ)
		<< bSRCSlotID
		<< bDESTSlotID;

	Say(&vMSG);
}

void CTClientSession::SendCS_HELMETHIDE_REQ( BYTE bHide)
{
	CPacket vMSG;

	vMSG.SetID(CS_HELMETHIDE_REQ)
		<< bHide;

	Say(&vMSG);
}

void CTClientSession::SendCS_TELEPORT_REQ( WORD wPotalID, WORD wNpcID)
{
	CPacket vMSG;

	vMSG.SetID(CS_TELEPORT_REQ)
		<< wNpcID
		<< wPotalID;

	Say(&vMSG);
}

void CTClientSession::SendCS_SKILLBUY_REQ( WORD wNpcID,
										   WORD wSkillID )
{
	CPacket vMSG;

	vMSG.SetID(CS_SKILLBUY_REQ)
		<< wNpcID
		<< wSkillID;

	Say(&vMSG);
}

void CTClientSession::SendCS_CANCELSKILL_REQ( BYTE bType, DWORD dwID, WORD wSkillID)
{
	CPacket vMSG;

	vMSG.SetID(CS_CANCELSKILL_REQ)
		<< bType
		<< dwID
		<< wSkillID;

	Say(&vMSG);
}

void CTClientSession::SendCS_SKILLINIT_REQ( WORD wSkillID,
											BYTE bInvenID,
											BYTE bSlotID)
{
	CPacket vMSG;

	vMSG.SetID(CS_SKILLINIT_REQ)
		<< wSkillID
		<< bInvenID
		<< bSlotID;

	Say(&vMSG);
}

void CTClientSession::SendCS_SKILLINITPOSSIBLE_REQ( BYTE bInvenID,
													BYTE bSlotID)
{
	CPacket vMSG;

	vMSG.SetID(CS_SKILLINITPOSSIBLE_REQ)
		<< bInvenID
		<< bSlotID;

	Say(&vMSG);
}

void CTClientSession::SendCS_SWITCHCHANGE_REQ( DWORD dwSwitchID)
{
	CPacket vMSG;

	vMSG.SetID(CS_SWITCHCHANGE_REQ)
		<< dwSwitchID;

	Say(&vMSG);
}

void CTClientSession::SendCS_CHGCORPSCOMMANDER_REQ( WORD wSquadID)
{
	CPacket vMSG;

	vMSG.SetID(CS_CHGCORPSCOMMANDER_REQ)
		<< wSquadID;

	Say(&vMSG);
}

void CTClientSession::SendCS_CHGPARTYCHIEF_REQ( DWORD dwTargetID)
{
	CPacket vMSG;

	vMSG.SetID(CS_CHGPARTYCHIEF_REQ)
		<< dwTargetID;

	Say(&vMSG);
}

void CTClientSession::SendCS_SETRETURNPOS_REQ( WORD wNpcID)
{
	CPacket vMSG;

	vMSG.SetID(CS_SETRETURNPOS_REQ)
		<< wNpcID;

	Say(&vMSG);
}

void CTClientSession::SendCS_DEALITEMASK_REQ( CString strTARGET)
{
	CPacket vMSG;

	vMSG.SetID(CS_DEALITEMASK_REQ)
		<< strTARGET;

	Say(&vMSG);
}

void CTClientSession::SendCS_DEALITEMRLY_REQ( BYTE bReply,
											  CString strReqNAME)
{
	CPacket vMSG;

	vMSG.SetID(CS_DEALITEMRLY_REQ)
		<< bReply
		<< strReqNAME;

	Say(&vMSG);
}

void CTClientSession::SendCS_DEALITEM_REQ( BYTE bOkey)
{
	CPacket vMSG;

	vMSG.SetID(CS_DEALITEM_REQ)
		<< bOkey;

	Say(&vMSG);
}

void CTClientSession::SendCS_ITEMUPGRADE_REQ( BYTE bUpInvenID,
										   BYTE bUpSlotID,
										   BYTE bMatInvenID,
										   BYTE bMatSlotID,
										   WORD wNpcID,
										   BYTE bInvenIDofOpenCash,
										   BYTE bSlotIDofOpenCash,
										   WORD wColor )
{
	CPacket vMSG;

	vMSG.SetID(CS_ITEMUPGRADE_REQ)
		<< bUpInvenID
		<< bUpSlotID
		<< bMatInvenID
		<< bMatSlotID
		<< wNpcID
		<< bInvenIDofOpenCash
		<< bSlotIDofOpenCash
		<< wColor;

	Say(&vMSG);
}

void CTClientSession::SendCS_STOREOPEN_REQ(
	CString strName,
	BYTE bCount,
	LPPRVSHOP_ITEM pItem)
{
	CPacket vMSG;

	vMSG.SetID(CS_STOREOPEN_REQ)
		<< strName
		<< bCount;
		
	for(INT i=0; i<bCount; ++i)
	{
		vMSG << pItem[i].dwRune
			 << pItem[i].dwLuna
			 << pItem[i].dwCron
			 << pItem[i].bInvenID
			 << pItem[i].bItemID
			 << pItem[i].bItemCount;
	}

	Say(&vMSG);
}

void CTClientSession::SendCS_STORECLOSE_REQ()
{
	CPacket vMSG;

	vMSG.SetID(CS_STORECLOSE_REQ);

	Say(&vMSG);
}

void CTClientSession::SendCS_STOREITEMLIST_REQ(
	CString strTarget )
{
	CPacket vMSG;

	vMSG.SetID(CS_STOREITEMLIST_REQ)
		<< strTarget;

	Say(&vMSG);
}

void CTClientSession::SendCS_STOREITEMBUY_REQ(
	CString strTarget,
	BYTE bItem,
	BYTE bItemCount)
{
	CPacket vMSG;

	vMSG.SetID(CS_STOREITEMBUY_REQ)
		<< strTarget
		<< bItem
		<< bItemCount;

	Say(&vMSG);
}



void CTClientSession::SendCS_FRIENDASK_REQ(CString strInvitee)
{
	CPacket vMSG;

	vMSG.SetID(CS_FRIENDASK_REQ)
		<< strInvitee;

	Say(&vMSG);
}

void CTClientSession::SendCS_FRIENDREPLY_REQ(CString strInviter,BYTE bReply)
{
	CPacket vMSG;

	vMSG.SetID(CS_FRIENDREPLY_REQ)
		<< strInviter
		<< bReply;

	Say(&vMSG);
}

void CTClientSession::SendCS_FRIENDERASE_REQ(DWORD dwFriendID)
{
	CPacket vMSG;

	vMSG.SetID(CS_FRIENDERASE_REQ)
		<< dwFriendID;

	Say(&vMSG);
}

void CTClientSession::SendCS_FRIENDGROUPMAKE_REQ(BYTE bGroup, CString strName)
{
	CPacket vMSG;

	if( strName.GetLength() > TMAX_FRIENDGROUP_NAME )
		strName = strName.Left(TMAX_FRIENDGROUP_NAME);

	vMSG.SetID(CS_FRIENDGROUPMAKE_REQ)
		<< bGroup
		<< strName;

	Say(&vMSG);
}

void CTClientSession::SendCS_FRIENDGROUPDELETE_REQ(BYTE bGroupID)
{
	CPacket vMSG;

	vMSG.SetID(CS_FRIENDGROUPDELETE_REQ)
		<< bGroupID;

	Say(&vMSG);
}

void CTClientSession::SendCS_FRIENDGROUPCHANGE_REQ(DWORD dwFriend, BYTE bWantedGroup)
{
	CPacket vMSG;

	vMSG.SetID(CS_FRIENDGROUPCHANGE_REQ)
		<< dwFriend
		<< bWantedGroup;

	Say(&vMSG);
}

void CTClientSession::SendCS_FRIENDGROUPNAME_REQ(BYTE bGroup, CString strName)
{
	CPacket vMSG;

	if( strName.GetLength() > TMAX_FRIENDGROUP_NAME )
		strName = strName.Left(TMAX_FRIENDGROUP_NAME);

	vMSG.SetID(CS_FRIENDGROUPNAME_REQ)
		<< bGroup
		<< strName;

	Say(&vMSG);
}

void CTClientSession::SendCS_TMSINVITE_REQ(DWORD dwTMS, const VECTORDWORD& vTARGET)
{
	BYTE bCount = (BYTE)vTARGET.size();

	CPacket vMSG;

	vMSG.SetID(CS_TMSINVITE_REQ)
		<< dwTMS
		<< bCount;

	for(BYTE i=0; i<bCount; ++i)
		vMSG << vTARGET[i];

	Say(&vMSG);
}

void CTClientSession::SendCS_TMSSEND_REQ(DWORD dwTMS, CString strMessage)
{
//	strMessage = CCurse::ConvertString( strMessage, TRUE);
	if( !CTStrFilter::ConvertString( strMessage, true ) )
		return ;

	CPacket vMSG;

	vMSG.SetID(CS_TMSSEND_REQ)
		<< dwTMS
		<< strMessage;

	Say(&vMSG);
}

void CTClientSession::SendCS_TMSOUT_REQ(DWORD dwTMS)
{
	CPacket vMSG;

	vMSG.SetID(CS_TMSOUT_REQ)
		<< dwTMS;

	Say(&vMSG);
}

void CTClientSession::SendCS_PROTECTEDADD_REQ(
	CString strProtected)
{
	CPacket vMSG;

	vMSG.SetID(CS_PROTECTEDADD_REQ)
		<< strProtected;

	Say(&vMSG);
}

void CTClientSession::SendCS_PROTECTEDOPTION_REQ(
	DWORD dwCharID,
	BYTE bOption )
{
	CPacket vMSG;

	vMSG.SetID( CS_PROTECTEDOPTION_REQ )
		<< dwCharID
		<< bOption;

	Say(&vMSG);
}

void CTClientSession::SendCS_PROTECTEDERASE_REQ(
	CString strProtected)
{
	CPacket vMSG;

	vMSG.SetID(CS_PROTECTEDERASE_REQ)
		<< strProtected;

	Say(&vMSG);
}

void CTClientSession::SendCS_POSTSEND_REQ( CString strTarget,
										   CString strTitle,
										   CString strMessage,
										   BYTE bType,
										   DWORD dwGold,
										   DWORD dwSilver,
										   DWORD dwCooper,
										   BYTE bInvenID,
										   BYTE bSlotID)
{
	CPacket vMSG;

	if(strTitle.IsEmpty())
	{
		strTitle = CTChart::LoadString( TSTR_FMT_NOTITLE);
		strTitle = CTTextLinker::GetInstance()->BuildNetString( _T(""), strTitle);
	}

//	strMessage = CCurse::ConvertString( strMessage, TRUE);
//	strTitle = CCurse::ConvertString( strTitle, TRUE);
	if( !CTStrFilter::ConvertString( strMessage, true ) ||
		!CTStrFilter::ConvertString( strTitle, true ) )
		return ;

	vMSG.SetID(CS_POSTSEND_REQ)
		<< strTarget
		<< strTitle
		<< strMessage
		<< bType
		<< dwGold
		<< dwSilver
		<< dwCooper
		<< bInvenID
		<< bSlotID;

	Say(&vMSG);
}

void CTClientSession::SendCS_POSTVIEW_REQ(DWORD dwPostID)
{
	CPacket vMSG;

	vMSG.SetID(CS_POSTVIEW_REQ)
		<< dwPostID;

	Say(&vMSG);
}

void CTClientSession::SendCS_POSTDEL_REQ(DWORD dwPostID)
{
	CPacket vMSG;

	vMSG.SetID(CS_POSTDEL_REQ)
		<< dwPostID;

	Say(&vMSG);
}

void CTClientSession::SendCS_POSTGETITEM_REQ(DWORD dwPostID)
{
	CPacket vMSG;

	vMSG.SetID(CS_POSTGETITEM_REQ)
		<< dwPostID;

	Say(&vMSG);
}

void CTClientSession::SendCS_POSTRETURN_REQ(DWORD dwPostID, BYTE bReturnType)
{
	CPacket vMSG;

	vMSG.SetID(CS_POSTRETURN_REQ)
		<< dwPostID
		<< bReturnType;

	Say(&vMSG);
}

void CTClientSession::SendCS_PETMAKE_REQ(BYTE bInven, BYTE bSlot, CString szName)
{
	CPacket vMSG;

	vMSG.SetID(CS_PETMAKE_REQ)
		<< bInven
		<< bSlot
		<< szName;

	Say(&vMSG);
}

void CTClientSession::SendCS_PETDEL_REQ(WORD wPetID)
{
	CPacket vMSG;

	vMSG.SetID(CS_PETDEL_REQ)
		<< wPetID;

	Say(&vMSG);
}

void CTClientSession::SendCS_PETRECALL_REQ(WORD wPetID)
{
	CPacket vMSG;

	vMSG.SetID(CS_PETRECALL_REQ)
		<< wPetID;

	Say(&vMSG);
}

void CTClientSession::SendCS_PETRIDING_REQ(DWORD dwMonID, BOOL bRide)
{
	BYTE bAction = bRide? PETACTION_RIDING: PETACTION_DISMOUNT;

	CPacket vMSG;

	vMSG.SetID(CS_PETRIDING_REQ)
		<< dwMonID
		<< bAction;

	Say(&vMSG);
}

void CTClientSession::SendCS_OTHERSELF_REQ(DWORD dwHostID, DWORD dwOtherSelfMonID)
{
	CPacket vMSG;

	vMSG.SetID(CS_OTHERSELF_REQ)
		<< dwHostID
		<< dwOtherSelfMonID;

	Say(&vMSG);
}

void CTClientSession::SendCS_CHGCHANNEL_REQ(BYTE bChannel)
{
	CPacket vMSG;

	vMSG.SetID(CS_CHGCHANNEL_REQ)
		<< bChannel;

	Say(&vMSG);
}

void CTClientSession::SendCS_CABINETPUTIN_REQ(	BYTE bCabinetID,
												BYTE bInven,
												BYTE bItemID,
												BYTE bCount,
												BYTE bInvenIDofOpenCash,
												BYTE bSlotIDofOpenCash )
{
	CPacket vMSG;

	vMSG.SetID(CS_CABINETPUTIN_REQ)
		<< bCabinetID
		<< bInven
		<< bItemID
		<< bCount
		<< bInvenIDofOpenCash
		<< bSlotIDofOpenCash;

	Say(&vMSG);
}

void CTClientSession::SendCS_CABINETTAKEOUT_REQ( BYTE bCabinetID,
												 DWORD dwStItemID,
												 BYTE bCount,
												 BYTE bInvenID,
												 BYTE bItemID,
												 BYTE bInvenIDofOpenCash,
												 BYTE bSlotIDofOpenCash )
{
	CPacket vMSG;

	vMSG.SetID(CS_CABINETTAKEOUT_REQ)
		<< bCabinetID
		<< dwStItemID
		<< bCount
		<< bInvenID
		<< bItemID
		<< bInvenIDofOpenCash
		<< bSlotIDofOpenCash;

	Say(&vMSG);
}

void CTClientSession::SendCS_CABINETLIST_REQ()
{
	CPacket vMSG;

	vMSG.SetID(CS_CABINETLIST_REQ);

	Say(&vMSG);
}

void CTClientSession::SendCS_CABINETITEMLIST_REQ(BYTE bCabinetID)
{
	CPacket vMSG;

	vMSG.SetID(CS_CABINETITEMLIST_REQ)
		<< bCabinetID;

	Say(&vMSG);
}

void CTClientSession::SendCS_CABINETOPEN_REQ(BYTE bCabinetID)
{
	CPacket vMSG;

	vMSG.SetID(CS_CABINETOPEN_REQ)
		<< bCabinetID;

	Say(&vMSG);
}

void CTClientSession::SendCS_DUELINVITE_REQ(DWORD dwTarget)
{
	CPacket vMSG;

	vMSG.SetID(CS_DUELINVITE_REQ)
		<< dwTarget;

	Say(&vMSG);
}

void CTClientSession::SendCS_DUELINVITEREPLY_REQ(BYTE bResult, DWORD dwInviter)
{
	CPacket vMSG;

	vMSG.SetID(CS_DUELINVITEREPLY_REQ)
		<< bResult
		<< dwInviter;

	Say(&vMSG);
}

void CTClientSession::SendCS_DUELEND_REQ()
{
	CPacket vMSG;

	vMSG.SetID(CS_DUELEND_REQ);

	Say(&vMSG);
}

void CTClientSession::SendCS_GETTARGET_REQ( DWORD dwCharID)
{
	CPacket vMSG;

	vMSG.SetID(CS_GETTARGET_REQ)
		<< dwCharID;

	Say(&vMSG);
}

void CTClientSession::SendCS_GETTARGETANS_REQ( DWORD dwAnswerCharID, 
											   DWORD dwTargetID, 
											   BYTE bTargetType)
{
	CPacket vMSG;

	vMSG.SetID(CS_GETTARGETANS_REQ)
		<< dwAnswerCharID
		<< dwTargetID
		<< bTargetType;

	Say(&vMSG);
}

void CTClientSession::SendCS_SMSSEND_REQ( BYTE bInvenID,
										  BYTE bItemID,
										  BYTE bType,
										  CString strTargetName,
										  CString strMessage)
{
	CPacket vMSG;

	vMSG.SetID(CS_SMSSEND_REQ)
		<< bInvenID
		<< bItemID
		<< bType
		<< strTargetName
		<< strMessage;

	Say(&vMSG);
}


void CTClientSession::SendCS_GUILDESTABLISH_REQ(
		CString strName)
{
	CPacket vMSG;

	vMSG.SetID(CS_GUILDESTABLISH_REQ)
		<< strName;

	Say(&vMSG);
}

void CTClientSession::SendCS_GUILDDISORGANIZATION_REQ(BOOL bCancel)
{
	CPacket vMSG;

	vMSG.SetID(CS_GUILDDISORGANIZATION_REQ)
		<< !bCancel;

	Say(&vMSG);
}

void CTClientSession::SendCS_GUILDINVITE_REQ(
	CString strTargetChar)
{
	CPacket vMSG;

	vMSG.SetID(CS_GUILDINVITE_REQ)
		<< strTargetChar;

	Say(&vMSG);
}

void CTClientSession::SendCS_GUILDINVITEANSWER_REQ(
	BYTE bAnswer,
	DWORD dwInviterID)
{
	CPacket vMSG;

	vMSG.SetID(CS_GUILDINVITEANSWER_REQ)
		<< bAnswer
		<< dwInviterID;

	Say(&vMSG);
}

void CTClientSession::SendCS_GUILDLEAVE_REQ()
{
	CPacket vMSG;

	vMSG.SetID(CS_GUILDLEAVE_REQ);

	Say(&vMSG);
}

void CTClientSession::SendCS_GUILDKICKOUT_REQ(
	CString strTargetChar )
{
	CPacket vMSG;

	vMSG.SetID(CS_GUILDKICKOUT_REQ)
		<< strTargetChar;

	Say(&vMSG);
}

void CTClientSession::SendCS_GUILDDUTY_REQ(
	CString strTargetChar,
	BYTE bDuty )
{
	CPacket vMSG;

	vMSG.SetID(CS_GUILDDUTY_REQ)
		<< strTargetChar
		<< bDuty;

	Say(&vMSG);
}

void CTClientSession::SendCS_GUILDMEMBERLIST_REQ()
{
	CPacket vMSG;

	vMSG.SetID(CS_GUILDMEMBERLIST_REQ);

	Say(&vMSG);
}

void CTClientSession::SendCS_GUILDPEER_REQ(
	CString strTargetChar,
	BYTE bPeer )
{
	CPacket vMSG;

	vMSG.SetID(CS_GUILDPEER_REQ)
		<< strTargetChar
		<< bPeer;

	Say(&vMSG);
}

void CTClientSession::SendCS_GUILDINFO_REQ()
{
	CPacket vMSG;

	vMSG.SetID(CS_GUILDINFO_REQ);

	Say(&vMSG);
}

void CTClientSession::SendCS_GUILDLOCALLIST_REQ()
{
	CPacket vMSG;

	vMSG.SetID(CS_GUILDLOCALLIST_REQ);

	Say(&vMSG);
}

void CTClientSession::SendCS_GUILDLOCALRETURN_REQ(WORD wID)
{
	CPacket vMSG;

	vMSG.SetID(CS_GUILDLOCALRETURN_REQ)
		<< wID;

	Say(&vMSG);
}

void CTClientSession::SendCS_GUILDCABINETLIST_REQ()
{
	CPacket vMSG;

	vMSG.SetID(CS_GUILDCABINETLIST_REQ);

	Say(&vMSG);
}

void CTClientSession::SendCS_GUILDCABINETPUTIN_REQ(	BYTE bInvenID,	
													BYTE bItemID,
													BYTE bCount)
{
	CPacket vMSG;

	vMSG.SetID(CS_GUILDCABINETPUTIN_REQ)
		<< bInvenID
		<< bItemID
		<< bCount;

	Say(&vMSG);
}

void CTClientSession::SendCS_GUILDCABINETTAKEOUT_REQ(DWORD dwItemID, 
													 BYTE bCount,
													 BYTE bInvenID,
													 BYTE bItemID)
{
	CPacket vMSG;

	vMSG.SetID(CS_GUILDCABINETTAKEOUT_REQ)
		<< dwItemID
		<< bCount
		<< bInvenID
		<< bItemID;

	Say(&vMSG);
}

void CTClientSession::SendCS_GUILDCONTRIBUTION_REQ(	DWORD dwExp,	
													DWORD dwGold,	
													DWORD dwSilver,
													DWORD dwCooper)
{
	CPacket vMSG;

	vMSG.SetID(CS_GUILDCONTRIBUTION_REQ)
		<< dwExp
		<< dwGold
		<< dwSilver
		<< dwCooper;

	Say(&vMSG);
}

void CTClientSession::SendCS_GUILDARTICLELIST_REQ()
{
	CPacket vMSG;

	vMSG.SetID(CS_GUILDARTICLELIST_REQ);

	Say(&vMSG);
}

void CTClientSession::SendCS_GUILDARTICLEADD_REQ(CString strTitle,
												 CString strArticle)
{
	CPacket vMSG;

	if(strTitle.IsEmpty())
	{
		strTitle = CTChart::LoadString( TSTR_FMT_NOTITLE);
		strTitle = CTTextLinker::GetInstance()->BuildNetString( _T(""), strTitle);
	}

//	strArticle = CCurse::ConvertString( strArticle, TRUE);
//	strTitle = CCurse::ConvertString( strTitle, TRUE);
	if( !CTStrFilter::ConvertString( strArticle, true ) ||
		!CTStrFilter::ConvertString( strTitle, true ) )
		return ;

	vMSG.SetID(CS_GUILDARTICLEADD_REQ)
		<< strTitle
		<< strArticle;

	Say(&vMSG);
}

void CTClientSession::SendCS_GUILDARTICLEDEL_REQ(DWORD dwID)
{
	CPacket vMSG;

	vMSG.SetID(CS_GUILDARTICLEDEL_REQ)
		<< dwID;

	Say(&vMSG);
}

void CTClientSession::SendCS_GUILDFAME_REQ(DWORD dwFame,
										   DWORD dwFameColor)
{
	CPacket vMSG;

	vMSG.SetID(CS_GUILDFAME_REQ)
		<< dwFame
		<< dwFameColor;

	Say(&vMSG);
}

void CTClientSession::SendCS_GUILDWANTEDLIST_REQ()
{
	CPacket vMSG;

	vMSG.SetID(CS_GUILDWANTEDLIST_REQ);

	Say(&vMSG);
}

void CTClientSession::SendCS_GUILDWANTEDADD_REQ(
	DWORD dwID,
	CString strTitle,
	CString strText,
	BYTE bMinLevel,
	BYTE bMaxLevel)
{
	CPacket vMSG;

	if(strTitle.IsEmpty())
		strTitle = CTTextLinker::GetInstance()->BuildNetString( _T(""), strTitle);

	if(strText.IsEmpty())
		strText = CTTextLinker::GetInstance()->BuildNetString( _T(""), strText);

//	strTitle = CCurse::ConvertString( strTitle, TRUE);
//	strText = CCurse::ConvertString( strText, TRUE);
	if( !CTStrFilter::ConvertString( strTitle, true ) || 
		!CTStrFilter::ConvertString( strText, true ) )
		return ;

	vMSG.SetID(CS_GUILDWANTEDADD_REQ)
		<< dwID
		<< strTitle
		<< strText
		<< bMinLevel
		<< bMaxLevel;

	Say(&vMSG);
}

void CTClientSession::SendCS_GUILDWANTEDDEL_REQ(DWORD dwID)
{
	CPacket vMSG;

	vMSG.SetID(CS_GUILDWANTEDDEL_REQ)
		<< dwID;

	Say(&vMSG);
}

void CTClientSession::SendCS_GUILDVOLUNTEERLIST_REQ()
{
	CPacket vMSG;

	vMSG.SetID(CS_GUILDVOLUNTEERLIST_REQ);

	Say(&vMSG);
}

void CTClientSession::SendCS_GUILDVOLUNTEERING_REQ(DWORD dwID)
{
	CPacket vMSG;

	vMSG.SetID(CS_GUILDVOLUNTEERING_REQ)
		<< dwID;

	Say(&vMSG);
}

void CTClientSession::SendCS_GUILDVOLUNTEERINGDEL_REQ()
{
	CPacket vMSG;

	vMSG.SetID(CS_GUILDVOLUNTEERINGDEL_REQ);

	Say(&vMSG);
}

void CTClientSession::SendCS_GUILDTACTICSLIST_REQ()
{
	CPacket vMSG;

	vMSG.SetID(CS_GUILDTACTICSLIST_REQ);

	Say(&vMSG);
}

void CTClientSession::SendCS_GUILDTACTICSINVITE_REQ(
	CString strName,
	BYTE bDay,
	DWORD dwPoint,
	DWORD dwGold,
	DWORD dwSilver,
	DWORD dwCooper )
{
	CPacket vMSG;

	vMSG.SetID(CS_GUILDTACTICSINVITE_REQ)
		<< strName
		<< bDay
		<< dwPoint
		<< dwGold
		<< dwSilver
		<< dwCooper;

	Say(&vMSG);
}

void CTClientSession::SendCS_GUILDTACTICSANSWER_REQ(
	BYTE bAnswer,
	CString strInviter,
	BYTE bDay,
	DWORD dwPoint,
	DWORD dwGold,
	DWORD dwSilver,
	DWORD dwCooper )
{
	CPacket vMSG;

	vMSG.SetID(CS_GUILDTACTICSANSWER_REQ)
		<< bAnswer
		<< strInviter
		<< bDay
		<< dwPoint
		<< dwGold
		<< dwSilver
		<< dwCooper;

	Say(&vMSG);
}

void CTClientSession::SendCS_GUILDARTICLEUPDATE_REQ( DWORD dwID,
													 CString strTitle,
													 CString strArticle)
{
	CPacket vMSG;

	if(strTitle.IsEmpty())
	{
		strTitle = CTChart::LoadString( TSTR_FMT_NOTITLE);
		strTitle = CTTextLinker::GetInstance()->BuildNetString( _T(""), strTitle);
	}

//	strArticle = CCurse::ConvertString( strArticle, TRUE);
//	strTitle = CCurse::ConvertString( strTitle, TRUE);
	if( !CTStrFilter::ConvertString( strArticle, true ) ||
		CTStrFilter::ConvertString( strTitle, true ) )
		return ;

	vMSG.SetID(CS_GUILDARTICLEUPDATE_REQ)
		<< dwID
		<< strTitle
		<< strArticle;

	Say(&vMSG);
}

void CTClientSession::SendCS_CASTLEAPPLY_REQ(
	WORD wCastleID,
	DWORD dwTarget)
{
	CPacket vMSG;

	vMSG.SetID(CS_CASTLEAPPLY_REQ)
		<< wCastleID
		<< dwTarget;

	Say(&vMSG);
}

void CTClientSession::SendCS_PARTYMEMBERRECALL_REQ( BYTE bInvenID,
													BYTE bItemID,
													CString strTarget)
{
	CPacket vMSG;

	vMSG.SetID(CS_PARTYMEMBERRECALL_REQ)
		<< bInvenID
		<< bItemID
		<< strTarget;

	Say(&vMSG);
}

void CTClientSession::SendCS_PARTYMEMBERRECALLANS_REQ(	BYTE bAnswer,
														CString strInviter,
														BYTE bInvenID,
														BYTE bItemID )
{
	CPacket vMSG;

	vMSG.SetID(CS_PARTYMEMBERRECALLANS_REQ)
		<< bAnswer
		<< strInviter
		<< bInvenID
		<< bItemID;

	Say(&vMSG);
}

void CTClientSession::SendCS_CASHITEMCABINET_REQ()
{
	CPacket vMSG;

	vMSG.SetID(CS_CASHITEMCABINET_REQ);

	Say(&vMSG);
}

void CTClientSession::SendCS_CASHITEMGET_REQ(DWORD dwID,
											 BYTE bInvenID,
											 BYTE bItemID)
{
	CPacket vMSG;

	vMSG.SetID(CS_CASHITEMGET_REQ)
		<< dwID
		<< bInvenID
		<< bItemID;

	Say(&vMSG);
}

void CTClientSession::SendCS_CASHSHOPITEMLIST_REQ(
	WORD wItemID,
	BYTE bInvenID,
	BYTE bSlotID)
{
	CPacket vMSG;

	vMSG.SetID(CS_CASHSHOPITEMLIST_REQ);
	vMSG << wItemID;

	Say(&vMSG);

	if( wItemID != 0 )
	{
		CTNewCashShopDlg::m_ExtendPeriodInfo.wItemID = wItemID;
		CTNewCashShopDlg::m_ExtendPeriodInfo.bInvenID = bInvenID;
		CTNewCashShopDlg::m_ExtendPeriodInfo.bSlotID = bSlotID;
	}
	else
		ZeroMemory( &CTNewCashShopDlg::m_ExtendPeriodInfo, sizeof(CASHEXTENDPERIODINFO));
}

void CTClientSession::SendCS_CASHITEMBUY_REQ(WORD wCashItemID)
{
	CPacket vMSG;

	vMSG.SetID(CS_CASHITEMBUY_REQ)
		<< wCashItemID;

	Say(&vMSG);
}

void CTClientSession::SendCS_CASHITEMPRESENT_REQ( CString strTarget,
												  WORD wCashItemID)
{
	CPacket vMSG;

	vMSG.SetID(CS_CASHITEMPRESENT_REQ)
		<< strTarget
		<< wCashItemID;

	Say(&vMSG);
}


void CTClientSession::SendCS_SOULMATESEARCH_REQ(
	BYTE bInvenIDofOpenCash,
	BYTE bSlotIDofOpenCash )
{
	CPacket vMSG;

	vMSG.SetID(CS_SOULMATESEARCH_REQ)
		<< bInvenIDofOpenCash
		<< bSlotIDofOpenCash;

	Say(&vMSG);
}

void CTClientSession::SendCS_SOULMATEREGREADY_REQ(
	CString strName,
	BYTE bInvenIDofOpenCash,
	BYTE bSlotIDofOpenCash)
{
	CPacket vMSG;

	vMSG.SetID(CS_SOULMATEREGREADY_REQ)
		<< strName
		<< bInvenIDofOpenCash
		<< bSlotIDofOpenCash;

	Say(&vMSG);
}

void CTClientSession::SendCS_SOULMATEREG_REQ(
	CString strName,
	BYTE bInvenIDofOpenCash,
	BYTE bSlotIDofOpenCash )
{
	CPacket vMSG;

	vMSG.SetID(CS_SOULMATEREG_REQ)
		<< strName
		<< bInvenIDofOpenCash
		<< bSlotIDofOpenCash;

	Say(&vMSG);
}

void CTClientSession::SendCS_SOULMATEEND_REQ()
{
	CPacket vMSG;

	vMSG.SetID(CS_SOULMATEEND_REQ);

	Say(&vMSG);
}

void CTClientSession::SendCS_GAMBLECHECK_REQ(BYTE bInven,
											 BYTE bSlotID,
											 WORD wNpcID )
{
	CPacket vMSG;

	vMSG.SetID(CS_GAMBLECHECK_REQ)
		<< bInven
		<< bSlotID
		<< wNpcID;

	Say(&vMSG);
}

void CTClientSession::SendCS_GAMBLEOPEN_REQ(BYTE bInven,
											BYTE bSlotID,
											WORD wNpcID,
											BYTE bInvenIDofOpenCash,
											BYTE bSlotIDofOpenCash )
{
	CPacket vMSG;

	vMSG.SetID(CS_GAMBLEOPEN_REQ)
		<< bInven
		<< bSlotID
		<< wNpcID
		<< bInvenIDofOpenCash
		<< bSlotIDofOpenCash;

	Say(&vMSG);
}

void CTClientSession::SendCS_TAKEGODBALL_REQ(WORD wGodBallID)
{
	CPacket vMSG;

	vMSG.SetID(CS_TAKEGODBALL_REQ)
		<< wGodBallID;

	Say(&vMSG);

	TRACE("SendCS_TAKEGODBALL_REQ (wGodBallID:%u)\n", 
		wGodBallID);
}

void CTClientSession::SendCS_MOUNTGODBALL_REQ(WORD wGodTower)
{
	CPacket vMSG;

	vMSG.SetID(CS_MOUNTGODBALL_REQ)
		<< wGodTower;

	TRACE("SendCS_MOUNTGODBALL_REQ (wGodTower:%u)\n", 
		wGodTower);

	Say(&vMSG);
}

void CTClientSession::SendCS_DEMOUNTGODBALL_REQ(WORD wGodTower)
{
	CPacket vMSG;

	vMSG.SetID(CS_DEMOUNTGODBALL_REQ)
		<< wGodTower;

	TRACE("SendCS_DEMOUNTGODBALL_REQ (wGodTower:%u)\n", 
		wGodTower);

	Say(&vMSG);
}

void CTClientSession::SendCS_DURATIONREP_REQ(BYTE bNeedCost,
											 BYTE bType,
											 BYTE bInven,		
											 BYTE bItemID,
											 WORD wNpcID,
											 BYTE bInvenIDofOpenCash,
											 BYTE bSlotIDofOpenCash)
{
	CPacket vMSG;

	vMSG.SetID(CS_DURATIONREP_REQ)
		<< bNeedCost
		<< bType
		<< bInven
		<< bItemID
		<< wNpcID
		<< bInvenIDofOpenCash
		<< bSlotIDofOpenCash;

	Say(&vMSG);
}

void CTClientSession::SendCS_REFINE_REQ(BYTE bNeedCost,	
										BYTE bInven,
										BYTE bItemID,
										WORD wNpcID,
										BYTE bInvenIDofOpenCash,
										BYTE bSlotIDofOpenCash,
										LPTINVENITEM_ARRAY pAddItems)
{
	CPacket vMSG;

	BYTE bAddCount = (BYTE)pAddItems->size();

	vMSG.SetID(CS_REFINE_REQ)
		<< bNeedCost
		<< bInven
		<< bItemID
		<< bAddCount
		<< wNpcID
		<< bInvenIDofOpenCash
		<< bSlotIDofOpenCash;

	for(BYTE i=0; i<bAddCount; ++i)
	{
		vMSG << (*pAddItems)[i].m_bInven
			 << (*pAddItems)[i].m_bItem;
	}

	Say(&vMSG);
}

void CTClientSession::SendCS_ACTEND_REQ()
{
	CPacket vMSG;

	vMSG.SetID(CS_ACTEND_REQ);

	Say(&vMSG);
}

void CTClientSession::SendCS_CASHCABINETBUY_REQ( WORD wShopItemID )
{
	CPacket vMSG;

	vMSG.SetID(CS_CASHCABINETBUY_REQ);

	vMSG << wShopItemID;

	Say(&vMSG);
}

void CTClientSession::SendCS_CASHCABINETBUY_REQ( BYTE bNeedInven, BYTE bNeedItem )
{
	CPacket vMSG;

	vMSG.SetID(CS_CASHCABINETBUY_REQ);

	vMSG
		<< bNeedInven
		<< bNeedItem;

	Say(&vMSG);
}

void CTClientSession::SendCS_CASHITEMPUTIN_REQ(
							  BYTE bInvenID,
							  BYTE bItemID)
{
	CPacket vMSG;

	vMSG.SetID(CS_CASHITEMPUTIN_REQ);

	vMSG <<bInvenID
		<< bItemID;

	Say(&vMSG);

	CTClientGame::m_bSendedCS_CASHITEMPUTIN_REQ = TRUE;
}

void CTClientSession::SendCS_HEROSELECT_REQ(
	WORD wBattleZoneID,
	CString strHeroName)
{
	CPacket vMSG;
	vMSG.SetID(CS_HEROSELECT_REQ);

	vMSG <<wBattleZoneID
		<< strHeroName;

	Say(&vMSG);
}

void CTClientSession::SendCS_HEROLIST_REQ()
{
	CPacket vMSG;
	vMSG.SetID(CS_HEROLIST_REQ);

	Say(&vMSG);
}

void CTClientSession::SendCS_CHANGENAME_REQ(
	BYTE bInvenID,
	BYTE bItemID,
	CString szName)
{
	CPacket vMSG;
	vMSG.SetID(CS_CHANGENAME_REQ);

	vMSG
		<< bInvenID
		<< bItemID
		<< szName;

	Say(&vMSG);
}

void CTClientSession::SendCS_STOPTHECLOCK_REQ(
	BYTE bInven,
	BYTE bItem, //가방일 경우 INVALID_SLOT
	WORD wShopItemID)
{
	CPacket vMSG;
	vMSG.SetID(CS_STOPTHECLOCK_REQ);

	vMSG
		<< bInven
		<< bItem
		<< wShopItemID;

	Say(&vMSG);
}

void CTClientSession::SendCS_STOPTHECLOCK_REQ(
	BYTE bInven,
	BYTE bItem,
	BYTE bNeedInven,
	BYTE bNeedItem )
{
	CPacket vMSG;
	vMSG.SetID(CS_STOPTHECLOCK_REQ);

	vMSG
		<< bInven
		<< bItem
		<< bNeedInven
		<< bNeedItem;

	Say(&vMSG);
}

void CTClientSession::SendCS_CHECKRELAY_REQ()
{
	CPacket vMSG;
	vMSG.SetID(CS_CHECKRELAY_REQ);
	Say(&vMSG);
}

//1218

void CTClientSession::SendCS_GUILDPOINTLOG_REQ()
{
	CPacket vMSG;
	vMSG.SetID(CS_GUILDPOINTLOG_REQ);
	Say(&vMSG);
}

void CTClientSession::SendCS_GUILDPOINTREWARD_REQ(
	CString strTarget,
	DWORD dwPoint,
	CString strMessage)
{
	CPacket vMSG;
	vMSG.SetID(CS_GUILDPOINTREWARD_REQ);
	
	vMSG
		<< strTarget
		<< dwPoint
		<< strMessage;

	Say(&vMSG);
}

void CTClientSession::SendCS_GUILDPVPRECORD_REQ()
{
	CPacket vMSG;
	vMSG.SetID(CS_GUILDPVPRECORD_REQ);
	Say(&vMSG);
}

void CTClientSession::SendCS_PVPRECORD_REQ(
	BYTE bType )
{
	CPacket vMSG;
	vMSG.SetID(CS_PVPRECORD_REQ);

	vMSG
		<< bType;

	Say(&vMSG);
}

void CTClientSession::SendCS_FRIENDLIST_REQ()
{
	CPacket vMSG;
	vMSG.SetID( CS_FRIENDLIST_REQ );

	Say( &vMSG );
}

void CTClientSession::SendCS_ITEMCHANGE_REQ( BYTE bInvenID, BYTE bItemID )
{
	CPacket vMSG;
	vMSG.SetID( CS_ITEMCHANGE_REQ );

	vMSG
		<< bInvenID
		<< bItemID;

	Say( &vMSG );
}

void CTClientSession::SendCS_AUCTIONFIND_REQ( LPTAUCTIONFINDINFO pFindInfo )
{
	CPacket vMSG;
	vMSG.SetID( CS_AUCTIONFIND_REQ );

	vMSG
		<< pFindInfo->wNpcID
		<< pFindInfo->bMinWearLv
		<< pFindInfo->bMaxWearLv
		<< pFindInfo->bMinPowerLv
		<< pFindInfo->bMaxPowerLv
		<< pFindInfo->bOption
		<< pFindInfo->wPageNum
		<< pFindInfo->bCountPerPage
		<< pFindInfo->bAlignKind;

	WORD size = pFindInfo->vAuctionCode.size();
	vMSG << size;
	size_t i =0;
	for( ; i < size; ++i )
	{
		vMSG
			<< (DWORD)pFindInfo->vAuctionCode[i]
			<< (WORD)pFindInfo->vItemID[i];
	}

	Say( &vMSG );
}

void CTClientSession::SendCS_AUCTIONREGLIST_REQ( LPTAUCTIONFINDINFO pFindInfo )
{
	CPacket vMSG;
	vMSG.SetID( CS_AUCTIONREGLIST_REQ );

	vMSG
		<< pFindInfo->wNpcID
		<< pFindInfo->wPageNum
		<< pFindInfo->bCountPerPage
		<< pFindInfo->bAlignKind;

	Say( &vMSG );
}

void CTClientSession::SendCS_AUCTIONBIDLIST_REQ( LPTAUCTIONFINDINFO pFindInfo )
{
	CPacket vMSG;
	vMSG.SetID( CS_AUCTIONBIDLIST_REQ );

	vMSG
		<< pFindInfo->wNpcID
		<< pFindInfo->wPageNum
		<< pFindInfo->bCountPerPage
		<< pFindInfo->bAlignKind;

	Say( &vMSG );
}

void CTClientSession::SendCS_GUILDTACTICSWANTEDADD_REQ(
	DWORD dwID,			//새로올릴때-0 / 수정-아이디
	CString strTitle,		//제목
	CString strText,		//내용
	BYTE bDay,			//기간
	BYTE bMinLevel,		//최소레벨
	BYTE bMaxLevel,		//최대레벨
	DWORD dwPoint,		//공적
	DWORD dwGold,
	DWORD dwSilver,
	DWORD dwCooper)
{
	if(strTitle.IsEmpty())
		strTitle = CTTextLinker::GetInstance()->BuildNetString( _T(""), strTitle);

	if(strText.IsEmpty())
		strText = CTTextLinker::GetInstance()->BuildNetString( _T(""), strText);

//	strTitle = CCurse::ConvertString( strTitle, TRUE);
//	strText = CCurse::ConvertString( strText, TRUE);
	if( !CTStrFilter::ConvertString( strTitle, true ) ||
		CTStrFilter::ConvertString( strTitle, true ) )
		return ;

	CPacket vMSG;
	vMSG.SetID( CS_GUILDTACTICSWANTEDADD_REQ );
	vMSG << dwID
		<< strTitle
		<< strText
		<< bDay
		<< bMinLevel
		<< bMaxLevel
		<< dwPoint
		<< dwGold
		<< dwSilver
		<< dwCooper;

	Say( &vMSG );
}

void CTClientSession::SendCS_GUILDTACTICSWANTEDDEL_REQ(
//	DWORD dwGuildID,
	DWORD dwID )
{
	CPacket vMSG;
	vMSG.SetID( CS_GUILDTACTICSWANTEDDEL_REQ );

	vMSG
//		<< dwGuildID
		<< dwID;

	Say( &vMSG );
}

void CTClientSession::SendCS_GUILDTACTICSWANTEDLIST_REQ()
{
	CPacket vMSG;
	vMSG.SetID( CS_GUILDTACTICSWANTEDLIST_REQ );
	Say( &vMSG );
}

void CTClientSession::SendCS_GUILDTACTICSVOLUNTEERING_REQ(
	DWORD dwGuildID,
	DWORD dwID )
{
	CPacket vMSG;
	vMSG.SetID( CS_GUILDTACTICSVOLUNTEERING_REQ );
	
	vMSG
		<< dwGuildID
		<< dwID;

	Say( &vMSG );
}

void CTClientSession::SendCS_GUILDTACTICSVOLUNTEERINGDEL_REQ()
{
	CPacket vMSG;
	vMSG.SetID( CS_GUILDTACTICSVOLUNTEERINGDEL_REQ );
	Say( &vMSG );
}

void CTClientSession::SendCS_GUILDTACTICSREPLY_REQ(
	DWORD dwCharID,
	BYTE bReply )
{
	CPacket vMSG;
	vMSG.SetID( CS_GUILDTACTICSREPLY_REQ );
	vMSG
		<< dwCharID
		<< bReply;
	Say( &vMSG );
}

void CTClientSession::SendCS_GUILDTACTICSKICKOUT_REQ(
	DWORD dwCharID )
{
	CPacket vMSG;
	vMSG.SetID( CS_GUILDTACTICSKICKOUT_REQ );
	vMSG
		<< dwCharID;
	Say( &vMSG );
}

void CTClientSession::SendCS_GUILDTACTICSVOLUNTEERLIST_REQ()
{
	CPacket vMSG;
	vMSG.SetID( CS_GUILDTACTICSVOLUNTEERLIST_REQ );
	Say( &vMSG );
}

void CTClientSession::SendCS_GUILDVOLUNTEERREPLY_REQ(
	DWORD dwCharID,
	BYTE bReply )
{
	CPacket vMSG;
	vMSG.SetID( CS_GUILDVOLUNTEERREPLY_REQ );
	vMSG
		<< dwCharID
		<< bReply;

	Say( &vMSG );
}

void CTClientSession::SendCS_AUCTIONINTERESTLIST_REQ( LPTAUCTIONFINDINFO pFindInfo )
{
	CPacket vMSG;
	vMSG.SetID( CS_AUCTIONINTERESTLIST_REQ );

	vMSG
		<< pFindInfo->wNpcID
		<< pFindInfo->wPageNum
		<< pFindInfo->bCountPerPage
		<< pFindInfo->bAlignKind;

	Say( &vMSG );
}

void CTClientSession::SendCS_AUCTIONBID_REQ( WORD wNpcID, DWORD dwAuctionID, __int64 ldwBidPrice )
{
	CPacket vMSG;
	vMSG.SetID( CS_AUCTIONBID_REQ );

	vMSG
		<< wNpcID
		<< dwAuctionID
		<< ldwBidPrice;

	Say( &vMSG );
}

void CTClientSession::SendCS_AUCTIONBUYDIRECT_REQ( WORD wNpcID, DWORD dwAuctionID, BYTE bCount )
{
	CPacket vMSG;
	vMSG.SetID( CS_AUCTIONBUYDIRECT_REQ );

	vMSG
		<< wNpcID
		<< dwAuctionID
		<< bCount;

	Say( &vMSG );
}

void CTClientSession::SendCS_AUCTIONREG_REQ( LPTAUCTIONREGINFO pInfo )
{
	CPacket vMSG;
	vMSG.SetID( CS_AUCTIONREG_REQ );

	vMSG
		<< pInfo->wNpcID
		<< pInfo->wHour
		<< pInfo->bInvenID
		<< pInfo->bItemID
		<< pInfo->ldwDirectPrice
		<< pInfo->ldwStartPrice
		<< pInfo->dwCode;

	Say( &vMSG );
}

void CTClientSession::SendCS_AUCTIONREGCANCEL_REQ( WORD wNpcID, DWORD dwAuctionID )
{
	CPacket vMSG;
	vMSG.SetID( CS_AUCTIONREGCANCEL_REQ );

	vMSG
		<< wNpcID
		<< dwAuctionID;

	Say( &vMSG );
}

void CTClientSession::SendCS_AUCTIONINTEREST_REQ( BYTE bType, DWORD dwAuctionID, WORD wNpcID )
{
	CPacket vMSG;
	vMSG.SetID( CS_AUCTIONINTEREST_REQ );

	vMSG
		<< bType
		<< dwAuctionID
		<< wNpcID;

	Say( &vMSG );
}

void CTClientSession::SendCS_COUNTDOWN_REQ(DWORD dwCommand)
{
	CPacket vMSG;
	vMSG.SetID( CS_COUNTDOWN_REQ)
		<< dwCommand;

	Say( &vMSG );
}

void CTClientSession::SendCS_WASTEREFINE_REQ( BYTE bInvenID, BYTE bItemID )
{
	CPacket vMSG;
	vMSG.SetID( CS_WASTEREFINE_REQ );

	vMSG
		<< bInvenID
		<< bItemID;

	Say( &vMSG );
}

void CTClientSession::SendCS_PETCANCEL_REQ()
{
	CPacket vMSG;
	vMSG.SetID( CS_PETCANCEL_REQ );

	Say( &vMSG );
}

void CTClientSession::SendCS_APEXSTART_REQ( long re )
{
	CPacket vMSG;

	vMSG.SetID(CS_APEXSTART_REQ);

	vMSG
		<< re;

	Say(&vMSG);
}

long CTClientSession::SnedCS_APEXDATA_REQ( const char * pBuffer, int nLen )
{
	CPacket vMSG;

	vMSG.SetID( CS_APEXDATA_REQ )
		<< nLen;

	vMSG.AttachBinary( (LPVOID)pBuffer, nLen );

	CTClientGame* pGame = CTClientGame::GetInstance();
	CTClientSession* pSession = pGame->GetSession();

	if ( pSession )
		pSession->Say( &vMSG );

	return 0;
}

void CTClientSession::SendCS_KICKOUTMAP_REQ()
{
	CPacket vMSG;
	vMSG.SetID( CS_KICKOUTMAP_REQ );
	Say(&vMSG);
}

void CTClientSession::SendCS_REGGUILDCLOAK_REQ( BYTE bInvenID, BYTE bItemID, BYTE bReg )
{
	CPacket vMSG;
	vMSG.SetID( CS_REGGUILDCLOAK_REQ )
		<< bInvenID
		<< bItemID
		<< bReg;

	Say(&vMSG);
}

void CTClientSession::SendCS_FAMERANKLIST_REQ( BYTE bType, BYTE bMonth )
{
	CPacket vMSG;

	vMSG.SetID( CS_FAMERANKLIST_REQ )
		<< bType
		<< bMonth;

	Say(&vMSG);
}

void CTClientSession::SendCS_MONTHRANKLIST_REQ()
{
	CPacket vMSG;
	vMSG.SetID( CS_MONTHRANKLIST_REQ );
	Say(&vMSG);
}

void CTClientSession::SendCS_WARLORDSAY_REQ( BYTE bType, CString strSay )
{
	CPacket vMSG;

	vMSG.SetID( CS_WARLORDSAY_REQ )
		<< bType
		<< strSay;

	Say(&vMSG);
}

void CTClientSession::SendCS_CHATBAN_REQ(CString strName, WORD wMin)
{
	CPacket vMSG;

	vMSG.SetID( CS_CHATBAN_REQ )
		<< strName
		<< wMin;

	Say(&vMSG);
}

void CTClientSession::SendCS_POSTLIST_REQ(WORD wPage)
{
	CPacket vMSG;

	vMSG.SetID( CS_POSTLIST_REQ )
		<< wPage;

	Say(&vMSG);
}
void CTClientSession::SendCS_FIRSTGRADEGROUP_REQ()
{
	CPacket vMSG;
	vMSG.SetID( CS_FIRSTGRADEGROUP_REQ );
	Say(&vMSG);
}

#ifdef ADD_TOURNAMENT
void CTClientSession::SendCS_TOURNAMENTAPPLYINFO_REQ()
{
	CPacket vMSG;
	vMSG.SetID( CS_TOURNAMENTAPPLYINFO_REQ );
	Say(&vMSG);
}

void CTClientSession::SendCS_TOURNAMENTAPPLY_REQ( BYTE bEntryID )
{
	CPacket vMSG;
	vMSG.SetID( CS_TOURNAMENTAPPLY_REQ )
		<< bEntryID;
	Say(&vMSG);
}

void CTClientSession::SendCS_TOURNAMENTJOINLIST_REQ()
{
	CPacket vMSG;
	vMSG.SetID( CS_TOURNAMENTJOINLIST_REQ );
	Say(&vMSG);
}

void CTClientSession::SendCS_TOURNAMENTPARTYLIST_REQ( DWORD dwChiefID )
{
	CPacket vMSG;
	vMSG.SetID( CS_TOURNAMENTPARTYLIST_REQ )
		<< dwChiefID;
	Say(&vMSG);
}

void CTClientSession::SendCS_TOURNAMENTPARTYADD_REQ( CString strName )
{
	CPacket vMSG;
	vMSG.SetID( CS_TOURNAMENTPARTYADD_REQ )
		<< strName;
	Say(&vMSG);
}

void CTClientSession::SendCS_TOURNAMEMTMATCHLIST_REQ()
{
	CPacket vMSG;
	vMSG.SetID( CS_TOURNAMEMTMATCHLIST_REQ );
	Say(&vMSG);
}

void CTClientSession::SendCS_TOURNAMENTEVENTLIST_REQ()
{
	CPacket vMSG;
	vMSG.SetID( CS_TOURNAMENTEVENTLIST_REQ );
	Say(&vMSG);
}

void CTClientSession::SendCS_TOURNAMENTEVENTINFO_REQ( BYTE bEntryID )
{
	CPacket vMSG;
	vMSG.SetID( CS_TOURNAMENTEVENTINFO_REQ )
		<< bEntryID;
	Say(&vMSG);
}

void CTClientSession::SendCS_TOURNAMENTEVENTJOIN_REQ( BYTE bEntryID, DWORD dwCharID )
{
	CPacket vMSG;
	vMSG.SetID( CS_TOURNAMENTEVENTJOIN_REQ )
		<< bEntryID
		<< dwCharID;
	Say(&vMSG);
}

void CTClientSession::SendCS_TOURNAMENTCHEER_REQ( DWORD dwCharID, BYTE bInven, BYTE bSlot )
{
	CPacket vMSG;
	vMSG.SetID( CS_TOURNAMENTCHEER_REQ )
		<< bInven
		<< bSlot
		<< dwCharID;

	Say(&vMSG);
}

void CTClientSession::SendCS_TOURNAMENTAPPLYCANCEL_REQ()
{
	CPacket vMSG;
	vMSG.SetID( CS_TOURNAMENTAPPLYCANCEL_REQ );
	Say(&vMSG);
}

void CTClientSession::SendCS_TOURNAMENTPARTYDEL_REQ( DWORD dwTargetID )
{
	CPacket vMSG;
	vMSG.SetID( CS_TOURNAMENTPARTYDEL_REQ )
		<< dwTargetID;
	Say(&vMSG);
}

void CTClientSession::SendCS_TOURNAMENTSCHEDULE_REQ()
{
	CPacket vMSG;
	vMSG.SetID( CS_TOURNAMENTSCHEDULE_REQ );
	Say(&vMSG);
}
#endif

#ifdef ADD_RPS

void CTClientSession::SendCS_RPSSTART_REQ( BYTE bType, BYTE bInven, BYTE bItemID )
{
	CPacket vMSG;
	vMSG.SetID( CS_RPSSTART_REQ )
		<< bType
		<< bInven
		<< bItemID;

	Say(&vMSG);
}

void CTClientSession::SendCS_RPSGAME_REQ( BYTE bRPS )
{
	CPacket vMSG;
	vMSG.SetID( CS_RPSGAME_REQ )
		<< bRPS;
	Say(&vMSG);
}

#endif

#ifdef KEYHOOK_CTRLALTDEL
void CTClientSession::SendCS_ACDCLOSE_REQ()
{
	CPacket vMSG;
	vMSG.SetID( CS_ACDCLOSE_REQ );
	Say(&vMSG);
}
#endif