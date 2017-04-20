#if (!defined __TCMDMANAGER_H)
#define __TCMDMANAGER_H

#if (!defined __TMFC_DEF_H)
	#include <TMfcDef.h>
#endif // __TMFC_DEF_H

#if (!defined __AFXMT_H__)
	#include <AfxMt.h>
#endif

#if (!defined __AFXTEMPL_H__)
	#include <AfxTempl.h>
#endif


#define __MAX_UI_ITEM_TEXT					1024

#define __TMFC_DEF_RARELY_USED_PERCENT		10
#define __ID_MDIWNDLIST_COUNT				9
#define __ID_MDIWNDLIST_FIRST				AFX_IDM_FIRST_MDICHILD
#define __ID_MDIWNDLIST_LAST				(__ID_MDIWNDLIST_FIRST + __ID_MDIWNDLIST_COUNT - 1)
#define __ID_MDIWND_DLGWINDOWS				(__ID_MDIWNDLIST_LAST + 1)
#define __TMFC_DEF_PROFILE_NAME				_T("default")

class __TUI_API CTCmdManager
{
public:
	static inline BOOL IsCommand( UINT nCmd)
	{
		if( nCmd == 0 || (int) nCmd <= (int) -1 )
			return FALSE;

		return TRUE;
	};

	static inline BOOL IsSystemCommand( UINT nCmd)
	{
		BOOL bSystemCommand = nCmd >= 0xF000 && nCmd < 0xF1F0 ? TRUE : FALSE;
		return bSystemCommand;
	};

	static inline BOOL IsAfxOleCommand( UINT nCmd)
	{
		if( ID_OLE_VERB_FIRST <= nCmd &&
			nCmd <= ID_OLE_VERB_LAST )
			return TRUE;

		return FALSE;
	};

	static inline BOOL IsAfxMdiWindowsCommand( UINT nCmd)
	{
		if(( __ID_MDIWNDLIST_FIRST <= nCmd &&
			nCmd <= __ID_MDIWNDLIST_LAST ) ||
			nCmd == __ID_MDIWND_DLGWINDOWS )
			return TRUE;

		return FALSE;
	};

	static inline BOOL IsAfxFileMruCommand( UINT nCmd)
	{
		if( ID_FILE_MRU_FIRST <= nCmd &&
			nCmd <= ID_FILE_MRU_LAST )
			return TRUE;

		return FALSE;
	};

	static inline BOOL IsBasicCommand( UINT nCmd)
	{
		if( IsAfxMdiWindowsCommand(nCmd) ||
			IsAfxFileMruCommand(nCmd) ||
			IsSystemCommand(nCmd) ||
			IsAfxOleCommand(nCmd) )
			return TRUE;

		return FALSE;
	}

	static inline BOOL IsRarelyCommand( UINT nCmd)
	{
		return FALSE;
	};

	static inline BOOL IsCommandNeedsSpecUpdate( UINT nCmd)
	{
		if( IsSystemCommand(nCmd) ||
			IsAfxMdiWindowsCommand(nCmd))
			return TRUE;

		return FALSE;
	};

	class cmd_profile;
	class __TUI_API icon
	{
		void InitClassInstance()
		{
			m_pProfile = NULL;
			m_hIcon = NULL;
			m_szIconOrginal.cx = 0;
			m_szIconOrginal.cy = 0;
		};

		void AssignFromHICON(
			const HICON &hIcon,
			BOOL bCreateCopy)
		{
			DestroyIcon();

			if(!hIcon)
				return;

			if(bCreateCopy)
			{
				ICONINFO ii;

				::memset( &ii, 0, sizeof(ICONINFO));
				::GetIconInfo( hIcon, &ii);

				m_szIconOrginal.cx = (BYTE) ii.xHotspot * 2;
				m_szIconOrginal.cy = (BYTE) ii.yHotspot * 2;
				m_hIcon = ::CreateIconIndirect(&ii);
			}
			else
			{
				m_hIcon = hIcon;
				RecalcIconOriginalSize();
			}
		};

