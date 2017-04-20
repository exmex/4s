#pragma once

#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "ChartDB.h"

#define DATA_BASESTAT_MAX	1
#define DATA_CLASSSTAT_MAX	6
#define DATA_RACESTAT_MAX	6
#define DATA_RACE_MAX	256

#define DATA_FACTOR_MAX	35
#define DATA_RESULT_MAX	41
#define DATA_ENABLEITEM_MAX	19

#define DATA_STATPOINT		6

#define _BAL_LEVELMAX	125
#define _BAL_VIEWLEVELMAX	(_BAL_LEVELMAX-25)

#define DATA_CHARSKILL_MAX		120

#define _STATFILE_VER	0.13

#define _HP_POTION	200

class CStat
{
	// Constructor, Destructor & private initializer
	public:
		CStat();
		~CStat();

	private :

	public:
		void Init(void);
		BOOL Save(FILE *);
		BOOL Load(FILE *);
		void CalcTotal(void);

		// Set
		void SetId(int i) { mId = i; };
		void SetStat(int i, char c=0) { mStatus[c] = i; };
		void SetStr(int i) { mStatus[0] = i; };
		void SetDex(int i) { mStatus[1] = i; };
		void SetCon(int i) { mStatus[2] = i; };
		void SetInt(int i) { mStatus[3] = i; };
		void SetWis(int i) { mStatus[4] = i; };
		void SetMen(int i) { mStatus[5] = i; };
		void SetTotal(int i) { mStatus[6] = i; };
		void SetName(char *ch) { strcpy(mName, ch); };
		void SetRange(WORD w, char c=0) { mRange[c] = w; };
		void SetUseItem(char c) { mUseItem = c; };
		void SetUseItemArmor(char c) { mUseItemArmor = c; };
		void SetSpeed(double f) { mfSpeed = f; };

		// Report Interfaces
		int GetId(void) { return mId; };
		int GetStat(char c=0) { return mStatus[c]; };
		int GetStr(void) { return mStatus[0]; };
		int GetDex(void) { return mStatus[1]; };
		int GetCon(void) { return mStatus[2]; };
		int GetInt(void) { return mStatus[3]; };
		int GetWis(void) { return mStatus[4]; };
		int GetMen(void) { return mStatus[5]; };
		int GetTotal(void) { return mStatus[6]; };
		char * GetName(void) { return mName; };
		WORD GetRange(char c=0) { return mRange[c]; };
		char GetUseItem(void) { return mUseItem; };
		char GetUseItemArmor(void) { return mUseItemArmor; };
		double GetSpeed(void) { return mfSpeed; };

	// Attributes
	protected:
		int mId;
		int	mStatus[7];
		char mName[_MAX_FNAME];
		WORD mRange[8];
		char mUseItem;
		char mUseItemArmor;
		double mfSpeed;
};
typedef CStat* LPStat;

//	사정거리
#define STAT_RANGE_MIN		0
#define STAT_RANGE_MAX		1
#define STAT_RRANGE_MIN		2
#define STAT_RRANGE_MAX		3
#define STAT_MRANGE_MIN		4
#define STAT_MRANGE_MAX		5
#define STAT_MRRANGE_MIN	6
#define STAT_MRRANGE_MAX	7

class CFormula
{
	// Constructor, Destructor & private initializer
	public:
		CFormula();
		~CFormula();

	private :

	public:
		BOOL Save(FILE *);
		BOOL Load(FILE *);

		// Set
		void SetId(int i) { mId = i; };
		void SetName(char *ch) { strcpy(mName, ch); };
		void SetInit(int i) { mInit = i; };
		void SetX(double d) { mRateX = d; };
		void SetY(double d) { mRateY = d; };

		// Report Interfaces
		int GetId(void) { return mId; };
		char * GetName(void) { return mName; };
		int GetInit(void) { return mInit; };
		double GetX(void) { return mRateX; };
		double GetY(void) { return mRateY; };

