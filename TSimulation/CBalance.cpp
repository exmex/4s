#include "stdafx.h"
#include <math.h>
#include <stdio.h>

#include "CBalance.h"
#include "ChartDB.h"


CStat::CStat()
{
	ZeroMemory(this, sizeof(this));
}

CStat::~CStat()
{
}

void CStat::Init(void)
{
	int i;

	mId = 0;
	for(i=0 ; i<7 ; i++)
	{
		mStatus[i] = 0;
	}
	strcpy(mName, "[이름없음]");
	for(i=0 ; i<8 ; i++)
	{
		mRange[i] = 0;
	}
	mUseItem = 0;
	mUseItemArmor = 0;
	mfSpeed = 4.95;
}

BOOL CStat::Save(FILE * fp)
// 저장
{
	if (fp == NULL) return FALSE;

	if(fwrite(&mId, sizeof(mId), 1, fp) != 1) return FALSE;
	if(fwrite(mStatus, sizeof(mStatus), 1, fp) != 1) return FALSE;
	if(fwrite(mName, sizeof(mName), 1, fp) != 1) return FALSE;
	if(fwrite(mRange, sizeof(mRange), 1, fp) != 1) return FALSE;
	if(fwrite(&mUseItem, sizeof(mUseItem), 1, fp) != 1) return FALSE;
	if(fwrite(&mUseItemArmor, sizeof(mUseItemArmor), 1, fp) != 1) return FALSE;
	if(fwrite(&mfSpeed, sizeof(mfSpeed), 1, fp) != 1) return FALSE;

	return TRUE;
}

BOOL CStat::Load(FILE * fp)
// 열기
{
	if (fp == NULL) return FALSE;

	if(fread(&mId, sizeof(mId), 1, fp) != 1) return FALSE;
	if(fread(mStatus, sizeof(mStatus), 1, fp) != 1) return FALSE;
	if(fread(mName, sizeof(mName), 1, fp) != 1) return FALSE;
	if(fread(mRange, sizeof(mRange), 1, fp) != 1) return FALSE;
	if(fread(&mUseItem, sizeof(mUseItem), 1, fp) != 1) return FALSE;
	if(fread(&mUseItemArmor, sizeof(mUseItemArmor), 1, fp) != 1) return FALSE;
	if(fread(&mfSpeed, sizeof(mfSpeed), 1, fp) != 1) return FALSE;

	return TRUE;
}

void CStat::CalcTotal(void)
{
	SetTotal(GetStat(0)+GetStat(1)+GetStat(2)+GetStat(3)+GetStat(4)+GetStat(5));
}


CFormula::CFormula()
{
	ZeroMemory(this, sizeof(this));
}

CFormula::~CFormula()
{
}

BOOL CFormula::Save(FILE * fp)
// 저장
{
	if (fp == NULL) return FALSE;

	if(fwrite(&mId, sizeof(mId), 1, fp) != 1) return FALSE;
	if(fwrite(mName, sizeof(mName), 1, fp) != 1) return FALSE;
	if(fwrite(&mInit, sizeof(mInit), 1, fp) != 1) return FALSE;
	if(fwrite(&mRateX, sizeof(mRateX), 1, fp) != 1) return FALSE;
	if(fwrite(&mRateY, sizeof(mRateY), 1, fp) != 1) return FALSE;

	return TRUE;
}

BOOL CFormula::Load(FILE * fp)
// 열기
{
	if (fp == NULL) return FALSE;

	if(fread(&mId, sizeof(mId), 1, fp) != 1) return FALSE;
	if(fread(mName, sizeof(mName), 1, fp) != 1) return FALSE;
	if(fread(&mInit, sizeof(mInit), 1, fp) != 1) return FALSE;
	if(fread(&mRateX, sizeof(mRateX), 1, fp) != 1) return FALSE;
	if(fread(&mRateY, sizeof(mRateY), 1, fp) != 1) return FALSE;

	return TRUE;
}

CFactor::CFactor()
{
	ZeroMemory(this, sizeof(this));
}

CFactor::~CFactor()
{
}

CCharSkill::CCharSkill()
{
	ZeroMemory(this, sizeof(this));
}

CCharSkill::~CCharSkill()
{
}

CBalance::CBalance()
{
	ZeroMemory(this, sizeof(this));
}

CBalance::~CBalance()
{
}

CBalancePool::CBalancePool(void)
{
	ZeroMemory(this, sizeof(this));
	Init();
}

CBalancePool::~CBalancePool(void)
{
	int i;

	for(i=0 ; i<=_BAL_LEVELMAX ; i++)
	{
		delete mFactorDefense[i];
	}

	for(i=0 ; i<=_BAL_LEVELMAX ; i++)
	{
		delete mFactorOffense[i];
	}

	delete mFactor;
	delete mFactorTemp;

	for(i=0 ; i<=_BAL_LEVELMAX ; i++)
	{
		delete mBalance[i];
	}
	for(i=0 ; i<DATA_FACTOR_MAX ; i++)
	{
		delete mFormula[i];
	}
	for(i=0 ; i<DATA_RACE_MAX ; i++)
	{
		delete mRaceStat[i];
	}
	for(i=0 ; i<DATA_CLASSSTAT_MAX ; i++)
	{
		delete mClassStat[i];
	}
	for(i=0 ; i<DATA_BASESTAT_MAX ; i++)
	{
		delete mBaseStat[i];
	}
	for(i=0 ; i<DATA_BASESTAT_MAX ; i++)
	{
		delete mClassSkill[i];
	}
}

void CBalancePool::CreateMember(void)
{
	int i;

	for(i=0 ; i<DATA_BASESTAT_MAX ; i++)
	{
		mBaseStat[i] = new CStat();
		ZeroMemory(mBaseStat[i], sizeof(CStat));
	}
	for(i=0 ; i<DATA_CLASSSTAT_MAX ; i++)
	{
		mClassStat[i] = new CStat();
		ZeroMemory(mClassStat[i], sizeof(CStat));
	}
	for(i=0 ; i<DATA_RACE_MAX ; i++)
	{
		mRaceStat[i] = new CStat();
		ZeroMemory(mRaceStat[i], sizeof(CStat));
	}
	for(i=0 ; i<DATA_FACTOR_MAX ; i++)
	{
		mFormula[i] = new CFormula();
		ZeroMemory(mFormula[i], sizeof(CFormula));
	}
	for(i=0 ; i<=_BAL_LEVELMAX ; i++)
	{
		mBalance[i] = new CBalance();
		ZeroMemory(mBalance[i], sizeof(CBalance));
		mBalance[i]->SetLevel(i);
	}

	mFactor = new CFactor();
	ZeroMemory(mFactor, sizeof(CFactor));

	mFactorTemp = new CFactor();
	ZeroMemory(mFactorTemp, sizeof(CFactor));

	for(i=0 ; i<=_BAL_LEVELMAX ; i++)
	{
		mFactorOffense[i] = new CFactor();
		ZeroMemory(mFactorOffense[i], sizeof(CFactor));
	}

	for(i=0 ; i<=_BAL_LEVELMAX ; i++)
	{
		mFactorDefense[i] = new CFactor();
		ZeroMemory(mFactorDefense[i], sizeof(CFactor));
	}

	for(i=0 ; i<=_BAL_LEVELMAX ; i++)
	{
		mClassSkill[i] = new CCharSkill();
		ZeroMemory(mClassSkill[i], sizeof(CCharSkill));
	}



}

void CBalancePool::Init(void)
{
	int i;

	miStatPoint = DATA_STATPOINT;
	mRaceMax=0;
	CreateMember();
	InitName();


	//	Zgl.01. 경험치 밸런스를 위한 데이터 설정 시작

	//	이전 데이터의 레벨별 경험치 요구량 계산
	//	총 경험치와 누적 경험치를 계산한다.

	//	초기값 설정
	mBalance[2]->SetExp(120);
	mBalance[2]->SetExpTotal(120);
	mBalance[2]->SetMobExp(24);
	mBalance[2]->SetMobHunt(5);
	mBalance[2]->SetMobHuntTotal(5);

	for(i=1 ; i<=_BAL_LEVELMAX ; i++)
	{
//		mBalance[i]->SetExp(int(30.*pow(double(i),double(2.1)))
		mBalance[i]->SetExp(int(30.*pow(double(i),double(2.303)))
			+ (((i+1)/10. - int((i+1)/10)==0)? int(pow(double(int((i+1)/10)), double(2.5))*3000) : 0)
			+ ((i>89) ? int(pow(double(i), double(1.88))*3000) : 0)
			+ ((i>98) ? int(pow(double(i), double(2.35))*3000) : 0)
			);
		if(i>=100)
			mBalance[i]->SetExp(0);

		mBalance[i]->SetExpTotal(mBalance[i]->GetExp()+mBalance[i-1]->GetExpTotal());
	}

	for(i=1 ; i<=_BAL_LEVELMAX ; i++)
	{
//		mBalance[i]->SetMobExp(int(43.*log10(double(i+1))+i/6.));
//		mBalance[i]->SetMobExp(int(24.*log10(double(i+1))+i/6.));
		mBalance[i]->SetMobExp(int(30.*log10(double(i+1))+i*2.));
		mBalance[i]->SetMobGold(i*4);
//		=int(43*log(i+1)+i/6)

		//	몹 사냥 수 계산
		//	몹 사냥 수 = 다음 경험치 / 몹 경험치
		if(i<_BAL_LEVELMAX)
		{
			mBalance[i]->SetMobHunt(int(mBalance[i]->GetExp()/1./mBalance[i]->GetMobExp()+0.9999));
			mBalance[i]->SetMobHuntTotal(mBalance[i-1]->GetMobHuntTotal() + mBalance[i]->GetMobHunt());
			mBalance[i]->SetGoldTotal(mBalance[i-1]->GetGoldTotal() + mBalance[i]->GetMobGold()*mBalance[i]->GetMobHunt());
		}
	}
	//	Zgl.01. 경험치 밸런스를 위한 데이터 설정 완료

}

BOOL CBalancePool::SaveFile(LPCSTR szFileName)
// 저장
{
	FILE* fp;
	int i;
	double dVer = _STATFILE_VER;

	fp = fopen(szFileName, "wb");
	if (fp == NULL) return FALSE;

	//	버전 저장
	if(fwrite(&dVer, sizeof(dVer), 1, fp) != 1) return FALSE;
	
	//	헤더 저장
	if(fwrite(&miStatPoint, sizeof(miStatPoint), 1, fp) != 1) return FALSE;
	if(fwrite(&mRaceMax, sizeof(mRaceMax), 1, fp) != 1) return FALSE;

	for(i=0 ; i<DATA_BASESTAT_MAX ; i++)
	{
		if(!mBaseStat[i]->Save(fp)) return FALSE;
	}
	for(i=0 ; i<DATA_CLASSSTAT_MAX ; i++)
	{
		if(!mClassStat[i]->Save(fp)) return FALSE;
	}
	for(i=0 ; i<mRaceMax ; i++)
	{
		if(!mRaceStat[i]->Save(fp)) return FALSE;
	}
	for(i=0 ; i<DATA_FACTOR_MAX ; i++)
	{
		if(!mFormula[i]->Save(fp)) return FALSE;
	}

	fclose(fp);
	return TRUE;

}

BOOL CBalancePool::IsLoadFile(LPCSTR szFileName)
{
	FILE* fp;
//	char str[_MAX_FNAME];
	double dVer;

	fp = fopen(szFileName, "rb");
	if (fp == NULL) return FALSE;

	//	버전 읽기
	if(fread(&dVer, sizeof(dVer), 1, fp) != 1) return FALSE;
	if(dVer!=_STATFILE_VER) return FALSE;

	//	헤더 읽기
	if(fread(&miStatPoint, sizeof(miStatPoint), 1, fp) != 1) return FALSE;
	if(fread(&mRaceMax, sizeof(mRaceMax), 1, fp) != 1) return FALSE;

	fclose(fp);
	return TRUE;
}


BOOL CBalancePool::LoadFile(LPCSTR szFileName)
// 읽기
{
	FILE* fp;
	int i;
	char str[_MAX_FNAME];

	fp = fopen(szFileName, "rb");
	if (fp == NULL) return FALSE;

	//	버전 읽기
	if(fread(str, sizeof(_STATFILE_VER), 1, fp) != 1) return FALSE;

	//	헤더 읽기
	if(fread(&miStatPoint, sizeof(miStatPoint), 1, fp) != 1) return FALSE;
	if(fread(&mRaceMax, sizeof(mRaceMax), 1, fp) != 1) return FALSE;
	for(i=0 ; i<DATA_BASESTAT_MAX ; i++)
	{
		if(!mBaseStat[i]->Load(fp)) return FALSE;
	}
	for(i=0 ; i<DATA_CLASSSTAT_MAX ; i++)
	{
		if(!mClassStat[i]->Load(fp)) return FALSE;
	}
	for(i=0 ; i<mRaceMax ; i++)
	{
		if(!mRaceStat[i]->Load(fp)) return FALSE;
	}
	for(i=0 ; i<DATA_FACTOR_MAX ; i++)
	{
		if(!mFormula[i]->Load(fp)) return FALSE;
	}

	fclose(fp);
	return TRUE;

}

void CBalancePool::InitName(void)
{
	SetClassName(0, "직업전체");
	SetClassName(1, "물리전체");
	SetClassName(2, "마법전체");
	mClassExCount=3;

	SetRaceName(0, "인,수,요 평균");
	mRaceExCount=1;
}

int CBalancePool::GetStat(int Level, int Class, int Race, int Sel)
{
	int iData, iRet;

	iData = GetBaseStat(0)->GetStat(Sel) + GetClassStat(Class)->GetStat(Sel) + GetRaceStat(Race)->GetStat(Sel);

	iRet=iData+int(((Level-1)*GetStatPoint())*iData/100);

	return iRet;
}

