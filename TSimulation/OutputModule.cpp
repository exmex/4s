#include "stdafx.h"
#include <commctrl.h>
#include "OutputModule.h"
#include "ChartDB.h"
#include "TextOut.h"
#include "LogFile.h"
#include "Global.h"

LPBalancePool pBalance=NULL;

void InitClassModule(void)
{
	pBalance = new CBalancePool();
	ChartToPool();
	ChartToPoolSkill();
}

void DeleteClassModule(void)
{
	delete pBalance;
}

void ChartToPool(void)
{
	int i;

	for(i=0 ; i<giTBASESTATCHART_COUNT ; i++)
	{
		pBalance->GetBaseStat(i)->SetId(TBASESTATCHART[i].bBaseID);
		pBalance->GetBaseStat(i)->SetStr(TBASESTATCHART[i].wSTR);
		pBalance->GetBaseStat(i)->SetDex(TBASESTATCHART[i].wDEX);
		pBalance->GetBaseStat(i)->SetCon(TBASESTATCHART[i].wCON);
		pBalance->GetBaseStat(i)->SetInt(TBASESTATCHART[i].wINT);
		pBalance->GetBaseStat(i)->SetWis(TBASESTATCHART[i].wWIS);
		pBalance->GetBaseStat(i)->SetMen(TBASESTATCHART[i].wMEN);
		pBalance->GetBaseStat(i)->CalcTotal();
		pBalance->GetBaseStat(i)->SetName(TBASESTATCHART[i].szName);
	}
	for(i=0 ; i<giTCLASSCHART_COUNT ; i++)
	{
		pBalance->GetClassStat(i)->SetId(TCLASSCHART[i].bClassID);
		pBalance->GetClassStat(i)->SetStr(TCLASSCHART[i].wSTR);
		pBalance->GetClassStat(i)->SetDex(TCLASSCHART[i].wDEX);
		pBalance->GetClassStat(i)->SetCon(TCLASSCHART[i].wCON);
		pBalance->GetClassStat(i)->SetInt(TCLASSCHART[i].wINT);
		pBalance->GetClassStat(i)->SetWis(TCLASSCHART[i].wWIS);
		pBalance->GetClassStat(i)->SetMen(TCLASSCHART[i].wMEN);
		pBalance->GetClassStat(i)->CalcTotal();
		pBalance->GetClassStat(i)->SetName(TCLASSCHART[i].szName);
		pBalance->GetClassStat(i)->SetRange(TCLASSCHART[i].wRangeMin, 0);
		pBalance->GetClassStat(i)->SetRange(TCLASSCHART[i].wRangeMax, 1);
		pBalance->GetClassStat(i)->SetRange(TCLASSCHART[i].wRRangeMin, 2);
		pBalance->GetClassStat(i)->SetRange(TCLASSCHART[i].wRRangeMax, 3);
		pBalance->GetClassStat(i)->SetRange(TCLASSCHART[i].wMRangeMin, 4);
		pBalance->GetClassStat(i)->SetRange(TCLASSCHART[i].wMRangeMax, 5);
		pBalance->GetClassStat(i)->SetRange(TCLASSCHART[i].wMRRangeMin, 6);
		pBalance->GetClassStat(i)->SetRange(TCLASSCHART[i].wMRRangeMax, 7);
	}
	for(i=0 ; i<giTRACECHART_COUNT ; i++)
	{
		pBalance->GetRaceStat(i)->SetId(TRACECHART[i].bRaceID);
		pBalance->GetRaceStat(i)->SetStr(TRACECHART[i].wSTR);
		pBalance->GetRaceStat(i)->SetDex(TRACECHART[i].wDEX);
		pBalance->GetRaceStat(i)->SetCon(TRACECHART[i].wCON);
		pBalance->GetRaceStat(i)->SetInt(TRACECHART[i].wINT);
		pBalance->GetRaceStat(i)->SetWis(TRACECHART[i].wWIS);
		pBalance->GetRaceStat(i)->SetMen(TRACECHART[i].wMEN);
		pBalance->GetRaceStat(i)->CalcTotal();
		pBalance->GetRaceStat(i)->SetName(TRACECHART[i].szName);
		pBalance->GetRaceStat(i)->SetUseItem(TRACECHART[i].bUseItem);
		pBalance->GetRaceStat(i)->SetUseItemArmor(TRACECHART[i].bUseItemArmor);
		pBalance->GetRaceStat(i)->SetSpeed(TRACECHART[i].fSpeed);
	}
	pBalance->SetRaceMax(giTRACECHART_COUNT);

	for(i=0 ; i<min(DATA_FACTOR_MAX, giTFORMULACHART_COUNT) ; i++)
	{
		pBalance->GetFormula(i)->SetId(TFORMULACHART[i].bID);
		pBalance->GetFormula(i)->SetName(TFORMULACHART[i].szName);
		pBalance->GetFormula(i)->SetInit(TFORMULACHART[i].dwInit);
		pBalance->GetFormula(i)->SetX(TFORMULACHART[i].fRateX);
		pBalance->GetFormula(i)->SetY(TFORMULACHART[i].fRateY);
	}
}

void ChartToPoolSkill(void)
{
	int i, j;
	int iLevel, iClass;
	int iSkillCount;
	for(iClass = 0 ; iClass <giTCLASSCHART_COUNT ; iClass++)
	{
		for(iLevel = 1 ; iLevel<=_BAL_LEVELMAX ; iLevel++)
		{
			//	직업별 사용가능 스킬 확인
			iSkillCount=0;
			for(i=0 ; i<giTSKILLCHART_COUNT ; i++)
			{
				if(
					(TSKILLCHART[i].bLevel<=iLevel) && 
					((TSKILLCHART[i].dwClassID & (1<<iClass)) == (1<<iClass))
					&& (TSKILLCHART[i].bCanLearn==1)
					)	//레벨에 따라 해당 직업이 사용할 수 있는 스킬
				{
					pBalance->GetClassSkill(iLevel)->SetSkillID(iClass, iSkillCount, TSKILLCHART[i].wID);
					iSkillCount++;
				}
			}
			//	보유스킬에 따라 장착여부 설정
			for(i=0 ; i<iSkillCount ; i++)
			{
				if(pBalance->GetClassSkill(iLevel)->GetSkillID(iClass, i) >0)
					//보유한 스킬이 있을 때
				{
					for(j=0 ; j<giTSKILLDATA_COUNT ; j++)
					{
						if(TSKILLDATA[j].wSkillID == pBalance->GetClassSkill(iLevel)->GetSkillID(iClass, i))
							//	스킬 데이터에서 보유한 스킬의 일련번호를 찾는다.
						{
							if(TSKILLDATA[j].bType == 0)
								//	스킬 효과가 착용인 경우
							{
								pBalance->GetClassSkill(iLevel)->EnableItemSetBit(TSKILLDATA[j].bExec, iClass);
							}
							if(TSKILLDATA[j].bType == 2 && TSKILLDATA[j].bExec == 7)
								//	스킬 효과가 소환이고 효과 내용이 몬스터인 경우
							{
								pBalance->GetClassSkill(iLevel)->SetIsSummon(1, iClass);
							}
						}
					}
				}
			}
		}
	}

	iSkillCount=0;
}

