// TChatEdit.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TChatClient.h"
#include "TChatEdit.h"

#include "TChatClientDoc.h"
#include "TChatClientView.h"


const DWORD CTChatEdit::CHAT_MODE_CMD[] = 
{
	IDS_KEYWORD_CHAT_WHISPER,			// CHAT_WHISPER
	IDS_KEYWORD_CHAT_NORMAL,			// CHAT_NEAR
	IDS_KEYWORD_CHAT_NATION,			// CHAT_MAP
	IDS_KEYWORD_CHAT_WORLD,				// CHAT_WORLD
	IDS_KEYWORD_CHAT_PARTY,				// CHAT_PARTY
	IDS_KEYWORD_CHAT_GUILD,				// CHAT_GUILD
	IDS_KEYWORD_CHAT_FORCE,				// CHAT_FORCE
	IDS_KEYWORD_CHAT_INFO,				// CHAT_OPERATOR
};

// CTChatEdit

IMPLEMENT_DYNAMIC(CTChatEdit, CEdit)
CTChatEdit::CTChatEdit()
{
	m_pHost = NULL;
}

CTChatEdit::~CTChatEdit()
{
}


BEGIN_MESSAGE_MAP(CTChatEdit, CEdit)
	ON_WM_CHAR()
END_MESSAGE_MAP()



// CTChatEdit 메시지 처리기입니다.

void CTChatEdit::OnChar( UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch(nChar)
	{
	case VK_RETURN	:
		if(m_pHost)
		{
			CString strTMSG;

			GetWindowText(strTMSG);
			if(!strTMSG.IsEmpty())
			{
				CString strHeader;

				CString strSIZE;
				strSIZE.Format( _T("%04X%04X"), strHeader.GetLength(), strTMSG.GetLength());

				CString strCHAT = strSIZE + strHeader + strTMSG;

				m_pHost->Chat(strCHAT);
				SetWindowText(_T(""));

				return;
			}
		}

		break;

	case VK_SPACE	:
		if(CheckTCMD())
		{
			SetWindowText(_T(""));
			return;
		}

		break;
	}

	CEdit::OnChar( nChar, nRepCnt, nFlags);
}

BYTE CTChatEdit::CheckTCMD()
{
	CString strTMSG;

	GetWindowText(strTMSG);
	strTMSG.TrimLeft();

	if( m_pHost && strTMSG[0] == '/' )
	{
		CString strHEADER;
		int nPOS = 0;

		strHEADER = strTMSG.Tokenize( _T(" "), nPOS);
		for( int i=0; i<TCHAT_FLAG_COUNT; i++)
		{
			CString strTCMD;

			strTCMD.LoadString(CHAT_MODE_CMD[i]);
			if( strTCMD.Find(strHEADER) > -1 )
			{
				if( i == CHAT_WHISPER )
				{
					m_pHost->m_strTARGET = strTMSG.Tokenize( _T(" "), nPOS);
					m_pHost->m_strTARGET.TrimRight();
					m_pHost->m_strTARGET.TrimLeft();

					if(m_pHost->m_strTARGET.IsEmpty())
						return FALSE;
				}
				else
					m_pHost->m_strTARGET.Empty();

				m_pHost->m_bChatType = i;
				m_pHost->SetChatTarget();

				return TRUE;
			}
		}
	}

	return FALSE;
}