void CBalancePool::CalcFactor(int Level, int Class, int Race)
{
	double dData=0.;

	//	스탯을 능력치로 계산하는 공식을 적용한다.

	//	물리공격력
	dData = GetFormula(DATA_ATTACKPOINT)->GetInit() + GetStat(Level, Class, Race, 0) * GetFormula(DATA_ATTACKPOINT)->GetX();
	GetFactor()->SetData(dData, DATA_ATTACKPOINT);
	//	소지무게
	dData = GetFormula(DATA_WEIGHT)->GetInit() + GetStat(Level, Class, Race, 0) * GetFormula(DATA_WEIGHT)->GetX();
	GetFactor()->SetData(dData, DATA_WEIGHT);
	//	원거리공격력
	dData = GetFormula(DATA_RANGEATTACKPOINT)->GetInit() + GetStat(Level, Class, Race, 1) * GetFormula(DATA_RANGEATTACKPOINT)->GetX();
	GetFactor()->SetData(dData, DATA_RANGEATTACKPOINT);
	//	일반공격속도
	dData = GetFormula(DATA_SPEED)->GetInit() + GetStat(Level, Class, Race, 1) * GetFormula(DATA_SPEED)->GetX();
	GetFactor()->SetData(dData, DATA_SPEED);
	//	공격등급
	dData = GetFormula(DATA_ATACKGRADE)->GetInit() + GetStat(Level, Class, Race, 1) * GetFormula(DATA_ATACKGRADE)->GetX();
	GetFactor()->SetData(dData, DATA_ATACKGRADE);
	//	방어등급
	dData = GetFormula(DATA_DEFENSEGRADE)->GetInit() + GetStat(Level, Class, Race, 1) * GetFormula(DATA_DEFENSEGRADE)->GetX();
	GetFactor()->SetData(dData, DATA_DEFENSEGRADE);
	//	물리치명확률
	dData = GetFormula(DATA_CRITICALRATIO)->GetInit() + GetStat(Level, Class, Race, 1) * GetFormula(DATA_CRITICALRATIO)->GetX();
	GetFactor()->SetData(dData, DATA_CRITICALRATIO);
	//	생명력
	dData = GetFormula(DATA_HP)->GetInit() + GetStat(Level, Class, Race, 2) * GetFormula(DATA_HP)->GetX() + pow(double(Level),GetFormula(DATA_HP)->GetY());
	GetFactor()->SetData(dData, DATA_HP);
	//	생명력 회복율
	dData = GetFormula(DATA_HPRECOVERY)->GetInit() + GetStat(Level, Class, Race, 2) * GetFormula(DATA_HPRECOVERY)->GetX() + pow(double(Level),GetFormula(DATA_HPRECOVERY)->GetY());
	GetFactor()->SetData(dData, DATA_HPRECOVERY);
	//	물리상태저항
	dData = GetFormula(DATA_PHYSICALRESIST)->GetInit() + GetStat(Level, Class, Race, 2) * GetFormula(DATA_PHYSICALRESIST)->GetX();
	GetFactor()->SetData(dData, DATA_PHYSICALRESIST);
	//	평균물리치명최대
	dData = GetFormula(DATA_CRITICALMAX)->GetInit() + Level * GetFormula(DATA_CRITICALMAX)->GetX();
	GetFactor()->SetData(dData, DATA_CRITICALMAX);
	//	평균물리방어력
//	dData = GetFormula(DATA_ITEMDEFENSE)->GetInit() + Level * GetFormula(DATA_ITEMDEFENSE)->GetX();
	//	아이템 물리방어력
	dData = 0.;//GetFormula(DATA_ITEMDEFENSE)->GetInit();
	if(GetRaceStat(Race)->GetUseItemArmor()==1)
		dData += CalcDP(Level, Class);
	GetFactor()->SetData(dData, DATA_ITEMDEFENSE);
	//	아이템 물리 공격력
	dData = 0.;
	if(GetRaceStat(Race)->GetUseItem()==1)
		dData = CalcAP(Level, Class);
	GetFactor()->SetData(dData, DATA_ITEM_AP);
	//	아이템 물리 원거리 공격력
	dData = 0.;
	if(GetRaceStat(Race)->GetUseItem()==1)
		dData = CalcRAP(Level, Class);
	GetFactor()->SetData(dData, DATA_ITEM_RAP);
	//	물리 기본 아이템 속도증감
	dData = 100.;
	if(GetRaceStat(Race)->GetUseItem()==1)
		dData = CalcAP(Level, Class, 1);
	GetFactor()->SetData(dData, DATA_ITEM_SP);
	//	물리 원거리 아이템 속도증감
	dData = 100.;
	if(GetRaceStat(Race)->GetUseItem()==1)
		dData = CalcRAP(Level, Class, 1);
	GetFactor()->SetData(dData, DATA_ITEM_RSP);

	//	마법공격력
	dData = GetFormula(DATA_MAGICATTACKPOINT)->GetInit() + GetStat(Level, Class, Race, 3) * GetFormula(DATA_MAGICATTACKPOINT)->GetX();
	GetFactor()->SetData(dData, DATA_MAGICATTACKPOINT);
	//	저주성공율
	dData = GetFormula(DATA_CURSERATIO)->GetInit() + GetStat(Level, Class, Race, 3) * GetFormula(DATA_CURSERATIO)->GetX();
	GetFactor()->SetData(dData, DATA_CURSERATIO);
	//	마법원거리공격력
	dData = GetFormula(DATA_MAGICRATTACKPOINT)->GetInit() + GetStat(Level, Class, Race, 4) * GetFormula(DATA_MAGICRATTACKPOINT)->GetX();
	GetFactor()->SetData(dData, DATA_MAGICRATTACKPOINT);
	//	마법시전속도
	dData = GetFormula(DATA_MAGICSPEED)->GetInit() + GetStat(Level, Class, Race, 4) * GetFormula(DATA_MAGICSPEED)->GetX();
	GetFactor()->SetData(dData, DATA_MAGICSPEED);
	//	마법성공율
	dData = GetFormula(DATA_MAGICRATIO)->GetInit() + GetStat(Level, Class, Race, 4) * GetFormula(DATA_MAGICRATIO)->GetX();
	GetFactor()->SetData(dData, DATA_MAGICRATIO);
	//	마법치명확률
	dData = GetFormula(DATA_MAGICCRITICALRATIO)->GetInit() + GetStat(Level, Class, Race, 4) * GetFormula(DATA_MAGICCRITICALRATIO)->GetX();
	GetFactor()->SetData(dData, DATA_MAGICCRITICALRATIO);
	//	정신력
	dData = GetFormula(DATA_MP)->GetInit() + GetStat(Level, Class, Race, 5) * GetFormula(DATA_MP)->GetX() + pow(double(Level),GetFormula(DATA_MP)->GetY());
	GetFactor()->SetData(dData, DATA_MP);
	//	정신력 회복율
	dData = GetFormula(DATA_MPRECOVERY)->GetInit() + GetStat(Level, Class, Race, 5) * GetFormula(DATA_MPRECOVERY)->GetX() + pow(double(Level),GetFormula(DATA_MPRECOVERY)->GetY());
	GetFactor()->SetData(dData, DATA_MPRECOVERY);
	//	마법상태저항
	dData = GetFormula(DATA_MAGICRESIST)->GetInit() + GetStat(Level, Class, Race, 5) * GetFormula(DATA_MAGICRESIST)->GetX();
	GetFactor()->SetData(dData, DATA_MAGICRESIST);
	//	평균마법치명최대
	dData = GetFormula(DATA_MAGICCRITICALMAX)->GetInit() + Level * GetFormula(DATA_MAGICCRITICALMAX)->GetX();
	GetFactor()->SetData(dData, DATA_MAGICCRITICALMAX);
	//	평균마법방어력
//	dData = GetFormula(DATA_ITEMMAGICDEFENSE)->GetInit() + Level * GetFormula(DATA_ITEMMAGICDEFENSE)->GetX();
	//	아이템 마법방어력
	dData = 0.;//GetFormula(DATA_ITEMMAGICDEFENSE)->GetInit();
	if(GetRaceStat(Race)->GetUseItemArmor()==1)
		dData += CalcMDP(Level, Class);
	GetFactor()->SetData(dData, DATA_ITEMMAGICDEFENSE);
	//	아이템 마법 공격력
	dData = 0.;
	if(GetRaceStat(Race)->GetUseItem()==1)
		dData = CalcMAP(Level, Class);
	GetFactor()->SetData(dData, DATA_ITEM_MAP);
	//	아이템 마법 원거리 공격력
	dData = 0.;
	if(GetRaceStat(Race)->GetUseItem()==1)
		dData = CalcMRAP(Level, Class);
	GetFactor()->SetData(dData, DATA_ITEM_MRAP);
	//	마법 기본 아이템 속도증감
	dData = 100.;
	if(GetRaceStat(Race)->GetUseItem()==1)
		dData = CalcMAP(Level, Class, 1);
	GetFactor()->SetData(dData, DATA_ITEM_MSP);
	//	마법 원거리 아이템 속도증감
	dData = 100.;
	if(GetRaceStat(Race)->GetUseItem()==1)
		dData = CalcMRAP(Level, Class, 1);
	GetFactor()->SetData(dData, DATA_ITEM_MRSP);

	//	직업 번호
	dData = double(Class);
	GetFactor()->SetData(dData, DATA_CLASSNUMBER);
	//	레벨
	dData = double(Level);
	GetFactor()->SetData(dData, DATA_LEVEL);
	//	종족 번호
	dData = double(Race);
	GetFactor()->SetData(dData, DATA_RACENUMBER);

	//	소환수의 초당 물리 공격력
	dData = CalcCreatureAP(Level, Class);
	GetFactor()->SetData(dData, DATA_CREATURE_AP_SEC);
}