void BaseStat(char *PathName)
{
	BOOL flag, ScrFlag;
	int i, j;
	int iStr, iDex, iCon, iInt, iWis, iMen, iTotal;
	char szName[_MAX_FNAME] = "초기 능력치";
	char szFname[_MAX_FNAME];

	flag = GetIsLogFile();
	ScrFlag = GetIsLogScreen();

	ClearTextOut();
	TextoutScreen("\t\t\t\t< %s >\r\n", szName);
	sprintf(szFname, "%s\\%s.txt", PathName, szName);
	if(gsLevelStat.mIsFile==TRUE) LogFileStart(szFname);
	SetIsLogFile(TRUE);
	SetIsLogScreen(FALSE);


	TextoutScreen("%15s%15s%5s%5s%5s%5s%5s%5s%5s"
		, "종족", "직업", "힘", "민첩", "체력", "지능", "지혜" , "정신", "합계");
	TextoutScreen(" ");

	iStr = pBalance->GetBaseStat(0)->GetStr();
	iDex = pBalance->GetBaseStat(0)->GetDex();
	iCon = pBalance->GetBaseStat(0)->GetCon();
	iInt = pBalance->GetBaseStat(0)->GetInt();
	iWis = pBalance->GetBaseStat(0)->GetWis();
	iMen = pBalance->GetBaseStat(0)->GetMen();
	iTotal = pBalance->GetBaseStat(0)->GetTotal();

	TextoutScreen("%15s%15s%5d%5d%5d%5d%5d%5d%5d"
		, "기본_능력치", "", iStr, iDex, iCon, iInt, iWis, iMen, iTotal
		);
	TextoutScreen(" ");

	for(i=0 ; i<giTCLASSCHART_COUNT ; i++)
	{
		iStr = pBalance->GetClassStat(i)->GetStr();
		iDex = pBalance->GetClassStat(i)->GetDex();
		iCon = pBalance->GetClassStat(i)->GetCon();
		iInt = pBalance->GetClassStat(i)->GetInt();
		iWis = pBalance->GetClassStat(i)->GetWis();
		iMen = pBalance->GetClassStat(i)->GetMen();
		iTotal = pBalance->GetClassStat(i)->GetTotal();

		TextoutScreen("%15s%15s%5d%5d%5d%5d%5d%5d%5d"
			, "직업별_능력치", pBalance->GetClassStat(i)->GetName(), iStr, iDex, iCon, iInt, iWis, iMen, iTotal
			);
	}
	TextoutScreen(" ");

	for(i=0 ; i<pBalance->GetRaceMax() ; i++)
	{
		iStr = pBalance->GetRaceStat(i)->GetStr();
		iDex = pBalance->GetRaceStat(i)->GetDex();
		iCon = pBalance->GetRaceStat(i)->GetCon();
		iInt = pBalance->GetRaceStat(i)->GetInt();
		iWis = pBalance->GetRaceStat(i)->GetWis();
		iMen = pBalance->GetRaceStat(i)->GetMen();
		iTotal = pBalance->GetRaceStat(i)->GetTotal();

		TextoutScreen("%15s%15s%5d%5d%5d%5d%5d%5d%5d"
			, "종족별_능력치", pBalance->GetRaceStat(i)->GetName(), iStr, iDex, iCon, iInt, iWis, iMen, iTotal
			);
	}
	TextoutScreen(" ");

	for(i=0 ; i<pBalance->GetRaceMax() ; i++)
	{
		for(j=0 ; j<giTCLASSCHART_COUNT ; j++)
		{
			iStr = pBalance->GetBaseStat(0)->GetStr() + pBalance->GetClassStat(j)->GetStr() + pBalance->GetRaceStat(i)->GetStr();
			iDex = pBalance->GetBaseStat(0)->GetDex() + pBalance->GetClassStat(j)->GetDex() + pBalance->GetRaceStat(i)->GetDex();
			iCon = pBalance->GetBaseStat(0)->GetCon() + pBalance->GetClassStat(j)->GetCon() + pBalance->GetRaceStat(i)->GetCon();
			iInt = pBalance->GetBaseStat(0)->GetInt() + pBalance->GetClassStat(j)->GetInt() + pBalance->GetRaceStat(i)->GetInt();
			iWis = pBalance->GetBaseStat(0)->GetWis() + pBalance->GetClassStat(j)->GetWis() + pBalance->GetRaceStat(i)->GetWis();
			iMen = pBalance->GetBaseStat(0)->GetMen() + pBalance->GetClassStat(j)->GetMen() + pBalance->GetRaceStat(i)->GetMen();
			iTotal = pBalance->GetBaseStat(0)->GetTotal() + pBalance->GetClassStat(j)->GetTotal() + pBalance->GetRaceStat(i)->GetTotal();;

			TextoutScreen("%15s%15s%5d%5d%5d%5d%5d%5d%5d"
				, pBalance->GetRaceStat(i)->GetName()
				, pBalance->GetClassStat(j)->GetName()
				, iStr
				, iDex
				, iCon
				, iInt
				, iWis
				, iMen
				, iTotal
				);
		}
		TextoutScreen(" ");
	}

	SetIsLogScreen(ScrFlag);
	SetIsLogFile(flag);
	LogFileEnd();
	if(gsLevelStat.mIsFile==TRUE) TextoutScreen("\r\n>> 결과는 '%s'에 저장되었습니다.", szFname);
	else TextoutScreen("\r\n>> '보고서를 파일로 출력'을 선택하면 파일로 저장됩니다.");
}

void LevelStat(char *PathName)
{
	BOOL flag, ScrFlag;
	int i, j;
	int iStr, iDex, iCon, iInt, iWis, iMen, iTotal;
	char szName[_MAX_FNAME] = "레벨 적용시 스탯";
	char szFname[_MAX_FNAME];
	char szStr[_MAX_FNAME];

	flag = GetIsLogFile();
	ScrFlag = GetIsLogScreen();

	ClearTextOut();
	TextoutScreen("\t\t\t\t< %s >\r\n", szName);
	sprintf(szFname, "%s\\%s.txt", PathName, szName);
	if(gsLevelStat.mIsFile==TRUE) LogFileStart(szFname);
	SetIsLogFile(TRUE);
	SetIsLogScreen(FALSE);

	TextoutScreen("%15s%15s%5s%5s%5s%5s%5s%5s%5s%5s %7s%5s%7s%9s%5s%5s%9s%5s%7s%9s%9s%9s %7s%7s%11s%9s%7s%9s%5s%7s%9s%9s%9s %13s%9s%9s%9s %13s%11s%11s%11s %13s%9s%9s%9s %13s%9s%9s%9s %7s%7s%9s"
		, "종족", "직업", "레벨", "힘", "민첩", "체력", "지능", "지혜" , "정신", "합계"
		//%7s%5s%7s%9s%5s%5s%9s%5s%7s%9s%9s%9s %7s%7s%9s%7s%9s%5s%7s%9s%9s%9s %13s%9s%9s%9s %13s%11s%11s%11s %13s%9s%9s%9s %7s%7s%9s"
		, " 물리 ", "소지", "원거리", "  일반  ", "공격", "방어", "  물리  ", "HP ", "  HP  ", "  물리  ", "평균물리", "Item물리"
//		, "공격력", "무게", "공격력", "공격속도", "등급", "등급", "치명확률", "   ", "회복율", "상태저항", "치명최대", " 방어력 "
		, " 마법 ", " 저주 ", "마법원거리", "  마법  ", " 마법 ", "  마법  ", "MP ", "  MP  ", "  마법  ", "평균마법", "Item마법"
//		, "공격력", "성공률", "  공격력  ", "시전속도", "성공율", "치명확률", "   ", "회복율", "상태저항", "치명최대", " 방어력 "
		, "  초당물리  ", "초당물리", "초당물리", "평균물리"
//		, "공격력(치명)", " 공격력 ", " 데미지 ", "공격횟수"
		, " 초당원거리 ", "초당원거리", "초당원거리", "평균원거리"
//		, "공격력(치명)", "  공격력  ", "  데미지  ", " 공격횟수 "
		, "  초당마법  ", "초당마법", "초당마법", "평균마법"
//		, "공격력(치명)", " 공격력 ", " 데미지 ", "공격횟수"
		, "  초당마원  ", "초당마원", "초당마원", "평균마원"
//		, "공격력(치명)", " 공격력 ", " 데미지 ", "공격횟수"
		, "HP회복", "MP회복", "        "
//		, " 시간 ", " 시간 ", "공격방식"
		);

	TextoutScreen("%15s%15s%5s%5s%5s%5s%5s%5s%5s%5s %7s%5s%7s%9s%5s%5s%9s%5s%7s%9s%9s%9s %7s%7s%11s%9s%7s%9s%5s%7s%9s%9s%9s %13s%9s%9s%9s %13s%11s%11s%11s %13s%9s%9s%9s %13s%9s%9s%9s %7s%7s%9s"
		, "", "", "", "", "", "", "", "" , "", ""
		, "공격력", "무게", "공격력", "공격속도", "등급", "등급", "치명확률", "   ", "회복율", "상태저항", "치명최대", " 방어력 "
		, "공격력", "성공률", "  공격력  ", "시전속도", "성공율", "치명확률", "   ", "회복율", "상태저항", "치명최대", " 방어력 "
		, "공격력(치명)", " 공격력 ", " 데미지 ", "공격횟수"
		, "공격력(치명)", "  공격력  ", "  데미지  ", " 공격횟수 "
		, "공격력(치명)", " 공격력 ", " 데미지 ", "공격횟수"
		, "공격력(치명)", " 공격력 ", " 데미지 ", "공격횟수"
		, " 시간 ", " 시간 ", "공격방식"
		);

	TextoutScreen(" ");

	for(i=0 ; i<pBalance->GetRaceMax() ; i++)
	{
		for(j=0 ; j<giTCLASSCHART_COUNT ; j++)
		{
			iStr = pBalance->GetStat(gsLevelStat.mLevel, j, i, 0);
			iDex = pBalance->GetStat(gsLevelStat.mLevel, j, i, 1);
			iCon = pBalance->GetStat(gsLevelStat.mLevel, j, i, 2);
			iInt = pBalance->GetStat(gsLevelStat.mLevel, j, i, 3);
			iWis = pBalance->GetStat(gsLevelStat.mLevel, j, i, 4);
			iMen = pBalance->GetStat(gsLevelStat.mLevel, j, i, 5);
			iTotal = iStr + iDex + iCon + iInt + iWis + iMen;

			pBalance->CalcCombat(gsLevelStat.mLevel, j, i, gsLevelStat.mLevel, j, i);

	switch(pBalance->GetFactorOffense()->GetResultDataInt(RDATA_ATTACK_TYPE))
	{
		case 0:
			strcpy(szStr, "물리  ");
			break;
		case 1:
			strcpy(szStr, "원거리 ");
			break;
		case 2:
			strcpy(szStr, "마법  ");
			break;
	}

			TextoutScreen("%15s%15s%5d%5d%5d%5d%5d%5d%5d%5d %7.2f%5d%7.2f%9d%5d%5d%9d%5d%7d%9d%9d%9d %7.2f%7d%11.2f%9d%7d%9d%5d%7d%9d%9d%9d %13.2f%9.2f%9.2f%9.2f %13.2f%11.2f%11.2f%11.2f %13.2f%9.2f%9.2f%9.2f %13.2f%9.2f%9.2f%9.2f %7.2f%7.2f%9s"
				, pBalance->GetRaceStat(i)->GetName()
				, pBalance->GetClassStat(j)->GetName()
				, gsLevelStat.mLevel
				, iStr
				, iDex
				, iCon
				, iInt
				, iWis
				, iMen
				, iTotal
				, pBalance->GetFactorOffense()->GetData(DATA_ATTACKPOINT)
				, pBalance->GetFactorOffense()->GetDataInt(DATA_WEIGHT)
				, pBalance->GetFactorOffense()->GetData(DATA_RANGEATTACKPOINT)
				, pBalance->GetFactorOffense()->GetDataInt(DATA_SPEED)
				, pBalance->GetFactorOffense()->GetDataInt(DATA_ATACKGRADE)
				, pBalance->GetFactorOffense()->GetDataInt(DATA_DEFENSEGRADE)
				, pBalance->GetFactorOffense()->GetDataInt(DATA_CRITICALRATIO)
				, pBalance->GetFactorOffense()->GetDataInt(DATA_HP)
				, pBalance->GetFactorOffense()->GetDataInt(DATA_HPRECOVERY)
				, pBalance->GetFactorOffense()->GetDataInt(DATA_PHYSICALRESIST)
				, pBalance->GetFactorOffense()->GetDataInt(DATA_CRITICALMAX)
				, pBalance->GetFactorOffense()->GetDataInt(DATA_ITEMDEFENSE)

				, pBalance->GetFactorOffense()->GetData(DATA_MAGICATTACKPOINT)
				, pBalance->GetFactorOffense()->GetDataInt(DATA_CURSERATIO)
				, pBalance->GetFactorOffense()->GetData(DATA_MAGICRATTACKPOINT)
				, pBalance->GetFactorOffense()->GetDataInt(DATA_MAGICSPEED)
				, pBalance->GetFactorOffense()->GetDataInt(DATA_MAGICRATIO)
				, pBalance->GetFactorOffense()->GetDataInt(DATA_MAGICCRITICALRATIO)
				, pBalance->GetFactorOffense()->GetDataInt(DATA_MP)
				, pBalance->GetFactorOffense()->GetDataInt(DATA_MPRECOVERY)
				, pBalance->GetFactorOffense()->GetDataInt(DATA_MAGICRESIST)
				, pBalance->GetFactorOffense()->GetDataInt(DATA_MAGICCRITICALMAX)
				, pBalance->GetFactorOffense()->GetDataInt(DATA_ITEMMAGICDEFENSE)

				, pBalance->GetFactorOffense()->GetResultData(RDATA_CRITCAL_AP_SEC)
				, pBalance->GetFactorOffense()->GetResultData(RDATA_AP_SEC)
				, pBalance->GetFactorOffense()->GetResultData(RDATA_DAMAGE_SEC)
				, pBalance->GetFactorOffense()->GetResultData(RDATA_ATTACK_COUNT)

				, pBalance->GetFactorOffense()->GetResultData(RDATA_CRITCAL_RAP_SEC)
				, pBalance->GetFactorOffense()->GetResultData(RDATA_RAP_SEC)
				, pBalance->GetFactorOffense()->GetResultData(RDATA_RDAMAGE_SEC)
				, pBalance->GetFactorOffense()->GetResultData(RDATA_RATTACK_COUNT)

				, pBalance->GetFactorOffense()->GetResultData(RDATA_CRITCAL_MAP_SEC)
				, pBalance->GetFactorOffense()->GetResultData(RDATA_MAP_SEC)
				, pBalance->GetFactorOffense()->GetResultData(RDATA_MDAMAGE_SEC)
				, pBalance->GetFactorOffense()->GetResultData(RDATA_MATTACK_COUNT)

				, pBalance->GetFactorOffense()->GetResultData(RDATA_CRITCAL_MRAP_SEC)
				, pBalance->GetFactorOffense()->GetResultData(RDATA_MRAP_SEC)
				, pBalance->GetFactorOffense()->GetResultData(RDATA_MRDAMAGE_SEC)
				, pBalance->GetFactorOffense()->GetResultData(RDATA_MRATTACK_COUNT)

				, pBalance->GetFactorOffense()->GetResultData(RDATA_HPMAX_RECOVERY_TIME)
				, pBalance->GetFactorOffense()->GetResultData(RDATA_MPMAX_RECOVERY_TIME)
				, szStr
				);
		}
		TextoutScreen(" ");
	}

	SetIsLogScreen(ScrFlag);
	SetIsLogFile(flag);
	LogFileEnd();
	if(gsLevelStat.mIsFile==TRUE) TextoutScreen("\r\n>> 결과는 '%s'에 저장되었습니다.", szFname);
	else TextoutScreen("\r\n>> '보고서를 파일로 출력'을 선택하면 파일로 저장됩니다.");
}

