#pragma once

#include "ProtocolBase.h"


//////////////////////////////////////////////////////////////////////
// TClient/TLoginSvr Protocol

#define CS_LOGIN_REQ				(CS_LOGIN + 0x0001)
// WORD wVersion
// STRING strUserID
// STRING strPasswd

#define CS_LOGIN_ACK				(CS_LOGIN + 0x0002)
// BYTE bResult
// DWORD dwUserID
// BYTE bCreateCnt
// DWORD dwCharID
// DWORD dwKEY
// DWORD dwMapIP
// WORD wPort
// BYTE bCharCreateCount
// BYTE bInPcBang
// DWORD dwPremium
// __time64_t dCurrentTime

#define CS_GROUPLIST_REQ			(CS_LOGIN + 0x0003)
#define CS_GROUPLIST_ACK			(CS_LOGIN + 0x0004)
// BYTE bCount
// {
// STRING strName
// BYTE bGroupID
// BYTE bType
// BYTE bStatus
// BYTE bCharCnt
// }

#define CS_CHANNELLIST_REQ			(CS_LOGIN + 0x0005)
// BYTE bGroupID

#define CS_CHANNELLIST_ACK			(CS_LOGIN + 0x0006)
// BYTE bCount
// {
// STRING strName
// BYTE bChannel
// }

#define CS_CHARLIST_REQ				(CS_LOGIN + 0x0007)
// BYTE bGroupID

#define CS_CHARLIST_ACK				(CS_LOGIN + 0x0008)
// BYTE bCount
// {
// DWORD dwID
// STRING strName
// BYTE bSlotID
// BYTE bLevel
// BYTE bClass
// BYTE bRace
// BYTE bCountry
// BYTE bSex
// BYTE bHair
// BYTE bFace
// BYTE bBody
// BYTE bPants
// BYTE bHand
// BYTE bFoot
// DWORD dwRegionID
// BYTE bEquipItemCount
//		{
//		BYTE bItemID
//		WORD wItemID
//		BYTE bLevel
//		BYTE bGradeEffect	//0316 수정
//		}
// }

#define CS_CREATECHAR_REQ			(CS_LOGIN + 0x0009)
// BYTE bGroupID
// STRING strName
// BYTE bSlotID
// BYTE bClass
// BYTE bRace
// BYTE bCountry
// BYTE bSex
// BYTE bHair
// BYTE bFace
// BYTE bBody
// BYTE bPants
// BYTE bHand
// BYTE bFoot

#define CS_CREATECHAR_ACK			(CS_LOGIN + 0x000A)
// BYTE bResult
// DWORD dwID
// STRING strName
// BYTE bSlotID
// BYTE bClass
// BYTE bRace
// BYTE bCountry
// BYTE bSex
// BYTE bHair
// BYTE bFace
// BYTE bBody
// BYTE bPants
// BYTE bHand
// BYTE bFoot
// BYTE bCreateCnt

#define CS_DELCHAR_REQ				(CS_LOGIN + 0x000B)
// BYTE bGroupID
// STRING strPasswd
// DWORD dwCharID

#define CS_DELCHAR_ACK				(CS_LOGIN + 0x000C)
// BYTE bResult
// DWORD dwCharID

#define CS_START_REQ				(CS_LOGIN + 0x000D)
// BYTE bGroupID
// BYTE bChannel
// DWORD dwCharID

#define CS_START_ACK				(CS_LOGIN + 0x000E)
// BYTE bResult
// DWORD dwMapIP
// WORD wPort
// BYTE bServerID

#define CS_TESTLOGIN_REQ			(CS_LOGIN + 0x000F) // 현승룡 Svr Tester
#define CS_TESTVERSION_REQ			(CS_LOGIN + 0x0011)
#define CS_TESTVERSION_ACK			(CS_LOGIN + 0x0012)
#define CS_AGREEMENT_REQ			(CS_LOGIN + 0x0013)
// WORD wVersion
#define CS_HOTSEND_REQ				(CS_LOGIN + 0x0014)
#define CS_HOTSEND_ACK				(CS_LOGIN + 0x0015)

//////////////////////////////////////////////////////////////////////
// TClient/TMapSvr Protocol

#define CS_CONNECT_REQ				(CS_MAP + 0x0001)
// WORD wVersion			: Version
// BYTE bChannel			: Channel ID
// DWORD dwUserID			: User ID
// DWORD dwCharID			: Character ID
// DWORD dwKEY				: Enter key
// DWORD dwIPAddr			: IP address
// WORD wPort				: Port

#define CS_CONNECT_ACK				(CS_MAP + 0x0002)
// BYTE bResult				: enum TCONNECT_RESULT value

#define CS_INVALIDCHAR_ACK			(CS_MAP + 0x0003)
#define CS_ADDCONNECT_ACK			(CS_MAP + 0x0004)
// BYTE bCount
// {
// DWORD dwIPAddr
// WORD wPort
// BYTE bServerID
// }

#define CS_CHARINFO_ACK				(CS_MAP + 0x0005)
// DWORD dwCharID
// STRING strNAME
// BYTE bStartAct		0:연출불필요 1:연출필요		12m10d
// BYTE bClass
// BYTE bRace
// BYTE bCountry
// BYTE bAidCountry		100914
// BYTE bSex
// BYTE bHair
// BYTE bFace
// BYTE bBody
// BYTE bPants
// BYTE bHand
// BYTE bFoot
// BYTE bHelmetHide		: 투구숨기기
// BYTE bLevel
// WORD wPartyID
// DWORD dwGuildID
// DWORD dwFame			: 길드문양
// DWORD dwFameColor	: 길드문양색깔
// BYTE bGuildDuty
// BYTE bGuildPeer
// STRING strGuildName
// DWORD m_dwTacticsID // 용병
// STRING strTacticsName 
// DWORD dwGold
// DWORD dwSilver
// DWORD dwCooper
// DWORD dwPrevExp
// DWORD dwNextExp
// DWORD dwEXP
// DWORD dwMaxHP
// DWORD dwHP
// DWORD dwMaxMP
// DWORD dwMP
// DWORD dwPartyChiefID
// WORD wCommanderID
// DWORD dwRegionID
// WORD wMapID
// FLOAT fPosX
// FLOAT fPosY
// FLOAT fPosZ
// WORD wDIR
// WORD wMySkillPoint
// BYTE bLuckyNumber
// WORD wSkillPoint_1
// WORD wSkillPoint_2
// WORD wSkillPoint_3
// WORD wSkillPoint_4
// BYTE bInvenCount
// {
//		BYTE bInvenID
//		WORD wItemID
//		__time64_t m_dEndTime				//사용 기한
//		BYTE bItemCount
//		{
//			BYTE bItemID
//			WORD wItemID
//			BYTE bLevel
//			BYTE bCount
//			DWORD dwDuraMax						//내구도
//			DWORD dwDuraCurrent
//			BYTE bRefineMax
//			BYTE bRefineCurrent
//			BYTE bGLevel
//			__time64_t dEndTime					//사용 기한
//			BYTE bGradeEffect
//			BYTE bWrap
//			BYTE bELD
//			BYTE bMagicCount
//			{
//				BYTE bMagicID
//				WORD wValue
//				DWORD dwTime
//			}
//		}
// }
// BYTE bSkillCount
// {
//		WORD wSkillID
//		BYTE bLevel
//		DWORD dwTick
// }
// BYTE bMaintainSkillCount
// {
//		WORD wSkillID
//		BYTE bLevel
//		DWORD dwTick
//		DWORD dwAttackID
//		BYTE bAttackType
//		DWORD dwHostID
//		BYTE bHostType
//		BYTE bHit
//		WORD wAttackLevel
//		BYTE bAttackerLevel
//		DWORD dwPysMinPower
//		DWORD dwPysMaxPower
//		DWORD dwMgMinPower
//		DWORD dwMgMaxPower
//		BYTE bCanSelect
//		BYTE bAttackCountry
//		FLOAT fPosX
//		FLOAT fPosY
//		FLOAT fPosZ
// }
// BYTE bPartyMemberCount
// {
//		DWORD dwCharID
//		STRING strName
//		STRING strGuildName
//		BYTE bLevel
//		DWORD dwMaxHP
//		DWORD dwHP
//		DWORD dwMaxMP
//		DWORD dwMP
//		BYTE bRace
//		BYTE bSex
//		BYTE bFace
//		BYTE bHair
// }
// BYTE bHotkeyCount
// {
//		BYTE bInvenID
//		{					MAX_HOTKEY_POS 만큼 순서대로...
//			BYTE bType
//			WORD wID
//		}
// }
// BYTE bUsedITemCount
// {
//		BYTE bKind
//		DWORD dwRemainTick
// }
// DWORD dwPvPTotalPoint		//1218
// DWORD dwPvPUseablePoint		//1218
#define CS_ENTER_ACK				(CS_MAP + 0x0006)	//1106
// DWORD dwCharID
// STRING strNAME
// STRING strComment
// DWORD dwGuildID
// DWORD dwFame			: 길드문양
// DWORD dwFameColor	: 길드문양색깔
// STRING strGuildName
// BYTE bGuildPeer
// DWORD dwTacticsID // 용병
// STRING strTacticsName
// BYTE bStore
// STRING strStoreName
// DWORD dwRiding
// BYTE bClass
// BYTE bRace
// BYTE bCountry
// BYTE bAidCountry		100914
// BYTE bSex
// BYTE bHair
// BYTE bFace
// BYTE bBody
// BYTE bPants
// BYTE bHand
// BYTE bFoot
// BYTE bLevel
// BYTE bHelmetHide		: 투구숨기기
// DWORD dwMaxHP
// DWORD dwHP
// DWORD dwMaxMP
// DWORD dwMP
// DWORD dwPartyChiefID
// WORD wPartyID
// WORD wCommanderID
// FLOAT fPosX
// FLOAT fPosY
// FLOAT fPosZ
// BYTE bAction
// BYTE bBlock
// BYTE bMode
// WORD wPitch
// WORD wDIR
// BYTE bMouseDIR
// BYTE bKeyDIR
// BYTE bColor
// DWORD dwRegionID
// BYTE bInPcBang
// WORD wCastle				//공성전
// BYTE bCamp				//공성전
// WORD wGodBall			//공성전
// BYTE bMaintainSkillCount
// {
//		WORD wSkillID
//		BYTE bLevel
//		DWORD dwTick
//		DWORD dwAttackID
//		BYTE bAttackType
//		DWORD dwHostID
//		BYTE bHostType
//		BYTE bHit
//		WORD wAttackLevel
//		BYTE bAttackerLevel
//		DWORD dwPysMinPower
//		DWORD dwPysMaxPower
//		DWORD dwMgMinPower
//		DWORD dwMgMaxPower
//		BYTE bCanSelect
//		BYTE bAttackCountry
//		FLOAT fPosX
//		FLOAT fPosY
//		FLOAT fPosZ
// }
// BYTE bItemCount
// {
// BYTE bItemID
// WORD wItemID
// BYTE bLevel
// BYTE bCount
// DWORD dwDuraMax						//내구도
// DWORD dwDuraCurrent
// BYTE bRefineMax
// BYTE bRefineCurrent
// BYTE bGLevel
// __time64_t m_dEndTime				//사용 기한
// BYTE bGradeEffect
// BYTE bWrap
// BYTE bELD
// WORD wColor
// BYTE bRegGuild
// BYTE bMagicCount
// {
// BYTE bMagicID
// WORD wValue
// DWORD dwTime
// }
// }
// BYTE bNewMember

#define CS_LEAVE_ACK				(CS_MAP + 0x0007)
// DWORD dwCharID
// BYTE bExitMap

#define CS_CONREADY_REQ				(CS_MAP + 0x0008)
#define CS_MOVE_REQ					(CS_MAP + 0x0009)
// WORD wMapID
// FLOAT fPosX
// FLOAT fPosY
// FLOAT fPosZ
// WORD wPitch
// WORD wDIR
// BYTE bMouseDIR
// BYTE bKeyDIR
// BYTE bAction
// BYTE bGhost

#define CS_MOVE_ACK					(CS_MAP + 0x000A)
// DWORD dwCharID
// FLOAT fPosX
// FLOAT fPosY
// FLOAT fPosZ
// WORD wPitch
// WORD wDIR
// BYTE bMouseDIR
// BYTE bKeyDIR
// BYTE bAction

#define CS_JUMP_REQ					(CS_MAP + 0x000B)
// FLOAT fPosX
// FLOAT fPosY
// FLOAT fPosZ
// WORD wPitch
// WORD wDIR
// BYTE bAction

#define CS_JUMP_ACK					(CS_MAP + 0x000C)
// DWORD dwCharID
// FLOAT fPosX
// FLOAT fPosY
// FLOAT fPosZ
// WORD wPitch
// WORD wDIR
// BYTE bAction

#define CS_BLOCK_REQ				(CS_MAP + 0x000D)
// FLOAT fPosX
// FLOAT fPosY
// FLOAT fPosZ
// WORD wPitch
// WORD wDIR
// BYTE bAction

#define CS_BLOCK_ACK				(CS_MAP + 0x000E)
// DWORD dwCharID
// FLOAT fPosX
// FLOAT fPosY
// FLOAT fPosZ
// WORD wPitch
// WORD wDIR
// BYTE bAction

#define CS_MONHOST_ACK				(CS_MAP + 0x000F)
// DWORD dwMonID
// BYTE bSet

#define CS_ADDMON_ACK				(CS_MAP + 0x0011)
// DWORD dwMonID
// WORD wTemplateID
// BYTE bLevel
// DWORD dwMaxHP
// DWORD dwHP
// DWORD dwMaxMP
// DWORD dwMP
// FLOAT fPosX
// FLOAT fPosY
// FLOAT fPosZ
// WORD wPitch
// WORD wDIR
// BYTE bMouseDIR
// BYTE bKeyDIR
// BYTE bAction
// BYTE bMode
// BYTE bNewMember
// BYTE bColor
// DWORD dwRegionID
// BYTE bMaintainSkillCount
// {
//		WORD wSkillID
//		BYTE bLevel
//		DWORD dwTick
//		DWORD dwAttackID
//		BYTE bAttackType
//		DWORD dwHostID
//		BYTE bHostType
//		BYTE bHit
//		WORD wAttackLevel
//		BYTE bAttackerLevel
//		DWORD dwPysMinPower
//		DWORD dwPysMaxPower
//		DWORD dwMgMinPower
//		DWORD dwMgMaxPower
//		BYTE bCanSelect
//		BYTE bAttackCountry
//		FLOAT fPosX
//		FLOAT fPosY
//		FLOAT fPosZ
// }
#define CS_DELMON_ACK				(CS_MAP + 0x0012)
// DWORD dwMonID
// BYTE bExitMap

#define CS_MONACTION_ACK			(CS_MAP + 0x0013)
// DWORD dwMonID
// BYTE bAction
// WORD wPosX
// WORD wPosZ
// DWORD dwTargetID
// BYTE bTargetType

#define CS_MONMOVE_REQ				(CS_MAP + 0x0014)
// WORD wMonCount
// {
// DWORD dwMonID
// BYTE bObjType
// BYTE bChannel
// WORD wMapID
// FLOAT fPosX
// FLOAT fPosY
// FLOAT fPosZ
// WORD wPitch
// WORD wDIR
// BYTE bMouseDIR
// BYTE bKeyDIR
// BYTE bAction
// }

#define CS_MONMOVE_ACK				(CS_MAP + 0x0015)
// WORD wMonCount
// {
// DWORD dwMonID
// BYTE bObjType
// FLOAT fPosX
// FLOAT fPosY
// FLOAT fPosZ
// WORD wPitch
// WORD wDIR
// BYTE bMouseDIR
// BYTE bKeyDIR
// BYTE bAction
// }
#define CS_ENTERLB_REQ				(CS_MAP + 0x0018)
// DWORD dwCharID
// DWORD dwTargetID
// BYTE bTargetType
// DWORD dwMonID
// BYTE bChannel
// WORD wMapID

#define CS_LEAVELB_REQ				(CS_MAP + 0x0019)
// DWORD dwCharID
// DWORD dwTargetID
// BYTE bTargetType
// DWORD dwMonID
// BYTE bChannel
// WORD wMapID

#define CS_ENTERAB_REQ				(CS_MAP + 0x001A)
// DWORD dwCharID
// DWORD dwTargetID
// BYTE bTargetType
// DWORD dwMonID
// BYTE bChannel
// WORD wMapID

#define CS_LEAVEAB_REQ				(CS_MAP + 0x001B)
// DWORD dwCharID
// DWORD dwTargetID
// BYTE bTargetType
// DWORD dwMonID
// BYTE bChannel
// WORD wMapID

#define CS_CHGMODE_REQ				(CS_MAP + 0x001C)
// BYTE bMode

#define CS_CHGMODE_ACK				(CS_MAP + 0x001D)
// DWORD dwID
// BYTE bType
// BYTE bMode

#define CS_ATTACK_REQ				(CS_MAP + 0x001E)
// DWORD dwTargetID
// BYTE bTargetType
// DWORD dwActID
// DWORD dwAniID

#define CS_ATTACK_ACK				(CS_MAP + 0x001F)
// DWORD dwAttackID
// DWORD dwTargetID
// BYTE bAttackType
// BYTE bTargetType
// DWORD dwActID
// DWORD dwAniID
// DWORD dwAttackPower
// WORD wAttackLevel
// BYTE bHit

