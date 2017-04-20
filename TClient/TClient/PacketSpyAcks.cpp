#include "Stdafx.h"
#include "PacketSpyAcks.h"
#include "TClient.h"
#include "PacketSpyDlg.h"

#define REG_ACK(p)				theApp.m_pPacketSpy->RegPacket(FALSE, p, #p, GET_##p)

#define BEGIN_ACK_SPY			CString _strOut, _strTemp; pPacket->Rewind(FALSE)
#define END_ACK_SPY				return _strOut

#define SPYFMT_CASE(p,v)		case v : _strTemp.Format(#p " = %s\n", (LPCSTR)#v); _strOut += _strTemp
#define SPYFMT_STR(p,v) 		_strTemp.Format(#p " = %s\n", (LPCSTR)#v); _strOut += _strTemp
#define SPYFMT_INT(p) 			_strTemp.Format(#p " = %d\n", p); _strOut += _strTemp
#define SPYFMT_UINT(p)			_strTemp.Format(#p " = %u\n", p); _strOut += _strTemp
#define SPYFMT_FLOAT(p) 		_strTemp.Format(#p " = %.2f\n", p); _strOut += _strTemp

#define SPYOUT_BYTE(p) 			BYTE p; (*pPacket)>>p; _strTemp.Format(#p " = %u\n", p); _strOut += _strTemp
#define SPYOUT_WORD(p) 			WORD p; (*pPacket)>>p; _strTemp.Format(#p " = %u\n", p); _strOut += _strTemp
#define SPYOUT_DWORD(p) 		DWORD p; (*pPacket)>>p; _strTemp.Format(#p " = %u\n", p); _strOut += _strTemp
#define SPYOUT_FLOAT(p) 		FLOAT p; (*pPacket)>>p; _strTemp.Format(#p " = %.2f\n", p); _strOut += _strTemp
#define SPYOUT_STRING(p) 		CString p; (*pPacket)>>p; _strTemp.Format(#p " = %s\n", (LPCSTR)p); _strOut += _strTemp

#define SPYFMT_BEGIN_BLOCK(n)	_strTemp.Format("\n#%d\n{\n", n); _strOut += _strTemp
#define SPYFMT_END_BLOCK		_strOut += "}\n"

void CPacketSpyAcks::RegAllAcks()
{
	REG_ACK(CS_INVALIDCHAR_ACK);
	REG_ACK(CS_ADDCONNECT_ACK);
	REG_ACK(CS_SHUTDOWN_ACK);
	REG_ACK(CS_CONNECT_ACK);

	REG_ACK(CS_CHARSTATINFO_ACK);
	REG_ACK(CS_CHARINFO_ACK);
	REG_ACK(CS_ENTER_ACK);

	REG_ACK(CS_MOVE_ACK);
	REG_ACK(CS_DEFEND_ACK);
	REG_ACK(CS_SKILLUSE_ACK);
	REG_ACK(CS_ACTION_ACK);

	REG_ACK(CS_STOREOPEN_ACK);
	REG_ACK(CS_STORECLOSE_ACK);
	REG_ACK(CS_STOREITEMLIST_ACK);
	REG_ACK(CS_STOREITEMSELL_ACK);
}

CString CPacketSpyAcks::GET_CS_INVALIDCHAR_ACK(CPacket* pPacket)
{
	return "NONE";
}

CString CPacketSpyAcks::GET_CS_ADDCONNECT_ACK(CPacket* pPacket)
{
	BEGIN_ACK_SPY;

	BYTE bCount;
	(*pPacket) >> bCount;

	for( BYTE i=0; i<bCount; i++)
	{
		SPYFMT_BEGIN_BLOCK(i);

		DWORD dwIPAddr;
		WORD wPort;
		BYTE bServerID;

		(*pPacket)
			>> dwIPAddr
			>> wPort
			>> bServerID;

		SPYFMT_UINT(dwIPAddr);
		SPYFMT_UINT(wPort);
		SPYFMT_UINT(bServerID);

		SPYFMT_END_BLOCK;
	}

	END_ACK_SPY;
}