void AllLevelStat(char *PathName)
{
	BOOL flag, ScrFlag;
	int i;
	int iStr, iDex, iCon, iInt, iWis, iMen, iTotal;
	char szFname[_MAX_FNAME];
	char szName[_MAX_FNAME] = "전체 레벨 스탯";
	char szStr[_MAX_FNAME];
	char szStrRace[_MAX_FNAME];
	char szStrRace2[_MAX_FNAME] = "";
	char szStrClass[_MAX_FNAME];
	int iClass, iClassS, iClassE, iClassCount;
	int iRace, iRaceS, iRaceE, iRaceCount;

	flag = GetIsLogFile();
	ScrFlag = GetIsLogScreen();

	//	종족 이름 가져오기
	strcpy(szStrRace, pBalance->GetRaceStat(gsLevelStat.mRace)->GetName());
	if(gsLevelStat.mRace != gsLevelStat.mRace2) 
		sprintf(szStrRace2, "~%s 평균", pBalance->GetRaceStat(gsLevelStat.mRace2)->GetName());

	//	직업 이름 가져오기
	if(gsLevelStat.mClass-giTCLASSCHART_COUNT<0) 
		strcpy(szStrClass, pBalance->GetClassStat(gsLevelStat.mClass)->GetName());
	else
		strcpy(szStrClass, pBalance->GetClassName(gsLevelStat.mClass-giTCLASSCHART_COUNT));

	sprintf(szStr, "%s (%s%s-%s)", szName, szStrRace, szStrRace2, szStrClass);

	ClearTextOut();
	TextoutScreen("\t\t\t< %s >\r\n", szStr);
	sprintf(szFname, "%s\\%s.txt", PathName, szStr);
	if(gsLevelStat.mIsFile==TRUE) LogFileStart(szFname);
	SetIsLogFile(TRUE);
	SetIsLogScreen(FALSE);

	//	직업 전체
	if(gsLevelStat.mClass-giTCLASSCHART_COUNT==0) 
	{
		iClassS = 0;
		iClassE = 5;
		iClassCount = 6;
	}
	//	물리 전체
	else if(gsLevelStat.mClass-giTCLASSCHART_COUNT==1) 
	{
		iClassS = 0;
		iClassE = 2;
		iClassCount = 3;
	}
	//	마법 전체
	else if(gsLevelStat.mClass-giTCLASSCHART_COUNT==2) 
	{
		iClassS = 3;
		iClassE = 5;
		iClassCount = 3;
	}
	//	단일 직업 선택
	else 
	{
		iClassS = gsLevelStat.mClass;
		iClassE = gsLevelStat.mClass;
		iClassCount = 1;
	}

	iRaceS = gsLevelStat.mRace;
	iRaceE = gsLevelStat.mRace2;
	iRaceCount = gsLevelStat.mRace2-gsLevelStat.mRace+1;


	TextoutScreen("%5s%5s%5s%5s%5s%5s%5s%5s %7s%5s%7s%9s%5s%5s%9s%5s%7s%9s%9s%9s %7s%7s%9s%7s%9s%5s%7s%9s%9s%9s %13s%9s%9s%9s%9s%9s %13s%11s%11s%11s%9s%9s %13s%9s%9s%9s%9s%9s %13s%9s%9s%9s%9s%9s %7s%7s%9s"
		, "레벨", "힘", "민첩", "체력", "지능", "지혜" , "정신", "합계"
//		, "", "", "", "", "", "" , "", ""
		, " 물리 ", "소지", "원거리", "  일반  ", "공격", "방어", "  물리  ", "HP ", "  HP  ", "  물리  ", "평균물리", "Item물리"
//		, "공격력", "무게", "공격력", "공격속도", "등급", "등급", "치명확률", "   ", "회복율", "상태저항", "치명최대", " 방어력 "
		, " 마법 ", " 저주 ", "  마법  ", " 마법 ", "  마법  ", "MP ", "  MP  ", "  마법  ", "평균마법", "Item마법"
//		, "공격력", "성공률", "시전속도", "성공율", "치명확률", "   ", "회복율", "상태저항", "치명최대", " 방어력 "
		, "  초당물리  ", "초당물리", "초당물리", "평균물리", "Item물리", "Item속도"
//		, "공격력(치명)", " 공격력 ", " 데미지 ", "공격횟수"
		, " 초당원거리 ", "초당원거리", "초당원거리", "평균원거리", "Item원", "Item속도"
//		, "공격력(치명)", "  공격력  ", "  데미지  ", " 공격횟수"
		, "  초당마법  ", "초당마법", "초당마법", "평균마법", "Item마법", "Item속도"
//		, "공격력(치명)", " 공격력 ", " 데미지 ", "공격횟수"
		, "  초당마원  ", "초당마원", "초당마원", "평균마원", "Item마원", "Item속도"
//		, "공격력(치명)", " 공격력 ", " 데미지 ", "공격횟수"
		, "HP회복", "MP회복", " 최소  "
//		, " 시간 ", " 시간 ", "공격횟수"
		);

	TextoutScreen("%5s%5s%5s%5s%5s%5s%5s%5s %7s%5s%7s%9s%5s%5s%9s%5s%7s%9s%9s%9s %7s%7s%9s%7s%9s%5s%7s%9s%9s%9s %13s%9s%9s%9s%9s%9s %13s%11s%11s%11s%9s%9s %13s%9s%9s%9s%9s%9s %13s%9s%9s%9s%9s%9s %7s%7s%9s"
		, "", "", "", "", "", "" , "", ""
		, "공격력", "무게", "공격력", "공격속도", "등급", "등급", "치명확률", "   ", "회복율", "상태저항", "치명최대", " 방어력 "
		, "공격력", "성공률", "시전속도", "성공율", "치명확률", "   ", "회복율", "상태저항", "치명최대", " 방어력 "
		, "공격력(치명)", " 공격력 ", " 데미지 ", "공격횟수", " ", " "
		, "공격력(치명)", "  공격력  ", "  데미지  ", " 공격횟수", " ", " "
		, "공격력(치명)", " 공격력 ", " 데미지 ", "공격횟수", " ", " "
		, "공격력(치명)", " 공격력 ", " 데미지 ", "공격횟수", " ", " "
		, " 시간 ", " 시간 ", "공격횟수"
		);

	TextoutScreen(" ");


	for(i=1 ; i<=_BAL_LEVELMAX ; i++)
	{
		iStr = iDex = iCon = iInt = iWis = iMen = 0;
		pBalance->InitFactor(pBalance->GetFactorTemp());

		for(iClass = iClassS ; iClass<=iClassE ; iClass++)
		{
			for(iRace = iRaceS ; iRace<=iRaceE ; iRace++)
			{
				iStr += pBalance->GetStat(i, iClass, iRace, 0);
				iDex += pBalance->GetStat(i, iClass, iRace, 1);
				iCon += pBalance->GetStat(i, iClass, iRace, 2);
				iInt += pBalance->GetStat(i, iClass, iRace, 3);
				iWis += pBalance->GetStat(i, iClass, iRace, 4);
				iMen += pBalance->GetStat(i, iClass, iRace, 5);

				pBalance->CalcCombat(i, iClass, iRace, i, iClass, iRace);
				pBalance->AddFactor(pBalance->GetFactorTemp(), pBalance->GetFactorOffense());
			}
		}
			iStr = iStr/iClassCount/iRaceCount;
			iDex = iDex/iClassCount/iRaceCount;
			iCon = iCon/iClassCount/iRaceCount;
			iInt = iInt/iClassCount/iRaceCount;
			iWis = iWis/iClassCount/iRaceCount;
			iMen = iMen/iClassCount/iRaceCount;
			iTotal = iStr + iDex + iCon + iInt + iWis + iMen;

			pBalance->AvrFactor(pBalance->GetFactorTemp(), double(iClassCount*iRaceCount));
			pBalance->CopyFactor(pBalance->GetFactorOffense(), pBalance->GetFactorTemp());

			TextoutScreen("%5d%5d%5d%5d%5d%5d%5d%5d %7.2f%5d%7.2f%9d%5d%5d%9d%5d%7d%9d%9d%9d %7.2f%7d%9d%7d%9d%5d%7d%9d%9d%9d %13.2f%9.2f%9.2f%9.2f%9.2f%9.2f %13.2f%11.2f%11.2f%11.2f%9.2f%9.2f %13.2f%9.2f%9.2f%9.2f%9.2f%9.2f %13.2f%9.2f%9.2f%9.2f%9.2f%9.2f %7.2f%7.2f%9.2f"
				, i
				, iStr
				, iDex
				, iCon
				, iInt
				, iWis
				, iMen
				, iTotal
				, pBalance->GetFactorOffense()->GetData(DATA_ATTACKPOINT)
				, pBalance->GetFactorOffense()->GetDataInt(DATA_WEIGHT)
				, pBalance->GetFactorOffense()->GetData(DATA_RANGEATTACKPOINT)
				, pBalance->GetFactorOffense()->GetDataInt(DATA_SPEED)
				, pBalance->GetFactorOffense()->GetDataInt(DATA_ATACKGRADE)
				, pBalance->GetFactorOffense()->GetDataInt(DATA_DEFENSEGRADE)
				, pBalance->GetFactorOffense()->GetDataInt(DATA_CRITICALRATIO)
				, pBalance->GetFactorOffense()->GetDataInt(DATA_HP)
				, pBalance->GetFactorOffense()->GetDataInt(DATA_HPRECOVERY)
				, pBalance->GetFactorOffense()->GetDataInt(DATA_PHYSICALRESIST)
				, pBalance->GetFactorOffense()->GetDataInt(DATA_CRITICALMAX)
				, pBalance->GetFactorOffense()->GetDataInt(DATA_ITEMDEFENSE)

				, pBalance->GetFactorOffense()->GetData(DATA_MAGICATTACKPOINT)
				, pBalance->GetFactorOffense()->GetDataInt(DATA_CURSERATIO)
				, pBalance->GetFactorOffense()->GetDataInt(DATA_MAGICSPEED)
				, pBalance->GetFactorOffense()->GetDataInt(DATA_MAGICRATIO)
				, pBalance->GetFactorOffense()->GetDataInt(DATA_MAGICCRITICALRATIO)
				, pBalance->GetFactorOffense()->GetDataInt(DATA_MP)
				, pBalance->GetFactorOffense()->GetDataInt(DATA_MPRECOVERY)
				, pBalance->GetFactorOffense()->GetDataInt(DATA_MAGICRESIST)
				, pBalance->GetFactorOffense()->GetDataInt(DATA_MAGICCRITICALMAX)
				, pBalance->GetFactorOffense()->GetDataInt(DATA_ITEMMAGICDEFENSE)

				, pBalance->GetFactorOffense()->GetResultData(RDATA_CRITCAL_AP_SEC)
				, pBalance->GetFactorOffense()->GetResultData(RDATA_AP_SEC)
				, pBalance->GetFactorOffense()->GetResultData(RDATA_DAMAGE_SEC)
				, pBalance->GetFactorOffense()->GetResultData(RDATA_ATTACK_COUNT)
				, pBalance->GetFactorOffense()->GetData(DATA_ITEM_AP)
				, pBalance->GetFactorOffense()->GetData(DATA_ITEM_SP)

				, pBalance->GetFactorOffense()->GetResultData(RDATA_CRITCAL_RAP_SEC)
				, pBalance->GetFactorOffense()->GetResultData(RDATA_RAP_SEC)
				, pBalance->GetFactorOffense()->GetResultData(RDATA_RDAMAGE_SEC)
				, pBalance->GetFactorOffense()->GetResultData(RDATA_RATTACK_COUNT)
				, pBalance->GetFactorOffense()->GetData(DATA_ITEM_RAP)
				, pBalance->GetFactorOffense()->GetData(DATA_ITEM_RSP)

				, pBalance->GetFactorOffense()->GetResultData(RDATA_CRITCAL_MAP_SEC)
				, pBalance->GetFactorOffense()->GetResultData(RDATA_MAP_SEC)
				, pBalance->GetFactorOffense()->GetResultData(RDATA_MDAMAGE_SEC)
				, pBalance->GetFactorOffense()->GetResultData(RDATA_MATTACK_COUNT)
				, pBalance->GetFactorOffense()->GetData(DATA_ITEM_MAP)
				, pBalance->GetFactorOffense()->GetData(DATA_ITEM_MSP)

				, pBalance->GetFactorOffense()->GetResultData(RDATA_CRITCAL_MRAP_SEC)
				, pBalance->GetFactorOffense()->GetResultData(RDATA_MRAP_SEC)
				, pBalance->GetFactorOffense()->GetResultData(RDATA_MRDAMAGE_SEC)
				, pBalance->GetFactorOffense()->GetResultData(RDATA_MRATTACK_COUNT)
				, pBalance->GetFactorOffense()->GetData(DATA_ITEM_MRAP)
				, pBalance->GetFactorOffense()->GetData(DATA_ITEM_MRSP)

				, pBalance->GetFactorOffense()->GetResultData(RDATA_HPMAX_RECOVERY_TIME)
				, pBalance->GetFactorOffense()->GetResultData(RDATA_MPMAX_RECOVERY_TIME)
				, pBalance->GetFactorOffense()->GetResultData(RDATA_MIN_ATTACKCOUNT)
				);
		
	}

	SetIsLogScreen(ScrFlag);
	SetIsLogFile(flag);
	LogFileEnd();
	if(gsLevelStat.mIsFile==TRUE) TextoutScreen("\r\n>> 결과는 '%s'에 저장되었습니다.", szFname);
	else TextoutScreen("\r\n>> '보고서를 파일로 출력'을 선택하면 파일로 저장됩니다.");
}

