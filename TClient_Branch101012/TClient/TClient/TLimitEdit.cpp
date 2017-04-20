#include "Stdafx.h"
#include "TLimitEdit.h"
#include "TClientWnd.h"

// ====================================================================================================
CTLimitEdit::CTLimitEdit(TComponent *pParent, LP_FRAMEDESC pDesc, INT nValidByteSize)
: TEdit(pParent,pDesc),
m_nValidByteSize(nValidByteSize)
{
}
// ----------------------------------------------------------------------------------------------------
CTLimitEdit::~CTLimitEdit()
{
}
// ====================================================================================================
void CTLimitEdit::ClearText()
{
	m_strText.Empty();
	MoveCaretToFront();
}
// ----------------------------------------------------------------------------------------------------
void CTLimitEdit::SetText(const CString& txt)
{
	ClearText();

	if( !txt.IsEmpty() )
	{
		m_strText = txt;
		CheckAndFixString(m_strText);

		MoveCaretToBack();
	}
}
// ====================================================================================================
void CTLimitEdit::OnChar(UINT nChar, int nRepCnt, UINT nFlags)
{
	TEdit::OnChar(nChar, nRepCnt, nFlags);
	NotifyError();
}
// ----------------------------------------------------------------------------------------------------
void CTLimitEdit::OnKeyDown(UINT nChar, int nRepCnt, UINT nFlags)
{
	TEdit::OnKeyDown(nChar,nRepCnt,nFlags);
	NotifyError();
}
// ----------------------------------------------------------------------------------------------------
void CTLimitEdit::OnImeComposition(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	TEdit::OnImeComposition( hWnd, wParam, lParam );
	NotifyError();
}
// ====================================================================================================
void CTLimitEdit::CheckAndFixString(CString& strTEXT)
{
	CString strRESULT;

	INT nTotalByteSize = 0;
	for( int i=0; i < strTEXT.GetLength() ; ++i )
	{
		BYTE bCODE = strTEXT.GetAt(i);

		if(MBCS_LEAD_TEST(bCODE))
		{
			if( m_nValidByteSize - nTotalByteSize >= 2 )
			{
				strRESULT.AppendChar( bCODE ); //TCHAR_MBCS_LEAD
				strRESULT.AppendChar( strTEXT.GetAt(i+1) ); //TCHAR_MBCS_TAIL
				nTotalByteSize += 2;
			}
			else
				break;

			++i;
		}
		else
		{
			if( m_nValidByteSize - nTotalByteSize >= 1 )
			{
				strRESULT.AppendChar( bCODE ); //TCHAR_MBCS_SINGLE
				++nTotalByteSize;
			}
			else
				break;
		}
	}

	strTEXT = strRESULT;
}
// ====================================================================================================
void CTLimitEdit::OnLoseFocus( TComponent* pSetFocus )
{
	TEdit::OnLoseFocus( pSetFocus );
	CheckAndFixString(m_strText);
}

void CTLimitEdit::SetValidByteSize( INT nValidByteSize )
{
	m_nValidByteSize = nValidByteSize;
}

void CTLimitEdit::NotifyError()
{
	CTClientWnd* pWND = CTClientGame::GetInstance()->GetMainWnd();
	if( m_nValidByteSize - (m_strText.GetLength() + m_strComposition.GetLength()) < 0 )
	{
		pWND->m_vCOMMAND.push_back( GM_CHAR_NAMING_LONG );
	}
	else
	{
		if( CTClientWnd::CheckCharID(m_strText) < 0 )
		{
			pWND->m_vCOMMAND.push_back( GM_CHAR_NAMING_WRONG );	
			return ;
		}

		pWND->m_vCOMMAND.push_back( GM_CHAR_NAMING_OK );
	}
}