#define CS_DEFEND_REQ				(CS_MAP + 0x0020)
// DWORD dwAttackHostID
// DWORD dwAttackID
// DWORD dwTargetID
// BYTE bAttackType
// BYTE bTargetType
// WORD wAttackPartyID
// DWORD dwActID
// DWORD dwAniID
// BYTE bChannel
// WORD wMapID
// BYTE bAttackerLevel
// DWORD dwPysMinPower
// DWORD dwPysMaxPower
// DWORD dwMgMinPower
// DWORD dwMgMaxPower
// WORD wSTR
// WORD wINT
// BYTE bCurse
// BYTE bSpecial
// BYTE bCanSelect
// BYTE bAttackCountry
// WORD wAttackLevel
// BYTE bHit
// WORD wSkillID
// BYTE bSkillLevel
// FLOAT fAtkPosX
// FLOAT fAtkPosY
// FLOAT fAtkPosZ
// FLOAT fDefPosX
// FLOAT fDefPosY
// FLOAT fDefPosZ
// DWORD dwRemainTick

#define CS_DEFEND_ACK				(CS_MAP + 0x0021)
// DWORD dwAttackID
// DWORD dwTargetID
// BYTE bAttackType
// BYTE bTargetType
// DWORD dwHostID
// BYTE bHostType
// DWORD dwActID
// DWORD dwAniID
// BYTE bMaintain
// DWORD dwTick
// BYTE bHit
// BYTE bAtkHit
// WORD wAttackLevel
// BYTE bAttackerLevel
// DWORD dwPysMinPower
// DWORD dwPysMaxPower
// DWORD dwMgMinPower
// DWORD dwMgMaxPower
// BYTE bCanSelect
// BYTE bCancelCharge
// BYTE bAttackCountry
// WORD wSkillID
// BYTE bSkillLevel
// BYTE bSkillPerform
// FLOAT fAtkPosX
// FLOAT fAtkPosY
// FLOAT fAtkPosZ
// FLOAT fDefPosX
// FLOAT fDefPosY
// FLOAT fDefPosZ
// BYTE bDamageCount
// {
//		BYTE bDamageType
//		WORD wDamage
// }
#define CS_HPMP_ACK					(CS_MAP + 0x0022)
// DWORD dwID
// BYTE bType
// DWORD dwMaxHP
// DWORD dwHP
// DWORD dwMaxMP
// DWORD dwMP

#define CS_LEVEL_ACK				(CS_MAP + 0x0023)
// DWORD dwCharID
// BYTE bLevel

#define CS_EXP_ACK					(CS_MAP + 0x0024)
// DWORD dwEXP
// DWORD dwPrevExp
// DWORD dwNextExp

#define CS_DIE_ACK					(CS_MAP + 0x0025)
// DWORD dwID
// BYTE bType

#define CS_REVIVAL_REQ				(CS_MAP + 0x0026)
// FLOAT fPosX
// FLOAT fPosY
// FLOAT fPosZ
// BYTE bType

#define CS_REVIVAL_ACK				(CS_MAP + 0x0027)
// DWORD dwCharID
// FLAOT fPosX
// FLOAT fPosY
// FLAOT fPosZ

#define CS_MOVEITEM_REQ				(CS_MAP + 0x0028)
// BYTE bInvenSRC
// BYTE bSRCItemID
// BYTE bInvenDEST
// BYTE bDESTItemID
// BYTE bCount

#define CS_MOVEITEM_ACK				(CS_MAP + 0x0029)
// BYTE bResult		: enum TMOVEITEM_RESULT value

#define CS_UPDATEITEM_ACK			(CS_MAP + 0x002A)
// BYTE bInvenID
// BYTE bItemID
// WORD wItemID
// BYTE bLevel
// BYTE bCount
// DWORD dwDuraMax						//내구도
// DWORD dwDuraCurrent
// BYTE bRefineMax
// BYTE bRefineCurrent
// BYTE bGLevel
// __time64_t m_dEndTime				//사용 기한
// BYTE bGradeEffect
// BYTE bWrap
// BYTE bELD
// BYTE bMagicCount
// {
// BYTE bMagicID
// WORD wValue
// DWORD dwTime
// }

#define CS_ADDITEM_ACK				(CS_MAP + 0x002B)
// BYTE bInvenID
// BYTE bItemID
// WORD wItemID
// BYTE bLevel
// BYTE bCount
// DWORD dwDuraMax						//내구도
// DWORD dwDuraCurrent
// BYTE bRefineMax
// BYTE bRefineCurrent
// BYTE bGLevel
// __time64_t m_dEndTime				//사용 기한
// BYTE bGradeEffect
// BYTE bWrap
// BYTE bELD
// BYTE bMagicCount
// {
// BYTE bMagicID
// WORD wValue
// DWORD dwTime
// }

#define CS_DELITEM_ACK				(CS_MAP + 0x002C)
// BYTE bInvenID
// BYTE bItemID

#define CS_EQUIP_ACK				(CS_MAP + 0x002D)
// DWORD dwCharID
// BYTE bItemCount
// {
// BYTE bItemID
// WORD wItemID
// BYTE bLevel
// BYTE bCount
// DWORD dwDuraMax						//내구도
// DWORD dwDuraCurrent
// BYTE bRefineMax
// BYTE bRefineCurrent
// BYTE bGLevel
// __time64_t m_dEndTime				//사용 기한
// BYTE bGradeEffect
// BYTE bWrap
// BYTE bELD
// BYTE bMagicCount
// {
// BYTE bMagicID
// WORD wValue
// DWORD dwTime
// }
// }

#define CS_ACTION_REQ				(CS_MAP + 0x002E)
// DWORD dwObjID
// BYTE bObjType
// BYTE bActionID
// DWORD dwActID
// DWORD dwAniID
// BYTE bChannel
// WORD wMapID
// WORD wSkillID

#define CS_ACTION_ACK				(CS_MAP + 0x002F)
// BYTE bResult
// DWORD dwObjID
// BYTE bObjType
// BYTE bActionID
// DWORD dwActID
// DWORD dwAniID
// WORD wSkillID

#define CS_MONATTACK_REQ			(CS_MAP + 0x0030)
// DWORD dwAttackID
// DWORD dwTargetID
// BYTE bAttackType
// BYTE bTargetType
// DWORD dwActID
// DWORD dwAniID
// BYTE bChannel
// WORD wMapID

#define CS_MONATTACK_ACK			(CS_MAP + 0x0031)
// DWORD dwAttackID
// DWORD dwTargetID
// BYTE bAttackType
// BYTE bTargetType
// WORD wSkillID

#define CS_SKILLBUY_REQ				(CS_MAP + 0x0032)
// WORD wNpcID
// WORD wSkillID
#define CS_SKILLBUY_ACK				(CS_MAP + 0x0033)
// BYTE	bRet
// WORD wSkillID
// BYTE bLevel
// DWORD dwGold
// DWORD dwSilver
// DWORD dwCooper
// WORD wMySkillPoint
// WORD wSkillPoint_1
// WORD wSkillPoint_2
// WORD wSkillPoint_3
// WORD wSkillPoint_4
#define CS_SKILLUSE_REQ				(CS_MAP + 0x0034)
// DWORD dwAttackID
// BYTE bAttackType
// BYTE bChannel
// WORD wMapID
// WORD wSkillID
// BYTE bActionID
// DWORD dwActID
// DWORD dwAniID
// FLOAT fPosX
// FLOAT fPosY
// FLOAT fPosZ
// BYTE bCount
// {
//		DWORD dwTarget
//		BYTE bType
//		BYTE bIsTarget
// }
#define CS_SKILLUSE_ACK				(CS_MAP + 0x0035)
// BYTE bResult
// DWORD dwAttackID
// BYTE bAttackType
// WORD wSkillID
// BYTE bActionID
// DWORD dwActID
// DWORD dwAniID
// BYTE bSkillLevel
// WORD wAttackLevel
// BYTE bAttackerLevel
// DWORD dwPysMinPower
// DWORD dwPysMaxPower
// DWORD dwMgMinPower
// DWORD dwMgMaxPower
// WORD wSTR
// WORD wINT
// BYTE bCurse
// BYTE	bSpecial
// BYTE bCanSelect
// BYTE bAttackCountry
// BYTE bHit
// FLOAT fPosX
// FLOAT fPosY
// FLOAT fPosZ
// BYTE bCount
// {
//		DWORD dwTargetID
//		BYTE bTargetType
// }
#define CS_SKILLEND_REQ				(CS_MAP + 0x0036)
// DWORD dwObjID
// BYTE bObjType
// DWORD dwHostID
// DWORD dwAttackID
// BYTE bAttackType
// WORD wSkillID
// WORD wMapID
// BYTE bChannelID
#define CS_SKILLEND_ACK				(CS_MAP + 0x0037)
// DWORD dwObjID
// BYTE bObjType
// WORD wSkillID
#define CS_MONKEEPOWNER_ACK			(CS_MAP + 0x0039) 
// DWORD dwMonID
// BYTE bOwnerType
// DWORD dwOwner
#define CS_PARTYADD_REQ				(CS_MAP + 0x0040)
// STRING strTarget
// BYTE bObtainType
#define CS_PARTYADD_ACK				(CS_MAP + 0x0041)
// STRING strOrigin
// STRING strTarget
// BYTE	bResult
#define CS_PARTYJOINASK_ACK			(CS_MAP + 0x0042)
// STRING strOrigin
// BYTE bObtainType
#define CS_PARTYJOIN_REQ			(CS_MAP + 0x0043)
// STRING strOrigin
// BYTE bObtainType
// BYTE bReply
#define CS_PARTYJOIN_ACK			(CS_MAP + 0x0044)
// WORD	wPartyID
// STRING strMemberName
// DWORD dwMemberID
// DWORD dwChiefID
// WORD wCommanderID
// STRING strGuildName 
// BYTE bLevel
// DWORD dwMaxHP
// DWORD dwHP
// DWORD dwMaxMP
// DWORD dwMP
// BYTE bRace
// BYTE bSex
// BYTE bFace
// BYTE bHair
// BYTE bPartyType
#define CS_PARTYDEL_REQ				(CS_MAP + 0x0045)
// DWORD dwMemberID
#define CS_PARTYDEL_ACK				(CS_MAP + 0x0046)
// DWORD dwMemberID
// DWORD dwChiefID
// WORD wCommanderID
// WORD wPartyID
// BYTE bKick
#define CS_PARTYMANSTAT_ACK			(CS_MAP + 0x0047)
// DWORD dwID
// BYTE bType
// BYTE bLevel
// DWORD dwMaxHP
// DWORD dwCurHP
// DWORD dwMaxMP
// DWORD dwCurMP
#define CS_PARTYATTR_ACK			(CS_MAP + 0x0048)
// DWORD dwPlayerID
// WORD wPartyID
// DWORD dwPartyChiefID
// WORD wCommanderID
#define CS_MONEY_ACK				(CS_MAP + 0x0049)
// DWORD dwGold
// DWORD dwSilver
// DWORD dwCooper
#define CS_ITEMUSE_REQ				(CS_MAP + 0x004A)
// WORD wTempID
// BYTE bInvenID
// BYTE bItemID
// BYTE bKind
// BYTE bCount
// {
//		DWORD dwTarget
//		BYTE bType
// }
#define CS_ITEMUSE_ACK				(CS_MAP + 0x004B)
// BYTE bResult
// BYTE bKind
// DWORD dwTick
/////////////////////////////////////////quest//////////////////////////////////////////////////
#define CS_QUESTADD_ACK						(CS_MAP + 0x0050)
// DWORD dwQuestID
// BYTE bType
#define CS_QUESTLIST_ACK					(CS_MAP + 0x0051)
// BYTE bCount
//{
//		DWORD dwQuestID
//		BYTE bType
//		BYTE bTermCount
//		{
//			DWORD dwTermID
//			BYTE bTermType
//			BYTE bNeedCount
//			BYTE bCurrentCount
//			BYTE bStatus
//		}
//}
#define CS_QUESTEXEC_REQ					(CS_MAP + 0x0052)
// DWORD dwQuestID
// BYTE bRewardType
// DWORD dwRewardID
#define CS_QUESTUPDATE_ACK					(CS_MAP + 0x0053)
// DWORD dwQuestID
// DWORD dwTermID
// BYTE bType
// BYTE bCount
// BYTE bStatus
#define CS_QUESTCOMPLETE_ACK				(CS_MAP + 0x0054)
// BYTE bResult
// DWORD dwQuestID
// DWORD dwTermID
// BYTE bType
// DWORD dwDropID
#define CS_QUESTDROP_REQ					(CS_MAP + 0x0055)
// DWORD dwQuestID
#define CS_QUESTLIST_COMPLETE_ACK			(CS_MAP + 0x0056)
// BYTE bCount
//{
// DWORD dwQuestID
//}
#define CS_QUESTSTARTTIMER_ACK				(CS_MAP + 0x0057)
// DWORD dwQuestID
// DWORD dwTick
#define CS_QUESTENDTIMER_REQ				(CS_MAP + 0x0058)
// DWORD dwQuestID
#define CS_QUESTLIST_POSSIBLE_REQ			(CS_MAP + 0x0059)
// BYTE bCount
// {
//		WORD wNpcID
// }
#define CS_QUESTLIST_POSSIBLE_ACK			(CS_MAP + 0x005A)
// BYTE bCount
// {
//		WORD wNpcID
//		BYTE bCountry
//		BYTE bCount
//		{
//			DWORD dwQuestID
//			BYTE  bQuestCON
//		}
// }
#define CS_QUESTTALK_REQ					(CS_MAP + 0x005B)
// WORD wNpcID

///////////////////// Guild /////////////////////////////////////
#define CS_GUILDESTABLISH_REQ				(CS_MAP + 0x0060)
// STRING strName
#define CS_GUILDESTABLISH_ACK				(CS_MAP + 0x0061)
// BYTE bResult
// DWORD dwGuildID
// CString strName
#define CS_GUILDDISORGANIZATION_REQ			(CS_MAP + 0x0062)
// BYTE bResult
#define CS_GUILDDISORGANIZATION_ACK			(CS_MAP + 0x0063)
// BYTE bResult
#define CS_GUILDINVITE_REQ					(CS_MAP + 0x0064)
// STRING strTargetChar
#define CS_GUILDINVITE_ACK					(CS_MAP + 0x0065)
// BYTE bResult
// STRING strGuildName
// DWORD dwInviterID
// STRING strInviterName
#define CS_GUILDINVITEANSWER_REQ			(CS_MAP + 0x0066)
// BYTE bAnswer
// DWORD dwInviterID
#define CS_GUILDJOIN_ACK					(CS_MAP + 0x0067)
// BYTE bRet
// DWORD dwGuildID
// STRING strGuildName
// DWORD dwNewMemberID
// STRING strNewMemberName
#define CS_GUILDLEAVE_REQ					(CS_MAP + 0x0068)
#define CS_GUILDKICKOUT_REQ					(CS_MAP + 0x0069)
// STRING strTarget
#define CS_GUILDLEAVE_ACK					(CS_MAP + 0x006A)
// BYTE bResult
// STRING strTarget
// BYTE bReason
#define CS_GUILDDUTY_REQ					(CS_MAP + 0x006B)
// STRING strTarget
// BYTE bDuty
#define CS_GUILDDUTY_ACK					(CS_MAP + 0x006C)
// BYTE bResult
// STRING strTarget
// BYTE bDuty
#define CS_GUILDMEMBERLIST_REQ				(CS_MAP + 0x006D)
#define CS_GUILDMEMBERLIST_ACK				(CS_MAP + 0x006E)
// BYTE bRet
// DWORD dwGuildID
// STRING strGuildName
// WORD wCount
// {
//		DWORD dwCharID
//		STRING strName
//		BYTE bLevel
//		BYTE bClass
//		BYTE bDuty
//		BYTE bPeer
//		BYTE bIsConnect
//		DWORD dwRegion					100914 bService 삭제
//		WORD wCastle
//		BYTE bCamp
//		DWORD dwTactic
//		BYTE bWarCountry				100914
//		INT64 dlConnectedDate			100914
// }
#define CS_GUILDATTR_ACK					(CS_MAP + 0x006F)
// DWORD dwPlayerID
// DWORD dwGuildID
// DWORD dwFame			: 길드 문양
// DWORD dwFameColor	: 길드 문양 색깔
// CString strGuildName
// BYTE bGuildPeer
// DWORD dwTacticsID // 용병
// CString strTacticsName
#define CS_GUILDPEER_REQ					(CS_MAP + 0x0070)
// STRING strTarget
// BYTE bPeer
#define CS_GUILDPEER_ACK					(CS_MAP + 0x0071)
// BYTE bResult
// STRING strTarget
// BYTE bPeer
#define CS_GUILDINFO_REQ					(CS_MAP + 0x0072)
#define CS_GUILDINFO_ACK					(CS_MAP + 0x0073)
// BYTE bRet
// DWORD dwGuildID
// STRING strGuildName
// STRING strEstablishTime
// WORD wMemberCount
// WORD wMaxMemberCount	: 길드전체수용인원 
// STRING strChief
// BYTE bPeer			: 길드장작위
// STRING strViceChief1
// STRING strViceChief2
// BYTE bLevel
// DWORD dwFame			: bFame->dwFame 수정
// DWORD dwFameColor	: 문양색깔
// DWORD dwGI
// DWORD dwExp
// DWORD dwNextExp		: 다음 레벨업 경험치
// BYTE bGPoint
// BYTE bStatus
// DWORD dwGold			: 길드 자금 추가
// DWORD dwSilver
// DWORD dwCooper
// BYTE bDuty			: 나의 직위 추가
// BYTE bPeer			: 나의 작위 추가			100914 // BYTE bService 삭제
// CString strArticleTitle : 최근 공지사항 제목
// DWORD dwPvPTotalPoint
// DWORD dwPvPUseablePoint
#define CS_CHAT_REQ							(CS_MAP + 0x0074)
// BYTE bGroup
// DWORD dwTargetID		
// STRING strName
// STRING strTalk
#define CS_CHAT_ACK							(CS_MAP + 0x0075)
// BYTE bGroup
// DWORD dwSenderID
// STRING strName
// STRING strTalk
#define CS_CABINETPUTIN_REQ					(CS_MAP + 0x0076)
// BYTE bCabinetID
// BYTE bInven
// BYTE bItemID
// BYTE bCount
// BYTE bNpcInvenID
// BYTE bNpcItemID
#define CS_CABINETTAKEOUT_REQ				(CS_MAP + 0x0078)
// BYTE bCabinetID
// DWORD dwStItemID
// BYTE bCount
// BYTE bInvenID
// BYTE bItemID
// BYTE bNpcInvenID
// BYTE bNpcItemID
#define CS_CABINETLIST_REQ					(CS_MAP + 0x007A)
#define CS_CABINETLIST_ACK					(CS_MAP + 0x007B)
// BYTE bCount
// {
//		BYTE bCabinetID
//		BYTE bUse
// }
#define CS_CABINETITEMLIST_REQ				(CS_MAP + 0x007C)
// BYTE bCabinetID
#define CS_CABINETITEMLIST_ACK				(CS_MAP + 0x007D)
// BYTE bResult
// BYTE bCabinetID
// DWORD bItemCount
// {
//		DWORD dwStItemID
//		WORD wItemID
//		BYTE bLevel
//		BYTE bCount
//		DWORD dwDuraMax						//내구도
//		DWORD dwDuraCurrent
//		BYTE bRefineMax
//		BYTE bRefineCurrent
//		BYTE bGLevel
//		__time64_t m_dEndTime				//사용 기한
//		BYTE bGradeEffect
//		BYTE bWrap
//		BYTE bELD
//		BYTE bMagicCount
//		{
//			BYTE bMagicID
//			WORD wValue
//			DWORD dwTime
//		}
// }
#define CS_CABINETOPEN_REQ					(CS_MAP + 0x007E)
// BYTE bCabinetID
#define CS_CABINETOPEN_ACK					(CS_MAP + 0x007F)
// BYTE bResult
// BYTE bCabinetID