void Combat1(char *PathName)
{
	BOOL flag, ScrFlag;
	char szName[_MAX_FNAME] = "1v1 모의전투";
	char szFname[_MAX_FNAME];

	flag = GetIsLogFile();
	ScrFlag = GetIsLogScreen();

	ClearTextOut();
	TextoutScreen("\t\t\t\t< %s >\r\n", szName);
	sprintf(szFname, "%s\\%s.txt", PathName, szName);
	if(gsLevelStat.mIsFile==TRUE) LogFileStart(szFname);
	SetIsLogFile(TRUE);
	SetIsLogScreen(FALSE);

	pBalance->CalcCombat(gsLevelStat.mLevelAt, gsLevelStat.mClassAt, gsLevelStat.mRaceAt, gsLevelStat.mLevelDf, gsLevelStat.mClassDf, gsLevelStat.mRaceDf);

	TextoutScreen("공격자 : %3d 레벨, %s, %s", gsLevelStat.mLevelAt, pBalance->GetRaceStat(gsLevelStat.mRaceAt)->GetName(), pBalance->GetClassStat(gsLevelStat.mClassAt)->GetName());
	TextoutScreen(" ");
	CombatResult(pBalance->GetFactorOffense(), gsLevelStat.mLevelAt, gsLevelStat.mRaceAt, gsLevelStat.mClassAt);

	TextoutScreen(" ");

	TextoutScreen("방어자 : %3d 레벨, %s, %s", gsLevelStat.mLevelDf, pBalance->GetRaceStat(gsLevelStat.mRaceDf)->GetName(), pBalance->GetClassStat(gsLevelStat.mClassDf)->GetName());
	TextoutScreen(" ");
	CombatResult(pBalance->GetFactorDefense(), gsLevelStat.mLevelDf, gsLevelStat.mRaceDf, gsLevelStat.mClassDf);

	SetIsLogScreen(ScrFlag);
	SetIsLogFile(flag);
	LogFileEnd();
	if(gsLevelStat.mIsFile==TRUE) TextoutScreen("\r\n>> 결과는 '%s'에 저장되었습니다.", szFname);
	else TextoutScreen("\r\n>> '보고서를 파일로 출력'을 선택하면 파일로 저장됩니다.");
}

