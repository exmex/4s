#include "StdAfx.h"
#include <afxisapi.h>
#include <exdispid.h>
#include "THtmlDocWnd.h"
#include "TOccManager.h"


class TOleControlSite : public COleControlSite
{
public:
	TOleControlSite( COleControlContainer *pCnt, THttpCtrl *pTHOST) : COleControlSite(pCnt)
	{
		m_xOleCtrlSite.m_pTHOST = pTHOST;
		m_xOleCtrlSite.m_bReady = FALSE;
	};

	virtual ~TOleControlSite() {};

	virtual HRESULT CreateControl(
		CWnd* pWndCtrl,
		REFCLSID clsid,
		LPCTSTR lpszWindowName,
		DWORD dwStyle,
		const POINT* ppt,
		const SIZE* psize,
		UINT nID,
		CFile* pPersist = NULL,
		BOOL bStorage = FALSE,
		BSTR bstrLicKey = NULL);
	virtual BOOL QuickActivate();

BEGIN_INTERFACE_PART( HostUIHandler, IDocHostUIHandler)
	STDMETHODIMP GetExternal( IDispatch __RPC_FAR *__RPC_FAR *ppDispatch);
	STDMETHODIMP GetHostInfo( DOCHOSTUIINFO __RPC_FAR *pInfo);

	STDMETHODIMP OnFrameWindowActivate( BOOL fActive);
	STDMETHODIMP OnDocWindowActivate( BOOL fActive);
	STDMETHODIMP EnableModeless( BOOL fEnable);
	STDMETHODIMP UpdateUI();
	STDMETHODIMP HideUI();

	STDMETHODIMP FilterDataObject(
		IDataObject __RPC_FAR *pDO,
		IDataObject __RPC_FAR *__RPC_FAR *ppDORet);

	STDMETHODIMP GetDropTarget(
		IDropTarget __RPC_FAR *pDropTarget,
		IDropTarget __RPC_FAR *__RPC_FAR *ppDropTarget);

	STDMETHODIMP GetOptionKeyPath(
		LPOLESTR __RPC_FAR *pchKey,
		DWORD dwReserved);

	STDMETHODIMP ResizeBorder(
		LPCRECT prcBorder,
		IOleInPlaceUIWindow __RPC_FAR *pUIWindow,
		BOOL fFrameWindow);

	STDMETHODIMP ShowContextMenu(
		DWORD dwID,
		POINT __RPC_FAR *ppt,
		IUnknown __RPC_FAR *pcmdtReserved,
		IDispatch __RPC_FAR *pdispReserved);

	STDMETHODIMP ShowUI(
		DWORD dwID,
		IOleInPlaceActiveObject __RPC_FAR *pActiveObject,
		IOleCommandTarget __RPC_FAR *pCommandTarget,
		IOleInPlaceFrame __RPC_FAR *pFrame,
		IOleInPlaceUIWindow __RPC_FAR *pDoc);

	STDMETHODIMP TranslateAccelerator(
		LPMSG lpMsg,
		const GUID __RPC_FAR *pguidCmdGroup,
		DWORD nCmdID);

	STDMETHODIMP TranslateUrl(
		DWORD dwTranslate,
		OLECHAR __RPC_FAR *pchURLIn,
		OLECHAR __RPC_FAR *__RPC_FAR *ppchURLOut);
END_INTERFACE_PART(HostUIHandler)

BEGIN_INTERFACE_PART( OleCtrlSite, IOleControlSite)
	THttpCtrl *m_pTHOST;
	BYTE m_bReady;

	STDMETHODIMP GetExtendedControl( LPDISPATCH* ppDisp);
	STDMETHODIMP LockInPlaceActive( BOOL fLock);
	STDMETHODIMP OnFocus( BOOL fGotFocus);

	STDMETHODIMP OnControlInfoChanged();
	STDMETHODIMP ShowPropertyFrame();

	STDMETHODIMP TransformCoords(
		POINTL* lpptlHimetric,
		POINTF* lpptfContainer,
		DWORD flags);

	STDMETHODIMP TranslateAccelerator(
		LPMSG lpMsg,
		DWORD grfModifiers);
END_INTERFACE_PART(OleCtrlSite)

BEGIN_INTERFACE_PART( TEventSink, IDispatch)
	STDMETHODIMP GetTypeInfoCount( unsigned int *pctinfo);
	STDMETHODIMP GetTypeInfo(
		unsigned int itinfo,
		LCID lcid,
		ITypeInfo** pptinfo);

