#include "TCMLParser.h"
#include <stdio.h>


extern FILE *TCMLin;
extern COMPDESC* TCMLFrame;
extern LP_TCML_LOGFONT TCMLFont;

TCMLParser::TCMLParser()
{
	m_bDeleteFont = FALSE;
}

TCMLParser::~TCMLParser()
{
	Release();
}

void TCMLParser::Release()
{
	FRAME_MAP::iterator itFRAME;
	COMP_MAP::iterator itCOMP;
	FONT_MAP::iterator itFONT;

	for( itFRAME = m_Frames.begin(); itFRAME != m_Frames.end(); itFRAME++)
		delete (*itFRAME).second;
	m_Frames.clear();

	if(m_bDeleteFont)
	{
		for( itFONT = m_Fonts.begin(); itFONT != m_Fonts.end(); itFONT++)
			delete (*itFONT).second;
		m_bDeleteFont = FALSE;
	}

	FiniTCMLInterface();
	m_Fonts.clear();
	m_Comps.clear();
}

void TCMLParser::Load( char* fname, TCMLParserProgress* pProgress)
{
	FILE *pFILE = fopen( fname, "rb");
	int nCount = 0;

	fread( &nCount, sizeof(int), 1, pFILE);
	for( int i=0; i<nCount; i++)
	{
		LP_FRAMEDESC pFRAME = LoadFRAME(pFILE);

		if(pFRAME)
			m_Frames.insert( FRAME_MAP::value_type( pFRAME->m_vCOMP.m_dwID, pFRAME));

		if( pProgress )
			pProgress->OnProgress( (FLOAT) i / (FLOAT) nCount );
	}

	fread( &nCount, sizeof(int), 1, pFILE);
	for( i=0; i<nCount; i++)
	{
		LP_TCML_LOGFONT pFONT = new TCML_LOGFONT();

		fread( pFONT, sizeof(TCML_LOGFONT), 1, pFILE);
		m_Fonts.insert( FONT_MAP::value_type( pFONT->tlfId, pFONT));
	}
	m_bDeleteFont = TRUE;

	fclose(pFILE);
}

LP_FRAMEDESC TCMLParser::LoadFRAME( FILE *pFILE)
{
	LP_FRAMEDESC pFRAME = new FRAMEDESC();
	LP_FRAMEDESC *pNEXT = NULL;
	char pBUF[MAX_TCML_SYMBOL];
	DWORD dwID = 0;
	int nCount = 0;

	fread( &pFRAME->m_vCOMP.m_dwID, sizeof(DWORD), 1, pFILE);
	fread( &pFRAME->m_vCOMP.m_bType, sizeof(BYTE), 1, pFILE);

	fread( pFRAME->m_vCOMP.m_vMENU, sizeof(DWORD), TCML_MENU_COUNT, pFILE);
	fread( pFRAME->m_vCOMP.m_dwImageID, sizeof(DWORD), 2, pFILE);
	fread( &pFRAME->m_vCOMP.m_dwTooltipID, sizeof(DWORD), 1, pFILE);
	fread( &pFRAME->m_vCOMP.m_dwFontID, sizeof(DWORD), 1, pFILE);
	fread( &pFRAME->m_vCOMP.m_dwStyle, sizeof(DWORD), 1, pFILE);
	fread( &pFRAME->m_vCOMP.m_dwCOLOR, sizeof(DWORD), 1, pFILE);
	fread( &pFRAME->m_vCOMP.m_dwSND, sizeof(DWORD), 1, pFILE);

	fread( &pFRAME->m_vCOMP.m_nMargineH, sizeof(int), 1, pFILE);
	fread( &pFRAME->m_vCOMP.m_nMargineV, sizeof(int), 1, pFILE);
	fread( &pFRAME->m_vCOMP.m_nPosX, sizeof(int), 1, pFILE);
	fread( &pFRAME->m_vCOMP.m_nPosY, sizeof(int), 1, pFILE);
	fread( &pFRAME->m_vCOMP.m_nWidth, sizeof(int), 1, pFILE);
	fread( &pFRAME->m_vCOMP.m_nHeight, sizeof(int), 1, pFILE);

	fread( &pFRAME->m_vCOMP.m_bDisplay, sizeof(BYTE), 1, pFILE);
	fread( &pFRAME->m_vCOMP.m_bAlign, sizeof(BYTE), 1, pFILE);
	fread( &pFRAME->m_vCOMP.m_vEX, sizeof(TSATR), 1, pFILE);

	fread( &nCount, sizeof(int), 1, pFILE);
	if( nCount > 0 )
	{
		fread( pBUF, sizeof(char), nCount, pFILE);
		pBUF[nCount] = '\0';
		pFRAME->m_vCOMP.m_strTooltip.Format( "%s", pBUF);
	}

	fread( &nCount, sizeof(int), 1, pFILE);
	if( nCount > 0 )
	{
		fread( pBUF, sizeof(char), nCount, pFILE);
		pBUF[nCount] = '\0';
		pFRAME->m_vCOMP.m_strText.Format( "%s", pBUF);
	}

	fread( &nCount, sizeof(int), 1, pFILE);
	pNEXT = &pFRAME->m_pCHILD;

	for( int i=0; i<nCount; i++)
	{
		(*pNEXT) = LoadFRAME(pFILE);
		pNEXT = &(*pNEXT)->m_pNEXT;
	}

	return pFRAME;
}

