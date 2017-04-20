#include "stdafx.h"

BYTE CTachyonMedia::m_bMasterVolume = VOLUME_MAX;
BYTE CTachyonMedia::m_bBACK = FALSE;
BYTE CTachyonMedia::m_bON = TRUE;


CTachyonMedia::CTachyonMedia()
{
	m_mapDSOUND.clear();
	m_mapDMUSIC.clear();
	m_mapDSHOW.clear();

	m_pDS = NULL;
}

CTachyonMedia::~CTachyonMedia()
{
	ReleaseAll();
}

BYTE CTachyonMedia::InitMEDIA( HWND hWnd,
							   DWORD dwCoopLevel,
							   DWORD dwPrimaryChannels,
							   DWORD dwPrimaryFreq,
							   DWORD dwPrimaryBitRate)
{
	LPDIRECTSOUNDBUFFER pDSB = NULL;
	DS3DLISTENER vLISTENER;

	if(m_pDS)
	{
		m_pDS->Release();
		m_pDS = NULL;
	}

	if(FAILED(DirectSoundCreate8( NULL, &m_pDS, NULL)))
		return FALSE;

	if(FAILED(m_pDS->SetCooperativeLevel( hWnd, dwCoopLevel)))
		return FALSE;

	m_pDS->SetSpeakerConfig( DSSPEAKER_COMBINED(
		DSSPEAKER_STEREO,
		DSSPEAKER_GEOMETRY_WIDE));

	DSBUFFERDESC desc;
	ZeroMemory( &desc, sizeof(DSBUFFERDESC));
	desc.dwSize = sizeof(DSBUFFERDESC);
	desc.dwFlags = DSBCAPS_CTRL3D|DSBCAPS_PRIMARYBUFFER;
	desc.dwBufferBytes = 0;
	desc.lpwfxFormat = NULL;

	if(FAILED(m_pDS->CreateSoundBuffer( &desc, &pDSB, NULL)))
		return FALSE;

	WAVEFORMATEX wfx;
	ZeroMemory( &wfx, sizeof(WAVEFORMATEX));
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nChannels = (WORD) dwPrimaryChannels;
	wfx.nSamplesPerSec = dwPrimaryFreq;
	wfx.wBitsPerSample = (WORD) dwPrimaryBitRate;
	wfx.nBlockAlign = (WORD) (wfx.wBitsPerSample / 8 * wfx.nChannels);
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

	if(FAILED(pDSB->QueryInterface( IID_IDirectSound3DListener8, (LPVOID *) &CD3DSound::m_pLISTENER)))
		return FALSE;

	if(FAILED(pDSB->SetFormat(&wfx)))
		return FALSE;

	CD3DSound::GetListener(&vLISTENER);
	pDSB->Release();

	vLISTENER.vOrientFront = D3DXVECTOR3( 0.0f, 0.0f, 1.0f);
	vLISTENER.vOrientTop = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
	vLISTENER.vPosition = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	CD3DSound::SetListener( &vLISTENER, DS3D_IMMEDIATE);

	CD3DSound::InitGARBAGE();
	CD3DSound::ResetLISTENER(
		&D3DXVECTOR3(vLISTENER.vPosition),
		&D3DXVECTOR3(vLISTENER.vOrientFront),
		&D3DXVECTOR3(vLISTENER.vOrientTop));
	CT3DMusic::DecodeINIT(m_pDS);

	return TRUE;
}

void CTachyonMedia::ReleaseAll()
{
	MAPMEDIA::iterator it;

	for( it = m_mapDMUSIC.begin(); it != m_mapDMUSIC.end(); it++)
		if((*it).second)
			delete ((CT3DMusic *) (*it).second);

	for( it = m_mapDSOUND.begin(); it != m_mapDSOUND.end(); it++)
		if((*it).second)
			delete ((CD3DSound *) (*it).second);

	for( it = m_mapDSHOW.begin(); it != m_mapDSHOW.end(); it++)
		if((*it).second)
			delete ((CTShow *) (*it).second);

	CD3DSound::ClearGARBAGE();
	m_mapDMUSIC.clear();
	m_mapDSOUND.clear();
	m_mapDSHOW.clear();

	if(CD3DSound::m_pLISTENER)
	{
		CD3DSound::m_pLISTENER->Release();
		CD3DSound::m_pLISTENER = NULL;
	}

	if(m_pDS)
	{
		m_pDS->Release();
		m_pDS = NULL;
	}
}