void CBalancePool::CalcCombat(int OffLevel, int OffClass, int OffRace, int DefLevel, int DefClass, int DefRace, char OffNum, char DefNum)
{
	double dDataOff[5]={0.,};
	double dDataDef[5]={0.,};
	double dOffDmg=0., dDefDmg=0.;
	double dOffHP, dDefHP;
	double dTemp;

	//	스탯을 능력치로 계산하는 공식을 적용한다.
	CalcFactor(OffLevel, OffClass, OffRace);
	CopyFactorToTarget(GetFactorOffense(OffNum));
//	CopyFactorToOffense(OffNum);
	CalcFactor(DefLevel, DefClass, DefRace);
	CopyFactorToTarget(GetFactorDefense(DefNum));
//	CopyFactorToDefense(DefNum);

	CalcCombatFactor(GetFactorOffense(OffNum), GetFactorDefense(DefNum));
	CalcCombatFactor(GetFactorDefense(DefNum), GetFactorOffense(OffNum));

	switch(GetFactorOffense(OffNum)->GetResultDataInt(RDATA_ATTACK_TYPE))
	{
		//	공격자(선공) 공격 타입	0:물리	1:원거리	2:	마법
		case 0:
			{
				switch(GetFactorDefense(DefNum)->GetResultDataInt(RDATA_ATTACK_TYPE))
				{
					//	방어자(후공) 공격 타입	0:물리	1:원거리	2:	마법
					case 0:
						//	무피해 원거리 공격 횟수
						dTemp = GetClassStat(OffClass)->GetRange(STAT_RRANGE_MAX) - max(GetClassStat(OffClass)->GetRange(STAT_RRANGE_MIN), GetClassStat(DefClass)->GetRange(STAT_RANGE_MAX));
						dTemp = max(dTemp, 0);
//						dDataOff[0] = int(dTemp / GetRaceStat(DefRace)->GetSpeed())+1.;
						dDataOff[0] = int((dTemp / GetRaceStat(DefRace)->GetSpeed())*(GetFactorOffense(OffNum)->GetData(DATA_SPEED)+GetFactorOffense(OffNum)->GetData(DATA_ITEM_RSP)-100)/100)+1.;
						//	무피해 마법 공격 횟수
						dDataOff[1] = 0.;
						//	무피해 마법 원거리 공격 횟수
						dDataOff[2] = 0.;
						//	후속 공격 타입
						dDataOff[3] = 0.;
						//	후속 공격 데미지
						dOffDmg = GetFactorOffense(OffNum)->GetResultData(RDATA_DAMAGE_SEC);

						//	무피해 원거리 공격 횟수
						dDataDef[0] = 0.;
						//	무피해 마법 공격 횟수
						dDataDef[1] = 0.;
						//	무피해 마법 원거리 공격 횟수
						dDataDef[2] = 0.;
						//	후속 공격 타입
						dDataDef[3] = 0.;
						//	후속 공격 데미지
						dDefDmg = GetFactorDefense(DefNum)->GetResultData(RDATA_DAMAGE_SEC);
						break;
					case 1:
						//	무피해 원거리 공격 횟수
						dTemp = GetClassStat(OffClass)->GetRange(STAT_RRANGE_MAX) - max(GetClassStat(OffClass)->GetRange(STAT_RRANGE_MIN), GetClassStat(DefClass)->GetRange(STAT_RRANGE_MAX));
						dTemp = max(dTemp, 0);
//						dDataOff[0] = int(dTemp / GetRaceStat(DefRace)->GetSpeed())+1.;
						dDataOff[0] = int((dTemp / GetRaceStat(DefRace)->GetSpeed())*(GetFactorOffense(OffNum)->GetData(DATA_SPEED)+GetFactorOffense(OffNum)->GetData(DATA_ITEM_RSP)-100)/100)+1.;
						//	무피해 마법 공격 횟수
						dDataOff[1] = 0.;
						//	무피해 마법 원거리 공격 횟수
						dDataOff[2] = 0.;
						//	후속 공격 타입
						dDataOff[3] = 0.;
						//	후속 공격 데미지
						dOffDmg = GetFactorOffense(OffNum)->GetResultData(RDATA_DAMAGE_SEC);
						//	접근시 원거리 공격 횟수
//						dTemp = GetClassStat(OffClass)->GetRange(STAT_RRANGE_MAX) - max(GetClassStat(OffClass)->GetRange(STAT_RRANGE_MIN), GetClassStat(OffClass)->GetRange(STAT_RANGE_MAX));
//						dTemp = max(dTemp, 0);
//						dDataOff[4] = int(dTemp / GetRaceStat(OffRace)->GetSpeed())*1.;
						dDataOff[4] = 0.;

						//	무피해 원거리 공격 횟수
						dTemp = GetClassStat(DefClass)->GetRange(STAT_RRANGE_MAX) - max(GetClassStat(DefClass)->GetRange(STAT_RRANGE_MIN), GetClassStat(OffClass)->GetRange(STAT_RRANGE_MAX));
						dTemp = max(dTemp, 0);
//						dDataDef[0] = int(dTemp / GetRaceStat(OffRace)->GetSpeed())*1.;
						dDataDef[0] = int((dTemp / GetRaceStat(OffRace)->GetSpeed())*(GetFactorDefense(OffNum)->GetData(DATA_SPEED)+GetFactorDefense(OffNum)->GetData(DATA_ITEM_RSP)-100)/100)+1.;
						//	무피해 마법 공격 횟수
						dDataDef[1] = 0.;
						//	무피해 마법 원거리 공격 횟수
						dDataDef[2] = 0.;
						//	후속 공격 타입
						dDataDef[3] = 1.;
						//	후속 공격 데미지
						dDefDmg = GetFactorDefense(DefNum)->GetResultData(RDATA_RDAMAGE_SEC);
						break;
					case 2:
						//	무피해 원거리 공격 횟수
						dTemp = GetClassStat(OffClass)->GetRange(STAT_RRANGE_MAX) - max(GetClassStat(OffClass)->GetRange(STAT_RRANGE_MIN), GetClassStat(DefClass)->GetRange(STAT_MRRANGE_MAX));
						dTemp = max(dTemp, 0);
//						dDataOff[0] = int(dTemp / GetRaceStat(DefRace)->GetSpeed())+1.;
						dDataOff[0] = int((dTemp / GetRaceStat(DefRace)->GetSpeed())*(GetFactorOffense(OffNum)->GetData(DATA_SPEED)+GetFactorOffense(OffNum)->GetData(DATA_ITEM_RSP)-100)/100)+1.;
						//	무피해 마법 공격 횟수
						dDataOff[1] = 0.;
						//	무피해 마법 원거리 공격 횟수
						dDataOff[2] = 0.;
						//	후속 공격 타입
						dDataOff[3] = 0.;
						//	후속 공격 데미지
						dOffDmg = GetFactorOffense(OffNum)->GetResultData(RDATA_DAMAGE_SEC);
						//	접근시 원거리 공격 횟수
//						dTemp = GetClassStat(OffClass)->GetRange(STAT_RRANGE_MAX) - max(GetClassStat(OffClass)->GetRange(STAT_RRANGE_MIN), GetClassStat(OffClass)->GetRange(STAT_RANGE_MAX));
//						dTemp = max(dTemp, 0);
//						dDataOff[4] = int(dTemp / GetRaceStat(OffRace)->GetSpeed())*1.;
						dDataOff[4] = 0.;

						//	무피해 원거리 공격 횟수
						dDataDef[0] = 0.;
						//	무피해 마법 공격 횟수
						dTemp = GetClassStat(DefClass)->GetRange(STAT_MRANGE_MAX) - max(GetClassStat(DefClass)->GetRange(STAT_MRANGE_MIN), GetClassStat(OffClass)->GetRange(STAT_RRANGE_MAX));
						dTemp = max(dTemp, 0);
//						dDataDef[1] = int(dTemp / GetRaceStat(OffRace)->GetSpeed())*1.;
						dDataDef[1] = int((dTemp / GetRaceStat(OffRace)->GetSpeed())*(GetFactorDefense(OffNum)->GetData(DATA_MAGICSPEED)+GetFactorDefense(OffNum)->GetData(DATA_ITEM_MSP)-100)/100)+1.;
						//	무피해 마법 원거리 공격 횟수
						dDataDef[2] = 0.;
						//	후속 공격 타입
						dDataDef[3] = 2.;
						//	후속 공격 데미지
						dDefDmg = GetFactorDefense(DefNum)->GetResultData(RDATA_MDAMAGE_SEC);
						break;
				}
			}
			break;
		case 1:
			{
				switch(GetFactorDefense(DefNum)->GetResultDataInt(RDATA_ATTACK_TYPE))
				{
					//	방어자(후공) 공격 타입	0:물리	1:원거리	2:	마법
					case 0:
						//	무피해 원거리 공격 횟수
						dTemp = GetClassStat(OffClass)->GetRange(STAT_RRANGE_MAX) - max(GetClassStat(OffClass)->GetRange(STAT_RRANGE_MIN), GetClassStat(DefClass)->GetRange(STAT_RANGE_MAX));
						dTemp = max(dTemp, 0);
//						dDataOff[0] = int(dTemp / GetRaceStat(DefRace)->GetSpeed())+1.;
						dDataOff[0] = int((dTemp / GetRaceStat(DefRace)->GetSpeed())*(GetFactorOffense(OffNum)->GetData(DATA_SPEED)+GetFactorOffense(OffNum)->GetData(DATA_ITEM_RSP)-100)/100)+1.;
						//	무피해 마법 공격 횟수
						dDataOff[1] = 0.;
						//	무피해 마법 원거리 공격 횟수
						dDataOff[2] = 0.;
						//	후속 공격 타입
						dDataOff[3] = 0.;
						//	후속 공격 데미지
						dOffDmg = GetFactorOffense(OffNum)->GetResultData(RDATA_DAMAGE_SEC);

						//	무피해 원거리 공격 횟수
						dDataDef[0] = 0.;
						//	무피해 마법 공격 횟수
						dDataDef[1] = 0.;
						//	무피해 마법 원거리 공격 횟수
						dDataDef[2] = 0.;
						//	후속 공격 타입
						dDataDef[3] = 0.;
						//	후속 공격 데미지
						dDefDmg = GetFactorDefense(DefNum)->GetResultData(RDATA_DAMAGE_SEC);
						break;
					case 1:
						//	무피해 원거리 공격 횟수
						dTemp = GetClassStat(OffClass)->GetRange(STAT_RRANGE_MAX) - max(GetClassStat(OffClass)->GetRange(STAT_RRANGE_MIN), GetClassStat(DefClass)->GetRange(STAT_RRANGE_MAX));
						dTemp = max(dTemp, 0);
//						dDataOff[0] = int(dTemp / GetRaceStat(DefRace)->GetSpeed())+1.;
						dDataOff[0] = int((dTemp / GetRaceStat(DefRace)->GetSpeed())*(GetFactorOffense(OffNum)->GetData(DATA_SPEED)+GetFactorOffense(OffNum)->GetData(DATA_ITEM_RSP)-100)/100)+1.;
						//	무피해 마법 공격 횟수
						dDataOff[1] = 0.;
						//	무피해 마법 원거리 공격 횟수
						dDataOff[2] = 0.;
						//	후속 공격 타입
						dDataOff[3] = 0.;
						//	후속 공격 데미지
						dOffDmg = GetFactorOffense(OffNum)->GetResultData(RDATA_DAMAGE_SEC);
						//	접근시 원거리 공격 횟수
						dTemp = GetClassStat(OffClass)->GetRange(STAT_RRANGE_MAX) - max(GetClassStat(OffClass)->GetRange(STAT_RRANGE_MIN), GetClassStat(OffClass)->GetRange(STAT_RANGE_MAX));
						dTemp = max(dTemp, 0);
//						dDataOff[4] = int(dTemp / GetRaceStat(OffRace)->GetSpeed())*1.;
						dDataOff[4] = int((dTemp / GetRaceStat(OffRace)->GetSpeed())*(GetFactorOffense(OffNum)->GetData(DATA_SPEED)+GetFactorOffense(OffNum)->GetData(DATA_ITEM_RSP)-100)/100)+1.;

						//	무피해 원거리 공격 횟수
						dTemp = GetClassStat(DefClass)->GetRange(STAT_RRANGE_MAX) - max(GetClassStat(DefClass)->GetRange(STAT_RRANGE_MIN), GetClassStat(OffClass)->GetRange(STAT_RRANGE_MAX));
						dTemp = max(dTemp, 0);
//						dDataDef[0] = int(dTemp / GetRaceStat(OffRace)->GetSpeed())*1.;
						dDataDef[0] = int((dTemp / GetRaceStat(OffRace)->GetSpeed())*(GetFactorDefense(OffNum)->GetData(DATA_SPEED)+GetFactorDefense(OffNum)->GetData(DATA_ITEM_RSP)-100)/100)+1.;
						//	무피해 마법 공격 횟수
						dDataDef[1] = 0.;
						//	무피해 마법 원거리 공격 횟수
						dDataDef[2] = 0.;
						//	후속 공격 타입
						dDataDef[3] = 1.;
						//	후속 공격 데미지
						dDefDmg = GetFactorDefense(DefNum)->GetResultData(RDATA_RDAMAGE_SEC);
						break;
					case 2:
						//	무피해 원거리 공격 횟수
						dTemp = GetClassStat(OffClass)->GetRange(STAT_RRANGE_MAX) - max(GetClassStat(OffClass)->GetRange(STAT_RRANGE_MIN), GetClassStat(DefClass)->GetRange(STAT_MRRANGE_MAX));
						dTemp = max(dTemp, 0);
//						dDataOff[0] = int(dTemp / GetRaceStat(DefRace)->GetSpeed())+1.;
						dDataOff[0] = int((dTemp / GetRaceStat(DefRace)->GetSpeed())*(GetFactorOffense(OffNum)->GetData(DATA_SPEED)+GetFactorOffense(OffNum)->GetData(DATA_ITEM_RSP)-100)/100)+1.;
						//	무피해 마법 공격 횟수
						dDataOff[1] = 0.;
						//	무피해 마법 원거리 공격 횟수
						dDataOff[2] = 0.;
						//	후속 공격 타입
						dDataOff[3] = 0.;
						//	후속 공격 데미지
						dOffDmg = GetFactorOffense(OffNum)->GetResultData(RDATA_DAMAGE_SEC);
						//	접근시 원거리 공격 횟수
						dTemp = GetClassStat(OffClass)->GetRange(STAT_RRANGE_MAX) - max(GetClassStat(OffClass)->GetRange(STAT_RRANGE_MIN), GetClassStat(OffClass)->GetRange(STAT_RANGE_MAX));
						dTemp = max(dTemp, 0);
//						dDataOff[4] = int(dTemp / GetRaceStat(OffRace)->GetSpeed())*1.;
						dDataOff[4] = int((dTemp / GetRaceStat(OffRace)->GetSpeed())*(GetFactorOffense(OffNum)->GetData(DATA_SPEED)+GetFactorOffense(OffNum)->GetData(DATA_ITEM_RSP)-100)/100)+1.;

						//	무피해 원거리 공격 횟수
						dDataDef[0] = 0.;
						//	무피해 마법 공격 횟수
						dTemp = GetClassStat(DefClass)->GetRange(STAT_MRANGE_MAX) - max(GetClassStat(DefClass)->GetRange(STAT_MRANGE_MIN), GetClassStat(OffClass)->GetRange(STAT_RRANGE_MAX));
						dTemp = max(dTemp, 0);
						dDataDef[1] = int(dTemp / GetRaceStat(OffRace)->GetSpeed())*1.;
						dDataDef[1] = int((dTemp / GetRaceStat(OffRace)->GetSpeed())*(GetFactorDefense(OffNum)->GetData(DATA_MAGICSPEED)+GetFactorDefense(OffNum)->GetData(DATA_ITEM_MSP)-100)/100)+1.;
						//	무피해 마법 원거리 공격 횟수
						dDataDef[2] = 0.;
						//	후속 공격 타입
						dDataDef[3] = 2.;
						//	후속 공격 데미지
						dDefDmg = GetFactorDefense(DefNum)->GetResultData(RDATA_MDAMAGE_SEC);
						break;
				}
			}
			break;
		case 2:
			{
				switch(GetFactorDefense(DefNum)->GetResultDataInt(RDATA_ATTACK_TYPE))
				{
					//	방어자(후공) 공격 타입	0:물리	1:원거리	2:	마법
					case 0:
						//	무피해 원거리 공격 횟수
						dDataOff[0] = 0.;
						//	무피해 마법 공격 횟수
						dTemp = GetClassStat(OffClass)->GetRange(STAT_MRANGE_MAX) - max(GetClassStat(OffClass)->GetRange(STAT_MRANGE_MIN), GetClassStat(DefClass)->GetRange(STAT_RANGE_MAX));
						dTemp = max(dTemp, 0);
						dDataOff[1] = int(dTemp / GetRaceStat(DefRace)->GetSpeed())*1.;
						//	무피해 마법 원거리 공격 횟수
						dTemp = GetClassStat(OffClass)->GetRange(STAT_MRRANGE_MAX) - max(GetClassStat(OffClass)->GetRange(STAT_MRANGE_MAX), GetClassStat(DefClass)->GetRange(STAT_RANGE_MAX));
						dTemp = max(dTemp, 0);
//						dDataOff[2] = int(dTemp / GetRaceStat(DefRace)->GetSpeed())+1.;
						dDataOff[2] = int((dTemp / GetRaceStat(DefRace)->GetSpeed())*(GetFactorOffense(OffNum)->GetData(DATA_MAGICSPEED)+GetFactorOffense(OffNum)->GetData(DATA_ITEM_MRSP)-100)/100)+1.;
						//	후속 공격 타입
						dDataOff[3] = 2.;
						//	후속 공격 데미지
						dOffDmg = GetFactorOffense(OffNum)->GetResultData(RDATA_MDAMAGE_SEC);

						//	무피해 원거리 공격 횟수
						dDataDef[0] = 0.;
						//	무피해 마법 공격 횟수
						dDataDef[1] = 0.;
						//	무피해 마법 원거리 공격 횟수
						dDataDef[2] = 0.;
						//	후속 공격 타입
						dDataDef[3] = 0.;
						//	후속 공격 데미지
						dDefDmg = GetFactorDefense(DefNum)->GetResultData(RDATA_DAMAGE_SEC);
						break;
					case 1:
						//	무피해 원거리 공격 횟수
						dDataOff[0] = 0.;
						//	무피해 마법 공격 횟수
						dTemp = GetClassStat(OffClass)->GetRange(STAT_MRANGE_MAX) - max(GetClassStat(OffClass)->GetRange(STAT_MRANGE_MIN), GetClassStat(DefClass)->GetRange(STAT_RRANGE_MAX));
						dTemp = max(dTemp, 0);
						dDataOff[1] = int(dTemp / GetRaceStat(DefRace)->GetSpeed())*1.;
						//	무피해 마법 원거리 공격 횟수
						dTemp = GetClassStat(OffClass)->GetRange(STAT_MRRANGE_MAX) - max(GetClassStat(OffClass)->GetRange(STAT_MRANGE_MAX), GetClassStat(DefClass)->GetRange(STAT_RRANGE_MAX));
						dTemp = max(dTemp, 0);
//						dDataOff[2] = int(dTemp / GetRaceStat(DefRace)->GetSpeed())+1.;
						dDataOff[2] = int((dTemp / GetRaceStat(DefRace)->GetSpeed())*(GetFactorOffense(OffNum)->GetData(DATA_MAGICSPEED)+GetFactorOffense(OffNum)->GetData(DATA_ITEM_MRSP)-100)/100)+1.;
						//	후속 공격 타입
						dDataOff[3] = 2.;
						//	후속 공격 데미지
						dOffDmg = GetFactorOffense(OffNum)->GetResultData(RDATA_MDAMAGE_SEC);

						//	무피해 원거리 공격 횟수
						dTemp = GetClassStat(DefClass)->GetRange(STAT_RRANGE_MAX) - max(GetClassStat(DefClass)->GetRange(STAT_RRANGE_MIN), GetClassStat(OffClass)->GetRange(STAT_RRANGE_MAX));
						dTemp = max(dTemp, 0);
//						dDataDef[0] = int(dTemp / GetRaceStat(OffRace)->GetSpeed())*1.;
						dDataDef[0] = int((dTemp / GetRaceStat(OffRace)->GetSpeed())*(GetFactorDefense(OffNum)->GetData(DATA_SPEED)+GetFactorDefense(OffNum)->GetData(DATA_ITEM_RSP)-100)/100)+1.;
						//	무피해 마법 공격 횟수
						dDataDef[1] = 0.;
						//	무피해 마법 원거리 공격 횟수
						dDataDef[2] = 0.;
						//	후속 공격 타입
						dDataDef[3] = 1.;
						//	후속 공격 데미지
						dDefDmg = GetFactorDefense(DefNum)->GetResultData(RDATA_RDAMAGE_SEC);
						break;
					case 2:
						//	무피해 원거리 공격 횟수
						dDataOff[0] = 0.;
						//	무피해 마법 공격 횟수
						dTemp = GetClassStat(OffClass)->GetRange(STAT_MRANGE_MAX) - max(GetClassStat(OffClass)->GetRange(STAT_MRANGE_MIN), GetClassStat(DefClass)->GetRange(STAT_MRANGE_MAX));
						dTemp = max(dTemp, 0);
						dDataOff[1] = int(dTemp / GetRaceStat(DefRace)->GetSpeed())*1.;
						//	무피해 마법 원거리 공격 횟수
						dTemp = GetClassStat(OffClass)->GetRange(STAT_MRRANGE_MAX) - max(GetClassStat(OffClass)->GetRange(STAT_MRANGE_MAX), GetClassStat(DefClass)->GetRange(STAT_MRANGE_MAX));
						dTemp = max(dTemp, 0);
//						dDataOff[2] = int(dTemp / GetRaceStat(DefRace)->GetSpeed())+1.;
						dDataOff[2] = int((dTemp / GetRaceStat(DefRace)->GetSpeed())*(GetFactorOffense(OffNum)->GetData(DATA_MAGICSPEED)+GetFactorOffense(OffNum)->GetData(DATA_ITEM_MRSP)-100)/100)+1.;
						//	후속 공격 타입
						dDataOff[3] = 2.;
						//	후속 공격 데미지
						dOffDmg = GetFactorOffense(OffNum)->GetResultData(RDATA_MDAMAGE_SEC);

						//	무피해 원거리 공격 횟수
						dDataDef[0] = 0.;
						//	무피해 마법 공격 횟수
						dDataDef[1] = 0.;
						//	무피해 마법 원거리 공격 횟수
						dDataDef[2] = 0.;
						//	후속 공격 타입
						dDataDef[3] = 2.;
						//	후속 공격 데미지
						dDefDmg = GetFactorDefense(DefNum)->GetResultData(RDATA_MDAMAGE_SEC);
						break;
				}
			}
			break;
	}

	//	공격자의 현재 HP 임시 계산
	dOffHP = GetFactorOffense(OffNum)->GetDataInt(DATA_HP) 
		- (GetFactorDefense(DefNum)->GetResultData(RDATA_RDAMAGE_SEC) * dDataDef[0])
		- (GetFactorDefense(DefNum)->GetResultData(RDATA_MDAMAGE_SEC) * dDataDef[1])
		- (GetFactorDefense(DefNum)->GetResultData(RDATA_MRDAMAGE_SEC) * dDataDef[2])
		;
	if(dOffHP<0.) 
	{
		dOffHP=0.;
		if(dDataDef[0]>0.)
			dDataDef[0] = double(int(GetFactorOffense(OffNum)->GetDataInt(DATA_HP)/GetFactorDefense(DefNum)->GetResultData(RDATA_RDAMAGE_SEC)+0.9999));
		if(dDataDef[1]>0.)
			dDataDef[1] = double(int(GetFactorOffense(OffNum)->GetDataInt(DATA_HP)/GetFactorDefense(DefNum)->GetResultData(RDATA_MDAMAGE_SEC)+0.9999));
		if(dDataDef[2]>0.)
			dDataDef[2] = double(int(GetFactorOffense(OffNum)->GetDataInt(DATA_HP)/GetFactorDefense(DefNum)->GetResultData(RDATA_MRDAMAGE_SEC)+0.9999));
	}
	//	방어자의 현재 HP 임시 계산
	dDefHP = GetFactorDefense(DefNum)->GetDataInt(DATA_HP) 
		- (GetFactorOffense(OffNum)->GetResultData(RDATA_RDAMAGE_SEC) * dDataOff[0])
		- (GetFactorOffense(OffNum)->GetResultData(RDATA_MDAMAGE_SEC) * dDataOff[1])
		- (GetFactorOffense(OffNum)->GetResultData(RDATA_MRDAMAGE_SEC) * dDataOff[2])
		;
	if(dDefHP<0.) 
	{
		dDefHP=0.;
		if(dDataOff[0]>0.)
			dDataOff[0] = double(int(GetFactorDefense(DefNum)->GetDataInt(DATA_HP)/GetFactorOffense(OffNum)->GetResultData(RDATA_RDAMAGE_SEC)+0.9999));
		if(dDataOff[1]>0.)
			dDataOff[1] = double(int(GetFactorDefense(DefNum)->GetDataInt(DATA_HP)/GetFactorOffense(OffNum)->GetResultData(RDATA_MDAMAGE_SEC)+0.9999));
		if(dDataOff[2]>0.)
			dDataOff[2] = double(int(GetFactorDefense(DefNum)->GetDataInt(DATA_HP)/GetFactorOffense(OffNum)->GetResultData(RDATA_MRDAMAGE_SEC)+0.9999));
	}
	else
	{
		//	방어자의 현재 HP 에 공격자가 접근하면서 공격
		dTemp = dDefHP;
		dTemp = dTemp
			- (GetFactorOffense(OffNum)->GetResultData(RDATA_RDAMAGE_SEC) * dDataOff[4])
			;
		dDefHP = dTemp;
		if(dTemp<0.) 
		{
			dDataOff[4] = double(int(dDefHP/GetFactorOffense(OffNum)->GetResultData(RDATA_RDAMAGE_SEC)+0.9999));
			dDefHP=0.;
		}
	}

	//	공격자 무피해 원거리 공격 횟수
	GetFactorOffense(OffNum)->SetResultData(dDataOff[0], RDATA_ND_RATTACKCOUNT);
	//	공격자 무피해 마법 공격 횟수
	GetFactorOffense(OffNum)->SetResultData(dDataOff[1], RDATA_ND_MATTACKCOUNT);
	//	공격자 무피해 마법 원거리 공격 횟수
	GetFactorOffense(OffNum)->SetResultData(dDataOff[2], RDATA_ND_MRATTACKCOUNT);
	//	공격자 후속 공격 타입
	GetFactorOffense(OffNum)->SetResultData(dDataOff[3], RDATA_FA_TYPE);
	//	접근시 원거리 공격 횟수
	GetFactorOffense(OffNum)->SetResultData(dDataOff[4], RDATA_D_RATTACKCOUNT);

	//	방어자 무피해 원거리 공격 횟수
	GetFactorDefense(DefNum)->SetResultData(dDataDef[0], RDATA_ND_RATTACKCOUNT);
	//	방어자 무피해 마법 공격 횟수
	GetFactorDefense(DefNum)->SetResultData(dDataDef[1], RDATA_ND_MATTACKCOUNT);
	//	방어자 무피해 마법 원거리 공격 횟수
	GetFactorDefense(DefNum)->SetResultData(dDataDef[2], RDATA_ND_MRATTACKCOUNT);
	//	방어자 후속 공격 타입
	GetFactorDefense(DefNum)->SetResultData(dDataDef[3], RDATA_FA_TYPE);
	//	접근시 원거리 공격 횟수
	GetFactorDefense(DefNum)->SetResultData(dDataDef[4], RDATA_D_RATTACKCOUNT);

	//	공격자 후속 공격 횟수
	GetFactorOffense(OffNum)->SetResultData(dDefHP/dOffDmg+0.9999, RDATA_FA_COUNT);
	//	방어자 후속 공격 횟수
	GetFactorDefense(DefNum)->SetResultData(dOffHP/dDefDmg+0.9999, RDATA_FA_COUNT);

	////	공격자 총 공격 횟수
	//dOffNum = GetFactorOffense(OffNum)->GetResultData(RDATA_ND_RATTACKCOUNT) 
	//	+ GetFactorOffense(OffNum)->GetResultData(RDATA_ND_MATTACKCOUNT) 
	//	+ GetFactorOffense(OffNum)->GetResultData(RDATA_FA_COUNT);
	//GetFactorOffense(OffNum)->SetResultData(dOffNum, RDATA_WIN_ATTACK_COUNT);

	////	방어자 총 공격 횟수
	//dDefNum = GetFactorDefense(DefNum)->GetResultData(RDATA_ND_RATTACKCOUNT) 
	//	+ GetFactorDefense(DefNum)->GetResultData(RDATA_ND_MATTACKCOUNT) 
	//	+ GetFactorDefense(DefNum)->GetResultData(RDATA_FA_COUNT);
	//GetFactorDefense(DefNum)->SetResultData(dDefNum, RDATA_WIN_ATTACK_COUNT);

	//	전투 결과
	//	후속 공격 횟수+접근시 공격 횟수를 비교하여 공격자횟수가 적거나 같을 때
	if(GetFactorOffense(OffNum)->GetResultDataInt(RDATA_FA_COUNT) + GetFactorOffense(OffNum)->GetResultDataInt(RDATA_D_RATTACKCOUNT) <= GetFactorDefense(DefNum)->GetResultDataInt(RDATA_FA_COUNT) + GetFactorDefense(DefNum)->GetResultDataInt(RDATA_D_RATTACKCOUNT))
	{
		//	공격자 승리
        GetFactorOffense(OffNum)->SetResultData(0., RDATA_WIN_RESULT);
		//	공격자 실 후속 공격 횟수
        GetFactorOffense(OffNum)->SetResultData(GetFactorOffense(OffNum)->GetResultData(RDATA_FA_COUNT), RDATA_REAL_FA_COUNT);	//

		//	방어자 패배
        GetFactorDefense(DefNum)->SetResultData(1., RDATA_WIN_RESULT);
		//	방어자 실 후속 공격 횟수
        GetFactorDefense(DefNum)->SetResultData(GetFactorOffense(OffNum)->GetResultData(RDATA_FA_COUNT)-1., RDATA_REAL_FA_COUNT);	//
	}
	else
	{
		//	방어자 승리
        GetFactorDefense(DefNum)->SetResultData(0., RDATA_WIN_RESULT);
		//	방어자 실 후속 공격 횟수
        GetFactorDefense(DefNum)->SetResultData(GetFactorDefense(DefNum)->GetResultData(RDATA_FA_COUNT), RDATA_REAL_FA_COUNT);	//

		//	공격자 패배
        GetFactorOffense(OffNum)->SetResultData(1., RDATA_WIN_RESULT);
		//	공격자 실 후속 공격 횟수
        GetFactorOffense(OffNum)->SetResultData(GetFactorDefense(DefNum)->GetResultData(RDATA_FA_COUNT), RDATA_REAL_FA_COUNT);	//
	}

	CalcCombatFactor2(GetFactorOffense(OffNum), GetFactorDefense(DefNum));
	CalcCombatFactor2(GetFactorDefense(DefNum), GetFactorOffense(OffNum));
}