		void AssignFromOther( const icon &other)
		{
			m_pProfile = other.m_pProfile;
			AssignFromHICON( other.m_hIcon, TRUE);
		};

		void RecalcIconOriginalSize()
		{
			m_szIconOrginal.cx = 0;
			m_szIconOrginal.cy = 0;

			if(!m_hIcon)
				return;

			ICONINFO ii;
			::memset( &ii, 0, sizeof(ICONINFO));
			::GetIconInfo( m_hIcon, &ii);

			m_szIconOrginal.cx = (BYTE) ii.xHotspot * 2;
			m_szIconOrginal.cy = (BYTE) ii.yHotspot * 2;
			::DeleteObject(ii.hbmMask);
			::DeleteObject(ii.hbmColor);
		};

		HICON m_hIcon;
		CSize m_szIconOrginal;
		cmd_profile* m_pProfile;

	public:
		BOOL IsEmpty() const
		{
			return m_hIcon == NULL;
		};

		void DestroyIcon()
		{
			if(m_hIcon)
			{
				::DestroyIcon(m_hIcon);
				m_hIcon = NULL;
			}

			m_szIconOrginal.cx = 0;
			m_szIconOrginal.cy = 0;
		};

		HICON Detach()
		{
			HICON hIcon = m_hIcon;
			m_hIcon = NULL;

			m_szIconOrginal.cx = 0;
			m_szIconOrginal.cy = 0;
			return hIcon;
		};

		void Attach( HICON hIcon)
		{
			DestroyIcon();
			AssignFromHICON( hIcon, FALSE);
		};

		icon()
		{
			InitClassInstance();
		};

		icon( const icon &other)
		{
			InitClassInstance();
			AssignFromOther(other);
		};

		icon( const HICON &hIcon,
			  BOOL bCreateCopy)
		{
			InitClassInstance();
			AssignFromHICON( hIcon, bCreateCopy);
		};

		~icon()
		{
			DestroyIcon();
		};

		icon& operator=( const icon& other)
		{
			AssignFromOther(other);
			return *this;
		};

		icon& operator=(const HICON& hIcon)
		{
			AssignFromHICON( hIcon, TRUE);
			return *this;
		};

		HICON GetIcon()
		{
			return m_hIcon;
		};

		const HICON GetIcon() const
		{
			return m_hIcon;
		};

		operator HICON()
		{
			return GetIcon();
		};

		operator HICON() const
		{
			return GetIcon();
		};

		CSize GetSize()
		{
			return m_szIconOrginal;
		};

		const CSize GetSize() const
		{
			return m_szIconOrginal;
		};

		operator CSize()
		{
			return GetSize();
		};

		operator CSize() const
		{
			return GetSize();
		};
	}; // class icon

	// command ui update class
	class cmd;
	class cmd_ui : public CCmdUI
	{
		cmd *m_pCmd;

	public:
		cmd_ui( cmd *pCmd, int nIndexMax = 0)
		{
			m_pCmd = pCmd;
			m_nIndexMax = nIndexMax;
			m_nID = pCmd->m_nCmdID;
			m_pOther = NULL;
		};

		// Operations to do in ON_UPDATE_COMMAND_UI
		virtual void Enable( BOOL bOn = TRUE)
		{
			m_pCmd->StateEnable(bOn);
		}

		virtual void SetCheck( int nCheck = 1)
		{
			if( nCheck == 0 || nCheck == 1 )
				m_pCmd->StateSetCheck( nCheck == 0 );
			else
				m_pCmd->StateSetIndeterminate(TRUE);
		};

		virtual void SetRadio( BOOL bOn = TRUE)
		{
			m_pCmd->StateSetRadio(bOn);
		};

		virtual void SetText( LPCTSTR lpszText)
		{
			m_pCmd->m_strMenuText = lpszText;
		};
	}; // class cmd_ui