void CombatResult(LPFactor pFactor, int iLevel, int iRace, int iClass)
{
	int iStr, iDex, iCon, iInt, iWis, iMen, iTotal;
	char szStr[_MAX_FNAME], szStr2[_MAX_FNAME], szStrTemp[_MAX_FNAME];

	iStr = pBalance->GetStat(iLevel, iClass, iRace, 0);
	iDex = pBalance->GetStat(iLevel, iClass, iRace, 1);
	iCon = pBalance->GetStat(iLevel, iClass, iRace, 2);
	iInt = pBalance->GetStat(iLevel, iClass, iRace, 3);
	iWis = pBalance->GetStat(iLevel, iClass, iRace, 4);
	iMen = pBalance->GetStat(iLevel, iClass, iRace, 5);
	iTotal = iStr + iDex + iCon + iInt + iWis + iMen;

	TextoutScreen(" [스탯별 능력치]");
	TextoutScreen(" ");

	TextoutScreen("%5s %3d %17s %.2f", "힘 :", iStr, "물리공격력:", pFactor->GetData(DATA_ATTACKPOINT));
	TextoutScreen("%5s %3s %17s %d", "", "", "소지무게:", pFactor->GetDataInt(DATA_WEIGHT));
	TextoutScreen(" ");

	TextoutScreen("%5s %3d %17s %.2f", "민첩:", iDex, "원거리공격력:", pFactor->GetData(DATA_RANGEATTACKPOINT));
	TextoutScreen("%5s %3s %17s %d", "", "", "일반공격속도:", pFactor->GetDataInt(DATA_SPEED));
	TextoutScreen("%5s %3s %17s %d", "", "", "공격등급:", pFactor->GetDataInt(DATA_ATACKGRADE));
	TextoutScreen("%5s %3s %17s %d", "", "", "방어등급:", pFactor->GetDataInt(DATA_DEFENSEGRADE));
	TextoutScreen("%5s %3s %17s %d", "", "", "물리치명확률:", pFactor->GetDataInt(DATA_CRITICALRATIO));
	TextoutScreen(" ");

	TextoutScreen("%5s %3d %17s %d", "체력:", iCon, "HP:", pFactor->GetDataInt(DATA_HP));
	TextoutScreen("%5s %3s %17s %d", "", "", "HP회복율:", pFactor->GetDataInt(DATA_HPRECOVERY));
	TextoutScreen("%5s %3s %17s %d", "", "", "물리상태저항:", pFactor->GetDataInt(DATA_PHYSICALRESIST));
	TextoutScreen(" ");

	TextoutScreen("%5s %3s %17s %d", "", "", "평균물리치명최대:", pFactor->GetDataInt(DATA_CRITICALMAX));
	TextoutScreen("%5s %3s %17s %d", "", "", "아이템물리방어력:", pFactor->GetDataInt(DATA_ITEMDEFENSE));
	TextoutScreen(" ");

	TextoutScreen(" ");

	TextoutScreen("%5s %3d %17s %.2f", "지능:", iInt, "마법공격력:", pFactor->GetData(DATA_MAGICATTACKPOINT));
	TextoutScreen("%5s %3s %17s %d", "", "", "저주성공율:", pFactor->GetDataInt(DATA_CURSERATIO));
	TextoutScreen(" ");

	TextoutScreen("%5s %3d %17s %.2f", "지혜:", iWis, "마법원거리공격력", pFactor->GetData(DATA_MAGICRATTACKPOINT));
	TextoutScreen("%5s %3s %17s %d", "", "", "마법시전속도", pFactor->GetDataInt(DATA_MAGICSPEED));
	TextoutScreen("%5s %3s %17s %d", "", "", "마법성공율:", pFactor->GetDataInt(DATA_MAGICRATIO));
	TextoutScreen("%5s %3s %17s %d", "", "", "마법치명확률:", pFactor->GetDataInt(DATA_MAGICCRITICALRATIO));
	TextoutScreen(" ");

	TextoutScreen("%5s %3d %17s %d", "정신:", iMen, "MP:", pFactor->GetDataInt(DATA_MP));
	TextoutScreen("%5s %3s %17s %d", "", "", "MP회복율:", pFactor->GetDataInt(DATA_MPRECOVERY));
	TextoutScreen("%5s %3s %17s %d", "", "", "마법상태저항:", pFactor->GetDataInt(DATA_MAGICRESIST));
	TextoutScreen(" ");

	TextoutScreen("%5s %3s %17s %d", "", "", "평균마법치명최대:", pFactor->GetDataInt(DATA_MAGICCRITICALMAX));
	TextoutScreen("%5s %3s %17s %d", "", "", "아이템마법방어력:", pFactor->GetDataInt(DATA_ITEMMAGICDEFENSE));
	TextoutScreen(" ");

	TextoutScreen("%5s %3d", "합계:", iTotal);

	TextoutScreen(" ");

	TextoutScreen(" [아이템과 능력이 적용될 때의 예상 능력치]");
	TextoutScreen(" ");

	//
	TextoutScreen("%27s %.2f", "스탯 물리공격력:", pFactor->GetData(DATA_ATTACKPOINT));
	TextoutScreen("%27s %.2f", "아이템 물리공격력:", pFactor->GetData(DATA_ITEM_AP));
	TextoutScreen("%27s %.2f", "초당물리공격력(치명):", pFactor->GetResultData(RDATA_CRITCAL_AP_SEC));
	TextoutScreen("%27s %.2f", "초당물리공격력:", pFactor->GetResultData(RDATA_AP_SEC));
	TextoutScreen("%27s %.2f", "초당물리데미지:", pFactor->GetResultData(RDATA_DAMAGE_SEC));
	TextoutScreen("%27s %.2f", "평균물리공격횟수:", pFactor->GetResultData(RDATA_ATTACK_COUNT));
	TextoutScreen(" ");

	TextoutScreen("%27s %.2f", "스탯 물리원거리공격력:", pFactor->GetData(DATA_RANGEATTACKPOINT));
	TextoutScreen("%27s %.2f", "아이템 물리원거리공격력:", pFactor->GetData(DATA_ITEM_RAP));
	TextoutScreen("%27s %.2f", "초당원거리공격력(치명):", pFactor->GetResultData(RDATA_CRITCAL_RAP_SEC));
	TextoutScreen("%27s %.2f", "초당원거리공격력:", pFactor->GetResultData(RDATA_RAP_SEC));
	TextoutScreen("%27s %.2f", "초당원거리데미지:", pFactor->GetResultData(RDATA_RDAMAGE_SEC));
	TextoutScreen("%27s %.2f", "평균원거리공격횟수:", pFactor->GetResultData(RDATA_RATTACK_COUNT));
	TextoutScreen(" ");

	TextoutScreen("%27s %.2f", "스탯 마법 공격력:", pFactor->GetData(DATA_MAGICATTACKPOINT));
	TextoutScreen("%27s %.2f", "아이템 마법공격력:", pFactor->GetData(DATA_ITEM_MAP));
	TextoutScreen("%27s %.2f", "초당마법공격력(치명):", pFactor->GetResultData(RDATA_CRITCAL_MAP_SEC));
	TextoutScreen("%27s %.2f", "초당마법공격력:", pFactor->GetResultData(RDATA_MAP_SEC));
	TextoutScreen("%27s %.2f", "초당마법데미지:", pFactor->GetResultData(RDATA_MDAMAGE_SEC));
	TextoutScreen("%27s %.2f", "평균마법공격횟수:", pFactor->GetResultData(RDATA_MATTACK_COUNT));
	TextoutScreen(" ");

	TextoutScreen("%27s %.2f", "스탯 마법원거리공격력:", pFactor->GetData(DATA_MAGICRATTACKPOINT));
	TextoutScreen("%27s %.2f", "아이템 마법원거리공격력:", pFactor->GetData(DATA_ITEM_MRAP));
	TextoutScreen("%27s %.2f", "초당마법원거리공격력(치명):", pFactor->GetResultData(RDATA_CRITCAL_MRAP_SEC));
	TextoutScreen("%27s %.2f", "초당마법원거리공격력:", pFactor->GetResultData(RDATA_MRAP_SEC));
	TextoutScreen("%27s %.2f", "초당마법원거리데미지:", pFactor->GetResultData(RDATA_MRDAMAGE_SEC));
	TextoutScreen("%27s %.2f", "평균마법원거리공격횟수:", pFactor->GetResultData(RDATA_MRATTACK_COUNT));
	TextoutScreen(" ");

	TextoutScreen("%27s %.2f", "HP회복시간:", pFactor->GetResultData(RDATA_HPMAX_RECOVERY_TIME));
	TextoutScreen("%27s %.2f", "MP회복시간:", pFactor->GetResultData(RDATA_MPMAX_RECOVERY_TIME));
	TextoutScreen("%27s %.2f", "최소공격횟수:", pFactor->GetResultData(RDATA_MIN_ATTACKCOUNT));
	TextoutScreen(" ");

	TextoutScreen("%27s %.2f", "물리 디버프 성공률:", pFactor->GetResultData(RDATA_DEBUF_RATIO));
	TextoutScreen("%27s %.2f", "마법 디버프 성공률:", pFactor->GetResultData(RDATA_MDEBUF_RATIO));
	TextoutScreen(" ");

	switch(pFactor->GetResultDataInt(RDATA_FA_TYPE))
	{
		case 0:
			strcpy(szStr, "물리 공격");
			break;
		case 1:
			strcpy(szStr, "원거리 공격");
			break;
		case 2:
			strcpy(szStr, "마법 공격");
			break;
	}
	TextoutScreen(" [단일 상대]");
	TextoutScreen(" ");

	strcpy(szStr2, "");
	strcpy(szStrTemp, "");

	if(pFactor->GetResultDataInt(RDATA_ND_RATTACKCOUNT) > 0)
	{
		sprintf(szStr2, "%s %d회, ", "원거리 공격", pFactor->GetResultDataInt(RDATA_ND_RATTACKCOUNT));
		strcat(szStrTemp, szStr2);
	}
	if(pFactor->GetResultDataInt(RDATA_ND_MRATTACKCOUNT) > 0) 
	{
		sprintf(szStr2, "%s %d회, ", "마법 원거리 공격", pFactor->GetResultDataInt(RDATA_ND_MRATTACKCOUNT));
		strcat(szStrTemp, szStr2);
	}
	if(pFactor->GetResultDataInt(RDATA_ND_MATTACKCOUNT) > 0) 
	{
		sprintf(szStr2, "%s %d회, ", "마법 공격", pFactor->GetResultDataInt(RDATA_ND_MATTACKCOUNT));
		strcat(szStrTemp, szStr2);
	}
	if(pFactor->GetResultDataInt(RDATA_D_RATTACKCOUNT) > 0) 
	{
		sprintf(szStr2, "%s %d회, ", "접근시 원거리 공격", pFactor->GetResultDataInt(RDATA_D_RATTACKCOUNT));
		strcat(szStrTemp, szStr2);
	}


	TextoutScreen("%17s %3d회 : %s%s %d회"
		, "승리 총 공격 횟수", pFactor->GetResultDataInt(RDATA_WIN_ATTACK_COUNT)
		, szStrTemp, szStr, pFactor->GetResultDataInt(RDATA_FA_COUNT));

	strcpy(szStr2, "");
	strcpy(szStrTemp, "");

	if(pFactor->GetResultDataInt(RDATA_ND_RATTACKCOUNT) > 0)
	{
		sprintf(szStr2, "%s %d회, ", "원거리 공격", pFactor->GetResultDataInt(RDATA_ND_RATTACKCOUNT));
		strcat(szStrTemp, szStr2);
	}
	if(pFactor->GetResultDataInt(RDATA_ND_MRATTACKCOUNT) > 0) 
	{
		sprintf(szStr2, "%s %d회, ", "마법 원거리 공격", pFactor->GetResultDataInt(RDATA_ND_MRATTACKCOUNT));
		strcat(szStrTemp, szStr2);
	}
	if(pFactor->GetResultDataInt(RDATA_ND_MATTACKCOUNT) > 0) 
	{
		sprintf(szStr2, "%s %d회, ", "마법 공격", pFactor->GetResultDataInt(RDATA_ND_MATTACKCOUNT));
		strcat(szStrTemp, szStr2);
	}
	if(pFactor->GetResultDataInt(RDATA_D_RATTACKCOUNT) > 0) 
	{
		sprintf(szStr2, "%s %d회, ", "접근시 원거리 공격", pFactor->GetResultDataInt(RDATA_D_RATTACKCOUNT));
		strcat(szStrTemp, szStr2);
	}

	TextoutScreen("%17s %3d회 : %s%s %d회"
		, "실제 총 공격 횟수", pFactor->GetResultDataInt(RDATA_REAL_ATTACK_COUNT)
		, szStrTemp, szStr, pFactor->GetResultDataInt(RDATA_REAL_FA_COUNT));

	TextoutScreen("%23s : %d"
		, "전투시 받은 피해", pFactor->GetResultDataInt(RDATA_COMBAT_DAMAGE) );
	TextoutScreen("%23s : %d"
		, "남은 HP", pFactor->GetResultDataInt(RDATA_REMAIN_HP) );
	TextoutScreen("%23s : %.2f"
		, "HP 회복 시간", pFactor->GetResultData(RDATA_HP_RECOVERY_TIME));
	
	TextoutScreen("%23s : %.2f"
		, "총 전투 소요 시간", pFactor->GetResultData(RDATA_COMBAT_TIME));

	if(pFactor->GetResultDataInt(RDATA_WIN_RESULT) == 0)
		TextoutScreen("              전투 결과 : 승리");
	else
		TextoutScreen("              전투 결과 : 사망(패배)");
	TextoutScreen(" ");

	TextoutScreen(" [연속 상대]");
	TextoutScreen(" ");
	TextoutScreen("%23s : %d"
		, "매회 전투 피해", pFactor->GetResultDataInt(RDATA_COMBAT_DAMAGE) );
	TextoutScreen("%23s : %d"
		, "승리 횟수",pFactor->GetResultDataInt(RDATA_CONTINUOUS_WIN_COUNT) );

	TextoutScreen("%23s : %d"
		, "연속 전투후 HP",pFactor->GetResultDataInt(RDATA_CONTINUOUS_REMAIN_HP) );

	TextoutScreen("%23s : %.2f"
		, "전투후 HP 회복시간",pFactor->GetResultData(RDATA_CONTINUOUS_HP_RECOVERY_TIME) );

	TextoutScreen("%23s : %.2f"
		, "연속 총 전투 시간",pFactor->GetResultData(RDATA_CONTINUOUS_COMBAT_TIME) );

	TextoutScreen("%23s : %.2f"
		, "매 전투 평균 소요 시간",pFactor->GetResultData(RDATA_ONE_COMBAT_TIME) );

	
	TextoutScreen(" ");
}