void CBalancePool::CalcWinCombat(int OffLevel, int OffClass, int OffRace, int DefLevel, int DefClass, int DefRace, char OffNum, char DefNum)
{
	double dDataOff[5]={0.,};
	double dDataDef[5]={0.,};
	double dOffDmg=0., dDefDmg=0.;
	double dOffHP, dDefHP;
	double dTemp;

	//	스탯을 능력치로 계산하는 공식을 적용한다.
	CalcFactor(OffLevel, OffClass, OffRace);
	CopyFactorToTarget(GetFactorOffense(OffNum));
//	CopyFactorToOffense(OffNum);
	CalcFactor(DefLevel, DefClass, DefRace);
	CopyFactorToTarget(GetFactorDefense(DefNum));
//	CopyFactorToDefense(DefNum);

	CalcCombatFactor(GetFactorOffense(OffNum), GetFactorDefense(DefNum));
	CalcCombatFactor(GetFactorDefense(DefNum), GetFactorOffense(OffNum));

	switch(GetFactorOffense(OffNum)->GetResultDataInt(RDATA_ATTACK_TYPE))
	{
		//	공격자(선공) 공격 타입	0:물리	1:원거리	2:	마법
		case 0:
			{
				switch(GetFactorDefense(DefNum)->GetResultDataInt(RDATA_ATTACK_TYPE))
				{
					//	방어자(후공) 공격 타입	0:물리	1:원거리	2:	마법
					case 0:
						//	무피해 원거리 공격 횟수
						dTemp = GetClassStat(OffClass)->GetRange(STAT_RRANGE_MAX) - max(GetClassStat(OffClass)->GetRange(STAT_RRANGE_MIN), GetClassStat(DefClass)->GetRange(STAT_RANGE_MAX));
						dTemp = max(dTemp, 0);
//						dDataOff[0] = int(dTemp / GetRaceStat(DefRace)->GetSpeed())+1.;
						dDataOff[0] = int((dTemp / GetRaceStat(DefRace)->GetSpeed())*(GetFactorOffense(OffNum)->GetData(DATA_SPEED)+GetFactorOffense(OffNum)->GetData(DATA_ITEM_RSP)-100)/100)+1.;
						//	무피해 마법 공격 횟수
						dDataOff[1] = 0.;
						//	무피해 마법 원거리 공격 횟수
						dDataOff[2] = 0.;
						//	후속 공격 타입
						dDataOff[3] = 0.;
						//	후속 공격 데미지
						dOffDmg = GetFactorOffense(OffNum)->GetResultData(RDATA_DAMAGE_SEC);

						//	무피해 원거리 공격 횟수
						dDataDef[0] = 0.;
						//	무피해 마법 공격 횟수
						dDataDef[1] = 0.;
						//	무피해 마법 원거리 공격 횟수
						dDataDef[2] = 0.;
						//	후속 공격 타입
						dDataDef[3] = 0.;
						//	후속 공격 데미지
						dDefDmg = GetFactorDefense(DefNum)->GetResultData(RDATA_DAMAGE_SEC);
						break;
					case 1:
						//	무피해 원거리 공격 횟수
						dTemp = GetClassStat(OffClass)->GetRange(STAT_RRANGE_MAX) - max(GetClassStat(OffClass)->GetRange(STAT_RRANGE_MIN), GetClassStat(DefClass)->GetRange(STAT_RRANGE_MAX));
						dTemp = max(dTemp, 0);
//						dDataOff[0] = int(dTemp / GetRaceStat(DefRace)->GetSpeed())+1.;
						dDataOff[0] = int((dTemp / GetRaceStat(DefRace)->GetSpeed())*(GetFactorOffense(OffNum)->GetData(DATA_SPEED)+GetFactorOffense(OffNum)->GetData(DATA_ITEM_RSP)-100)/100)+1.;
						//	무피해 마법 공격 횟수
						dDataOff[1] = 0.;
						//	무피해 마법 원거리 공격 횟수
						dDataOff[2] = 0.;
						//	후속 공격 타입
						dDataOff[3] = 0.;
						//	후속 공격 데미지
						dOffDmg = GetFactorOffense(OffNum)->GetResultData(RDATA_DAMAGE_SEC);
						//	접근시 원거리 공격 횟수
//						dTemp = GetClassStat(OffClass)->GetRange(STAT_RRANGE_MAX) - max(GetClassStat(OffClass)->GetRange(STAT_RRANGE_MIN), GetClassStat(OffClass)->GetRange(STAT_RANGE_MAX));
//						dTemp = max(dTemp, 0);
//						dDataOff[4] = int(dTemp / GetRaceStat(OffRace)->GetSpeed())*1.;
						dDataOff[4] = 0.;

						//	무피해 원거리 공격 횟수
						dTemp = GetClassStat(DefClass)->GetRange(STAT_RRANGE_MAX) - max(GetClassStat(DefClass)->GetRange(STAT_RRANGE_MIN), GetClassStat(OffClass)->GetRange(STAT_RRANGE_MAX));
						dTemp = max(dTemp, 0);
//						dDataDef[0] = int(dTemp / GetRaceStat(OffRace)->GetSpeed())*1.;
						dDataDef[0] = int((dTemp / GetRaceStat(OffRace)->GetSpeed())*(GetFactorDefense(OffNum)->GetData(DATA_SPEED)+GetFactorDefense(OffNum)->GetData(DATA_ITEM_RSP)-100)/100)+1.;
						//	무피해 마법 공격 횟수
						dDataDef[1] = 0.;
						//	무피해 마법 원거리 공격 횟수
						dDataDef[2] = 0.;
						//	후속 공격 타입
						dDataDef[3] = 1.;
						//	후속 공격 데미지
						dDefDmg = GetFactorDefense(DefNum)->GetResultData(RDATA_RDAMAGE_SEC);
						break;
					case 2:
						//	무피해 원거리 공격 횟수
						dTemp = GetClassStat(OffClass)->GetRange(STAT_RRANGE_MAX) - max(GetClassStat(OffClass)->GetRange(STAT_RRANGE_MIN), GetClassStat(DefClass)->GetRange(STAT_MRRANGE_MAX));
						dTemp = max(dTemp, 0);
//						dDataOff[0] = int(dTemp / GetRaceStat(DefRace)->GetSpeed())+1.;
						dDataOff[0] = int((dTemp / GetRaceStat(DefRace)->GetSpeed())*(GetFactorOffense(OffNum)->GetData(DATA_SPEED)+GetFactorOffense(OffNum)->GetData(DATA_ITEM_RSP)-100)/100)+1.;
						//	무피해 마법 공격 횟수
						dDataOff[1] = 0.;
						//	무피해 마법 원거리 공격 횟수
						dDataOff[2] = 0.;
						//	후속 공격 타입
						dDataOff[3] = 0.;
						//	후속 공격 데미지
						dOffDmg = GetFactorOffense(OffNum)->GetResultData(RDATA_DAMAGE_SEC);
						//	접근시 원거리 공격 횟수
//						dTemp = GetClassStat(OffClass)->GetRange(STAT_RRANGE_MAX) - max(GetClassStat(OffClass)->GetRange(STAT_RRANGE_MIN), GetClassStat(OffClass)->GetRange(STAT_RANGE_MAX));
//						dTemp = max(dTemp, 0);
//						dDataOff[4] = int(dTemp / GetRaceStat(OffRace)->GetSpeed())*1.;
						dDataOff[4] = 0.;

						//	무피해 원거리 공격 횟수
						dDataDef[0] = 0.;
						//	무피해 마법 공격 횟수
						dTemp = GetClassStat(DefClass)->GetRange(STAT_MRANGE_MAX) - max(GetClassStat(DefClass)->GetRange(STAT_MRANGE_MIN), GetClassStat(OffClass)->GetRange(STAT_RRANGE_MAX));
						dTemp = max(dTemp, 0);
//						dDataDef[1] = int(dTemp / GetRaceStat(OffRace)->GetSpeed())*1.;
						dDataDef[1] = int((dTemp / GetRaceStat(OffRace)->GetSpeed())*(GetFactorDefense(OffNum)->GetData(DATA_MAGICSPEED)+GetFactorDefense(OffNum)->GetData(DATA_ITEM_MSP)-100)/100)+1.;
						//	무피해 마법 원거리 공격 횟수
						dDataDef[2] = 0.;
						//	후속 공격 타입
						dDataDef[3] = 2.;
						//	후속 공격 데미지
						dDefDmg = GetFactorDefense(DefNum)->GetResultData(RDATA_MDAMAGE_SEC);
						break;
				}
			}
			break;
		case 1:
			{
				switch(GetFactorDefense(DefNum)->GetResultDataInt(RDATA_ATTACK_TYPE))
				{
					//	방어자(후공) 공격 타입	0:물리	1:원거리	2:	마법
					case 0:
						//	무피해 원거리 공격 횟수
						dTemp = GetClassStat(OffClass)->GetRange(STAT_RRANGE_MAX) - max(GetClassStat(OffClass)->GetRange(STAT_RRANGE_MIN), GetClassStat(DefClass)->GetRange(STAT_RANGE_MAX));
						dTemp = max(dTemp, 0);
//						dDataOff[0] = int(dTemp / GetRaceStat(DefRace)->GetSpeed())+1.;
						dDataOff[0] = int((dTemp / GetRaceStat(DefRace)->GetSpeed())*(GetFactorOffense(OffNum)->GetData(DATA_SPEED)+GetFactorOffense(OffNum)->GetData(DATA_ITEM_RSP)-100)/100)+1.;
						//	무피해 마법 공격 횟수
						dDataOff[1] = 0.;
						//	무피해 마법 원거리 공격 횟수
						dDataOff[2] = 0.;
						//	후속 공격 타입
						dDataOff[3] = 0.;
						//	후속 공격 데미지
						dOffDmg = GetFactorOffense(OffNum)->GetResultData(RDATA_DAMAGE_SEC);

						//	무피해 원거리 공격 횟수
						dDataDef[0] = 0.;
						//	무피해 마법 공격 횟수
						dDataDef[1] = 0.;
						//	무피해 마법 원거리 공격 횟수
						dDataDef[2] = 0.;
						//	후속 공격 타입
						dDataDef[3] = 0.;
						//	후속 공격 데미지
						dDefDmg = GetFactorDefense(DefNum)->GetResultData(RDATA_DAMAGE_SEC);
						break;
					case 1:
						//	무피해 원거리 공격 횟수
						dTemp = GetClassStat(OffClass)->GetRange(STAT_RRANGE_MAX) - max(GetClassStat(OffClass)->GetRange(STAT_RRANGE_MIN), GetClassStat(DefClass)->GetRange(STAT_RRANGE_MAX));
						dTemp = max(dTemp, 0);
//						dDataOff[0] = int(dTemp / GetRaceStat(DefRace)->GetSpeed())+1.;
						dDataOff[0] = int((dTemp / GetRaceStat(DefRace)->GetSpeed())*(GetFactorOffense(OffNum)->GetData(DATA_SPEED)+GetFactorOffense(OffNum)->GetData(DATA_ITEM_RSP)-100)/100)+1.;
						//	무피해 마법 공격 횟수
						dDataOff[1] = 0.;
						//	무피해 마법 원거리 공격 횟수
						dDataOff[2] = 0.;
						//	후속 공격 타입
						dDataOff[3] = 0.;
						//	후속 공격 데미지
						dOffDmg = GetFactorOffense(OffNum)->GetResultData(RDATA_DAMAGE_SEC);
						//	접근시 원거리 공격 횟수
						dTemp = GetClassStat(OffClass)->GetRange(STAT_RRANGE_MAX) - max(GetClassStat(OffClass)->GetRange(STAT_RRANGE_MIN), GetClassStat(OffClass)->GetRange(STAT_RANGE_MAX));
						dTemp = max(dTemp, 0);
//						dDataOff[4] = int(dTemp / GetRaceStat(OffRace)->GetSpeed())*1.;
						dDataOff[4] = int((dTemp / GetRaceStat(OffRace)->GetSpeed())*(GetFactorOffense(OffNum)->GetData(DATA_SPEED)+GetFactorOffense(OffNum)->GetData(DATA_ITEM_RSP)-100)/100)+1.;

						//	무피해 원거리 공격 횟수
						dTemp = GetClassStat(DefClass)->GetRange(STAT_RRANGE_MAX) - max(GetClassStat(DefClass)->GetRange(STAT_RRANGE_MIN), GetClassStat(OffClass)->GetRange(STAT_RRANGE_MAX));
						dTemp = max(dTemp, 0);
//						dDataDef[0] = int(dTemp / GetRaceStat(OffRace)->GetSpeed())*1.;
						dDataDef[0] = int((dTemp / GetRaceStat(OffRace)->GetSpeed())*(GetFactorDefense(OffNum)->GetData(DATA_SPEED)+GetFactorDefense(OffNum)->GetData(DATA_ITEM_RSP)-100)/100)+1.;
						//	무피해 마법 공격 횟수
						dDataDef[1] = 0.;
						//	무피해 마법 원거리 공격 횟수
						dDataDef[2] = 0.;
						//	후속 공격 타입
						dDataDef[3] = 1.;
						//	후속 공격 데미지
						dDefDmg = GetFactorDefense(DefNum)->GetResultData(RDATA_RDAMAGE_SEC);
						break;
					case 2:
						//	무피해 원거리 공격 횟수
						dTemp = GetClassStat(OffClass)->GetRange(STAT_RRANGE_MAX) - max(GetClassStat(OffClass)->GetRange(STAT_RRANGE_MIN), GetClassStat(DefClass)->GetRange(STAT_MRRANGE_MAX));
						dTemp = max(dTemp, 0);
//						dDataOff[0] = int(dTemp / GetRaceStat(DefRace)->GetSpeed())+1.;
						dDataOff[0] = int((dTemp / GetRaceStat(DefRace)->GetSpeed())*(GetFactorOffense(OffNum)->GetData(DATA_SPEED)+GetFactorOffense(OffNum)->GetData(DATA_ITEM_RSP)-100)/100)+1.;
						//	무피해 마법 공격 횟수
						dDataOff[1] = 0.;
						//	무피해 마법 원거리 공격 횟수
						dDataOff[2] = 0.;
						//	후속 공격 타입
						dDataOff[3] = 0.;
						//	후속 공격 데미지
						dOffDmg = GetFactorOffense(OffNum)->GetResultData(RDATA_DAMAGE_SEC);
						//	접근시 원거리 공격 횟수
						dTemp = GetClassStat(OffClass)->GetRange(STAT_RRANGE_MAX) - max(GetClassStat(OffClass)->GetRange(STAT_RRANGE_MIN), GetClassStat(OffClass)->GetRange(STAT_RANGE_MAX));
						dTemp = max(dTemp, 0);
//						dDataOff[4] = int(dTemp / GetRaceStat(OffRace)->GetSpeed())*1.;
						dDataOff[4] = int((dTemp / GetRaceStat(OffRace)->GetSpeed())*(GetFactorOffense(OffNum)->GetData(DATA_SPEED)+GetFactorOffense(OffNum)->GetData(DATA_ITEM_RSP)-100)/100)+1.;

						//	무피해 원거리 공격 횟수
						dDataDef[0] = 0.;
						//	무피해 마법 공격 횟수
						dTemp = GetClassStat(DefClass)->GetRange(STAT_MRANGE_MAX) - max(GetClassStat(DefClass)->GetRange(STAT_MRANGE_MIN), GetClassStat(OffClass)->GetRange(STAT_RRANGE_MAX));
						dTemp = max(dTemp, 0);
						dDataDef[1] = int(dTemp / GetRaceStat(OffRace)->GetSpeed())*1.;
						dDataDef[1] = int((dTemp / GetRaceStat(OffRace)->GetSpeed())*(GetFactorDefense(OffNum)->GetData(DATA_MAGICSPEED)+GetFactorDefense(OffNum)->GetData(DATA_ITEM_MSP)-100)/100)+1.;
						//	무피해 마법 원거리 공격 횟수
						dDataDef[2] = 0.;
						//	후속 공격 타입
						dDataDef[3] = 2.;
						//	후속 공격 데미지
						dDefDmg = GetFactorDefense(DefNum)->GetResultData(RDATA_MDAMAGE_SEC);
						break;
				}
			}
			break;
		case 2:
			{
				switch(GetFactorDefense(DefNum)->GetResultDataInt(RDATA_ATTACK_TYPE))
				{
					//	방어자(후공) 공격 타입	0:물리	1:원거리	2:	마법
					case 0:
						//	무피해 원거리 공격 횟수
						dDataOff[0] = 0.;
						//	무피해 마법 공격 횟수
						dTemp = GetClassStat(OffClass)->GetRange(STAT_MRANGE_MAX) - max(GetClassStat(OffClass)->GetRange(STAT_MRANGE_MIN), GetClassStat(DefClass)->GetRange(STAT_RANGE_MAX));
						dTemp = max(dTemp, 0);
						dDataOff[1] = int(dTemp / GetRaceStat(DefRace)->GetSpeed())*1.;
						//	무피해 마법 원거리 공격 횟수
						dTemp = GetClassStat(OffClass)->GetRange(STAT_MRRANGE_MAX) - max(GetClassStat(OffClass)->GetRange(STAT_MRANGE_MAX), GetClassStat(DefClass)->GetRange(STAT_RANGE_MAX));
						dTemp = max(dTemp, 0);
//						dDataOff[2] = int(dTemp / GetRaceStat(DefRace)->GetSpeed())+1.;
						dDataOff[2] = int((dTemp / GetRaceStat(DefRace)->GetSpeed())*(GetFactorOffense(OffNum)->GetData(DATA_MAGICSPEED)+GetFactorOffense(OffNum)->GetData(DATA_ITEM_MRSP)-100)/100)+1.;
						//	후속 공격 타입
						dDataOff[3] = 2.;
						//	후속 공격 데미지
						dOffDmg = GetFactorOffense(OffNum)->GetResultData(RDATA_MDAMAGE_SEC);

						//	무피해 원거리 공격 횟수
						dDataDef[0] = 0.;
						//	무피해 마법 공격 횟수
						dDataDef[1] = 0.;
						//	무피해 마법 원거리 공격 횟수
						dDataDef[2] = 0.;
						//	후속 공격 타입
						dDataDef[3] = 0.;
						//	후속 공격 데미지
						dDefDmg = GetFactorDefense(DefNum)->GetResultData(RDATA_DAMAGE_SEC);
						break;
					case 1:
						//	무피해 원거리 공격 횟수
						dDataOff[0] = 0.;
						//	무피해 마법 공격 횟수
						dTemp = GetClassStat(OffClass)->GetRange(STAT_MRANGE_MAX) - max(GetClassStat(OffClass)->GetRange(STAT_MRANGE_MIN), GetClassStat(DefClass)->GetRange(STAT_RRANGE_MAX));
						dTemp = max(dTemp, 0);
						dDataOff[1] = int(dTemp / GetRaceStat(DefRace)->GetSpeed())*1.;
						//	무피해 마법 원거리 공격 횟수
						dTemp = GetClassStat(OffClass)->GetRange(STAT_MRRANGE_MAX) - max(GetClassStat(OffClass)->GetRange(STAT_MRANGE_MAX), GetClassStat(DefClass)->GetRange(STAT_RRANGE_MAX));
						dTemp = max(dTemp, 0);
//						dDataOff[2] = int(dTemp / GetRaceStat(DefRace)->GetSpeed())+1.;
						dDataOff[2] = int((dTemp / GetRaceStat(DefRace)->GetSpeed())*(GetFactorOffense(OffNum)->GetData(DATA_MAGICSPEED)+GetFactorOffense(OffNum)->GetData(DATA_ITEM_MRSP)-100)/100)+1.;
						//	후속 공격 타입
						dDataOff[3] = 2.;
						//	후속 공격 데미지
						dOffDmg = GetFactorOffense(OffNum)->GetResultData(RDATA_MDAMAGE_SEC);

						//	무피해 원거리 공격 횟수
						dTemp = GetClassStat(DefClass)->GetRange(STAT_RRANGE_MAX) - max(GetClassStat(DefClass)->GetRange(STAT_RRANGE_MIN), GetClassStat(OffClass)->GetRange(STAT_RRANGE_MAX));
						dTemp = max(dTemp, 0);
//						dDataDef[0] = int(dTemp / GetRaceStat(OffRace)->GetSpeed())*1.;
						dDataDef[0] = int((dTemp / GetRaceStat(OffRace)->GetSpeed())*(GetFactorDefense(OffNum)->GetData(DATA_SPEED)+GetFactorDefense(OffNum)->GetData(DATA_ITEM_RSP)-100)/100)+1.;
						//	무피해 마법 공격 횟수
						dDataDef[1] = 0.;
						//	무피해 마법 원거리 공격 횟수
						dDataDef[2] = 0.;
						//	후속 공격 타입
						dDataDef[3] = 1.;
						//	후속 공격 데미지
						dDefDmg = GetFactorDefense(DefNum)->GetResultData(RDATA_RDAMAGE_SEC);
						break;
					case 2:
						//	무피해 원거리 공격 횟수
						dDataOff[0] = 0.;
						//	무피해 마법 공격 횟수
						dTemp = GetClassStat(OffClass)->GetRange(STAT_MRANGE_MAX) - max(GetClassStat(OffClass)->GetRange(STAT_MRANGE_MIN), GetClassStat(DefClass)->GetRange(STAT_MRANGE_MAX));
						dTemp = max(dTemp, 0);
						dDataOff[1] = int(dTemp / GetRaceStat(DefRace)->GetSpeed())*1.;
						//	무피해 마법 원거리 공격 횟수
						dTemp = GetClassStat(OffClass)->GetRange(STAT_MRRANGE_MAX) - max(GetClassStat(OffClass)->GetRange(STAT_MRANGE_MAX), GetClassStat(DefClass)->GetRange(STAT_MRANGE_MAX));
						dTemp = max(dTemp, 0);
//						dDataOff[2] = int(dTemp / GetRaceStat(DefRace)->GetSpeed())+1.;
						dDataOff[2] = int((dTemp / GetRaceStat(DefRace)->GetSpeed())*(GetFactorOffense(OffNum)->GetData(DATA_MAGICSPEED)+GetFactorOffense(OffNum)->GetData(DATA_ITEM_MRSP)-100)/100)+1.;
						//	후속 공격 타입
						dDataOff[3] = 2.;
						//	후속 공격 데미지
						dOffDmg = GetFactorOffense(OffNum)->GetResultData(RDATA_MDAMAGE_SEC);

						//	무피해 원거리 공격 횟수
						dDataDef[0] = 0.;
						//	무피해 마법 공격 횟수
						dDataDef[1] = 0.;
						//	무피해 마법 원거리 공격 횟수
						dDataDef[2] = 0.;
						//	후속 공격 타입
						dDataDef[3] = 2.;
						//	후속 공격 데미지
						dDefDmg = GetFactorDefense(DefNum)->GetResultData(RDATA_MDAMAGE_SEC);
						break;
				}
			}
			break;
	}

	//	공격자의 현재 HP 임시 계산
	dOffHP = GetFactorOffense(OffNum)->GetDataInt(DATA_HP) + _HP_POTION
		- (GetFactorDefense(DefNum)->GetResultData(RDATA_RDAMAGE_SEC) * dDataDef[0])
		- (GetFactorDefense(DefNum)->GetResultData(RDATA_MDAMAGE_SEC) * dDataDef[1])
		- (GetFactorDefense(DefNum)->GetResultData(RDATA_MRDAMAGE_SEC) * dDataDef[2])
		;
	if(dOffHP<0.) 
	{
		dOffHP=0.;
		if(dDataDef[0]>0.)
			dDataDef[0] = double(int((GetFactorOffense(OffNum)->GetDataInt(DATA_HP)+_HP_POTION)/GetFactorDefense(DefNum)->GetResultData(RDATA_RDAMAGE_SEC)+0.9999));
		if(dDataDef[1]>0.)
			dDataDef[1] = double(int((GetFactorOffense(OffNum)->GetDataInt(DATA_HP)+_HP_POTION)/GetFactorDefense(DefNum)->GetResultData(RDATA_MDAMAGE_SEC)+0.9999));
		if(dDataDef[2]>0.)
			dDataDef[2] = double(int((GetFactorOffense(OffNum)->GetDataInt(DATA_HP)+_HP_POTION)/GetFactorDefense(DefNum)->GetResultData(RDATA_MRDAMAGE_SEC)+0.9999));
	}
	//	방어자의 현재 HP 임시 계산
	dDefHP = GetFactorDefense(DefNum)->GetDataInt(DATA_HP) 
		- (GetFactorOffense(OffNum)->GetResultData(RDATA_RDAMAGE_SEC) * dDataOff[0])
		- (GetFactorOffense(OffNum)->GetResultData(RDATA_MDAMAGE_SEC) * dDataOff[1])
		- (GetFactorOffense(OffNum)->GetResultData(RDATA_MRDAMAGE_SEC) * dDataOff[2])
		;
	if(dDefHP<0.) 
	{
		dDefHP=0.;
		if(dDataOff[0]>0.)
			dDataOff[0] = double(int(GetFactorDefense(DefNum)->GetDataInt(DATA_HP)/GetFactorOffense(OffNum)->GetResultData(RDATA_RDAMAGE_SEC)+0.9999));
		if(dDataOff[1]>0.)
			dDataOff[1] = double(int(GetFactorDefense(DefNum)->GetDataInt(DATA_HP)/GetFactorOffense(OffNum)->GetResultData(RDATA_MDAMAGE_SEC)+0.9999));
		if(dDataOff[2]>0.)
			dDataOff[2] = double(int(GetFactorDefense(DefNum)->GetDataInt(DATA_HP)/GetFactorOffense(OffNum)->GetResultData(RDATA_MRDAMAGE_SEC)+0.9999));
	}
	else
	{
		//	방어자의 현재 HP 에 공격자가 접근하면서 공격
		dTemp = dDefHP;
		dTemp = dTemp
			- (GetFactorOffense(OffNum)->GetResultData(RDATA_RDAMAGE_SEC) * dDataOff[4])
			;
		dDefHP = dTemp;
		if(dTemp<0.) 
		{
			dDataOff[4] = double(int(dDefHP/GetFactorOffense(OffNum)->GetResultData(RDATA_RDAMAGE_SEC)+0.9999));
			dDefHP=0.;
		}
	}

	//	공격자 무피해 원거리 공격 횟수
	GetFactorOffense(OffNum)->SetResultData(dDataOff[0], RDATA_ND_RATTACKCOUNT);
	//	공격자 무피해 마법 공격 횟수
	GetFactorOffense(OffNum)->SetResultData(dDataOff[1], RDATA_ND_MATTACKCOUNT);
	//	공격자 무피해 마법 원거리 공격 횟수
	GetFactorOffense(OffNum)->SetResultData(dDataOff[2], RDATA_ND_MRATTACKCOUNT);
	//	공격자 후속 공격 타입
	GetFactorOffense(OffNum)->SetResultData(dDataOff[3], RDATA_FA_TYPE);
	//	접근시 원거리 공격 횟수
	GetFactorOffense(OffNum)->SetResultData(dDataOff[4], RDATA_D_RATTACKCOUNT);

	//	방어자 무피해 원거리 공격 횟수
	GetFactorDefense(DefNum)->SetResultData(dDataDef[0], RDATA_ND_RATTACKCOUNT);
	//	방어자 무피해 마법 공격 횟수
	GetFactorDefense(DefNum)->SetResultData(dDataDef[1], RDATA_ND_MATTACKCOUNT);
	//	방어자 무피해 마법 원거리 공격 횟수
	GetFactorDefense(DefNum)->SetResultData(dDataDef[2], RDATA_ND_MRATTACKCOUNT);
	//	방어자 후속 공격 타입
	GetFactorDefense(DefNum)->SetResultData(dDataDef[3], RDATA_FA_TYPE);
	//	접근시 원거리 공격 횟수
	GetFactorDefense(DefNum)->SetResultData(dDataDef[4], RDATA_D_RATTACKCOUNT);

	//	공격자 후속 공격 횟수
	GetFactorOffense(OffNum)->SetResultData(dDefHP/dOffDmg+0.9999, RDATA_FA_COUNT);
	//	방어자 후속 공격 횟수
	GetFactorDefense(DefNum)->SetResultData(dOffHP/dDefDmg+0.9999, RDATA_FA_COUNT);

	////	공격자 총 공격 횟수
	//dOffNum = GetFactorOffense(OffNum)->GetResultData(RDATA_ND_RATTACKCOUNT) 
	//	+ GetFactorOffense(OffNum)->GetResultData(RDATA_ND_MATTACKCOUNT) 
	//	+ GetFactorOffense(OffNum)->GetResultData(RDATA_FA_COUNT);
	//GetFactorOffense(OffNum)->SetResultData(dOffNum, RDATA_WIN_ATTACK_COUNT);

	////	방어자 총 공격 횟수
	//dDefNum = GetFactorDefense(DefNum)->GetResultData(RDATA_ND_RATTACKCOUNT) 
	//	+ GetFactorDefense(DefNum)->GetResultData(RDATA_ND_MATTACKCOUNT) 
	//	+ GetFactorDefense(DefNum)->GetResultData(RDATA_FA_COUNT);
	//GetFactorDefense(DefNum)->SetResultData(dDefNum, RDATA_WIN_ATTACK_COUNT);

	//	전투 결과
	//	후속 공격 횟수+접근시 공격 횟수를 비교하여 공격자횟수가 적거나 같을 때
	if(GetFactorOffense(OffNum)->GetResultDataInt(RDATA_FA_COUNT) + GetFactorOffense(OffNum)->GetResultDataInt(RDATA_D_RATTACKCOUNT) <= GetFactorDefense(DefNum)->GetResultDataInt(RDATA_FA_COUNT) + GetFactorDefense(DefNum)->GetResultDataInt(RDATA_D_RATTACKCOUNT))
	{
		//	공격자 승리
        GetFactorOffense(OffNum)->SetResultData(0., RDATA_WIN_RESULT);
		//	공격자 실 후속 공격 횟수
        GetFactorOffense(OffNum)->SetResultData(GetFactorOffense(OffNum)->GetResultData(RDATA_FA_COUNT), RDATA_REAL_FA_COUNT);	//

		//	방어자 패배
        GetFactorDefense(DefNum)->SetResultData(1., RDATA_WIN_RESULT);
		//	방어자 실 후속 공격 횟수
        GetFactorDefense(DefNum)->SetResultData(GetFactorOffense(OffNum)->GetResultData(RDATA_FA_COUNT)-1., RDATA_REAL_FA_COUNT);	//
	}
	else
	{
		//	방어자 승리
        GetFactorDefense(DefNum)->SetResultData(0., RDATA_WIN_RESULT);
		//	방어자 실 후속 공격 횟수
        GetFactorDefense(DefNum)->SetResultData(GetFactorDefense(DefNum)->GetResultData(RDATA_FA_COUNT), RDATA_REAL_FA_COUNT);	//

		//	공격자 패배
        GetFactorOffense(OffNum)->SetResultData(1., RDATA_WIN_RESULT);
		//	공격자 실 후속 공격 횟수
        GetFactorOffense(OffNum)->SetResultData(GetFactorDefense(DefNum)->GetResultData(RDATA_FA_COUNT), RDATA_REAL_FA_COUNT);	//
	}

//	CalcWinCombatFactor2(GetFactorOffense(OffNum), GetFactorDefense(DefNum));
	CalcCombatFactor2(GetFactorDefense(DefNum), GetFactorOffense(OffNum));
	CalcWinCombatFactor2(GetFactorOffense(OffNum), GetFactorDefense(DefNum));
}

