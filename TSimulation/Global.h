#pragma once

extern HWND hWndMain, hWndEdit, hWndPage1, hWndPage2, hWndPage3, hWndPage4;
extern HWND hWndReportPage1, hWndReportPage2, hWndReportPage3, hWndReportPage4;
extern HINSTANCE hInstMain;

//	메뉴에서 사용되는 임시 변수
struct EditResultData
{
	int		mLevel;
	char	mRace;
	char	mRace2;
	char	mClass;

	int		mLevelAt;
	char	mRaceAt;
	char	mClassAt;

	int		mLevelDf;
	char	mRaceDf;
	char	mClassDf;

	int		mExpLevelAt;
	char	mExpRaceAt;
	char	mExpRaceAt2;
	char	mExpClassAt;

	int		mExpLevelDf;
	char	mExpRaceDf;
	char	mExpRaceDf2;
	char	mExpClassDf;

	char	mAllExpRaceAt;
	char	mAllExpRaceAt2;
	char	mAllExpClassAt;
	char	mAllExpClassDf;

	char	mIsFile;
};

extern EditResultData gsLevelStat;

extern char ReportPath[_MAX_FNAME];