	// command item class
	class __TUI_API cmd
	{
		void AssignFromOther( const cmd& other)
		{
			m_nUsageTickCount = other.m_nUsageTickCount;
			m_nStateFlags = other.m_nStateFlags;
			m_pProfile = other.m_pProfile;
			m_nIconIdx = other.m_nIconIdx;
			m_nCmdID = other.m_nCmdID;

			m_strToolbarText = other.m_strToolbarText;
			m_strTipStatus = other.m_strTipStatus;
			m_strAccelText = other.m_strAccelText;
			m_strMenuText = other.m_strMenuText;
			m_strTipTool = other.m_strTipTool;
		};

	public:
		enum e_state
		{
			STATE_ENABLED = 0x00000000,
			STATE_DISABLED = 0x00000001,
			STATE_CHECK = 0x00000002,
			STATE_RADIO = 0x00000004,
			STATE_INDETERMINATE = 0x00000008,
			STATE_BASICCMDPROP = 0x00000010,
			STATE_FORCE_RARELY = 0x00000020
		};

		cmd_profile* m_pProfile;
		int m_nIconIdx;

		BYTE m_nStateFlags;
		UINT m_nCmdID;

		CString m_strToolbarText;
		CString m_strTipStatus;
		CString m_strAccelText;
		CString m_strMenuText;
		CString m_strTipTool;

	protected:
		UINT m_nUsageTickCount;

	public:
		cmd( UINT nCmdID = IDC_STATIC) : m_pProfile(NULL),
			m_nCmdID(nCmdID),
			m_nStateFlags(STATE_ENABLED),
			m_nIconIdx(-1),
			m_strMenuText(_T("")),
			m_strToolbarText(_T("")),
			m_strTipTool(_T("")),
			m_strTipStatus(_T("")),
			m_strAccelText(_T("")),
			m_nUsageTickCount(0)
		{
		};

		cmd( const cmd &other) : m_pProfile(NULL),
			m_nCmdID((UINT) IDC_STATIC),
			m_nStateFlags(STATE_ENABLED),
			m_nIconIdx(-1),
			m_strMenuText(_T("")),
			m_strToolbarText(_T("")),
			m_strTipTool(_T("")),
			m_strTipStatus(_T("")),
			m_strAccelText(_T("")),
			m_nUsageTickCount(0)
		{
			AssignFromOther(other);
		};

		~cmd()
		{
		};

		cmd &operator=(const cmd &other)
		{
			AssignFromOther(other);
			return *this;
		};

		void TipsClean()
		{
			m_strTipTool = _T("");
			m_strTipStatus = _T("");
		};

		BOOL TipsLoad()
		{
			CString strText;

			TipsClean();
			if(!CTCmdManager::IsCommand(m_nCmdID))
				return FALSE;

			if(!strText.LoadString(m_nCmdID))
				return FALSE;

			strText.Replace( _T("\t"), _T(" "));
			strText.Replace( _T("\r"), _T(""));
			strText.TrimLeft();
			strText.TrimRight();

			if(strText.IsEmpty())
				return FALSE;

			int nSep = strText.ReverseFind('\n');
			if( nSep < 0 )
			{
				m_strTipTool = strText;
				return TRUE;
			}

			int nLen = strText.GetLength();
			m_strTipTool = strText.Right(nLen - nSep);
			m_strTipTool.TrimLeft();
			m_strTipTool.TrimRight();
			m_strTipStatus = strText.Left(nSep);
			m_strTipStatus.TrimLeft();
			m_strTipStatus.TrimRight();

			return TRUE;
		};

		void UpdateMoreExact( const cmd &other)
		{
			if( m_nIconIdx < 0 )
				m_nIconIdx = other.m_nIconIdx;

			if(!other.m_strMenuText.IsEmpty())
				m_strMenuText = other.m_strMenuText;

			if(!other.m_strTipTool.IsEmpty())
				m_strTipTool = other.m_strTipTool;

			if(!other.m_strTipStatus.IsEmpty())
				m_strTipStatus = other.m_strTipStatus;

			if(!other.m_strAccelText.IsEmpty())
				m_strAccelText = other.m_strAccelText;
		};

