#include "StdAfx.h"
#include "TIntegrityCheckSum.h"
#include "TCmdManager.h"
#include "limits.h"


#pragma message(" Automatically linking with version.lib")
#pragma message(" (Version info support)")
#pragma comment( lib, "version.lib")


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CTCmdManager::CTCmdManagerAutoPtr g_CmdManager;

volatile DWORD CTCmdManager::CTCmdManagerAutoPtr::m_dwVersion = 0;
BOOL CTCmdManager::m_bDisableCmdIfNoHandler = TRUE;

#define __MAX_TOTAL_TICK_COUNT					(UINT_MAX - 1)
#define __DECREACE_TICK_COUNT_BY_REF(_REF_)		(_REF_) /= 2


void CTCmdManager::cmd::AnalyzeGlobalUsageOverflow()
{
	if( m_pProfile->m_nTotalTickCount < __MAX_TOTAL_TICK_COUNT )
		return;

	__DECREACE_TICK_COUNT_BY_REF(m_pProfile->m_nTotalTickCount);
	POSITION pos = m_pProfile->m_cmds.GetStartPosition();

	while(pos)
	{
		cmd *p_cmd = NULL;
		UINT nCmdID;

		m_pProfile->m_cmds.GetNextAssoc( pos, nCmdID, p_cmd);
		__DECREACE_TICK_COUNT_BY_REF(p_cmd->m_nUsageTickCount);
	}
}

CTCmdManager::CTCmdManagerAutoPtr::CTCmdManagerAutoPtr()
{
	m_pCmdMenager = new CTCmdManager();
}

CTCmdManager::CTCmdManagerAutoPtr::~CTCmdManagerAutoPtr()
{
	if(m_pCmdMenager)
	{
		delete m_pCmdMenager;
		m_pCmdMenager = NULL;
	}
}

CTCmdManager *CTCmdManager::CTCmdManagerAutoPtr::operator->()
{
	return m_pCmdMenager;
}

CTCmdManager::CTCmdManager()
{
	ProfileSetup(__TMFC_DEF_PROFILE_NAME);
}

CTCmdManager::~CTCmdManager()
{
	_RemoveAllProfilesImpl();
}

BOOL CTCmdManager::ProfileSetup( LPCTSTR strProfileName, HWND hProfileWnd)
{
	if(!strProfileName)
		strProfileName = __TMFC_DEF_PROFILE_NAME;

	cmd_profile *p_profile = NULL;
	m_cs.Lock();

	BOOL bExist = m_profiles.Lookup( strProfileName, (void *&) p_profile);
	if(!bExist)
	{
		p_profile = new cmd_profile(strProfileName);
		m_profiles.SetAt( strProfileName, p_profile);
	}
	m_cs.Unlock();

	if(hProfileWnd)
		ProfileWndAdd( strProfileName, hProfileWnd);

	return TRUE;
}

BOOL CTCmdManager::ProfileWndAdd( LPCTSTR strProfileName, HWND hProfileWnd)
{
	if(!hProfileWnd)
		return FALSE;

	cmd_profile *p_profile = ProfileGetPtr(strProfileName);
	if(!p_profile)
		return FALSE;

	m_cs.Lock();
	cmd_profile *p_profile2 = NULL;
	BOOL bExist = m_profile_wnds.Lookup( hProfileWnd, p_profile2);

	if(!bExist)
		m_profile_wnds.SetAt( hProfileWnd, p_profile);

	m_cs.Unlock();
	return TRUE;
}

BOOL CTCmdManager::ProfileWndRemove( HWND hProfileWnd)
{
	if(!hProfileWnd)
		return FALSE;
	m_cs.Lock();

	cmd_profile *p_profile = NULL;
	BOOL bExists = m_profile_wnds.Lookup( hProfileWnd, p_profile);

	if(bExists)
		m_profile_wnds.RemoveKey(hProfileWnd);

	m_cs.Unlock();
	return TRUE;
}

CTCmdManager::cmd_profile *CTCmdManager::ProfileGetPtr( LPCTSTR strProfileName)
{
	if(!strProfileName)
		strProfileName = __TMFC_DEF_PROFILE_NAME;

	m_cs.Lock();
	cmd_profile *p_profile = NULL;
	BOOL bExists = m_profiles.Lookup( strProfileName, (void *&) p_profile);

	if(!bExists)
		p_profile = NULL;

	m_cs.Unlock();
	return p_profile;
}

