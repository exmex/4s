#include "Stdafx.h"
#include "TTextLinker.h"

// ===============================================================================
const CString	CTTextLinker::LINK_DEF_TOK(_T("\n\r"));

const CHAR		CTTextLinker::LINK_STR_START		= '[';
const CHAR		CTTextLinker::LINK_STR_END			= ']';
const DWORD		CTTextLinker::LINK_DATA_POOL_SIZE	= 100;
const DWORD		CTTextLinker::LINK_QUEST_COLOR		= 0xFFFFFF7F;
// ===============================================================================

// ===============================================================================
CTTextLinker::CTTextLinker()
{
	m_nTextLinkDataPos = 0;
	m_pTextLinkDataPool = new CTTextLinkData[LINK_DATA_POOL_SIZE];
}
// -------------------------------------------------------------------------------
CTTextLinker::~CTTextLinker()
{
	delete [] m_pTextLinkDataPool;
}
// -------------------------------------------------------------------------------
CTTextLinker* CTTextLinker::GetInstance()
{
	static CTTextLinker instance;
	return &instance;
}
// ===============================================================================

// ===============================================================================
DWORD CTTextLinker::NewTextLinkData(CTTextLinkData** pOut)
{
	*pOut = &(m_pTextLinkDataPool[ m_nTextLinkDataPos ]);
	
	(*pOut)->FromStr("");
	(*pOut)->ResetPosition();

	DWORD nRetID = m_nTextLinkDataPos;
	if( m_nTextLinkDataPos >= LINK_DATA_POOL_SIZE-1 )
		m_nTextLinkDataPos = 0;
	else
		++m_nTextLinkDataPos;

	return nRetID;
}
// -------------------------------------------------------------------------------
CTTextLinkData* CTTextLinker::GetTextLinkData(DWORD nPos) const
{
	return &(m_pTextLinkDataPool[nPos]);
}

// ===============================================================================
CString CTTextLinker::MakeItemNetText( const CString& strFormat, const VDWORD& vItemID)
{
	WORD wSIZE = TCHAT_DWORD_SIZE;
	WORD wPOS = 0;

	CString strHEAD(_T(""));
	CString strBODY(_T(""));

	for( int i=0; i<INT(vItemID.size()); i++)
	{
		int nPOS = strFormat.Find( _T("%s"), wPOS);

		if( nPOS < wPOS )
			break;

		CTTextLinkData *pLINK = GetTextLinkData(vItemID[i]);
		WORD wLEN = nPOS - wPOS;

		if(wLEN)
		{
			strBODY += strFormat.Mid( wPOS, wLEN);
			wSIZE += wLEN;

			if( wSIZE > TCHAT_CHAR_MAX )
			{
				wLEN = strBODY.GetLength();
				wSIZE -= TCHAT_CHAR_MAX;

				if( wSIZE > wLEN )
					return BuildNetString( CString(_T("")), CString(_T("")));

				return BuildNetString( strHEAD, strBODY.Left(wLEN - wSIZE));
			}
		}

		if(pLINK)
		{
			const CString& strLINK = pLINK->ToStr();
			WORD vLINK[2] = {
				strLINK.GetLength(),
				strBODY.GetLength()};

			if(vLINK[0])
			{
				CString strSIZE;

				if( wSIZE + vLINK[0] + TCHAT_DWORD_SIZE > TCHAT_CHAR_MAX )
					return BuildNetString( strHEAD, strBODY);

				strSIZE.Format( _T("%04X%04X"), vLINK[0], vLINK[1]);
				strHEAD += strSIZE + strLINK;
				wSIZE += vLINK[0] + TCHAT_DWORD_SIZE;
			}
		}

		wPOS = nPOS + 2;
	}

	WORD wLEN = strFormat.GetLength();
	if( wPOS < wLEN )
	{
		strBODY += strFormat.Mid(wPOS);
		wSIZE += wLEN - wPOS;

		if( wSIZE > TCHAT_CHAR_MAX )
		{
			wLEN = strBODY.GetLength();
			wSIZE -= TCHAT_CHAR_MAX;

			if( wSIZE > wLEN )
				return BuildNetString( CString(_T("")), CString(_T("")));

			strBODY = strBODY.Left(wLEN - wSIZE);
		}
	}

	return BuildNetString( strHEAD, strBODY);
}

// ===============================================================================
CString CTTextLinker::BuildNetString( const CString& strHeader, const CString& strBody)
{
	CString strSIZE;

	strSIZE.Format( _T("%04X%04X"), strHeader.GetLength(), strBody.GetLength());
	return strSIZE + strHeader + strBody;
}

