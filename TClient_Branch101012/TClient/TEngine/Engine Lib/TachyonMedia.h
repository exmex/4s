#pragma once


class CTachyonMedia
{
public:
	static BYTE m_bMasterVolume;
	static BYTE m_bBACK;
	static BYTE m_bON;

protected:
	LPDIRECTSOUND8 m_pDS;

	MAPMEDIA m_mapDSOUND;
	MAPMEDIA m_mapDMUSIC;
	MAPMEDIA m_mapDSHOW;

public:
	BYTE InitMEDIA(
		HWND hWnd,
		DWORD dwCoopLevel = DSSCL_EXCLUSIVE,
		DWORD dwPrimaryChannels = 2,
		DWORD dwPrimaryFreq = 44100,
		DWORD dwPrimaryBitRate = 16);

	void LoadDShow(
		CWnd *pHost,
		CWnd *pWnd,
		DWORD dwID,
		DWORD dwMessage,
		LPTSTR szFileName,
		BYTE bVolume);

	void LoadDMusic(
		DWORD dwID,
		LPTSTR szFileName,
		BYTE bVolume);

	void LoadDSound(
		DWORD dwID,
		LPTSTR szFileName,
		BYTE bVolume);

	LPVOID GetData(
		BYTE bType,
		DWORD dwID);

	void Unregister(
		BYTE bType,
		DWORD dwID);

	void Register(
		BYTE bType,
		DWORD dwID,
		LPVOID pItem);

	void ResetVolume( BYTE bType);
	void ResetVolume();

	void ResetVolume(
		BYTE bType,
		DWORD dwID);

	void ResetVolume(
		BYTE bType,
		DWORD dwID,
		int nIndex);

	void SetPos(
		BYTE bType,
		DWORD dwID,
		int nIndex,
		LONGLONG nPos);

	LONGLONG GetLength(
		BYTE bType,
		DWORD dwID);

	LONGLONG GetPos(
		BYTE bType,
		DWORD dwID,
		int nIndex);

	void Toggle(
		BYTE bType,
		DWORD dwID);

	void Pause(
		BYTE bType,
		DWORD dwID,
		int nIndex);

	void Stop( BYTE bType);
	void Stop(
		BYTE bType,
		DWORD dwID,
		int nIndex);

	void Play(
		BYTE bType,
		DWORD dwID,
		int nIndex);

	int Play(
		BYTE bType,
		DWORD dwID);

	BYTE IsPlay(
		BYTE bType,
		DWORD dwID,
		int nIndex);

public:
	void OnWindowPosition(
		BYTE bType,
		DWORD dwID);

	void OnSetFocus( BYTE bFocus);
	void ReleaseAll();

public:
	CTachyonMedia();
	virtual ~CTachyonMedia();
};