LPVOID CTachyonMedia::GetData( BYTE bType, DWORD dwID)
{
	switch(bType)
	{
	case MEDIA_TMUSIC	:
		{
			MAPMEDIA::iterator finder = m_mapDMUSIC.find(dwID);

			if( finder != m_mapDMUSIC.end() )
				return (*finder).second;
		}

		break;

	case MEDIA_TSOUND	:
		{
			MAPMEDIA::iterator finder = m_mapDSOUND.find(dwID);

			if( finder != m_mapDSOUND.end() )
				return (*finder).second;
		}

		break;

	case MEDIA_TSHOW	:
		{
			MAPMEDIA::iterator finder = m_mapDSHOW.find(dwID);

			if( finder != m_mapDSHOW.end() )
				return (*finder).second;
		}

		break;
	}

	return NULL;
}

void CTachyonMedia::Unregister( BYTE bType, DWORD dwID)
{
	switch(bType)
	{
	case MEDIA_TMUSIC	:
		{
			MAPMEDIA::iterator finder = m_mapDMUSIC.find(dwID);

			if( finder != m_mapDMUSIC.end() )
			{
				delete ((CT3DMusic *) (*finder).second);
				m_mapDMUSIC.erase(finder);
			}
		}

		break;

	case MEDIA_TSOUND	:
		{
			MAPMEDIA::iterator finder = m_mapDSOUND.find(dwID);

			if( finder != m_mapDSOUND.end() )
			{
				delete ((CD3DSound *) (*finder).second);
				m_mapDSOUND.erase(finder);
			}
		}

		break;

	case MEDIA_TSHOW	:
		{
			MAPMEDIA::iterator finder = m_mapDSHOW.find(dwID);

			if( finder != m_mapDSHOW.end() )
			{
				delete ((CTShow *) (*finder).second);
				m_mapDSHOW.erase(finder);
			}
		}

		break;
	}
}

void CTachyonMedia::Register( BYTE bType,
							  DWORD dwID,
							  LPVOID pItem)
{
	switch(bType)
	{
	case MEDIA_TMUSIC	:
		{
			m_mapDMUSIC.insert( MAPMEDIA::value_type(
				dwID,
				pItem));
		}

		break;

	case MEDIA_TSOUND	:
		{
			m_mapDSOUND.insert( MAPMEDIA::value_type(
				dwID,
				pItem));
		}

		break;

	case MEDIA_TSHOW	:
		{
			m_mapDSHOW.insert( MAPMEDIA::value_type(
				dwID,
				pItem));
		}

		break;
	}
}

void CTachyonMedia::ResetVolume()
{
	MAPMEDIA::iterator it;

	for( it = m_mapDMUSIC.begin(); it != m_mapDMUSIC.end(); it++)
		((CT3DMusic *) (*it).second)->ResetVolume();

	for( it = m_mapDSOUND.begin(); it != m_mapDSOUND.end(); it++)
		((CD3DSound *) (*it).second)->ResetVolume();

	for( it = m_mapDSHOW.begin(); it != m_mapDSHOW.end(); it++)
		((CTShow *) (*it).second)->ResetVolume();
}

void CTachyonMedia::ResetVolume( BYTE bType)
{
	switch(bType)
	{
	case MEDIA_TMUSIC	:
		{
			MAPMEDIA::iterator it;

			for( it = m_mapDMUSIC.begin(); it != m_mapDMUSIC.end(); it++)
				((CT3DMusic *) (*it).second)->ResetVolume();
		}

		break;

	case MEDIA_TSOUND	:
		{
			MAPMEDIA::iterator it;

			for( it = m_mapDSOUND.begin(); it != m_mapDSOUND.end(); it++)
				((CD3DSound *) (*it).second)->ResetVolume();
		}

		break;

	case MEDIA_TSHOW	:
		{
			MAPMEDIA::iterator it;

			for( it = m_mapDSHOW.begin(); it != m_mapDSHOW.end(); it++)
				((CTShow *) (*it).second)->ResetVolume();
		}

		break;
	}
}

