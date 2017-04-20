//////////////////////////////////////////////////////////////////////
//	File Name	:	XPtrList.h
//
//	Copyright	:	(c) Zemi Interactive, Inc 2002~2008
//
//////////////////////////////////////////////////////////////////////
#pragma once


class CXPtrList : public CPtrList  
{
public:
	CXPtrList();
	virtual ~CXPtrList();

private:
	
	CRITICAL_SECTION	m_cs;

public:

	DWORD		GetCount();
	BOOL		IsEmpty();

	POSITION	FindIndex( int nIndex );
	POSITION	Find(void* pObject);

	int			CopyTmpList( void* pList[]);

	void		RemovePtr(void* pObject);
	void		RemoveAt( POSITION pos);

	void		RemoveAll();
	void		AddTail(void* pObject);
	void*		RemoveHead();
};