#define CS_NPCITEMLIST_REQ					(CS_MAP + 0x0082)
// WORD wNpcID
#define CS_NPCITEMLIST_ACK					(CS_MAP + 0x0083)
// WORD wNpcID
// BYTE bType
// BYTE bDiscountRate
// BYTE bCount
// {
//		WORD wItemID
// }
#define CS_ITEMBUY_REQ						(CS_MAP + 0x0084)
// WORD wNpcID
// DWORD dwQuestID
// WORD wItemID
// BYTE bCount
// BYTE bNpcInvenID
// BYTE bNpcItemID
#define CS_ITEMBUY_ACK						(CS_MAP + 0x0085)
// BYTE bRet
// WORD wItemID
// DWORD dwGold
// DWORD dwSilver
// DWORD dwCooper
#define CS_ITEMSELL_REQ						(CS_MAP + 0x0086)
// BYTE bInven
// BYTE bPos
// BYTE bCount
// BYTE bNpcInvenID
// BYTE bNpcItemID
#define CS_ITEMSELL_ACK						(CS_MAP + 0x0087)
// BYTE bResult
// DWORD dwGold
// DWORD dwSilver
// DWORD dwCooper
#define CS_MONITEMLIST_REQ					(CS_MAP + 0x0088)
// BYTE bWant
// DWORD dwMonID
#define CS_MONITEMLIST_ACK					(CS_MAP + 0x0089)
// BYTE bResult
// BYTE bUpdate
// DWORD dwMonID
// DWORD dwGold
// DWORD dwSilver
// DWORD dwCooper
// BYTE bItemCount
// {
// 		BYTE bItemID
// 		WORD wItemID
//		BYTE bLevel
// 		BYTE bCount
//		DWORD dwDuraMax						//내구도
//		DWORD dwDuraCurrent
//		BYTE bRefineMax
//		BYTE bRefineCurrent
//		BYTE bGLevel
//		__time64_t m_dEndTime				//사용 기한
//		BYTE bGradeEffect
//		BYTE bWrap
//		BYTE bELD
// 		BYTE bMagicCount
// 		{
// 			BYTE bMagicID
// 			WORD wValue
// 			DWORD dwTime
// 		}
// }
#define CS_MONITEMTAKE_REQ					(CS_MAP + 0x008A)
// DWORD dwMonID
// BYTE bItemID
// BYTE bInvenID
// BYTE bSlotID
#define CS_MONITEMTAKE_ACK					(CS_MAP + 0x008B)
// BYTE bResult
#define CS_MONITEMLOTTERY_REQ				(CS_MAP + 0x008C)
// BYTE bAnswer
// DWORD dwMonID
// WORD wItemID
#define CS_MONITEMLOTTERY_ACK				(CS_MAP + 0x008D)
// DWORD dwMonID
// WORD wItemID
// BYTE bLevel
// BYTE bCount
// DWORD dwDuraMax						//내구도
// DWORD dwDuraCurrent
// BYTE bRefineMax
// BYTE bRefineCurrent
// BYTE bGLevel
// __time64_t m_dEndTime				//사용 기한
// BYTE bGradeEffect
// BYTE bWrap
// BYTE bELD
// BYTE bMagicCount
// {
// BYTE bMagicID
// WORD wValue
// DWORD dwTime
// }
#define CS_FRIENDASK_REQ					(CS_MAP + 0x008E)
// STRING strInvitee
#define CS_FRIENDASK_ACK					(CS_MAP + 0x008F)
// STRING strInviter
#define CS_FRIENDREPLY_REQ					(CS_MAP + 0x0090)
// STRING strInviter
// BYTE bReply
#define CS_FRIENDADD_ACK					(CS_MAP + 0x0091)
// BYTE bResult
// DWORD dwFriendID
// STRING strName
// BYTE bLevel
// BYTE bGroup
// BYTE bClass
// DWORD dwRegion
#define CS_FRIENDERASE_REQ					(CS_MAP + 0x0092)
// DWORD dwFriendID
#define CS_FRIENDERASE_ACK					(CS_MAP + 0x0093)
// BYTE bResult
// DWORD dwFriendID
#define CS_FRIENDLIST_ACK					(CS_MAP + 0x0094)
// DWORD dwSoulID
// if(dwSoulID != 0)
// {
// STRING strSoulName
// BYTE bSoulLevel
// BYTE bSoulClass
// BYTE bSoulConnected
// DWORD dwSoulRegion
// }
// BYTE bGroupCount
// {
//		BYTE bGroupID
//		STRING strName
// }
// BYTE bFriendCount
// {
//		DWORD dwCharID
//		STRING strName
//		BYTE bLevel
//		BYTE bGroup
//		BYTE bClass
//		BYTE bConnected
//		DWORD dwRegion
// }
#define CS_FRIENDCONNECTION_ACK				(CS_MAP + 0x0095)
// BYTE bRet
// STRING strName
// DWORD dwRegion
#define CS_FRIENDREGION_ACK					(CS_MAP + 0x0096)
// CString strName
// DWORD dwRegion

#define CS_HOTKEYADD_REQ					(CS_MAP + 0x009F)
// BYTE bType
// WORD wID
// BYTE bTargetInven
// BYTE bTargetPos
#define CS_HOTKEYDEL_REQ					(CS_MAP + 0x00A1)
// BYTE bInvenID
// BYTE bPos
#define CS_HOTKEYCHANGE_ACK					(CS_MAP + 0x00A2)
// BYTE bInvenID
// BYTE bCount
// {
//		BYTE bPos
//		BYTE bType
//		WORD wID
// }
#define CS_CHARSTATINFO_REQ					(CS_MAP + 0x00A3)
// DWORD dwCharID
#define CS_CHARSTATINFO_ACK					(CS_MAP + 0x00A4)
// DWORD dwCharID
// WORD wSTR
// WORD wDEX
// WORD wCON
// WORD wINT
// WORD wWIS
// WORD wMEN
// DWORD dwMinPysAtkPower
// DWORD dwMaxPysAtkPower
// DWORD dwPysDefPower
// DWORD dwMinLongAtkPower
// DWORD dwMaxLongAtkPower
// DWORD dwAttackDelay
// DWORD dwLongAttackDelay
// DWORD dwMAttackDelay
// DWORD dwAttackDelayR
// DWORD dwLongAttackDelayR
// DWORD dwMAttackDelayR
// WORD wAtkLevel
// WORD wDefLevel
// BYTE bPysCriticalProb
// WORD wPysResist			//삭제
// DWORD dwMinMagicAP
// DWORD dwMaxMagicAP
// DWORD dwMagicDefPower
// WORD	wMagicAtkLevel		//DWORD dwMinMagicLAP 교체
// WORD wMagicDefLevel		//DWORD dwMaxMagicLAP 교체
// BYTE bChargeSpeed
// BYTE bChargeProb
// BYTE bMagicCriticalProb
// WORD wMagicResist		//삭제
// WORD bFR		화염저항	//삭제
// WORD bIR		냉기저항	//삭제
// WORD bER		전기저항	//삭제
// WORD bSR		신성저항	//삭제
// WORD bDR		암흑저항	//삭제
// WORD wSkillPoint
// BYTE bAftermathStep	부활후유증 단계

#define CS_CANCELACTION_REQ					(CS_MAP + 0x00A5)
// DWORD dwObjID
// BYTE bObjType
#define CS_CANCELACTION_ACK					(CS_MAP + 0x00A6)
// DWORD dwObjID
// BYTE bObjType
#define CS_LOCALENABLE_ACK					(CS_MAP + 0x00A7)
// WORD wID
// BYTE bStatus
#define CS_LOCALOCCUPY_ACK					(CS_MAP + 0x00A8)
// BYTE bType
// WORD wLocalID
// BYTE bCountry
// DWORD dwGuildID
#define CS_ITEMUPGRADE_REQ					(CS_MAP + 0x00A9)
// BYTE bTargetInven
// BYTE bTargetItemID
// BYTE bGradeItemInven
// BYTE bGradeItemID
// WORD wNpcID
// BYTE bNpcInvenID
// BYTE bNpcItemID
#define CS_ITEMUPGRADE_ACK					(CS_MAP + 0x00AA)
// BYTE bResult
// BYTE bTargetInven
// BYTE bTargetItemID
// BYTE bGrade								//0316 bResult==ITEMUPGRADE_SUCCESS_WRAP?TRUE:bGrade, bResult==ITEMUPGRADE_SUCCESS_ELD?bDownValue:bGrade
// BYTE bGradeEffect						//0316 bResult==ITEMUPGRADE_SUCCESS||ITEMUPGRADE_DOWNGRADE?유효한값:pass
// WORD wColor								//090922 bResult==ITEMUPGRADE_SUCCESS_COLOR ? 유효한값 : pass
#define CS_ITEMMAGICGRADE_ACK				(CS_MAP + 0x00AB)
// BYTE bResult
// BYTE bTargetInven
// BYTE bTargetItemID
// BYTE bMagicCount
// {
// BYTE bMagicID
// WORD wValue
// DWORD dwTime								//0316 삭제
// }

#define CS_CORPSASK_REQ						(CS_MAP + 0x00AC)
// STRING strTargetName

#define CS_CORPSASK_ACK						(CS_MAP + 0x00AD)
// STRING strReqName

#define CS_CORPSREPLY_REQ					(CS_MAP + 0x00AE)
// BYTE bReply
// STRING strReqName

#define CS_CORPSREPLY_ACK					(CS_MAP + 0x00AF)
// BYTE bResult
// STRING strTargetName

#define CS_CORPSLEAVE_REQ					(CS_MAP + 0x00B0)
// WORD wSquadID

#define CS_CORPSJOIN_ACK					(CS_MAP + 0x00B1)
// WORD wCorpsID
// WORD wCommanderID

#define CS_CORPSCMD_REQ						(CS_MAP + 0x00B2)
// WORD wSquadID
// DWORD dwCharID
// WORD wMapID
// BYTE bCMD
// DWORD dwTargetID
// BYTE bTargetType
// WORD wTargetX
// WORD wTargetZ

#define CS_CORPSCMD_ACK						(CS_MAP + 0x00B3)
// WORD wSquadID
// DWORD dwCharID
// WORD wMapID
// BYTE bCMD
// DWORD dwTargetID
// BYTE bTargetType
// WORD wTargetX
// WORD wTargetZ

#define CS_CORPSENEMYLIST_REQ				(CS_MAP + 0x00B4)
// WORD wSquadID
// DWORD dwCharID
// BYTE bCount
// {
// 		DWORD dwEnemyID
// 		BYTE bEnemyType
// 		FLOAT fSPEED
//		WORD wMapID
// 		WORD wPosX
// 		WORD wPosZ
// 		WORD wDIR
// 		BYTE bReporterCount
// 		{
// 			DWORD dwReporterID
// 		}
// }

#define CS_CORPSENEMYLIST_ACK				(CS_MAP + 0x00B5)
// BYTE bCount
// {
// 		DWORD dwEnemyID
// 		BYTE bEnemyType
// 		FLOAT fSPEED
//		WORD wMapID
// 		WORD wPosX
// 		WORD wPosZ
// 		WORD wDIR
// 		BYTE bReporterCount
// 		{
// 			DWORD dwReporterID
// 		}
// }

#define CS_MOVECORPSENEMY_REQ				(CS_MAP + 0x00B6)
// DWORD dwEnemyID
// BYTE bEnemyType
// FLOAT fSPEED
// WORD wMapID
// WORD wPosX
// WORD wPosZ
// WORD wDIR

#define CS_MOVECORPSENEMY_ACK				(CS_MAP + 0x00B7)
// DWORD dwEnemyID
// BYTE bEnemyType
// FLOAT fSPEED
// WORD wMapID
// WORD wPosX
// WORD wPosZ
// WORD wDIR

#define CS_MOVECORPSUNIT_REQ				(CS_MAP + 0x00B8)
// WORD wSquadID
// DWORD dwCharID
// FLOAT fSPEED
// WORD wMapID
// WORD wPosX
// WORD wPosZ
// WORD wDIR

#define CS_MOVECORPSUNIT_ACK				(CS_MAP + 0x00B9)
// WORD wSquadID
// DWORD dwCharID
// FLOAT fSPEED
// WORD wMapID
// WORD wPosX
// WORD wPosZ
// WORD wDIR

#define CS_ADDCORPSENEMY_REQ				(CS_MAP + 0x00BA)
// DWORD dwReporterID
// DWORD dwEnemyID
// BYTE bEnemyType
// FLOAT fSPEED
// WORD wMapID
// WORD wPosX
// WORD wPosZ
// WORD wDIR

#define CS_ADDCORPSENEMY_ACK				(CS_MAP + 0x00BB)
// DWORD dwReporterID
// DWORD dwEnemyID
// BYTE bEnemyType
// FLOAT fSPEED
// WORD wMapID
// WORD wPosX
// WORD wPosZ
// WORD wDIR

#define CS_DELCORPSENEMY_REQ				(CS_MAP + 0x00BC)
// DWORD dwReporterID
// DWORD dwEnemyID
// BYTE bEnemyType

#define CS_DELCORPSENEMY_ACK				(CS_MAP + 0x00BD)
// DWORD dwReporterID
// DWORD dwEnemyID
// BYTE bEnemyType

#define CS_ADDCORPSUNIT_ACK					(CS_MAP + 0x00BE)
// WORD wSquadID
// DWORD dwCharID
// STRING strNAME
// FLOAT fSPEED
// DWORD dwTargetID
// DWORD dwMaxHP
// DWORD dwHP
// WORD wTargetX
// WORD wTargetZ
// WORD wMapID
// WORD wPosX
// WORD wPosZ
// WORD wDIR
// BYTE bTargetType
// BYTE bLevel
// BYTE bClass
// BYTE bRaceID
// BYTE bSexID
// BYTE bFaceID
// BYTE bHairID
// BYTE bTCMD

#define CS_DELCORPSUNIT_ACK					(CS_MAP + 0x00BF)
// WORD wSquadID
// DWORD dwCharID

#define CS_ADDSQUAD_ACK						(CS_MAP + 0x00C0)
// DWORD dwChiefID
// WORD wSquadID
// BYTE bCount
// {
// DWORD dwCharID
// STRING strNAME
// FLOAT fSPEED
// DWORD dwTargetID
// DWORD dwMaxHP
// DWORD dwHP
// WORD wTargetX
// WORD wTargetZ
// WORD wMapID
// WORD wPosX
// WORD wPosZ
// WORD wDIR
// BYTE bTargetType
// BYTE bLevel
// BYTE bClass
// BYTE bRaceID
// BYTE bSexID
// BYTE bFaceID
// BYTE bHairID
// BYTE bTCMD
// }

