#ifndef __TCML_PARSER__
#define __TCML_PARSER__

#include "cinterf.h"
#include <Afx.h>

#include <map>
#include <string>
using namespace std;

typedef struct tagFRAMEDESC		FRAMEDESC, *LP_FRAMEDESC;
typedef struct tagCOMPINST		COMPINST, *LP_COMPINST;

struct tagCOMPINST
{
	CString m_strTooltip;
	CString m_strText;
	TSATR m_vEX;

	DWORD m_dwID;
	BYTE m_bType;

	DWORD m_vMENU[TCML_MENU_COUNT];
	DWORD m_dwImageID[2];
	DWORD m_dwTooltipID;
	DWORD m_dwFontID;
	DWORD m_dwStyle;
	DWORD m_dwCOLOR;
	DWORD m_dwSND;

	int m_nMargineH;
	int m_nMargineV;
	int m_nPosX;
	int m_nPosY;
	int m_nWidth;
	int m_nHeight;

	BYTE m_bDisplay;
	BYTE m_bAlign;
};

struct tagFRAMEDESC
{
	COMPINST m_vCOMP;

	LP_FRAMEDESC m_pCHILD;
	LP_FRAMEDESC m_pNEXT;

	tagFRAMEDESC()
	{
		m_pCHILD = NULL;
		m_pNEXT = NULL;
	};

	~tagFRAMEDESC()
	{
		if(m_pCHILD)
		{
			delete m_pCHILD;
			m_pCHILD = NULL;
		}

		if(m_pNEXT)
		{
			delete m_pNEXT;
			m_pNEXT = NULL;
		}
	};
};

typedef map<unsigned int, LP_TCML_LOGFONT>		FONT_MAP;
typedef map<unsigned int, LP_COMPDESC>			COMP_MAP;
typedef map<unsigned int, LP_FRAMEDESC>			FRAME_MAP;

#include "TCML.cpp.h"
int TCMLparse();

class TCMLParserProgress
{
public:
	virtual void OnProgress(float percent)=0;
};

class TCMLParser
{
public:
	void MoveFirstFont();
	LP_TCML_LOGFONT MoveNextFont();

	int AddLogFont(LP_TCML_LOGFONT ptr);
	LP_TCML_LOGFONT FindLogFont(unsigned int id);

	LP_FRAMEDESC FindFrameTemplate(unsigned int id);
	LP_FRAMEDESC LoadFRAME( FILE *pFILE);

	int AddParserTemplate( LP_COMPDESC ptr);

	TCMLParser();
	virtual ~TCMLParser();

	void Load( char* fname, TCMLParserProgress* pProgress=NULL);
	int Parse( char* fname );
	void Release();

	FRAME_MAP	m_Frames;
	COMP_MAP	m_Comps;
	FONT_MAP	m_Fonts;
	FONT_MAP::iterator m_it;
	BYTE m_bDeleteFont;
};

#endif
