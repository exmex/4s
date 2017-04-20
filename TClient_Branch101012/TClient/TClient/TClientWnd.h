#pragma once
#include "TClientGame.h"
#include "TClientNET.h"

class CReleaseChart
{
public:
	CReleaseChart();
	~CReleaseChart();
};


class CTClientWnd : public CTachyonWnd
{
public:
	CTClientWnd();
	virtual ~CTClientWnd();

	CReleaseChart m_vReleaseChart;

public:
	HCURSOR m_vTCURSOR[TCURSOR_COUNT];						// Cursor pool

	VTCHARINFO m_vTCHARINFO;								// Character information pool
	TCHARINFO m_vNEWTCHAR;									// New character pool
	MAPTGROUP m_mapTGROUP;									// Group pool

// Attributes
public:
	CTClientSession m_session[MAX_CON];
	CTClientSession *m_pTNet;
	CTClientWebCtrl m_vWebCtrl;
	MAPTPENDINGMAPSESSION m_mapPendingMapSession;

	CTClientMsgBox m_MessageBox;
	TMESSAGEBOX_STACK m_MsgBoxStack;
	CTClientNET m_TNet;
	TFrame* m_pTNetFRAME[TFRAME_COUNT];

	CString m_strLOADMSG;
	CD3DFont m_FontLOAD;

	CTClientGame m_MainGame;
	CTClientCAM m_vTextCAM;
	CTClientCAM m_Camera;

	TGROUP m_vTGROUP;
	DWORD m_dwUserID;
	DWORD m_dwKickID;
	DWORD m_dwKEY;

	HANDLE m_hResetEvent;
	BYTE m_bCreateCardCnt;
	BYTE m_bResetDevice;
	BYTE m_bResetTick;
	BYTE m_bSlotID;

	BYTE m_bInPCROOM;
	DWORD m_dwPrmPCROOM;

	DWORD m_dwLoginDelay;
	BYTE m_bAutoLogin;
	BYTE m_bRelogin;

	TFrame *m_pMainFrameBak;
	BYTE m_bModalFrame;

	BYTE m_bChannelNumber;
	BYTE m_bChannel;

	TCHANNEL_ARRAY m_vTCHANNELS;
	BOOL m_bNeedReadAgreement;

	DWORD m_dwCmdParam;
	BYTE m_bIntendLogout;
	BYTE m_bNetCableDisconnected;

	BOOL m_bWebShop;
	
public:
	DWORD LoadThread();

public:
	CTClientSession *GetIdleSession();
	DWORD GetTClientTickCount();
	CString CaptureIMG();
	void SetCursor();
	BOOL IsLoading();
	void OnAgreementPage();
	void LoadImageFile( const CString& strFILE, CT3DImage* pT3DIMG);
	void SetMainFrame( TFrame* );
	TFrame* GetMainFrame();

	BOOL IsWebShop();

	// Operations
public:
	virtual void OnConnect( CTachyonSession *pSession, int nErrorCode);
	virtual void OnClose( CTachyonSession *pSession, int nErrorCode);

	virtual void OnSessionMsg( CTachyonSession *pSession, CPacket *pPacket);
	virtual void OnCommand( TCOMMAND cmd);

	virtual void ProcessHotCommand( int nID);
	virtual void Render();

	virtual void OnChar(
		UINT nChar,
		UINT nRep,
		UINT nFlags);

	virtual void InitResource(
		CString strGroupID,
		BYTE bGroupID);
	virtual BOOL ResetDevices();

	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

// Implementation
public:
	void OnERROR( int nERROR);

	void InitTDYNAMICHELP();
	void InitTRACETEMP();
	void InitTRECTVB();
	void InitTRESIMG();
	
	void ReleaseTITEMTEMP();
	void ReleaseTQuestMagicClientItem();
	void ReleaseTCHARINFO();
	void ReleaseTRECTVB();
	void ReleaseTRESIMG();

	void SaveCustomCompPos();
	void LoadCustomCompPos();

	void InitCamera();
	void InitChart();
	void InitRSCS();
	void InitNAV();
	void InitUI( BOOL bResetOnlyPosition = FALSE);
	void InitCommonConst();
	void InitTexture();

	void ReleaseChart();
	void ReleaseTexture();

	BYTE RestoreDevice();
	void ResetTOption();
	void ReFresh();

	void SelectCONTRY();
	void SelectNEWTCHAR();
	void SelectCHAR();
	void LoadingIMG();
	void ExitGame();
	void EquipStartChar(
		LPTCHARINFO pINFO);

	void ClearMessageBox();
	void ClearMsgBoxStackForced();
	void InsertMsgBoxStack(
		CString strMSG,
		CString strEditTitle,
		CString strEditText,
		DWORD dwYesTextID,
		DWORD dwNoTextID,
		DWORD dwFrameID,
		TCOMMAND vYesID,
		TCOMMAND vNoID,
		TCOMMAND vIgnoreID,
		BYTE bEdit,
		BYTE bModal,
		CString strTitle,
		DWORD dwImageIndex,
		CString strImageTitle,
		DWORD dwSound,
		BYTE bSoundType );

