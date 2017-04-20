// D3DSound.cpp: implementation of the CD3DSound class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

VDIRECTSOUND3DBUFFER CD3DSound::m_v3DGARBAGE;
VDIRECTSOUNDBUFFER CD3DSound::m_vGARBAGE;

LPDIRECTSOUND3DLISTENER8 CD3DSound::m_pLISTENER = NULL;
D3DXMATRIX CD3DSound::m_vLISTENER;

BYTE CD3DSound::m_bMasterVolume = VOLUME_MAX;
BYTE CD3DSound::m_bON = TRUE;


//////////////////////////////////////////////////////////////////////
// class CD3DSound
//////////////////////////////////////////////////////////////////////

CD3DSound::CD3DSound()
{
	m_v3DBUF.clear();
	m_vBUF.clear();
	m_vLOCK.clear();

	m_strFile.Empty();
	m_pDS = NULL;

	m_bFadeVolume = VOLUME_MAX;
	m_bVolume = VOLUME_MAX;
	m_dwSize = 0;
}

CD3DSound::~CD3DSound()
{
	Release();
}

void CD3DSound::Initialize( LPDIRECTSOUND8 pDS,
						    CString strFile)
{
	Release();

	m_strFile = strFile;
	m_pDS = pDS;
}

BYTE CD3DSound::LoadData()
{
	if(!m_pDS)
		return FALSE;

	LPDIRECTSOUND3DBUFFER p3DBUF = NULL;
	LPDIRECTSOUNDBUFFER pDSBUF = NULL;

	CWaveFile vFILE;
	Release();

	vFILE.Load(m_strFile);
	m_dwSize = vFILE.GetSize();

	DSBUFFERDESC desc;
	ZeroMemory( &desc, sizeof(DSBUFFERDESC));
	desc.dwSize = sizeof(DSBUFFERDESC);
	desc.dwFlags = DSBCAPS_CTRL3D|DSBCAPS_CTRLVOLUME|DSBCAPS_GLOBALFOCUS;
	desc.dwBufferBytes = m_dwSize;
	desc.guid3DAlgorithm = DS3DALG_NO_VIRTUALIZATION;
	desc.lpwfxFormat = vFILE.m_pWFX;

	if(FAILED(m_pDS->CreateSoundBuffer( &desc, &pDSBUF, NULL)))
	{
		desc.dwFlags = DSBCAPS_CTRLVOLUME|DSBCAPS_GLOBALFOCUS;
		desc.guid3DAlgorithm = DS3DALG_DEFAULT;

		if(FAILED(m_pDS->CreateSoundBuffer( &desc, &pDSBUF, NULL)))
		{
			vFILE.Release();
			return FALSE;
		}
	}

	if(!LoadData( pDSBUF, &vFILE))
	{
		pDSBUF->Release();
		vFILE.Release();

		return FALSE;
	}
	vFILE.Release();

	if( SUCCEEDED(pDSBUF->QueryInterface(
		IID_IDirectSound3DBuffer8,
		(LPVOID *) &p3DBUF)))
	{
		DS3DBUFFER param;
		param.dwSize = sizeof(DS3DBUFFER);
		p3DBUF->GetAllParameters(&param);
		param.dwMode = DS3DMODE_HEADRELATIVE;
		p3DBUF->SetAllParameters( &param, DS3D_IMMEDIATE);
	}

	m_v3DBUF.push_back(p3DBUF);
	m_vLOCK.push_back(FALSE);
	m_vBUF.push_back(pDSBUF);

	pDSBUF->SetCurrentPosition(0);
	ResetVolume();

	return TRUE;
}

BYTE CD3DSound::LoadData( LPDIRECTSOUNDBUFFER pDSBUF, CWaveFile *pFILE)
{
	LPVOID pBUF = NULL;

	DWORD dwRead = 0;
	DWORD dwSize = 0;

	if( FAILED(pDSBUF->Lock(
		0, m_dwSize,
		&pBUF,
		&dwSize,
		NULL,
		NULL, 0L)))
		return FALSE;

	pFILE->Reset();
	dwRead = pFILE->Read(
		(LPBYTE) pBUF,
		dwSize);

	if( dwRead != 0 )
	{
		while( dwRead < dwSize )
		{
			pFILE->Reset();

			dwRead += pFILE->Read(
				(LPBYTE) pBUF + dwRead,
				dwSize - dwRead);
		}
	}
	else
		FillMemory( pBUF, dwSize, (BYTE) (pFILE->m_pWFX->wBitsPerSample == 8 ? 128 : 0));
	pDSBUF->Unlock( pBUF, dwSize, NULL, 0);

	return TRUE;
}

