// MediaDoc.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "MediaFrame.h"
#include "MediaDoc.h"
#include "MainFrm.h"


// CMediaDoc

IMPLEMENT_DYNCREATE(CMediaDoc, CTachyonDoc)

CMediaDoc::CMediaDoc()
{
	m_pItem = NULL;
	m_nIndex = 0;
}

BOOL CMediaDoc::OnNewDocument()
{
	if (!CTachyonDoc::OnNewDocument())
		return FALSE;
	return TRUE;
}

CMediaDoc::~CMediaDoc()
{
	if(m_pItem)
	{
		m_pItem->SetMainDocument(NULL);
		m_pItem = NULL;
	}
}


BEGIN_MESSAGE_MAP(CMediaDoc, CTachyonDoc)
END_MESSAGE_MAP()


// CMediaDoc 진단입니다.

#ifdef _DEBUG
void CMediaDoc::AssertValid() const
{
	CTachyonDoc::AssertValid();
}

void CMediaDoc::Dump(CDumpContext& dc) const
{
	CTachyonDoc::Dump(dc);
}
#endif //_DEBUG


// CMediaDoc serialization입니다.

void CMediaDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}


// CMediaDoc 명령입니다.

CPlayCtrl *CMediaDoc::GetPlayCtrl()
{
	CMediaFrame *pFrame = (CMediaFrame *) GetFrameWnd();
	return pFrame ? pFrame->m_wndMediaBar.GetPlayCtrl() : NULL;
}

CMediaDlg *CMediaDoc::GetMediaDlg()
{
	CMediaFrame *pFrame = (CMediaFrame *) GetFrameWnd();
	return pFrame ? (CMediaDlg *) pFrame->m_wndMediaBar.m_pContent : NULL;
}

void CMediaDoc::InitData( LPVOID pData)
{
	CTachyonDoc::InitData(pData);
	if(!pData)
		return;

	m_pItem = (CMediaItem *) pData;
	m_pItem->SetMainDocument(this);

	Update();
}

void CMediaDoc::Update()
{
	if(m_pItem)
	{
		CString strFormat = m_pItem->m_strName;
		CMediaDlg *pControl = GetMediaDlg();

		switch(m_pItem->m_bMediaType)
		{
		case MEDIATYPE_WAV	: strFormat.Format( "%s[WAV]", m_pItem->m_strName); break;
		case MEDIATYPE_MPA	: strFormat.Format( "%s[MPA]", m_pItem->m_strName); break;
		case MEDIATYPE_MP2	: strFormat.Format( "%s[MP2]", m_pItem->m_strName); break;
		case MEDIATYPE_MP3	: strFormat.Format( "%s[MP3]", m_pItem->m_strName); break;
		case MEDIATYPE_AU	: strFormat.Format( "%s[AU]", m_pItem->m_strName); break;
		case MEDIATYPE_AIF	: strFormat.Format( "%s[AIF]", m_pItem->m_strName); break;
		case MEDIATYPE_AIFF	: strFormat.Format( "%s[AIFF]", m_pItem->m_strName); break;
		case MEDIATYPE_SND	: strFormat.Format( "%s[SND]", m_pItem->m_strName); break;
		case MEDIATYPE_MID	: strFormat.Format( "%s[MID]", m_pItem->m_strName); break;
		case MEDIATYPE_MIDI	: strFormat.Format( "%s[MIDI]", m_pItem->m_strName); break;
		case MEDIATYPE_RMI	: strFormat.Format( "%s[RMI]", m_pItem->m_strName); break;
		case MEDIATYPE_AVI	: strFormat.Format( "%s[AVI]", m_pItem->m_strName); break;
		case MEDIATYPE_QT	: strFormat.Format( "%s[QT]", m_pItem->m_strName); break;
		case MEDIATYPE_MOV	: strFormat.Format( "%s[MOV]", m_pItem->m_strName); break;
		case MEDIATYPE_MPG	: strFormat.Format( "%s[MPG]", m_pItem->m_strName); break;
		case MEDIATYPE_MPEG	: strFormat.Format( "%s[MPEG]", m_pItem->m_strName); break;
		case MEDIATYPE_M1V	: strFormat.Format( "%s[M1V]", m_pItem->m_strName); break;
		case MEDIATYPE_DAT	: strFormat.Format( "%s[DAT]", m_pItem->m_strName); break;
		}

		SetTitle(strFormat);
		m_pItem->Regulate();

		if(pControl)
			pControl->SetItem(m_pItem);
	}

	UpdateAllViews(NULL);
}