LPCTSTR CTCmdManager::ProfileNameFromWnd( HWND hWnd)
{
	if(!hWnd)
		return NULL;

	LPCTSTR strProfileName = NULL;
	m_cs.Lock();

	while( !strProfileName && hWnd )
	{
		cmd_profile *p_profile = NULL;

		if(m_profile_wnds.Lookup( hWnd, p_profile))
		{
			strProfileName = p_profile->m_strName;
			break;
		}

		hWnd = GetParent(hWnd);
	}

	m_cs.Unlock();
	return strProfileName;
}

BOOL CTCmdManager::CmdSetup( LPCTSTR strProfileName,
							 const CTCmdManager::cmd &_cmd,
							 BOOL bReplaceOld,
							 BOOL *pWasAddedNew)
{
	if(pWasAddedNew)
		*pWasAddedNew = FALSE;

	cmd_profile *p_profile = ProfileGetPtr(strProfileName);
	if(!p_profile)
		return FALSE;

	m_cs.Lock();
	BOOL bRetVal = p_profile->CmdSetup( _cmd, bReplaceOld, pWasAddedNew);
	m_cs.Unlock();

	return bRetVal;
}

BOOL CTCmdManager::CmdRemove( LPCTSTR strProfileName, UINT nCmdID, BOOL *pWasRemoved)
{
	if(pWasRemoved)
		*pWasRemoved = FALSE;

	cmd_profile *p_profile = ProfileGetPtr(strProfileName);
	if(!p_profile)
		return FALSE;

	m_cs.Lock();
	BOOL bRetVal = p_profile->CmdRemove( nCmdID, pWasRemoved);
	m_cs.Unlock();

	return bRetVal;
}

CTCmdManager::cmd *CTCmdManager::CmdAllocPtr( LPCTSTR strProfileName, UINT nCmdID)
{
	cmd_profile *p_profile = ProfileGetPtr(strProfileName);

	if(!p_profile)
		return NULL;

	m_cs.Lock();
	CTCmdManager::cmd *p_cmd = p_profile->CmdAllocPtr(nCmdID);
	m_cs.Unlock();

	return p_cmd;
}

CTCmdManager::cmd *CTCmdManager::CmdGetPtr( LPCTSTR strProfileName, UINT nCmdID)
{
	cmd_profile *p_profile = ProfileGetPtr(strProfileName);

	if(!p_profile)
		return NULL;

	m_cs.Lock();
	CTCmdManager::cmd *p_cmd = p_profile->CmdGetPtr(nCmdID);
	m_cs.Unlock();

	return p_cmd;
}

BOOL CTCmdManager::CmdSetIcon( LPCTSTR strProfileName,
							   UINT nCmdID,
							   HICON hIcon,
							   BOOL bCopyIcon)
{
	cmd_profile *p_profile = ProfileGetPtr(strProfileName);

	if(!p_profile)
		return FALSE;

	m_cs.Lock();
	BOOL bRetVal = p_profile->CmdSetIcon( nCmdID, hIcon, bCopyIcon);
	m_cs.Unlock();

	return bRetVal;
}

CTCmdManager::icon *CTCmdManager::CmdGetIconPtr( LPCTSTR strProfileName, UINT nCmdID)
{
	cmd_profile *p_profile = ProfileGetPtr(strProfileName);

	if(!p_profile)
		return NULL;

	m_cs.Lock();
	CTCmdManager::icon *p_icon = p_profile->CmdGetIconPtr(nCmdID);
	m_cs.Unlock();

	return p_icon;
}

BOOL CTCmdManager::CmdIsRegistered( LPCTSTR strProfileName, UINT nCmdID)
{
	return (BOOL) CmdGetPtr( strProfileName, nCmdID);
}

BOOL CTCmdManager::UpdateFromMenu( LPCTSTR strProfileName,
								   HMENU hMenu,
								   BOOL bReplaceOld,
								   BOOL bRecursive,
								   BOOL bLoadTips)
{
	cmd_profile *p_profile = ProfileGetPtr(strProfileName);

	if(!p_profile)
		return FALSE;

	m_cs.Lock();
	BOOL bRetVal = p_profile->UpdateFromMenu(
		hMenu,
		bReplaceOld,
		bRecursive,
		bLoadTips);
	m_cs.Unlock();

	return bRetVal;
}

