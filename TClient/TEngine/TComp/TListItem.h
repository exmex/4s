// TListItem.h: interface for the TListItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TLISTITEM_H__282554E2_1934_4B1B_853E_807ADD0AA0C3__INCLUDED_)
#define AFX_TLISTITEM_H__282554E2_1934_4B1B_853E_807ADD0AA0C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TComponent.h"

class TListItem  
{
public:
	CString m_strText;
	DWORD m_param;
	DWORD m_dwTextColor;
	BOOL m_blIsImageList;
	int m_nImageIndex;
	INT m_nHMargine;

	TComponent::TextSettingSet m_TextSettingSet;

public:
	TListItem();
	virtual ~TListItem();
};

typedef map< BYTE, TListItem* >		MAP_LISTITEM;
typedef vector< MAP_LISTITEM* >		VECTOR_MAP_LISTITEM;

#endif // !defined(AFX_TLISTITEM_H__282554E2_1934_4B1B_853E_807ADD0AA0C3__INCLUDED_)