	// Attributes
	protected:
		int mId;
		char mName[50];
		int mInit;
		double mRateX;
		double mRateY;
};
typedef CFormula* LPFormula;

class CFactor
{
	// Constructor, Destructor & private initializer
	public:
		CFactor();
		~CFactor();

	private :

	public:
		// Set
		void SetData(double d, char c=0) { mData[c] = d; };
		void SetResultData(double d, char c=0) { mResultData[c] = d; };

		// Report Interfaces
		double GetData(char c=0) { return mData[c]; };
		int GetDataInt(char c=0) { return int(mData[c]); };
		double GetResultData(char c=0) { return mResultData[c]; };
		int GetResultDataInt(char c=0) { return int(mResultData[c]); };

	// Attributes
	protected:
		double mData[DATA_FACTOR_MAX];
		double mResultData[DATA_RESULT_MAX];
};
typedef CFactor* LPFactor;

//	mData에 저장되는 자료의 내용
#define DATA_ATTACKPOINT		0	//물리공격력
#define DATA_WEIGHT				1	//소지무게
#define DATA_RANGEATTACKPOINT	2	//원거리공격력
#define DATA_SPEED				3	//일반공격속도
#define DATA_ATACKGRADE			4	//공격등급
#define DATA_DEFENSEGRADE		5	//방어등급
#define DATA_CRITICALRATIO		6	//물리치명확률
#define DATA_HP					7	//생명력
#define DATA_HPRECOVERY			8	//생명력 회복율
#define DATA_PHYSICALRESIST		9	//물리상태저항
#define DATA_CRITICALMAX		10	//평균물리치명최대
#define DATA_ITEMDEFENSE		11	//아이템 물리방어력
#define DATA_MAGICATTACKPOINT	12	//마법공격력
#define DATA_CURSERATIO			13	//저주성공율
#define DATA_MAGICRATTACKPOINT	14	//마법원거리공격력
#define DATA_MAGICSPEED			15	//마법시전속도
#define DATA_MAGICRATIO			16	//마법성공율
#define DATA_MAGICCRITICALRATIO	17	//마법치명확률
#define DATA_MP					18	//정신력
#define DATA_MPRECOVERY			19	//정신력 회복율
#define DATA_MAGICRESIST		20	//마법상태저항
#define DATA_MAGICCRITICALMAX	21	//평균마법치명최대
#define DATA_ITEMMAGICDEFENSE	22	//아이템 마법방어력
#define DATA_CLASSNUMBER		23	//직업 번호
#define DATA_LEVEL				24	//레벨
#define DATA_ITEM_AP			25	//아이템 물리 공격력
#define DATA_ITEM_MAP			26	//아이템 마법 공격력
#define DATA_ITEM_RAP			27	//아이템 물리 원거리 공격력
#define DATA_ITEM_MRAP			28	//아이템 마법 원거리 공격력
#define DATA_ITEM_SP			29	//물리 기본 아이템 속도증감
#define DATA_ITEM_RSP			30	//물리 원거리 아이템 속도증감
#define DATA_ITEM_MSP			31	//마법 기본 아이템 속도증감
#define DATA_ITEM_MRSP			32	//마법 원거리 아이템 속도증감
#define DATA_CREATURE_AP_SEC	33	//소환수의 초당 물리 공격력
#define DATA_RACENUMBER			34	//종족 번호

