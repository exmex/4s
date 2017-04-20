// D3DSound.h: interface for the CD3DSound class.
//
//////////////////////////////////////////////////////////////////////

#if !defined __D3DSOUND_H
#define __D3DSOUND_H


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CWaveFile
{
public:
	LPWAVEFORMATEX m_pWFX;
	HMMIO m_hMMIO;

	MMCKINFO m_mmckRIFF;
	MMCKINFO m_mmckINFO;
	DWORD m_dwSize;

public:
	void Load( CString strFileName);

	void Release();
	void Reset();

	DWORD Read( LPBYTE pBUF, DWORD dwSize);
	DWORD GetSize();

public:
	CWaveFile();
	virtual ~CWaveFile();
};


class CD3DSound
{
	friend class CTachyonMedia;

public:
	static BYTE m_bMasterVolume;
	static BYTE m_bON;

protected:
	static VDIRECTSOUND3DBUFFER m_v3DGARBAGE;
	static VDIRECTSOUNDBUFFER m_vGARBAGE;

	static LPDIRECTSOUND3DLISTENER8 m_pLISTENER;
	static D3DXMATRIX m_vLISTENER;

public:
	static void ResetLISTENER(
		LPD3DXVECTOR3 pPosition,
		LPD3DXVECTOR3 pAxisZ,
		LPD3DXVECTOR3 pAxisY);

	static D3DXVECTOR3 ConvertPOS(
		FLOAT fPosX,
		FLOAT fPosY,
		FLOAT fPosZ);

	static void GetListener( LPDS3DLISTENER pLISTENER);
	static void SetListener(
		LPDS3DLISTENER pLISTENER,
		DWORD dwApply);

	static BYTE IsPlay( LPDIRECTSOUNDBUFFER pBUF);
	static void ClearGARBAGE();
	static void InitGARBAGE();

protected:
	VDIRECTSOUND3DBUFFER m_v3DBUF;
	VDIRECTSOUNDBUFFER m_vBUF;
	VECTORBYTE m_vLOCK;

	LPDIRECTSOUND8 m_pDS;
	CString m_strFile;
	DWORD m_dwSize;

public:
	BYTE m_bFadeVolume;
	BYTE m_bVolume;

public:
	void Initialize(
		LPDIRECTSOUND8 pDS,
		CString strFile);

	BYTE LoadData(
		LPDIRECTSOUNDBUFFER pDSBUF,
		CWaveFile *pFILE);

	BYTE LoadData();
	void Restore();
	void Release();

public:
	LPDIRECTSOUND3DBUFFER GetDS3D( int nIndex);
	LPDIRECTSOUNDBUFFER GetDSB( int nIndex);

	BYTE ResetVolume( int nIndex);
	BYTE ResetVolume();

	DWORD GetPos( int nIndex);
	DWORD GetLength();

	BYTE SetPos( int nIndex, DWORD dwPos);
	BYTE IsPlay( int nIndex);

	BYTE Pause( int nIndex);
	BYTE Stop( int nIndex);
	BYTE Play( int nIndex);

	void Stop();
	int Play();

	void Unlock( int nIndex);
	int Lock();

public:
	CD3DSound();
	virtual ~CD3DSound();
};


#endif // !defined __D3DSOUND_H