		void StateSetBasic( BOOL bOn = TRUE)
		{
			if(bOn)
				m_nStateFlags |= STATE_BASICCMDPROP;
			else
				m_nStateFlags &= ~STATE_BASICCMDPROP;
		};

		BOOL StateIsBasic()
		{
			if(CTCmdManager::IsBasicCommand(m_nCmdID))
				return TRUE;

			if(CTCmdManager::IsRarelyCommand(m_nCmdID))
				return FALSE;

			BOOL bBasic = m_nStateFlags&STATE_BASICCMDPROP;
			return bBasic;
		};

		BOOL StateIsRarelyUsed()
		{
			if(StateIsBasic())
				return FALSE;

			UINT nUsagePercent = GetUsagePercent();
			return nUsagePercent < m_pProfile->m_nRarelyPercent;
		}

		void StateForceRarely( BOOL bOn = TRUE)
		{
			if(bOn)
				m_nStateFlags |= STATE_FORCE_RARELY;
			else
				m_nStateFlags &= ~STATE_FORCE_RARELY;
		};

		BOOL StateIsForceRarely()
		{
			if(CTCmdManager::IsRarelyCommand(m_nCmdID))
				return TRUE;

			if(CTCmdManager::IsBasicCommand(m_nCmdID))
				return FALSE;

			BOOL bForceRarely = m_nStateFlags&STATE_FORCE_RARELY;
			return bForceRarely;
		};

		void StateEnable( BOOL bOn = TRUE)
		{
			if(bOn)
				m_nStateFlags &= ~STATE_DISABLED;
			else
				m_nStateFlags |= STATE_DISABLED;
		};

		BOOL StateIsEnable()
		{
			BOOL bEnable = !(m_nStateFlags&STATE_DISABLED);
			return bEnable;
		};

		void StateSetCheck( BOOL bOn = TRUE)
		{
			m_nStateFlags &= ~(STATE_RADIO|STATE_CHECK|STATE_INDETERMINATE);

			if(bOn)
				m_nStateFlags |= STATE_CHECK;
		};

		BOOL StateGetCheck()
		{
			BOOL bCheck = m_nStateFlags&STATE_CHECK;
			return bCheck;
		};

		void StateSetRadio( BOOL bOn = TRUE)
		{
			m_nStateFlags &= ~(STATE_RADIO|STATE_CHECK|STATE_INDETERMINATE);

			if(bOn)
				m_nStateFlags |= STATE_RADIO;
		};

		BOOL StateGetRadio()
		{
			BOOL bRadio = m_nStateFlags&STATE_RADIO;
			return bRadio;
		};

		void StateSetIndeterminate( BOOL bOn = TRUE)
		{
			m_nStateFlags &= ~(STATE_RADIO|STATE_CHECK|STATE_INDETERMINATE);

			if(bOn)
				m_nStateFlags |= STATE_INDETERMINATE;
		};

		BOOL StateGetIndeterminate()
		{
			BOOL bIndeterminate = m_nStateFlags&STATE_INDETERMINATE;
			return bIndeterminate;
		};

		BOOL DoUpdateCmdUI( CCmdTarget *pCmdTarget, UINT nIndex = 0)
		{
			cmd_ui ui(this);

			if(IsCommandNeedsSpecUpdate(m_nCmdID))
			{
				ui.Enable(TRUE);
				return TRUE;
			}
			else
			{
				ui.m_nIndex = nIndex;

				BOOL bRetVal = ui.DoUpdate(
					pCmdTarget,
					CTCmdManager::m_bDisableCmdIfNoHandler);

				return bRetVal;
			}
		};