//	mResultData에 저장되는 자료의 내용
#define RDATA_CRITCAL_AP_SEC				0	//초당물리공격력(치명)
#define RDATA_AP_SEC						1	//초당물리공격력
#define RDATA_DAMAGE_SEC					2	//초당물리데미지
#define RDATA_ATTACK_COUNT					3	//평균물리공격횟수
#define RDATA_CRITCAL_RAP_SEC				4	//초당원거리공격력(치명)
#define RDATA_RAP_SEC						5	//초당원거리공격력
#define RDATA_RDAMAGE_SEC					6	//초당원거리데미지
#define RDATA_RATTACK_COUNT					7	//평균원거리공격횟수
#define RDATA_CRITCAL_MAP_SEC				8	//초당마법공격력(치명)
#define RDATA_MAP_SEC						9	//초당마법공격력
#define RDATA_MDAMAGE_SEC					10	//초당마법데미지
#define RDATA_MATTACK_COUNT					11	//평균마법공격횟수
#define RDATA_CRITCAL_MRAP_SEC				12	//초당마법원거리공격력(치명)
#define RDATA_MRAP_SEC						13	//초당마법원거리공격력
#define RDATA_MRDAMAGE_SEC					14	//초당마법원거리데미지
#define RDATA_MRATTACK_COUNT				15	//평균마법원거리공격횟수
#define RDATA_HPMAX_RECOVERY_TIME			16	//총HP회복시간
#define RDATA_MPMAX_RECOVERY_TIME			17	//총MP회복시간
#define RDATA_MIN_ATTACKCOUNT				18	//최소공격횟수
#define RDATA_ATTACK_TYPE					19	//공격 타입	0:물리	1:원거리	2:	마법	3: 마법 원거리
#define RDATA_ND_RATTACKCOUNT				20	//무피해 원거리 공격 횟수
#define RDATA_ND_MATTACKCOUNT				21	//무피해 마법 공격 횟수
#define RDATA_ND_MRATTACKCOUNT				22	//무피해 마법 원거리 공격 횟수
#define RDATA_D_RATTACKCOUNT				23	//접근시 피해 원거리 공격 횟수
#define RDATA_FA_TYPE						24	//후속 공격 타입	0:물리	1:원거리	2:	마법	3: 마법 원거리
#define RDATA_FA_COUNT						25	//후속 공격 횟수
#define RDATA_WIN_ATTACK_COUNT				26	//승리 총 공격 횟수
#define RDATA_WIN_RESULT					27	//승패	0:승리	1:패배
#define RDATA_REAL_FA_COUNT					28	//실 후속 공격 횟수
#define RDATA_REAL_ATTACK_COUNT				29	//실 총 공격 횟수
#define RDATA_COMBAT_DAMAGE					30	//전투시 받은 피해
#define RDATA_REMAIN_HP						31	//남은 HP
#define RDATA_HP_RECOVERY_TIME				32	//HP 회복 시간
#define RDATA_COMBAT_TIME					33	//전투 소요 시간
#define RDATA_CONTINUOUS_WIN_COUNT			34	//연속 승리 횟수
#define RDATA_CONTINUOUS_REMAIN_HP			35	//연속 전투 후 남은 HP
#define RDATA_CONTINUOUS_HP_RECOVERY_TIME	36	//연속 전투 HP 회복 시간
#define RDATA_CONTINUOUS_COMBAT_TIME		37	//연속 총 전투 시간
#define RDATA_ONE_COMBAT_TIME				38	//매 전투 평균 소요 시간
#define RDATA_DEBUF_RATIO					39	//물리 디버프 성공률
#define RDATA_MDEBUF_RATIO					40	//마법 디버프 성공률

class CCharSkill
{
	// Constructor, Destructor & private initializer
	public:
		CCharSkill();
		~CCharSkill();

	private :

	public:
		// Set
		void SetSkillID(char dclass, int skillnum, WORD skillid) { mSkillID[dclass][skillnum] = skillid; };
		void SetEnableItem(DWORD d, char dclass=0) { mEnableItem[dclass] = d; };
		void EnableItemSetBit(BYTE b, char dclass=0) { mEnableItem[dclass] = mEnableItem[dclass]+(1<<(b-1)); };
		void EnableItemResetBit(BYTE b, char dclass=0) { mEnableItem[dclass] = mEnableItem[dclass]-(1<<(b-1)); };
		void SetIsSummon(BYTE b, char dclass=0) { mbIsSummmon[dclass]=b; };