#define CS_DELSQUAD_ACK						(CS_MAP + 0x00C1)
// WORD wSquadID

#define CS_FRIENDGROUPMAKE_REQ				(CS_MAP + 0x00C2)
// BYTE bGroup
// CString strName
#define CS_FRIENDGROUPMAKE_ACK				(CS_MAP + 0x00C3)
// BYTE bRet
// BYTE bGroupID
// CString strName
#define CS_FRIENDGROUPDELETE_REQ			(CS_MAP + 0x00C4)
// BYTE bGroupID
#define CS_FRIENDGROUPDELETE_ACK			(CS_MAP + 0x00C5)
// BYTE bRet
// BYTE bGroupID
#define CS_FRIENDGROUPCHANGE_REQ			(CS_MAP + 0x00C6)
// DWORD dwFriend
// BYTE bWantedGroup
#define CS_FRIENDGROUPCHANGE_ACK			(CS_MAP + 0x00C7)
// BYTE bRet
// BYTE bGroup
// DWORD dwFriend
#define CS_FRIENDGROUPNAME_REQ				(CS_MAP + 0x00C8)
// BYTE bGroup
// STRING strName
#define CS_FRIENDGROUPNAME_ACK				(CS_MAP + 0x00C9)
// BYTE bRet
// BYTE bGroup
// STRING strName
#define CS_TMSSEND_REQ						(CS_MAP + 0x00CA)
// DWORD dwTMS
// STRING strMessage
#define CS_TMSRECV_ACK						(CS_MAP + 0x00CB)
// DWORD dwTMS
// STRING strSender
// STRING strMessage
#define CS_TMSINVITE_REQ					(CS_MAP + 0x00CC)
// DWORD dwInviter
// DWORD dwTMS
// BYTE bCount
// {
//		DWORD dwTarget
// }
#define CS_TMSINVITE_ACK					(CS_MAP + 0x00CD) // --메신져추가
// DWORD dwTMS
// BYTE bCount
// {
//		DWORD dwCharID
//		STRING strName
//		BYTE bClass
//		BYTE bLevel
// }
#define CS_TMSOUT_REQ						(CS_MAP + 0x00D0)
// DWORD dwTMS
#define CS_TMSOUT_ACK						(CS_MAP + 0x00D1)
// DWORD dwTMS
// STRING strName
#define CS_CASTLEENABLE_ACK					(CS_MAP + 0x00D2)
// BYTE bCastleID
// BYTE bStatus
#define CS_CASTLEAPPLY_REQ					(CS_MAP + 0x00D3)
// WORD wCastleID							//공성전
// DWORD dwTarget
#define CS_CASTLEAPPLY_ACK					(CS_MAP + 0x00D4)
// BYTE bResult
// WORD wCastle					//공성전
// DWORD dwTarget				//공성전
// BYTE	bCamp					//공성전
#define CS_KICKOUT_REQ						(CS_MAP + 0x00D5)
// DWORD dwCharID
#define CS_ANNOUNCEMENTCHAT_ACK				(CS_MAP + 0x00D6)
// CString strAnnounce
#define CS_TELEPORT_REQ						(CS_MAP + 0x00D7)
// WORD wNpcID
// WORD wPosID
#define CS_TELEPORT_ACK						(CS_MAP + 0x00D8)
// BYTE bResult
// DWORD dwObjID
// BYTE bObjType
// DWORD dwRange
// WORD wMapID
// WORD wPosX
// WORD wPosY
// WORD wPosZ
#define CS_ADDRECALLMON_ACK					(CS_MAP + 0x00D9)
// DWORD dwHostID
// DWORD dwMonID
// WORD wTemplateID
// WORD wPetID
// CString strName
// BYTE bCountry
// BYTE bColor
// BYTE bLevel
// DWORD dwMaxHP
// DWORD dwHP
// DWORD dwMaxMP
// DWORD dwMP
// FLOAT fPosX
// FLOAT fPosY
// FLOAT fPosZ
// WORD wPitch
// WORD wDIR
// BYTE bMouseDIR
// BYTE bKeyDIR
// BYTE bAction
// BYTE bMode
// BYTE bNewMember
// DWORD dwRegionID
// BYTE bRecallType
// BYTE bRecallHIT
// BYTE bSkillLevel
// WORD wAttackLevel
// BYTE bAttackerLevel
// DWORD dwPysMinPower
// DWORD dwPysMaxPower
// DWORD dwMgMinPower
// DWORD dwMgMaxPower
// DWORD dwLifeTime
// BYTE bMaintainSkillCount
// {
//		WORD wSkillID
//		BYTE bLevel
//		DWORD dwTick
//		DWORD dwAttackID
//		BYTE bAttackType
//		DWORD dwHostID
//		BYTE bHostType
//		BYTE bAtkHit
//		WORD wAttackLevel
//		BYTE bAttackerLevel
//		DWORD dwPysMinPower
//		DWORD dwPysMaxPower
//		DWORD dwMgMinPower
//		DWORD dwMgMaxPower
//		BYTE bCanSelect
//		BYTE bAttackCountry
//		FLOAT fPosX
//		FLOAT fPosY
//		FLOAT fPosZ
// }
#define CS_DELRECALLMON_ACK					(CS_MAP + 0x00DA)
// DWORD dwHostID
// DWORD dwMonID
// BYTE bExitMap
#define CS_CASTLEOCCUPY_ACK					(CS_MAP + 0x00DB)
// BYTE bType
// BYTE bCastle
// DWORD dwGuildID
#define CS_MONSETSKILL_ACK					(CS_MAP + 0x00DC)
// DWORD dwMonID
// WORD wSkillID
// WORD wAtkRange
#define CS_NPCTALK_REQ						(CS_MAP + 0x00DD)
// WORD wNpcID
#define CS_NPCTALK_ACK						(CS_MAP + 0x00DE)
// DWORD dwQuestID
// WORD wNpcID
#define CS_CHANGECOLOR_ACK					(CS_MAP + 0x00DF)
// BYTE bType
// DWORD dwObjID
// BYTE bColor
// BYTE bCountry
#define CS_CHAPTERMSG_ACK					(CS_MAP + 0x00E0)
// DWORD dwQuestID
#define CS_DELRECALLMON_REQ					(CS_MAP + 0x00E1)
// DWORD dwMonID
#define CS_CHGMODERECALLMON_REQ				(CS_MAP + 0x00E2)
// DWORD dwMonID
// BYTE bMode
#define CS_INVENADD_REQ						(CS_MAP + 0x00E3)
// BYTE bDESInvenID
// BYTE bSRCInvenID
// BYTE bSRCItemID
#define CS_INVENADD_ACK						(CS_MAP + 0x00E4)
// BYTE bResult
// BYTE bInvenID
// WORD wItemID
// __time64_t m_dEndTime				//사용 기한
#define CS_INVENDEL_REQ						(CS_MAP + 0x00E5)
// BYTE bSRCInven
// BYTE bDESInven
// BYTE bPosID
#define CS_INVENDEL_ACK						(CS_MAP + 0x00E6)
// BYTE bResult
// BYTE bInvenID
#define CS_INVENMOVE_REQ					(CS_MAP + 0x00E7)
// BYTE bSRCInven
// BYTE bDESInven
#define CS_INVENMOVE_ACK					(CS_MAP + 0x00E8)
// BYTE bResult
// BYTE bSRCInven
// BYTE bDESInven
#define CS_RESETHOST_REQ					(CS_MAP + 0x00E9)
// DWORD dwMonID
// BYTE bChannel
// WORD wMapID
#define CS_GATEADD_ACK						(CS_MAP + 0x00EA)
// DWORD dwGateID
// BYTE bOpened
#define CS_GATEDEL_ACK						(CS_MAP + 0x00EB)
// DWORD dwGateID
#define CS_GATECHANGE_ACK					(CS_MAP + 0x00EC)
// DWORD dwGateID
// BYTE bOpened
#define CS_SWITCHADD_ACK					(CS_MAP + 0x00ED)
// DWORD dwSwitchID
// BYTE bOpened
#define CS_SWITCHDEL_ACK					(CS_MAP + 0x00EE)
// DWORD dwSwitchID
#define CS_SWITCHCHANGE_REQ					(CS_MAP + 0X00EF)
// DWORD dwSwitchID
#define CS_SWITCHCHANGE_ACK					(CS_MAP + 0x00F0)
// DWORD dwSwitchID
// BYTE bOpened
#define CS_REGION_REQ						(CS_MAP + 0x00F1)
// DWORD dwObjID
// BYTE bObjType
// BYTE bChannel
// WORD wMapID
// DWORD dwRegionID
// WORD wLocalID
#define CS_LOOPSKILL_REQ					(CS_MAP + 0x00F2)
// DWORD dwAttackID
// BYTE bAttackType
// BYTE bChannel
// WORD wMapID
// WORD wSkillID
// FLOAT fPosX
// FLOAT fPosY
// FLOAT fPosZ
// BYTE bCount
// {
//		DWORD dwTarget
//		BYTE bType
//		BYTE bIsTarget
// }
#define CS_LOOPSKILL_ACK					(CS_MAP + 0x00F3)
// BYTE bResult
// DWORD dwAttackID
// BYTE bAttackType
// WORD wSkillID
// BYTE bSkillLevel
// WORD wAttackLevel
// BYTE bAttackerLevel
// DWORD dwPysMinPower
// DWORD dwPysMaxPower
// DWORD dwMgMinPower
// DWORD dwMgMaxPower
// BYTE bHit
// BYTE bCanSelect
// BYTE bAttackCountry
// FLOAT fPosX
// FLOAT fPosY
// FLOAT fPosZ
// BYTE bCount
// {
//		DWORD dwTargetID
//		BYTE bTargetType
// }
#define CS_ADDSELFOBJ_ACK					(CS_MAP + 0x00F4)
// DWORD dwHostID
// DWORD dwMonID
// WORD wTemplateID
// BYTE bCountry
// BYTE bColor
// BYTE bLevel
// DWORD dwMaxHP
// DWORD dwHP
// DWORD dwMaxMP
// DWORD dwMP
// FLOAT fPosX
// FLOAT fPosY
// FLOAT fPosZ
// WORD wPitch
// WORD wDIR
// BYTE bAction
// BYTE bMode
// BYTE bNewMember
// DWORD dwRegionID
// BYTE bRecallType
// BYTE bRecallHIT
// BYTE bSkillLevel
// WORD wAttackLevel
// BYTE bAttackerLevel
// DWORD dwPysMinPower
// DWORD dwPysMaxPower
// DWORD dwMgMinPower
// DWORD dwMgMaxPower
// DWORD dwLifeTime
// BYTE bMaintainSkillCount
// {
//		WORD wSkillID
//		BYTE bLevel
//		DWORD dwTick
//		DWORD dwAttackID
//		BYTE bAttackType
//		DWORD dwHostID
//		BYTE bHostType
//		BYTE bHit
//		WORD wAttackLevel
//		BYTE bAttackerLevel
//		DWORD dwPysMinPower
//		DWORD dwPysMaxPower
//		DWORD dwMgMinPower
//		DWORD dwMgMaxPower
//		BYTE bCanSelect
//		BYTE bAttackCountry
//		FLOAT fPosX
//		FLOAT fPosY
//		FLOAT fPosZ
// }
#define CS_DELSELFOBJ_ACK					(CS_MAP + 0x00F5)
// DWORD dwMonID
// BYTE bExitMap

#define CS_PROTECTEDLIST_ACK				(CS_MAP + 0x00F6)
// BYTE bCount
// {
//		DWORD dwProtected
//		STRING strName
//		BYTE bOption						//차단
// }
#define CS_PROTECTEDADD_REQ					(CS_MAP + 0x00F7)
// STRING strProtected
#define CS_PROTECTEDADD_ACK					(CS_MAP + 0x00F8)
// BYTE bRet
// DWORD dwCharID
// STRING strProtected
// BYTE bOption								//차단
#define CS_PROTECTEDERASE_REQ				(CS_MAP + 0x00F9)
// STRING strProtected
#define CS_PROTECTEDERASE_ACK				(CS_MAP + 0x00FA)
// BYTE bRet
// STRING strProtected

#define CS_REVIVALASK_REQ					(CS_MAP + 0x00FB)
// BYTE bReply
// DWORD dwAttackerID
// BYTE bAttackerType
// WORD wSkillID
// BYTE bLevel
#define CS_REVIVALASK_ACK					(CS_MAP + 0x00FC)
// DWORD dwAttackerID
// BYTE bAttackerType
// WORD wSkillID
// BYTE bLevel

#define CS_CORPSHP_REQ						(CS_MAP + 0x00FD)
// WORD wSquadID
// DWORD dwCharID
// DWORD dwMaxHP
// DWORD dwHP

#define CS_CORPSHP_ACK						(CS_MAP + 0x00FE)
// WORD wSquadID
// DWORD dwCharID
// DWORD dwMaxHP
// DWORD dwHP

#define CS_PARTYMOVE_REQ					(CS_MAP + 0x00FF)
// CString strTargetName
// CString strDestName
// WORD wDestPartyID

#define CS_PARTYMOVE_ACK					(CS_MAP + 0x0100)
// BYTE bResult

#define CS_CHGPARTYCHIEF_REQ				(CS_MAP + 0x0101)
// DWORD dwTargetID

#define CS_CHGPARTYCHIEF_ACK				(CS_MAP + 0x0102)
// BYTE bRET

#define CS_CHGCORPSCOMMANDER_REQ			(CS_MAP + 0x0103)
// WORD wPartyID

#define CS_CHGCORPSCOMMANDER_ACK			(CS_MAP + 0x0104)
// BYTE bRET

#define CS_REPORTENEMYLIST_ACK				(CS_MAP + 0x0105)
// WORD wSquadID
// DWORD dwCharID

#define CS_CHGSQUADCHIEF_ACK				(CS_MAP + 0x0106)
// WORD wSquadID
// DWORD dwChiefID

#define CS_REVIVALREPLY_ACK					(CS_MAP + 0x0107)
// BYTE bReply
// DWORD dwDefenderID

#define CS_DROPDAMAGE_REQ					(CS_MAP + 0x0108)
// DWORD dwID
// BYTE bType
// DWORD dwDamage
// BYTE bChannelID
// WORD wMapID

#define CS_DROPDAMAGE_ACK					(CS_MAP + 0x0109)
// DWORD dwID
// BYTE bType
// DWORD dwDamage

#define CS_SETRETURNPOS_REQ					(CS_MAP + 0x010A)
// WORD wNpcID

#define CS_SETRETURNPOS_ACK					(CS_MAP + 0x010B)
// BYTE bResult

#define CS_DEALITEMASK_REQ					(CS_MAP + 0x010C)
// STRING strTarget

#define CS_DEALITEMASK_ACK					(CS_MAP + 0x010D)
// STRING strInviter

#define CS_DEALITEMRLY_REQ					(CS_MAP + 0x010E)
// BYTE bReply
// STRING strInviter

#define CS_DEALITEMSTART_ACK				(CS_MAP + 0x010F)

#define CS_DEALITEMADD_REQ					(CS_MAP + 0x0110)
// DWORD dwGold
// DWORD dwSilver
// DWORD dwCooper
// BYTE bItemCount
// {
//		BYTE bInvenID
//		BYTE bItemID
// }

#define CS_DEALITEMADD_ACK					(CS_MAP + 0x0111)
// DWORD dwGold
// DWORD dwSilver
// DWORD dwCooper
// BYTE bItemCount
// {
//		WORD wItemID
//		BYTE bLevel
//		BYTE bCount
//		DWORD dwDuraMax						//내구도
//		DWORD dwDuraCurrent
//		BYTE bRefineMax
//		BYTE bRefineCurrent
//		BYTE bGLevel
//		__time64_t m_dEndTime				//사용 기한
//		BYTE bGradeEffect
//		BYTE bWrap
//		BYTE bELD
//		BYTE bMagicCount
//		{
//			BYTE bMagicID
//			WORD wValue
//			DWORD dwTime
//		}
// }

#define CS_DEALITEM_REQ						(CS_MAP + 0x0112)
// BYTE bOkey

#define CS_DEALITEMEND_ACK					(CS_MAP + 0x0113)
// BYTE bResult
// STRING strTarget

#define CS_MONMONEYTAKE_REQ					(CS_MAP + 0x0114)
// DWORD dwMonID