// -------------------------------------------------------------------------------
CString CTTextLinker::AppendNetString( const CString& strMSG1, const CString& strMSG2)
{
	WORD wLENGTH = strMSG1.GetLength();

	if( wLENGTH < TCHAT_DWORD_SIZE )
		return BuildNetString( CString(_T("")), CString(_T("")));

	DWORD dwCODE = 0;
	WORD wHEAD = 0;
	WORD wBODY = 0;
	WORD wBASE = 0;

	sscanf( LPCSTR(strMSG1.Left(TCHAT_DWORD_SIZE)), _T("%X"), &dwCODE);
	wHEAD = HIWORD(dwCODE);
	wBODY = LOWORD(dwCODE);

	if( wHEAD + wBODY + TCHAT_DWORD_SIZE != wLENGTH )
		return BuildNetString( CString(_T("")), CString(_T("")));

	CString strBODY = strMSG1.Mid( TCHAT_DWORD_SIZE + wHEAD, wBODY);
	CString strHEAD = strMSG1.Mid( TCHAT_DWORD_SIZE, wHEAD);
	CString strDATA(_T(""));

	wLENGTH = strMSG2.GetLength();
	wBASE = wBODY;

	if( wLENGTH < TCHAT_DWORD_SIZE )
		return strMSG1;

	sscanf( LPCSTR(strMSG2.Left(TCHAT_DWORD_SIZE)), _T("%X"), &dwCODE);
	wHEAD = HIWORD(dwCODE);
	wBODY = LOWORD(dwCODE);

	if( wHEAD + wBODY + TCHAT_DWORD_SIZE != wLENGTH )
		return strMSG1;

	strBODY += strMSG2.Mid( TCHAT_DWORD_SIZE + wHEAD, wBODY);
	strDATA = strMSG2.Mid( TCHAT_DWORD_SIZE, wHEAD);

	wLENGTH = strDATA.GetLength();
	wBODY = strBODY.GetLength();

	while( wLENGTH >= TCHAT_DWORD_SIZE )
	{
		CString strSIZE;

		sscanf( LPCSTR(strDATA.Left(TCHAT_DWORD_SIZE)), _T("%X"), &dwCODE);
		LPWORD pPOS = LPWORD(&dwCODE);

		if( wLENGTH < TCHAT_DWORD_SIZE + pPOS[1] ||
			wBODY < wBASE + pPOS[0] )
			break;

		pPOS[0] += wBASE;
		strSIZE.Format( _T("%08X"), dwCODE);

		strHEAD += strSIZE + strDATA.Mid( TCHAT_DWORD_SIZE, pPOS[1]);
		strDATA = strDATA.Mid(TCHAT_DWORD_SIZE + pPOS[1]);
		wLENGTH = strDATA.GetLength();
	}

	return BuildNetString( strHEAD, strBODY);
}

// ===============================================================================
CString CTTextLinker::SplitTextByCrLf( const CString& strMSG, const CString& strTOK, INT& nPos, BOOL bTrimCrLf)
{
	WORD wLENGTH = strMSG.GetLength();

	if( wLENGTH < TCHAT_DWORD_SIZE )
		return CString(_T(""));

	DWORD dwCODE = 0;
	WORD wHEAD = 0;
	WORD wBODY = 0;

	sscanf( LPCSTR(strMSG.Left(TCHAT_DWORD_SIZE)), _T("%X"), &dwCODE);
	wHEAD = HIWORD(dwCODE);
	wBODY = LOWORD(dwCODE);

	if( wHEAD + wBODY + TCHAT_DWORD_SIZE != wLENGTH || wBODY < nPos )
		return CString(_T(""));

	CString strBODY = strMSG.Mid( TCHAT_DWORD_SIZE + wHEAD + nPos, wBODY - nPos);
	CString strDATA = strMSG.Mid( TCHAT_DWORD_SIZE, wHEAD);
	CString strHEAD(_T(""));

	if(bTrimCrLf)
		strBODY.TrimLeft(strTOK);

	wLENGTH = strBODY.GetLength();
	if( wBODY < wLENGTH )
		return CString(_T(""));

	int nFIND = strBODY.FindOneOf(strTOK);
	int nPOS = wBODY - wLENGTH;

	if( nFIND >= 0 )
		strBODY = strBODY.Left(nFIND + 1);
	else
		nFIND = strBODY.GetLength();
	nFIND++;

	while( wHEAD >= TCHAT_DWORD_SIZE )
	{
		sscanf( LPCSTR(strDATA.Left(TCHAT_DWORD_SIZE)), _T("%X"), &dwCODE);
		LPWORD pPOS = LPWORD(&dwCODE);

		if( wHEAD < TCHAT_DWORD_SIZE + pPOS[1] ||
			nPOS + nFIND <= pPOS[0] )
			break;

		if( nPos <= pPOS[0] && pPOS[0] < nPOS )
			pPOS[0] = nPOS;

		if( nPOS <= pPOS[0] )
		{
			CString strSIZE;

			pPOS[0] -= nPOS;
			strSIZE.Format( _T("%08X"), dwCODE);

			strHEAD += strSIZE + strDATA.Mid( TCHAT_DWORD_SIZE, pPOS[1]);
		}

		strDATA = strDATA.Mid(TCHAT_DWORD_SIZE + pPOS[1]);
		wHEAD = strDATA.GetLength();
	}

	if(bTrimCrLf)
		strBODY.TrimRight(strTOK);
	nPos = nPOS + nFIND;

	if( strHEAD.IsEmpty() && strBODY.IsEmpty() )
		return CString(_T(""));

	return BuildNetString( strHEAD, strBODY);
}
