#pragma once

struct STRUCTTBASESTATCHART {
BYTE bBaseID;
WORD wSTR;
WORD wDEX;
WORD wCON;
WORD wINT;
WORD wWIS;
WORD wMEN;
char szName[50];
};
STRUCTTBASESTATCHART TBASESTATCHART[];
extern int giTBASESTATCHART_COUNT;

struct STRUCTTCLASSCHART {
BYTE bClassID;
WORD wSTR;
WORD wDEX;
WORD wCON;
WORD wINT;
WORD wWIS;
WORD wMEN;
char szName[50];
WORD wRangeMin;
WORD wRangeMax;
WORD wRRangeMin;
WORD wRRangeMax;
WORD wMRangeMin;
WORD wMRangeMax;
WORD wMRRangeMin;
WORD wMRRangeMax;
};
STRUCTTCLASSCHART TCLASSCHART[];
extern int giTCLASSCHART_COUNT;

struct STRUCTTRACECHART {
BYTE bRaceID;
WORD wSTR;
WORD wDEX;
WORD wCON;
WORD wINT;
WORD wWIS;
WORD wMEN;
char szName[50];
BYTE bUseItem;
BYTE bUseItemArmor;
double fSpeed;
};
STRUCTTRACECHART TRACECHART[];
extern int giTRACECHART_COUNT;


struct STRUCTTFORMULACHART {
BYTE bID;
char szName[50];
DWORD dwInit;
double fRateX;
double fRateY;
};
STRUCTTFORMULACHART TFORMULACHART[];
extern int giTFORMULACHART_COUNT;

struct STRUCTTITEMCHART {
WORD wItemID;
BYTE bType;
BYTE bKind;
char szNAME[50];
WORD wAttrID;
WORD wUseValue;
DWORD dwSlotID;
DWORD dwClassID;
BYTE bPrmSlotID;
BYTE bSubSlotID;
BYTE bLevel;
double fPrice;
BYTE bMinRange;
BYTE bMaxRange;
BYTE bStack;
BYTE bEquipSkill;
BYTE bSlotCount;
BYTE bUseItemKind;
BYTE bUseItemCount;
BYTE bCanGrade;
BYTE bCanMagic;
BYTE bCanRare;
BYTE bCanEnable;
BYTE bSpeedInc;
double fRevision;
double fMRevision;
double fAtRate;
double fMAtRate;
BYTE bGrade;
DWORD dwDelay;
BYTE bIsSell;
BYTE bItemCountry;
BYTE bIsSpecial;
WORD wUseTime;
};
STRUCTTITEMCHART TITEMCHART[];
extern int giTITEMCHART_COUNT;

struct STRUCTTITEMATTRCHART {
WORD wID;
BYTE bGrade;
WORD wMinAP;
WORD wMaxAP;
WORD wDP;
WORD wMinMAP;
WORD wMaxMAP;
WORD wMDP;
BYTE bBlockProb;
};
STRUCTTITEMATTRCHART TITEMATTRCHART[];
extern int giTITEMATTRCHART_COUNT;

struct STRUCTTSKILLCHART {
WORD wID;
char szName[50];
WORD wParentSkillID;
WORD wPrevActiveID;
DWORD dwClassID;
BYTE bKind;
BYTE bPriority;
BYTE bPositive;
BYTE bLevel;
BYTE bNextLevel;
BYTE bMaxLevel;
double fPrice;
DWORD dwWeaponID;
DWORD dwUseHP;
DWORD dwUseHPInc;
DWORD dwUseMP;
DWORD dwUseMPInc;
DWORD dwActionTime;
int  bIsRide;
DWORD dwReuseDelay;
int nReuseDelayInc;
DWORD dwKindDelay;
BYTE bIsDismount;
DWORD dwLoopDelay;
BYTE bSpeedApply;
DWORD dwAggro;
DWORD dwAggroInc;
DWORD dwConditionID;
WORD wPosture;
BYTE bIsuse;
BYTE bCanLearn;
BYTE bTargetHit;
BYTE bTarget;
BYTE bTargetRange;
WORD wMinRange;
WORD wMaxRange;
DWORD dwDuration;
DWORD dwDurationInc;
BYTE bUseTypeSP;
BYTE bUseSP;
WORD wTargetActiveID;
BYTE bMaintainType;
};
STRUCTTSKILLCHART TSKILLCHART[];
extern int giTSKILLCHART_COUNT;

struct STRUCTTSKILLDATA {
WORD wSkillID;
BYTE bAction;
BYTE bType;
BYTE bAttr;
BYTE bExec;
BYTE bInc;
WORD wValue;
WORD wValueInc;
BYTE bProb;
BYTE bProbInc;
};
STRUCTTSKILLDATA TSKILLDATA[];
extern int giTSKILLDATA_COUNT;

struct STRUCTTMONSTERCHART {
WORD wID;
char szName[50];
BYTE bLevel;
BYTE bRace;
BYTE bClass;
WORD wKind;
BYTE bAIType;
BYTE bIsSelf;
BYTE bRecallType;
BYTE bAtkRange;
BYTE bRange;
WORD wChaseRange;
WORD wExp;
BYTE bMoneyProb;
BYTE bDropCount;
BYTE bItemProb;
DWORD dwMinMoney;
DWORD dwMaxMoney;
BYTE bRoamProb;
BYTE bCall;
BYTE bRemove;
WORD wTransSkillID;
WORD wSkill1;
WORD wSkill2;
WORD wSkill3;
WORD wSkill4;
BYTE bIsSpecial;
WORD wMonAttr;
WORD wSummonAttr;
BYTE bCanSelect;
BYTE bCanAttack;
BYTE bTame;
double fSize;
};
STRUCTTMONSTERCHART TMONSTERCHART[];
extern int giTMONSTERCHART_COUNT;

//	찾는 함수
int ChartDB_ITEMCHART_SearchIndex_Kind(BYTE);
int ChartDB_ITEMCHART_SearchIndex_Attr(WORD);
int ChartDB_ITEMMATTERCHART_SearchIndex_ID(WORD);