#define CS_STOREOPEN_REQ					(CS_MAP + 0x0119)
// STRING strName
// BYTE bCount
// {
// DWORD dwGold
// DWORD dwSilver
// DWORD dwCooper
// BYTE bInvenID
// BYTE bItemID
// BYTE bItemCount
// }
#define CS_STOREOPEN_ACK					(CS_MAP + 0x011A)
// BYTE bResult
// DWORD dwCharID
// STRING strName
#define CS_STORECLOSE_REQ					(CS_MAP + 0x011B)
#define CS_STORECLOSE_ACK					(CS_MAP + 0x011C)
// BYTE bResult
// DWORD dwCharID
#define CS_STOREITEMLIST_REQ				(CS_MAP + 0x011D)
// STRING strTarget
#define CS_STOREITEMLIST_ACK				(CS_MAP + 0x011E)
// DWORD dwCharID
// STRING strName
// BYTE bCount
// {
// BYTE bItem
// DWORD dwGold
// DWORD dwSilver
// DWORD dwCooper
// WORD wItemID
// BYTE bLevel
// BYTE bItemCount
// DWORD dwDuraMax						//내구도
// DWORD dwDuraCurrent
// BYTE bRefineMax
// BYTE bRefineCurrent
// BYTE bGLevel
// __time64_t m_dEndTime				//사용 기한
// BYTE bGradeEffect
// BYTE bWrap
// BYTE bELD
// BYTE bMagicCount
//		{
//			BYTE bMagic
//			WORD wValue
//			DWORD dwTime
//		}
// }
#define CS_STOREITEMBUY_REQ					(CS_MAP + 0x011F)
// STRING strTarget
// BYTE bItem
// BYTE bItemCount
#define CS_STOREITEMBUY_ACK					(CS_MAP + 0x0120)
// BYTE bResult
// WORD wItemID
// BYTE bCount
#define CS_STOREITEMSELL_ACK				(CS_MAP + 0x0121)
// BYTE bItem
// BYTE bCount

#define CS_POSTSEND_REQ						(CS_MAP + 0x0122)
// STRING strTarget
// STRING strTitle
// STRING strMessage
// BYTE bType
// DWORD dwGold
// DWORD dwSilver
// DWORD dwCooper
// BYTE bInvenID
// BYTE bSlotID
#define CS_POSTSEND_ACK						(CS_MAP + 0x0123)
// BYTE bResult
#define CS_POSTRECV_ACK						(CS_MAP + 0x0124)
//	DWORD dwPostID
//	BYTE bRead
//	BYTE bType
//	STRING strSender
//	STRING strTitle
//	__int64 ldwTime

#define CS_POSTLIST_ACK						(CS_MAP + 0x0125)
// WORD wTotalCount							//0201
// WORD wNotReadCount
// WORD wPage
// WORD wCount
// {
//		DWORD dwPostID
//		BYTE bRead
//		BYTE bType
//		STRING strSender
//		STRING strTitle
//		__int64 nTime
// }
#define CS_POSTVIEW_REQ						(CS_MAP + 0x0126)
// DWORD dwPostID
#define CS_POSTVIEW_ACK						(CS_MAP + 0x0127)
// DWORD dwPostID
// BYTE bRead				// if(bRead == FALSE) 대금청구 
// STRING strMessage
// DWORD dwGold
// DWORD dwSilver
// DWORD dwCooper
// BYTE bItemCount
//{
//		WORD wItemID
//		BYTE bLevel
//		BYTE bCount
//		DWORD dwDuraMax						//내구도
//		DWORD dwDuraCurrent
//		BYTE bRefineMax
//		BYTE bRefineCurrent
//		BYTE bGLevel
//		__time64_t m_dEndTime				//사용 기한
//		BYTE bGradeEffect
//		BYTE bWrap
//		BYTE bELD
//		BYTE bMagicCount
//	{
//		BYTE bMagic
//		WORD wValue
//		DWORD dwTime
//	}
//}
#define CS_POSTDEL_REQ						(CS_MAP + 0x0128)
// DWORD dwPostID
#define CS_POSTDEL_ACK						(CS_MAP + 0x0129)
// DWORD dwPostID
#define CS_POSTGETITEM_REQ					(CS_MAP + 0x012A)
// DWORD dwPostID
#define CS_POSTGETITEM_ACK					(CS_MAP + 0x012B)
// BYTE bResult
#define CS_POSTRETURN_REQ					(CS_MAP + 0x012C)
// DWORD dwPostID
// BYTE bReturnType
#define CS_POSTRETURN_ACK					(CS_MAP + 0x012D)
// BYTE bResult

#define CS_PETMAKE_REQ						(CS_MAP + 0x012E)
// BYTE bInven
// BYTe bSlot
// CString szName
#define CS_PETMAKE_ACK						(CS_MAP + 0x012F)
// BYTE bResult
// WORD wPetID
// CString strName
// __int64 ldwTime
#define CS_PETDEL_REQ						(CS_MAP + 0x0130)
// WORD wPetID
#define CS_PETDEL_ACK						(CS_MAP + 0x0131)
// BYTE bResult
// WORD wPetID
#define CS_PETLIST_ACK						(CS_MAP + 0x0132)
// BYTE bCount
// {
//		WORD wPetID
//		CString strName
//		__int64 ldwTime
// }
#define CS_PETRECALL_REQ					(CS_MAP + 0x0133)
// WORD wPetID
#define CS_PETRECALL_ACK					(CS_MAP + 0x0134)
// BYTE bResult
#define CS_PETRIDING_REQ					(CS_MAP + 0x0135)
// DWORD dwMonID
// BYTE bAction
#define CS_PETRIDING_ACK					(CS_MAP + 0x0136)
// BYTE bResult
// DWORD dwCharID
// DWORD dwMonID
// BYTE bAction
#define CS_OTHERSELF_REQ					(CS_MAP + 0x0137)
// DWORD dwHostID
// DWORD dwOtherSelfMonID
#define CS_OTHERSELF_ACK					(CS_MAP + 0x0138)
// DWORD dwOtherSelfMonID
// DWORD dwCharID
// STRING strNAME
// DWORD dwGuildID
// DWORD dwFame			: 길드문양
// DWORD dwFameColor	: 길드문양색깔
// STRING strGuildName
// BYTE bGuildPeer
// DWORD dwTacticsID // 용병
// STRING strTacticsName
// BYTE bClass
// BYTE bRace
// BYTE bCountry
// BYTE bAidCountry		100914
// BYTE bSex
// BYTE bHair
// BYTE bFace
// BYTE bBody
// BYTE bPants
// BYTE bHand
// BYTE bFoot
// BYTE bHelmetHide		: 투구숨기기
// BYTE bLevel
// DWORD dwPartyChiefID
// WORD wPartyID
// WORD wCommanderID
// BYTE bItemCount
// {
//	BYTE bItemID
//	WORD wItemID
//	BYTE bLevel
//	BYTE bCount
//	DWORD dwDuraMax						//내구도
//	DWORD dwDuraCurrent
//	BYTE bRefineMax
//	BYTE bRefineCurrent
//	BYTE bGLevel
// __time64_t m_dEndTime				//사용 기한
//	BYTE bGradeEffect
//  BYTE bWrap
//  BYTE bELD
//	BYTE bMagicCount
//	{
//		BYTE bMagicID
//		WORD wValue
//		DWORD dwTime
//	}
// }
#define CS_BEGINTELEPORT_ACK				(CS_MAP + 0x0139)
// BYTE bChannel

#define CS_SHUTDOWN_ACK						(CS_MAP + 0x013B)
#define CS_DISCONNECT_REQ					(CS_MAP + 0x013C)
#define CS_CHGCHANNEL_REQ					(CS_MAP + 0x013D)
// BYTE bChannel
#define CS_CHGCHANNEL_ACK					(CS_MAP + 0x013E)
// BYTE bResult
#define CS_TESTMSG_ACK						(CS_MAP + 0x013F)
// BYTE bResult

#define CS_CHGPARTYTYPE_REQ					(CS_MAP + 0x0140)
// BYTE bPartyType
#define CS_CHGPARTYTYPE_ACK					(CS_MAP + 0x0141)
// BYTE bRet
// BYTE bPartyType

#define CS_PARTYITEMTAKE_ACK				(CS_MAP + 0x0142)
// DWORD dwCharID
// WORD wItemID
// BYTE bLevel
// BYTE bCount
// DWORD dwDuraMax						//내구도
// DWORD dwDuraCurrent
// BYTE bRefineMax
// BYTE bRefineCurrent
// BYTE bGLevel
// __time64_t m_dEndTime				//사용 기한
// BYTE bGradeEffect
// BYTE bWrap
// BYTE bELD
// BYTE bMagicCount
// {
// BYTE bMagicID
// WORD wValue
// DWORD dwTime
// }

#define CS_GETITEM_ACK						(CS_MAP + 0x0144)
// WORD wItemID
// BYTE bLevel
// BYTE bCount
// DWORD dwDuraMax						//내구도
// DWORD dwDuraCurrent
// BYTE bRefineMax
// BYTE bRefineCurrent
// BYTE bGLevel
// __time64_t m_dEndTime				//사용 기한
// BYTE bGradeEffect
// BYTE bWrap
// BYTE bELD
// BYTE bMagicCount
// {
// BYTE bMagicID
// WORD wValue
// DWORD dwTime
// }

#define CS_EXITMAP_REQ						(CS_MAP + 0x0145)

#define CS_CANCELSKILL_REQ					(CS_MAP + 0x0146)
// WORD	wSkillID

#define CS_DUELINVITE_REQ					(CS_MAP + 0x0147) // 결투초대
// DWORD dwTarget
#define CS_DUELINVITE_ACK					(CS_MAP + 0x0148) // 결투초대
// DWORD dwInviter
#define CS_DUELINVITEREPLY_REQ				(CS_MAP + 0x0149) // 결투초대응답
// BYTE bResult
// DWORD dwInviter
#define CS_DUELSTART_ACK					(CS_MAP + 0x014A) // 결투시작
// BYTE bResult
// DWORD dwInviter
// DWORD dwTarget
#define CS_DUELEND_REQ						(CS_MAP + 0x014B) // 결투포기
#define CS_DUELEND_ACK						(CS_MAP + 0x014C) // 결투종료
// DWORD dwLoser
#define CS_DUELSTANDBY_ACK					(CS_MAP + 0x014D) // 결투대기상태
// DWORD dwInviter
// DWORD dwTarget
// FLOAT fCenterX
// FLOAT fCenterZ

#define CS_GETTARGET_REQ					(CS_MAP + 0x014E) // 타겟 요청
// DWORD dwCharID
#define CS_GETTARGET_ACK					(CS_MAP + 0x014F) // 타겟 요청
// DWORD dwTargetID
// BYTE bTargetType
#define CS_GETTARGETANS_REQ					(CS_MAP + 0x0150) // 타겟 요청 응답
// DWORD dwAnswerCharID
// DWORD dwTargetID
// BYTE bTargetType
#define CS_GETTARGETANS_ACK					(CS_MAP + 0x0151) // 타겟 요청 응답
// DWORD dwAnswerCharID
#define CS_MONITEMTAKEALL_REQ				(CS_MAP + 0x0152)
// DWORD dwMonID
#define CS_SMSSEND_REQ						(CS_MAP + 0x0153)
// BYTE bInvenID
// BYTE bItemID
// BYTE bType	: 1 - 개인, 2 - 길드
// STRING strTargetName
// STRING strMessage
#define CS_SMSSEND_ACK						(CS_MAP + 0x0154)
// BYTE bResult
// BYTE bType	: 1 - 개인, 2 - 길드
// STRING TargetName

// < 길드 영토 성 / 소속 점령지 보기 > 조건 : 길드 UI내에서
#define CS_GUILDLOCALLIST_REQ				(CS_MAP + 0x0155)
#define CS_GUILDLOCALLIST_ACK				(CS_MAP + 0x0156)
// WORD wCount	: 성 목록 수
// {
// 		WORD wCastle			: 성아이디
// 		CString strCastleName	: 성이름
//		BYTE bCanApplyCastle	: 공성전 참가 신청 가능		//공성전
//		DWORD dwGuildID			: 점령 길드
//		CString strGuildName	: 점령 길드 이름
//		BYTE bCountry			: 소속 국가
//		INT64 dlNextWar			: 다음 공성 시간			//공성전
//		CString strHero			: 공성 영웅					//공성전
//		CString strAtkGuild		: 공격측 길드				//공성전
//		WORD wDefGuildPoint
//		BYTE bDefCount
//		WORD wAtkGuildPoint
//		WORD wAtkPoint
//		BYTE bAtkCount
//		WORD wMyGuildPoint
//		WORD wLocalCnt			: 점령지 목록수
//		{
//			WORD wLocal				: 점령지아이디
// 			CString strLocalName	: 점령지이름
//			DWORD dwGuildID			: 점령 길드
//			CString strGuildName	: 점령 길드 이름
//			BYTE bCountry			: 소속 국가
//			INT64 dlNextWar
//			CString strHero
//		}
// }
// BYTE bMissionCount
// {
//		WORD wMissionID
//		CString strName
//		BYTE bCountry
//		INT64 dlNextWar
// }

// < 길드 소유 영토 국가 반납 > 조건 : 길드 UI내에서
#define CS_GUILDLOCALRETURN_REQ				(CS_MAP + 0x0157)
// WORD wID		: 소유 성 (유닛아이디) / 점령지(로컬아이디)
#define CS_GUILDLOCALRETURN_ACK				(CS_MAP + 0x0158)
// BYTE bResult

// < 길드 창고 목록 >
#define CS_GUILDCABINETLIST_REQ				(CS_MAP + 0x0159)
#define CS_GUILDCABINETLIST_ACK				(CS_MAP + 0x015A)
// BYTE bMax	: 전체칸수
// BYTE bCount	: 창고 칸수
// {
//		DWORD dwItemID : 창고에서 아이탬 아이디
//		WORD wItemID
//		BYTE bLevel
//		BYTE bCount
//		DWORD dwDuraMax						//내구도
//		DWORD dwDuraCurrent
//		BYTE bRefineMax
//		BYTE bRefineCurrent
//		BYTE bGLevel
//		__time64_t m_dEndTime				//사용 기한
//		BYTE bGradeEffect
//		BYTE bWrap
//		BYTE bELD
//		BYTE bMagicCount
//		{
//			BYTE bMagicID
//			WORD wValue
//			DWORD dwTime
//		}
// }

// < 길드 창고 아이탬 넣기 >
#define CS_GUILDCABINETPUTIN_REQ			(CS_MAP + 0x015B)
// BYTE bInvenID : 자신의 인벤아이디
// BYTE bItemID
// BYTE bCount
#define CS_GUILDCABINETPUTIN_ACK			(CS_MAP + 0x015C)
// BYTE bResult

// < 길드 창고 아이탬 빼기 >
#define CS_GUILDCABINETTAKEOUT_REQ			(CS_MAP + 0x015D)
// DWORD dwItemID : 길드 창고에서 아이탬 아이디
// BYTE bCount
// BYTE bItemID
// BYTE bInvenID
#define CS_GUILDCABINETTAKEOUT_ACK			(CS_MAP + 0x015E)
// BYTE bResult

// < 길드 경험치 / 자금 기부 >
#define CS_GUILDCONTRIBUTION_REQ			(CS_MAP + 0x015F)
// DWORD dwExp		: 경험치
// DWORD dwGold		: 돈 (1룬이하 1루나이상)
// DWORD dwSilver
// DWORD dwCooper
#define CS_GUILDCONTRIBUTION_ACK			(CS_MAP + 0x0160)
// BYTE bResult

// < 길드 공지사항 보기 > 조건 : 길드 UI내에서
#define CS_GUILDARTICLELIST_REQ				(CS_MAP + 0x0161)
#define CS_GUILDARTICLELIST_ACK				(CS_MAP + 0x0162)
// BYTE bCount	: 목록 수
// {
// 		DWORD dwID			: 일련번호
//		BYTE bDuty			: 권한
//		CString strWritter	: 작성자
//		CString strTitle	: 제목
// 		CString strArticle	: 공지사항
// 		CString strDate		: 작성일
// }

// < 길드 공지사항 등록/삭제 > 조건 : 부길드장 이상 / 길드 UI내에서
#define CS_GUILDARTICLEADD_REQ				(CS_MAP + 0x0163)
// CString strTitle
// CString strArticle
#define CS_GUILDARTICLEADD_ACK				(CS_MAP + 0x0164)
// BYTE bResult
#define CS_GUILDARTICLEDEL_REQ				(CS_MAP + 0x0165)
// DWORD dwID
#define CS_GUILDARTICLEDEL_ACK				(CS_MAP + 0x0166)
// BYTE bResult


// < 길드전용 문양 등록 > 조건 : 길드장 / 길드 UI내에서
#define CS_GUILDFAME_REQ					(CS_MAP + 0x0167)
// DWORD dwFame
// DWORD dwFameColor
#define CS_GUILDFAME_ACK					(CS_MAP + 0x0168)
// BYTE bResult
// DWORD dwFame
// DWORD dwFameColor

// < 길드 모집 / 용병단 광고 보기 >
#define CS_GUILDARTICLEUPDATE_REQ			(CS_MAP + 0x0169)
// DWORD dwID
// CString strTitle
// CString strArticle
#define CS_GUILDARTICLEUPDATE_ACK			(CS_MAP + 0x016A)
// BYTE bResult

//090522
// < 길드 모집 광고 올리기 - 지우기 > 조건 : 부길드장 이상
#define CS_GUILDWANTEDADD_REQ				(CS_MAP + 0x016B)
// DWORD dwID			: 새로올릴때-0 / 수정-아이디
// CString strTitle		: 제목
// CString strText		: 내용
// BYTE bMinLevel		: 최소레벨
// BYTE bMaxLevel		: 최대레벨

#define CS_GUILDWANTEDADD_ACK				(CS_MAP + 0x016C)
// BYTE bResult
#define CS_GUILDWANTEDDEL_REQ				(CS_MAP + 0x016D)
// DWORD dwID
#define CS_GUILDWANTEDDEL_ACK				(CS_MAP + 0x016E)
// BYTE bResult