int TCMLParser::Parse( char* fname)
{
	int ret= -1;

	TCMLin = fopen(fname, "r");
	if(InitTCMLInterface())
	{
		fclose(TCMLin);
		return -1;
	}

	ret = TCMLparse();

	if( 0 == ret )
	{
		LP_COMPDESC ptr1 = TCMLFrame;
		while(ptr1)
		{
			if(AddParserTemplate(ptr1))
			{
				fclose(TCMLin);
				return -2;
			}

			ptr1 = ptr1->next;
		}

		LP_TCML_LOGFONT ptr2 = TCMLFont;
		while(ptr2)
		{
			if(0!=AddLogFont(ptr2))
			{
				fclose(TCMLin);
				return -3;
			}

			ptr2 = ptr2->next;
		}
	}

	fseek( TCMLin, 0, SEEK_SET);
	fclose(TCMLin);

	return ret;
}

int TCMLParser::AddParserTemplate( LP_COMPDESC ptr)
{
	COMP_MAP::iterator finder = m_Comps.find(ptr->id);

	if( finder != m_Comps.end() )
		return 1;

	m_Comps.insert( COMP_MAP::value_type( ptr->id, ptr));
	return 0;
}

LP_FRAMEDESC TCMLParser::FindFrameTemplate( unsigned int id)
{
	FRAME_MAP::iterator finder = m_Frames.find(id);

	if( finder == m_Frames.end() )
		return NULL;

	return (*finder).second;
}

int TCMLParser::AddLogFont(LP_TCML_LOGFONT ptr)
{
	FONT_MAP::iterator it;
	it = m_Fonts.find(ptr->tlfId);
	if(it != m_Fonts.end())	return 1;

	m_Fonts.insert(FONT_MAP::value_type(ptr->tlfId, ptr));
	return 0;
}

LP_TCML_LOGFONT TCMLParser::FindLogFont(unsigned int id)
{
	FONT_MAP::iterator it;
	it = m_Fonts.find(id);
	if(it == m_Fonts.end())
		return NULL;

	return (LP_TCML_LOGFONT)(*it).second;
}

void TCMLParser::MoveFirstFont()
{
	m_it = m_Fonts.begin();
}

LP_TCML_LOGFONT TCMLParser::MoveNextFont()
{
	if(m_it != m_Fonts.end())
		return (*m_it++).second;

	return NULL;
}