void CBalancePool::CalcCombatFactor2(LPFactor pFactorOffense, LPFactor pFactorDefense)
{
	double dData, dDmg;
	
	//	승리 총 공격 횟수
	dData = pFactorOffense->GetResultData(RDATA_ND_RATTACKCOUNT) 
		+ pFactorOffense->GetResultData(RDATA_ND_MATTACKCOUNT) 
		+ pFactorOffense->GetResultData(RDATA_ND_MRATTACKCOUNT) 
		+ pFactorOffense->GetResultData(RDATA_D_RATTACKCOUNT) 
		+ pFactorOffense->GetResultData(RDATA_FA_COUNT);
	pFactorOffense->SetResultData(dData, RDATA_WIN_ATTACK_COUNT);

	//	실 총 공격 횟수 계산
	dData = pFactorOffense->GetResultData(RDATA_ND_RATTACKCOUNT) 
		+ pFactorOffense->GetResultData(RDATA_ND_MATTACKCOUNT) 
		+ pFactorOffense->GetResultData(RDATA_ND_MRATTACKCOUNT) 
		+ pFactorOffense->GetResultData(RDATA_D_RATTACKCOUNT) 
		+ pFactorOffense->GetResultData(RDATA_REAL_FA_COUNT);
	pFactorOffense->SetResultData(dData, RDATA_REAL_ATTACK_COUNT);	

	switch(pFactorDefense->GetResultDataInt(RDATA_FA_TYPE))
	{
		case 0:
			dDmg = pFactorDefense->GetResultData(RDATA_DAMAGE_SEC);
			break;
		case 1:
			dDmg = pFactorDefense->GetResultData(RDATA_RDAMAGE_SEC);
			break;
		case 2:
			dDmg = pFactorDefense->GetResultData(RDATA_MDAMAGE_SEC);
			break;
	}
	//	전투시 받은 피해(전투시 HP 감소량)
	dData = (pFactorDefense->GetResultData(RDATA_RDAMAGE_SEC) * pFactorDefense->GetResultData(RDATA_ND_RATTACKCOUNT))
		+ (pFactorDefense->GetResultData(RDATA_MDAMAGE_SEC) * pFactorDefense->GetResultData(RDATA_ND_MATTACKCOUNT))
		+ (pFactorDefense->GetResultData(RDATA_MRDAMAGE_SEC) * pFactorDefense->GetResultData(RDATA_ND_MRATTACKCOUNT))
		+ (pFactorDefense->GetResultData(RDATA_RDAMAGE_SEC) * pFactorDefense->GetResultData(RDATA_D_RATTACKCOUNT))
		+ (dDmg * pFactorDefense->GetResultDataInt(RDATA_REAL_FA_COUNT)) ;
	pFactorOffense->SetResultData(dData, RDATA_COMBAT_DAMAGE);

	//	남은 HP
	dData = OffDataInt(DATA_HP) - pFactorOffense->GetResultDataInt(RDATA_COMBAT_DAMAGE);
	if(dData <0.) dData=0.;
	pFactorOffense->SetResultData(dData, RDATA_REMAIN_HP);

	//	HP 회복 시간
	dData = double((OffDataInt(DATA_HP)-pFactorOffense->GetResultDataInt(RDATA_REMAIN_HP)))/OffDataInt(DATA_HPRECOVERY)*5.+5.;
	if(pFactorOffense->GetResultDataInt(RDATA_COMBAT_DAMAGE)==0) dData-=5.;
	if(pFactorOffense->GetResultDataInt(RDATA_WIN_RESULT)==1) dData=0.;
	pFactorOffense->SetResultData(dData, RDATA_HP_RECOVERY_TIME);

	//	전투 소요 시간
	dData = pFactorOffense->GetResultData(RDATA_HP_RECOVERY_TIME)+pFactorOffense->GetResultDataInt(RDATA_REAL_ATTACK_COUNT)+pFactorDefense->GetResultDataInt(RDATA_ND_RATTACKCOUNT)+pFactorDefense->GetResultDataInt(RDATA_ND_MATTACKCOUNT);
	pFactorOffense->SetResultData(dData, RDATA_COMBAT_TIME);

	//	연속 승리 횟수
	if(pFactorOffense->GetResultDataInt(RDATA_COMBAT_DAMAGE)==0)
		dData = 99.;
	else
		dData = double(OffDataInt(DATA_HP)/pFactorOffense->GetResultDataInt(RDATA_COMBAT_DAMAGE));
	pFactorOffense->SetResultData(dData, RDATA_CONTINUOUS_WIN_COUNT);

	//	연속 전투 후 남은 HP
	dData = double(OffDataInt(DATA_HP)-pFactorOffense->GetResultDataInt(RDATA_COMBAT_DAMAGE)*pFactorOffense->GetResultDataInt(RDATA_CONTINUOUS_WIN_COUNT));
	if(pFactorOffense->GetResultDataInt(RDATA_COMBAT_DAMAGE)>=OffDataInt(DATA_HP)) dData=0;
	pFactorOffense->SetResultData(dData, RDATA_CONTINUOUS_REMAIN_HP);

	//	연속 전투 후 HP 회복 시간
	dData = double((OffDataInt(DATA_HP)-pFactorOffense->GetResultDataInt(RDATA_CONTINUOUS_REMAIN_HP)))/OffDataInt(DATA_HPRECOVERY)*5.+5.;
	if(pFactorOffense->GetResultDataInt(RDATA_COMBAT_DAMAGE)==0) dData-=5.;
	if(pFactorOffense->GetResultDataInt(RDATA_WIN_RESULT)==1) dData=0.;
	pFactorOffense->SetResultData(dData, RDATA_CONTINUOUS_HP_RECOVERY_TIME);

	//	연속 총 전투 시간
	dData = (pFactorOffense->GetResultDataInt(RDATA_REAL_ATTACK_COUNT)+pFactorDefense->GetResultDataInt(RDATA_ND_RATTACKCOUNT)+pFactorDefense->GetResultDataInt(RDATA_ND_MATTACKCOUNT)) * pFactorOffense->GetResultDataInt(RDATA_CONTINUOUS_WIN_COUNT) + pFactorOffense->GetResultData(RDATA_CONTINUOUS_HP_RECOVERY_TIME);
	if(pFactorOffense->GetResultDataInt(RDATA_WIN_RESULT)==1) 
		dData = pFactorOffense->GetResultDataInt(RDATA_COMBAT_TIME);
	pFactorOffense->SetResultData(dData, RDATA_CONTINUOUS_COMBAT_TIME);

	//	매 전투 평균 소요 시간
	if(pFactorOffense->GetResultDataInt(RDATA_CONTINUOUS_WIN_COUNT)==0) 
		dData = pFactorOffense->GetResultData(RDATA_CONTINUOUS_COMBAT_TIME);	
	else
		dData = pFactorOffense->GetResultData(RDATA_CONTINUOUS_COMBAT_TIME) / pFactorOffense->GetResultDataInt(RDATA_CONTINUOUS_WIN_COUNT);
	pFactorOffense->SetResultData(dData, RDATA_ONE_COMBAT_TIME);

	//	레벨 차이 계산
	if(pFactorDefense->GetDataInt(DATA_RACENUMBER)<3 && pFactorOffense->GetDataInt(DATA_RACENUMBER)<3)
	{
		//캐릭터 대 캐릭터
		dDmg = min(max(pFactorDefense->GetDataInt(DATA_LEVEL)-pFactorOffense->GetDataInt(DATA_LEVEL), 0), 20)/2.;
	}
	else
	{
		//캐릭터 대 몬스터
		dDmg = min(max(pFactorDefense->GetDataInt(DATA_LEVEL)-pFactorOffense->GetDataInt(DATA_LEVEL), 0), 10);
	}

	//물리 디버프 성공률
	dData = (GetStat(pFactorOffense->GetDataInt(DATA_LEVEL), pFactorOffense->GetDataInt(DATA_CLASSNUMBER), pFactorOffense->GetDataInt(DATA_RACENUMBER), 0) * 6.
		/(GetStat(pFactorOffense->GetDataInt(DATA_LEVEL), pFactorOffense->GetDataInt(DATA_CLASSNUMBER), pFactorOffense->GetDataInt(DATA_RACENUMBER), 0) * 6.
		+pFactorDefense->GetData(DATA_PHYSICALRESIST)
		)
		);
	//렙차에 의한 확률 보정
	dData = max(dData - (0.1 * dDmg), 0.1);
	pFactorOffense->SetResultData(dData, RDATA_DEBUF_RATIO);

	//마법 디버프 성공률
	dData = (GetStat(pFactorOffense->GetDataInt(DATA_LEVEL), pFactorOffense->GetDataInt(DATA_CLASSNUMBER), pFactorOffense->GetDataInt(DATA_RACENUMBER), 3) * 6.
		/(GetStat(pFactorOffense->GetDataInt(DATA_LEVEL), pFactorOffense->GetDataInt(DATA_CLASSNUMBER), pFactorOffense->GetDataInt(DATA_RACENUMBER), 3) * 6.
		+pFactorDefense->GetData(DATA_MAGICRESIST)
		)
		);

	//마법 디버프 실제 성공률
	dData = pFactorOffense->GetData(DATA_CURSERATIO) / 100 * dData;
	//렙차에 의한 확률 보정
	dData = max(dData - (0.1 * dDmg), 0.1);
	pFactorOffense->SetResultData(dData, RDATA_MDEBUF_RATIO);

}