// < 길드 모집 광고 보기 >
#define CS_GUILDWANTEDLIST_REQ				(CS_MAP + 0x016F)
#define CS_GUILDWANTEDLIST_ACK				(CS_MAP + 0x0170)
// DWORD dwCount	: 광고 수
// {
// 		DWORD dwGuildID		: 길드아이디
//		CString strName		: 길드이름
//		CString strTitle	: 제목
//		CString strText		: 내용
//		BYTE bMinLevel
//		BYTE bMaxLevel
//		INT64 dlTime
//		BYTE bApplied
// }

// < 길드 지원 신청/취소 > 조건 : 길드모집 UI내에서 
#define CS_GUILDVOLUNTEERING_REQ			(CS_MAP + 0x171)
// DWORD dwID		: 길드 모집광고 아이디
#define CS_GUILDVOLUNTEERING_ACK			(CS_MAP + 0x172)
// BYTE bResult
#define CS_GUILDVOLUNTEERINGDEL_REQ			(CS_MAP + 0x173)
#define CS_GUILDVOLUNTEERINGDEL_ACK			(CS_MAP + 0x174)
// BYTE bResult

// < 길드 지원병 목록 > 조건 : 길드 UI내에서 
#define CS_GUILDVOLUNTEERLIST_REQ			(CS_MAP + 0x0175)
#define CS_GUILDVOLUNTEERLIST_ACK			(CS_MAP + 0x0176)
// DWORD dwCount : 지원병 수
// {
//		DWORD dwCharID
//		CString strName
//		BYTE bLevel
//		BYTE bClass
//		DWORD dwRegion		: 현재 위치
// }

// 길드장이 입단 희망자에 대한 의사표시
#define CS_GUILDVOLUNTEERREPLY_REQ			(CS_MAP + 0x0177)
// DWORD dwCharID
// BYTE bReply
#define CS_GUILDVOLUNTEERREPLY_ACK			(CS_MAP + 0x0178)
// BYTE bResult

// <용병>
// < 용병 모집 광고 올리기 - 지우기>
#define CS_GUILDTACTICSWANTEDADD_REQ		(CS_MAP + 0x0179)
// DWORD dwID			: 새로올릴때-0 / 수정-아이디
// CString strTitle		: 제목
// CString strText		: 내용
// BYTE bDay			: 기간
// BYTE bMinLevel		: 최소레벨
// BYTE bMaxLevel		: 최대레벨
// DWORD dwPoint		: 공적
// DWORD dwGold
// DWORD dwSilver
// DWORD dwCooper
#define CS_GUILDTACTICSWANTEDADD_ACK		(CS_MAP + 0x017A)
// BYTE bResult
#define CS_GUILDTACTICSWANTEDDEL_REQ		(CS_MAP + 0x017B)
// DWORD dwGuildID
// DWORD dwID
#define CS_GUILDTACTICSWANTEDDEL_ACK		(CS_MAP + 0x017C)
// BYTE bResult

// <용병 모집 광고 리스트>
#define CS_GUILDTACTICSWANTEDLIST_REQ		(CS_MAP + 0x017D)
#define CS_GUILDTACTICSWANTEDLIST_ACK		(CS_MAP + 0x017E)
// DWORD dwCount	: 광고 수
// {
//		DWORD dwID			: 아이디
// 		DWORD dwGuildID		: 길드아이디
//		CString strName		: 길드이름
//		CString strTitle	: 제목
//		CString strText		: 내용
//		BYTE bDay			: 기간
//		BYTE bMinLevel		: 최소레벨
//		BYTE bMaxLevel		: 최대레벨
//		DWORD dwPoint		: 공적
//		DWORD dwGold
//		DWORD dwSilver
//		DWORD dwCooper
//		INT64 dlTime
//		BYTE bApplied
// }

// <용병 지원/취소>
#define CS_GUILDTACTICSVOLUNTEERING_REQ		(CS_MAP + 0x017F)
// DWORD dwID
#define CS_GUILDTACTICSVOLUNTEERING_ACK		(CS_MAP + 0x0180)
// BYTE bResult
#define CS_GUILDTACTICSVOLUNTEERINGDEL_REQ	(CS_MAP + 0x0181)
#define CS_GUILDTACTICSVOLUNTEERINGDEL_ACK	(CS_MAP + 0x0182)
// BYTE bResult

// < 용병 지원자 리스트 > 조건 : 길드 UI내에서
#define CS_GUILDTACTICSVOLUNTEERLIST_REQ	(CS_MAP + 0x0183)
#define CS_GUILDTACTICSVOLUNTEERLIST_ACK	(CS_MAP + 0x0184)
// BYTE bCount
// {
//		DWORD dwCharID
//		CString strName
//		BYTE bLevel
//		BYTE bClass
//		DWORD dwRegion
//		BYTE bDay
//		DWORD dwPoint
//		DWORD dwGold
//		DWORD dwSilver
//		DWORD dwCooper
// }

//길드장의 용병희망자에 대한 의사표현
#define CS_GUILDTACTICSREPLY_REQ			(CS_MAP + 0x0185)
// DWORD dwCharID
// BYTE bReply
#define CS_GUILDTACTICSREPLY_ACK			(CS_MAP + 0x0186)
// BYTE bResult

// < 길드 용병 강퇴 >
#define CS_GUILDTACTICSKICKOUT_REQ			(CS_MAP + 0x0187)
// DWORD dwCharID	100914 본인 탈퇴시 본신 CharID
#define CS_GUILDTACTICSKICKOUT_ACK			(CS_MAP + 0x0188)
// BYTE bResult
// DWORD dwTarget

////////////////////////////////////////////////////////////////////
#define CS_SKILLINIT_REQ					(CS_MAP + 0x0189)
// WORD wSkillID
// BYTE bInvenID
// BYTE bItemID
#define CS_SKILLINIT_ACK					(CS_MAP + 0x018A)
// BYTE bResult
// WORD wSkillID
#define CS_SKILLLIST_ACK					(CS_MAP + 0x018B)
// WORD wSkillPoint
// WORD wKindPoint1
// WORD wKindPoint2
// WORD wKindPoint3
// WORD wKindPoint4
// BYTE bCount
// {
//		WORD wSkillID
//		BYTE bLevel
//		DWORD dwTick
// }
#define CS_SKILLINITPOSSIBLE_REQ			(CS_MAP + 0x018C)
// BYTE bInvenID
// BYTE bItemID
#define CS_SKILLINITPOSSIBLE_ACK			(CS_MAP + 0x018D)
// BYTE bCount
// {
//		WORD wSkillID
// }
#define CS_HELMETHIDE_REQ					(CS_MAP + 0x018E)
// BYTE bHide	: 1 - 투구숨기기 0 - 투구보이기
#define CS_HELMETHIDE_ACK					(CS_MAP + 0x018F)
// DWORD dwCharID
// BYTE bHide
#define CS_PARTYMEMBERRECALL_REQ			(CS_MAP + 0x0190)
// BYTE bInvenID
// BYTE bItemID
// STRING strTarget
#define CS_PARTYMEMBERRECALL_ACK			(CS_MAP + 0x0191)
// BYTE bResult
// BYTE bType	// 0:파티원 소환 1:대상으로 이동
// CString strTarget

#define CS_CASHITEMCABINET_REQ				(CS_MAP + 0x0192)
#define CS_CASHITEMCABINET_ACK				(CS_MAP + 0x0193)
// __int64 ldwTime												//12m12d
// WORD wItemCount
// {
//		DWORD dwID
//		WORD wItemID
//		BYTE bLevel
//		BYTE bCount
//		DWORD dwDuraMax						//내구도
//		DWORD dwDuraCurrent
//		BYTE bRefineMax
//		BYTE bRefineCurrent
//		BYTE bGLevel
//		__time64_t m_dEndTime				//사용 기한
//		BYTE bGradeEffect
//		BYTE bWrap
//		BYTE bELD
//		BYTE bMagicCount
//		{
//			BYTE bMagicID
//			WORD wValue
//			DWORD dwTime
//		}
// }
#define CS_CASHITEMGET_REQ					(CS_MAP + 0x0194)
// DWORD dwID
// BYTE bInven
// BYTE bSlot
#define CS_CASHITEMGET_ACK					(CS_MAP + 0x0195)
// BYTE bResult
// DWORD dwCashItemID

#define CS_CASHSHOPITEMLIST_REQ				(CS_MAP + 0x0196)
// WORD wItemID
#define CS_CASHSHOPITEMLIST_ACK				(CS_MAP + 0x0197) //1106
// DWORD dwCash
// DWORD dwCashBonus
// BYTE bCategoryCount
// {
//	 STRING strCategoryName
//	 WORD wItemCount
//	 {
//			WORD wCashItemID
//			WORD wInfoID
//			DWORD dwMoney
//			BYTE bKind							//New=1, Best=2
//			WORD wItemID
//			BYTE bLevel
//			BYTE bCount
//			DWORD dwDuraMax						//내구도
//			DWORD dwDuraCurrent
//			BYTE bRefineMax
//			BYTE bRefineCurrent
//			BYTE bGLevel
//			WORD wUseDay						//사용 기한
//			BYTE bGradeEffect
//			BYTE bWrap
//			BYTE bELD
//			BYTE bMagicCount
//			{
//				BYTE bMagicID
//				WORD wValue
//				DWORD dwTime
//			}
//	 }
// }
#define CS_CASHITEMBUY_REQ					(CS_MAP + 0x0198)
// WORD wCashItemID
// BYTE bInven
// BYTE bSlot
#define CS_CASHITEMBUY_ACK					(CS_MAP + 0x0199)
// BYTE bResult
// DWORD dwCash
// DWORD dwCashBonus

#define CS_SOULMATE_REQ						(CS_MAP + 0x019A)
#define CS_SOULMATE_ACK						(CS_MAP + 0x019B)
// DWORD dwSoulmate
// Cstring strName
#define CS_SOULMATESEARCH_REQ				(CS_MAP + 0x019C)
// BYTE bNpcInvenID
// BYTE bNpcItemID
#define CS_SOULMATESEARCH_ACK				(CS_MAP + 0x019D)
// BYTE bResult
// CString strName
#define CS_SOULMATEREG_REQ					(CS_MAP + 0x019E)
// CString strName
#define CS_SOULMATEREG_ACK					(CS_MAP + 0x019F)
// BYTE bResult
// DWORD dwCharID
// CString strName
// DWORD dwRegion
#define CS_SOULMATEEND_REQ					(CS_MAP + 0x01A0)
#define CS_SOULMATEEND_ACK					(CS_MAP + 0x01A1)
// BYTE bResult
#define CS_SOULMATEREGREADY_REQ				(CS_MAP + 0x01A2)
// CString strName
#define CS_SOULMATEREGREADY_ACK				(CS_MAP + 0x01A3)
// BYTE bResult
// CString strName
#define CS_GAMBLECHECK_REQ					(CS_MAP + 0x01A4)
// BYTE bInven
// BYTE bItemID
// WORD wNpcID
#define CS_GAMBLECHECK_ACK					(CS_MAP + 0x01A5)
// BYTE bResult
// BYTE bInven
// BYTE bItemID
// DWORD dwNeedMoney
// BYTE bDiscountRate
#define CS_GAMBLEOPEN_REQ					(CS_MAP + 0x01A6)
// BYTE bInven
// BYTE bItemID
// WORD wNpcID
// BYTE bNpcInvenID
// BYTE bNpcItemID
#define CS_GAMBLEOPEN_ACK					(CS_MAP + 0x01A7)
// BYTE bResult
// DWORD dwGold
// DWORD dwSilver
// DWORD dwCooper
// BYTE bInvenID
// BYTE bItemID
// WORD wItemID
// BYTE bLevel
// BYTE bCount
// DWORD dwDuraMax						//내구도
// DWORD dwDuraCurrent
// BYTE bRefineMax
// BYTE bRefineCurrent
// BYTE bGLevel
// __time64_t m_dEndTime				//사용 기한
// BYTE bGradeEffect
// BYTE bWrap
// BYTE bELD
// BYTE bMagicCount
// {
// BYTE bMagicID
// WORD wValue
// DWORD dwTime
// }

#define CS_ADDGODTOWER_ACK					(CS_MAP + 0x01A8)
// WORD wID
// FLOAT fPosX						//공성전
// FLOAT fPosY
// FLOAT fPosZ
// WORD wGadBallID
// BYTE bCamp

#define CS_DELGODTOWER_ACK					(CS_MAP + 0x01A9)
// WORD wID

#define CS_ADDGODBALL_ACK					(CS_MAP + 0x01AA)
// WORD wID
// BYTE bCamp
// FLOAT fPosX
// FLOAT fPosY
// FLAOT fPosZ

#define CS_DELGODBALL_ACK					(CS_MAP + 0x01AB)
// WORD wGodBallID

#define CS_TAKEGODBALL_REQ					(CS_MAP + 0x01AC)
// WORD wGodBallID

#define CS_TAKEGODBALL_ACK					(CS_MAP + 0x01AD)
// DWORD dwCharID
// WORD wGodBallID

#define CS_REMOVEGODBALL_ACK				(CS_MAP + 0x01AE)
// DWORD dwCharID

#define CS_DROPGODBALL_ACK					(CS_MAP + 0x01AF)
// WORD wGodBallID
// BYTE bCamp
// DWORD dwCharID
// FLOAT fPosX
// FLOAT fPosY
// FLOAT fPosZ

#define CS_MOUNTGODBALL_REQ					(CS_MAP + 0x01B0)
// WORD wGodTower

#define CS_MOUNTGODBALL_ACK					(CS_MAP + 0x01B1)
// WORD wGodTower
// WORD wGodBall
// BYTE bCamp
// DWORD dwCharID

#define CS_DEMOUNTGODBALL_REQ				(CS_MAP + 0x01B2)
// WORD wGodTower

#define CS_DEMOUNTGODBALL_ACK				(CS_MAP + 0x01B3)
// WORD wGodTower
// DWORD dwCharID

#define CS_BALANCEOFPOWER_ACK				(CS_MAP + 0x01B4)
// FLOAT fPower_Def
// DWORD dwLeftTick
// WORD wKillPoint_Atk			//공성전
// WORD wKillPoint_Def
// CString strOwner1_Atk
// CString strOwner2_Atk
// CString strOwner1_Def
// CString strOwner2_Def
// WORD wKeep1_Atk
// WORD wKeep2_Atk
// WORD wKeep1_Def
// WORD wKeep2_Def

#define CS_PARTYMEMBERRECALLANS_REQ			(CS_MAP + 0x01B5)
// BYTE bAnswer
// STRING strInviter
// BYTE bInvenID
// BYTE bItemID
#define CS_PARTYMEMBERRECALLANS_ACK			(CS_MAP + 0x01B6)
// STRING strInviter
// BYTE bInvenID
// BYTE bItemID
#define CS_CASHITEMPRESENT_REQ				(CS_MAP + 0x01B7)
// STRING strTarget
// WORD wCashItemID
#define CS_CASHITEMPRESENT_ACK				(CS_MAP + 0x01B8)
// BYTE bResult
// DWORD dwCash
// DWORD dwCashBonus
#define CS_RESETPCBANG_ACK					(CS_MAP + 0x01B9)
// BYTE bInPcBang

