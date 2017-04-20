#include "Stdafx.h"
#include "TNumberEdit.h"
#include "TClientGame.h"
#include "TClientWnd.h"

// ====================================================================================================
CTNumberEdit::CTNumberEdit(TComponent *pParent, LP_FRAMEDESC pDesc, INT nValidCnt)
: TEdit(pParent,pDesc),
m_nValidCnt(nValidCnt),
m_bUseLimit(FALSE),
m_nMin(0),
m_nMax(0),
m_strDefault( "0" )
{
}
// ----------------------------------------------------------------------------------------------------
CTNumberEdit::~CTNumberEdit()
{
}
// ====================================================================================================
void CTNumberEdit::SetUseLimit( INT nMin, INT nMax )
{
	m_bUseLimit = TRUE;
	m_nMin = nMin;
	m_nMax = nMax;
}
// ====================================================================================================
void CTNumberEdit::SetDefault( CString strDefault )
{
	m_strDefault = strDefault;
}
// ====================================================================================================
DWORD CTNumberEdit::GetValue() const
{
	return ::atoi( (LPCSTR)m_strText );
}
// ====================================================================================================
void CTNumberEdit::ClearText()
{
	m_strText = m_strDefault;
	MoveCaretToFront();
}
// ----------------------------------------------------------------------------------------------------
void CTNumberEdit::SetText(const CString& txt)
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
void CTNumberEdit::OnChar(UINT nChar, int nRepCnt, UINT nFlags)
{
	if( nChar < '0' || nChar > '9' )
		return;

	if( !m_bUseLimit && m_nValidCnt <= m_strText.GetLength() )
		return ;

	if( GetValue() == 0 )
	{
		if( nChar == '0' )
			return;

		m_strText.Empty();
		m_nCaret = 0;
	}
	
	if( nChar == '0' && m_nCaret == 0 )
		return;

	CString strOldText = TEdit::m_strText;
	TEdit::OnChar(nChar, nRepCnt, nFlags);

	if( m_bUseLimit )
	{
		INT nValue = atoi( (LPCTSTR)m_strText );
		if( nValue < m_nMin ||
			nValue > m_nMax )
		{
			TEdit::SetText( strOldText );

			if( !m_strWarning.IsEmpty() )
			{
				CTClientWnd* pWND = CTClientGame::GetInstance()->GetMainWnd();
				pWND->MessageBoxOK(
					m_strWarning,
					TSTR_OK,
					GM_CLOSE_MSGBOX,
					0,
					TRUE,
					ID_SND_ERROR);
			}
		}
	}
}
// ----------------------------------------------------------------------------------------------------
void CTNumberEdit::OnKeyDown(UINT nChar, int nRepCnt, UINT nFlags)
{
	TEdit::OnKeyDown(nChar,nRepCnt,nFlags);
}
// ----------------------------------------------------------------------------------------------------
void CTNumberEdit::OnImeComposition(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if( CTNationOption::JAPAN )
	{
		CString strComposition;
		CString strResult;

		HandleImeComposition(
			hWnd,
			wParam,
			lParam,
			strComposition,
			strResult,
			NULL,
			NULL, 
			NULL,
			NULL );

		CString strORG;
		strORG.Empty();

		if( !strComposition.IsEmpty() )
		{
			strORG = strComposition;

			HIMC hImc = ImmGetContext( hWnd );
			ImmNotifyIME( hImc, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
			ImmReleaseContext( hWnd, hImc);
		}
		else if( !strResult.IsEmpty() )
		{
			strORG = strResult;
		}

		if( strORG.GetLength() == 1 )
		{
			CHAR nChar = strORG.GetAt(0);

			if( '0' <= nChar && nChar <= '9' )
			{
				CTNumberEdit::OnChar( nChar, 1, 0);
			}
		}
		else if( strORG.GetLength() >= 2 )
		{
			BYTE cLeadByte = strORG.GetAt( 0 );
			BYTE cTailByte = strORG.GetAt( 1 );
		    
			if( cLeadByte == 0x82 &&
				0x4F <= cTailByte && cTailByte <= 0x58 ) // 일본 전각 숫자 범위 : 0x824F ~ 0x8258
			{
				WCHAR pBuffer[64];
				MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strORG, strORG.GetLength(), pBuffer, 64);
				int n = _wtoi( pBuffer );

				if( 0 <= n && n <= 9 )
				{
					char str[3] = { '\n', };
					itoa( n, str, 10 );

					if( str[0] == '0' && m_nCaret == 0 )
						return;

					CTNumberEdit::OnChar(str[0], 1, 0);
				}
			}
		}
	}
}
// ====================================================================================================
void CTNumberEdit::CheckAndFixString(CString& txt)
{
	if( txt.IsEmpty() )
	{
		txt = m_strDefault;
		return;
	}

	INT nTrimCnt = txt.GetLength() - m_nValidCnt;
	if( nTrimCnt > 0 )
		txt.Delete(0, nTrimCnt);

	BOOL bDelZero = TRUE;
	INT i = 0;
	INT nDelCnt = 0;

	while( i < txt.GetLength() )
	{
		CHAR c = txt.GetAt(i);
		if( !(bDelZero && c == '0') && c >= '0' && c <= '9' )
		{
			++i;
			bDelZero = FALSE;

			continue;
		}

		++nDelCnt;
		txt.Delete(i);
	}

	m_nCaret -= nDelCnt;
	if( m_nCaret < 0 )
		m_nCaret = 0;

	if( txt.IsEmpty() )
		txt = m_strDefault;
}
// ====================================================================================================
void CTNumberEdit::OnLoseFocus( TComponent* pSetFocus )
{
	TEdit::OnLoseFocus( pSetFocus );
	CheckAndFixString(m_strText);
}

void CTNumberEdit::SetValidCount( INT nValidCnt )
{
	m_nValidCnt = nValidCnt;
}