	STDMETHODIMP GetIDsOfNames(
		REFIID riid,
		LPOLESTR* rgszNames,
		unsigned int cNames,
		LCID lcid,
		DISPID* rgdispid);

	STDMETHODIMP Invoke(
		DISPID dispidMember,
		REFIID riid,
		LCID lcid,
		unsigned short wFlags,
		DISPPARAMS* pdispparams,
		VARIANT* pvarResult,
		EXCEPINFO* pexcepinfo,
		unsigned int* puArgErr);

END_INTERFACE_PART(TEventSink)

DECLARE_INTERFACE_MAP()
};


COleControlSite *TOccManager::CreateSite( COleControlContainer *pCtrlCont)
{
	return new TOleControlSite( pCtrlCont, m_pTHOST);
}

HRESULT TOleControlSite::CreateControl( CWnd* pWndCtrl,
									    REFCLSID clsid,
										LPCTSTR lpszWindowName,
										DWORD dwStyle,
										const POINT* ppt,
										const SIZE* psize,
										UINT nID,
										CFile* pPersist,
										BOOL bStorage,
										BSTR bstrLicKey)
{
	HRESULT hr = COleControlSite::CreateControl(
		pWndCtrl,
		clsid,
		lpszWindowName,
		dwStyle,
		ppt,
		psize,
		nID,
		pPersist,
		bStorage,
		bstrLicKey);

	if( hr == S_OK )
	{
		DisconnectSink( m_iidEvents, m_dwEventSink);
		m_dwEventSink = ConnectSink( m_iidEvents, &m_xTEventSink);
	}

	return hr;
}

BOOL TOleControlSite::QuickActivate()
{
	static const struct { DISPID dwDispID; DWORD dwFlag;} vAmbients[] =
	{
		{ DISPID_AMBIENT_USERMODE,          QACONTAINER_USERMODE},
		{ DISPID_AMBIENT_UIDEAD,            QACONTAINER_UIDEAD},
		{ DISPID_AMBIENT_SHOWGRABHANDLES,   QACONTAINER_SHOWGRABHANDLES},
		{ DISPID_AMBIENT_SHOWHATCHING,      QACONTAINER_SHOWHATCHING},
		{ DISPID_AMBIENT_DISPLAYASDEFAULT,  QACONTAINER_DISPLAYASDEFAULT},
		{ DISPID_AMBIENT_AUTOCLIP,          QACONTAINER_AUTOCLIP},
		{ DISPID_AMBIENT_MESSAGEREFLECT,    QACONTAINER_MESSAGEREFLECT},
		{ DISPID_AMBIENT_SUPPORTSMNEMONICS, QACONTAINER_SUPPORTSMNEMONICS},
	};

	IQuickActivate* pQuick = NULL;
	BOOL bQuickActivated = FALSE;

	if(SUCCEEDED(m_pObject->QueryInterface( IID_IQuickActivate, reinterpret_cast<void**>(&pQuick))))
	{
		ASSERT(pQuick != NULL);

		// Initialize QACONTAINER structure.
		QACONTAINER qaContainer;

		qaContainer.cbSize = sizeof(QACONTAINER);
		qaContainer.pClientSite = &m_xOleClientSite;
		qaContainer.pAdviseSink = NULL;
		qaContainer.pPropertyNotifySink = &m_xPropertyNotifySink;
		qaContainer.pUnkEventSink = &m_xTEventSink;
		qaContainer.pUndoMgr = NULL;
		qaContainer.hpal = NULL;
		qaContainer.pBindHost = NULL;

		// Fill ambient property values in QACONTAINER.
		COleVariant var;
		CWnd *pWndContain = m_pCtrlCont->m_pWnd;

		qaContainer.dwAmbientFlags = 0;
		for( int i=0; i<_countof(vAmbients); i++)
		{
			pWndContain->OnAmbientProperty( this, vAmbients[i].dwDispID, &var);
			if(V_BOOL(&var))
				qaContainer.dwAmbientFlags |= vAmbients[i].dwFlag;
		}

		pWndContain->OnAmbientProperty( this, DISPID_AMBIENT_FORECOLOR, &var);
		qaContainer.colorFore = V_I4(&var);
		pWndContain->OnAmbientProperty( this, DISPID_AMBIENT_BACKCOLOR, &var);
		qaContainer.colorBack = V_I4(&var);
		pWndContain->OnAmbientProperty( this, DISPID_AMBIENT_APPEARANCE, &var);
		qaContainer.dwAppearance = V_I2(&var);
		pWndContain->OnAmbientProperty( this, DISPID_AMBIENT_LOCALEID, &var);
		qaContainer.lcid = V_I4(&var);
		pWndContain->OnAmbientProperty( this, DISPID_AMBIENT_FONT, &var);

		if(FAILED(V_DISPATCH(&var)->QueryInterface( IID_IFont, reinterpret_cast<void**>(&qaContainer.pFont))))
			qaContainer.pFont = NULL;

		// Initialize QACONTROL structure.
		QACONTROL qaControl;
		qaControl.cbSize = sizeof(QACONTROL);

		// Do the quick activation.
		if(SUCCEEDED(pQuick->QuickActivate( &qaContainer, &qaControl)))
		{
			// Extract return values from QACONTROL structure.
			m_dwMiscStatus = qaControl.dwMiscStatus;
			m_dwEventSink = qaControl.dwEventCookie;
			m_dwPropNotifySink = qaControl.dwPropNotifyCookie;
			bQuickActivated = TRUE;
		}
		pQuick->Release();

		if( qaContainer.pFont != NULL )
			qaContainer.pFont->Release();
	}

	return bQuickActivated;
}