#define CS_DURATIONDEC_ACK					(CS_MAP + 0x01BA)
// BYTE bInven
// BYTE bItemID
// DWORD dwDuration
#define CS_DURATIONEND_ACK					(CS_MAP + 0x01BB)	//12m10d
// BYTE bInven
// BYTE bItemID
// BYTE bDeleted	0:성능없음 1:아이템 날라감
#define CS_DURATIONREP_REQ					(CS_MAP + 0x01BC)
// BYTE bNeedCost	0:수리, 1:비용요청
// BYTE bType	0:일반수리, 1:장비수리 2:전부수리
// BYTE bInven	일반수리일 경우
// BYTE bItemID		"
// WORD wNpcID
// BYTE bNpcInvenID
// BYTE bNpcItemID
#define CS_DURATIONREPCOST_ACK				(CS_MAP + 0x01BD)
// DWORD dwCost
// BYTE bDiscountRate
#define CS_DURATIONREP_ACK					(CS_MAP + 0x01BE)
// BYTE bResult
// BYTE bCount
// {
//		BYTE bInven
//		BYTE bItemID
//		DWORD dwDuraMax
//		DWORD dwDuraCurrent
// }
#define CS_REFINE_REQ						(CS_MAP + 0x01BF)
// BYTE bNeedCost	0:제련 1:비용요청
// BYTE bInven
// BYTE bItemID
// BYTE bAddCount
// WORD wNpcID
// BYTE bNpcInvenID
// BYTE bNpcItemID
// {
//		BYTE bAddInven
//		BYTE bAddItem
// }
#define CS_REFINECOST_ACK					(CS_MAP + 0x01C0)
// DWORD dwCost
// BYTE bDiscountRate
#define CS_REFINE_ACK						(CS_MAP + 0x01C1)
// BYTE bResult
// BYTE bInven
// BYTE bItemID
// WORD wItemID
// BYTE bLevel
// BYTE bCount
// DWORD dwDuraMax
// DWORD dwDuraCurrent
// BYTE bRefineMax
// BYTE bRefineCurrent
// BYTE bGLevel
// __time64_t m_dEndTime				//사용 기한
// BYTE bGradeEffect
// BYTE bWrap
// BYTE bELD
// BYTE bMagicCount
// {
// BYTE bMagicID
// WORD wValue
// DWORD dwTime
// }
#define CS_RESETCOOLTIME_ACK				(CS_MAP + 0x01C2)
// WORD wExceptSkill
#define CS_ACTEND_REQ						(CS_MAP + 0x01C3)	//12m10d
#define CS_CASHCABINETBUY_REQ				(CS_MAP + 0x01C4)	//12m12d
// BYTE bNeedCash	0:buy 1:필요캐쉬요구
#define CS_CASHCABINETBUY_ACK				(CS_MAP + 0x01C5)	//12m12d
// BYTE bResult
// __int64 ldwTime
// DWORD dwCash
// DWORD dwCashBonus
#define CS_CASHITEMPUTIN_REQ				(CS_MAP + 0x01C6)	//12m12d
// BYTE bInvenID
// BYTE bItemID
#define CS_CASHITEMPUTIN_ACK				(CS_MAP + 0x01C7)	//12m12d
// BYTE bResult
// DWORD dwNewID
// WORD wItemID
// BYTE bLevel
// BYTE bCount
// DWORD dwDuraMax						//내구도
// DWORD dwDuraCurrent
// BYTE bRefineMax
// BYTE bRefineCurrent
// BYTE bGLevel
// __time64_t m_dEndTime				//사용 기한
// BYTE bGradeEffect
// BYTE bWrap
// BYTE bELD
// BYTE bMagicCount
// {
//		BYTE bMagicID
//		WORD wValue
//		DWORD dwTime
// }
#define CS_CASHCABINETBUYCASH_ACK			(CS_MAP + 0x01C8)
// DWORD dwCash
#define CS_CHANGENAME_REQ					(CS_MAP + 0x01C9)
// BYTE bInvenID
// BYTE bItemID
// STRING szName
#define CS_CHANGECHARBASE_ACK				(CS_MAP + 0x01CA)
// BYTE bResult
// DWORD dwCharID
// BYTE bType
// BYTE bValue
// CString strName
#define CS_STOPTHECLOCK_REQ					(CS_MAP + 0x01CB)
// BYTE bInven
// BYTE bItem		가방일 경우 INVALID_SLOT
// WORD wShopItemID
#define CS_STOPTHECLOCK_ACK					(CS_MAP + 0x01CC)
// BYTE bInven
// BYTE bItem
// __time64_t dEndTime
#define CS_HEROSELECT_REQ					(CS_MAP + 0x01CD) 
// WORD wBattleZoneID
// CString strHeroName
#define CS_HEROSELECT_ACK					(CS_MAP + 0x01CE) 
// BYTE bResult
#define CS_HEROLIST_REQ						(CS_MAP + 0x01CF) 
#define CS_HEROLIST_ACK						(CS_MAP + 0x01D0) 
// BYTE bLocalCount
// {
//		WORD wLocalID
//		CString strHeroName;
// }
// BYTE bCastleCount
// {
//		WORD wCaltleID
//		CString strHeroName;
// }
#define CS_SYSTEMMSG_ACK					(CS_MAP + 0x01D1)
// BYTE bSysMsgType
// ...
#define CS_TERMINATE_REQ					(CS_MAP + 0x01D2)
#define CS_OPENMONEY_ACK					(CS_MAP + 0x01D3)
// DWORD dwMoney

///////////////////////////////////////////////////////////////////////
/////////RELAY SERVER//////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
#define CS_RELAYCONNECT_ACK					(CS_MAP + 0x01D4)
// DWORD dwRelayIP
// WORD wPort 
#define CS_RELAYCHARDATA_REQ				(CS_MAP + 0x01D5)
// WORD wVersion
// DWORD dwCharID
// STRING strName
#define CS_RELAYCHARDATA_ACK				(CS_MAP + 0x01D6)
// BYTE bResult
#define CS_RELAYPROTECTEDLIST_REQ			(CS_MAP + 0x01D7)
// WORD wCount
// {
//		STRING strName
//		BYTE bOption						//차단
// }
#define CS_RELAYPROTECTEDADD_REQ			(CS_MAP + 0x01D8)
// STRING strName
// BYTE bOption								//차단
#define CS_RELAYPROTECTEDDEL_REQ			(CS_MAP + 0x01D9)
// STRING strName
#define CS_CHECKRELAY_REQ					(CS_MAP + 0x01DA)
#define CS_RELAYPROTECTEDOPTION_REQ			(CS_MAP + 0x01DB) // if(bOption == PROTECTED_CHAT)
// CString strName							//차단
// BYTE bOption
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#define CS_COMMENT_REQ						(CS_MAP + 0x01E0) //1106
// STRING strComment
#define CS_COMMENT_ACK						(CS_MAP + 0x01E1) //1106
// DWORD dwCharID
// STRING strComment
#define CS_PVPPOINT_ACK						(CS_MAP + 0x01E2) //1218
// DWORD dwTotalPoint
// DWORD dwUseablePoint
// BYTE bEvent
#define CS_GUILDPOINTLOG_REQ				(CS_MAP + 0x01E3)
#define CS_GUILDPOINTLOG_ACK				(CS_MAP + 0x01E4)
// WORD wCount
// {
// INT64 dlDate
// STRING strName
// DWORD dwPoint
// }
#define CS_GUILDPOINTREWARD_REQ				(CS_MAP + 0x01E5)
// STRING strTarget
// DWORD dwPoint
// STRING strMessage
#define CS_GUILDPOINTREWARD_ACK				(CS_MAP + 0x01E6)
// BYTE bResult
// DWORD dwRemainGuildPoint
#define CS_GUILDPVPRECORD_REQ				(CS_MAP + 0x01E7)
#define CS_GUILDPVPRECORD_ACK				(CS_MAP + 0x01E8)
// WORD wCount
// {
//		DWORD dwCharID
//		WORD wKillCount_W
//		WORD wDieCount_W
//		DWORD dwKillPoint_H_W
//		DWORD dwKillPoint_E_W
//		DWORD dwKillPoint_L_W
//		DWORD dwGodMonKillPoint_W
//		DWORD dwEntryPoint_W
//
//		WORD wKillCount_D
//		WORD wDieCount_D
//		DWORD dwKillPoint_H_D
//		DWORD dwKillPoint_E_D
//		DWORD dwKillPoint_L_D
//		DWORD dwGodMonKillPoint_D
//		DWORD dwEntryPoint_D
// }
#define CS_PVPRECORD_REQ					(CS_MAP + 0x01E9)
// BYTE bType // 0:전쟁 1:대결 
#define CS_PVPRECORD_ACK					(CS_MAP + 0x01EA)
// BYTE bType
// DWORD dwRankOrder
// BYTE bRankPercent
// INT nPoint
// 직업 순서대로(TCLASS_COUNT)
// {
//		DWORD dwKillCount
//		DWORD dwDieCount
// }
// BYTE bCount
// {
//		STRING strName
//		BYTE bWin
//		BYTE bClass
//		BYTE bLevel
//		DWORD dwPoint
//		INT64 dlDate
// }
#define CS_ENDWAR_ACK						(CS_MAP + 0x01EB)
//	BYTE bType								//공성전
//	DWORD dwWinGuildID
//	DWORD dwDefTotal
//	DWORD dwAtkTotal
//	STRING strDefName
//	DWORD dwDefPower
//	WORD wDefPoint
//	WORD wDefKillPoint
//	STRING strAtkName
//	DWORD dwAtkPower
//	WORD wAtkPoint
//	WORD wAtkKillPoint
#define CS_NPCMONSTERLIST_ACK				(CS_MAP + 0x01EC)	//공성전
#define CS_MONSTERBUY_REQ					(CS_MAP + 0x01ED)	//공성전
#define CS_MONSTERBUY_ACK					(CS_MAP + 0x01EE)	//공성전
// BYTE bResult
#define CS_ENTERCASTLE_ACK					(CS_MAP + 0x01EF)
// WORD wCastle
// BYTE bCamp
#define CS_LEAVECASTLE_ACK					(CS_MAP + 0x01F0)
#define CS_WARP_ACK							(CS_MAP + 0x01F1)
// DWORD dwObjID
// BYTE bObjType
// DWORD dwRange
// FLOAT fPosX
// FLOAT fPosY
// FLOAT fPosZ
#define CS_PROTECTEDOPTION_REQ				(CS_MAP + 0x01F2)	//차단
// DWORD dwCharID
// BYTE bOption
#define CS_FRIENDLIST_REQ					(CS_MAP + 0x01F3)	//0309
#define CS_ITEMCHANGE_REQ					(CS_MAP + 0x01F4)
// BYTE bInvenID
// BYTE bItemID
#define CS_ITEMCHANGE_ACK					(CS_MAP + 0x01F5)
// BYTE bResult
// BYTE bAddInven
// BYTE bAddItem
#define CS_COUNTDOWN_REQ					(CS_MAP + 0x01F6)
#define CS_COUNTDOWN_ACK					(CS_MAP + 0x01F7)
#define CS_CHANGEITEMATTR_ACK				(CS_MAP + 0x01F8)
// BYTE bInven
// BYTE bItemID
// WORD wItemID
// BYTE bLevel
// BYTE bCount
// DWORD dwDuraMax
// DWORD dwDuraCurrent
// BYTE bRefineMax
// BYTE bRefineCurrent
// BYTE bGLevel
// __time64_t m_dEndTime				//사용 기한
// BYTE bGradeEffect
// BYTE bWrap
// BYTE bELD
// BYTE bMagicCount
// {
// BYTE bMagicID
// WORD wValue
// }
#define CS_WASTEREFINE_REQ					(CS_MAP + 0x01F9)
// BYTE bInvenID
// BYTE bItemID
#define CS_WASTEREFINE_ACK					(CS_MAP + 0x01FA)
// BYTE bResult
#define CS_PETCANCEL_REQ					(CS_MAP + 0x01FB)
#define CS_CASHSHOPSTOP_ACK					(CS_MAP + 0x01FC)
// BYTE bRet

#define CS_AUCTIONREG_REQ					(CS_MAP + 0x01FD)
// WORD wNpcID
// WORD wHour
// BYTE bInvenID
// BYTE bItemID
// BYTE bCount
// __int64 ldwDirectPrice
// __int64 ldwStartPrice
// DWORD dwCode
#define CS_AUCTIONREG_ACK					(CS_MAP + 0x01FE)
// BYTE bResult
// DWORD dwAuctionID
#define CS_AUCTIONINTEREST_REQ				(CS_MAP + 0x01FF)
// BYTE bType
// DWORD dwAuctionID
#define CS_AUCTIONINTEREST_ACK				(CS_MAP + 0x0200)
// BYTE bResult
#define CS_AUCTIONINTERESTLIST_REQ			(CS_MAP + 0x0201)
// WORD wNpcID
#define CS_AUCTIONINTERESTLIST_ACK			(CS_MAP + 0x0202)
// BYTE bResult
// BYTE bCount
// {
//		DWORD dwAuctionID
//		CString strSeller
//		__time64_t dEnd
//		DWORD dwRemainTick
//		__int64 ldwDirectPrice
//		__int64	ldwStartPrice
//		__int64	m_ldwBidPrice
//		BYTE bBidCount
//		DWORD dwCode
//		__int64 m_dlID
//		BYTE m_bItemID
//		WORD m_wItemID
//		BYTE m_bLevel
//		BYTE m_bCount
//		BYTE m_bGLevel
//		DWORD m_dwDuraMax
//		DWORD m_dwDuraCur
//		BYTE m_bRefineCur
//		__time64_t m_dEndTime
//		BYTE m_bGradeEffect
//		BYTE bMagicCount
//		{
//		
//			BYTE bMagic
//			WORD wValue
//			DWORD dwTime
//		}
// }
#define CS_AUCTIONBIDLIST_REQ				(CS_MAP + 0x0203)
// WORD wNpcID
#define CS_AUCTIONBIDLIST_ACK				(CS_MAP + 0x0204)
// BYTE bResult
// BYTE bCount
// {
//		DWORD dwAuctionID
//		CString strSeller
//		__time64_t dEnd
//		DWORD dwRemainTick
//		__int64 ldwDirectPrice
//		__int64	ldwStartPrice
//		__int64	m_ldwBidPrice
//		BYTE bBidCount
//		DWORD dwCode
//		__int64 m_dlID
//		BYTE m_bItemID
//		WORD m_wItemID
//		BYTE m_bLevel
//		BYTE m_bCount
//		BYTE m_bGLevel
//		DWORD m_dwDuraMax
//		DWORD m_dwDuraCur
//		BYTE m_bRefineCur
//		__time64_t m_dEndTime
//		BYTE m_bGradeEffect
//		BYTE bMagicCount
//		{
//		
//			BYTE bMagic
//			WORD wValue
//			DWORD dwTime
//		}
// }
#define CS_AUCTIONREGCANCEL_REQ				(CS_MAP + 0x0205)
// WORD wNpcID
// DWORD dwAuctionID
#define CS_AUCTIONREGCANCEL_ACK				(CS_MAP + 0x0206)
// BYTE bResult
#define CS_AUCTIONBID_REQ					(CS_MAP + 0x0207)
// WORD wNpcID
// DWORD dwAuctionID
// __int64 ldwBidPrice 
#define CS_AUCTIONBID_ACK					(CS_MAP + 0x0208)
// BYTE bResult
#define CS_AUCTIONBUYDIRECT_REQ				(CS_MAP + 0x0209)
// WORD wNpcID
// DWORD dwAuctionID
// BYTE bCount
// __int64 ldwBidPrice // 개당가격
#define CS_AUCTIONBUYDIRECT_ACK				(CS_MAP + 0x020A)
// BYTE bResult
#define CS_AUCTIONFIND_REQ					(CS_MAP + 0x020B)
// WORD wNpcID
// BYTE bMinWearLv
// BYTE bMaxWearLv
// BYTE bMinPowerLv
// BYTE bMaxPowerLv
// BYTE bOption
// WORD wPageNum
// BYTE bCountPerPage
// BYTE bAlignKind
// WORD wCount
// {
//		DWORD dwCode
//		WORD wItemID
// }
#define CS_AUCTIONFIND_ACK					(CS_MAP + 0x020C)
// BYTE bResult
// WORD wTotalCount
// BYTE bPageNum
// WORD wCount
// {
//		DWORD dwAuctionID
//		CString strSeller
//		__time64_t dEnd
//		DWORD dwRemainTick
//		__int64 ldwDirectPrice
//		__int64	ldwStartPrice
//		__int64	m_ldwBidPrice
//		BYTE bBidCount
//		DWORD dwCode
//		BYTE m_bItemID
//		WORD m_wItemID
//		BYTE m_bLevel
//		BYTE m_bCount
//		BYTE m_bGLevel
//		DWORD m_dwDuraMax
//		DWORD m_dwDuraCur
//		BYTE m_bRefineCur
//		__time64_t m_dEndTime
//		BYTE m_bGradeEffect
//		BYTE bMagicCount
//		{		
//			BYTE bMagic
//			WORD wValue
//			DWORD dwTime
//		}
// }
//}
#define CS_AUCTIONREGLIST_REQ				(CS_MAP + 0x020D)
// WORD wNpcID
// BYTE bPageNum
// BYTE bCountPerPage
// BYTE bAlignKind
#define CS_AUCTIONREGLIST_ACK				(CS_MAP + 0x020E)
// BYTE bResult
// BYTE bCount
// {
//		DWORD dwAuctionID
//		CString strSeller
//		__time64_t dEnd
//		DWORD dwRemainTick
//		__int64 ldwDirectPrice
//		__int64	ldwStartPrice
//		__int64	m_ldwBidPrice
//		BYTE bBidCount
//		DWORD dwCode
//		BYTE m_bItemID
//		WORD m_wItemID
//		BYTE m_bLevel
//		BYTE m_bCount
//		BYTE m_bGLevel
//		DWORD m_dwDuraMax
//		DWORD m_dwDuraCur
//		BYTE m_bRefineCur
//		__time64_t m_dEndTime
//		BYTE m_bGradeEffect
//		BYTE bMagicCount
//		{		
//			BYTE bMagic
//			WORD wValue
//			DWORD dwTime
//		}
// }
//}
#define CS_KICKOUTMAP_REQ					(CS_MAP + 0x020F)
#define CS_KICKOUTMAP_ACK					(CS_MAP + 0x0210)
// WORD wMapID