		BOOL Deliver( HWND hWndCmdTarget, BOOL bSend = FALSE)
		{
			if(!hWndCmdTarget)
				return FALSE;

			IncrementUsageCount();
			UINT nMsg = WM_COMMAND;
			LPARAM lParam = 0;

			if(CTCmdManager::IsSystemCommand(m_nCmdID))
			{
				nMsg = WM_SYSCOMMAND;
				POINT point = { 0, 0};

				::GetCursorPos(&point);
				lParam = MAKELONG(
					point.x,
					point.y);
			}

			if(bSend)
			{
				if(!::SendMessage( hWndCmdTarget, nMsg, m_nCmdID, lParam))
					return FALSE;
			}
			else if(!::PostMessage( hWndCmdTarget, nMsg, m_nCmdID, lParam))
				return FALSE;

			return TRUE;
		};

		BOOL Deliver( CWnd *pWndCmdTarget, BOOL bSend = FALSE)
		{
			return Deliver(
				pWndCmdTarget->GetSafeHwnd(),
				bSend);
		};

		BOOL Deliver( CControlBar *pWndCmdSource, BOOL bSend = FALSE)
		{
			if( !pWndCmdSource || !pWndCmdSource->GetSafeHwnd() )
				return FALSE;

			CWnd *pOwner = pWndCmdSource->GetOwner();
			if(!pOwner)
				return FALSE;

			return Deliver(
				pOwner->GetSafeHwnd(),
				bSend);
		};

		UINT GetUsageTickCount() const
		{
			return m_nUsageTickCount;
		};

		UINT GetUsagePercent() const
		{
			if( m_pProfile->m_nTotalTickCount == 0 )
				return 0;

			return 100 * m_nUsageTickCount / m_pProfile->m_nTotalTickCount;
		};

		void IncrementUsageCount()
		{
			if( StateIsBasic() || StateIsForceRarely() )
				return;

			m_pProfile->m_nTotalTickCount++;
			m_nUsageTickCount++;
			AnalyzeGlobalUsageOverflow();
		};

		UINT GetProfileTickCount()
		{
			return m_pProfile->m_nTotalTickCount;
		};

		UINT GetProfileRarelyPercent()
		{
			return m_pProfile->m_nRarelyPercent;
		};

		void SetProfileRarelyPercent( UINT nRarelyPercent)
		{
			if( nRarelyPercent > 100 )
				nRarelyPercent = 100;

			m_pProfile->m_nRarelyPercent = nRarelyPercent;
		};

	protected:
		void AnalyzeGlobalUsageOverflow();

		friend class CTCmdManager;
		friend class cmd_profile;
	}; // class cmd

	typedef CArray< icon*, icon*>				icon_container;
	typedef CMap< UINT, UINT, cmd*, cmd *&>		cmd_container;

	class __TUI_API cmd_profile
	{
	public:
		UINT m_nRarelyPercent;
		UINT m_nTotalTickCount;

		CString m_strName;

		icon_container m_icons;
		cmd_container m_cmds;

		cmd_profile( LPCTSTR strName = NULL) : m_strName( !strName ? _T("") : strName),
			m_nRarelyPercent(__TMFC_DEF_RARELY_USED_PERCENT),
			m_nTotalTickCount(0)
		{
		};

		cmd_profile( const cmd_profile &other) : m_strName(_T("")),
			m_nRarelyPercent(__TMFC_DEF_RARELY_USED_PERCENT),
			m_nTotalTickCount(0)
		{
			AssignFromOther(other);
		};

		~cmd_profile()
		{
			_RemoveAllIconsImpl();
			_RemoveAllCmdsImpl();
		};

		cmd_profile &operator=( const cmd_profile &other)
		{
			AssignFromOther(other);
			return *this;
		};

