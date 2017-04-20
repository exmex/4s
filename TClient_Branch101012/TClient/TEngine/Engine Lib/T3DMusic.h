#pragma once


typedef struct tagMPEG_HEADER
{
	int m_nVersion;
	int m_nLayer;
	int m_nCRC;
	int m_nBitrateIndex;
	int m_nFreqIndex;
	int m_nPadding;
	int m_nExtension;
	int m_nMode;
	int m_nModeEx;
	int m_nCopyright;
	int m_nOriginal;
	int m_nEmphasis;
} MPEG_HEADER, *LPMPEG_HEADER;


typedef struct tagMPEG_DECODE_PARAM
{
	MPEG_HEADER m_vHeader;

	LPVOID m_pOutput;
	LPVOID m_pInput;

	int m_nBitRate;
	int m_nOutput;
	int m_nInput;
} MPEG_DECODE_PARAM, *LPMPEG_DECODE_PARAM;


typedef struct tagSFBAND
{
	int m_nL[23];
	int m_nS[14];
} SFBAND, *LPSFBAND;


class CT3DMusic
{
public:
	static MPEG_DECODE_OPTION m_vDecodeOption;
	static BYTE m_bMasterVolume;
	static BYTE m_bON;
	static BYTE m_bEQ;

	static FLOAT m_vEQ[32];

private:
	static LPDIRECTSOUND8 m_pDS;
	static WORD m_wBufferingMax;

	// Bitrate, Frequency table
	static int m_vBITRATE[3][3][16];
	static int m_vFREQ[3][4];
	static int m_vJOINT[16];

	static BYTE m_vBATCount[5][4];
	static BYTE m_vBAT[5][4][16];

	static FLOAT m_vWinCoef[264];
	static SFBAND m_vSFBT[3][3];

	// Table for layer1
	static FLOAT m_vValueL1[18];

	// Table for layer2
	static WORD m_vTableGRP9[1024][3];
	static BYTE m_vTableGRP5[128][3];
	static BYTE m_vTableGRP3[32][3];

	static FLOAT m_vValueL2[18];
	static FLOAT m_vTableSF[64];

	// Table for layer3
	static FLOAT m_vValueL3[262];

	static FLOAT m_vScaleL3[2][4][32];
	static FLOAT m_vPowL3[2 * IS_MAX];
	static FLOAT m_vSubBlockL3[8];
	static FLOAT m_vCSA[8][2];

	static FLOAT m_vISData2[2][2][64][2];
	static FLOAT m_vISData1[2][8][2];

	static FLOAT m_vCoef32[31];
	static FLOAT m_vCoef[9][4];
	static FLOAT m_vW1[18];
	static FLOAT m_vW2[9];

	static FLOAT m_vWIN[4][36];
	static FLOAT m_fCoef87;
	static FLOAT m_vV1[6];
	static FLOAT m_vV2[3];

public:
	static void DecodeINIT( LPDIRECTSOUND8 pDS);

private:
	// SBT Function for L3
	static void L3SBTMono32S( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH);
	static void L3SBTDual32S( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH);
	static void L3SBTMono32B( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH);
	static void L3SBTDual32B( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH);
	static void L3SBTMono16S( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH);
	static void L3SBTDual16S( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH);
	static void L3SBTMono16B( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH);
	static void L3SBTDual16B( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH);
	static void L3SBTMono8S( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH);
	static void L3SBTDual8S( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH);
	static void L3SBTMono8B( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH);
	static void L3SBTDual8B( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH);

	// SBT Function
	static void SBTMono32S( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH);
	static void SBTDual32S( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH);
	static void SBTMono32B( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH);
	static void SBTDual32B( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH);
	static void SBTMono16S( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH);
	static void SBTDual16S( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH);
	static void SBTMono16B( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH);
	static void SBTDual16B( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH);
	static void SBTMono8S( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH);
	static void SBTDual8S( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH);
	static void SBTMono8B( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH);
	static void SBTDual8B( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH);

