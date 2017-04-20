#pragma once

class CTFameRank
{
public:
	CTFameRank();
	virtual ~CTFameRank();

public:
	DWORD dwTotalRank;
	DWORD dwMonthRank;
	DWORD dwCharID;
	CString strName;
	DWORD dwTotalPoint;
	DWORD dwMonthPoint;
	WORD wWin;
	WORD wLose;
	DWORD dwTotalWin;
	DWORD dwTotalLose;
	BYTE bCountry;
	BYTE bLevel;
	BYTE bClass;
	BYTE bRace;
	BYTE bSex;
	BYTE bHair;
	BYTE bFace;
	CString strSay;
	CString strGuild;
};
