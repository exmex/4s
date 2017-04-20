/**********************************************************************
 *<
	FILE: DllEntry.cpp

	DESCRIPTION: Tachyon Engine Mesh Data Export Plugin Dll Entry

	CREATED BY: Hyunho, Shin

	HISTORY: Created At 2002/2/19

 *>	Copyright (c) 2000, All Rights Reserved.
 **********************************************************************/
#include "ExportTMF.h"

extern ClassDesc2* GetCExportTMFDesc();

HINSTANCE hInstance;
int controlsInit = FALSE;


BOOL WINAPI DllMain( HINSTANCE hinstDLL, ULONG fdwReason, LPVOID lpvReserved)
{
	hInstance = hinstDLL;

	if(!controlsInit)
	{
		controlsInit = TRUE;
		InitCustomControls(hInstance);
		InitCommonControls();
	}

	return TRUE;
}

__declspec( dllexport ) const TCHAR* LibDescription()
{
	return GetString(IDS_LIBDESCRIPTION);
}

//TODO: Must change this number when adding a new class
__declspec( dllexport ) int LibNumberClasses()
{
	return 1;
}

__declspec( dllexport ) ClassDesc* LibClassDesc( int i)
{
	switch(i)
	{
	case 0	: return GetCExportTMFDesc();
	default	: return 0;
	}
}

__declspec( dllexport ) ULONG LibVersion()
{
	return VERSION_3DSMAX;
}

TCHAR *GetString( int id)
{
	static TCHAR buf[256];

	if(hInstance)
		return LoadString(hInstance, id, buf, sizeof(buf)) ? buf : NULL;

	return NULL;
}