	// WIN Function
	static void WinDual32S( FLOAT *pBUF, int nPOS, LPVOID pPCM);
	static void WinDual32B( FLOAT *pBUF, int nPOS, LPVOID pPCM);
	static void WinMono32S( FLOAT *pBUF, int nPOS, LPVOID pPCM);
	static void WinMono32B( FLOAT *pBUF, int nPOS, LPVOID pPCM);
	static void WinDual16S( FLOAT *pBUF, int nPOS, LPVOID pPCM);
	static void WinDual16B( FLOAT *pBUF, int nPOS, LPVOID pPCM);
	static void WinMono16S( FLOAT *pBUF, int nPOS, LPVOID pPCM);
	static void WinMono16B( FLOAT *pBUF, int nPOS, LPVOID pPCM);
	static void WinDual8S( FLOAT *pBUF, int nPOS, LPVOID pPCM);
	static void WinDual8B( FLOAT *pBUF, int nPOS, LPVOID pPCM);
	static void WinMono8S( FLOAT *pBUF, int nPOS, LPVOID pPCM);
	static void WinMono8B( FLOAT *pBUF, int nPOS, LPVOID pPCM);

	// FDCT Function
	static void FdctDual32( FLOAT *pSAMPLE, FLOAT *pBUF);
	static void FdctMono32( FLOAT *pSAMPLE, FLOAT *pBUF);
	static void FdctDual16( FLOAT *pSAMPLE, FLOAT *pBUF);
	static void FdctMono16( FLOAT *pSAMPLE, FLOAT *pBUF);
	static void FdctDual8( FLOAT *pSAMPLE, FLOAT *pBUF);
	static void FdctMono8( FLOAT *pSAMPLE, FLOAT *pBUF);

	// XForm Function for L3
	static void XFormDualL(
		CT3DMusic *pMUSIC,
		LPSIDE_INFO pSIDE,
		LPVOID pPCM,
		int nVersion,
		int nFREQ,
		int nGROUP);

	static void XFormDualR(
		CT3DMusic *pMUSIC,
		LPSIDE_INFO pSIDE,
		LPVOID pPCM,
		int nVersion,
		int nFREQ,
		int nGROUP);

	static void XFormDualMono(
		CT3DMusic *pMUSIC,
		LPSIDE_INFO pSIDE,
		LPVOID pPCM,
		int nVersion,
		int nFREQ,
		int nGROUP);

	static void XFormDual(
		CT3DMusic *pMUSIC,
		LPSIDE_INFO pSIDE,
		LPVOID pPCM,
		int nVersion,
		int nFREQ,
		int nGROUP);

	static void XFormMono(
		CT3DMusic *pMUSIC,
		LPSIDE_INFO pSIDE,
		LPVOID pPCM,
		int nVersion,
		int nFREQ,
		int nGROUP,
		int nPREV,
		int nSIDE,
		int nCH);

private:
	static DWORD WINAPI DecodeMPEG( LPVOID pParam);

	static XFORM_PROC GetXFormProc( int nDUAL);
	static SBT_PROC GetL3SBTProc(
		int nTYPE,
		int nBIT,
		int nDUAL);

	static SBT_PROC GetSBTProc(
		int nTYPE,
		int nBIT,
		int nDUAL);

	static BYTE GetMPEGHeader(
		LPBYTE pDATA,
		LPMPEG_HEADER pHEADER);

	static BYTE GetMPEGInfo(
		LPMPEG_DECODE_INFO pINFO,
		LPBYTE pDATA,
		int nSIZE);

	static void DecodeL1INIT();
	static void DecodeL2INIT();
	static void DecodeL3INIT();

	static void InitQuant();
	static void InitAlias();
	static void InitIMDCT();
	static void InitFDCT();
	static void InitMSIS();
	static void InitHWIN();

	static void DecodeL1(
		CT3DMusic *pMUSIC,
		int nFRAME);

	static void DecodeL2(
		CT3DMusic *pMUSIC,
		int nFRAME);

	static void DecodeL3(
		CT3DMusic *pMUSIC,
		int nFRAME);