void CD3DSound::Restore()
{
	DWORD dwStatus;

	if(FAILED(m_vBUF[0]->GetStatus(&dwStatus)))
		return;

	if( dwStatus & DSBSTATUS_BUFFERLOST )
		while( m_vBUF[0]->Restore() == DSERR_BUFFERLOST );
}

void CD3DSound::Release()
{
	for( int i=0; i<INT(m_vBUF.size()); i++)
		if(m_vBUF[i])
			if(IsPlay(i))
			{
				m_v3DGARBAGE.push_back(m_v3DBUF[i]);
				m_vGARBAGE.push_back(m_vBUF[i]);
			}
			else
			{
				if(m_v3DBUF[i])
					m_v3DBUF[i]->Release();
				m_vBUF[i]->Release();
			}

	m_v3DBUF.clear();
	m_vBUF.clear();
	m_vLOCK.clear();
	m_dwSize = 0;
}

LPDIRECTSOUND3DBUFFER CD3DSound::GetDS3D( int nIndex)
{
	return nIndex < INT(m_v3DBUF.size()) ? m_v3DBUF[nIndex] : NULL;
}

LPDIRECTSOUNDBUFFER CD3DSound::GetDSB( int nIndex)
{
	return nIndex < INT(m_vBUF.size()) ? m_vBUF[nIndex] : NULL;
}

BYTE CD3DSound::ResetVolume( int nIndex)
{
	if( nIndex < INT(m_vBUF.size()) )
	{
		FLOAT fVolume = CTachyonMedia::m_bBACK ? 0.0f : FLOAT(CTachyonMedia::m_bMasterVolume) * 
			FLOAT(m_bMasterVolume) * FLOAT(m_bFadeVolume) * FLOAT(m_bVolume) /
			FLOAT(VOLUME_MAX * VOLUME_MAX * VOLUME_MAX * VOLUME_MAX);

		if( fVolume > 0.0f )
			fVolume = -2000.0f * log10(1.0f / fVolume);
		else
			fVolume = DSBVOLUME_MIN;

		m_vBUF[nIndex]->SetVolume((LONG)fVolume);
	}
	else
		return FALSE;

	return TRUE;
}

BYTE CD3DSound::ResetVolume()
{
	for( DWORD i=0; i<INT(m_vBUF.size()); i++)
		if(m_vLOCK[i])
			ResetVolume(i);

	return TRUE;
}

BYTE CD3DSound::SetPos( int nIndex, DWORD dwPos)
{
	if( nIndex < INT(m_vBUF.size()) )
		m_vBUF[nIndex]->SetCurrentPosition(dwPos);
	else
		return FALSE;

	return TRUE;
}

DWORD CD3DSound::GetPos( int nIndex)
{
	DWORD dwResult = 0;

	if( nIndex >= INT(m_vBUF.size()) )
		return 0;

	if(FAILED(m_vBUF[nIndex]->GetCurrentPosition( &dwResult, NULL)))
		return 0;

	return dwResult;
}

DWORD CD3DSound::GetLength()
{
	return m_dwSize;
}

BYTE CD3DSound::Pause( int nIndex)
{
	if( nIndex >= INT(m_vBUF.size()) )
		return FALSE;

	if(FAILED(m_vBUF[nIndex]->Stop()))
		return FALSE;

	return TRUE;
}

BYTE CD3DSound::Stop( int nIndex)
{
	if( nIndex >= INT(m_vBUF.size()) )
		return FALSE;

	if(FAILED(m_vBUF[nIndex]->Stop()))
		return FALSE;
	m_vBUF[nIndex]->SetCurrentPosition(0);

	return TRUE;
}

BYTE CD3DSound::Play( int nIndex)
{
	DWORD dwStatus;

	if( !CTachyonMedia::m_bON || !m_bON )
		return FALSE;

	if( nIndex >= INT(m_vBUF.size()) )
		return FALSE;

	if(FAILED(m_vBUF[nIndex]->GetStatus(&dwStatus)))
		return FALSE;

	if( dwStatus & DSBSTATUS_BUFFERLOST )
	{
		while( m_vBUF[0]->Restore() == DSERR_BUFFERLOST );
		CWaveFile vFILE;

		vFILE.Load(m_strFile);
		LoadData(
			m_vBUF[0],
			&vFILE);
		vFILE.Release();

		for( int i=0; i<INT(m_vBUF.size()); i++)
			m_vBUF[i]->SetCurrentPosition(0);
	}

	if(FAILED(ResetVolume(nIndex)))
		return FALSE;
	
	if(FAILED(m_vBUF[nIndex]->Play( 0, 0, 0)))
		return FALSE;

	return TRUE;
}

