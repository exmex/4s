// BindDesc.cpp: implementation of the CBindDesc class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBindDesc::CBindDesc()
{
	m_ptr = NULL;
}

CBindDesc::CBindDesc( SQLSMALLINT type, int size)
{
	m_ptr = NULL;
	m_type = type;
	m_size = size;
}

CBindDesc::~CBindDesc()
{
	Free();
}

void CBindDesc::MAlloc()
{
	MAlloc(m_size);
}

void CBindDesc::MAlloc(DWORD cb)
{
	Free();
	m_ptr = malloc(cb);
}

void CBindDesc::Free()
{
	if(m_ptr)	
		free(m_ptr);
	m_ptr = NULL;
}
