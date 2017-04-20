#include "Stdafx.h"
#include "TSystemInfo.h"

// =====================================================================
CTSystemInfo::CTSystemInfo()
{
}
// =====================================================================

// =====================================================================
void CTSystemInfo::InitInfo()
{
	HRESULT hr;

	bool bGotDirectXVersion = false;
    IDxDiagProvider* pDxDiagProvider = NULL;
    hr = CoCreateInstance(
		CLSID_DxDiagProvider,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IDxDiagProvider,
		(LPVOID*) &pDxDiagProvider);

    if( SUCCEEDED(hr) )
    {
        DXDIAG_INIT_PARAMS dxDiagInitParam;
        ZeroMemory( &dxDiagInitParam, sizeof(DXDIAG_INIT_PARAMS) );
        dxDiagInitParam.dwSize                  = sizeof(DXDIAG_INIT_PARAMS);
        dxDiagInitParam.dwDxDiagHeaderVersion   = DXDIAG_DX9_SDK_VERSION;
        dxDiagInitParam.bAllowWHQLChecks        = false;
        dxDiagInitParam.pReserved               = NULL;

        hr = pDxDiagProvider->Initialize( &dxDiagInitParam );
        if( SUCCEEDED(hr) )
        {
            IDxDiagContainer* pDxDiagRoot = NULL;
            hr = pDxDiagProvider->GetRootContainer( &pDxDiagRoot );
            if( SUCCEEDED(hr) )
            {	
				InitSystemInfo(pDxDiagRoot);  
				InitDisplayInfo(pDxDiagRoot);

                pDxDiagRoot->Release();
            }
        }

        pDxDiagProvider->Release();
    }
}
// =====================================================================
CString CTSystemInfo::GetInfo() const
{
	CString result;

	result  = "====================================\n";
	result += "System Information\n";
	result += "------------------------------------\n";
	result += "Machine : " + m_strMachineName + "\n";
	result += "Language : " + m_strLanguages + "\n";
	result += "Processor : " + m_strProcessor + "\n";
	result += "Memory : " + m_strPhysicalMemory + "\n";
	result += "BIOS : " + m_strBIOS + "\n";
	result += "OS : " + m_strOSEnglish + "\n";
	result += "DirectX Version : " + m_strDirectXVersion + "\n";
	result += "====================================\n";
	result += "Display Information\n";
	result += "------------------------------------\n";
	result += "Device : " + m_strDeviceName + "\n";
	result += "Chip Type : " + m_strChipType + "\n";
	result += "Display Memory : " + m_strDisplayMemory + "\n";
	result += "Driver : " + m_strDriverName + "\n";
	result += "Driver Version : " + m_strDriverVersion + "\n";
	result += "====================================\n";

	return result;
}
// =====================================================================