BEGIN_INTERFACE_MAP( TOleControlSite, COleControlSite)
	INTERFACE_PART( TOleControlSite, IID_IDocHostUIHandler, HostUIHandler)
	INTERFACE_PART( TOleControlSite, DIID_DWebBrowserEvents2, TEventSink)
	INTERFACE_PART( TOleControlSite, IID_IOleControlSite, OleCtrlSite)
END_INTERFACE_MAP()


ULONG TOleControlSite::XTEventSink::AddRef()
{
	return 1;
}

ULONG TOleControlSite::XTEventSink::Release()
{
	return 0;
}

HRESULT TOleControlSite::XTEventSink::QueryInterface( REFIID riid, void **ppvObj)
{
	METHOD_PROLOGUE( TOleControlSite, TEventSink)

	if( IsEqualIID( riid, IID_IUnknown) ||
		IsEqualIID( riid, IID_IDispatch) ||
		IsEqualIID( riid, pThis->m_iidEvents) )
	{
		*ppvObj = this;
		AddRef();

		return S_OK;
	}

	return E_NOINTERFACE;
}

STDMETHODIMP TOleControlSite::XTEventSink::GetTypeInfoCount( unsigned int *pctinfo)
{
	return E_NOTIMPL;
}

STDMETHODIMP TOleControlSite::XTEventSink::GetTypeInfo( unsigned int itinfo,
													    LCID lcid,
														ITypeInfo** pptinfo)
{
	return E_NOTIMPL;
}

STDMETHODIMP TOleControlSite::XTEventSink::GetIDsOfNames( REFIID riid,
														  LPOLESTR* rgszNames,
														  unsigned int cNames,
														  LCID lcid,
														  DISPID* rgdispid)
{
	return E_NOTIMPL;
}

STDMETHODIMP TOleControlSite::XTEventSink::Invoke( DISPID dispidMember,
												   REFIID riid,
												   LCID lcid,
												   unsigned short wFlags,
												   DISPPARAMS* pdispparams,
												   VARIANT* pvarResult,
												   EXCEPINFO* pexcepinfo,
												   unsigned int* puArgErr)
{
	UNUSED(wFlags);

	METHOD_PROLOGUE( TOleControlSite, TEventSink)
	ASSERT(pThis->m_pCtrlCont != NULL);
	ASSERT(pThis->m_pCtrlCont->m_pWnd != NULL);
	ASSERT(wFlags == DISPATCH_METHOD);

	AFX_EVENT event(
		AFX_EVENT::event,
		dispidMember,
		pdispparams,
		pexcepinfo,
		puArgErr);
	pThis->OnEvent(&event);

	switch(dispidMember)
	{
	case DISPID_DOCUMENTCOMPLETE	:
		{
			THttpCtrl *pHTTP = pThis->m_xOleCtrlSite.m_pTHOST;

			EnumChildWindows( pThis->m_hWnd, THtmlDocWnd::__HookDocWnd, (LPARAM) pHTTP);
			pThis->m_xOleCtrlSite.m_bReady = TRUE;

			if( pHTTP && !pHTTP->HaveCaret(NULL) )
				pHTTP->OnLoseFocus(NULL);
		}

		break;

	case DISPID_DOWNLOADCOMPLETE	:
		{
			THttpCtrl *pHTTP = pThis->m_xOleCtrlSite.m_pTHOST;

			if(EnumChildWindows( pThis->m_hWnd, THtmlDocWnd::__PutReady, TRUE))
				pThis->m_xOleCtrlSite.m_bReady = TRUE;

			if( pHTTP && !pHTTP->HaveCaret(NULL) )
				pHTTP->OnLoseFocus(NULL);
		}

		break;

	case DISPID_BEFORENAVIGATE2		:
	case DISPID_DOWNLOADBEGIN		:
		{
			THttpCtrl *pHTTP = pThis->m_xOleCtrlSite.m_pTHOST;

			EnumChildWindows( pThis->m_hWnd, THtmlDocWnd::__PutReady, FALSE);
			pThis->m_xOleCtrlSite.m_bReady = FALSE;

			if( pHTTP && !pHTTP->HaveCaret(NULL) )
				pHTTP->OnLoseFocus(NULL);
		}

		break;
	}

	if(pvarResult)
		::VariantClear(pvarResult);

	return event.m_hResult;
}