void CD3DSound::Stop()
{
	for( int i=0; i<INT(m_vBUF.size()); i++)
		if(m_vBUF[i])
			if(IsPlay(i))
			{
				Stop(i);

				if(m_v3DBUF[i])
					m_v3DBUF[i]->Release();
				m_vBUF[i]->Release();
			}

	m_v3DBUF.clear();
	m_vBUF.clear();
	m_vLOCK.clear();
}

int CD3DSound::Play()
{
	int nIndex = Lock();

	if( nIndex >= 0 )
	{
		if(!Play(nIndex))
		{
			Unlock(nIndex);
			return -1;
		}
		
		Unlock(nIndex);
	}

	return nIndex;
}

BYTE CD3DSound::IsPlay( int nIndex)
{
	return nIndex < INT(m_vBUF.size()) ? IsPlay(m_vBUF[nIndex]) : FALSE;
}

BYTE CD3DSound::IsPlay( LPDIRECTSOUNDBUFFER pBUF)
{
	DWORD dwStatus;

	if(FAILED(pBUF->GetStatus(&dwStatus)))
		return FALSE;

	return dwStatus & DSBSTATUS_PLAYING ? TRUE : FALSE;
}

void CD3DSound::Unlock( int nIndex)
{
	if( nIndex < INT(m_vBUF.size()) )
	{
		if( m_vLOCK[nIndex] && nIndex > 0 )
		{
			if(IsPlay(nIndex))
			{
				m_v3DGARBAGE.push_back(m_v3DBUF[nIndex]);
				m_vGARBAGE.push_back(m_vBUF[nIndex]);
			}
			else
			{
				if(m_v3DBUF[nIndex])
					m_v3DBUF[nIndex]->Release();
				m_vBUF[nIndex]->Release();
			}

			m_v3DBUF[nIndex] = NULL;
			m_vBUF[nIndex] = NULL;
		}
		m_vLOCK[nIndex] = FALSE;

		for( int i=0; i<INT(m_vLOCK.size()); i++)
			if(m_vLOCK[i])
				return;

		Release();
	}
}

int CD3DSound::Lock()
{
	if( !m_pDS || !CTachyonMedia::m_bON || !m_bON )
		return -1;

	if(m_vBUF.empty())
	{
		if(!LoadData())
			return -1;
		m_vLOCK[0] = TRUE;

		return 0;
	}

	for( int i=0; i<INT(m_vLOCK.size()); i++)
		if(!m_vLOCK[i])
		{
			if(!m_vBUF[i])
			{
				DS3DBUFFER param;

				if(FAILED(m_pDS->DuplicateSoundBuffer( m_vBUF[0], &m_vBUF[i])))
					return -1;

				if( SUCCEEDED(m_vBUF[i]->QueryInterface(
					IID_IDirectSound3DBuffer8,
					(LPVOID *) &m_v3DBUF[i])))
				{
					param.dwSize = sizeof(DS3DBUFFER);
					m_v3DBUF[i]->GetAllParameters(&param);
					param.dwMode = DS3DMODE_HEADRELATIVE;
					m_v3DBUF[i]->SetAllParameters( &param, DS3D_IMMEDIATE);
				}
			}
			m_vLOCK[i] = TRUE;

			return i;
		}

	LPDIRECTSOUND3DBUFFER p3DBUF = NULL;
	LPDIRECTSOUNDBUFFER pDSBUF = NULL;

	if(FAILED(m_pDS->DuplicateSoundBuffer( m_vBUF[0], &pDSBUF)))
		return -1;

	if( SUCCEEDED(pDSBUF->QueryInterface(
		IID_IDirectSound3DBuffer8,
		(LPVOID *) &p3DBUF)))
	{
		DS3DBUFFER param;
		param.dwSize = sizeof(DS3DBUFFER);
		p3DBUF->GetAllParameters(&param);
		param.dwMode = DS3DMODE_HEADRELATIVE;
		p3DBUF->SetAllParameters( &param, DS3D_IMMEDIATE);
	}

	m_v3DBUF.push_back(p3DBUF);
	m_vBUF.push_back(pDSBUF);
	m_vLOCK.push_back(TRUE);

	return INT(m_vBUF.size()) - 1;
}