// =====================================================================
void CTSystemInfo::InitSystemInfo(IDxDiagContainer* pDxDiagRoot)
{
	HRESULT hr;
	IDxDiagContainer* pDxDiagSystemInfo = NULL;

    hr = pDxDiagRoot->GetChildContainer( L"DxDiag_SystemInfo", &pDxDiagSystemInfo );
    if( SUCCEEDED(hr) )
    {
        VARIANT var;
        VariantInit( &var );

        hr = pDxDiagSystemInfo->GetProp( L"szMachineNameLocalized", &var );
        if( SUCCEEDED(hr) && var.vt == VT_BSTR && SysStringLen( var.bstrVal ) != 0 )
		{
			char temp[256];
            WideCharToMultiByte(CP_ACP, 0, var.bstrVal, -1, temp, sizeof(temp), NULL, NULL);
			m_strMachineName = temp;
		}
        VariantClear( &var );

		hr = pDxDiagSystemInfo->GetProp( L"szOSEnglish", &var );
        if( SUCCEEDED(hr) && var.vt == VT_BSTR && SysStringLen( var.bstrVal ) != 0 )
			m_strOSEnglish = var.bstrVal;
        VariantClear( &var );

		hr = pDxDiagSystemInfo->GetProp( L"szLanguagesEnglish", &var );
        if( SUCCEEDED(hr) && var.vt == VT_BSTR && SysStringLen( var.bstrVal ) != 0 )
			m_strLanguages = var.bstrVal;
        VariantClear( &var );

		hr = pDxDiagSystemInfo->GetProp( L"szBIOSEnglish", &var );
        if( SUCCEEDED(hr) && var.vt == VT_BSTR && SysStringLen( var.bstrVal ) != 0 )
			m_strBIOS = var.bstrVal;
        VariantClear( &var );

		hr = pDxDiagSystemInfo->GetProp( L"szPhysicalMemoryEnglish", &var );
        if( SUCCEEDED(hr) && var.vt == VT_BSTR && SysStringLen( var.bstrVal ) != 0 )
			m_strPhysicalMemory = var.bstrVal;
        VariantClear( &var );

		hr = pDxDiagSystemInfo->GetProp( L"szProcessorEnglish", &var );
        if( SUCCEEDED(hr) && var.vt == VT_BSTR && SysStringLen( var.bstrVal ) != 0 )
			m_strProcessor = var.bstrVal;
        VariantClear( &var );

		DWORD dwDXMaj=0;
		hr = pDxDiagSystemInfo->GetProp( L"dwDirectXVersionMajor", &var );
        if( SUCCEEDED(hr) && var.vt == VT_UI4 )
			dwDXMaj = var.ulVal;
        VariantClear( &var );

		DWORD dwDXMin=0;
		hr = pDxDiagSystemInfo->GetProp( L"dwDirectXVersionMinor", &var );
        if( SUCCEEDED(hr) && var.vt == VT_UI4 )
			dwDXMin = var.ulVal;
        VariantClear( &var );

		CHAR cDXLetter = ' ';
		hr = pDxDiagSystemInfo->GetProp( L"szDirectXVersionLetter", &var );
        if( SUCCEEDED(hr) && var.vt == VT_BSTR && SysStringLen( var.bstrVal ) != 0 )
		{
			char temp[10];
			WideCharToMultiByte( CP_ACP, 0, var.bstrVal, -1, temp, sizeof(temp), NULL, NULL );
            cDXLetter = temp[0];
		}
        VariantClear( &var );

		CString strDXNum;
		hr = pDxDiagSystemInfo->GetProp( L"szDirectXVersionEnglish", &var );
        if( SUCCEEDED(hr) && var.vt == VT_BSTR && SysStringLen( var.bstrVal ) != 0 )
            strDXNum = var.bstrVal;
        VariantClear( &var );

		if(cDXLetter == ' ' || cDXLetter == 0)
			m_strDirectXVersion.Format("DirectX %d.%d", dwDXMaj,dwDXMin);
		else
			m_strDirectXVersion.Format("DirectX %d.%d%c", dwDXMaj,dwDXMin,cDXLetter);
		m_strDirectXVersion += " (" + strDXNum + ")";

		pDxDiagSystemInfo->Release();
	}
}
// =====================================================================
void CTSystemInfo::InitDisplayInfo(IDxDiagContainer* pDxDiagRoot)
{
	HRESULT hr;
	IDxDiagContainer* pDxDiagDevicesInfo = NULL;
	IDxDiagContainer* pDxDiag1stDeviInfo = NULL;

    hr = pDxDiagRoot->GetChildContainer( L"DxDiag_DisplayDevices", &pDxDiagDevicesInfo );
    if( SUCCEEDED(hr) )
    {
		hr = pDxDiagDevicesInfo->GetChildContainer( L"0", &pDxDiag1stDeviInfo );
		if( SUCCEEDED(hr) )
		{
			VARIANT var;
			VariantInit( &var );

			hr = pDxDiag1stDeviInfo->GetProp( L"szDescription", &var );
			if( SUCCEEDED(hr) && var.vt == VT_BSTR && SysStringLen( var.bstrVal ) != 0 )
				m_strDeviceName = var.bstrVal;
			VariantClear( &var );

			hr = pDxDiag1stDeviInfo->GetProp( L"szChipType", &var );
			if( SUCCEEDED(hr) && var.vt == VT_BSTR && SysStringLen( var.bstrVal ) != 0 )
				m_strChipType = var.bstrVal;
			VariantClear( &var );

			hr = pDxDiag1stDeviInfo->GetProp( L"szDisplayMemoryEnglish", &var );
			if( SUCCEEDED(hr) && var.vt == VT_BSTR && SysStringLen( var.bstrVal ) != 0 )
				m_strDisplayMemory = var.bstrVal;
			VariantClear( &var );

			hr = pDxDiag1stDeviInfo->GetProp( L"szDriverName", &var );
			if( SUCCEEDED(hr) && var.vt == VT_BSTR && SysStringLen( var.bstrVal ) != 0 )
				m_strDriverName = var.bstrVal;
			VariantClear( &var );

			hr = pDxDiag1stDeviInfo->GetProp( L"szDriverVersion", &var );
			if( SUCCEEDED(hr) && var.vt == VT_BSTR && SysStringLen( var.bstrVal ) != 0 )
				m_strDriverVersion = var.bstrVal;
			VariantClear( &var );

			pDxDiag1stDeviInfo->Release();
		}

        pDxDiagDevicesInfo->Release();
    }
}
