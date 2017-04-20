#include "StdAfx.h"


CTNICEventSink::CTNICEventSink()
{
	m_pIWBEM = NULL;
	m_nRef = 1;
}

CTNICEventSink::~CTNICEventSink()
{
	ReleaseTEVENT();
}

STDMETHODIMP CTNICEventSink::QueryInterface( REFIID riid, LPVOID *ppv)
{
	*ppv = NULL;

	if( riid == IID_IUnknown || riid == IID_IWbemObjectSink )
	{
		*ppv = (IWbemObjectSink *) this;
		AddRef();

		return NOERROR;
	}

	return E_NOINTERFACE;
}

ULONG CTNICEventSink::AddRef()
{
	m_nRef++;
	return m_nRef;
}

ULONG CTNICEventSink::Release()
{
	m_nRef--;

	if(!m_nRef)
	{
		delete this;
		return 0;
	}

	return m_nRef;
}

HRESULT CTNICEventSink::Indicate( long lObjectCount, IWbemClassObject **ppObjArray)
{
	for( long i=0; i<lObjectCount; i++)
	{
		VARIANT vCLASS;
		VARIANT vINST;

		if( SUCCEEDED(ppObjArray[i]->Get( L"__CLASS", 0, &vCLASS, NULL, NULL)) &&
			!wcscmp( vCLASS.bstrVal, L"__InstanceModificationEvent") &&
			SUCCEEDED(ppObjArray[i]->Get( L"TargetInstance", 0, &vINST, NULL, NULL)) )
		{
			IWbemClassObject *pIWbemObj = (IWbemClassObject *) vINST.punkVal;

			VARIANT vTSTATUS;
			VARIANT vTCLASS;

			if( SUCCEEDED(pIWbemObj->Get( L"__CLASS", 0, &vTCLASS, NULL, NULL)) &&
				!wcscmp( vTCLASS.bstrVal, L"Win32_NetworkAdapter") &&
				SUCCEEDED(pIWbemObj->Get( L"NetConnectionStatus", 0, &vTSTATUS, NULL, NULL)) &&
				vTSTATUS.lVal != 2 )
				OnMediaDisconnect(pIWbemObj);

			VariantClear(&vTSTATUS);
			VariantClear(&vTCLASS);
		}

		VariantClear(&vCLASS);
		VariantClear(&vINST);
	}

	return WBEM_NO_ERROR;
}

HRESULT CTNICEventSink::SetStatus( long lFlags,
								   HRESULT hResult,
								   BSTR strParam,
								   IWbemClassObject *pObjParam)
{
	return WBEM_NO_ERROR;
}

void CTNICEventSink::OnMediaDisconnect( IWbemClassObject *pObj)
{
}

void CTNICEventSink::ReleaseTEVENT()
{
	if(m_pIWBEM)
	{
		m_pIWBEM->CancelAsyncCall(this);
		m_pIWBEM->Release();
		m_pIWBEM = NULL;
	}
}