	void CloseMessageBox(
		BOOL bCloseAll=FALSE);

	BOOL IsMsgBoxVisible();

	BOOL MessageBoxOK(
		DWORD dwMessage,
		DWORD dwOkText,
		TCOMMAND vOK,
		TCOMMAND vIgnore,
		BYTE bModal=TRUE,
		DWORD dwSound=ID_SND_INFO,
		BYTE bSoundType=MEDIA_TSOUND,
		enum TMSGBOXOPEN_TYPE eOpenType=TMSGBOXOPEN_NONE,
		DWORD dwTitle=0 );

	BOOL MessageBoxOK(
		CString strMSG,
		DWORD dwOkText,
		TCOMMAND vOK,
		TCOMMAND vIgnore,
		BYTE bModal=TRUE,
		DWORD dwSound=ID_SND_INFO,
		BYTE bSoundType=MEDIA_TSOUND,
		enum TMSGBOXOPEN_TYPE eOpenType=TMSGBOXOPEN_NONE,
		DWORD dwTitle=0 );

	BOOL MessageBoxYesNo(
		DWORD dwMessage,
		DWORD dwYesText,
		DWORD dwNoText,
		TCOMMAND vYesID,
		TCOMMAND vNoID,
		TCOMMAND vIgnoreID=0,
		BYTE bModal=TRUE,
		DWORD dwSound=ID_SND_INFO,
		BYTE bSoundType=MEDIA_TSOUND,
		enum TMSGBOXOPEN_TYPE eOpenType=TMSGBOXOPEN_NONE,
		DWORD dwTitle=0 );

	BOOL MessageBoxYesNo(
		CString strMSG,
		DWORD dwYesText,
		DWORD dwNoText,
		TCOMMAND vYesID,
		TCOMMAND vNoID,
		TCOMMAND vIgnoreID=0,
		BYTE bModal=TRUE,
		DWORD dwSound=ID_SND_INFO,
		BYTE bSoundType=MEDIA_TSOUND,
		enum TMSGBOXOPEN_TYPE eOpenType=TMSGBOXOPEN_NONE,
		DWORD dwTitle=0 );

	BOOL MessageBoxItemOK(
		CString strMSG,
		DWORD dwOkText,
		TCOMMAND vOK,
		TCOMMAND vIgnore,
		DWORD dwImageIndex=0,
		CString strImageTitle="",
		BYTE bModal=TRUE,
		DWORD dwSound=ID_SND_INFO,
		BYTE bSoundType=MEDIA_TSOUND,
		enum TMSGBOXOPEN_TYPE eOpenType=TMSGBOXOPEN_NONE);

	BOOL MessageBoxItemYesNo(
		CString strMSG,
		DWORD dwYesText,
		DWORD dwNoText,
		TCOMMAND vYesID,
		TCOMMAND vNoID,
		TCOMMAND vIgnoreID=0,
		DWORD dwImageIndex=0,
		CString strImageTitle="",
		BYTE bModal=TRUE,
		DWORD dwSound=ID_SND_INFO,
		BYTE bSoundType=MEDIA_TSOUND,
		enum TMSGBOXOPEN_TYPE eOpenType=TMSGBOXOPEN_NONE );

	BOOL MessageBox(
		DWORD dwMessageID,
		DWORD dwEditTitleID,
		DWORD dwEditTextID,
		DWORD dwYesTextID,
		DWORD dwNoTextID,
		DWORD dwFrameID,
		TCOMMAND vYesID,
		TCOMMAND vNoID,
		TCOMMAND vIgnoreID,
		BYTE bEdit,
		BYTE bModal,
		DWORD dwTitle=0,
		DWORD dwImageIndex=0,
		CString strImageTitle="",
		DWORD dwSound=ID_SND_INFO,
		BYTE bSoundType=MEDIA_TSOUND,
		TMSGBOXOPEN_TYPE eOpenType=TMSGBOXOPEN_NONE);

	BOOL MessageBox(
		CString strMSG,
		CString strEditTitle,
		CString strEditText,
		DWORD dwYesTextID,
		DWORD dwNoTextID,
		DWORD dwFrameID,
		TCOMMAND vYesID,
		TCOMMAND vNoID,
		TCOMMAND vIgnoreID,
		BYTE bEdit,
		BYTE bModal,
		CString strTitle="",
		DWORD dwImageIndex=0,
		CString strImageTitle="",
		DWORD dwSound=ID_SND_INFO,
		BYTE bSoundType=MEDIA_TSOUND,
		TMSGBOXOPEN_TYPE eOpenType=TMSGBOXOPEN_NONE,
		BOOL bInsertStack=TRUE);

	CString GetMsgBoxStrInEditor(DWORD dwMessageID);
	void ClearMsgBoxEditor(DWORD dwMessageID);

	void DoModalFrame(BYTE bFrameID);
	void CloseModalFrame();