CString CPacketSpyAcks::GET_CS_CONNECT_ACK(CPacket* pPacket)
{
	BEGIN_ACK_SPY;

	BYTE bResult;
	(*pPacket) >> bResult;

	switch(bResult)
	{
		SPYFMT_CASE(bResult, CN_ALREADYEXIST); break;
		SPYFMT_CASE(bResult, CN_INVALIDVER); break;
		SPYFMT_CASE(bResult, CN_NOCHANNEL); break;
		SPYFMT_CASE(bResult, CN_NOCHAR); break;
		SPYFMT_CASE(bResult, CN_INTERNAL); break;
		SPYFMT_CASE(bResult, CN_SUCCESS);
		{
			BYTE bCnt;
			(*pPacket) >> bCnt;

			for(BYTE i=0; i<bCnt; ++i)
			{
				SPYFMT_BEGIN_BLOCK(i);
			
				BYTE bSvrID;
				(*pPacket) >> bSvrID;
				SPYFMT_UINT(bSvrID);

				SPYFMT_END_BLOCK;
			}
		}
		break;
	}

	END_ACK_SPY;
}

CString CPacketSpyAcks::GET_CS_SHUTDOWN_ACK(CPacket* pPacket)
{
	return "NONE";
}

CString CPacketSpyAcks::GET_CS_CHARSTATINFO_ACK(CPacket* pPacket)
{
	BEGIN_ACK_SPY;

	SPYOUT_DWORD(dwCharID);
	SPYOUT_WORD(wSTR);
	SPYOUT_WORD(wDEX);
	SPYOUT_WORD(wCON);
	SPYOUT_WORD(wINT);
	SPYOUT_WORD(wWIS);
	SPYOUT_WORD(wMEN);
	SPYOUT_DWORD(dwMinPysAtkPower);
	SPYOUT_DWORD(dwMaxPysAtkPower);
	SPYOUT_DWORD(dwPysDefPower);
	SPYOUT_DWORD(dwMinLongAtkPower);
	SPYOUT_DWORD(dwMaxLongAtkPower);
	SPYOUT_DWORD(dwAttackDelay);
	SPYOUT_DWORD(dwLongAttackDelay);
	SPYOUT_DWORD(dwMAttackDelay);
	SPYOUT_DWORD(dwMLongAttackDelay);
	SPYOUT_WORD(wAtkLevel);
	SPYOUT_WORD(wDefLevel);
	SPYOUT_BYTE(bPysCriticalProb);
	SPYOUT_WORD(wPysResist);
	SPYOUT_DWORD(dwMinMagicAP);
	SPYOUT_DWORD(dwMaxMagicAP);
	SPYOUT_DWORD(dwMagicDefPower);
	SPYOUT_DWORD(dwMinMagicLAP);
	SPYOUT_DWORD(dwMaxMagicLAP);
	SPYOUT_BYTE(bChargeSpeed);
	SPYOUT_BYTE(bChargeProb);
	SPYOUT_BYTE(bMagicCriticalProb);
	SPYOUT_WORD(wMagicResist);
	SPYOUT_WORD(bFR);
	SPYOUT_WORD(bIR);
	SPYOUT_WORD(bER);
	SPYOUT_WORD(bSR);
	SPYOUT_WORD(bDR);
	SPYOUT_WORD(wSkillPoint);
	SPYOUT_BYTE(bAftermathStep);

	END_ACK_SPY;
}

