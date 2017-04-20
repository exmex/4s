#include "StdAfx.h"
#include "TClient.h"


CTClientKEY::CTClientKEY()
{
	m_point = CPoint( 0, 0);
	m_dwTick = 0;

	ResetSTATE();
}

CTClientKEY::~CTClientKEY()
{
}

void CTClientKEY::ResetSTATE()
{
	memset( m_vSTATE, 0, TKEY_COUNT * sizeof(TKEY_STATE));
	memset( m_vTICK, 0, TKEY_COUNT * sizeof(DWORD));
}

WORD CTClientKEY::GetCurMOD()
{
	WORD wRES = 0;

	if( GetCTRL() )
		wRES |= TKEYMOD_CTRL;
	if( GetALT() )
		wRES |= TKEYMOD_ALT;
	if( GetSHIFT() )
		wRES |= TKEYMOD_SHIFT;

	return wRES;
}

BYTE CTClientKEY::GetCTRL()
{
	return GetKeyState(VK_CONTROL) < 0 ? 0x01 : 0x00;
}

BYTE CTClientKEY::GetALT()
{
	return GetKeyState(VK_MENU) < 0 ? 0x01 : 0x00;
}

BYTE CTClientKEY::GetSHIFT()
{
	return GetKeyState(VK_SHIFT) < 0 ? 0x01 : 0x00;
}

BYTE CTClientKEY::IsKeyDown(BYTE bVKey)
{
	return GetAsyncKeyState(bVKey) < 0 ? 0x01 : 0x00;
}