	static void DecodeMainL3(
		CT3DMusic *pMUSIC,
		LPSIDE_INFO pSIDE,
		LPBYTE pOUTPUT,
		int nMSMode,
		int nISMode,
		int nChannels,
		int nGROUP,
		int& nBIT);

	static void ForwardBF(
		FLOAT *pCOEF,
		FLOAT *pDATA,
		FLOAT *pBUF,
		int nCount,
		int nSIZE);

	static void BackBF(
		FLOAT *pDATA,
		FLOAT *pBUF,
		int nCount,
		int nSIZE);

	static int AddHybrid(
		FLOAT *pIN,
		FLOAT *pLEFT,
		FLOAT *pOUT,
		int nTYPE,
		int nLONG,
		int nTOT);

	static int Hybrid(
		FLOAT *pIN,
		FLOAT *pPREV,
		FLOAT *pOUT,
		int nTYPE,
		int nLONG,
		int nTOT,
		int nPREV,
		int nBand);

	static void InvFREQ(
		FLOAT *pDATA,
		int nCount);

	static int HuffmanQ(
		CTBitStream *pSTREAM,
		int *pVALUE,
		int nCount,
		int nBits,
		int nTable);

	static void Huffman(
		CTBitStream *pSTREAM,
		int *pVALUE,
		int nCount,
		int nTable);

	static void Antialias(
		FLOAT *pVALUE,
		int nCount);

	static void ProcessMS(
		FLOAT *pVALUE,
		int nCount);

	static void ProcessIS1(
		FLOAT *pVALUE,
		LPSCALE_FACTOR pSF,
		LPCB_INFO pCB,
		int nVersion,
		int nFREQ,
		int nMSMode,
		int nCount);

	static void ProcessIS2(
		FLOAT *pVALUE,
		LPSCALE_FACTOR pSF,
		LPIS_SF_INFO pISSF,
		LPCB_INFO pCB,
		int nVersion,
		int nFREQ,
		int nMSMode,
		int nCount);

	static int GetID3TagSIZE( LPBYTE pDATA);
	static void Imdct6_3( FLOAT *pDATA);
	static void Imdct18( FLOAT *pDATA);

private:
	static inline int GetBITRATE( LPMPEG_HEADER pHEADER)
	{
		return m_vBITRATE[pHEADER->m_nVersion - 1][pHEADER->m_nLayer - 1][pHEADER->m_nBitrateIndex] * 1000;
	};

	static inline int GetFREQ( LPMPEG_HEADER pHEADER)
	{
		return m_vFREQ[pHEADER->m_nVersion - 1][pHEADER->m_nFreqIndex];
	};

	static inline int GetFrameSize( LPMPEG_HEADER pHEADER)
	{
		switch(pHEADER->m_nLayer)
		{
		case 1	: return (12 * GetBITRATE(pHEADER) / GetFREQ(pHEADER) + pHEADER->m_nPadding) * 4;
		case 2	: return 144 * GetBITRATE(pHEADER) / GetFREQ(pHEADER) + pHEADER->m_nPadding;
		case 3	: return pHEADER->m_nVersion == 1 ? 144 * GetBITRATE(pHEADER) / GetFREQ(pHEADER) + pHEADER->m_nPadding : 72 * GetBITRATE(pHEADER) / GetFREQ(pHEADER) + pHEADER->m_nPadding;
		}

		return 0;
	};

	static inline int GetBandIndex( int nVersion, int nFREQ, int nCBType, int nIndex)
	{
		return nCBType ? 3 * m_vSFBT[nVersion][nFREQ].m_nS[nIndex + 1] : m_vSFBT[nVersion][nFREQ].m_nL[nIndex + 1];
	};

private:
	inline DWORD Tick2Frame( DWORD dwTick)
	{
		static FLOAT vFactor[] = { 26.12245f, 24.0f, 36.0f};
		return DWORD(dwTick / vFactor[(m_pFRAME[2] >> 2) & 3]);
	};