CString CPacketSpyAcks::GET_CS_CHARINFO_ACK(CPacket* pPacket)
{
	BEGIN_ACK_SPY;

	SPYOUT_DWORD(dwCharID);
	SPYOUT_STRING(strNAME);
	SPYOUT_BYTE(bClass);
	SPYOUT_BYTE(bRace);
	SPYOUT_BYTE(bCountry);
	SPYOUT_BYTE(bSex);
	SPYOUT_BYTE(bHair);
	SPYOUT_BYTE(bFace);
	SPYOUT_BYTE(bBody);
	SPYOUT_BYTE(bPants);
	SPYOUT_BYTE(bHand);
	SPYOUT_BYTE(bFoot);
	SPYOUT_BYTE(bLevel);
	SPYOUT_WORD(wPartyID);
	SPYOUT_DWORD(dwGuildID);
	SPYOUT_BYTE(bGuildDuty);
	SPYOUT_BYTE(bGuildPeer);
	SPYOUT_STRING(strGuildName);
	SPYOUT_DWORD(dwGold);
	SPYOUT_DWORD(dwSilver);
	SPYOUT_DWORD(dwCooper);
	SPYOUT_DWORD(dwPrevExp);
	SPYOUT_DWORD(dwNextExp);
	SPYOUT_DWORD(dwEXP);
	SPYOUT_DWORD(dwMaxHP);
	SPYOUT_DWORD(dwHP);
	SPYOUT_DWORD(dwMaxMP);
	SPYOUT_DWORD(dwMP);
	SPYOUT_DWORD(dwPartyChiefID);
	SPYOUT_WORD(wCommanderID);
	SPYOUT_DWORD(dwRegionID);
	SPYOUT_WORD(wMapID);
	SPYOUT_FLOAT(fPosX);
	SPYOUT_FLOAT(fPosY);
	SPYOUT_FLOAT(fPosZ);
	SPYOUT_WORD(wDIR);
	SPYOUT_WORD(wMySkillPoint);
	SPYOUT_WORD(wSkillPoint_1);
	SPYOUT_WORD(wSkillPoint_2);
	SPYOUT_WORD(wSkillPoint_3);
	SPYOUT_WORD(wSkillPoint_4);
	SPYOUT_BYTE(bInvenCount);
	
	for(BYTE i=0; i<bInvenCount; ++i)
	{
		SPYFMT_BEGIN_BLOCK(i);

		SPYOUT_BYTE(bInvenID);
		SPYOUT_WORD(wItemID);
		SPYOUT_BYTE(bItemCount);

		for(BYTE j=0; j<bItemCount; ++j)
		{
			SPYFMT_BEGIN_BLOCK(j);

			SPYOUT_BYTE(bItemID);
			SPYOUT_WORD(wItemID);
			SPYOUT_BYTE(bLevel);
			SPYOUT_BYTE(bCount);
			SPYOUT_BYTE(bMagicCount);

			for(BYTE k=0; k<bMagicCount; ++k)
			{
				SPYFMT_BEGIN_BLOCK(k);
	
				SPYOUT_BYTE(bMagicID);
				SPYOUT_WORD(wValue);
				SPYOUT_DWORD(dwTime);

				SPYFMT_END_BLOCK;
			}
			
			SPYFMT_END_BLOCK;
		}

		SPYFMT_END_BLOCK;
	}


	SPYOUT_BYTE(bSkillCount);
	for(BYTE i=0; i<bSkillCount; ++i)
	{
		SPYFMT_BEGIN_BLOCK(i);
	
		SPYOUT_WORD(wSkillID);
		SPYOUT_BYTE(bLevel);
		SPYOUT_DWORD(dwTick);

		SPYFMT_END_BLOCK;
	}

	SPYOUT_BYTE(bMaintainSkillCount);
	for(BYTE i=0; i<bMaintainSkillCount; ++i)
	{
		SPYFMT_BEGIN_BLOCK(i);
	
		SPYOUT_WORD(wSkillID);
		SPYOUT_BYTE(bLevel);
		SPYOUT_DWORD(dwTick);
		SPYOUT_DWORD(dwAttackID);
		SPYOUT_BYTE(bAttackType);
		SPYOUT_DWORD(dwHostID);
		SPYOUT_BYTE(bHostType);
		SPYOUT_BYTE(bHit);
		SPYOUT_WORD(wAttackLevel);
		SPYOUT_BYTE(bAttackerLevel);
		SPYOUT_DWORD(dwPysMinPower);
		SPYOUT_DWORD(dwPysMaxPower);
		SPYOUT_DWORD(dwMgMinPower);
		SPYOUT_DWORD(dwMgMaxPower);
		SPYOUT_BYTE(bCanSelect);
		SPYOUT_BYTE(bAttackCountry);
		SPYOUT_FLOAT(fPosX);
		SPYOUT_FLOAT(fPosY);
		SPYOUT_FLOAT(fPosZ);

		SPYFMT_END_BLOCK;
	}

	SPYOUT_BYTE(bPartyMemberCount);
	for(BYTE i=0; i<bPartyMemberCount; ++i)
	{
		SPYFMT_BEGIN_BLOCK(i);

		SPYOUT_DWORD(dwCharID);
		SPYOUT_STRING(strName);
		SPYOUT_STRING(strGuildName);
		SPYOUT_BYTE(bLevel);
		SPYOUT_DWORD(dwMaxHP);
		SPYOUT_DWORD(dwHP);
		SPYOUT_DWORD(dwMaxMP);
		SPYOUT_DWORD(dwMP);
		SPYOUT_BYTE(bRace);
		SPYOUT_BYTE(bSex);
		SPYOUT_BYTE(bFace);
		SPYOUT_BYTE(bHair);

		SPYFMT_END_BLOCK;
	}

	SPYOUT_BYTE(bHotkeyCount);
	for(BYTE i=0; i<bHotkeyCount; ++i)
	{
		SPYFMT_BEGIN_BLOCK(i);
	
		SPYOUT_BYTE(bInvenID);
		for(BYTE j=0; j<bInvenID; ++j)
		{
			SPYFMT_BEGIN_BLOCK(j);
			SPYOUT_BYTE(bType);
			SPYOUT_WORD(wID);
			SPYFMT_END_BLOCK;
		}
		SPYFMT_END_BLOCK;
	}

	SPYOUT_BYTE(bUsedITemCount);
	for(BYTE i=0; i<bUsedITemCount; ++i)
	{
		SPYFMT_BEGIN_BLOCK(i);
		SPYOUT_BYTE(bKind);
		SPYOUT_DWORD(dwRemainTick);
		SPYFMT_END_BLOCK;
	}

	END_ACK_SPY;
}

