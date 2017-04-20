// SqlDirect.h: interface for the CSqlDirect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SQLDIRECT_H__30225C78_FEAA_4F08_87A4_31095BF3C51B__INCLUDED_)
#define AFX_SQLDIRECT_H__30225C78_FEAA_4F08_87A4_31095BF3C51B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef vector<CBindDesc *>			VSQLBINDDESC, *LPVSQLBINDDESC;


class CSqlDirect : public CSqlBase
{
public:
	VSQLBINDDESC m_cols;

public:
	BOOL Textize( int nCol, CString& text);
	void AddBindDesc(CBindDesc *pDesc);
	CSqlDirect(CSqlDatabase *pdb, LPCTSTR lpszQuery);
	virtual ~CSqlDirect();

	virtual BOOL AllocStatement();
};

#endif // !defined(AFX_SQLDIRECT_H__30225C78_FEAA_4F08_87A4_31095BF3C51B__INCLUDED_)