void ExpLevel(HWND hWnd, char *PathName)
{
	BOOL flag, ScrFlag;
	int i;
	int iMobHuntTime, iMobHuntTotalTime, iMobHuntTimeAvr[_BAL_LEVELMAX+1]={0,};
	int iMobHuntTime_d, iMobHuntTime_h, iMobHuntTime_m, iMobHuntTime_s;
	int iMobHuntTotalTime_d, iMobHuntTotalTime_h, iMobHuntTotalTime_m, iMobHuntTotalTime_s;
	char szName[_MAX_FNAME] = "레벨별 경험치";
	char szFname[_MAX_FNAME];
	char szStr[_MAX_FNAME];
	char szStrRaceAt[_MAX_FNAME];
	char szStrRaceAt2[_MAX_FNAME]="";
	char szStrRaceDf[_MAX_FNAME];
	char szStrRaceDf2[_MAX_FNAME]="";
	char szStrClassAt[_MAX_FNAME];
	char szStrClassDf[_MAX_FNAME];
	char cClassAt, cRaceAt, cClassDf, cRaceDf;
	int iClassAt, iRaceAt, iClassDf, iRaceDf;
	int iClassAts, iRaceAts, iClassDfs, iRaceDfs;
	int iClassAtCount, iRaceAtCount, iClassDfCount, iRaceDfCount;

	flag = GetIsLogFile();
	ScrFlag = GetIsLogScreen();

	//	종족 이름 가져오기
	strcpy(szStrRaceAt, pBalance->GetRaceStat(gsLevelStat.mExpRaceAt)->GetName());
	if(gsLevelStat.mExpRaceAt != gsLevelStat.mExpRaceAt2) 
		sprintf(szStrRaceAt2, "~%s 평균", pBalance->GetRaceStat(gsLevelStat.mExpRaceAt2)->GetName());

	strcpy(szStrRaceDf, pBalance->GetRaceStat(gsLevelStat.mExpRaceDf)->GetName());
	if(gsLevelStat.mExpRaceDf != gsLevelStat.mExpRaceDf2) 
		sprintf(szStrRaceDf2, "~%s 평균", pBalance->GetRaceStat(gsLevelStat.mExpRaceDf2)->GetName());

	//	직업 이름 가져오기
	if(gsLevelStat.mExpClassAt-giTCLASSCHART_COUNT<0) 
		strcpy(szStrClassAt, pBalance->GetClassStat(gsLevelStat.mExpClassAt)->GetName());
	else
		strcpy(szStrClassAt, pBalance->GetClassName(gsLevelStat.mExpClassAt-giTCLASSCHART_COUNT));

	if(gsLevelStat.mExpClassDf-giTCLASSCHART_COUNT<0) 
		strcpy(szStrClassDf, pBalance->GetClassStat(gsLevelStat.mExpClassDf)->GetName());
	else
		strcpy(szStrClassDf, pBalance->GetClassName(gsLevelStat.mExpClassDf-giTCLASSCHART_COUNT));

	sprintf(szStr, "%s (%s%s-%s) vs (%s%s-%s)", szName, szStrRaceAt, szStrRaceAt2, szStrClassAt, szStrRaceDf, szStrRaceDf2, szStrClassDf);

	ClearTextOut();
	TextoutScreen("\t\t< %s >\r\n", szStr);
	sprintf(szFname, "%s\\%s.txt", PathName, szStr);
	if(gsLevelStat.mIsFile==TRUE) LogFileStart(szFname);
	SetIsLogFile(TRUE);
	SetIsLogScreen(FALSE);

	//	출력전 계산

	//	공격자 직업 전체
	if(gsLevelStat.mExpClassAt-giTCLASSCHART_COUNT==0) 
	{
		iClassAts = 0;
		cClassAt = 5;
		iClassAtCount = 6;
	}
	//	공격자 물리 전체
	else if(gsLevelStat.mExpClassAt-giTCLASSCHART_COUNT==1) 
	{
		iClassAts = 0;
		cClassAt = 2;
		iClassAtCount = 3;
	}
	//	공격자 마법 전체
	else if(gsLevelStat.mExpClassAt-giTCLASSCHART_COUNT==2) 
	{
		iClassAts = 3;
		cClassAt = 5;
		iClassAtCount = 3;
	}
	//	공격자 단일 직업 선택
	else 
	{
		iClassAts = gsLevelStat.mExpClassAt;
		cClassAt = gsLevelStat.mExpClassAt;
		iClassAtCount = 1;
	}

	//	공격자 평균	종족 선택
	iRaceAts = gsLevelStat.mExpRaceAt;
	cRaceAt = gsLevelStat.mExpRaceAt2;
	iRaceAtCount = gsLevelStat.mExpRaceAt2 - gsLevelStat.mExpRaceAt + 1;

	//	방어자 직업 전체
	if(gsLevelStat.mExpClassDf-giTCLASSCHART_COUNT==0) 
	{
		iClassDfs = 0;
		cClassDf = 5;
		iClassDfCount = 6;
	}
	//	방어자 물리 전체
	else if(gsLevelStat.mExpClassDf-giTCLASSCHART_COUNT==1) 
	{
		iClassDfs = 0;
		cClassDf = 2;
		iClassDfCount = 3;
	}
	//	방어자 마법 전체
	else if(gsLevelStat.mExpClassDf-giTCLASSCHART_COUNT==2) 
	{
		iClassDfs = 3;
		cClassDf = 5;
		iClassDfCount = 3;
	}
	//	방어자 단일 직업 선택
	else 
	{
		iClassDfs = gsLevelStat.mExpClassDf;
		cClassDf = gsLevelStat.mExpClassDf;
		iClassDfCount = 1;
	}

	//	방어자 평균	종족 선택
	iRaceDfs = gsLevelStat.mExpRaceDf;
	cRaceDf = gsLevelStat.mExpRaceDf2;
	iRaceDfCount = gsLevelStat.mExpRaceDf2 - gsLevelStat.mExpRaceDf+ 1;


///////////////	전체 계산 추가
	for(i=1 ; i<=_BAL_VIEWLEVELMAX ; i++)
	{
//		pBalance->CalcCombat(i, cClassAt, cRaceAt, i, cClassDf, cRaceDf, i, i);
		for(iRaceDf=iRaceDfs ; iRaceDf<=cRaceDf ; iRaceDf++)
		for(iClassDf=iClassDfs ; iClassDf<=cClassDf ; iClassDf++)
		{
			for(iRaceAt=iRaceAts ; iRaceAt<=cRaceAt ; iRaceAt++)
			for(iClassAt=iClassAts ; iClassAt<=cClassAt ; iClassAt++)
			{
				pBalance->CalcWinCombat(i, iClassAt, iRaceAt, i, iClassDf, iRaceDf, i, i);
				iMobHuntTimeAvr[i] += int((pBalance->GetFactorOffense(i)->GetResultData(RDATA_ONE_COMBAT_TIME)+0.9999));
			}
		}
		iMobHuntTimeAvr[i] = int(iMobHuntTimeAvr[i]/iClassAtCount/(iRaceAtCount*iClassDfCount*iRaceDfCount)*pBalance->GetBalance(i)->GetMobHunt());
//		SendMessage(hWnd, PBM_SETPOS, (WPARAM)i, 0);
		SendMessage(hWnd, PBM_SETPOS, (WPARAM)int(i*100/_BAL_VIEWLEVELMAX), 0);
	}

	//	출력
	TextoutScreen("%5s%9s%11s%7s%9s%9s%17s%16s%16s"
		, "레벨", "경험요구", "누적경험", "몹경험", "몹사냥", "총몹사냥" , "다음레벨시간", "총소요시간", "총획득 골드");
	TextoutScreen("%5s%9s%11s%7s%9s%9s%7s%4s%3s%3s%7s%4s%3s%3s"
		, "", "", "", "", "", "" , " 일 " , "시간" , "분" , "초", " 일 " , "시간" , "분" , "초");
	TextoutScreen(" ");

	iMobHuntTotalTime = 0;
	for(i=1 ; i<=_BAL_VIEWLEVELMAX ; i++)
	{
//		iMobHuntTime = int(pBalance->GetBalance(i)->GetMobHunt()*pBalance->GetFactorOffense(i)->GetResultData(RDATA_ONE_COMBAT_TIME)+0.9999);
		iMobHuntTime = iMobHuntTimeAvr[i];
		iMobHuntTime_d = int(iMobHuntTime/3600/24);
		iMobHuntTime_h = int(iMobHuntTime/3600) - iMobHuntTime_d*24;
		iMobHuntTime_m = int(iMobHuntTime/60) - iMobHuntTime_h*60 - iMobHuntTime_d*24*60;
		iMobHuntTime_s = iMobHuntTime - iMobHuntTime_m*60 - iMobHuntTime_h*60*60 - iMobHuntTime_d*24*60*60;
		iMobHuntTotalTime += iMobHuntTime;
		iMobHuntTotalTime_d = int(iMobHuntTotalTime/3600/24);
		iMobHuntTotalTime_h = int(iMobHuntTotalTime/3600) - iMobHuntTotalTime_d*24;
		iMobHuntTotalTime_m = int(iMobHuntTotalTime/60) - iMobHuntTotalTime_h*60 - iMobHuntTotalTime_d*24*60;
		iMobHuntTotalTime_s = iMobHuntTotalTime - iMobHuntTotalTime_m*60 - iMobHuntTotalTime_h*60*60 - iMobHuntTotalTime_d*24*60*60;
		TextoutScreen("%4d %9d%11d%7d%9d%9d%7d%4d%3d%3d%7d%4d%3d%3d%16d"
			, pBalance->GetBalance(i)->GetLevel()
			, pBalance->GetBalance(i)->GetExp()
			, pBalance->GetBalance(i)->GetExpTotal()
			, pBalance->GetBalance(i)->GetMobExp()
			, pBalance->GetBalance(i)->GetMobHunt()
			, pBalance->GetBalance(i)->GetMobHuntTotal()
			, iMobHuntTime_d
			, iMobHuntTime_h
			, iMobHuntTime_m
			, iMobHuntTime_s
			, iMobHuntTotalTime_d
			, iMobHuntTotalTime_h
			, iMobHuntTotalTime_m
			, iMobHuntTotalTime_s
			, pBalance->GetBalance(i)->GetGoldTotal()
			);
	}
	SetIsLogScreen(ScrFlag);
	SetIsLogFile(flag);
	LogFileEnd();
	if(gsLevelStat.mIsFile==TRUE) TextoutScreen("\r\n>> 결과는 '%s'에 저장되었습니다.", szFname);
	else TextoutScreen("\r\n>> '보고서를 파일로 출력'을 선택하면 파일로 저장됩니다.");
}