CString CPacketSpyAcks::GET_CS_ENTER_ACK(CPacket* pPacket)
{
	BEGIN_ACK_SPY;

	SPYOUT_DWORD(dwCharID);
	SPYOUT_STRING(strNAME);
	SPYOUT_DWORD(dwGuildID);
	SPYOUT_STRING(strGuildName);
	SPYOUT_BYTE(bGuildPeer);
	SPYOUT_BYTE(bStore);
	SPYOUT_STRING(strStoreName);
	SPYOUT_DWORD(dwRiding);
	SPYOUT_BYTE(bClass);
	SPYOUT_BYTE(bRace);
	SPYOUT_BYTE(bCountry);
	SPYOUT_BYTE(bSex);
	SPYOUT_BYTE(bHair);
	SPYOUT_BYTE(bFace);
	SPYOUT_BYTE(bBody);
	SPYOUT_BYTE(bPants);
	SPYOUT_BYTE(bHand);
	SPYOUT_BYTE(bFoot);
	SPYOUT_BYTE(bLevel);
	SPYOUT_DWORD(dwMaxHP);
	SPYOUT_DWORD(dwHP);
	SPYOUT_DWORD(dwMaxMP);
	SPYOUT_DWORD(dwMP);
	SPYOUT_DWORD(dwPartyChiefID);
	SPYOUT_WORD(wPartyID);
	SPYOUT_WORD(wCommanderID);
	SPYOUT_FLOAT(fPosX);
	SPYOUT_FLOAT(fPosY);
	SPYOUT_FLOAT(fPosZ);
	SPYOUT_BYTE(bAction);
	SPYOUT_BYTE(bBlock);
	SPYOUT_BYTE(bMode);
	SPYOUT_WORD(wPitch);
	SPYOUT_WORD(wDIR);
	SPYOUT_BYTE(bMouseDIR);
	SPYOUT_BYTE(bKeyDIR);
	SPYOUT_BYTE(bColor);
	SPYOUT_DWORD(dwRegionID);
	
	SPYOUT_BYTE(bMaintainSkillCount);
	for(BYTE i=0; i<bMaintainSkillCount; ++i)
	{
		SPYFMT_BEGIN_BLOCK(i);

		SPYOUT_WORD(wSkillID);
		SPYOUT_BYTE(bLevel);
		SPYOUT_DWORD(dwTick);
		SPYOUT_DWORD(dwAttackID);
		SPYOUT_BYTE(bAttackType);
		SPYOUT_DWORD(dwHostID);
		SPYOUT_BYTE(bHostType);
		SPYOUT_BYTE(bHit);
		SPYOUT_WORD(wAttackLevel);
		SPYOUT_BYTE(bAttackerLevel);
		SPYOUT_DWORD(dwPysMinPower);
		SPYOUT_DWORD(dwPysMaxPower);
		SPYOUT_DWORD(dwMgMinPower);
		SPYOUT_DWORD(dwMgMaxPower);
		SPYOUT_BYTE(bCanSelect);
		SPYOUT_BYTE(bAttackCountry);
		SPYOUT_FLOAT(fPosX);
		SPYOUT_FLOAT(fPosY);
		SPYOUT_FLOAT(fPosZ);

		SPYFMT_END_BLOCK;
	}
	
	SPYOUT_BYTE(bItemCount);
	for(BYTE i=0; i<bItemCount; ++i)
	{
		SPYFMT_BEGIN_BLOCK(i);

		SPYOUT_BYTE(bItemID);
		SPYOUT_WORD(wItemID);
		SPYOUT_BYTE(bLevel);
		SPYOUT_BYTE(bCount);
		
		SPYOUT_BYTE(bMagicCount);
		for(BYTE j=0; j<bMagicCount; ++j)
		{
			SPYFMT_BEGIN_BLOCK(j);

			SPYOUT_BYTE(bMagicID);
			SPYOUT_WORD(wValue);
			SPYOUT_DWORD(dwTime);

			SPYFMT_END_BLOCK;
		}

		SPYFMT_END_BLOCK;
	}
	
	SPYOUT_BYTE(bNewMember);

	END_ACK_SPY;
}

