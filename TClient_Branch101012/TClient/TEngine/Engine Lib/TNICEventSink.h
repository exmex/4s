#pragma once


class CTNICEventSink : public IWbemObjectSink
{
protected:
	virtual void OnMediaDisconnect( IWbemClassObject *pObj);

public:
	void ReleaseTEVENT();

	BYTE InitTEVENT(
		CString& strIP,
		int nTimer);

	BYTE GetMACAddress(
		LPBYTE pMACAddress,
		CString& strIP);

public:
	STDMETHOD(SetStatus)( long lFlags, HRESULT hResult, BSTR strParam, IWbemClassObject *pObjParam);
	STDMETHOD(Indicate)( long lObjectCount, IWbemClassObject **ppObjArray);
	STDMETHOD(QueryInterface)( REFIID riid, LPVOID *ppv);

	STDMETHOD_(ULONG, Release)();
	STDMETHOD_(ULONG, AddRef)();

protected:
	IWbemServices *m_pIWBEM;

private:
	ULONG m_nRef;

public:
	CTNICEventSink();
	~CTNICEventSink();
};