void CBalancePool::CalcWinCombatFactor2(LPFactor pFactorOffense, LPFactor pFactorDefense)
{
	double dData, dDmg;
	
	//	승리 총 공격 횟수
	dData = pFactorOffense->GetResultData(RDATA_ND_RATTACKCOUNT) 
		+ pFactorOffense->GetResultData(RDATA_ND_MATTACKCOUNT) 
		+ pFactorOffense->GetResultData(RDATA_ND_MRATTACKCOUNT) 
		+ pFactorOffense->GetResultData(RDATA_D_RATTACKCOUNT) 
		+ pFactorOffense->GetResultData(RDATA_FA_COUNT);
	pFactorOffense->SetResultData(dData, RDATA_WIN_ATTACK_COUNT);

	//	실 총 공격 횟수 계산
	dData = pFactorOffense->GetResultData(RDATA_ND_RATTACKCOUNT) 
		+ pFactorOffense->GetResultData(RDATA_ND_MATTACKCOUNT) 
		+ pFactorOffense->GetResultData(RDATA_ND_MRATTACKCOUNT) 
		+ pFactorOffense->GetResultData(RDATA_D_RATTACKCOUNT) 
		+ pFactorOffense->GetResultData(RDATA_REAL_FA_COUNT);
	pFactorOffense->SetResultData(dData, RDATA_REAL_ATTACK_COUNT);	

	switch(pFactorDefense->GetResultDataInt(RDATA_FA_TYPE))
	{
		case 0:
			dDmg = pFactorDefense->GetResultData(RDATA_DAMAGE_SEC);
			break;
		case 1:
			dDmg = pFactorDefense->GetResultData(RDATA_RDAMAGE_SEC);
			break;
		case 2:
			dDmg = pFactorDefense->GetResultData(RDATA_MDAMAGE_SEC);
			break;
	}
	//	전투시 받은 피해(전투시 HP 감소량)
	dData = (pFactorDefense->GetResultData(RDATA_RDAMAGE_SEC) * pFactorDefense->GetResultData(RDATA_ND_RATTACKCOUNT))
		+ (pFactorDefense->GetResultData(RDATA_MDAMAGE_SEC) * pFactorDefense->GetResultData(RDATA_ND_MATTACKCOUNT))
		+ (pFactorDefense->GetResultData(RDATA_MRDAMAGE_SEC) * pFactorDefense->GetResultData(RDATA_ND_MRATTACKCOUNT))
		+ (pFactorDefense->GetResultData(RDATA_RDAMAGE_SEC) * pFactorDefense->GetResultData(RDATA_D_RATTACKCOUNT))
		+ (dDmg * pFactorDefense->GetResultDataInt(RDATA_REAL_FA_COUNT)) ;
	pFactorOffense->SetResultData(dData, RDATA_COMBAT_DAMAGE);

	//	남은 HP
	dData = OffDataInt(DATA_HP)+_HP_POTION - pFactorOffense->GetResultDataInt(RDATA_COMBAT_DAMAGE);
	if(dData <0.) dData=0.;
	pFactorOffense->SetResultData(dData, RDATA_REMAIN_HP);

	//	HP 회복 시간
	dData = double((OffDataInt(DATA_HP)+_HP_POTION-pFactorOffense->GetResultDataInt(RDATA_REMAIN_HP)))/OffDataInt(DATA_HPRECOVERY)*5.+5.;
	if(pFactorOffense->GetResultDataInt(RDATA_COMBAT_DAMAGE)==0) dData-=5.;
	if(pFactorOffense->GetResultDataInt(RDATA_WIN_RESULT)==1) dData=0.;
	pFactorOffense->SetResultData(dData, RDATA_HP_RECOVERY_TIME);

	//	전투 소요 시간
	dData = pFactorOffense->GetResultData(RDATA_HP_RECOVERY_TIME)+pFactorOffense->GetResultDataInt(RDATA_REAL_ATTACK_COUNT)+pFactorDefense->GetResultDataInt(RDATA_ND_RATTACKCOUNT)+pFactorDefense->GetResultDataInt(RDATA_ND_MATTACKCOUNT);
	pFactorOffense->SetResultData(dData, RDATA_COMBAT_TIME);

	//	연속 승리 횟수
	if(pFactorOffense->GetResultDataInt(RDATA_COMBAT_DAMAGE)==0)
		dData = 99.;
	else
		dData = double((OffDataInt(DATA_HP)+_HP_POTION)/pFactorOffense->GetResultDataInt(RDATA_COMBAT_DAMAGE));
	pFactorOffense->SetResultData(dData, RDATA_CONTINUOUS_WIN_COUNT);

	//	연속 전투 후 남은 HP
	dData = double(OffDataInt(DATA_HP)+_HP_POTION-pFactorOffense->GetResultDataInt(RDATA_COMBAT_DAMAGE)*pFactorOffense->GetResultDataInt(RDATA_CONTINUOUS_WIN_COUNT));
	if(pFactorOffense->GetResultDataInt(RDATA_COMBAT_DAMAGE)>=OffDataInt(DATA_HP)+_HP_POTION) dData=0;
	pFactorOffense->SetResultData(dData, RDATA_CONTINUOUS_REMAIN_HP);

	//	연속 전투 후 HP 회복 시간
	dData = double((OffDataInt(DATA_HP)+_HP_POTION-pFactorOffense->GetResultDataInt(RDATA_CONTINUOUS_REMAIN_HP)))/OffDataInt(DATA_HPRECOVERY)*5.+5.;
	if(pFactorOffense->GetResultDataInt(RDATA_COMBAT_DAMAGE)==0) dData-=5.;
	if(pFactorOffense->GetResultDataInt(RDATA_WIN_RESULT)==1) dData=0.;
	pFactorOffense->SetResultData(dData, RDATA_CONTINUOUS_HP_RECOVERY_TIME);

	//	연속 총 전투 시간
	dData = (pFactorOffense->GetResultDataInt(RDATA_REAL_ATTACK_COUNT)+pFactorDefense->GetResultDataInt(RDATA_ND_RATTACKCOUNT)+pFactorDefense->GetResultDataInt(RDATA_ND_MATTACKCOUNT)) * pFactorOffense->GetResultDataInt(RDATA_CONTINUOUS_WIN_COUNT) + pFactorOffense->GetResultData(RDATA_CONTINUOUS_HP_RECOVERY_TIME);
	if(pFactorOffense->GetResultDataInt(RDATA_WIN_RESULT)==1) 
		dData = pFactorOffense->GetResultDataInt(RDATA_COMBAT_TIME);
	pFactorOffense->SetResultData(dData, RDATA_CONTINUOUS_COMBAT_TIME);

	//	매 전투 평균 소요 시간
	if(pFactorOffense->GetResultDataInt(RDATA_CONTINUOUS_WIN_COUNT)==0) 
		dData = pFactorOffense->GetResultData(RDATA_CONTINUOUS_COMBAT_TIME);	
	else
		dData = pFactorOffense->GetResultData(RDATA_CONTINUOUS_COMBAT_TIME) / pFactorOffense->GetResultDataInt(RDATA_CONTINUOUS_WIN_COUNT);
	pFactorOffense->SetResultData(dData, RDATA_ONE_COMBAT_TIME);
}

void CBalancePool::CalcCombatFactor(LPFactor pFactorOffense, LPFactor pFactorDefense)
{
	double dData=0.;

	//	공격자의 전투시 결과

	//	초당 물리 공격력
//	dData = OffData(DATA_ATTACKPOINT)*5*OffDataInt(DATA_SPEED)/100.;
	dData = OffData(DATA_ATTACKPOINT) * 3.;
	if(OffDataInt(DATA_CLASSNUMBER)==5)	//소환사는
		dData = OffData(DATA_ATTACKPOINT) * 1.6;
	if(GetRaceStat(OffDataInt(DATA_RACENUMBER))->GetUseItem()==1)
		dData += OffData(DATA_ITEM_AP);
	dData = (dData)*(OffDataInt(DATA_SPEED)-100+OffDataInt(DATA_ITEM_SP))/100.;
	pFactorOffense->SetResultData(dData, RDATA_AP_SEC);
	//	초당 물리 공격력(치명)
	dData = OffResult(RDATA_AP_SEC)*(100.-OffDataInt(DATA_CRITICALRATIO))/100.
		+ (OffResult(RDATA_AP_SEC)+(OffResult(RDATA_AP_SEC)*OffDataInt(DATA_CRITICALMAX)*0.01)/2.)*OffDataInt(DATA_CRITICALRATIO)/100.;
	pFactorOffense->SetResultData(dData, RDATA_CRITCAL_AP_SEC);
	//	초당 물리 데미지
//	dData=(OffResult(RDATA_CRITCAL_AP_SEC)-(DefDataInt(DATA_ITEMDEFENSE)/8.))*OffDataInt(DATA_ATACKGRADE)/(OffDataInt(DATA_ATACKGRADE)+DefDataInt(DATA_DEFENSEGRADE));
	dData=(OffResult(RDATA_CRITCAL_AP_SEC)-(DefDataInt(DATA_ITEMDEFENSE)/8.))*CalcAttackRatio(pFactorOffense, pFactorDefense);
		//	초당 데미지에 소환수가 주는 데미지 추가 (소환수는 캐릭터의 공격등급을 따른다.)
//	dData += max((OffData(DATA_CREATURE_AP_SEC)-(DefDataInt(DATA_ITEMDEFENSE)/8.)), 0)*OffDataInt(DATA_ATACKGRADE)/(OffDataInt(DATA_ATACKGRADE)+DefDataInt(DATA_DEFENSEGRADE));
	dData += max((OffData(DATA_CREATURE_AP_SEC)-(DefDataInt(DATA_ITEMDEFENSE)/8.)), 0)*CalcAttackRatio(pFactorOffense, pFactorDefense);
	if(dData<1.) dData=1.;
	pFactorOffense->SetResultData(dData, RDATA_DAMAGE_SEC);
	//	평균 물리 공격 횟수
	dData=DefDataInt(DATA_HP)/OffResult(RDATA_DAMAGE_SEC);
	pFactorOffense->SetResultData(dData, RDATA_ATTACK_COUNT);

	//	초당 원거리 공격력
//	dData=(OffData(DATA_RANGEATTACKPOINT)*5.*OffDataInt(DATA_SPEED)/100.);
	dData = OffData(DATA_RANGEATTACKPOINT);
	if(OffDataInt(DATA_CLASSNUMBER)==2)	//궁사는
		dData = OffData(DATA_RANGEATTACKPOINT)*2;
	if(OffDataInt(DATA_CLASSNUMBER)==5)	//소환사는
		dData = OffData(DATA_RANGEATTACKPOINT)*1.6;
	if(GetRaceStat(OffDataInt(DATA_RACENUMBER))->GetUseItem()==1)
		dData += OffData(DATA_ITEM_RAP);
	dData = (dData)*(OffDataInt(DATA_SPEED)-100+OffDataInt(DATA_ITEM_RSP))/100.;
	pFactorOffense->SetResultData(dData, RDATA_RAP_SEC);
	//	초당 원거리 공격력(치명)
	dData=OffResult(RDATA_RAP_SEC)*(100.-OffDataInt(DATA_CRITICALRATIO))/100.+(OffResult(RDATA_RAP_SEC)+(OffResult(RDATA_RAP_SEC)*OffDataInt(DATA_CRITICALMAX)*0.01)/2.)*OffDataInt(DATA_CRITICALRATIO)/100.;
	pFactorOffense->SetResultData(dData, RDATA_CRITCAL_RAP_SEC);
	//	초당 원거리 데미지
//	dData=(OffResult(RDATA_CRITCAL_RAP_SEC)-(DefDataInt(DATA_ITEMDEFENSE)/8.))*OffDataInt(DATA_ATACKGRADE)/(OffDataInt(DATA_ATACKGRADE)+DefDataInt(DATA_DEFENSEGRADE));
	dData=(OffResult(RDATA_CRITCAL_RAP_SEC)-(DefDataInt(DATA_ITEMDEFENSE)/8.))*CalcAttackRatio(pFactorOffense, pFactorDefense);
		//	초당 데미지에 소환수가 주는 데미지 추가 (소환수는 캐릭터의 공격등급을 따른다.)
//	dData += max((OffData(DATA_CREATURE_AP_SEC)-(DefDataInt(DATA_ITEMDEFENSE)/8.)), 0)*OffDataInt(DATA_ATACKGRADE)/(OffDataInt(DATA_ATACKGRADE)+DefDataInt(DATA_DEFENSEGRADE));
	dData += max((OffData(DATA_CREATURE_AP_SEC)-(DefDataInt(DATA_ITEMDEFENSE)/8.)), 0)*CalcAttackRatio(pFactorOffense, pFactorDefense);
	if(dData<1.) dData=1.;
	pFactorOffense->SetResultData(dData, RDATA_RDAMAGE_SEC);
	//	평균 원거리 공격 횟수
	dData=DefDataInt(DATA_HP)/OffResult(RDATA_RDAMAGE_SEC);
	pFactorOffense->SetResultData(dData, RDATA_RATTACK_COUNT);

	//	초당 마법 공격력
//	dData=OffData(DATA_MAGICATTACKPOINT)*5.*OffDataInt(DATA_MAGICSPEED)/100.;
	dData = OffData(DATA_MAGICATTACKPOINT)*3.;
	if(OffDataInt(DATA_CLASSNUMBER)==5)	//소환사는
		dData = OffData(DATA_MAGICATTACKPOINT)*1.6;
	if(GetRaceStat(OffDataInt(DATA_RACENUMBER))->GetUseItem()==1)
		dData += OffData(DATA_ITEM_MAP);
	dData = (dData)*(OffDataInt(DATA_MAGICSPEED)-100+OffDataInt(DATA_ITEM_MSP))/100.;
	pFactorOffense->SetResultData(dData, RDATA_MAP_SEC);
	//	초당 마법 공격력(치명)
	dData=OffResult(RDATA_MAP_SEC)*(100.-OffDataInt(DATA_MAGICCRITICALRATIO))/100.+(OffResult(RDATA_MAP_SEC)+(OffResult(RDATA_MAP_SEC)*OffDataInt(DATA_MAGICCRITICALMAX)*0.01)/2.)*OffDataInt(DATA_MAGICCRITICALRATIO)/100.;
	pFactorOffense->SetResultData(dData, RDATA_CRITCAL_MAP_SEC);
	//	초당 마법 데미지
//	dData=OffResult(RDATA_CRITCAL_MAP_SEC)-(DefDataInt(DATA_ITEMMAGICDEFENSE)/8.);
	dData=OffResult(RDATA_CRITCAL_MAP_SEC)-(DefDataInt(DATA_ITEMMAGICDEFENSE)/8.)*CalcMagicRatio(pFactorOffense, pFactorDefense);
		//	초당 데미지에 소환수가 주는 데미지 추가
//	dData += max((OffData(DATA_CREATURE_AP_SEC)-(DefDataInt(DATA_ITEMDEFENSE)/8.)), 0)*OffDataInt(DATA_ATACKGRADE)/(OffDataInt(DATA_ATACKGRADE)+DefDataInt(DATA_DEFENSEGRADE));
	dData += max((OffData(DATA_CREATURE_AP_SEC)-(DefDataInt(DATA_ITEMDEFENSE)/8.)), 0)*CalcAttackRatio(pFactorOffense, pFactorDefense);
	if(dData<1.) dData=1.;
	pFactorOffense->SetResultData(dData, RDATA_MDAMAGE_SEC);
	//	평균 마법 공격 횟수
	dData=DefDataInt(DATA_HP)/OffResult(RDATA_MDAMAGE_SEC);
	pFactorOffense->SetResultData(dData, RDATA_MATTACK_COUNT);

	//	초당 마법 원거리 공격력
//	dData=OffData(DATA_MAGICATTACKPOINT)*5.*OffDataInt(DATA_MAGICSPEED)/100.;
	dData = OffData(DATA_MAGICRATTACKPOINT);
	if(GetRaceStat(OffDataInt(DATA_RACENUMBER))->GetUseItem()==1)
		dData += OffData(DATA_ITEM_MRAP);
	dData = (dData)*(OffDataInt(DATA_MAGICSPEED)-100+OffDataInt(DATA_ITEM_MRSP))/100.;
	pFactorOffense->SetResultData(dData, RDATA_MRAP_SEC);
	//	초당 마법 공격력(치명)
	dData=OffResult(RDATA_MRAP_SEC)*(100.-OffDataInt(DATA_MAGICCRITICALRATIO))/100.+(OffResult(RDATA_MRAP_SEC)+(OffResult(RDATA_MRAP_SEC)*OffDataInt(DATA_MAGICCRITICALMAX)*0.01)/2.)*OffDataInt(DATA_MAGICCRITICALRATIO)/100.;
	pFactorOffense->SetResultData(dData, RDATA_CRITCAL_MRAP_SEC);
	//	초당 마법 데미지
//	dData=OffResult(RDATA_CRITCAL_MRAP_SEC)-(DefDataInt(DATA_ITEMMAGICDEFENSE)/8.);
	dData=OffResult(RDATA_CRITCAL_MRAP_SEC)-(DefDataInt(DATA_ITEMMAGICDEFENSE)/8.)*CalcMagicRatio(pFactorOffense, pFactorDefense);
		//	초당 데미지에 소환수가 주는 데미지 추가
//	dData += max((OffData(DATA_CREATURE_AP_SEC)-(DefDataInt(DATA_ITEMDEFENSE)/8.)), 0)*OffDataInt(DATA_ATACKGRADE)/(OffDataInt(DATA_ATACKGRADE)+DefDataInt(DATA_DEFENSEGRADE));
	dData += max((OffData(DATA_CREATURE_AP_SEC)-(DefDataInt(DATA_ITEMDEFENSE)/8.)), 0)*CalcAttackRatio(pFactorOffense, pFactorDefense);
	if(dData<1.) dData=1.;
	pFactorOffense->SetResultData(dData, RDATA_MRDAMAGE_SEC);
	//	평균 마법 원거리 공격 횟수
	dData=DefDataInt(DATA_HP)/OffResult(RDATA_MRDAMAGE_SEC);
	pFactorOffense->SetResultData(dData, RDATA_MRATTACK_COUNT);

	//	총 HP 회복 시간
	dData=5.*OffDataInt(DATA_HP)/OffDataInt(DATA_HPRECOVERY);
	pFactorOffense->SetResultData(dData, RDATA_HPMAX_RECOVERY_TIME);
	//	총 MP 회복 시간
	dData=5.*OffDataInt(DATA_MP)/OffDataInt(DATA_MPRECOVERY);
	pFactorOffense->SetResultData(dData, RDATA_MPMAX_RECOVERY_TIME);
	//	최소 공격 횟수
	dData=min(OffResult(RDATA_MATTACK_COUNT), min(OffResult(RDATA_RATTACK_COUNT), OffResult(RDATA_ATTACK_COUNT)));
	pFactorOffense->SetResultData(dData, RDATA_MIN_ATTACKCOUNT);

	//	공격 타입	0:물리	1:원거리	2:	마법

	switch(OffDataInt(DATA_CLASSNUMBER))
	{
		case 0:	//	전사
			dData = 0.;
			break;
		case 1:	//	살수
			dData = 0.;
			break;
		case 2:	//	궁사
			dData = 1.;
			break;
		case 3:	//	마법사
			dData = 2.;
			break;
		case 4:	//	사제
			dData = 2.;
			break;
		case 5:	//	소환사
			dData = 2.;
			break;
	}

	pFactorOffense->SetResultData(dData, RDATA_ATTACK_TYPE);
}