void CTachyonMedia::ResetVolume( BYTE bType, DWORD dwID)
{
	switch(bType)
	{
	case MEDIA_TMUSIC	:
		{
			MAPMEDIA::iterator finder = m_mapDMUSIC.find(dwID);

			if( finder != m_mapDMUSIC.end() )
				((CT3DMusic *) (*finder).second)->ResetVolume();
		}

		break;

	case MEDIA_TSOUND	:
		{
			MAPMEDIA::iterator finder = m_mapDSOUND.find(dwID);

			if( finder != m_mapDSOUND.end() )
				((CD3DSound *) (*finder).second)->ResetVolume();
		}

		break;

	case MEDIA_TSHOW	:
		{
			MAPMEDIA::iterator finder = m_mapDSHOW.find(dwID);

			if( finder != m_mapDSHOW.end() )
				((CTShow *) (*finder).second)->ResetVolume();
		}

		break;
	}
}

void CTachyonMedia::ResetVolume( BYTE bType, DWORD dwID, int nIndex)
{
	switch(bType)
	{
	case MEDIA_TMUSIC	:
		{
			MAPMEDIA::iterator finder = m_mapDMUSIC.find(dwID);

			if( finder != m_mapDMUSIC.end() )
				((CT3DMusic *) (*finder).second)->ResetVolume();
		}

		break;

	case MEDIA_TSOUND	:
		{
			MAPMEDIA::iterator finder = m_mapDSOUND.find(dwID);

			if( finder != m_mapDSOUND.end() )
				((CD3DSound *) (*finder).second)->ResetVolume(nIndex);
		}

		break;

	case MEDIA_TSHOW	:
		{
			MAPMEDIA::iterator finder = m_mapDSHOW.find(dwID);

			if( finder != m_mapDSHOW.end() )
				((CTShow *) (*finder).second)->ResetVolume();
		}

		break;
	}
}

void CTachyonMedia::LoadDShow( CWnd *pHost,
							   CWnd *pWnd,
							   DWORD dwID,
							   DWORD dwMessage,
							   LPTSTR szFileName,
							   BYTE bVolume)
{
	CTShow *pItem = new CTShow();

	pItem->InitDShow(
		pHost,
		pWnd,
		szFileName,
		dwMessage);
	pItem->m_bVolume = bVolume;

	Register( MEDIA_TSHOW, dwID, (LPVOID) pItem);
}

void CTachyonMedia::LoadDMusic( DWORD dwID,
							    LPTSTR szFileName,
								BYTE bVolume)
{
	CT3DMusic *pItem = new CT3DMusic();

	pItem->InitMusic(szFileName);
	pItem->m_bVolume = bVolume;
	Register( MEDIA_TMUSIC, dwID, pItem);
}

void CTachyonMedia::LoadDSound( DWORD dwID,
								LPTSTR szFileName,
								BYTE bVolume)
{
	CD3DSound *pItem = new CD3DSound();

	pItem->Initialize(
		m_pDS,
		szFileName);
	pItem->m_bVolume = bVolume;

	Register( MEDIA_TSOUND, dwID, pItem);
}

void CTachyonMedia::SetPos( BYTE bType,
						    DWORD dwID,
							int nIndex,
							LONGLONG nPos)
{
	switch(bType)
	{
	case MEDIA_TMUSIC	:
		{
			MAPMEDIA::iterator finder = m_mapDMUSIC.find(dwID);

			if( finder != m_mapDMUSIC.end() )
				((CT3DMusic *) (*finder).second)->SetPos((MUSIC_TIME) nPos);
		}

		break;

	case MEDIA_TSOUND	:
		{
			MAPMEDIA::iterator finder = m_mapDSOUND.find(dwID);

			if( finder != m_mapDSOUND.end() )
				((CD3DSound *) (*finder).second)->SetPos( nIndex, (DWORD) nPos);
		}

		break;

	case MEDIA_TSHOW	:
		{
			MAPMEDIA::iterator finder = m_mapDSHOW.find(dwID);

			if( finder != m_mapDSHOW.end() )
				((CTShow *) (*finder).second)->SetPos(nPos);
		}

		break;
	}
}