	inline WORD GetBufferingCount( DWORD dwSTART, DWORD dwEND)
	{
		WORD wBuffering = WORD(Tick2Frame(dwEND) - Tick2Frame(dwSTART) + 1);
		return min( m_wBufferingMax, wBuffering);
	};

	inline int GetStereoBIT()
	{
		return m_vJOINT[(m_vPARAM[m_bINDEX].m_vHeader.m_nMode << 2) + m_vPARAM[m_bINDEX].m_vHeader.m_nModeEx];
	};

	inline int GetCRCSize()
	{
		return m_vPARAM[m_bINDEX].m_vHeader.m_nCRC ? 2 : 0;
	};

	inline int GetChannels()
	{
		return m_vPARAM[m_bINDEX].m_vHeader.m_nMode == 3 ? 1 : 2;
	};

	inline int GetBATCountL2( int nINDEX)
	{
		return m_bBATMode ? 2 * m_vBATCount[m_nBATCount][nINDEX] : m_vBATCount[m_nBATCount][nINDEX];
	};

private:
	void BufferingFRAME( WORD wBuffering);
	void DecodeFRAME();

	BYTE DecodeFRAME( WORD wBuffering);
	BYTE DecodeSTART();

	BYTE ResetDSBUF( BYTE bINDEX);
	BYTE PlayBUF( BYTE bINDEX);

	void InitMPEG(
		LPMPEG_HEADER pHEADER,
		DWORD dwSIZE);

	void InitL1( LPMPEG_HEADER pHEADER);
	void InitL2( LPMPEG_HEADER pHEADER);
	void InitL3( LPMPEG_HEADER pHEADER);

	void Unlock();
	BYTE Lock();

public:
	BYTE SetPos( DWORD dwPOS);

	DWORD GetLength();
	DWORD GetPos();

public:
	BYTE InitMusic( LPTSTR szFile);
	BYTE ResetVolume();

	BYTE IsPlay();
	BYTE Pause();
	BYTE Play();
	BYTE Stop();

private:
	FRAME_PROC m_pFrameProc;
	XFORM_PROC m_pXFormProc;
	SBT_PROC m_pSBTProc;

	HANDLE m_hThread;
	HANDLE m_hEVENT;
	HANDLE m_hPAUSE;
	HANDLE m_hCOMP;

	DWORD m_dwStartTick;
	DWORD m_dwPlayTick;

	DWORD m_dwPartCount;
	DWORD m_dwPart;

	BYTE m_bBATMode;
	BYTE m_bINDEX;

private:
	LPSAMPLEBUF m_pSAMPLE;
	int m_vCOUNT[2][2];

	int m_nBandLimit12;
	int m_nBandLimit21;
	int m_nBandLimit;

	int m_nTimeDivider;
	int m_nMixedCBEnd;
	int m_nBATCount;
	int m_nMaxSB;
	int m_nLimit;
	int m_nGroup;

private:
	LPDIRECTSOUNDBUFFER m_vOUTPUT[MPEG_BUFFER_COUNT];
	MPEG_DECODE_PARAM m_vPARAM[MPEG_BUFFER_COUNT];
	WAVEHDR m_vHEADER[MPEG_BUFFER_COUNT];
	LPVOID m_pBUF[MPEG_BUFFER_COUNT];
	DWORD m_vBUFSIZE[MPEG_BUFFER_COUNT];

	CRITICAL_SECTION m_cs;
	HANDLE m_hFILE;
	HANDLE m_hMAP;

	LPBYTE m_pFRAME;
	LPBYTE m_pDATA;

	FLOAT *m_vSBTBUF[2];
	int m_vSBT[2];

	LPBYTE m_pL3BUF;
	int m_vL3POS[2];

public:
	CString m_strFILE;

	DWORD m_dwSTART;
	DWORD m_dwEND;
	DWORD m_dwPOS;
	DWORD m_dwSEEK;

	WORD m_wFRAME;

	BYTE m_bFadeVolume;
	BYTE m_bVolume;
	BYTE m_bSTATE;
	BYTE m_bLOOP;

public:
	CT3DMusic();
	virtual ~CT3DMusic();
};