BYTE CTNICEventSink::InitTEVENT( CString& strIP, int nTimer)
{
	IEnumWbemClassObject *pIEnumObj = NULL;
	IWbemClassObject *pIWbemObj = NULL;
	IWbemLocator *pIWbemLoc = NULL;
	ReleaseTEVENT();

	HRESULT hr = CoCreateInstance(
		CLSID_WbemLocator,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator,
		(LPVOID *) &pIWbemLoc);

	if(FAILED(hr))
		return FALSE;

	hr = pIWbemLoc->ConnectServer(
		L"root\\cimv2",
		NULL,
		NULL, 0,
		NULL,
		0, 0,
		&m_pIWBEM);
	pIWbemLoc->Release();

	if(FAILED(hr))
		return FALSE;

	hr = CoSetProxyBlanket(
		m_pIWBEM,
		RPC_C_AUTHN_WINNT,
		RPC_C_AUTHZ_NONE,
		NULL,
		RPC_C_AUTHN_LEVEL_CALL,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL,
		EOAC_NONE);

	if(FAILED(hr))
	{
		ReleaseTEVENT();
		return FALSE;
	}

	BYTE vTMACADDR[MAX_ADAPTER_ADDRESS_LENGTH];
	ULONG nCount = GetMACAddress(
		vTMACADDR,
		strIP);

	if(!nCount)
	{
		ReleaseTEVENT();
		return FALSE;
	}

	CStringW szWQL(L"SELECT * FROM Win32_NetworkAdapter WHERE NetConnectionStatus = 2 AND MACAddress = '");
	for( ULONG i=0; i<nCount; i++)
		szWQL.AppendFormat( L"%02X:", vTMACADDR[i]);

	szWQL.TrimRight(L":");
	szWQL.Append(L"'");

	hr = m_pIWBEM->ExecQuery(
		L"WQL",
		(LPWSTR) LPCWSTR(szWQL),
		WBEM_FLAG_RETURN_IMMEDIATELY|
		WBEM_FLAG_FORWARD_ONLY,
		NULL,
		&pIEnumObj);

	if(FAILED(hr))
	{
		ReleaseTEVENT();
		return FALSE;
	}

	CStringW szDeviceID;
	szDeviceID.Empty();

	hr = pIEnumObj->Next( WBEM_INFINITE, 1, &pIWbemObj, &nCount);
	while( SUCCEEDED(hr) && nCount )
	{
		VARIANT vDeviceID;

		pIWbemObj->Get( L"DeviceID", 0, &vDeviceID, NULL, NULL);
		szDeviceID.Format( L"%s", vDeviceID.bstrVal);
		VariantClear(&vDeviceID);

		pIWbemObj->Release();
		hr = pIEnumObj->Next( WBEM_INFINITE, 1, &pIWbemObj, &nCount);
	}
	pIEnumObj->Release();

	if(szDeviceID.IsEmpty())
	{
		ReleaseTEVENT();
		return FALSE;
	}

	szWQL.Format(
		L"SELECT * FROM __InstanceModificationEvent WITHIN %d WHERE TargetInstance ISA 'Win32_NetworkAdapter' AND TargetInstance.DeviceID = '%s'",
		nTimer,
		szDeviceID);

	hr = m_pIWBEM->ExecNotificationQueryAsync(
		L"WQL",
		(LPWSTR) LPCWSTR(szWQL), 0,
		NULL,
		this);

	if(FAILED(hr))
	{
		ReleaseTEVENT();
		return FALSE;
	}

	return TRUE;
}

BYTE CTNICEventSink::GetMACAddress( LPBYTE pMACAddress,
								    CString& strIP)
{
	LPBYTE pTHEAP = NULL;
	BYTE bRESULT = 0;
	ULONG nLEN = 0;

	if( GetAdaptersInfo( NULL, &nLEN) == ERROR_BUFFER_OVERFLOW && nLEN )
	{
		pTHEAP = new BYTE[nLEN];

		if(pTHEAP)
		{
			PIP_ADAPTER_INFO pTINFO = (PIP_ADAPTER_INFO) pTHEAP;

			if( GetAdaptersInfo( pTINFO, &nLEN) == NO_ERROR )
				while(pTINFO)
				{
					PIP_ADDR_STRING pIP = &pTINFO->IpAddressList;

					while(pIP)
					{
						if(!strcmp( pIP->IpAddress.String, LPCSTR(strIP)))
						{
							memcpy( pMACAddress, pTINFO->Address, pTINFO->AddressLength);
							bRESULT = pTINFO->AddressLength;

							delete[] pTHEAP;
							pTHEAP = NULL;

							return bRESULT;
						}

						pIP = pIP->Next;
					}

					pTINFO = pTINFO->Next;
				}

			delete[] pTHEAP;
			pTHEAP = NULL;
		}
	}

	return bRESULT;
}