double CBalancePool::CalcDP(int Level, int Class)
{

	/*
	double dData=0.;
	//	물리 상수
	double dConst = GetFormula(DATA_ITEMDEFENSE)->GetX();	
	//	방어력 비율
	double dRatio[8] = {0, 20, 40, 55, 65, 75, 90, 100};
	//	방어력 초기값
	double dInit[8];

	int i;
	//	방어구 종류
	int iType=0;
	int iLevel = Level+38;	//	+11 업그레이드시를 기준으로 성능 계산

	//	방어구의 종류 계산
	switch(Class)
	{
		case 0:	//	전사
			if(Level>=30) iType = 7;
			else if(Level>=20) iType = 6;
			else if(Level>=2) iType = 5;
			else iType = 1;
			break;
		case 1:	//	살수
			if(Level>=30) iType = 5;
			else if(Level>=20) iType = 4;
			else if(Level>=2) iType = 3;
			else iType = 1;
			break;
		case 2:	//	궁사
			if(Level>=30) iType = 6;
			else if(Level>=20) iType = 5;
			else if(Level>=2) iType = 3;
			else iType = 1;
			break;
		case 3:	//	마법사
			if(Level>=2) iType = 2;
			else iType = 1;
			break;
		case 4:	//	사제
			if(Level>=2) iType = 2;
			else iType = 1;
			break;
		case 5:	//	소환사
			if(Level>=20) iType = 4;
			else if(Level>=2) iType = 3;
			else iType = 1;
			break;
	}

	//	방어력 초기값 계산
	for(i=0 ; i<8 ; i++)
		dInit[i] = double(int(dRatio[i]*0.7));

	dData = double(int( dInit[iType] + iLevel * dConst*dRatio[iType] / 100.)  );

	return dData;
*/
	double dData=0.;
	int i, iArmorType=13;	//	기본값 - 의상
	BYTE bTemp;
	int ItemChartIndex, ItemMatterIndex, LevelOffset;

	//	방어구 착용 가능한 아이템 중 물리 방어력이 높은 것 선택 
	for (i=18 ; i>=14 ; i--)
	{
		bTemp = GetClassSkill(Level)->IsEnableItem(i, Class);
		if(bTemp>0)	
		{	
			iArmorType = (BYTE)(i);
			break;
		}
	}

	//	방어구 종류로 아이템의 인덱스를 구한다.
	ItemChartIndex = ChartDB_ITEMCHART_SearchIndex_Kind(iArmorType);
	if(ItemChartIndex == 0) return 0.;

	//
	LevelOffset = Level - TITEMCHART[ItemChartIndex].bLevel;


	//	장착 위치별 방어구 5개의 방어력을 더한다.
	for(i=0 ; i<5 ; i++)
	{
		//	아이템의 인덱스로 속성인덱스를 구한다.
		ItemMatterIndex = ChartDB_ITEMMATTERCHART_SearchIndex_ID(TITEMCHART[ItemChartIndex+i].wAttrID);
		if(ItemMatterIndex == 0) return 0.;

		dData += TITEMATTRCHART[ItemMatterIndex+LevelOffset].wDP;
	}


/*	검방 전사 계산용
	//	방패 아이템의 인덱스를 구한다.
	ItemChartIndex = ChartDB_ITEMCHART_SearchIndex_Kind(12);
	if(ItemChartIndex == 0) return 0.;
	LevelOffset = Level - TITEMCHART[ItemChartIndex].bLevel;
	ItemMatterIndex = ChartDB_ITEMMATTERCHART_SearchIndex_ID(TITEMCHART[ItemChartIndex].wAttrID);
	if(ItemMatterIndex == 0) return 0.;

	dData += TITEMATTRCHART[ItemMatterIndex+LevelOffset].wDP*TITEMATTRCHART[ItemMatterIndex+LevelOffset].bBlockProb/100;
//*/
	return dData;
}

double CBalancePool::CalcMDP(int Level, int Class)
{
/*
	double dData=0.;
	//	마법 상수
	double dConst = GetFormula(DATA_ITEMMAGICDEFENSE)->GetX();	
	//	방어력 비율
	double dRatio[8] = {0, 20, 100, 85, 75, 65, 50, 40};
	//	방어력 초기값
	double dInit[8];

	int i;
	//	방어구 종류
	int iType=0;
	int iLevel = Level+38;	//	+11 업그레이드시를 기준으로 성능 계산

	//	방어구의 종류 계산
	switch(Class)
	{
		case 0:	//	전사
			if(Level>=30) iType = 7;
			else if(Level>=20) iType = 6;
			else if(Level>=2) iType = 5;
			else iType = 1;
			break;
		case 1:	//	살수
			if(Level>=30) iType = 5;
			else if(Level>=20) iType = 4;
			else if(Level>=2) iType = 3;
			else iType = 1;
			break;
		case 2:	//	궁사
			if(Level>=30) iType = 6;
			else if(Level>=20) iType = 5;
			else if(Level>=2) iType = 3;
			else iType = 1;
			break;
		case 3:	//	마법사
			if(Level>=2) iType = 2;
			else iType = 1;
			break;
		case 4:	//	사제
			if(Level>=2) iType = 2;
			else iType = 1;
			break;
		case 5:	//	소환사
			if(Level>=20) iType = 4;
			else if(Level>=2) iType = 3;
			else iType = 1;
			break;
	}

	//	방어력 초기값 계산
	for(i=0 ; i<8 ; i++)
		dInit[i] = double(int(dRatio[i]*0.7));

	dData = double(int( dInit[iType] + iLevel * dConst*dRatio[iType] / 100.)  );

	return dData;
*/

	double dData=0.;
	int i, iArmorType=13;	//	기본값 - 의상
	BYTE bTemp;
	int ItemChartIndex, ItemMatterIndex, LevelOffset;

	//	방어구 착용 가능한 아이템 중 물리 방어력이 높은 것 선택 (이미 물리 방어력 기준으로 착용했으므로)
	for (i=18 ; i>=14 ; i--)
	{
		bTemp = GetClassSkill(Level)->IsEnableItem(i, Class);
		if(bTemp>0)	
		{	
			iArmorType = (BYTE)(i);
			break;
		}
	}

	//	방어구 종류로 아이템의 인덱스를 구한다.
	ItemChartIndex = ChartDB_ITEMCHART_SearchIndex_Kind(iArmorType);
	if(ItemChartIndex == 0) return 0.;

	//
	LevelOffset = Level - TITEMCHART[ItemChartIndex].bLevel;


	//	장착 위치별 방어구 5개의 방어력을 더한다.
	for(i=0 ; i<5 ; i++)
	{
		//	아이템의 인덱스로 속성인덱스를 구한다.
		ItemMatterIndex = ChartDB_ITEMMATTERCHART_SearchIndex_ID(TITEMCHART[ItemChartIndex+i].wAttrID);
		if(ItemMatterIndex == 0) return 0.;

		dData += TITEMATTRCHART[ItemMatterIndex+LevelOffset].wMDP;
	}

	return dData;

}

double CBalancePool::CalcAP(int Level, int Class, int IsSpeed)
{

	double dData=0., dTempData=0., dTempDataTotal=0., dSpeedData=0.;
	int i, iWeaponType=1;	//	기본값 - 한손검
	BYTE bTemp;
	int ItemChartIndex, SubItemChartIndex=0, ItemMatterIndex, SubItemMatterIndex=0, LevelOffset=0;
	int FoundItemChartIndex=0, FoundItemMatterIndex=0, FoundSubItemChartIndex=0, FoundSubItemMatterIndex=0, iSpeedTemp, ItemTempIndex=0;

	if(Level>=80) 
		Level=80;

	//	사용 가능한 아이템 중 물리 공격력이 높은 것 선택 
//	for (i=1 ; i<=1 ; i++)	//검방 전사일때	
	for (i=1 ; i<=12 ; i++)
	{
		bTemp = GetClassSkill(Level)->IsEnableItem(i, Class);
		if(bTemp>0)	
		{	
			iWeaponType = (BYTE)(i);
			//	아이템 종류로 아이템의 인덱스를 구한다.
			ItemChartIndex = ChartDB_ITEMCHART_SearchIndex_Kind(iWeaponType);
			if(ItemChartIndex == 0) continue;

			//	기본 무기(한손, 두손)외에는 제외한다.
			if(TITEMCHART[ItemChartIndex].bPrmSlotID!=0) continue;	

			//	한손 무기일 때
			if(TITEMCHART[ItemChartIndex].bSubSlotID!=1)
			{
				SubItemChartIndex = 0;
				if(GetClassSkill(Level)->IsEnableItem(2, Class)>0)	//	왼손무기 착용가능할 때
				{
					//	장착할 왼손 아이템의 인덱스를 구한다.
					SubItemChartIndex = ChartDB_ITEMCHART_SearchIndex_Kind(2);
				}
			}

			LevelOffset = Level - TITEMCHART[ItemChartIndex].bLevel;

			//	아이템의 인덱스로 속성인덱스를 구한다.
			ItemMatterIndex = ChartDB_ITEMMATTERCHART_SearchIndex_ID(TITEMCHART[ItemChartIndex].wAttrID);
			if(ItemMatterIndex == 0) continue;

			//	왼손 아이템의 인덱스가 있을 때 속성 인덱스를 구한다.
			if(SubItemChartIndex!=0)
				SubItemMatterIndex = ChartDB_ITEMMATTERCHART_SearchIndex_ID(TITEMCHART[SubItemChartIndex].wAttrID);

			ItemTempIndex = ChartDB_ITEMCHART_SearchIndex_Attr(TITEMATTRCHART[ItemMatterIndex+LevelOffset].wID);
			//	초당 공격력이 높은 아이템을 찾는다.
			dTempDataTotal = (TITEMATTRCHART[ItemMatterIndex+LevelOffset].wMinAP + TITEMATTRCHART[ItemMatterIndex+LevelOffset].wMaxAP)/2.;
			//	왼손 아이템의 인덱스가 있을 때
			if(SubItemChartIndex!=0)
				dTempDataTotal += (TITEMATTRCHART[SubItemMatterIndex+LevelOffset].wMinAP + TITEMATTRCHART[SubItemMatterIndex+LevelOffset].wMaxAP)/2.;
			dTempDataTotal += GetFactor()->GetData(DATA_ATTACKPOINT)*3;

			iSpeedTemp = GetFactor()->GetDataInt(DATA_SPEED)-100+TITEMCHART[ItemTempIndex].bSpeedInc;
			//	왼손 아이템의 인덱스가 있을 때
			if(SubItemChartIndex!=0)
				iSpeedTemp = iSpeedTemp-100+TITEMCHART[SubItemChartIndex].bSpeedInc;

			dTempDataTotal = (dTempDataTotal)*(iSpeedTemp)/100.;
//			dTempDataTotal = (TITEMATTRCHART[ItemMatterIndex+LevelOffset].wMinAP + TITEMATTRCHART[ItemMatterIndex+LevelOffset].wMaxAP)/200.*TITEMCHART[ItemChartIndex].bSpeedInc;
			if(dTempData<dTempDataTotal)
			{
				dTempData = dTempDataTotal;
				FoundItemChartIndex = ItemTempIndex;
				FoundItemMatterIndex = ItemMatterIndex+LevelOffset;
				FoundSubItemChartIndex = SubItemChartIndex;
				FoundSubItemMatterIndex = SubItemMatterIndex+LevelOffset;
			}
		}
	}

	dTempData = 0.;

	if(FoundItemMatterIndex>LevelOffset)
		dTempData += (TITEMATTRCHART[FoundItemMatterIndex].wMinAP + TITEMATTRCHART[FoundItemMatterIndex].wMaxAP)/2.;
	if(FoundSubItemMatterIndex>LevelOffset)
		dTempData += (TITEMATTRCHART[FoundSubItemMatterIndex].wMinAP + TITEMATTRCHART[FoundSubItemMatterIndex].wMaxAP)/2.;
	dData = dTempData;

	dSpeedData = 100.;
	if(FoundItemChartIndex>0)
		dSpeedData = TITEMCHART[FoundItemChartIndex].bSpeedInc;
	if(FoundSubItemChartIndex>0)
		dSpeedData += TITEMCHART[FoundSubItemChartIndex].bSpeedInc - 100;

	if(IsSpeed>0) 
		return dSpeedData;
	return dData;
}