ULONG TOleControlSite::XOleCtrlSite::AddRef()
{
	METHOD_PROLOGUE( TOleControlSite, OleCtrlSite)
	return pThis->InternalAddRef();
}

ULONG TOleControlSite::XOleCtrlSite::Release()
{
	METHOD_PROLOGUE( TOleControlSite, OleCtrlSite)
	return pThis->InternalRelease();
}

HRESULT TOleControlSite::XOleCtrlSite::QueryInterface( REFIID riid, void **ppvObj)
{
	METHOD_PROLOGUE( TOleControlSite, OleCtrlSite)
	return pThis->InternalQueryInterface( &riid, ppvObj);
}

STDMETHODIMP TOleControlSite::XOleCtrlSite::GetExtendedControl( LPDISPATCH* ppDisp)
{
	return E_NOTIMPL;
}

STDMETHODIMP TOleControlSite::XOleCtrlSite::LockInPlaceActive( BOOL fLock)
{
	return E_NOTIMPL;
}

STDMETHODIMP TOleControlSite::XOleCtrlSite::OnFocus( BOOL fGotFocus)
{
	if( m_bReady && m_pTHOST && fGotFocus )
	{
		TComponent *pTPARENT = m_pTHOST->GetParent();
		TComponent *pTFOCUS = m_pTHOST;

		while(pTPARENT)
		{
			if( pTPARENT->m_bType == TCML_TYPE_FRAME )
				((TFrame *) pTPARENT)->SwitchFocus(pTFOCUS);

			pTFOCUS = pTPARENT;
			pTPARENT = pTFOCUS->GetParent();
		}
	}

	return S_OK;
}

STDMETHODIMP TOleControlSite::XOleCtrlSite::OnControlInfoChanged()
{
	METHOD_PROLOGUE( TOleControlSite, OleCtrlSite)
	pThis->GetControlInfo();

	return S_OK;
}

STDMETHODIMP TOleControlSite::XOleCtrlSite::ShowPropertyFrame()
{
	return E_NOTIMPL;
}

STDMETHODIMP TOleControlSite::XOleCtrlSite::TransformCoords( POINTL* lpptlHimetric,
															 POINTF* lpptfContainer,
															 DWORD flags)
{
	METHOD_PROLOGUE( TOleControlSite, OleCtrlSite)
	HRESULT hr = S_OK;

	SIZE sz;
	CDC dc;

	dc.Attach(::GetDC(pThis->m_hWnd));
	if( flags & XFORMCOORDS_HIMETRICTOCONTAINER )
	{
		sz.cx = lpptlHimetric->x;
		sz.cy = lpptlHimetric->y;
		dc.HIMETRICtoDP(&sz);

		if( flags & XFORMCOORDS_SIZE )
		{
			lpptfContainer->x = (float) abs(sz.cx);
			lpptfContainer->y = (float) abs(sz.cy);
		}
		else if( flags & XFORMCOORDS_POSITION )
		{
			lpptfContainer->x = (float) sz.cx;
			lpptfContainer->y = (float) sz.cy;
		}
		else
			hr = E_INVALIDARG;
	}
	else if( flags & XFORMCOORDS_CONTAINERTOHIMETRIC )
	{
		sz.cx = (int) lpptfContainer->x;
		sz.cy = (int) lpptfContainer->y;
		dc.DPtoHIMETRIC(&sz);

		if( flags & XFORMCOORDS_SIZE )
		{
			lpptlHimetric->x = abs(sz.cx);
			lpptlHimetric->y = abs(sz.cy);
		}
		else if( flags & XFORMCOORDS_POSITION )
		{
			lpptlHimetric->x = sz.cx;
			lpptlHimetric->y = sz.cy;
		}
		else
			hr = E_INVALIDARG;
	}
	else
		hr = E_INVALIDARG;

	return hr;
}