		BOOL CmdSetup( const cmd &_cmd, BOOL bReplaceOld = FALSE, BOOL *pWasAddedNew = NULL)
		{
			if(pWasAddedNew)
				*pWasAddedNew = FALSE;

			if(!IsCommand(_cmd.m_nCmdID))
				return FALSE;

			cmd *p_cmd = NULL;
			BOOL bExist = m_cmds.Lookup( _cmd.m_nCmdID, p_cmd);

			if(!bExist)
			{
				p_cmd = new cmd(_cmd);
				p_cmd->m_pProfile = this;
				m_cmds.SetAt( _cmd.m_nCmdID, p_cmd);

				if(pWasAddedNew)
					*pWasAddedNew = TRUE;

				return TRUE;
			}

			p_cmd->UpdateMoreExact(_cmd);
			return TRUE;
		};

		BOOL CmdRemove( UINT nCmdID, BOOL *pWasRemoved = NULL)
		{
			if(pWasRemoved)
				*pWasRemoved = FALSE;

			if(!IsCommand(nCmdID))
				return FALSE;

			cmd *p_cmd = NULL;
			BOOL bExist = m_cmds.Lookup( nCmdID, p_cmd);

			if(!bExist)
				return TRUE;

			delete p_cmd;
			m_cmds.RemoveKey(nCmdID);

			if(pWasRemoved)
				*pWasRemoved = TRUE;

			return TRUE;
		};

		cmd *CmdAllocPtr( UINT nCmdID = 0)
		{
			if( nCmdID && CmdIsRegistered(nCmdID) )
				return NULL;

			if( nCmdID == 0 )
			{
				nCmdID = 65534;

				if(!m_cmds.IsEmpty())
				{
					while(TRUE)
					{
						cmd *p_cmd = NULL;
						BOOL bExist = m_cmds.Lookup( nCmdID, p_cmd);

						if(bExist)
						{
							nCmdID --;

							if( nCmdID == 0 )
								return NULL;

							continue;
						}

						break;
					}
				}
			}

			cmd _cmd(nCmdID);
			if(!CmdSetup(_cmd))
				return NULL;

			cmd *p_cmd = CmdGetPtr(nCmdID);
			return p_cmd;
		};

		cmd *CmdGetPtr( UINT nCmdID)
		{
			if(!IsCommand(nCmdID))
				return NULL;

			cmd *p_cmd = NULL;
			BOOL bExist = m_cmds.Lookup( nCmdID, p_cmd);

			if(!bExist)
				return NULL;

			return p_cmd;
		};

		BOOL CmdSetIcon( UINT nCmdID, HICON hIcon, BOOL bCopyIcon = TRUE)
		{
			if(!IsCommand(nCmdID))
				return FALSE;

			cmd *p_cmd = NULL;
			BOOL bExist = m_cmds.Lookup( nCmdID, p_cmd);

			if(!bExist)
				return FALSE;

			if(!hIcon)
			{
				if( p_cmd->m_nIconIdx < 0 )
					return TRUE;

				icon *p_icon = m_icons[p_cmd->m_nIconIdx];
				delete p_icon;

				POSITION pos = m_cmds.GetStartPosition();
				while(pos)
				{
					cmd *p_cmd2 = NULL;
					UINT nCmdID2 = (UINT) IDC_STATIC;

					m_cmds.GetNextAssoc( pos, nCmdID2, p_cmd2);
					if( p_cmd2 == p_cmd ||
						p_cmd2->m_nIconIdx < 0 ||
						p_cmd2->m_nIconIdx < p_cmd->m_nIconIdx )
						continue;

					p_cmd2->m_nIconIdx--;
				}

				p_cmd->m_nIconIdx = -1;
				return TRUE;
			}
			else
			{
				if( p_cmd->m_nIconIdx < 0 )
				{
					icon *pIcon = NULL;

					try
					{
						pIcon = new icon( hIcon, bCopyIcon);
					}
					catch( CException * pExcept)
					{
						pExcept->Delete();
						return FALSE;
					}
					catch(...)
					{
						return FALSE;
					}

					p_cmd->m_nIconIdx = (int) m_icons.GetSize();
					m_icons.Add(pIcon);
				}
				else
				{
					icon *p_icon = m_icons[p_cmd->m_nIconIdx];

					if(bCopyIcon)
						*p_icon = hIcon;
					else
					{
						HICON hIconOld = p_icon->Detach();
						::DestroyIcon(hIconOld);
						p_icon->Attach(hIcon);
					}
				}

				return TRUE;
			}
		}