CString CPacketSpyAcks::GET_CS_MOVE_ACK(CPacket* pPacket)
{
	BEGIN_ACK_SPY;

	DWORD dwCharID;
	FLOAT fPosX;
	FLOAT fPosY;
	FLOAT fPosZ;
	WORD wPitch;
	WORD wDIR;
	BYTE bMouseDIR;
	BYTE bKeyDIR;
	BYTE bAction;

	(*pPacket)
		>> dwCharID
		>> fPosX
		>> fPosY
		>> fPosZ
		>> wPitch
		>> wDIR
		>> bMouseDIR
		>> bKeyDIR
		>> bAction;

	SPYFMT_UINT(dwCharID);
	SPYFMT_FLOAT(fPosX);
	SPYFMT_FLOAT(fPosY);
	SPYFMT_FLOAT(fPosZ);
	SPYFMT_UINT(wPitch);
	SPYFMT_UINT(wDIR);
	SPYFMT_UINT(bMouseDIR);
	SPYFMT_UINT(bKeyDIR);
	SPYFMT_UINT(bAction);

	END_ACK_SPY;
}

CString CPacketSpyAcks::GET_CS_DEFEND_ACK(CPacket* pPacket)
{
	BEGIN_ACK_SPY;

	SPYOUT_DWORD( dwAttackID);
	SPYOUT_DWORD( dwTargetID);
	SPYOUT_BYTE( bAttackType);
	SPYOUT_BYTE( bTargetType);
	SPYOUT_DWORD( dwHostID);
	SPYOUT_BYTE( bHostType);
	SPYOUT_DWORD( dwActID);
	SPYOUT_DWORD( dwAniID);
	SPYOUT_BYTE( bMaintain);
	SPYOUT_DWORD( dwTick);
	SPYOUT_BYTE( bHit);
	SPYOUT_BYTE( bAtkHit);
	SPYOUT_WORD( wAttackLevel);
	SPYOUT_BYTE( bAttackerLevel);
	SPYOUT_DWORD( dwPysMinPower);
	SPYOUT_DWORD( dwPysMaxPower);
	SPYOUT_DWORD( dwMgMinPower);
	SPYOUT_DWORD( dwMgMaxPower);
	SPYOUT_BYTE( bCanSelect);
	SPYOUT_BYTE( bCancelCharge);
	SPYOUT_BYTE( bAttackCountry);
	SPYOUT_WORD( wSkillID);
	SPYOUT_BYTE( bSkillLeve);
	SPYOUT_BYTE( bSkillPerform);
	SPYOUT_FLOAT( fAtkPosX);
	SPYOUT_FLOAT( fAtkPosY);
	SPYOUT_FLOAT( fAtkPosZ);
	SPYOUT_FLOAT( fDefPosX);
	SPYOUT_FLOAT( fDefPosY);
	SPYOUT_FLOAT( fDefPosZ);

	SPYOUT_BYTE( bDamageCount);
	for(BYTE i=0; i<bDamageCount; ++i)
	{
		SPYFMT_BEGIN_BLOCK(i);
		SPYOUT_BYTE(bDamageType);
		SPYOUT_WORD(wDamage);
		SPYFMT_END_BLOCK;
	}

	END_ACK_SPY;
}

