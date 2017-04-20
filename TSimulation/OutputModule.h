#pragma once

#include <windows.h>
#include <stdio.h>
#include "CBalance.h"

extern LPBalancePool pBalance;

void InitClassModule(void);
void DeleteClassModule(void);
void ChartToPool(void);
void ChartToPoolSkill(void);
void BaseStat(char *PathName=NULL);
void LevelStat(char *PathName=NULL);
void AllLevelStat(char *PathName=NULL);
void Combat1(char *PathName=NULL);
void CombatResult(LPFactor pFactor, int iLevel, int iRace, int iClass);
void ExpLevel(HWND, char *PathName=NULL);
void AllExpLevel(HWND, char *PathName=NULL);