void AllExpLevel(HWND hWnd, char *PathName)
{
	BOOL flag, ScrFlag;
	int i;
	int iMobHuntTime, iMobHuntTotalTime, iMobHuntTimeAvr[_BAL_LEVELMAX+1]={0,};
	int iMobHuntTime_d, iMobHuntTime_h, iMobHuntTime_m, iMobHuntTime_s;
	int iMobHuntTotalTime_d, iMobHuntTotalTime_h, iMobHuntTotalTime_m, iMobHuntTotalTime_s;
	char szName[_MAX_FNAME] = "레벨별 경험치";
	char szFname[_MAX_FNAME];
	char szStr[_MAX_FNAME];
	char szStrRaceAt[_MAX_FNAME];
	char szStrRaceAt2[_MAX_FNAME]="";
	char szStrRaceDf2[_MAX_FNAME]="";
	char szStrClassAt[_MAX_FNAME];
	char szStrClassDf[_MAX_FNAME];
	char cClassAt, cRaceAt, cClassDf, cRaceDf;
	int iClassAt, iRaceAt, iClassDf, iRaceDf;
	int iClassAts, iRaceAts, iClassDfs, iRaceDfs;
	int iClassAtCount, iRaceAtCount, iClassDfCount, iRaceDfCount;

	flag = GetIsLogFile();
	ScrFlag = GetIsLogScreen();

	//	종족 이름 가져오기
	strcpy(szStrRaceAt, pBalance->GetRaceStat(gsLevelStat.mAllExpRaceAt)->GetName());
	if(gsLevelStat.mAllExpRaceAt != gsLevelStat.mAllExpRaceAt2) 
		sprintf(szStrRaceAt2, "~%s 평균", pBalance->GetRaceStat(gsLevelStat.mAllExpRaceAt2)->GetName());

//	strcpy(szStrRaceDf, pBalance->GetRaceStat(gsLevelStat.mExpRaceDf)->GetName());
//	if(gsLevelStat.mExpRaceDf != gsLevelStat.mExpRaceDf2) 
		sprintf(szStrRaceDf2, "레벨별 적정 몬스터");
//		sprintf(szStrRaceDf2, "~%s 평균", pBalance->GetRaceStat(gsLevelStat.mExpRaceDf2)->GetName());

	//	직업 이름 가져오기
	if(gsLevelStat.mAllExpClassAt-giTCLASSCHART_COUNT<0) 
		strcpy(szStrClassAt, pBalance->GetClassStat(gsLevelStat.mAllExpClassAt)->GetName());
	else
		strcpy(szStrClassAt, pBalance->GetClassName(gsLevelStat.mAllExpClassAt-giTCLASSCHART_COUNT));

	if(gsLevelStat.mAllExpClassDf-giTCLASSCHART_COUNT<0) 
		strcpy(szStrClassDf, pBalance->GetClassStat(gsLevelStat.mAllExpClassDf)->GetName());
	else
		strcpy(szStrClassDf, pBalance->GetClassName(gsLevelStat.mAllExpClassDf-giTCLASSCHART_COUNT));

	sprintf(szStr, "%s (%s%s-%s) vs (%s-%s)", szName, szStrRaceAt, szStrRaceAt2, szStrClassAt, szStrRaceDf2, szStrClassDf);

	ClearTextOut();
	TextoutScreen("\t\t< %s >\r\n", szStr);
	sprintf(szFname, "%s\\%s.txt", PathName, szStr);
	if(gsLevelStat.mIsFile==TRUE) LogFileStart(szFname);
	SetIsLogFile(TRUE);
	SetIsLogScreen(FALSE);

	//	출력전 계산

	//	공격자 직업 전체
	if(gsLevelStat.mAllExpClassAt-giTCLASSCHART_COUNT==0) 
	{
		iClassAts = 0;
		cClassAt = 5;
		iClassAtCount = 6;
	}
	//	공격자 물리 전체
	else if(gsLevelStat.mAllExpClassAt-giTCLASSCHART_COUNT==1) 
	{
		iClassAts = 0;
		cClassAt = 2;
		iClassAtCount = 3;
	}
	//	공격자 마법 전체
	else if(gsLevelStat.mAllExpClassAt-giTCLASSCHART_COUNT==2) 
	{
		iClassAts = 3;
		cClassAt = 5;
		iClassAtCount = 3;
	}
	//	공격자 단일 직업 선택
	else 
	{
		iClassAts = gsLevelStat.mAllExpClassAt;
		cClassAt = gsLevelStat.mAllExpClassAt;
		iClassAtCount = 1;
	}

	//	공격자 평균	종족 선택
	iRaceAts = gsLevelStat.mAllExpRaceAt;
	cRaceAt = gsLevelStat.mAllExpRaceAt2;
	iRaceAtCount = gsLevelStat.mAllExpRaceAt2 - gsLevelStat.mAllExpRaceAt + 1;

	//	방어자 직업 전체
	if(gsLevelStat.mAllExpClassDf-giTCLASSCHART_COUNT==0) 
	{
		iClassDfs = 0;
		cClassDf = 5;
		iClassDfCount = 6;
	}
	//	방어자 물리 전체
	else if(gsLevelStat.mAllExpClassDf-giTCLASSCHART_COUNT==1) 
	{
		iClassDfs = 0;
		cClassDf = 2;
		iClassDfCount = 3;
	}
	//	방어자 마법 전체
	else if(gsLevelStat.mAllExpClassDf-giTCLASSCHART_COUNT==2) 
	{
		iClassDfs = 3;
		cClassDf = 5;
		iClassDfCount = 3;
	}
	//	방어자 단일 직업 선택
	else 
	{
		iClassDfs = gsLevelStat.mAllExpClassDf;
		cClassDf = gsLevelStat.mAllExpClassDf;
		iClassDfCount = 1;
	}

	//	방어자 평균	종족 선택
//	iRaceDfs = 5;
//	cRaceDf = 6;
	iRaceDfCount = 2;


///////////////	전체 계산 추가
	for(i=1 ; i<=_BAL_VIEWLEVELMAX ; i++)
	{
//	전체 레벨 모의 실험용
//		iRaceDfs = 5+int((min(i, 80)-1)/9)*2;
		iRaceDfs = 5+int((min(i+3, 80)-1)*7/79)*2;

/*	30레벨까지 모의 실험용
		if(1<=i && i<=9)
			iRaceDfs = 5;
		if(10<=i && i<=18)
			iRaceDfs = 7;
		if(10<=i && i<=18)
			iRaceDfs = 9;
		if(19<=i && i<=25)
			iRaceDfs = 11;
		if(26<=i && i<=29)
			iRaceDfs = 13;
		if(30<=i)
			iRaceDfs = 15;
//*/

/*	80레벨까지 모의 실험용
		if(1<=i && i<=9)
			iRaceDfs = 5;
		if(10<=i && i<=18)
			iRaceDfs = 7;
		if(10<=i && i<=18)
			iRaceDfs = 9;
		if(19<=i && i<=25)
			iRaceDfs = 11;
		if(26<=i && i<=29)
			iRaceDfs = 13;
		if(30<=i)
			iRaceDfs = 15;
//*/
		cRaceDf = iRaceDfs+1;
		for(iRaceDf=iRaceDfs ; iRaceDf<=cRaceDf ; iRaceDf++)
		for(iClassDf=iClassDfs ; iClassDf<=cClassDf ; iClassDf++)
		{
			for(iRaceAt=iRaceAts ; iRaceAt<=cRaceAt ; iRaceAt++)
			for(iClassAt=iClassAts ; iClassAt<=cClassAt ; iClassAt++)
			{
				pBalance->CalcWinCombat(i, iClassAt, iRaceAt, i, iClassDf, iRaceDf, i, i);
				iMobHuntTimeAvr[i] += int((pBalance->GetFactorOffense(i)->GetResultData(RDATA_ONE_COMBAT_TIME)+0.9999));
			}
		}
		iMobHuntTimeAvr[i] = int(iMobHuntTimeAvr[i]/(iClassAtCount*iRaceAtCount*iClassDfCount*iRaceDfCount)*pBalance->GetBalance(i)->GetMobHunt());
//		iMobHuntTimeAvr[i] = int(iMobHuntTimeAvr[i]/iClassAtCount/(iRaceAtCount*iClassDfCount*iRaceDfCount)*pBalance->GetBalance(i)->GetMobHunt());
//		SendMessage(hWnd, PBM_SETPOS, (WPARAM)i, 0);
		SendMessage(hWnd, PBM_SETPOS, (WPARAM)int(i*100/_BAL_VIEWLEVELMAX), 0);
	}

	//	출력
	TextoutScreen("%5s%9s%11s%7s%9s%9s%17s%16s%16s"
		, "레벨", "경험요구", "누적경험", "몹경험", "몹사냥", "총몹사냥" , "다음레벨시간", "총소요시간", "총획득 골드");
	TextoutScreen("%5s%9s%11s%7s%9s%9s%7s%4s%3s%3s%7s%4s%3s%3s"
		, "", "", "", "", "", "" , " 일 " , "시간" , "분" , "초", " 일 " , "시간" , "분" , "초");
	TextoutScreen(" ");

	iMobHuntTotalTime = 0;
	for(i=1 ; i<=_BAL_VIEWLEVELMAX ; i++)
	{
//		iMobHuntTime = int(pBalance->GetBalance(i)->GetMobHunt()*pBalance->GetFactorOffense(i)->GetResultData(RDATA_ONE_COMBAT_TIME)+0.9999);
		iMobHuntTime = iMobHuntTimeAvr[i];
		iMobHuntTime_d = int(iMobHuntTime/3600/24);
		iMobHuntTime_h = int(iMobHuntTime/3600) - iMobHuntTime_d*24;
		iMobHuntTime_m = int(iMobHuntTime/60) - iMobHuntTime_h*60 - iMobHuntTime_d*24*60;
		iMobHuntTime_s = iMobHuntTime - iMobHuntTime_m*60 - iMobHuntTime_h*60*60 - iMobHuntTime_d*24*60*60;
		iMobHuntTotalTime += iMobHuntTime;
		iMobHuntTotalTime_d = int(iMobHuntTotalTime/3600/24);
		iMobHuntTotalTime_h = int(iMobHuntTotalTime/3600) - iMobHuntTotalTime_d*24;
		iMobHuntTotalTime_m = int(iMobHuntTotalTime/60) - iMobHuntTotalTime_h*60 - iMobHuntTotalTime_d*24*60;
		iMobHuntTotalTime_s = iMobHuntTotalTime - iMobHuntTotalTime_m*60 - iMobHuntTotalTime_h*60*60 - iMobHuntTotalTime_d*24*60*60;
		TextoutScreen("%4d %9d%11d%7d%9d%9d%7d%4d%3d%3d%7d%4d%3d%3d%16d"
			, pBalance->GetBalance(i)->GetLevel()
			, pBalance->GetBalance(i)->GetExp()
			, pBalance->GetBalance(i)->GetExpTotal()
			, pBalance->GetBalance(i)->GetMobExp()
			, pBalance->GetBalance(i)->GetMobHunt()
			, pBalance->GetBalance(i)->GetMobHuntTotal()
			, iMobHuntTime_d
			, iMobHuntTime_h
			, iMobHuntTime_m
			, iMobHuntTime_s
			, iMobHuntTotalTime_d
			, iMobHuntTotalTime_h
			, iMobHuntTotalTime_m
			, iMobHuntTotalTime_s
			, pBalance->GetBalance(i)->GetGoldTotal()
			);
	}
	SetIsLogScreen(ScrFlag);
	SetIsLogFile(flag);
	LogFileEnd();
	if(gsLevelStat.mIsFile==TRUE) TextoutScreen("\r\n>> 결과는 '%s'에 저장되었습니다.", szFname);
	else TextoutScreen("\r\n>> '보고서를 파일로 출력'을 선택하면 파일로 저장됩니다.");
}