CString CPacketSpyAcks::GET_CS_SKILLUSE_ACK(CPacket* pPacket)
{
	BEGIN_ACK_SPY;

	SPYOUT_BYTE(bResult);
	SPYOUT_DWORD(dwAttackID);
	SPYOUT_BYTE(bAttackType);
	SPYOUT_WORD(wSkillID);
	SPYOUT_BYTE(bActionID);
	SPYOUT_DWORD(dwActID);
	SPYOUT_DWORD(dwAniID);
	SPYOUT_BYTE(bSkillLevel);
	SPYOUT_WORD(wAttackLevel);
	SPYOUT_BYTE(bAttackerLevel);
	SPYOUT_DWORD(dwPysMinPower);
	SPYOUT_DWORD(dwPysMaxPower);
	SPYOUT_DWORD(dwMgMinPower);
	SPYOUT_DWORD(dwMgMaxPower);
	SPYOUT_BYTE(bSpecial);
	SPYOUT_BYTE(bCanSelect);
	SPYOUT_BYTE(bAttackCountry);
	SPYOUT_BYTE(bHit);
	SPYOUT_FLOAT(fPosX);
	SPYOUT_FLOAT(fPosY);
	SPYOUT_FLOAT(fPosZ);
	
	SPYOUT_BYTE(bCount);
	for(BYTE i=0; i<bCount; ++i)
	{
		SPYFMT_BEGIN_BLOCK(i);
		SPYOUT_DWORD(dwTargetID);
		SPYOUT_BYTE(bTargetType);
		SPYFMT_END_BLOCK;
	}

	END_ACK_SPY;
}

CString CPacketSpyAcks::GET_CS_ACTION_ACK(CPacket* pPacket)
{
	BEGIN_ACK_SPY;

	SPYOUT_BYTE(bResult);
	SPYOUT_DWORD(dwObjID);
	SPYOUT_BYTE(bObjType);
	SPYOUT_BYTE(bActionID);
	SPYOUT_DWORD(dwActID);
	SPYOUT_DWORD(dwAniID);
	SPYOUT_WORD(wSkillID);

	END_ACK_SPY;
}

CString CPacketSpyAcks::GET_CS_STOREOPEN_ACK(CPacket* pPacket)
{
	BEGIN_ACK_SPY;

	SPYOUT_BYTE(bResult);
	SPYOUT_DWORD(dwCharID);
	SPYOUT_STRING(strName);

	END_ACK_SPY;
}

CString CPacketSpyAcks::GET_CS_STORECLOSE_ACK(CPacket* pPacket)
{
	BEGIN_ACK_SPY;

	SPYOUT_BYTE(bResult);
	SPYOUT_DWORD(dwCharID);

	END_ACK_SPY;
}

CString CPacketSpyAcks::GET_CS_STOREITEMLIST_ACK(CPacket* pPacket)
{
	BEGIN_ACK_SPY;

	SPYOUT_DWORD(dwCharID);
	SPYOUT_STRING(strName);
	SPYOUT_BYTE(bCount);

	for(BYTE i=0; i<bCount; ++i)
	{
		SPYFMT_BEGIN_BLOCK(i);

		SPYOUT_BYTE(bNetID);
		SPYOUT_DWORD(dwRune);
		SPYOUT_DWORD(dwLuna);
		SPYOUT_DWORD(dwCron);
		SPYOUT_BYTE(bItemCnt);
		SPYOUT_WORD(wItemID);
		SPYOUT_BYTE(bGrade);
		SPYOUT_BYTE(bMagicCount);

		for(BYTE j=0; j<bMagicCount; ++j)
		{
			SPYFMT_BEGIN_BLOCK(j);

			SPYOUT_BYTE(bMagic);
			SPYOUT_WORD(wValue);
			SPYOUT_DWORD(dwTime);

			SPYFMT_END_BLOCK;
		}

		SPYFMT_END_BLOCK;
	}

	END_ACK_SPY;
}

CString CPacketSpyAcks::GET_CS_STOREITEMSELL_ACK(CPacket* pPacket)
{
	BEGIN_ACK_SPY;

	SPYOUT_BYTE(bResult);
	SPYOUT_WORD(wItemID);
	SPYOUT_BYTE(bCount);

	END_ACK_SPY;
}


