STDMETHODIMP TOleControlSite::XOleCtrlSite::TranslateAccelerator( LPMSG lpMsg,
																  DWORD grfModifiers)
{
	return E_NOTIMPL;
}

ULONG TOleControlSite::XHostUIHandler::AddRef()
{
	METHOD_PROLOGUE( TOleControlSite, HostUIHandler)
	return pThis->ExternalAddRef();
}

ULONG TOleControlSite::XHostUIHandler::Release()
{
	METHOD_PROLOGUE( TOleControlSite, HostUIHandler)
	return pThis->ExternalRelease();
}

HRESULT TOleControlSite::XHostUIHandler::QueryInterface( REFIID riid, void **ppvObj)
{
	METHOD_PROLOGUE( TOleControlSite, HostUIHandler)
	return pThis->ExternalQueryInterface( &riid, ppvObj);
}

STDMETHODIMP TOleControlSite::XHostUIHandler::GetExternal( IDispatch __RPC_FAR *__RPC_FAR *ppDispatch)
{
	return E_NOTIMPL;
}

STDMETHODIMP TOleControlSite::XHostUIHandler::GetHostInfo( DOCHOSTUIINFO __RPC_FAR *pInfo)
{
	pInfo->dwDoubleClick = DOCHOSTUIDBLCLK_DEFAULT;
	pInfo->dwFlags = DOCHOSTUIFLAG_SCROLL_NO;
	pInfo->cbSize = sizeof(DOCHOSTUIINFO);

	return S_OK;
}

STDMETHODIMP TOleControlSite::XHostUIHandler::OnFrameWindowActivate( BOOL fActive)
{
	return E_NOTIMPL;
}

STDMETHODIMP TOleControlSite::XHostUIHandler::OnDocWindowActivate( BOOL fActive)
{
	return E_NOTIMPL;
}

STDMETHODIMP TOleControlSite::XHostUIHandler::EnableModeless( BOOL fEnable)
{
	return E_NOTIMPL;
}

STDMETHODIMP TOleControlSite::XHostUIHandler::UpdateUI()
{
	return S_OK;
}

STDMETHODIMP TOleControlSite::XHostUIHandler::HideUI()
{
	return S_OK;
}

STDMETHODIMP TOleControlSite::XHostUIHandler::FilterDataObject( IDataObject __RPC_FAR *pDO,
															    IDataObject __RPC_FAR *__RPC_FAR *ppDORet)
{
	return E_NOTIMPL;
}

STDMETHODIMP TOleControlSite::XHostUIHandler::GetDropTarget( IDropTarget __RPC_FAR *pDropTarget,
															 IDropTarget __RPC_FAR *__RPC_FAR *ppDropTarget)
{
	return E_NOTIMPL;
}

STDMETHODIMP TOleControlSite::XHostUIHandler::GetOptionKeyPath( LPOLESTR __RPC_FAR *pchKey,
															    DWORD dwReserved)
{
	return E_NOTIMPL;
}

STDMETHODIMP TOleControlSite::XHostUIHandler::ResizeBorder( LPCRECT prcBorder,
														    IOleInPlaceUIWindow __RPC_FAR *pUIWindow,
															BOOL fFrameWindow)
{
	return E_NOTIMPL;
}

STDMETHODIMP TOleControlSite::XHostUIHandler::ShowContextMenu( DWORD dwID,
															   POINT __RPC_FAR *ppt,
															   IUnknown __RPC_FAR *pcmdtReserved,
															   IDispatch __RPC_FAR *pdispReserved)
{
	return S_OK;
}

STDMETHODIMP TOleControlSite::XHostUIHandler::ShowUI( DWORD dwID,
													  IOleInPlaceActiveObject __RPC_FAR *pActiveObject,
													  IOleCommandTarget __RPC_FAR *pCommandTarget,
													  IOleInPlaceFrame __RPC_FAR *pFrame,
													  IOleInPlaceUIWindow __RPC_FAR *pDoc)
{
	return S_OK;
}

STDMETHODIMP TOleControlSite::XHostUIHandler::TranslateAccelerator( LPMSG lpMsg,
																    const GUID __RPC_FAR *pguidCmdGroup,
																	DWORD nCmdID)
{
	return E_NOTIMPL;
}

STDMETHODIMP TOleControlSite::XHostUIHandler::TranslateUrl( DWORD dwTranslate,
														    OLECHAR __RPC_FAR *pchURLIn,
															OLECHAR __RPC_FAR *__RPC_FAR *ppchURLOut)
{
	return E_NOTIMPL;
}