		icon *CmdGetIconPtr( UINT nCmdID)
		{
			if(!IsCommand(nCmdID))
				return NULL;

			cmd *p_cmd = NULL;
			BOOL bExist = m_cmds.Lookup( nCmdID, p_cmd);

			if(!bExist)
				return NULL;
			if( p_cmd->m_nIconIdx < 0 )
				return NULL;

			icon *p_icon = m_icons[p_cmd->m_nIconIdx];
			return p_icon;
		};

		BOOL CmdIsRegistered( UINT nCmdID)
		{
			BOOL bRegistered = CmdGetPtr(nCmdID) != NULL;
			return bRegistered;
		};

		BOOL UpdateFromMenu(
			HMENU hMenu,
			BOOL bReplaceOld = FALSE,
			BOOL bRecursive = TRUE,
			BOOL bLoadTips = TRUE);

		BOOL UpdateFromMenu(
			UINT nResourceID,
			BOOL bReplaceOld = FALSE,
			BOOL bLoadTips = TRUE);

		BOOL UpdateFromToolBar(
			CToolBar &bar,
			BOOL bReplaceOld = FALSE,
			BOOL bLoadTips = TRUE);

		BOOL UpdateFromToolBar(
			UINT nResourceID,
			BOOL bReplaceOld = FALSE,
			BOOL bLoadTips = TRUE);

		BOOL SetBasicCommands(
			UINT *pCommands,
			BOOL bOn = TRUE);

	private:
		void AssignFromOther( const cmd_profile &other)
		{
			m_nTotalTickCount = other.m_nTotalTickCount;
			m_nRarelyPercent = other.m_nRarelyPercent;
			m_strName = other.m_strName;
			_RemoveAllCmdsImpl();

			POSITION pos = other.m_cmds.GetStartPosition();
			while(pos)
			{
				UINT nCmdID;
				cmd *p_cmd = NULL;

				other.m_cmds.GetNextAssoc( pos, nCmdID, p_cmd);
				m_cmds[nCmdID] = new cmd(*p_cmd);
			}

			_RemoveAllIconsImpl();
			for( int iIcon=0; iIcon<other.m_icons.GetSize(); iIcon++)
			{
				icon *p_icon = other.m_icons[iIcon];
				m_icons.Add(new icon(*p_icon));
			}
		};

		void _RemoveAllCmdsImpl()
		{
			POSITION pos = m_cmds.GetStartPosition();
			while(pos)
			{
				UINT nCmdID;
				cmd *p_cmd = NULL;

				m_cmds.GetNextAssoc( pos, nCmdID, p_cmd);
				delete p_cmd;
			}

			m_cmds.RemoveAll();
		};

		void _RemoveAllIconsImpl()
		{
			for( int iIcon=0; iIcon<m_icons.GetSize(); iIcon++)
			{
				icon *p_icon = m_icons[iIcon];
				delete p_icon;
			}

			m_icons.RemoveAll();
		};
	}; // class cmd_profile

	static BOOL m_bDisableCmdIfNoHandler;

private:
	typedef CMapStringToPtr profile_container;
	profile_container m_profiles;

	typedef CMap< HWND, HWND, cmd_profile *, cmd_profile *&> profile_wnd_container;
	profile_wnd_container m_profile_wnds;

	void _RemoveAllProfilesImpl()
	{
		POSITION pos = m_profiles.GetStartPosition();

		while(pos)
		{
			cmd_profile *pProfile = NULL;
			CString strProfileName;

			m_profiles.GetNextAssoc( pos, strProfileName, (void *&) pProfile);
			delete pProfile;
		}

		m_profiles.RemoveAll();
		m_profile_wnds.RemoveAll();
	};