LONGLONG CTachyonMedia::GetLength( BYTE bType, DWORD dwID)
{
	switch(bType)
	{
	case MEDIA_TMUSIC	:
		{
			MAPMEDIA::iterator finder = m_mapDMUSIC.find(dwID);

			if( finder != m_mapDMUSIC.end() )
				return ((CT3DMusic *) (*finder).second)->GetLength();
		}

		break;

	case MEDIA_TSOUND	:
		{
			MAPMEDIA::iterator finder = m_mapDSOUND.find(dwID);

			if( finder != m_mapDSOUND.end() )
				return ((CD3DSound *) (*finder).second)->GetLength();
		}

		break;

	case MEDIA_TSHOW	:
		{
			MAPMEDIA::iterator finder = m_mapDSHOW.find(dwID);

			if( finder != m_mapDSHOW.end() )
				return ((CTShow *) (*finder).second)->GetLength();
		}

		break;
	}

	return 0;
}

LONGLONG CTachyonMedia::GetPos( BYTE bType, DWORD dwID, int nIndex)
{
	switch(bType)
	{
	case MEDIA_TMUSIC	:
		{
			MAPMEDIA::iterator finder = m_mapDMUSIC.find(dwID);

			if( finder != m_mapDMUSIC.end() )
				return ((CT3DMusic *) (*finder).second)->GetPos();
		}

		break;

	case MEDIA_TSOUND	:
		{
			MAPMEDIA::iterator finder = m_mapDSOUND.find(dwID);

			if( finder != m_mapDSOUND.end() )
				return ((CD3DSound *) (*finder).second)->GetPos(nIndex);
		}

		break;

	case MEDIA_TSHOW	:
		{
			MAPMEDIA::iterator finder = m_mapDSHOW.find(dwID);

			if( finder != m_mapDSHOW.end() )
				return ((CTShow *) (*finder).second)->GetPos();
		}

		break;
	}

	return 0;
}

void CTachyonMedia::Toggle( BYTE bType, DWORD dwID)
{
	switch(bType)
	{
	case MEDIA_TSHOW	:
		{
			MAPMEDIA::iterator finder = m_mapDSHOW.find(dwID);

			if( finder != m_mapDSHOW.end() )
				((CTShow *) (*finder).second)->Toggle();
		}

		break;
	}
}

void CTachyonMedia::Pause( BYTE bType, DWORD dwID, int nIndex)
{
	switch(bType)
	{
	case MEDIA_TMUSIC	:
		{
			MAPMEDIA::iterator finder = m_mapDMUSIC.find(dwID);

			if( finder != m_mapDMUSIC.end() )
				((CT3DMusic *) (*finder).second)->Pause();
		}

		break;

	case MEDIA_TSOUND	:
		{
			MAPMEDIA::iterator finder = m_mapDSOUND.find(dwID);

			if( finder != m_mapDSOUND.end() )
				((CD3DSound *) (*finder).second)->Pause(nIndex);
		}

		break;

	case MEDIA_TSHOW	:
		{
			MAPMEDIA::iterator finder = m_mapDSHOW.find(dwID);

			if( finder != m_mapDSHOW.end() )
				((CTShow *) (*finder).second)->Pause();
		}

		break;
	}
}

void CTachyonMedia::Play( BYTE bType, DWORD dwID, int nIndex)
{
	switch(bType)
	{
	case MEDIA_TMUSIC	:
		{
			MAPMEDIA::iterator finder = m_mapDMUSIC.find(dwID);

			if( finder != m_mapDMUSIC.end() )
				((CT3DMusic *) (*finder).second)->Play();
		}

		break;

	case MEDIA_TSOUND	:
		{
			MAPMEDIA::iterator finder = m_mapDSOUND.find(dwID);

			if( finder != m_mapDSOUND.end() )
				((CD3DSound *) (*finder).second)->Play(nIndex);
		}

		break;

	case MEDIA_TSHOW	:
		{
			MAPMEDIA::iterator finder = m_mapDSHOW.find(dwID);

			if( finder != m_mapDSHOW.end() )
				((CTShow *) (*finder).second)->Play();
		}

		break;
	}
}

