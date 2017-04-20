// BindDesc.h: interface for the CBindDesc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BINDDESC_H__AF6491AE_835A_4748_B26A_1527798F8ADE__INCLUDED_)
#define AFX_BINDDESC_H__AF6491AE_835A_4748_B26A_1527798F8ADE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CBindDesc
{
public:
	SQLSMALLINT m_type;
	int			m_size;

	LPVOID		m_ptr;

public:
	CBindDesc();
	CBindDesc( SQLSMALLINT type, int size);
	virtual ~CBindDesc();

	void Free();
	void MAlloc();
	void MAlloc( DWORD cb);
};

#endif // !defined(AFX_BINDDESC_H__AF6491AE_835A_4748_B26A_1527798F8ADE__INCLUDED_)