void CD3DSound::ResetLISTENER( LPD3DXVECTOR3 pPosition,
							   LPD3DXVECTOR3 pAxisZ,
							   LPD3DXVECTOR3 pAxisY)
{
	if(!m_pLISTENER)
		return;

	D3DXMATRIX vSCALE(
		0.25f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.25f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.25f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	D3DXMatrixLookAtLH(
		&m_vLISTENER,
		pPosition,
		&((*pPosition) + (*pAxisZ)),
		pAxisY);
	m_vLISTENER *= vSCALE;

	m_pLISTENER->SetPosition(
		0.0f, 0.0f, 0.0f,
		DS3D_IMMEDIATE);

	VDIRECTSOUND3DBUFFER::iterator it3DBUF = m_v3DGARBAGE.begin();
	VDIRECTSOUNDBUFFER::iterator itBUF = m_vGARBAGE.begin();

	while(itBUF != m_vGARBAGE.end())
		if( !(*itBUF) || !IsPlay(*itBUF) )
		{
			if((*it3DBUF))
				(*it3DBUF)->Release();
			(*itBUF)->Release();

			m_v3DGARBAGE.erase(it3DBUF);
			m_vGARBAGE.erase(itBUF);
		}
		else
		{
			it3DBUF++;
			itBUF++;
		}
}

void CD3DSound::GetListener( LPDS3DLISTENER pLISTENER)
{
	if(m_pLISTENER)
	{
		pLISTENER->dwSize = sizeof(DS3DLISTENER);
		m_pLISTENER->GetAllParameters(pLISTENER);
	}
}

void CD3DSound::SetListener( LPDS3DLISTENER pLISTENER, DWORD dwApply)
{
	if(m_pLISTENER)
	{
		pLISTENER->dwSize = sizeof(DS3DLISTENER);
		m_pLISTENER->SetAllParameters(
			pLISTENER,
			dwApply);
	}
}

void CD3DSound::ClearGARBAGE()
{
	while(!m_v3DGARBAGE.empty())
	{
		if(m_v3DGARBAGE.back())
			m_v3DGARBAGE.back()->Release();

		m_v3DGARBAGE.pop_back();
	}

	while(!m_vGARBAGE.empty())
	{
		if(m_vGARBAGE.back())
			m_vGARBAGE.back()->Release();

		m_vGARBAGE.pop_back();
	}
}

void CD3DSound::InitGARBAGE()
{
	m_v3DGARBAGE.clear();
	m_vGARBAGE.clear();
}

D3DXVECTOR3 CD3DSound::ConvertPOS( FLOAT fPosX,
								   FLOAT fPosY,
								   FLOAT fPosZ)
{
	D3DXVECTOR3 vResult(
		fPosX,
		fPosY,
		fPosZ);

	CTMath::Transform(
		&m_vLISTENER,
		&vResult);

	return vResult;
}


CWaveFile::CWaveFile()
{
	m_hMMIO = NULL;
	m_pWFX = NULL;

	m_dwSize = 0;
}

CWaveFile::~CWaveFile()
{
	Release();
}

void CWaveFile::Load( CString strFileName)
{
	if(m_pWFX)
	{
		delete[] m_pWFX;
		m_pWFX = NULL;
	}

	m_hMMIO = mmioOpen(
		(LPSTR) LPCTSTR(strFileName),
		NULL,
		MMIO_ALLOCBUF|MMIO_READ);

	if(!m_hMMIO)
	{
		HGLOBAL hDATA;
		HRSRC hINFO;

		DWORD dwSize;
		LPVOID pBUF;

		hINFO = FindResource( NULL, strFileName, _T("WAVE"));
		if(!hINFO)
			hINFO = FindResource( NULL, strFileName, _T("WAV"));

		if(!hINFO)
			return;

		hDATA = LoadResource( NULL, hINFO);
		if(!hDATA)
			return;

		dwSize = SizeofResource( NULL, hINFO);
		if(dwSize)
			return;

		pBUF = LockResource(hDATA);
		if(!pBUF)
			return;

		LPBYTE pDATA = new BYTE[dwSize];
		memcpy( pDATA, pBUF, dwSize);

		MMIOINFO mmioINFO;
		ZeroMemory( &mmioINFO, sizeof(MMIOINFO));
		mmioINFO.fccIOProc = FOURCC_MEM;
		mmioINFO.cchBuffer = dwSize;
		mmioINFO.pchBuffer = (CHAR *) pDATA;

		m_hMMIO = mmioOpen(
			NULL,
			&mmioINFO,
			MMIO_ALLOCBUF|MMIO_READ);
	}

	PCMWAVEFORMAT pcm;
	MMCKINFO mmck;

	if( mmioDescend( m_hMMIO, &m_mmckRIFF, NULL, 0) ||
		m_mmckRIFF.ckid != FOURCC_RIFF ||
		m_mmckRIFF.fccType != mmioFOURCC( 'W', 'A', 'V', 'E'))
	{
		mmioClose( m_hMMIO, 0);
		return;
	}

	mmck.ckid = mmioFOURCC( 'f', 'm', 't', ' ');
	if( mmioDescend( m_hMMIO, &mmck, &m_mmckRIFF, MMIO_FINDCHUNK) ||
		mmck.cksize < (LONG) sizeof(PCMWAVEFORMAT) ||
		mmioRead( m_hMMIO, (HPSTR) &pcm, sizeof(PCMWAVEFORMAT)) != sizeof(PCMWAVEFORMAT))
	{
		mmioClose( m_hMMIO, 0);
		return;
	}

	if( pcm.wf.wFormatTag == WAVE_FORMAT_PCM )
	{
		m_pWFX = new WAVEFORMATEX();

		memcpy( m_pWFX, &pcm, sizeof(PCMWAVEFORMAT));
		m_pWFX->cbSize = 0;
	}
	else
	{
		LPBYTE pBUF = NULL;
		WORD wCBSIZE = 0L;

		if( mmioRead( m_hMMIO, (HPSTR) &wCBSIZE, sizeof(WORD)) != sizeof(WORD) )
		{
			mmioClose( m_hMMIO, 0);
			return;
		}

		m_pWFX = (LPWAVEFORMATEX) new BYTE[sizeof(WAVEFORMATEX) + wCBSIZE];
		memcpy( m_pWFX, &pcm, sizeof(PCMWAVEFORMAT));

		m_pWFX->cbSize = wCBSIZE;
		pBUF = (LPBYTE) m_pWFX;
		pBUF = pBUF + wCBSIZE;

		if( mmioRead( m_hMMIO, (HPSTR) pBUF, wCBSIZE) != wCBSIZE )
		{
			delete[] m_pWFX;
			m_pWFX = NULL;

			mmioClose( m_hMMIO, 0);
			return;
		}
	}

	if(mmioAscend( m_hMMIO, &mmck, 0))
	{
		delete[] m_pWFX;
		m_pWFX = NULL;

		mmioClose( m_hMMIO, 0);
		return;
	}

	Reset();
	m_dwSize = m_mmckINFO.cksize;
}

void CWaveFile::Release()
{
	if(m_pWFX)
	{
		delete[] m_pWFX;
		m_pWFX = NULL;
	}

	if(m_hMMIO)
	{
		mmioClose( m_hMMIO, 0);
		m_hMMIO = NULL;
	}

	m_dwSize = 0;
}

void CWaveFile::Reset()
{
	if( mmioSeek( m_hMMIO, m_mmckRIFF.dwDataOffset + sizeof(FOURCC), SEEK_SET) == -1 )
		return;

	m_mmckINFO.ckid = mmioFOURCC( 'd', 'a', 't', 'a');
	mmioDescend( m_hMMIO, &m_mmckINFO, &m_mmckRIFF, MMIO_FINDCHUNK);
}

DWORD CWaveFile::Read( LPBYTE pBUF, DWORD dwSize)
{
	MMIOINFO mmioINFO;

	if(mmioGetInfo( m_hMMIO, &mmioINFO, 0))
		return 0;

	if( dwSize > m_mmckINFO.cksize )
		dwSize = m_mmckINFO.cksize;
	m_mmckINFO.cksize -= dwSize;

	for( DWORD i=0; i<dwSize; i++)
	{
		if( mmioINFO.pchNext == mmioINFO.pchEndRead )
		{
			if(mmioAdvance( m_hMMIO, &mmioINFO, MMIO_READ))
				return 0;

			if( mmioINFO.pchNext == mmioINFO.pchEndRead )
				return 0;
		}

		pBUF[i] = *((LPBYTE) mmioINFO.pchNext);
		mmioINFO.pchNext++;
	}

	if(mmioSetInfo( m_hMMIO, &mmioINFO, 0))
		return 0;

	return dwSize;
}

DWORD CWaveFile::GetSize()
{
	return m_dwSize;
}