int CTachyonMedia::Play( BYTE bType, DWORD dwID)
{
	switch(bType)
	{
	case MEDIA_TMUSIC	:
		{
			MAPMEDIA::iterator finder = m_mapDMUSIC.find(dwID);

			if( finder != m_mapDMUSIC.end() )
				((CT3DMusic *) (*finder).second)->Play();
		}

		break;

	case MEDIA_TSOUND	:
		{
			MAPMEDIA::iterator finder = m_mapDSOUND.find(dwID);

			if( finder != m_mapDSOUND.end() )
				return ((CD3DSound *) (*finder).second)->Play();
		}

		break;

	case MEDIA_TSHOW	:
		{
			MAPMEDIA::iterator finder = m_mapDSHOW.find(dwID);

			if( finder != m_mapDSHOW.end() )
				((CTShow *) (*finder).second)->Play();
		}

		break;
	}

	return 0;
}

void CTachyonMedia::Stop( BYTE bType)
{
	switch(bType)
	{
	case MEDIA_TMUSIC	:
		{
			MAPMEDIA::iterator it;

			for( it = m_mapDMUSIC.begin(); it != m_mapDMUSIC.end(); it++)
				((CT3DMusic *) (*it).second)->Stop();
		}

		break;

	case MEDIA_TSOUND	:
		{
			MAPMEDIA::iterator it;

			for( it = m_mapDSOUND.begin(); it != m_mapDSOUND.end(); it++)
				((CD3DSound *) (*it).second)->Stop();
		}

		break;

	case MEDIA_TSHOW	:
		{
			MAPMEDIA::iterator it;

			for( it = m_mapDSHOW.begin(); it != m_mapDSHOW.end(); it++)
				((CTShow *) (*it).second)->Stop();
		}

		break;
	}
}

void CTachyonMedia::Stop( BYTE bType, DWORD dwID, int nIndex)
{
	switch(bType)
	{
	case MEDIA_TMUSIC	:
		{
			MAPMEDIA::iterator finder = m_mapDMUSIC.find(dwID);

			if( finder != m_mapDMUSIC.end() )
				((CT3DMusic *) (*finder).second)->Stop();
		}

		break;

	case MEDIA_TSOUND	:
		{
			MAPMEDIA::iterator finder = m_mapDSOUND.find(dwID);

			if( finder != m_mapDSOUND.end() )
				((CD3DSound *) (*finder).second)->Stop(nIndex);
		}

		break;

	case MEDIA_TSHOW	:
		{
			MAPMEDIA::iterator finder = m_mapDSHOW.find(dwID);

			if( finder != m_mapDSHOW.end() )
				((CTShow *) (*finder).second)->Stop();
		}

		break;
	}
}

BYTE CTachyonMedia::IsPlay( BYTE bType, DWORD dwID, int nIndex)
{
	switch(bType)
	{
	case MEDIA_TMUSIC	:
		{
			MAPMEDIA::iterator finder = m_mapDMUSIC.find(dwID);

			if( finder != m_mapDMUSIC.end() )
				return ((CT3DMusic *) (*finder).second)->IsPlay();
		}

		break;

	case MEDIA_TSOUND	:
		{
			MAPMEDIA::iterator finder = m_mapDSOUND.find(dwID);

			if( finder != m_mapDSOUND.end() )
				return ((CD3DSound *) (*finder).second)->IsPlay(nIndex);
		}

		break;

	case MEDIA_TSHOW	:
		{
			MAPMEDIA::iterator finder = m_mapDSHOW.find(dwID);

			if( finder != m_mapDSHOW.end() )
				return ((CTShow *) (*finder).second)->IsPlay();
		}

		break;
	}

	return FALSE;
}

void CTachyonMedia::OnWindowPosition( BYTE bType, DWORD dwID)
{
	switch(bType)
	{
	case MEDIA_TSHOW	:
		{
			MAPMEDIA::iterator finder = m_mapDSHOW.find(dwID);

			if( finder != m_mapDSHOW.end() )
				((CTShow *) (*finder).second)->OnWindowPosition();
		}

		break;
	}
}

void CTachyonMedia::OnSetFocus( BYTE bFocus)
{
	m_bBACK = bFocus;
	ResetVolume();
}