BOOL CTCmdManager::cmd_profile::UpdateFromMenu( HMENU hMenu,
											    BOOL bReplaceOld,
												BOOL bRecursive,
												BOOL bLoadTips)
{
	if(!hMenu)
		return FALSE;

	if(!::IsMenu(hMenu))
		return FALSE;

	int nCount = ::GetMenuItemCount(hMenu);
	for( int i=0; i<nCount; i++)
	{
		CString strMenuText;
		MENUITEMINFO mii;

		::memset( &mii, 0, sizeof(MENUITEMINFO));
		mii.cbSize = sizeof(MENUITEMINFO);
		mii.fMask = MIIM_CHECKMARKS|
			MIIM_DATA|
			MIIM_ID|
			MIIM_STATE|
			MIIM_SUBMENU|
			MIIM_TYPE;

		mii.cch = __MAX_UI_ITEM_TEXT;
		mii.dwTypeData = strMenuText.GetBuffer(__MAX_UI_ITEM_TEXT);

		if(!mii.dwTypeData)
			continue;

		if(!::GetMenuItemInfo( hMenu, i, TRUE, &mii))
		{
			strMenuText.ReleaseBuffer();
			continue;
		}

		strMenuText.ReleaseBuffer();
		if(mii.fType&MFT_SEPARATOR)
			continue;

		if(mii.hSubMenu)
		{
			if(!bRecursive)
				continue;

			UpdateFromMenu(
				mii.hSubMenu,
				bReplaceOld,
				TRUE);

			continue;
		}

		if(!IsCommand(mii.wID))
			continue;

		cmd _cmd;
		_cmd.m_nCmdID = mii.wID;

		strMenuText.Replace( _T("\n"), _T(""));
		strMenuText.Replace( _T("\r"), _T(""));
		strMenuText.TrimLeft();
		strMenuText.TrimRight();

		if(!strMenuText.IsEmpty())
		{
			int nSep = strMenuText.ReverseFind(_T('\t'));
			if( nSep >= 0 )
			{
				int nLen = strMenuText.GetLength();

				_cmd.m_strAccelText = strMenuText.Right(nLen - nSep);
				_cmd.m_strAccelText.TrimLeft();
				_cmd.m_strAccelText.TrimRight();
				_cmd.m_strMenuText = strMenuText.Left(nSep);
				_cmd.m_strMenuText.TrimLeft();
				_cmd.m_strMenuText.TrimRight();
			}
			else
				_cmd.m_strMenuText = strMenuText;
		}

		if(bLoadTips)
			_cmd.TipsLoad();

		CmdSetup(_cmd,bReplaceOld);
	}

	return TRUE;
}

BOOL CTCmdManager::UpdateFromMenu( LPCTSTR strProfileName,
								   UINT nResourceID,
								   BOOL bReplaceOld,
								   BOOL bLoadTips)
{
	cmd_profile *p_profile = ProfileGetPtr(strProfileName);

	if(!p_profile)
		return FALSE;

	m_cs.Lock();
	BOOL bRetVal = p_profile->UpdateFromMenu(
		nResourceID,
		bReplaceOld,
		bLoadTips);
	m_cs.Unlock();

	return bRetVal;
}

BOOL CTCmdManager::cmd_profile::UpdateFromMenu( UINT nResourceID,
											    BOOL bReplaceOld,
												BOOL bLoadTips)
{
	CMenu menu;

	if(!menu.LoadMenu(nResourceID))
		return FALSE;

	return UpdateFromMenu(
		menu.GetSafeHmenu(),
		bReplaceOld,
		TRUE,
		bLoadTips);
}

BOOL CTCmdManager::UpdateFromToolBar( LPCTSTR strProfileName,
									  CToolBar &bar,
									  BOOL bReplaceOld,
									  BOOL bLoadTips)
{
	cmd_profile *p_profile = ProfileGetPtr(strProfileName);

	if(!p_profile)
		return FALSE;

	m_cs.Lock();
	BOOL bRetVal = p_profile->UpdateFromToolBar(
		bar,
		bReplaceOld,
		bLoadTips);
	m_cs.Unlock();

	return bRetVal;
}