	CCriticalSection m_cs;

public:
	class __TUI_API CTCmdManagerAutoPtr
	{
		static volatile DWORD m_dwVersion;
		CTCmdManager *m_pCmdMenager;

	public:
		CTCmdManagerAutoPtr();
		~CTCmdManagerAutoPtr();

		CTCmdManager *operator->();

		static DWORD GetVersionDWORD( BOOL bForSerialization = FALSE);
		static LPCTSTR GetVersionString( CString &strBuff, TCHAR strSeparator = _T('.'));
	}; // class CTCmdManagerAutoPtr

public:
	CTCmdManager();
	~CTCmdManager();

public:
	BOOL ProfileSetup(
		LPCTSTR sProfileName = NULL,
		HWND hProfileWnd = NULL);

	CTCmdManager::cmd_profile *ProfileGetPtr( LPCTSTR strProfileName = NULL);
	BOOL ProfileWndRemove( HWND hProfileWnd);
	LPCTSTR ProfileNameFromWnd( HWND hWnd);

	BOOL ProfileWndAdd(
		LPCTSTR strProfileName,
		HWND hProfileWnd);

	BOOL CmdSetup(
		LPCTSTR sProfileName,
		const CTCmdManager::cmd& cmd,
		BOOL bReplaceOld = FALSE,
		BOOL *pWasAddedNew = NULL);

	BOOL CmdRemove(
		LPCTSTR sProfileName,
		UINT nCmdID,
		BOOL *pWasRemoved = NULL);

	CTCmdManager::cmd* CmdGetPtr(
		LPCTSTR sProfileName,
		UINT nCmdID);

	CTCmdManager::cmd* CmdAllocPtr(
		LPCTSTR sProfileName,
		UINT nCmdID = 0);

	BOOL CmdIsRegistered(
		LPCTSTR sProfileName,
		UINT nCmdID);

	BOOL CmdSetIcon(
		LPCTSTR sProfileName,
		UINT nCmdID,
		HICON hIcon,
		BOOL bCopyIcon = TRUE);

	CTCmdManager::icon* CmdGetIconPtr(
		LPCTSTR strProfileName,
		UINT nCmdID);

	HICON CmdGetHICON(
		LPCTSTR strProfileName,
		UINT nCmdID)
	{
		CTCmdManager::icon* pIcon = CmdGetIconPtr( strProfileName, nCmdID);

		if(!pIcon)
			return NULL;

		HICON hIcon = (*pIcon);
		return hIcon;
	};

	BOOL UpdateFromMenu(
		LPCTSTR strProfileName,
		HMENU hMenu,
		BOOL bReplaceOld = FALSE,
		BOOL bRecursive = TRUE,
		BOOL bLoadTips = TRUE);

	BOOL UpdateFromMenu(
		LPCTSTR strProfileName,
		UINT nResourceID,
		BOOL bReplaceOld = FALSE,
		BOOL bLoadTips = TRUE);

	BOOL UpdateFromToolBar(
		LPCTSTR sProfileName,
		CToolBar &bar,
		BOOL bReplaceOld = FALSE,
		BOOL bLoadTips = TRUE);

	BOOL UpdateFromToolBar(
		LPCTSTR strProfileName,
		UINT nResourceID,
		BOOL bReplaceOld = FALSE,
		BOOL bLoadTips = TRUE);

	BOOL SetBasicCommands(
		LPCTSTR strProfileName,
		UINT *pCommands,
		BOOL bOn = TRUE);

	friend class cmd;
	friend class cmd_profile;
}; // class CTCmdManager


extern __TUI_API CTCmdManager::CTCmdManagerAutoPtr g_CmdManager;

#endif // #if (!defined __TCMDMANAGER_H)