// 온라인 용병 초대/수락
#define CS_GUILDTACTICSINVITE_REQ			(CS_MAP + 0x0211)
// CString strName
// BYTE bDay
// DWORD dwPoint
// DWORD dwGold
// DWORD dwSilver
// DWORD dwCooper
#define CS_GUILDTACTICSINVITE_ACK			(CS_MAP + 0x0212)
// CString strGuild
// CString strInviter
// BYTE bDay
// DWORD dwPoint
// DWORD dwGold
// DWORD dwSilver
// DWORD dwCooper
#define CS_GUILDTACTICSANSWER_REQ			(CS_MAP + 0x0213)
// BYTE bAnswer
// CString strInviter
// BYTE bDay
// DWORD dwPoint
// DWORD dwGold
// DWORD dwSilver
// DWORD dwCooper
#define CS_GUILDTACTICSANSWER_ACK			(CS_MAP + 0x0214)
// BYTE bResult
// DWORD dwTacticsID		: 용병길드아이디
// CString strTactics		: 용병길드이름
// CString strTacticsName	: 새로 가입된 용병 이름
#define CS_GUILDTACTICSLIST_REQ				(CS_MAP + 0x0215)
#define CS_GUILDTACTICSLIST_ACK				(CS_MAP + 0x0216)
// BYTE bCount
// {
//		DWORD dwCharID
//		CString strName
//		BYTE bLevel
//		BYTE bClass
//		BYTE bDay
//		DWORD dwPoint
//		INT64 dlMoney		100914
//		INT64 dlEndTime
//		DWORD dwGainPoint
//		DWORD dwRegion
//		WORD wCastle
//		BYTE bCamp
// }
#define CS_REGGUILDCLOAK_REQ				(CS_MAP + 0x0217)
// BYTE bInvenID
// BYTE bItemID
#define CS_REGGUILDCLOAK_ACK				(CS_MAP + 0x0218)
// BYTE bResult
// BYTE bInvenID
// BYTE bItemID
#define CS_RESETCLOAK_ACK					(CS_MAP + 0x0219)
// DWORD dwCharID
// BYTE bInvenID
// BYTE bItemID

#define CS_FAMERANKLIST_REQ					(CS_MAP + 0x021A)
// BYTE bMonth
#define CS_FAMERANKLIST_ACK					(CS_MAP + 0x021B)
// BYTE	bMonth
// BYTE bCount
// {
//   BYTE		bRank
//   DWORD		dwCharID
//   CString	strName
//   DWORD		dwTotalPoint
//   DWORD		dwMonthPoint
//   WORD		wWin
//   WORD		wLose
//   BYTE		bCountry
//   BYTE		bLevel
//   BYTE		bClass
//   BYTE		bRace
//   BYTE		bSex
//   BYTE		bHair
//   BYTE		bFace
//   CString	strSay
// }
#define CS_UPDATEFAMERANKLIST_ACK			(CS_MAP + 0x021C)
// BYTE	bMonth
// BYTE bCount
// {
//   BYTE		bRank
//   DWORD		dwCharID
//   CString	strName
//   DWORD		dwTotalPoint
//   DWORD		dwMonthPoint
//   WORD		wWin
//   WORD		wLose
//   BYTE		bCountry
//   BYTE		bLevel
//   BYTE		bClass
//   BYTE		bRace
//   BYTE		bSex
//   BYTE		bHair
//   BYTE		bFace
//   CString	strSay
// }
#define CS_MONTHRANKLIST_REQ				(CS_MAP + 0x021D)
#define CS_MONTHRANKLIST_ACK				(CS_MAP + 0x021E)
// bRankMonth
// BYTE bCount
// BYTE bCountry
// {
//   BYTE		bRank
//   DWORD		dwCharID
//   CString	strName
//   DWORD		dwTotalPoint
//   DWORD		dwMonthPoint
//   WORD		wWin
//   WORD		wLose
//   BYTE		bCountry
//   BYTE		bLevel
//   BYTE		bClass
//   BYTE		bRace
//   BYTE		bSex
//   BYTE		bHair
//   BYTE		bFace
//   CString	strSay
// }
// BYTE bCountry
// {
//   BYTE		bRank
//   DWORD		dwCharID
//   CString	strName
//   DWORD		dwTotalPoint
//   DWORD		dwMonthPoint
//   WORD		wWin
//   WORD		wLose
//   BYTE		bCountry
//   BYTE		bLevel
//   BYTE		bClass
//   BYTE		bRace
//   BYTE		bSex
//   BYTE		bHair
//   BYTE		bFace
//   CString	strSay
// }
#define CS_WARLORDSAY_REQ					(CS_MAP + 0x021F)
// CString strSay
#define CS_FIRSTGRADEGROUP_REQ				(CS_MAP + 0x0220)
// BYTE bMonth
#define CS_FIRSTGRADEGROUP_ACK				(CS_MAP + 0x0221)
// bRankMonth
// BYTE bCount
// BYTE bCountry
// {
//   BYTE		bRank
//   DWORD		dwCharID
//   CString	strName
//   DWORD		dwTotalPoint
//   DWORD		dwMonthPoint
//   WORD		wWin
//   WORD		wLose
//   BYTE		bCountry
//   BYTE		bLevel
//   BYTE		bClass
//   BYTE		bRace
//   BYTE		bSex
//   BYTE		bHair
//   BYTE		bFace
//   CString	strSay
// }
// BYTE bCountry
// {
//   BYTE		bRank
//   DWORD		dwCharID
//   CString	strName
//   DWORD		dwTotalPoint
//   DWORD		dwMonthPoint
//   WORD		wWin
//   WORD		wLose
//   BYTE		bCountry
//   BYTE		bLevel
//   BYTE		bClass
//   BYTE		bRace
//   BYTE		bSex
//   BYTE		bHair
//   BYTE		bFace
//   CString	strSay
// }
#define CS_AFTERMATH_ACK					(CS_MAP + 0x0222)
//	DWORD dwCharID
//	BYTE bStep
#define CS_CHATBAN_REQ						(CS_MAP + 0x0223)
// CString strName
// WORD wMin
#define CS_CHATBAN_ACK						(CS_MAP + 0x0224)
// BYTE bRet
#define CS_POSTLIST_REQ						(CS_MAP + 0x0225)	//0201
// WORD wPage
#define CS_POSTINFO_ACK						(CS_MAP + 0x0226)
// WORD wTotalCount
// WORD wNotReadCount
#define CS_TOURNAMENTAPPLYINFO_REQ			(CS_MAP + 0x0227)
#define CS_TOURNAMENTAPPLYINFO_ACK			(CS_MAP + 0x0228)
// BYTE bEntryCount	종목수
// {
// 		BYTE bEntryID			종목
// 		BYTE bType				구분
// 		DWORD dwClass			종족
// 		BYTE bStep				진행상황
// 		BYTE bMyApplied			지원
//		DWORD dwFee				수수료
//		DWORD dwBackFee			환불료
//		BYTE bPermitCount		허가서 갯수
//		BYTE bNeed				일반정원
//		WORD wTotalApplicant	일반지원자수
//		BYTE bRewardCount		상품수
//		{
//			BYTE bChartType
//			WORD wItemID
//			BYTE bCount
//		}
// 		BYTE b1stCount			1군지원자
// 		{
//			DWORD dwCharID
//			BYTE bCountry
//			CString strName
//			BYTE bLevel
//			BYTE bClass
//			DWORD dwRank
//			DWORD dwMonthRank
// 		}
//	}
#define CS_TOURNAMENTAPPLY_REQ				(CS_MAP + 0x0229)
// BYTE bEntryID
#define CS_TOURNAMENTAPPLY_ACK				(CS_MAP + 0x022A)
// BYTE bResult
// BYTE bEntryID
#define CS_TOURNAMENTJOINLIST_REQ			(CS_MAP + 0x022B)
#define CS_TOURNAMENTJOINLIST_ACK			(CS_MAP + 0x022C)
// BYTE bEntryCount	종목수
// {
// 		BYTE bEntryID			종목
// 		BYTE bType				구분
// 		DWORD dwClass			종족
// 		BYTE bStep				진행상황
// 		BYTE bMyApplied			지원
//		BYTE bRewardCount		상품수
//		{
//			BYTE bChartType
//			WORD wItemID
//			BYTE bCount
//		}
// 		BYTE bJoinCount			참가자수
// 		{
//			DWORD dwCharID
//			BYTE bCountry
//			CString strName
//			BYTE bLevel
//			BYTE bClass
//			DWORD dwRank
//			DWORD dwMonthRank
// 		}
//	}
#define CS_TOURNAMENTPARTYLIST_REQ			(CS_MAP + 0x022D)
// DWORD dwChiefID
#define CS_TOURNAMENTPARTYLIST_ACK			(CS_MAP + 0x022E)
// DWORD dwChiefID
// BYTE bMemberCount
// {
//		DWORD dwCharID
//		BYTE bCountry
//		CString strName
//		BYTE bLevel
//		BYTE bClass
//		DWORD dwRank
//		DWORD dwMonthRank
// }
#define CS_TOURNAMENTPARTYADD_REQ			(CS_MAP + 0x022F)
// CString strName
#define CS_TOURNAMENTPARTYADD_ACK			(CS_MAP + 0x0230)
// BYTE bResult
// CString strName
#define CS_TOURNAMEMTMATCHLIST_REQ			(CS_MAP + 0x0231)
#define CS_TOURNAMENTMATCHLIST_ACK			(CS_MAP + 0x0232)
// BYTE bEntryCount	종목수
// {
// 		BYTE bEntryID			종목
// 		BYTE bType				구분
// 		DWORD dwClass			종족
// 		BYTE bStep				진행상황
// 		BYTE bMyApplied			지원
//		BYTE bRewardCount		상품수
//		{
//			BYTE bChartType
//			WORD wItemID
//			BYTE bCount
//		}
// 		BYTE bJoinCount			참가자수
// 		{
//			BYTE bSlotID
//			DWORD dwCharID
//			BYTE bCountry
//			CString strName
//			BYTE bLevel
//			BYTE bClass
//			DWORD dwRank
//			DWORD dwMonthRank
//			BYTE bQfWin			8강 승패
//			BYTE bSfWin			4강 승패
//			BYTE bFWin			결승 승패
// 		}
// }
#define CS_TOURNAMENTEVENTLIST_REQ			(CS_MAP + 0x0233)
#define CS_TOURNAMENTEVENTLIST_ACK			(CS_MAP + 0x0234)
// BYTE bBase					기본당첨금1022
// DWORD dwTotal				총 누적 당첨금1022
// BYTE bEntryCount	종목수
// {
// 		BYTE bEntryID			종목
// 		BYTE bType				구분
//		CString strChoice
//		BYTE bCountry
//		FLOAT fRate				당첨금배율(소수점 2자리)1022
//		DWORD dwSRate			당첨금1022
// }
#define CS_TOURNAMENTEVENTINFO_REQ			(CS_MAP + 0x0235)
// BYTE bEntryID
#define CS_TOURNAMENTEVENTINFO_ACK			(CS_MAP + 0x0236)
// BYTE bEntryID
// BYTE bBase					기본당첨금1022
// DWORD dwTotal				총 누적 당첨금1022
// BYTE bJoinCount			참가자수
// {
//		DWORD dwCharID
//		BYTE bCountry
//		CString strGuildName
//		CString strName
//		BYTE bLevel
//		BYTE bClass
//		DWORD dwRank
//		DWORD dwMonthRank
//		FLOAT fRate				당첨금배욜(소수점 2자리)1022
//		BYTE bPartyCount
//		{
//			DWORD dwCharID
//			BYTE bCountry
//			CString strGuildName
//			CString strName
//			BYTE bLevel
//			BYTE bClass
//			DWORD dwRank
//			DWORD dwMonthRank
//		}
// }
#define CS_TOURNAMENTEVENTJOIN_REQ			(CS_MAP + 0x0237)
// BYTE bEntryID
// DWORD dwCharID
#define CS_TOURNAMENTEVENTJOIN_ACK			(CS_MAP + 0x0238)
// BYTE bResult
#define CS_TOURNAMENTCHEER_REQ				(CS_MAP + 0x0239)
// DWORD dwCharID
#define CS_TOURNAMENTCHEER_ACK				(CS_MAP + 0x023A)
// BYTE bResult

#define CS_TOURNAMENTENTER_ACK				(CS_MAP + 0x023B)
// WORD wBlueKill
// WORD wRedKill
// BYTE bCount
// {
//		BYTE bSlotID					팀구분1022
//		DWORD dwCharID
//		BYTE bCountry
//		CString strName
//		BYTE bLevel
//		BYTE bClass
// }
#define CS_TOURNAMENTRESULT_ACK				(CS_MAP + 0x023C)
// BYTE bStep
// DWORD dwWinID
// DWORD dwLoseID
#define CS_TOURNAMENTPARTYDEL_REQ			(CS_MAP + 0x023D)
// DWORD dwTargetID
#define CS_TOURNAMENTPARTYDEL_ACK			(CS_MAP + 0x023E)
// BYTE bResult
// DWORD dwTargetID
#define CS_TOURNAMENTAPPLYCANCEL_REQ		(CS_MAP + 0x023F)
#define CS_TOURNAMENTAPPLYCANCEL_ACK		(CS_MAP + 0x0240)
// BYTE bResult
#define CS_TOURNAMENTSCHEDULE_REQ			(CS_MAP + 0x0241)
#define CS_TOURNAMENTSCHEDULE_ACK			(CS_MAP + 0x0242)
// BYTE bCount
// {
//		BYTE bStep
//		INT64 dlStartTime
// }
#define CS_TOURNAMENTSELECTWINNER_ACK		(CS_MAP + 0x0243)
// BYTE bBlueValue
// BYTE bRedValue
#define CS_HELPMESSAGE_ACK					(CS_MAP + 0x0244)
// BYTE bID
// CString strMessage
#define CS_ITEMLEVELREVISION_ACK			(CS_MAP + 0x0245)
// BYTE bLevel
#define CS_RPSSTART_REQ						(CS_MAP + 0x0246)
// BYTE bType
#define CS_RPSSTART_ACK						(CS_MAP + 0x0247)
// BYTE bResult
// DWORD dwGold
// DWORD dwSilver
// DWORD dwCooper
#define CS_RPSGAME_REQ						(CS_MAP + 0x0248)
// BYTE bRPS 0:가위1:바위:2보3:포기
#define CS_RPSGAME_ACK						(CS_MAP + 0x0249)
// BYTE bWinCount	0:패배
// BYTE bPlayerRPS
// BYTE bNpcRPS
#define CS_ACDCLOSE_REQ						(CS_MAP + 0x024A)

// 100914
#define CS_CHANGECOUNTRY_REQ				(CS_MAP + 0x024B)
// BYTE bType(국가변경:IK_COUNTRY, 지원국변경:IK_AIDCOUNTRY)
// BYTE bCountry
// ACK : CS_CHANGECHARBASE_ACK(bValue = bCountry)
#define CS_WARCOUNTRYBALANCE_REQ			(CS_MAP + 0x024C)
#define CS_WARCOUNTRYBALANCE_ACK			(CS_MAP + 0x024D)
// DWORD dwCount_D
// DWORD dwCount_C
#define CS_MEETINGROOM_REQ					(CS_MAP + 0x024E)
// BYTE bType (0:invite, 1:answer)
// BYTE bValue (bType == 1 ? ASK_YES, ASK_NO, ASK_BUSY : 0)
// CString strName ( bType==0 ? Target Name : Inviter Name)
#define CS_MEETINGROOM_ACK					(CS_MAP + 0x024F)
// BYTE bType	(0:invite, 1:answer)
// BYTE bResult (MEETING_RESULT)
// CString strName( bType==0 ? Inviter Name : Target Name)
#define CS_ARENA_REQ						(CS_MAP + 0x0250)
// BYTE bCommand ( 0:신청, 1:나가기)
// WORD wNpcID
#define CS_ARENA_ACK						(CS_MAP + 0x0251)
// BYTE bResult
#define CS_ARENATEAM_ACK					(CS_MAP + 0x0252)
// BYTE bTeam (0:Reset, 1:Blue, 2:Red)
// BYTE bCount
// {
// DWORD dwCharID
// }
#define CS_CMMOVE_REQ						(CS_MAP + 0x0253)
// BYTE bChannel
// WORD wMapID
// float fPosX
// float fPosY
// float fPosZ
#define CS_ENTERSKYGARDEN_ACK				(CS_MAP + 0x0254)
// WORD wSkyGardenID
// BYTE bCamp
#define CS_CMMOVETOUSER_REQ					(CS_MAP + 0x0255)
// CString strTargetUser
#define CS_MAGICITEMBUY_REQ					(CS_MAP + 0x0256)
// WORD wNpcID
// DWORD dwMagicItemID
// BYTE bCount
// BYTE bNpcInvenID
// BYTE bNpcItemID
#define CS_MAGICITEMBUY_ACK					(CS_MAP + 0x0257)
// BYTE bRet
// DWORD dwGold
// DWORD dwSilver
// DWORD dwCooper
#define CS_CMGIFT_REQ						(CS_MAP + 0x0258)
// CString TargetName
// WORD wGiftID
#define CS_CMGIFT_ACK						(CS_MAP + 0x0259)
// BYTE bRet	(0:success, 1:잘못된Target 2:잘못된GiftID, 3:이미받은선물, 4:실패)

//////////////////////////////////////////////////////////////////////////
//Apex
#define CS_APEXDATA_REQ						(CS_MAP + 0x0900)
#define CS_APEXDATA_ACK						(CS_MAP + 0x0901)
#define CS_APEXSTART_REQ					(CS_MAP + 0x0902)
//HackShield
#define CS_HACKSHIELD_REQ					(CS_MAP + 0x0920)
#define CS_HACKSHIELD_ACK					(CS_MAP + 0x0921)
//NProtect GameGuard
#define CS_NPROTECT_REQ						(CS_MAP + 0x0940)
#define CS_NPROTECT_ACK						(CS_MAP + 0x0941)