		// Report Interfaces
		WORD GetSkillID(char dclass, int skillnum) { return mSkillID[dclass][skillnum]; };
		DWORD GetEnableItem(char dclass=0) { return mEnableItem[dclass]; };
		BYTE IsEnableItem(int b, char dclass=0) { return (BYTE)((mEnableItem[dclass]&(1<<(b-1)))>>(b-1)); };
		BYTE IsSummon(char dclass=0) { return mbIsSummmon[dclass]; };

	// Attributes
	protected:
		WORD mSkillID[DATA_CLASSSTAT_MAX][DATA_CHARSKILL_MAX];
		DWORD mEnableItem[DATA_CLASSSTAT_MAX];
		BYTE mbIsSummmon[DATA_CLASSSTAT_MAX];
};
typedef CCharSkill* LPCharSkill;
//	mEnableItem에 저장되는 자료의 내용 비트 연산 
//	1	한손검
//	2	보조검
//	3	두손검
//	4	차크람
//	5	도끼
//	6	활
//	7	석궁
//	8	마법막대
//	9	마법지팡이
//	10	금강저
//	11	금강령
//	12	방패
//	13	의복
//	14	제복
//	15	경가죽
//	16	중가죽
//	17	경갑옷
//	18	중갑옷

class CBalance
{
	// Constructor, Destructor & private initializer
	public:
		CBalance();
		~CBalance();

	private :

	public:
		// Set
		void SetLevel(int i) { mLevel = i; };
		void SetExp(int i) { mExp = i; };
		void SetExpTotal(int i) { mExpTotal = i; };
		void SetGoldTotal(int i) { mGoldTotal = i; };
		void SetMobHP(int i) { mMobHP = i; };
		void SetMobExp(int i) { mMobExp = i; };
		void SetMobGold(int i) { mMobGold = i; };
		void SetMobHunt(int i) { mMobHunt = i; };
		void SetMobHuntTotal(int i) { mMobHuntTotal = i; };

		// Report Interfaces
		int GetLevel(void) { return mLevel; };
		int GetExp(void) { return mExp; };
		int GetExpTotal(void) { return mExpTotal; };
		int GetGoldTotal(void) { return mGoldTotal; };
		int GetMobHP(void) { return mMobHP; };
		int GetMobExp(void) { return mMobExp; };
		int GetMobGold(void) { return mMobGold; };
		int GetMobHunt(void) { return mMobHunt; };
		int GetMobHuntTotal(void) { return mMobHuntTotal; };

	// Attributes
	protected:
		int	mLevel;

		//	Zgl.01. 경험치 밸런스를 위한 데이터 설정 
		int mExp;
		int mExpTotal;
		int mGoldTotal;
		int mMobHP;
		int mMobExp;
		int mMobGold;
		int mMobHunt;
		int mMobHuntTotal;

};
typedef CBalance* LPBalance;

class CBalancePool
{
	// Constructor, Destructor & private initializer
	public:
		CBalancePool();
		~CBalancePool();

	private :
		void InitName(void);

	public:
		void Init(void);
		void CreateMember(void);
		BOOL IsLoadFile(LPCSTR szFileName);	
		BOOL LoadFile(LPCSTR szFileName);	
		BOOL SaveFile(LPCSTR szFileName);

		//	Set
		void SetStatPoint(int data) { miStatPoint = data; };
		void SetClassName(int i, char *str) { strcpy(mClassName[i], str); };
		void SetRaceName(int i, char *str) { strcpy(mRaceName[i], str); };
		void SetRaceMax(WORD d) { mRaceMax = d; };