double CBalancePool::CalcMAP(int Level, int Class, int IsSpeed)
{

	double dData=0., dTempData=0., dTempDataTotal=0., dSpeedData=0.;
	int i, iWeaponType=1;	//	기본값 - 한손검
	BYTE bTemp;
	int ItemChartIndex, SubItemChartIndex=0, ItemMatterIndex, SubItemMatterIndex=0, LevelOffset=0;
	int FoundItemChartIndex=0, FoundItemMatterIndex=0, FoundSubItemChartIndex=0, FoundSubItemMatterIndex=0, iSpeedTemp, ItemTempIndex=0;

	if(Level>=80) 
		Level=80;

	//	사용 가능한 아이템 중 마법 공격력이 높은 것 선택 
	for (i=1 ; i<=12 ; i++)
	{
		bTemp = GetClassSkill(Level)->IsEnableItem(i, Class);
		if(bTemp>0)	
		{	
			iWeaponType = (BYTE)(i);
			//	아이템 종류로 아이템의 인덱스를 구한다.
			ItemChartIndex = ChartDB_ITEMCHART_SearchIndex_Kind(iWeaponType);
			if(ItemChartIndex == 0) continue;

			//	기본 무기(한손, 두손)외에는 제외한다.
			if(TITEMCHART[ItemChartIndex].bPrmSlotID!=0) continue;	

			//	한손 무기일 때
			if(TITEMCHART[ItemChartIndex].bSubSlotID!=1)
			{
				SubItemChartIndex = 0;
				if(GetClassSkill(Level)->IsEnableItem(11, Class)>0)	//	왼손무기 착용가능할 때
				{
					//	장착할 왼손 아이템의 인덱스를 구한다.
					SubItemChartIndex = ChartDB_ITEMCHART_SearchIndex_Kind(11);
				}
			}

			LevelOffset = Level - TITEMCHART[ItemChartIndex].bLevel;

			//	아이템의 인덱스로 속성인덱스를 구한다.
			ItemMatterIndex = ChartDB_ITEMMATTERCHART_SearchIndex_ID(TITEMCHART[ItemChartIndex].wAttrID);
			if(ItemMatterIndex == 0) continue;

			//	왼손 아이템의 인덱스가 있을 때 메터 인덱스를 구한다.
			if(SubItemChartIndex!=0)
				SubItemMatterIndex = ChartDB_ITEMMATTERCHART_SearchIndex_ID(TITEMCHART[SubItemChartIndex].wAttrID);

			ItemTempIndex = ChartDB_ITEMCHART_SearchIndex_Attr(TITEMATTRCHART[ItemMatterIndex+LevelOffset].wID);
			//	초당 공격력이 높은 아이템을 찾는다.
			dTempDataTotal = (TITEMATTRCHART[ItemMatterIndex+LevelOffset].wMinMAP + TITEMATTRCHART[ItemMatterIndex+LevelOffset].wMaxMAP)/2.;
			//	왼손 아이템의 인덱스가 있을 때
			if(SubItemChartIndex!=0)
				dTempDataTotal += (TITEMATTRCHART[SubItemMatterIndex+LevelOffset].wMinMAP + TITEMATTRCHART[SubItemMatterIndex+LevelOffset].wMaxMAP)/2.;
			dTempDataTotal += GetFactor()->GetData(DATA_MAGICATTACKPOINT)*3;

			iSpeedTemp = GetFactor()->GetDataInt(DATA_SPEED)-100+TITEMCHART[ItemTempIndex].bSpeedInc;
			//	왼손 아이템의 인덱스가 있을 때
			if(SubItemChartIndex!=0)
				iSpeedTemp = iSpeedTemp-100+TITEMCHART[SubItemChartIndex].bSpeedInc;

			dTempDataTotal = (dTempDataTotal)*(iSpeedTemp)/100.;
//			dTempDataTotal = (TITEMATTRCHART[ItemMatterIndex+LevelOffset].wMinMAP + TITEMATTRCHART[ItemMatterIndex+LevelOffset].wMaxMAP)/200.*TITEMCHART[ItemChartIndex].bSpeedInc;
			if(dTempData<dTempDataTotal)
			{
				dTempData = dTempDataTotal;
				FoundItemChartIndex = ItemTempIndex;
				FoundItemMatterIndex = ItemMatterIndex+LevelOffset;
				FoundSubItemChartIndex = SubItemChartIndex;
				FoundSubItemMatterIndex = SubItemMatterIndex+LevelOffset;
			}
		}
	}

	dTempData = 0.;

	if(FoundItemMatterIndex>LevelOffset)
		dTempData += (TITEMATTRCHART[FoundItemMatterIndex].wMinMAP + TITEMATTRCHART[FoundItemMatterIndex].wMaxMAP)/2.;
	if(FoundSubItemMatterIndex>LevelOffset)
		dTempData += (TITEMATTRCHART[FoundSubItemMatterIndex].wMinMAP + TITEMATTRCHART[FoundSubItemMatterIndex].wMaxMAP)/2.;
	dData = dTempData;

	dSpeedData = 100.;
	if(FoundItemChartIndex>0)
		dSpeedData = TITEMCHART[FoundItemChartIndex].bSpeedInc;
	if(FoundSubItemChartIndex>0)
		dSpeedData += TITEMCHART[FoundSubItemChartIndex].bSpeedInc - 100;

	if(IsSpeed>0) return dSpeedData;
	return dData;
}

double CBalancePool::CalcRAP(int Level, int Class, int IsSpeed)
{

	double dData=0., dTempData=0., dTempDataTotal=0., dSpeedData=0.;
	int i, iWeaponType=6;	//	기본값 - 활
	BYTE bTemp;
	int ItemChartIndex, ItemMatterIndex, LevelOffset=0, FoundItemChartIndex=0, FoundItemMatterIndex=0, iSpeedTemp, ItemTempIndex=0;

	if(Level>=80) 
		Level=80;

	//	사용 가능한 아이템 중 물리 공격력이 높은 것 선택 
	for (i=1 ; i<=12 ; i++)
	{
		bTemp = GetClassSkill(Level)->IsEnableItem(i, Class);
		if(bTemp>0)	
		{	
			iWeaponType = (BYTE)(i);
			//	아이템 종류로 아이템의 인덱스를 구한다.
			ItemChartIndex = ChartDB_ITEMCHART_SearchIndex_Kind(iWeaponType);
			if(ItemChartIndex == 0) continue;

			//원거리 무기만 계산한다.
			if(TITEMCHART[ItemChartIndex].bPrmSlotID!=2) continue;	

			LevelOffset = Level - TITEMCHART[ItemChartIndex].bLevel;

			//	아이템의 인덱스로 속성인덱스를 구한다.
			ItemMatterIndex = ChartDB_ITEMMATTERCHART_SearchIndex_ID(TITEMCHART[ItemChartIndex].wAttrID);
			if(ItemMatterIndex == 0) continue;

			ItemTempIndex = ChartDB_ITEMCHART_SearchIndex_Attr(TITEMATTRCHART[ItemMatterIndex+LevelOffset].wID);
			//	초당 공격력이 높은 아이템을 찾는다.
			dTempDataTotal = (TITEMATTRCHART[ItemMatterIndex+LevelOffset].wMinAP + TITEMATTRCHART[ItemMatterIndex+LevelOffset].wMaxAP)/200.*TITEMCHART[ItemTempIndex].bSpeedInc;
			dTempDataTotal += GetFactor()->GetData(DATA_RANGEATTACKPOINT)*3;

			iSpeedTemp = GetFactor()->GetDataInt(DATA_SPEED)-100+TITEMCHART[ItemTempIndex].bSpeedInc;

			dTempDataTotal = (dTempDataTotal)*(iSpeedTemp)/100.;
			if(dTempData<dTempDataTotal)
			{
				dTempData = dTempDataTotal;
				FoundItemChartIndex = ItemTempIndex;
				FoundItemMatterIndex = ItemMatterIndex+LevelOffset;
			}
		}
	}

	dTempData = 0.;

	if(FoundItemMatterIndex>LevelOffset)
		dTempData += (TITEMATTRCHART[FoundItemMatterIndex].wMinAP + TITEMATTRCHART[FoundItemMatterIndex].wMaxAP)/2.;
	dData = dTempData;

	dSpeedData = 100.;

	if(FoundItemChartIndex>0)
		dSpeedData = TITEMCHART[FoundItemChartIndex].bSpeedInc;

	if(IsSpeed>0) return dSpeedData;
	return dData;
}

double CBalancePool::CalcMRAP(int Level, int Class, int IsSpeed)
{

	double dData=0., dTempData=0., dTempDataTotal=0., dSpeedData=0.;
	int i, iWeaponType=10;	//	기본값 - 금강저(마탄총)
	BYTE bTemp;
	int ItemChartIndex, ItemMatterIndex, LevelOffset=0, FoundItemChartIndex=0, FoundItemMatterIndex=0, iSpeedTemp, ItemTempIndex=0;

	if(Level>=80) 
		Level=80;

	//	사용 가능한 아이템 중 마법 공격력이 높은 것 선택 
	for (i=1 ; i<=12 ; i++)
	{
		bTemp = GetClassSkill(Level)->IsEnableItem(i, Class);
		if(bTemp>0)	
		{	
			iWeaponType = (BYTE)(i);
			//	아이템 종류로 아이템의 인덱스를 구한다.
			ItemChartIndex = ChartDB_ITEMCHART_SearchIndex_Kind(iWeaponType);
			if(ItemChartIndex == 0) continue;

			//원거리 무기만 계산한다.
			if(TITEMCHART[ItemChartIndex].bPrmSlotID!=2) continue;	

			LevelOffset = Level - TITEMCHART[ItemChartIndex].bLevel;

			//	아이템의 인덱스로 속성인덱스를 구한다.
			ItemMatterIndex = ChartDB_ITEMMATTERCHART_SearchIndex_ID(TITEMCHART[ItemChartIndex].wAttrID);
			if(ItemMatterIndex == 0) continue;

			ItemTempIndex = ChartDB_ITEMCHART_SearchIndex_Attr(TITEMATTRCHART[ItemMatterIndex+LevelOffset].wID);
			//	초당 공격력이 높은 아이템을 찾는다.
			dTempDataTotal = (TITEMATTRCHART[ItemMatterIndex+LevelOffset].wMinMAP + TITEMATTRCHART[ItemMatterIndex+LevelOffset].wMaxMAP)/200.*TITEMCHART[ItemTempIndex].bSpeedInc;
//			dTempDataTotal = (TITEMATTRCHART[ItemMatterIndex+LevelOffset].wMinMAP + TITEMATTRCHART[ItemMatterIndex+LevelOffset].wMaxMAP)/200.*TITEMCHART[ItemChartIndex].bSpeedInc;
			dTempDataTotal += GetFactor()->GetData(DATA_MAGICRATTACKPOINT)*3;

			iSpeedTemp = GetFactor()->GetDataInt(DATA_SPEED)-100+TITEMCHART[ItemTempIndex].bSpeedInc;
//			iSpeedTemp = GetFactor()->GetDataInt(DATA_SPEED)-100+TITEMCHART[ItemChartIndex].bSpeedInc;

			dTempDataTotal = (dTempDataTotal)*(iSpeedTemp)/100.;
			if(dTempData<dTempDataTotal)
			{
				dTempData = dTempDataTotal;
				FoundItemChartIndex = ItemTempIndex;
//				FoundItemChartIndex = ItemChartIndex;
				FoundItemMatterIndex = ItemMatterIndex+LevelOffset;
			}
		}
	}

	dTempData = 0.;

	if(FoundItemMatterIndex>LevelOffset)
		dTempData += (TITEMATTRCHART[FoundItemMatterIndex].wMinMAP + TITEMATTRCHART[FoundItemMatterIndex].wMaxMAP)/2.;
	dData = dTempData;

	dSpeedData = 100.;

	if(FoundItemChartIndex>0)
		dSpeedData = TITEMCHART[FoundItemChartIndex].bSpeedInc;

	if(IsSpeed>0) return dSpeedData;
	return dData;
}

double CBalancePool::CalcCreatureAP(int Level, int Class)
{
	double dData=0.;
//	int MonsterChartIndex;
	int Race;
	double Speed;

	if(GetClassSkill(Level)->IsSummon(Class)==1)
	{
//		MonsterChartIndex = ChartDB_MONSTERCHART_SearchIndex_ID(Level);
//		dData = TMONSTERCHART[MonsterChartIndex].wAP;
		Race = 72+int((min(Level+3, 80)-1)*7/79)*2;
		Speed = GetFormula(DATA_SPEED)->GetInit() + GetStat(Level, Class, Race, 1) * GetFormula(DATA_SPEED)->GetX();
//		CalcFactor(Level, Class, Race);
		dData = GetStat(Level, Class, Race, 0) * GetFormula(DATA_ATTACKPOINT)->GetX() * 3. * Speed/100.;
//			=(힘*'[T project 데이타 v1.0.xls]TFORMULACHART'!$D$6 * 3 * 속도/100)
	}

	return dData;
}

double CBalancePool::CalcAttackRatio(LPFactor pFactorOffense, LPFactor pFactorDefense)
{
	double dData=0.;
	int iLevelGap;

	iLevelGap = min(max(pFactorDefense->GetDataInt(DATA_LEVEL) - pFactorOffense->GetDataInt(DATA_LEVEL), 0), 10);

	dData = max(pFactorOffense->GetData(DATA_ATACKGRADE) / (pFactorOffense->GetData(DATA_ATACKGRADE) + pFactorDefense->GetDataInt(DATA_DEFENSEGRADE)) - (0.1*iLevelGap), 0.1);

	return dData;
}

double CBalancePool::CalcMagicRatio(LPFactor pFactorOffense, LPFactor pFactorDefense)
{
	double dData=0.;
	int iLevelGap;

	iLevelGap = min(max(pFactorDefense->GetDataInt(DATA_LEVEL) - pFactorOffense->GetDataInt(DATA_LEVEL), 0), 10);

	dData = max(1. - (0.1*iLevelGap), 0.1);

	return dData;
}

void CBalancePool::CopyFactorToTarget(LPFactor pFactor)
{
	int i;

	for(i=0 ; i<DATA_FACTOR_MAX ; i++)
	{
		pFactor->SetData(GetFactor()->GetData(i), i);
	}
}

void CBalancePool::CopyFactor(LPFactor pTFactor, LPFactor pSFactor)
{
	int i;

	for(i=0 ; i<DATA_FACTOR_MAX ; i++)
	{
		pTFactor->SetData(pSFactor->GetData(i), i);
	}
	for(i=0 ; i<DATA_RESULT_MAX ; i++)
	{
		pTFactor->SetResultData(pSFactor->GetResultData(i), i);
	}
}

void CBalancePool::AddFactor(LPFactor pTFactor, LPFactor pSFactor)
{
	int i;

	for(i=0 ; i<DATA_FACTOR_MAX ; i++)
	{
		if(i==DATA_CLASSNUMBER) continue;
		if(i==DATA_LEVEL) continue;
		if(i==DATA_RACENUMBER) continue;
		pTFactor->SetData(pTFactor->GetData(i) + pSFactor->GetData(i), i);
	}
	for(i=0 ; i<DATA_RESULT_MAX ; i++)
	{
		if(i==RDATA_ATTACK_TYPE) continue;
		if(i==RDATA_FA_TYPE) continue;
		if(i==RDATA_WIN_RESULT) continue;
		pTFactor->SetResultData(pTFactor->GetResultData(i) + pSFactor->GetResultData(i), i);
	}
}

void CBalancePool::AvrFactor(LPFactor pFactor, double dData)
{
	int i;

	for(i=0 ; i<DATA_FACTOR_MAX ; i++)
	{
		if(i==DATA_CLASSNUMBER) continue;
		if(i==DATA_LEVEL) continue;
		if(i==DATA_RACENUMBER) continue;
		pFactor->SetData(pFactor->GetData(i) / dData, i);
	}
	for(i=0 ; i<DATA_RESULT_MAX ; i++)
	{
		if(i==RDATA_ATTACK_TYPE) continue;
		if(i==RDATA_FA_TYPE) continue;
		if(i==RDATA_WIN_RESULT) continue;
		pFactor->SetResultData(pFactor->GetResultData(i) / dData, i);
	}
}

void CBalancePool::InitFactor(LPFactor pFactor)
{
	ZeroMemory(pFactor, sizeof(CFactor));
}

/*
void CBalancePool::CopyFactorToOffense(char c)
{
	int i;

	for(i=0 ; i<DATA_FACTOR_MAX ; i++)
	{
		GetFactorOffense(c)->SetData(GetFactor()->GetData(i), i);
	}
	//for(i=0 ; i<DATA_RESULT_MAX ; i++)
	//{
	//	GetFactorOffense(c)->SetResultData(GetFactor()->GetResultData(i), i);
	//}
}

void CBalancePool::CopyFactorToDefense(char c)
{
	int i;

	for(i=0 ; i<DATA_FACTOR_MAX ; i++)
	{
		GetFactorDefense(c)->SetData(GetFactor()->GetData(i), i);
	}
	//for(i=0 ; i<DATA_RESULT_MAX ; i++)
	//{
	//	GetFactorDefense(c)->SetResultData(GetFactor()->GetResultData(i), i);
	//}
}
*/

void CBalancePool::CopyResultToFactor(LPFactor pFactor)
{
	int i;

	for(i=0 ; i<DATA_RESULT_MAX ; i++)
	{
		GetFactor()->SetResultData(pFactor->GetResultData(i), i);
	}
}
/*
void CBalancePool::CopyResultOffenseToFactor(char c)
{
	int i;

	//for(i=0 ; i<DATA_FACTOR_MAX ; i++)
	//{
	//	GetFactor()->SetData(GetFactorOffense(c)->GetData(i), i);
	//}
	for(i=0 ; i<DATA_RESULT_MAX ; i++)
	{
		GetFactor()->SetResultData(GetFactorOffense(c)->GetResultData(i), i);
	}
}

void CBalancePool::CopyResultDefenseToFactor(char c)
{
	int i;

	//for(i=0 ; i<DATA_FACTOR_MAX ; i++)
	//{
	//	GetFactor()->SetData(GetFactorDefense(c)->GetData(i), i);
	//}
	for(i=0 ; i<DATA_RESULT_MAX ; i++)
	{
		GetFactor()->SetResultData(GetFactorDefense(c)->GetResultData(i), i);
	}
}
*/


BOOL CBalancePool::InsertRace(WORD num)
// 아이템 추가-CItemRoot 1개 메모리 할당해서...
{
	int i;
	if (num >= DATA_RACE_MAX) return FALSE;

	mRaceMax++;
	for(i=mRaceMax-1 ; i>num ; i--)
	{
		CopyRace(i, i-1);
	}
	mRaceStat[num+1]->Init();
	return TRUE;
}

BOOL CBalancePool::DeleteRace(WORD num)
{
	int i;
	if (num <= 0) return FALSE;
	if (num >= mRaceMax) return FALSE;

	for(i=num ; i<mRaceMax-1 ; i++)
	{
		CopyRace(i, i+1);
	}

	mRaceMax--;
	//	맨끝의 내용을 삭제한다.
	return TRUE;
}

BOOL CBalancePool::CopyRace(WORD tgtnum, WORD srcnum)
{
	int i;

	if (tgtnum <= 0||srcnum <= 0) return FALSE;
	if (tgtnum >= DATA_RACE_MAX||srcnum >= DATA_RACE_MAX) return FALSE;

	mRaceStat[tgtnum]->SetId(mRaceStat[srcnum]->GetId());
	
	for(i=0 ; i<7 ; i++)
	{
	mRaceStat[tgtnum]->SetStat(mRaceStat[srcnum]->GetStat(i), i);
	}
	mRaceStat[tgtnum]->SetName(mRaceStat[srcnum]->GetName());

	return TRUE;
}