	TFrame* GetCurrentFrame();

	BOOL SetGamma(float fGamma, int nOverBright, float fContrast);

	static int CheckCharID( LPCSTR str);
	static int CheckComment( LPCSTR str);
	BOOL CheckProcess(VECTORSTRING* pOutDoubtProcs=NULL);

	// Utility function
	CString ReadString( CFile *pFILE);
	TFrame *GetTMainFrame();

	void ProcLoadCMD(
		CTClientApp *pTAPP,
		LPTLOADCMD pTCMD);
	void InitChatProc();

	static void OnChangeResolutionCmdBtnDown(
		CTMiniPopupDlg* pPopupDlg,
		CTMiniPopupDlg::CTButtonMP* pButton);

	void CollectResolution();

	BOOL IsExistingMapSession( BYTE bSvrID );
	BYTE CheckModuleFile(BYTE bAll, INT64 dlCP);

// Msg Sender
public:
	void SendCS_GROUPLIST_REQ();
	void SendCS_CHARLIST_REQ();
	void SendCS_HOTSEND_REQ(INT64 dlValue, BYTE bAll = FALSE);

	void SendCS_CREATECHAR_REQ(
		BYTE bGroupID,
		CString strNAME,
		BYTE bSlotID,
		BYTE bClass,
		BYTE bRace,
		BYTE bContry,
		BYTE bSex,
		BYTE bHair,
		BYTE bFace,
		BYTE bBody,
		BYTE bPants,
		BYTE bHand,
		BYTE bFoot);

	void SendCS_DELCHAR_REQ(
		BYTE bGroupID,
		CString strPasswd,
		DWORD dwCharID);

	void SendCS_START_REQ(
		BYTE bGroupID,
		BYTE bChannel,
		DWORD dwCharID);

	void SendCS_LOGIN_REQ(
		CString strUserID,
		CString strPasswd,
		DWORD dwSiteCode = 0);

	void SendCS_CHANNELLIST_REQ(
		BYTE bGroupID);

	void SendCS_EXITMAP_REQ();
	void SendCS_AGREEMENT_REQ();

// Msg Handler
public:
	int OnCS_CREATECHAR_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_DELCHAR_ACK( CTachyonSession *pSession, CPacket *pPacket);

	int OnCS_GROUPLIST_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_CHARLIST_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_LOGIN_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_START_ACK( CTachyonSession *pSession, CPacket *pPacket);

	int OnCS_CHANNELLIST_ACK( CTachyonSession *pSession, CPacket *pPacket);

// Menu Handler
public:
	int OnGM_START_4STORY();

	int OnGM_TOGGLE_SCREEN_MODE();
	int OnGM_BACKTO_SERVER();
	int OnGM_BACKTO_LOGIN();
	int OnGM_GOTO_CONTRY();
	int OnGM_GOTO_NEWCHAR();
	int OnGM_CLOSE_MSGBOX();
	int OnGM_BACKTO_CHAR();
	int OnGM_DEL_CHARMSG();
	int OnGM_START_GAME();
	int OnGM_PREV_FACE();
	int OnGM_PREV_HAIR();
	int OnGM_NEXT_FACE();
	int OnGM_NEXT_HAIR();
	int OnGM_PREV_RACE();
	int OnGM_PREV_CLASS();
	int OnGM_NEXT_RACE();
	int OnGM_NEXT_CLASS();
	int OnGM_RACE_SEL_M();
	int OnGM_RACE_SEL_W();
	int OnGM_GOTO_CHAR();
	int OnGM_SEL_CHAR0();
	int OnGM_SEL_CHAR1();
	int OnGM_SEL_CHAR2();
	int OnGM_SEL_CHAR3();
	int OnGM_SEL_CHAR4();
	int OnGM_SEL_CHAR5();
	int OnGM_NEW_CHAR();
	int OnGM_DEL_CHAR();
	int OnGM_CONTRY_D();
	int OnGM_CONTRY_C();
	int OnGM_RSEL_HM();
	int OnGM_RSEL_HW();
	int OnGM_RSEL_BM();
	int OnGM_RSEL_BW();
	int OnGM_RSEL_NM();
	int OnGM_RSEL_NW();
	int OnGM_CSEL_0();
	int OnGM_CSEL_1();
	int OnGM_CSEL_2();
	int OnGM_CSEL_3();
	int OnGM_CSEL_4();
	int OnGM_CSEL_5();
	int OnGM_SELECT_CHANNEL();
	int OnGM_AGREEMENT();

	int OnGM_NETCABLE_DISCONNECTED();
	int OnGM_EXIT_GAME();
	int OnGM_LOGIN();
	int OnGM_EXIT();

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnOBJMessage( WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

private:
	static DWORD WINAPI _CaptureIMG( LPVOID lpParam);

public:
	static CTClientWnd* m_pINSTANCE;
	static CTClientWnd* GetInstance();
	
	static VTRESOLUTIONMODE m_vResolutionModes;
	afx_msg void OnTimer(UINT nIDEvent);
};