		// Report Interfaces
		int GetStatPoint(void) { return miStatPoint; };
		LPStat	GetBaseStat(WORD num) { return mBaseStat[num]; };
		LPStat	GetClassStat(WORD num) { return mClassStat[num]; };
		LPStat	GetRaceStat(WORD num) { return mRaceStat[num]; };
		WORD	GetRaceMax(void) { return mRaceMax; };
		LPFormula GetFormula(WORD num) { return mFormula[num]; };
		LPBalance GetBalance(WORD num) { return mBalance[num]; };
		char *	GetClassName(int i) { return mClassName[i]; };
		char *	GetRaceName(int i) { return mRaceName[i]; };
		WORD GetClassExCount(void) { return mClassExCount; };
		WORD GetRaceExCount(void) { return mRaceExCount; };
		LPFactor GetFactor(void) { return mFactor; };
		LPFactor GetFactorTemp(void) { return mFactorTemp; };
		LPFactor GetFactorOffense(int i=0) { return mFactorOffense[i]; };
		LPFactor GetFactorDefense(int i=0) { return mFactorDefense[i]; };
		int GetStat(int Level, int Class, int Race, int Sel);
		void CalcFactor(int Level, int Class, int Race);
		void CalcCombat(int OffLevel, int OffClass, int OffRace, int DefLevel, int DefClass, int DefRace, char OffNum=0, char DefNum=0);
		void CalcWinCombat(int OffLevel, int OffClass, int OffRace, int DefLevel, int DefClass, int DefRace, char OffNum=0, char DefNum=0);
		void CalcCombatFactor(LPFactor pFactorOffense, LPFactor pFactorDefense);
		void CalcCombatFactor2(LPFactor pFactorOffense, LPFactor pFactorDefense);
		void CalcWinCombatFactor2(LPFactor pFactorOffense, LPFactor pFactorDefense);
		double CalcDP(int Level, int Class);
		double CalcMDP(int Level, int Class);
		double CalcAP(int Level, int Class, int IsSpeed=0);
		double CalcMAP(int Level, int Class, int IsSpeed=0);
		double CalcRAP(int Level, int Class, int IsSpeed=0);
		double CalcMRAP(int Level, int Class, int IsSpeed=0);
		double CalcCreatureAP(int Level, int Class);
		double CalcAttackRatio(LPFactor pFactorOffense, LPFactor pFactorDefense);
		double CalcMagicRatio(LPFactor pFactorOffense, LPFactor pFactorDefense);
		void CopyFactorToTarget(LPFactor pFactor);
		void CopyFactor(LPFactor pTFactor, LPFactor pSFactor);
		void AddFactor(LPFactor pTFactor, LPFactor pSFactor);
		void AvrFactor(LPFactor pFactor, double dData);
		void InitFactor(LPFactor pFactor);
//		void CopyFactorToOffense(char c=0);
//		void CopyFactorToDefense(char c=0);
		void CopyResultToFactor(LPFactor pFactor);
//		void CopyResultOffenseToFactor(char c=0);
//		void CopyResultDefenseToFactor(char c=0);
		LPCharSkill GetClassSkill(int i=0) { return mClassSkill[i]; };

		BOOL InsertRace(WORD);
		BOOL DeleteRace(WORD);
		BOOL CopyRace(WORD, WORD);

		// Attributes
	protected:
		//	저장되는 멤버
		int miStatPoint;

		LPStat	mBaseStat[DATA_BASESTAT_MAX];
		LPStat	mClassStat[DATA_CLASSSTAT_MAX];
		LPStat	mRaceStat[DATA_RACE_MAX];
		WORD	mRaceMax;

		LPFormula mFormula[DATA_FACTOR_MAX];

		//	계산을 위한 멤버로 저장이 되지 않는다.
		char		mClassName[3][_MAX_FNAME];
		WORD		mClassExCount;
		char		mRaceName[1][_MAX_FNAME];
		WORD		mRaceExCount;
		LPBalance	mBalance[_BAL_LEVELMAX+1];
		LPFactor	mFactor;
		LPFactor	mFactorTemp;
		LPFactor	mFactorOffense[_BAL_LEVELMAX+1];
		LPFactor	mFactorDefense[_BAL_LEVELMAX+1];
		LPCharSkill	mClassSkill[_BAL_LEVELMAX+1];

};
typedef CBalancePool* LPBalancePool;

#define OffData(a) pFactorOffense->GetData(a)
#define OffDataInt(a) pFactorOffense->GetDataInt(a)
#define DefDataInt(a) pFactorDefense->GetDataInt(a)

#define OffResult(a) pFactorOffense->GetResultData(a)