BOOL CTCmdManager::cmd_profile::UpdateFromToolBar( CToolBar &bar,
												   BOOL bReplaceOld,
												   BOOL bLoadTips)
{
	if(!::IsWindow(bar.GetSafeHwnd()))
		return FALSE;

	CImageList *pImageList = bar.GetToolBarCtrl().GetImageList();
	if( !pImageList || !pImageList->GetSafeHandle() )
		return FALSE;

	int nCount = bar.GetCount();
	for( int i=0; i<nCount; i++)
	{
		UINT nCmdID = bar.GetItemID(i);
		if( nCmdID == ID_SEPARATOR )
			continue;

		TBBUTTONINFO tbi;
		::memset( &tbi, 0, sizeof(TBBUTTONINFO));
		tbi.cbSize = sizeof(TBBUTTONINFO);
		tbi.idCommand = nCmdID;
		tbi.dwMask = TBIF_IMAGE|TBIF_TEXT;
		tbi.cchText = __MAX_UI_ITEM_TEXT;
		CString strToolbarText;
		tbi.pszText = strToolbarText.GetBuffer(__MAX_UI_ITEM_TEXT);

		if(!tbi.pszText)
			continue;

		if(!bar.GetToolBarCtrl().GetButtonInfo( nCmdID, &tbi))
		{
			UINT nDummyStyle;
			UINT nDummyID;

			bar.GetButtonInfo(
				i, nDummyID,
				nDummyStyle,
				tbi.iImage);
		}
		strToolbarText.ReleaseBuffer();

		cmd _cmd;
		_cmd.m_nCmdID = nCmdID;
		_cmd.m_strToolbarText = strToolbarText;

		if(bLoadTips)
			_cmd.TipsLoad();

		if(!CmdSetup( _cmd, bReplaceOld))
			continue;

		if( tbi.iImage < 0 )
			continue;

		cmd *p_cmd = CmdGetPtr(nCmdID);
		if( p_cmd->m_nIconIdx >= 0 )
			continue;

		HICON hIcon = pImageList->ExtractIcon(tbi.iImage);
		if(!hIcon)
			continue;

		icon *p_icon = new icon;
		p_icon->Attach(hIcon);
		int nIconIndex = m_icons.Add(p_icon);
		p_cmd->m_nIconIdx = nIconIndex;
	}

	return TRUE;
}

BOOL CTCmdManager::UpdateFromToolBar( LPCTSTR strProfileName,
									  UINT nResourceID,
									  BOOL bReplaceOld,
									  BOOL bLoadTips)
{
	cmd_profile *p_profile = ProfileGetPtr(strProfileName);

	if(!p_profile)
		return FALSE;

	m_cs.Lock();
	BOOL bRetVal = p_profile->UpdateFromToolBar(
		nResourceID,
		bReplaceOld,
		bLoadTips);
	m_cs.Unlock();

	return bRetVal;
}

BOOL CTCmdManager::cmd_profile::UpdateFromToolBar( UINT nResourceID,
												   BOOL bReplaceOld,
												   BOOL bLoadTips)
{
	CToolBar bar;

	if(!bar.Create(::AfxGetMainWnd()))
		return FALSE;

	if(!bar.LoadToolBar(nResourceID))
		return FALSE;

	return UpdateFromToolBar(
		bar,
		bReplaceOld,
		bLoadTips);
}

BOOL CTCmdManager::SetBasicCommands( LPCTSTR strProfileName,
									 UINT *pCommands,
									 BOOL bOn)
{
	cmd_profile *p_profile = ProfileGetPtr(strProfileName);

	if(!p_profile)
		return FALSE;

	m_cs.Lock();
	BOOL bRetVal = p_profile->SetBasicCommands( pCommands, bOn);
	m_cs.Unlock();

	return bRetVal;
}

BOOL CTCmdManager::cmd_profile::SetBasicCommands( UINT *pCommands, BOOL bOn)
{
	if(!pCommands)
		return FALSE;

	BOOL bRetVal = TRUE;
	for(; *pCommands != 0; pCommands++)
	{
		if(IsSystemCommand(*pCommands))
		{
			bRetVal = FALSE;
			continue;
		}

		cmd *p_cmd = CmdGetPtr(*pCommands);
		if(!p_cmd)
		{
			bRetVal = FALSE;
			continue;
		}

		p_cmd->StateSetBasic(bOn);
	}

	return bRetVal;
}
