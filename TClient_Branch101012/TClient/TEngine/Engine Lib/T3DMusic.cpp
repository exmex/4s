#include "StdAfx.h"


MPEG_DECODE_OPTION CT3DMusic::m_vDecodeOption;
LPDIRECTSOUND8 CT3DMusic::m_pDS = NULL;

BYTE CT3DMusic::m_bMasterVolume = VOLUME_MAX;
BYTE CT3DMusic::m_bON = TRUE;
BYTE CT3DMusic::m_bEQ = FALSE;
WORD CT3DMusic::m_wBufferingMax = 256;

FLOAT CT3DMusic::m_vWinCoef[264] = {
	 0.000000000f,  0.000442505f, -0.003250122f,  0.007003784f,
	-0.031082151f,  0.078628540f, -0.100311279f,  0.572036743f,
	-1.144989014f, -0.572036743f, -0.100311279f, -0.078628540f,
	-0.031082151f, -0.007003784f, -0.003250122f, -0.000442505f,
	 0.000015259f,  0.000473022f, -0.003326416f,  0.007919312f,
	-0.030517576f,  0.084182739f, -0.090927124f,  0.600219727f,
	-1.144287109f, -0.543823242f, -0.108856201f, -0.073059082f,
	-0.031478882f, -0.006118774f, -0.003173828f, -0.000396729f,
	 0.000015259f,  0.000534058f, -0.003387451f,  0.008865356f,
	-0.029785154f,  0.089706421f, -0.080688477f,  0.628295898f,
	-1.142211914f, -0.515609741f, -0.116577141f, -0.067520142f,
	-0.031738281f, -0.005294800f, -0.003082275f, -0.000366211f,
	 0.000015259f,  0.000579834f, -0.003433228f,  0.009841919f,
	-0.028884888f,  0.095169067f, -0.069595337f,  0.656219482f,
	-1.138763428f, -0.487472534f, -0.123474121f, -0.061996460f,
	-0.031845093f, -0.004486084f, -0.002990723f, -0.000320435f,
	 0.000015259f,  0.000625610f, -0.003463745f,  0.010848999f,
	-0.027801514f,  0.100540161f, -0.057617184f,  0.683914185f,
	-1.133926392f, -0.459472656f, -0.129577637f, -0.056533810f,
	-0.031814575f, -0.003723145f, -0.002899170f, -0.000289917f,
	 0.000015259f,  0.000686646f, -0.003479004f,  0.011886597f,
	-0.026535034f,  0.105819702f, -0.044784546f,  0.711318970f,
	-1.127746582f, -0.431655884f, -0.134887695f, -0.051132202f,
	-0.031661987f, -0.003005981f, -0.002792358f, -0.000259399f,
	 0.000015259f,  0.000747681f, -0.003479004f,  0.012939452f,
	-0.025085449f,  0.110946655f, -0.031082151f,  0.738372803f,
	-1.120223999f, -0.404083252f, -0.139450073f, -0.045837402f,
	-0.031387329f, -0.002334595f, -0.002685547f, -0.000244141f,
	 0.000030518f,  0.000808716f, -0.003463745f,  0.014022826f,
	-0.023422241f,  0.115921021f, -0.016510010f,  0.765029907f,
	-1.111373901f, -0.376800537f, -0.143264771f, -0.040634155f,
	-0.031005858f, -0.001693726f, -0.002578735f, -0.000213623f,
	 0.000030518f,  0.000885010f, -0.003417969f,  0.015121460f,
	-0.021575928f,  0.120697014f, -0.001068115f,  0.791213989f,
	-1.101211548f, -0.349868774f, -0.146362305f, -0.035552979f,
	-0.030532837f, -0.001098633f, -0.002456665f, -0.000198364f,
	 0.000030518f,  0.000961304f, -0.003372192f,  0.016235352f,
	-0.019531250f,  0.125259399f,  0.015228271f,  0.816864014f,
	-1.089782715f, -0.323318481f, -0.148773193f, -0.030609131f,
	-0.029937742f, -0.000549316f, -0.002349854f, -0.000167847f,
	 0.000030518f,  0.001037598f, -0.003280640f,  0.017349243f,
	-0.017257690f,  0.129562378f,  0.032379150f,  0.841949463f,
	-1.077117920f, -0.297210693f, -0.150497437f, -0.025817871f,
	-0.029281614f, -0.000030518f, -0.002243042f, -0.000152588f,
	 0.000045776f,  0.001113892f, -0.003173828f,  0.018463135f,
	-0.014801024f,  0.133590698f,  0.050354004f,  0.866363525f,
	-1.063217163f, -0.271591187f, -0.151596069f, -0.021179199f,
	-0.028533936f,  0.000442505f, -0.002120972f, -0.000137329f,
	 0.000045776f,  0.001205444f, -0.003051758f,  0.019577026f,
	-0.012115479f,  0.137298584f,  0.069168091f,  0.890090942f,
	-1.048156738f, -0.246505737f, -0.152069092f, -0.016708374f,
	-0.027725220f,  0.000869751f, -0.002014160f, -0.000122070f,
	 0.000061035f,  0.001296997f, -0.002883911f,  0.020690918f,
	-0.009231566f,  0.140670776f,  0.088775635f,  0.913055420f,
	-1.031936646f, -0.221984863f, -0.151962280f, -0.012420653f,
	-0.026840210f,  0.001266479f, -0.001907349f, -0.000106812f,
	 0.000061035f,  0.001388550f, -0.002700806f,  0.021789551f,
	-0.006134033f,  0.143676758f,  0.109161377f,  0.935195923f,
	-1.014617920f, -0.198059082f, -0.151306152f, -0.008316040f,
	-0.025909424f,  0.001617432f, -0.001785278f, -0.000106812f,
	 0.000076294f,  0.001480103f, -0.002487183f,  0.022857666f,
	-0.002822876f,  0.146255493f,  0.130310059f,  0.956481934f,
	-0.996246338f, -0.174789429f, -0.150115967f, -0.004394531f,
	-0.024932859f,  0.001937866f, -0.001693726f, -0.000091553f,
	-0.001586914f, -0.023910521f, -0.148422241f, -0.976852417f,
	 0.152206421f,  0.000686646f, -0.002227783f,  0.000076294f};
FLOAT CT3DMusic::m_vEQ[32];

SFBAND CT3DMusic::m_vSFBT[3][3] = {{{
	{ 0, 4, 8, 12, 16, 20, 24, 30, 36, 44, 52, 62, 74, 90, 110, 134, 162, 196, 238, 288, 342, 418, 576},
	{ 0, 4, 8, 12, 16, 22, 30, 40, 52, 66, 84, 106, 136, 192}}, {
	{ 0, 4, 8, 12, 16, 20, 24, 30, 36, 42, 50, 60, 72, 88, 106, 128, 156, 190, 230, 276, 330, 384, 576},
	{ 0, 4, 8, 12, 16, 22, 28, 38, 50, 64, 80, 100, 126, 192}}, {
	{ 0, 4, 8, 12, 16, 20, 24, 30, 36, 44, 54, 66, 82, 102, 126, 156, 194, 240, 296, 364, 448, 550, 576},
	{ 0, 4, 8, 12, 16, 22, 30, 42, 58, 78, 104, 138, 180, 192}}}, {{
	{ 0, 6, 12, 18, 24, 30, 36, 44, 54, 66, 80, 96, 116, 140, 168, 200, 238, 284, 336, 396, 464, 522, 576},
	{ 0, 4, 8, 12, 18, 24, 32, 42, 56, 74, 100, 132, 174, 192}}, {
	{ 0, 6, 12, 18, 24, 30, 36, 44, 54, 66, 80, 96, 114, 136, 162, 194, 232, 278, 332, 394, 464, 540, 576},
	{ 0, 4, 8, 12, 18, 26, 36, 48, 62, 80, 104, 136, 180, 192}}, {
	{ 0, 6, 12, 18, 24, 30, 36, 44, 54, 66, 80, 96, 116, 140, 168, 200, 238, 284, 336, 396, 464, 522, 576},
	{ 0, 4, 8, 12, 18, 26, 36, 48, 62, 80, 104, 134, 174, 192}}}, {{
	{ 0, 6, 12, 18, 24, 30, 36, 44, 54, 66, 80, 96, 116, 140, 168, 200, 238, 284, 336, 396, 464, 522, 576},
	{ 0, 4, 8, 12, 18, 26, 36, 48, 62, 80, 104, 134, 174, 192}}, {
	{ 0, 6, 12, 18, 24, 30, 36, 44, 54, 66, 80, 96, 116, 140, 168, 200, 238, 284, 336, 396, 464, 522, 576},
	{ 0, 4, 8, 12, 18, 26, 36, 48, 62, 80, 104, 134, 174, 192}}, {
	{ 0, 12, 24, 36, 48, 60, 72, 88, 108, 132, 160, 192, 232, 280, 336, 400, 476, 566, 568, 570, 572, 574, 576},
	{ 0, 8, 16, 24, 36, 52, 72, 96, 124, 160, 162, 164, 166, 192}}}};

int CT3DMusic::m_vBITRATE[3][3][16] = {
	{// MPEG-1
	{ 0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448, 0}, // Layer1
	{ 0, 32, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384, 0}, // Layer2
	{ 0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 0}}, // Layer3
	{// MPEG-2
	{ 0, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256, 0}, // Layer1
	{ 0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, 0}, // Layer2
	{ 0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, 0}}, // Layer3
	{// MPEG-2.5
	{ 0, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256, 0}, // Layer1 (not available)
	{ 0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, 0}, // Layer2 (not available)
	{ 0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, 0}}}; // Layer3

int CT3DMusic::m_vFREQ[3][4] = {
	{ 44100, 48000, 32000, 0}, // MPEG-1
	{ 22050, 24000, 16000, 0}, // MPEG-2
	{ 11025, 12000, 8000, 0}}; // MPEG-2.5

int CT3DMusic::m_vJOINT[16] = {
	64, 64, 64, 64,
	8, 16, 24, 32,
	64, 64, 64, 64,
	32, 32, 32, 32};

BYTE CT3DMusic::m_vBAT[5][4][16] = {
	{ // BAT-A
	{ 0, 1, 3,  5, 6, 7, 8,  9, 10, 11, 12, 13, 14, 15, 16, 17},
	{ 0, 1, 2,  3, 4, 5, 6,  7,  8,  9, 10, 11, 12, 13, 14, 17},
	{ 0, 1, 2,  3, 4, 5, 6, 17,  0,  0,  0,  0,  0,  0,  0,  0},
	{ 0, 1, 2, 17, 0, 0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
	{ // BAT-B
	{ 0, 1, 3,  5, 6, 7, 8,  9, 10, 11, 12, 13, 14, 15, 16, 17},
	{ 0, 1, 2,  3, 4, 5, 6,  7,  8,  9, 10, 11, 12, 13, 14, 17},
	{ 0, 1, 2,  3, 4, 5, 6, 17,  0,  0,  0,  0,  0,  0,  0,  0},
	{ 0, 1, 2, 17, 0, 0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
	{ // BAT-C
	{ 0, 1, 2, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0,  0,  0},
	{ 0, 1, 2, 4, 5, 6, 7, 8, 0,  0,  0,  0,  0,  0,  0,  0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0,  0,  0}},
	{ // BAT-D
	{ 0, 1, 2, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0,  0,  0},
	{ 0, 1, 2, 4, 5, 6, 7, 8, 0,  0,  0,  0,  0,  0,  0,  0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0,  0,  0}},
	{ // BAT-E
	{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0,  0},
	{ 0, 1, 2, 4, 5, 6, 7, 8, 0, 0,  0,  0,  0,  0,  0,  0},
	{ 0, 1, 2, 4, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0,  0}}};

BYTE CT3DMusic::m_vBATCount[5][4] = {
	{ 3, 8, 12,  4},
	{ 3, 8, 12,  7},
	{ 2, 0,  6,  0},
	{ 2, 0, 10,  0},
	{ 4, 0,  7, 19}};

FLOAT CT3DMusic::m_vValueL1[18];

FLOAT CT3DMusic::m_vValueL2[18];
FLOAT CT3DMusic::m_vTableSF[64];

WORD CT3DMusic::m_vTableGRP9[1024][3];
BYTE CT3DMusic::m_vTableGRP5[128][3];
BYTE CT3DMusic::m_vTableGRP3[32][3];

FLOAT CT3DMusic::m_vValueL3[262];
FLOAT CT3DMusic::m_vScaleL3[2][4][32];
FLOAT CT3DMusic::m_vPowL3[2 * IS_MAX];
FLOAT CT3DMusic::m_vSubBlockL3[8];
FLOAT CT3DMusic::m_vCSA[8][2];

FLOAT CT3DMusic::m_vISData2[2][2][64][2];
FLOAT CT3DMusic::m_vISData1[2][8][2];

FLOAT CT3DMusic::m_vCoef32[31];
FLOAT CT3DMusic::m_vCoef[9][4];
FLOAT CT3DMusic::m_vW1[18];
FLOAT CT3DMusic::m_vW2[9];

FLOAT CT3DMusic::m_vWIN[4][36];
FLOAT CT3DMusic::m_fCoef87;
FLOAT CT3DMusic::m_vV1[6];
FLOAT CT3DMusic::m_vV2[3];


CT3DMusic::CT3DMusic()
{
	for( int i=0; i<MPEG_BUFFER_COUNT; i++)
	{
		ZeroMemory( &m_vPARAM[i], sizeof(MPEG_DECODE_PARAM));
		ZeroMemory( &m_vHEADER[i], sizeof(WAVEHDR));

		m_vBUFSIZE[i] = 0;
		m_pBUF[i] = NULL;
		m_vOUTPUT[i] = NULL;
	}

	for( i=0; i<2; i++)
	{
		for( int j=0; j<2; j++)
			m_vCOUNT[i][j] = 0;

		m_vL3POS[i] = 0;
		m_vSBT[i] = 0;

		m_vSBTBUF[i] = NULL;
	}

	m_bSTATE = MMS_CLOSED;
	m_hThread = NULL;
	m_hEVENT = NULL;
	m_hPAUSE = NULL;
	m_hCOMP = NULL;

	m_hFILE = INVALID_HANDLE_VALUE;
	m_hMAP = NULL;

	m_dwStartTick = 0;
	m_dwPlayTick = 0;

	m_dwPartCount = 0;
	m_dwPart = 0;
	m_wFRAME = 0;
	m_bINDEX = 0;
	m_strFILE.Empty();

	m_nBandLimit12 = 0;
	m_nBandLimit21 = 0;
	m_nBandLimit = 0;

	m_nTimeDivider = 0;
	m_nMixedCBEnd = 0;
	m_nBATCount = 0;
	m_nMaxSB = 0;
	m_nLimit = 0;
	m_nGroup = 0;

	m_pFrameProc = NULL;
	m_pXFormProc = NULL;
	m_pSBTProc = NULL;
	m_pSAMPLE = NULL;
	m_pL3BUF = NULL;
	m_pFRAME = NULL;
	m_pDATA = NULL;

	m_dwSTART = 0;
	m_dwEND = 0;
	m_dwPOS = 0;
	m_dwSEEK = 0;

	m_bBATMode = FALSE;
	m_bLOOP = FALSE;

	m_bFadeVolume = VOLUME_MAX;
	m_bVolume = VOLUME_MAX;

	InitializeCriticalSection(&m_cs);
}

CT3DMusic::~CT3DMusic()
{
	Unlock();
	DeleteCriticalSection(&m_cs);
}

BYTE CT3DMusic::InitMusic( LPTSTR szFile)
{
	m_strFILE = szFile;
	return TRUE;
}

int CT3DMusic::GetID3TagSIZE( LPBYTE pDATA)
{
	int nResult = 0;

	for( int i=0; i<4; i++)
		nResult += pDATA[3 - i] << (7 * i);

	return nResult;
}

void CT3DMusic::DecodeINIT( LPDIRECTSOUND8 pDS)
{
	m_vDecodeOption.m_nReduction = 0;
	m_vDecodeOption.m_nConvert = 0;
	m_vDecodeOption.m_nFreqLimit = 48000;
	m_pDS = pDS;

	DecodeL1INIT();
	DecodeL2INIT();
	DecodeL3INIT();
}

void CT3DMusic::DecodeL1INIT()
{
	int nValue = 4;

	for( int i=1; i<16; i++)
	{
		m_vValueL1[i] = 2.0f / FLOAT(nValue - 1);
		nValue = nValue << 1;
	}
}

void CT3DMusic::DecodeL2INIT()
{
	long vValueL2[18] = { 0, 3, 5, 7, 9, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191, 16383, 32767, 65535};

	for( int i=1; i<18; i++)
		m_vValueL2[i] = 2.0f / FLOAT(vValueL2[i]);

	for( i=0; i<64; i++)
		m_vTableSF[i] = FLOAT(32768.0f * 2.0f * powf( 2.0f, -i / 3.0f));

	for( i=0; i<32; i++)
	{
		int nCode = i;

		for( int j=0; j<3; j++)
		{
			m_vTableGRP3[i][j] = BYTE((nCode % 3) - 1);
			nCode /= 3;
		}
	}

	for( i=0; i<128; i++)
	{
		int nCode = i;

		for( int j=0; j<3; j++)
		{
			m_vTableGRP5[i][j] = BYTE((nCode % 5) - 2);
			nCode /= 5;
		}
	}

	for( i=0; i<1024; i++)
	{
		int nCode = i;

		for( int j=0; j<3; j++)
		{
			m_vTableGRP9[i][j] = WORD((nCode % 9) - 4);
			nCode /= 9;
		}
	}
}

void CT3DMusic::DecodeL3INIT()
{
	InitQuant();
	InitAlias();
	InitMSIS();
	InitFDCT();
	InitIMDCT();
	InitHWIN();
}

void CT3DMusic::InitQuant()
{
	for( int i=0; i<262; i++)
		m_vValueL3[i] = powf( 2.0f, 0.25f * (i - 216 + MPEG_GLOBAL_GAIN_SCALE));

	for( i=0; i<2; i++)
		for( int j=0; j<4; j++)
			for( int k=0; k<32; k++)
				m_vScaleL3[i][j][k] = powf( 2.0f, -0.5f * (1 + i) * (k + j));

	for( i=0; i<64; i++)
	{
		FLOAT fInverse = FLOAT(i - 32);
		m_vPowL3[i] = fInverse * powf( fabs(fInverse), 1.0f / 3.0f);
	}

	for( i=0; i<8; i++)
		m_vSubBlockL3[i] = powf( 2.0f, 0.25f * -8.0f * i);
}

void CT3DMusic::InitAlias()
{
	FLOAT vCi[8] = { -0.6f, -0.535f, -0.33f, -0.185f, -0.095f, -0.041f, -0.0142f, -0.0037f};

	for( int i=0; i<8; i++)
	{
		m_vCSA[i][1] = vCi[i] / sqrtf(1.0f + vCi[i] * vCi[i]);
		m_vCSA[i][0] = 1.0f / sqrtf(1.0f + vCi[i] * vCi[i]);
	}
}

void CT3DMusic::InitMSIS()
{
	FLOAT vFactor[2] = { 1.0f, sqrtf(2.0f)};
	FLOAT fTheta = D3DX_PI / 12.0f;

	for( int i=0; i<7; i++)
	{
		FLOAT fSIN = sinf(i * fTheta);
		FLOAT fCOS = cosf(i * fTheta);

		m_vISData1[1][i][0] = vFactor[1] * (fSIN / (fSIN + fCOS));
		m_vISData1[1][i][1] = vFactor[1] * (fCOS / (fSIN + fCOS));

		m_vISData1[0][i][0] = fSIN / (fSIN + fCOS);
		m_vISData1[0][i][1] = fCOS / (fSIN + fCOS);
	}

	m_vISData1[0][7][0] = 1.0f;
	m_vISData1[0][7][1] = 0.0f;

	m_vISData1[1][7][0] = 1.0f;
	m_vISData1[1][7][1] = 1.0f;

	for( i=0; i<2; i++)
	{
		fTheta = powf( 2.0f, -0.25f * (1 + i));

		for( int j=0; j<2; j++)
		{
			int nCount = 1;
			int nSF = 0;

			for( int k=0; k<6; k++)
			{
				for( int l=0; l<nCount - 1; l++)
				{
					if(!l)
					{
						m_vISData2[i][j][nSF][0] = vFactor[j];
						m_vISData2[i][j][nSF][1] = vFactor[j];
					}
					else if(l&1)
					{
						m_vISData2[i][j][nSF][0] = vFactor[j] * powf( fTheta, (FLOAT(l) + 1.0f) / 2.0f);
						m_vISData2[i][j][nSF][1] = vFactor[j];
					}
					else
					{
						m_vISData2[i][j][nSF][1] = vFactor[j] * powf( fTheta, FLOAT(l) / 2.0f);
						m_vISData2[i][j][nSF][0] = vFactor[j];
					}

					nSF++;
				}

				if(!j)
				{
					m_vISData2[i][j][nSF][0] = 1.0f;
					m_vISData2[i][j][nSF][1] = 0.0f;
				}
				else
				{
					m_vISData2[i][j][nSF][0] = 1.0f;
					m_vISData2[i][j][nSF][1] = 1.0f;
				}

				nCount += nCount;
				nSF++;
			}
		}
	}
}

void CT3DMusic::InitFDCT()
{
	int nCount = 16;
	int nIndex = 0;

	for( int i=0; i<5; i++)
	{
		for( int j=0; j<nCount; j++)
		{
			m_vCoef32[nIndex] = 0.5f / cosf((D3DX_PI / (4.0f * nCount)) * (2.0f * j + 1.0f));
			nIndex++;
		}

		nCount /= 2;
	}
}

void CT3DMusic::InitIMDCT()
{
	int nCount = 18;
	FLOAT fTheta = D3DX_PI / (4.0f * nCount);

	for( int i=0; i<nCount; i++)
		m_vW1[i] = 2.0f * cosf(fTheta * (2.0f * i + 1.0f));

	for( i=0; i<9; i++)
		m_vW2[i] = 2.0f * cosf(2.0f * fTheta * (2.0f * i + 1.0f));

	fTheta = D3DX_PI / (2.0f * nCount);
	for( i=0; i<9; i++)
		for( int j=0; j<4; j++)
			m_vCoef[i][j] = cosf(fTheta * (2.0f * i) * (2.0f * j + 1.0f));

	nCount = 6;
	fTheta = D3DX_PI / (4.0f * nCount);

	for( i=0; i<nCount; i++)
		m_vV1[i] = cosf(fTheta * (2.0f * i + 1.0f));

	for( i=0; i<3; i++)
		m_vV2[i] = 2.0f * cosf(2.0f * fTheta * (2.0f * i + 1.0f));

	fTheta = D3DX_PI / (2.0f * nCount);
	m_fCoef87 = 2.0f * cosf(2.0f * fTheta);
}

void CT3DMusic::InitHWIN()
{
	// Type 0
	for( int i=0; i<36; i++)
		m_vWIN[0][i] = sinf(D3DX_PI / 36.0f * (i + 0.5f));

	// Type 1
	for( i=0; i<18; i++)
		m_vWIN[1][i] = sinf(D3DX_PI / 36.0f * (i + 0.5f));

	for( i=18; i<24; i++)
		m_vWIN[1][i] = 1.0f;

	for( i=24; i<30; i++)
		m_vWIN[1][i] = sinf(D3DX_PI / 12.0f * (i + 0.5f - 18.0f));

	for( i=30; i<36; i++)
		m_vWIN[1][i] = 0.0f;

	// Type 2
	for( i=0; i<12; i++)
		m_vWIN[2][i] = sinf(D3DX_PI / 12.0f * (i + 0.5f));

	for( i=12; i<36; i++)
		m_vWIN[2][i] = 0.0f;

	// Type 3
	for( i=0; i<6; i++)
		m_vWIN[3][i] = 0.0f;

	for( i=6; i<12; i++)
		m_vWIN[3][i] = sinf(D3DX_PI / 12.0f * (i + 0.5f - 6.0f));

	for( i=12; i<18; i++)
		m_vWIN[3][i] = 1.0f;

	for( i=18; i<36; i++)
		m_vWIN[3][i] = sinf(D3DX_PI / 36.0f * (i + 0.5f));

	for( i=0; i<4; i++)
		if( i != 2 )
			for( int j=9; j<36; j++)
				m_vWIN[i][j] = -m_vWIN[i][j];

	for( i=3; i<12; i++)
		m_vWIN[2][i] = -m_vWIN[2][i];
}

BYTE CT3DMusic::Play()
{
	if( !CTachyonMedia::m_bON || !m_bON )
		return FALSE;

	if( m_bSTATE == MMS_PAUSE )
	{
		m_bSTATE = MMS_PLAY;

		if(!m_hPAUSE)
		{
			Unlock();
			return FALSE;
		}
		else
			SetEvent(m_hPAUSE);

		return TRUE;
	}

	if(!Lock())
		return FALSE;

	WORD wBuffering = GetBufferingCount(
		m_dwSTART,
		m_dwEND);
	DWORD dwFRAME = Tick2Frame(m_dwEND) - Tick2Frame(m_dwSTART) + 1;
	DWORD dwThreadID;

	m_dwPartCount = dwFRAME / wBuffering - (dwFRAME % wBuffering ? 0 : 1);
	m_dwPart = 0;
	m_bINDEX = 0;

	ZeroMemory( &m_vPARAM[m_bINDEX], sizeof(MPEG_DECODE_PARAM));
	m_vBUFSIZE[m_bINDEX] = 0;
	m_dwPOS = 0;

	if(!ResetVolume())
	{
		Unlock();
		return FALSE;
	}

	m_hThread = CreateThread(
		NULL, 0,
		DecodeMPEG,
		(LPVOID) this,
		CREATE_SUSPENDED,
		&dwThreadID);

	if(!m_hThread)
	{
		Unlock();
		return FALSE;
	}

	SetThreadPriority(
		m_hThread,
		THREAD_PRIORITY_TIME_CRITICAL);

	m_bSTATE = MMS_PLAY;
	ResumeThread(m_hThread);

	return TRUE;
}

BYTE CT3DMusic::Stop()
{
	Unlock();
	return TRUE;
}

BYTE CT3DMusic::Pause()
{
	if( m_bSTATE == MMS_PLAY )
	{
		if( !m_hThread || !m_hEVENT || !m_hPAUSE || !m_hCOMP )
		{
			Unlock();
			return FALSE;
		}
		else
		{
			SMART_LOCKCS(&m_cs);

			m_bSTATE = MMS_PAUSE;
			SetEvent(m_hEVENT);
		}

		WaitForSingleObject(
			m_hCOMP,
			INFINITE);
	}

	return TRUE;
}

BYTE CT3DMusic::ResetVolume()
{
	SMART_LOCKCS(&m_cs);

	FLOAT fVolume = CTachyonMedia::m_bBACK ? 0.0f : FLOAT(CTachyonMedia::m_bMasterVolume) * 
		FLOAT(m_bMasterVolume) * FLOAT(m_bFadeVolume) * FLOAT(m_bVolume) /
		FLOAT(VOLUME_MAX * VOLUME_MAX * VOLUME_MAX * VOLUME_MAX);

	if( fVolume > 0.0f )
		fVolume = -2000.0f * log10(1.0f / fVolume);
	else
		fVolume = DSBVOLUME_MIN;

	for( int i=0; i<MPEG_BUFFER_COUNT; i++)
		if( m_vOUTPUT[i] )
			m_vOUTPUT[i]->SetVolume((LONG)fVolume);

	return TRUE;
}

BYTE CT3DMusic::SetPos( DWORD dwPOS)
{
	BYTE bPLAY = m_bSTATE == MMS_PLAY ? TRUE : FALSE;

	if( m_bSTATE == MMS_PLAY || m_bSTATE == MMS_PAUSE )
	{
		Stop();
		Lock();
	}
	m_dwSEEK = dwPOS;

	if(bPLAY)
		Play();

	return TRUE;
}

DWORD CT3DMusic::GetLength()
{
	return m_dwEND - m_dwSTART;
}

DWORD CT3DMusic::GetPos()
{
	return m_dwSEEK;
}

BYTE CT3DMusic::IsPlay()
{
	return m_bSTATE == MMS_PLAY ? TRUE : FALSE;
}

BYTE CT3DMusic::ResetDSBUF( BYTE bINDEX)
{
	SMART_LOCKCS(&m_cs);
	if( !m_pDS || !m_vOUTPUT[bINDEX] )
		return FALSE;

	LPBYTE pBUF = NULL;
	DWORD dwSIZE = 0;

	if(FAILED(m_vOUTPUT[bINDEX]->Stop()))
	{
		m_vOUTPUT[bINDEX]->Release();
		m_vOUTPUT[bINDEX] = NULL;

		return FALSE;
	}

	if(FAILED(m_vOUTPUT[bINDEX]->SetCurrentPosition(0)))
	{
		m_vOUTPUT[bINDEX]->Release();
		m_vOUTPUT[bINDEX] = NULL;

		return FALSE;
	}

	if(FAILED(m_vOUTPUT[bINDEX]->Lock( 0, 0, (LPVOID *) &pBUF, &dwSIZE, NULL, NULL, DSBLOCK_ENTIREBUFFER)))
	{
		m_vOUTPUT[bINDEX]->Release();
		m_vOUTPUT[bINDEX] = NULL;

		return FALSE;
	}

	if( dwSIZE > m_vHEADER[bINDEX].dwBufferLength )
		ZeroMemory( pBUF + m_vHEADER[bINDEX].dwBufferLength, dwSIZE - m_vHEADER[bINDEX].dwBufferLength);

	if(m_vHEADER[bINDEX].dwBufferLength)
		memcpy( pBUF, m_vHEADER[bINDEX].lpData, m_vHEADER[bINDEX].dwBufferLength);

	if(FAILED(m_vOUTPUT[bINDEX]->Unlock( pBUF, dwSIZE, NULL, 0)))
	{
		m_vOUTPUT[bINDEX]->Release();
		m_vOUTPUT[bINDEX] = NULL;

		return FALSE;
	}

	return TRUE;
}

BYTE CT3DMusic::PlayBUF( BYTE bINDEX)
{
	SMART_LOCKCS(&m_cs);

	if(m_vOUTPUT[bINDEX])
	{
		DWORD dwSTATUS;

		if(FAILED(m_vOUTPUT[bINDEX]->GetStatus(&dwSTATUS)))
		{
			m_vOUTPUT[bINDEX]->Release();
			m_vOUTPUT[bINDEX] = NULL;

			return FALSE;
		}

		if( dwSTATUS & DSBSTATUS_BUFFERLOST )
		{
			while( m_vOUTPUT[bINDEX]->Restore() == DSERR_BUFFERLOST );

			if(!ResetDSBUF(bINDEX))
				return FALSE;

			if(!ResetVolume())
				return FALSE;
		}
	}
	else
		return FALSE;

	if(FAILED(m_vOUTPUT[bINDEX]->Play( 0, 0, 0)))
		return FALSE;

	return TRUE;
}

BYTE CT3DMusic::Lock()
{
	if( !m_pDS )
		return FALSE;

	static int vBITRATE[2][16] = {
		{ 0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 0},
		{ 0,  8, 16, 24, 32, 64, 80, 56,  64, 128, 160, 112, 128, 256, 320, 0}};

	static int vFREQ[2][4] = {
		{ 44100, 48000, 32000, 0},
		{ 22050, 24000, 16000, 0}};
	Unlock();

	m_hFILE = CreateFile(
		LPCSTR(m_strFILE),
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL|
		FILE_FLAG_RANDOM_ACCESS,
		NULL);

	if( m_hFILE == INVALID_HANDLE_VALUE )
	{
		Unlock();
		return FALSE;
	}

	m_hMAP = CreateFileMapping(
		m_hFILE,
		NULL,
		PAGE_READONLY,
		0, 0, NULL);

	if(!m_hMAP)
	{
		Unlock();
		return FALSE;
	}

	m_pDATA = (LPBYTE) MapViewOfFile(
		m_hMAP,
		FILE_MAP_READ,
		0, 0, 0);

	if(!m_pDATA)
	{
		Unlock();
		return FALSE;
	}

	if(!memcmp( m_pDATA, "ID3", 3))
	{
		m_pFRAME = m_pDATA + GetID3TagSIZE(&m_pDATA[6]) + 10;

		if( m_pDATA[3] == 3 && (m_pDATA[5] & 0x40) )
			m_pFRAME = m_pFRAME + GetID3TagSIZE(m_pFRAME);
	}
	else if(memcmp( m_pDATA, "RIFF", 4))
		m_pFRAME = m_pDATA;
	else if(!memcmp( m_pDATA + 8, "WAVEfmt", 7))
	{
		if(!memcmp( m_pDATA + 50, "fact", 4))
			m_pFRAME = m_pDATA + 70;
		else if(!memcmp( m_pDATA + 52, "fact", 4))
			m_pFRAME = m_pDATA + 72;
		else if(!memcmp( m_pDATA + 50, "data", 4))
			m_pFRAME = m_pDATA + 58;
	}
	else if(!memcmp( m_pDATA + 8, "RMP3", 4))
	{
		if(!memcmp( m_pDATA + 12, "data", 4))
			m_pFRAME = m_pDATA + 16;
	}

	if(!m_pFRAME)
	{
		Unlock();
		return FALSE;
	}

	DWORD dwSIZE = GetFileSize(
		m_hFILE,
		NULL);
	dwSIZE -= DWORD(m_pFRAME - m_pDATA);

	if( m_pDATA[dwSIZE - 128] == 'T' &&
		m_pDATA[dwSIZE - 127] == 'A' &&
		m_pDATA[dwSIZE - 126] == 'G' )
		dwSIZE -= 128;

	WORD wMPEG = (m_pFRAME[1] & 8) == 0 ? 1 : 0;
	WORD wFREQ = vFREQ[wMPEG][(m_pFRAME[2] >> 2) & 3];
	WORD wBITRATE = vBITRATE[wMPEG][m_pFRAME[2] >> 4];

	m_wFRAME = 144000 * wBITRATE / wFREQ;
	MPEG_DECODE_INFO vINFO;

	m_dwEND = (dwSIZE / m_wFRAME) * m_wFRAME / wBITRATE * 8;
	m_dwSTART = 0;

	GetMPEGInfo(
		&vINFO,
		m_pFRAME,
		m_wFRAME);

	InitMPEG(
		&vINFO.m_vHeader,
		dwSIZE);

	DSBUFFERDESC vDESC;
	WAVEFORMATEX vFMT;

	ZeroMemory( &vFMT, sizeof(WAVEFORMATEX));
	vFMT.wFormatTag = WAVE_FORMAT_PCM;
	vFMT.nChannels = vINFO.m_nChannels;
	vFMT.nBlockAlign = vFMT.nChannels * 2;

	vFMT.nAvgBytesPerSec = wFREQ * vFMT.nChannels * 2;
	vFMT.nSamplesPerSec = wFREQ;
	vFMT.wBitsPerSample = vINFO.m_nBitsPerSample;

	ZeroMemory( &vDESC, sizeof(DSBUFFERDESC));
	vDESC.dwSize = sizeof(DSBUFFERDESC);
	vDESC.dwFlags = DSBCAPS_CTRLVOLUME|DSBCAPS_GLOBALFOCUS;
	vDESC.dwBufferBytes = vINFO.m_nOutputSize * m_wBufferingMax;
	vDESC.guid3DAlgorithm = DS3DALG_DEFAULT;
	vDESC.lpwfxFormat = &vFMT;

	for( int i=0; i<MPEG_BUFFER_COUNT; i++)
	{
		m_pBUF[i] = GlobalAlloc( GPTR, vDESC.dwBufferBytes);

		if( !m_pBUF[i] || FAILED(m_pDS->CreateSoundBuffer( &vDESC, &m_vOUTPUT[i], NULL)) )
		{
			Unlock();
			return FALSE;
		}

		ZeroMemory( m_pBUF[i], vDESC.dwBufferBytes);
	}

	m_hEVENT = CreateEvent(
		NULL,
		FALSE,
		FALSE,
		NULL);

	if(!m_hEVENT)
	{
		Unlock();
		return FALSE;
	}

	m_hPAUSE = CreateEvent(
		NULL,
		FALSE,
		FALSE,
		NULL);

	if(!m_hPAUSE)
	{
		Unlock();
		return FALSE;
	}

	m_hCOMP = CreateEvent(
		NULL,
		FALSE,
		FALSE,
		NULL);

	if(!m_hCOMP)
	{
		Unlock();
		return FALSE;
	}

	m_nTimeDivider = wFREQ / 8 * vFMT.wBitsPerSample * vFMT.nChannels;
	m_bSTATE = MMS_OPEN;

	ResetEvent(m_hEVENT);
	ResetEvent(m_hPAUSE);
	ResetEvent(m_hCOMP);

	return TRUE;
}

void CT3DMusic::Unlock()
{
	if( m_bSTATE == MMS_PLAY ||
		m_bSTATE == MMS_PAUSE )
		m_bSTATE = MMS_OPEN;

	if(m_hThread)
	{
		if(m_hPAUSE)
			SetEvent(m_hPAUSE);

		if(m_hEVENT)
			SetEvent(m_hEVENT);

		WaitForSingleObject(
			m_hThread,
			INFINITE);

		m_hThread = NULL;
	}

	if(m_hEVENT)
	{
		CloseHandle(m_hEVENT);
		m_hEVENT = NULL;
	}

	if(m_hPAUSE)
	{
		CloseHandle(m_hPAUSE);
		m_hPAUSE = NULL;
	}

	if(m_hCOMP)
	{
		CloseHandle(m_hCOMP);
		m_hCOMP = NULL;
	}

	for( int i=0; i<MPEG_BUFFER_COUNT; i++)
	{
		if(m_vOUTPUT[i])
		{
			m_vOUTPUT[i]->Stop();
			m_vOUTPUT[i]->Release();

			m_vOUTPUT[i] = NULL;
		}

		if(m_pBUF[i])
		{
			GlobalFree(m_pBUF[i]);
			m_pBUF[i] = NULL;
		}

		ZeroMemory( &m_vHEADER[i], sizeof(WAVEHDR));
	}

	for( i=0; i<2; i++)
	{
		for( int j=0; j<2; j++)
			m_vCOUNT[i][j] = 0;

		if(m_vSBTBUF[i])
		{
			delete[] m_vSBTBUF[i];
			m_vSBTBUF[i] = NULL;
		}

		m_vL3POS[i] = 0;
		m_vSBT[i] = 0;
	}

	if(m_pSAMPLE)
	{
		delete m_pSAMPLE;
		m_pSAMPLE = NULL;
	}

	if(m_pL3BUF)
	{
		delete[] m_pL3BUF;
		m_pL3BUF = NULL;
	}

	if(m_pDATA)
	{
		UnmapViewOfFile(m_pDATA);

		m_pFRAME = NULL;
		m_pDATA = NULL;
	}

	if(m_hMAP)
	{
		CloseHandle(m_hMAP);
		m_hMAP = NULL;
	}

	if( m_hFILE == INVALID_HANDLE_VALUE )
	{
		CloseHandle(m_hFILE);
		m_hFILE = INVALID_HANDLE_VALUE;
	}

	m_pXFormProc = NULL;
	m_pSBTProc = NULL;
	m_bBATMode = FALSE;

	m_nBandLimit21 = 0;
	m_nBandLimit12 = 0;
	m_nBandLimit = 0;

	m_nTimeDivider = 0;
	m_nMixedCBEnd = 0;
	m_nBATCount = 0;
	m_nMaxSB = 0;
	m_nLimit = 0;
	m_wFRAME = 0;
	m_nGroup = 0;
	m_bINDEX = 0;

	m_dwStartTick = 0;
	m_dwPlayTick = 0;

	m_dwPartCount = 0;
	m_dwPart = 0;

	m_dwSTART = 0;
	m_dwEND = 0;
	m_dwPOS = 0;

	m_bSTATE = MMS_CLOSED;
}

void CT3DMusic::InitMPEG( LPMPEG_HEADER pHEADER,
						  DWORD dwSIZE)
{
	for( int i=0; i<2; i++)
	{
		for( int j=0; j<2; j++)
			m_vCOUNT[i][j] = 0;

		m_vSBTBUF[i] = new FLOAT[512];
		m_vSBT[i] = 0;

		ZeroMemory( m_vSBTBUF[i], 512 * sizeof(FLOAT));
	}

	m_pSAMPLE = new SAMPLEBUF;
	ZeroMemory( m_pSAMPLE, sizeof(SAMPLEBUF));

	switch(pHEADER->m_nLayer)
	{
	case 1	:
		{
			m_pFrameProc = (FRAME_PROC) DecodeL1;
			InitL1(pHEADER);
		}

		break;

	case 2	:
		{
			m_pFrameProc = (FRAME_PROC) DecodeL2;
			InitL2(pHEADER);
		}

		break;

	case 3	:
		{
			m_pFrameProc = (FRAME_PROC) DecodeL3;
			InitL3(pHEADER);
		}

		break;
	}
}

void CT3DMusic::InitL1( LPMPEG_HEADER pHEADER)
{
	int nLimit = 32 >> m_vDecodeOption.m_nReduction;
	int nFREQ = GetFREQ(pHEADER);

	m_nBATCount = 32;
	m_nMaxSB = m_nBATCount;
	m_nLimit = (m_vDecodeOption.m_nFreqLimit * 64L + nFREQ / 2) / nFREQ;

	if( nLimit > 8 )
		nLimit--;

	if( m_nLimit > nLimit )
		m_nLimit = nLimit;

	if( m_nLimit > m_nMaxSB )
		m_nLimit = m_nMaxSB;

	if( pHEADER->m_nMode != 3 )
	{
		m_nBATCount *= 2;
		m_nMaxSB *= 2;
		m_nLimit *= 2;
	}

	m_pSBTProc = GetSBTProc(
		m_vDecodeOption.m_nConvert & 8 ? 1 : 0,
		m_vDecodeOption.m_nReduction,
		pHEADER->m_nMode == 3 ? 0 : 1 + m_vDecodeOption.m_nConvert);
}

void CT3DMusic::InitL2( LPMPEG_HEADER pHEADER)
{
	static char vQT[4][3][16] = {{
		{ 1, -1, -1, -1, 2, -1, 2, 0, 0, 0, 1, 1, 1, 1, 1, -1},
		{ 0, -1, -1, -1, 2, -1, 2, 0, 0, 0, 0, 0, 0, 0, 0, -1},
		{ 1, -1, -1, -1, 3, -1, 3, 0, 0, 0, 1, 1, 1, 1, 1, -1}}, {
		{ 1, -1, -1, -1, 2, -1, 2, 0, 0, 0, 1, 1, 1, 1, 1, -1},
		{ 0, -1, -1, -1, 2, -1, 2, 0, 0, 0, 0, 0, 0, 0, 0, -1},
		{ 1, -1, -1, -1, 3, -1, 3, 0, 0, 0, 1, 1, 1, 1, 1, -1}}, {
		{ 1, -1, -1, -1, 2, -1, 2, 0, 0, 0, 1, 1, 1, 1, 1, -1},
		{ 0, -1, -1, -1, 2, -1, 2, 0, 0, 0, 0, 0, 0, 0, 0, -1},
		{ 1, -1, -1, -1, 3, -1, 3, 0, 0, 0, 1, 1, 1, 1, 1, -1}}, {
		{ 1, 2, 2, 0, 0, 0, 1, 1, 1, 1, 1, -1, -1, -1, -1, -1},
		{ 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1},
		{ 1, 3, 3, 0, 0, 0, 1, 1, 1, 1, 1, -1, -1, -1, -1, -1}}};

	int nLimit = 32 >> m_vDecodeOption.m_nReduction;
	int nFREQ = GetFREQ(pHEADER);

	m_nBATCount = pHEADER->m_nVersion == 1 ? vQT[pHEADER->m_nMode][pHEADER->m_nFreqIndex][pHEADER->m_nBitrateIndex] : 4;
	m_nMaxSB = 0;
	m_bBATMode = FALSE;

	if( m_nBATCount < 0 )
		return;

	for( int i=0; i<4; i++)
		m_nMaxSB += m_vBATCount[m_nBATCount][i];
	m_nLimit = (m_vDecodeOption.m_nFreqLimit * 64L + nFREQ / 2) / nFREQ;

	if( nLimit > 8 )
		nLimit--;

	if( m_nLimit > nLimit )
		m_nLimit = nLimit;

	if( m_nLimit > m_nMaxSB )
		m_nLimit = m_nMaxSB;

	if( pHEADER->m_nMode != 3 )
	{
		m_bBATMode = TRUE;
		m_nMaxSB *= 2;
		m_nLimit *= 2;
	}

	m_pSBTProc = GetSBTProc(
		m_vDecodeOption.m_nConvert & 8 ? 1 : 0,
		m_vDecodeOption.m_nReduction,
		pHEADER->m_nMode == 3 ? 0 : 1 + m_vDecodeOption.m_nConvert);
}

void CT3DMusic::InitL3( LPMPEG_HEADER pHEADER)
{
	int nLimit = 32 >> m_vDecodeOption.m_nReduction;
	int nFREQ = GetFREQ(pHEADER);
	int nVersion = pHEADER->m_nVersion - 1;

	for( int i=0; i<2; i++)
		m_vL3POS[i] = 0;

	m_pL3BUF = new BYTE[DEF_NBUF];
	ZeroMemory( m_pL3BUF, DEF_NBUF);

	m_nMixedCBEnd = nVersion ? 6 : 8;
	m_nGroup = 0;
	m_nLimit = (m_vDecodeOption.m_nFreqLimit * 64L + nFREQ / 2) / nFREQ;

	if( nLimit > 8 )
		nLimit--;

	if( m_nLimit > nLimit )
		m_nLimit = nLimit;
	nLimit = 18 * m_nLimit;

	if(nVersion)
	{
		m_nBandLimit12 = 3 * m_vSFBT[nVersion][pHEADER->m_nFreqIndex].m_nS[12];
		m_nBandLimit21 = m_vSFBT[nVersion][pHEADER->m_nFreqIndex].m_nL[21];
		m_nBandLimit = m_nBandLimit21;
	}
	else
	{
		m_nBandLimit12 = 3 * m_vSFBT[nVersion][pHEADER->m_nFreqIndex].m_nS[13];
		m_nBandLimit21 = m_vSFBT[nVersion][pHEADER->m_nFreqIndex].m_nL[22];
		m_nBandLimit = m_nBandLimit21;
	}
	m_nBandLimit += 8;

	if( m_nBandLimit > nLimit )
		m_nBandLimit = nLimit;

	if( m_nBandLimit21 > m_nBandLimit )
		m_nBandLimit21 = m_nBandLimit;

	if( m_nBandLimit12 > m_nBandLimit )
		m_nBandLimit12 = m_nBandLimit;

	int nCH = pHEADER->m_nMode == 3 ? 0 : 1;
	if(m_vDecodeOption.m_nConvert)
		nCH = 0;

	m_pSBTProc = GetL3SBTProc(
		m_vDecodeOption.m_nConvert & 8 ? 1 : 0,
		m_vDecodeOption.m_nReduction, nCH);
	m_pXFormProc = GetXFormProc(pHEADER->m_nMode == 3 ? 0 : 1 + m_vDecodeOption.m_nConvert);
}

XFORM_PROC CT3DMusic::GetXFormProc( int nDUAL)
{
	static XFORM_PROC vXFORM[5] = {
		XFormDualL,
		XFormDual,
		XFormDualMono,
		XFormDualL,
		XFormDualR};

	return vXFORM[nDUAL];
}

SBT_PROC CT3DMusic::GetL3SBTProc( int nTYPE,
								  int nBIT,
								  int nDUAL)
{
	static SBT_PROC vSBT[2][3][2] = {{{
		L3SBTMono32S,
		L3SBTDual32S}, {
		L3SBTMono16S,
		L3SBTDual16S}, {
		L3SBTMono8S,
		L3SBTDual8S}}, {{
		L3SBTMono32B,
		L3SBTDual32B}, {
		L3SBTMono16B,
		L3SBTDual16B}, {
		L3SBTMono8B,
		L3SBTDual8B}}};

	return vSBT[nTYPE][nBIT][nDUAL];
}

SBT_PROC CT3DMusic::GetSBTProc( int nTYPE,
							    int nBIT,
								int nDUAL)
{
	static SBT_PROC vSBT[2][3][2] = {{{
		SBTMono32S,
		SBTDual32S}, {
		SBTMono16S,
		SBTDual16S}, {
		SBTMono8S,
		SBTDual8S}}, {{
		SBTMono32B,
		SBTDual32B}, {
		SBTMono16B,
		SBTDual16B}, {
		SBTMono8B,
		SBTDual8B}}};

	return vSBT[nTYPE][nBIT][nDUAL];
}

BYTE CT3DMusic::GetMPEGHeader( LPBYTE pDATA, LPMPEG_HEADER pHEADER)
{
	pHEADER->m_nVersion = (pDATA[1] & 0x08) >> 3;
	pHEADER->m_nLayer = (pDATA[1] & 0x06) >> 1;
	pHEADER->m_nCRC = pDATA[1] & 0x01;

	pHEADER->m_nBitrateIndex = (pDATA[2] & 0xF0) >> 4;
	pHEADER->m_nFreqIndex = (pDATA[2] & 0x0C) >> 2;
	pHEADER->m_nPadding = (pDATA[2] & 0x02) >> 1;
	pHEADER->m_nExtension = pDATA[2] & 0x01;

	pHEADER->m_nCopyright = (pDATA[3] & 0x08) >> 3;
	pHEADER->m_nOriginal = (pDATA[3] & 0x04) >> 2;
	pHEADER->m_nEmphasis = pDATA[3] & 0x03;

	if( pDATA[0] != 0xFF )
		return FALSE;

	switch(pDATA[1]&0xF0)
	{
	case 0xF0	: pHEADER->m_nVersion = pHEADER->m_nVersion ? 1 : 2; break;
	case 0xE0	: pHEADER->m_nVersion = 3; break;
	default		: return FALSE;
	}

	if( pHEADER->m_nFreqIndex >= 3 ||
		pHEADER->m_nBitrateIndex == 0 ||
		pHEADER->m_nBitrateIndex >= 15 ||
		pHEADER->m_nLayer == 0 ||
		pHEADER->m_nLayer >= 4 )
		return FALSE;

	pHEADER->m_nCRC = pHEADER->m_nCRC ? FALSE : TRUE;
	pHEADER->m_nLayer = 4 - pHEADER->m_nLayer;

	return TRUE;
}

BYTE CT3DMusic::GetMPEGInfo( LPMPEG_DECODE_INFO pINFO,
							 LPBYTE pDATA,
							 int nSIZE)
{
	DWORD dwMinBitRate = 0xFFFFFFFF;
	DWORD dwMaxBitRate = 0;

	BYTE bVBR = TRUE;

	ZeroMemory( pINFO, sizeof(MPEG_DECODE_INFO));
	if( nSIZE < 156 )
		return FALSE;

	if(!GetMPEGHeader( pDATA, &pINFO->m_vHeader))
		return FALSE;

	pDATA += 4;
	if(pINFO->m_vHeader.m_nCRC)
		pDATA += 2;

	if( pINFO->m_vHeader.m_nLayer == 3 )
		if( pINFO->m_vHeader.m_nVersion == 1 )
			if( pINFO->m_vHeader.m_nMode != 3 )
				pDATA += 32;
			else
				pDATA += 17;
		else if( pINFO->m_vHeader.m_nMode != 3 )
			pDATA += 17;
		else
			pDATA += 9;

	pINFO->m_nBitRate = GetBITRATE(&pINFO->m_vHeader);
	int nFREQ = GetFREQ(&pINFO->m_vHeader);

	if(!memcmp( pDATA, "Xing", 4))
	{
		DWORD dwFlags;
		pDATA += 4;

		memcpy( &dwFlags, pDATA, sizeof(DWORD));
		pDATA += 4;

		if(!(dwFlags & (VBR_FRAMES_FLAG | VBR_BYTES_FLAG)))
			return FALSE;

		memcpy( &pINFO->m_nFrames, pDATA, sizeof(int));
		pDATA += 4;

		memcpy( &pINFO->m_nDataSize, pDATA, sizeof(int));
		pDATA += 4;

		if(dwFlags & VBR_TOC_FLAG)
			pDATA += 100;

		if(dwFlags & VBR_SCALE_FLAG)
			pDATA += 4;

		for( int i=1; i<15; i++)
		{
			DWORD dwBitRate = m_vBITRATE[pINFO->m_vHeader.m_nVersion - 1][pINFO->m_vHeader.m_nLayer - 1][i] * 1000;

			if( dwBitRate < dwMinBitRate )
				dwMinBitRate = dwBitRate;

			if( dwBitRate > dwMaxBitRate )
				dwMaxBitRate = dwBitRate;
		}
	}
	else if(!memcmp( pDATA, "VBRI", 4))
	{
		pDATA += 10;

		memcpy( &pINFO->m_nDataSize, pDATA, sizeof(int));
		pDATA += 4;

		memcpy( &pINFO->m_nFrames, pDATA, sizeof(int));
		pDATA += 4;

		for( int i=1; i<15; i++)
		{
			DWORD dwBitRate = m_vBITRATE[pINFO->m_vHeader.m_nVersion - 1][pINFO->m_vHeader.m_nLayer - 1][i] * 1000;

			if( dwBitRate < dwMinBitRate )
				dwMinBitRate = dwBitRate;

			if( dwBitRate > dwMaxBitRate )
				dwMaxBitRate = dwBitRate;
		}
	}
	else
	{
		pINFO->m_nDataSize = 0;
		pINFO->m_nFrames = 0;
		bVBR = FALSE;
	}

	switch(pINFO->m_vHeader.m_nLayer)
	{
	case 1	:
		{
			pINFO->m_nOutputSize = 384 >> m_vDecodeOption.m_nReduction;

			if(bVBR)
			{
				pINFO->m_nSkipSize = (12 * pINFO->m_nBitRate / nFREQ + pINFO->m_vHeader.m_nPadding) * 4;
				pINFO->m_nMinInputSize = (12 * dwMinBitRate / nFREQ) * 4;
				pINFO->m_nMaxInputSize = (12 * dwMaxBitRate / nFREQ) * 4 + 4;
			}
			else
			{
				pINFO->m_nMinInputSize = (12 * pINFO->m_nBitRate / nFREQ) * 4;
				pINFO->m_nMaxInputSize = pINFO->m_nMinInputSize + 4;
				pINFO->m_nSkipSize = 0;
			}
		}

		break;

	case 2	:
		{
			pINFO->m_nOutputSize = 1152 >> m_vDecodeOption.m_nReduction;

			if(bVBR)
			{
				pINFO->m_nSkipSize = 144 * pINFO->m_nBitRate / nFREQ + pINFO->m_vHeader.m_nPadding;
				pINFO->m_nMinInputSize = 144 * dwMinBitRate / nFREQ;
				pINFO->m_nMaxInputSize = 144 * dwMaxBitRate / nFREQ + 1;
			}
			else
			{
				pINFO->m_nMinInputSize = 144 * pINFO->m_nBitRate / nFREQ;
				pINFO->m_nMaxInputSize = pINFO->m_nMinInputSize + 1;
				pINFO->m_nSkipSize = 0;
			}
		}

		break;

	case 3	:
		{
			int nSCALE = pINFO->m_vHeader.m_nVersion == 1 ? 1 : 2;

			pINFO->m_nOutputSize = (1152 >> m_vDecodeOption.m_nReduction) / nSCALE;
			if(bVBR)
			{
				pINFO->m_nSkipSize = 144 * pINFO->m_nBitRate / nFREQ / nSCALE + pINFO->m_vHeader.m_nPadding;
				pINFO->m_nMinInputSize = 144 * dwMinBitRate / nFREQ / nSCALE;
				pINFO->m_nMaxInputSize = 144 * dwMaxBitRate / nFREQ / nSCALE + 1;
			}
			else
			{
				pINFO->m_nMinInputSize = 144 * pINFO->m_nBitRate / nFREQ / nSCALE;
				pINFO->m_nMaxInputSize = pINFO->m_nMinInputSize + 1;
				pINFO->m_nSkipSize = 0;
			}
		}

		break;
	}

	if( pINFO->m_vHeader.m_nMode == 3 || (m_vDecodeOption.m_nConvert & 3) )
		pINFO->m_nChannels = 1;
	else
		pINFO->m_nChannels = 2;

	if(m_vDecodeOption.m_nConvert & 8)
	{
		pINFO->m_nOutputSize *= pINFO->m_nChannels;
		pINFO->m_nBitsPerSample = 8;
	}
	else
	{
		pINFO->m_nOutputSize *= pINFO->m_nChannels * 2;
		pINFO->m_nBitsPerSample = 16;
	}

	if(bVBR)
		pINFO->m_nBitRate = 0;

	return TRUE;
}

DWORD WINAPI CT3DMusic::DecodeMPEG( LPVOID pParam)
{
	CT3DMusic *pMUSIC = (CT3DMusic *) pParam;

	while(TRUE)
	{
		if(pMUSIC->m_dwPlayTick)
		{
			DWORD dwTickCount = GetTickCount() - pMUSIC->m_dwStartTick;

			if( pMUSIC->m_dwPlayTick > dwTickCount )
				WaitForSingleObject( pMUSIC->m_hEVENT, pMUSIC->m_dwPlayTick - dwTickCount);
		}

		switch(pMUSIC->m_bSTATE)
		{
		case MMS_PLAY	:
			{
				if( !pMUSIC->m_dwPOS && !pMUSIC->DecodeSTART() )
				{
					pMUSIC->m_bSTATE = MMS_OPEN;
					pMUSIC->m_dwSEEK = 0;

					if(pMUSIC->m_hCOMP)
						SetEvent(pMUSIC->m_hCOMP);

					return 0;
				}
				pMUSIC->m_dwStartTick = GetTickCount();

				if( !pMUSIC->m_nTimeDivider || !pMUSIC->PlayBUF(pMUSIC->m_bINDEX) )
				{
					pMUSIC->m_bSTATE = MMS_OPEN;
					pMUSIC->m_dwSEEK = 0;

					if(pMUSIC->m_hCOMP)
						SetEvent(pMUSIC->m_hCOMP);

					return 0;
				}
				pMUSIC->m_dwPlayTick = DWORD(__int64(pMUSIC->m_vHEADER[pMUSIC->m_bINDEX].dwBufferLength) * __int64(1000) / __int64(pMUSIC->m_nTimeDivider));

				if( pMUSIC->m_dwPart < pMUSIC->m_dwPartCount )
				{
					WORD wBuffering = pMUSIC->GetBufferingCount(
						pMUSIC->m_dwSTART,
						pMUSIC->m_dwEND);

					pMUSIC->m_bINDEX ^= 1;
					pMUSIC->m_dwPart++;

					if( pMUSIC->m_dwPart == pMUSIC->m_dwPartCount )
						wBuffering = WORD(pMUSIC->Tick2Frame(pMUSIC->m_dwEND) - pMUSIC->Tick2Frame(pMUSIC->m_dwSEEK) + 1) % wBuffering;

					if(!pMUSIC->DecodeFRAME(wBuffering))
					{
						pMUSIC->m_bSTATE = MMS_OPEN;
						pMUSIC->m_dwSEEK = 0;

						if(pMUSIC->m_hCOMP)
							SetEvent(pMUSIC->m_hCOMP);

						return 0;
					}
				}
				else if(pMUSIC->m_bLOOP)
				{
					pMUSIC->m_bINDEX ^= 1;

					pMUSIC->m_dwPart = 0;
					pMUSIC->m_dwPOS = 0;
					pMUSIC->m_dwSEEK = 0;
				}
				else
				{
					SMART_LOCKCS(&pMUSIC->m_cs);

					if( pMUSIC->m_bSTATE != MMS_PAUSE )
					{
						pMUSIC->m_bSTATE = MMS_OPEN;
						pMUSIC->m_dwSEEK = 0;
					}
				}
			}

			break;

		case MMS_PAUSE	:
			{
				BYTE bINDEX = pMUSIC->m_bINDEX ^ 1;
				DWORD dwCurPOS = 0;

				if(pMUSIC->m_hCOMP)
					SetEvent(pMUSIC->m_hCOMP);

				if( !pMUSIC->m_vOUTPUT[bINDEX] || FAILED(pMUSIC->m_vOUTPUT[bINDEX]->Stop()) ||
					FAILED(pMUSIC->m_vOUTPUT[bINDEX]->GetCurrentPosition( NULL, &dwCurPOS)) )
				{
					pMUSIC->m_bSTATE = MMS_OPEN;
					pMUSIC->m_dwSEEK = 0;

					if(pMUSIC->m_hCOMP)
						SetEvent(pMUSIC->m_hCOMP);

					return 0;
				}

				WaitForSingleObject(
					pMUSIC->m_hPAUSE,
					INFINITE);
				pMUSIC->m_dwStartTick = GetTickCount();

				if( !pMUSIC->m_nTimeDivider || !pMUSIC->PlayBUF(bINDEX) )
				{
					pMUSIC->m_bSTATE = MMS_OPEN;
					pMUSIC->m_dwSEEK = 0;

					if(pMUSIC->m_hCOMP)
						SetEvent(pMUSIC->m_hCOMP);

					return 0;
				}
				pMUSIC->m_dwPlayTick = DWORD((__int64(pMUSIC->m_vHEADER[bINDEX].dwBufferLength) - __int64(dwCurPOS)) * __int64(1000) / __int64(pMUSIC->m_nTimeDivider));
			}

			break;

		default			:
			{
				pMUSIC->m_bSTATE = MMS_OPEN;
				pMUSIC->m_dwSEEK = 0;

				if(pMUSIC->m_hCOMP)
					SetEvent(pMUSIC->m_hCOMP);
			}

			return 0;
		}
	}

	if(pMUSIC->m_hCOMP)
		SetEvent(pMUSIC->m_hCOMP);

	return 0;
}

void CT3DMusic::BufferingFRAME( WORD wBuffering)
{
	for( WORD i=0; i<wBuffering; i++)
	{
		MPEG_DECODE_INFO vINFO;

		GetMPEGInfo(
			&vINFO,
			m_pFRAME + m_dwPOS,
			m_wFRAME);

		memcpy( &m_vPARAM[m_bINDEX].m_vHeader, &vINFO.m_vHeader, sizeof(MPEG_HEADER));
		m_vPARAM[m_bINDEX].m_nBitRate = vINFO.m_nBitRate;
		m_vPARAM[m_bINDEX].m_pInput = m_pFRAME + m_dwPOS;
		m_vPARAM[m_bINDEX].m_nInput = vINFO.m_vHeader.m_nPadding ? vINFO.m_nMaxInputSize : vINFO.m_nMinInputSize;
		m_vPARAM[m_bINDEX].m_pOutput = ((LPBYTE) m_pBUF[m_bINDEX]) + m_vBUFSIZE[m_bINDEX];
		m_vPARAM[m_bINDEX].m_nOutput = vINFO.m_nOutputSize;

		m_dwPOS += m_vPARAM[m_bINDEX].m_nInput;
		m_vBUFSIZE[m_bINDEX] += vINFO.m_nOutputSize;

		DecodeFRAME();
	}
}

BYTE CT3DMusic::DecodeFRAME( WORD wBuffering)
{
	m_vBUFSIZE[m_bINDEX] = 0;
	BufferingFRAME(wBuffering);

	ZeroMemory( &m_vHEADER[m_bINDEX], sizeof(WAVEHDR));
	m_vHEADER[m_bINDEX].dwBufferLength = m_vBUFSIZE[m_bINDEX];
	m_vHEADER[m_bINDEX].lpData = (LPSTR) m_pBUF[m_bINDEX];

	return ResetDSBUF(m_bINDEX);
}

BYTE CT3DMusic::DecodeSTART()
{
	WORD wBuffering = GetBufferingCount(
		m_dwSTART,
		m_dwEND);
	DWORD dwSTART = Tick2Frame(m_dwSTART);
	DWORD dwSEEK = Tick2Frame(m_dwSEEK);

	m_dwPart = 0;
	m_dwPOS = 0;

	if( dwSEEK < dwSTART )
		dwSEEK = dwSTART;

	if(dwSEEK)
	{
		for( DWORD i=0; i<dwSEEK; i++)
		{
			MPEG_DECODE_INFO vINFO;

			GetMPEGInfo(
				&vINFO,
				m_pFRAME + m_dwPOS,
				m_wFRAME);

			m_dwPOS += vINFO.m_vHeader.m_nPadding ? vINFO.m_nMaxInputSize : vINFO.m_nMinInputSize;
		}

		m_dwPart = dwSEEK / wBuffering + (dwSEEK % wBuffering ? 0 : 1);
	}

	return DecodeFRAME(wBuffering);
}

void CT3DMusic::DecodeFRAME()
{
	if( m_vPARAM[m_bINDEX].m_nInput <= 4 )
		return;
	int nFRAME = GetFrameSize(&m_vPARAM[m_bINDEX].m_vHeader);

	if( !nFRAME || m_vPARAM[m_bINDEX].m_nInput < nFRAME )
		return;
	m_pFrameProc( this, nFRAME);
}

void CT3DMusic::DecodeL1( CT3DMusic *pMUSIC,
						  int nFRAME)
{
	static const int vBitMASTER[16] = { 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

	LPBYTE pMPEG = (LPBYTE) pMUSIC->m_vPARAM[pMUSIC->m_bINDEX].m_pInput;
	CTBitStream vSTREAM( pMPEG + 4 + pMUSIC->GetCRCSize(), pMUSIC->m_vPARAM[pMUSIC->m_bINDEX].m_nInput);

	int nSTEREO = pMUSIC->GetStereoBIT();
	int nSKIP = 0;

	DWORD vDISPATCH[66];
	DWORD vDATA[64];

	FLOAT vCODE[64];
	FLOAT vSF[64];

	FLOAT *pSAMPLE = (FLOAT *) pMUSIC->m_pSAMPLE;

	for( int i=0; i<pMUSIC->m_nBATCount; i++)
	{
		vDISPATCH[i] = vSTREAM.PopBITs(4);
		vDATA[i] = vDISPATCH[i];

		if( i >= pMUSIC->m_nLimit )
			nSKIP += vBitMASTER[vDISPATCH[i]];

		vCODE[i] = m_vValueL1[vDISPATCH[i]];
		nSTEREO--;

		if( nSTEREO < 0 )
		{
			vDATA[i + 1] = vDATA[i];
			vDISPATCH[i] += 15;

			vDISPATCH[i + 1] = vDISPATCH[i];
			vCODE[i + 1] = vCODE[i];
			i++;
		}
	}

	vDISPATCH[pMUSIC->m_nLimit] = 31;
	vDISPATCH[i] = 30;

	for( i=0; i<pMUSIC->m_nBATCount; i++)
		if(vDATA[i])
			vSF[i] = vCODE[i] * m_vTableSF[vSTREAM.PopBITs(6)];

	for( i=0; i<12; i++)
	{
		BYTE bDISPATCH = TRUE;
		int nINDEX = -1;

		while(bDISPATCH)
		{
			nINDEX++;

			switch(vDISPATCH[nINDEX])
			{
			case 0	: pSAMPLE[nINDEX] = 0.0f; break;
			case 1	:
			case 2	:
			case 3	:
			case 4	:
			case 5	:
			case 6	:
			case 7	:
			case 8	:
			case 9	:
			case 10	:
			case 11	:
			case 12	:
			case 13	:
			case 14	: pSAMPLE[nINDEX] = vSF[nINDEX] * (vSTREAM.PopBITs(BYTE(vDISPATCH[nINDEX] + 1)) - ((1 << vDISPATCH[nINDEX]) - 1)); break;
			case 15	:
				{
					pSAMPLE[nINDEX] = 0.0f;
					nINDEX++;
					pSAMPLE[nINDEX] = 0.0f;
				}

				break;

			case 16	:
			case 17	:
			case 18	:
			case 19	:
			case 20	:
			case 21	:
			case 22	:
			case 23	:
			case 24	:
			case 25	:
			case 26	:
			case 27	:
			case 28	:
			case 29	:
				{
					DWORD dwDATA = vDISPATCH[nINDEX] - 14;

					dwDATA = vSTREAM.PopBITs(BYTE(dwDATA)) - ((1 << (dwDATA - 1)) - 1);
					pSAMPLE[nINDEX] = vSF[nINDEX] * dwDATA;
					nINDEX++;
					pSAMPLE[nINDEX] = vSF[nINDEX] * dwDATA;
				}

				break;

			case 31	: vSTREAM.SeekBITs(nSKIP);
			case 30	:
				{
					bDISPATCH = FALSE;
					pSAMPLE += 64;
				}

				break;
			}
		}
	}

	pMUSIC->m_pSBTProc( pMUSIC, (FLOAT *) pMUSIC->m_pSAMPLE->m_vB, pMUSIC->m_vPARAM[pMUSIC->m_bINDEX].m_pOutput, 12);
}

void CT3DMusic::DecodeL2( CT3DMusic *pMUSIC,
						  int nFRAME)
{
	static const int vBitMASTER[18] = { 0, 5, 7, 9, 10, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48};
	static const BYTE vBitCount[4] = { 4, 4, 3, 2};

	LPBYTE pMPEG = (LPBYTE) pMUSIC->m_vPARAM[pMUSIC->m_bINDEX].m_pInput;
	CTBitStream vSTREAM( pMPEG + 4 + pMUSIC->GetCRCSize(), pMUSIC->m_vPARAM[pMUSIC->m_bINDEX].m_nInput);

	int nSTEREO = pMUSIC->GetStereoBIT();
	int nINDEX = 0;
	int nSKIP = 0;

	DWORD vDISPATCHSF[66];
	DWORD vDISPATCH[66];
	DWORD vDATA[64];

	FLOAT vSF[3][64];
	FLOAT vCODE[64];

	FLOAT *pSAMPLE = (FLOAT *) pMUSIC->m_pSAMPLE;

	for( int i=0; i<4; i++)
		for( int j=0; j<pMUSIC->GetBATCountL2(i); j++, nINDEX++)
		{
			vDISPATCH[nINDEX] = m_vBAT[pMUSIC->m_nBATCount][i][vSTREAM.PopBITs(vBitCount[i])];
			vDATA[nINDEX] = vDISPATCH[nINDEX];

			if( nINDEX >= pMUSIC->m_nLimit )
				nSKIP += vBitMASTER[vDISPATCH[nINDEX]];

			vCODE[nINDEX] = m_vValueL2[vDISPATCH[nINDEX]];
			nSTEREO--;

			if( nSTEREO < 0 )
			{
				vDATA[nINDEX + 1] = vDATA[nINDEX];
				vDISPATCH[nINDEX] += 18;

				vDISPATCH[nINDEX + 1] = vDISPATCH[nINDEX];
				vCODE[nINDEX + 1] = vCODE[nINDEX];

				nINDEX++;
				j++;
			}
		}

	vDISPATCH[pMUSIC->m_nLimit] = 37;
	vDISPATCH[nINDEX] = 36;

	for( i=0; i<pMUSIC->m_nMaxSB; i++)
		vDISPATCHSF[i] = vDATA[i] ? vSTREAM.PopBITs(2) : 4;
	vDISPATCHSF[i] = 5;

	BYTE bDISPATCH = TRUE;
	nINDEX = -1;

	while(bDISPATCH)
	{
		nINDEX++;

		switch(vDISPATCHSF[nINDEX])
		{
		case 0	:
			for( i=0; i<3; i++)
				vSF[i][nINDEX] = vCODE[nINDEX] * m_vTableSF[vSTREAM.PopBITs(6)];

			break;

		case 1	:
			{
				vSF[0][nINDEX] = vCODE[nINDEX] * m_vTableSF[vSTREAM.PopBITs(6)];
				vSF[1][nINDEX] = vSF[0][nINDEX];
				vSF[2][nINDEX] = vCODE[nINDEX] * m_vTableSF[vSTREAM.PopBITs(6)];
			}

			break;

		case 2	:
			{
				vSF[0][nINDEX] = vCODE[nINDEX] * m_vTableSF[vSTREAM.PopBITs(6)];
				vSF[1][nINDEX] = vSF[0][nINDEX];
				vSF[2][nINDEX] = vSF[1][nINDEX];
			}

			break;

		case 3	:
			{
				vSF[0][nINDEX] = vCODE[nINDEX] * m_vTableSF[vSTREAM.PopBITs(6)];
				vSF[1][nINDEX] = vCODE[nINDEX] * m_vTableSF[vSTREAM.PopBITs(6)];
				vSF[2][nINDEX] = vSF[1][nINDEX];
			}

			break;

		case 4	:
			for( i=0; i<3; i++)
				vSF[i][nINDEX] = 0.0f;

			break;

		case 5	: bDISPATCH = FALSE; break;
		}
	}

	for( i=0; i<3; i++)
		for( int j=0; j<4; j++)
		{
			bDISPATCH = TRUE;
			nINDEX = -1;

			while(bDISPATCH)
			{
				nINDEX++;

				switch(vDISPATCH[nINDEX])
				{
				case 0	:
					{
						pSAMPLE[nINDEX + 128] = 0.0f;
						pSAMPLE[nINDEX + 64] = 0.0f;
						pSAMPLE[nINDEX] = 0.0f;
					}

					break;

				case 1	:
					{
						DWORD dwDATA = vSTREAM.PopBITs(5);

						pSAMPLE[nINDEX + 128] = vSF[i][nINDEX] * m_vTableGRP3[dwDATA][2];
						pSAMPLE[nINDEX + 64] = vSF[i][nINDEX] * m_vTableGRP3[dwDATA][1];
						pSAMPLE[nINDEX] = vSF[i][nINDEX] * m_vTableGRP3[dwDATA][0];
					}

					break;

				case 2	:
					{
						DWORD dwDATA = vSTREAM.PopBITs(7);

						pSAMPLE[nINDEX + 128] = vSF[i][nINDEX] * m_vTableGRP5[dwDATA][2];
						pSAMPLE[nINDEX + 64] = vSF[i][nINDEX] * m_vTableGRP5[dwDATA][1];
						pSAMPLE[nINDEX] = vSF[i][nINDEX] * m_vTableGRP5[dwDATA][0];
					}

					break;

				case 3	:
					{
						pSAMPLE[nINDEX] = vSF[i][nINDEX] * (vSTREAM.PopBITs(BYTE(vDISPATCH[nINDEX])) - ((1 << (vDISPATCH[nINDEX] - 1)) - 1));
						pSAMPLE[nINDEX + 64] = vSF[i][nINDEX] * (vSTREAM.PopBITs(BYTE(vDISPATCH[nINDEX])) - ((1 << (vDISPATCH[nINDEX] - 1)) - 1));
						pSAMPLE[nINDEX + 128] = vSF[i][nINDEX] * (vSTREAM.PopBITs(BYTE(vDISPATCH[nINDEX])) - ((1 << (vDISPATCH[nINDEX] - 1)) - 1));
					}

					break;

				case 4	:
					{
						DWORD dwDATA = vSTREAM.PopBITs(10);

						pSAMPLE[nINDEX + 128] = vSF[i][nINDEX] * m_vTableGRP9[dwDATA][2];
						pSAMPLE[nINDEX + 64] = vSF[i][nINDEX] * m_vTableGRP9[dwDATA][1];
						pSAMPLE[nINDEX] = vSF[i][nINDEX] * m_vTableGRP9[dwDATA][0];
					}

					break;

				case 5	:
				case 6	:
				case 7	:
				case 8	:
				case 9	:
				case 10	:
				case 11	:
				case 12	:
				case 13	:
				case 14	:
				case 15	:
				case 16	:
				case 17	:
					{
						DWORD dwDATA = vDISPATCH[nINDEX] - 1;

						pSAMPLE[nINDEX] = vSF[i][nINDEX] * (vSTREAM.PopBITs(BYTE(dwDATA)) - ((1 << (dwDATA - 1)) - 1));
						pSAMPLE[nINDEX + 64] = vSF[i][nINDEX] * (vSTREAM.PopBITs(BYTE(dwDATA)) - ((1 << (dwDATA - 1)) - 1));
						pSAMPLE[nINDEX + 128] = vSF[i][nINDEX] * (vSTREAM.PopBITs(BYTE(dwDATA)) - ((1 << (dwDATA - 1)) - 1));
					}

					break;

				case 18	:
					{
						pSAMPLE[nINDEX + 128] = 0.0f;
						pSAMPLE[nINDEX + 64] = 0.0f;
						pSAMPLE[nINDEX] = 0.0f;
						nINDEX++;

						pSAMPLE[nINDEX + 128] = 0.0f;
						pSAMPLE[nINDEX + 64] = 0.0f;
						pSAMPLE[nINDEX] = 0.0f;
					}

					break;

				case 19	:
					{
						DWORD dwDATA = vSTREAM.PopBITs(5);

						pSAMPLE[nINDEX + 128] = vSF[i][nINDEX] * m_vTableGRP3[dwDATA][2];
						pSAMPLE[nINDEX + 64] = vSF[i][nINDEX] * m_vTableGRP3[dwDATA][1];
						pSAMPLE[nINDEX] = vSF[i][nINDEX] * m_vTableGRP3[dwDATA][0];
						nINDEX++;

						pSAMPLE[nINDEX + 128] = vSF[i][nINDEX] * m_vTableGRP3[dwDATA][2];
						pSAMPLE[nINDEX + 64] = vSF[i][nINDEX] * m_vTableGRP3[dwDATA][1];
						pSAMPLE[nINDEX] = vSF[i][nINDEX] * m_vTableGRP3[dwDATA][0];
					}

					break;

				case 20	:
					{
						DWORD dwDATA = vSTREAM.PopBITs(7);

						pSAMPLE[nINDEX + 128] = vSF[i][nINDEX] * m_vTableGRP5[dwDATA][2];
						pSAMPLE[nINDEX + 64] = vSF[i][nINDEX] * m_vTableGRP5[dwDATA][1];
						pSAMPLE[nINDEX] = vSF[i][nINDEX] * m_vTableGRP5[dwDATA][0];
						nINDEX++;

						pSAMPLE[nINDEX + 128] = vSF[i][nINDEX] * m_vTableGRP5[dwDATA][2];
						pSAMPLE[nINDEX + 64] = vSF[i][nINDEX] * m_vTableGRP5[dwDATA][1];
						pSAMPLE[nINDEX] = vSF[i][nINDEX] * m_vTableGRP5[dwDATA][0];
					}

					break;

				case 21	:
					{
						BYTE bBIT = BYTE(vDISPATCH[nINDEX] - 18);
						DWORD dwDATA = vSTREAM.PopBITs(bBIT) - ((1 << (bBIT - 1)) - 1);

						pSAMPLE[nINDEX + 1] = vSF[i][nINDEX + 1] * dwDATA;
						pSAMPLE[nINDEX] = vSF[i][nINDEX] * dwDATA;

						dwDATA = vSTREAM.PopBITs(bBIT) - ((1 << (bBIT - 1)) - 1);
						pSAMPLE[nINDEX + 64 + 1] = vSF[i][nINDEX + 1] * dwDATA;
						pSAMPLE[nINDEX + 64] = vSF[i][nINDEX] * dwDATA;

						dwDATA = vSTREAM.PopBITs(bBIT) - ((1 << (bBIT - 1)) - 1);
						pSAMPLE[nINDEX + 128 + 1] = vSF[i][nINDEX + 1] * dwDATA;
						pSAMPLE[nINDEX + 128] = vSF[i][nINDEX] * dwDATA;
						nINDEX++;
					}

					break;

				case 22	:
					{
						DWORD dwDATA = vSTREAM.PopBITs(10);

						pSAMPLE[nINDEX + 128] = vSF[i][nINDEX] * m_vTableGRP9[dwDATA][2];
						pSAMPLE[nINDEX + 64] = vSF[i][nINDEX] * m_vTableGRP9[dwDATA][1];
						pSAMPLE[nINDEX] = vSF[i][nINDEX] * m_vTableGRP9[dwDATA][0];
						nINDEX++;

						pSAMPLE[nINDEX + 128] = vSF[i][nINDEX] * m_vTableGRP9[dwDATA][2];
						pSAMPLE[nINDEX + 64] = vSF[i][nINDEX] * m_vTableGRP9[dwDATA][1];
						pSAMPLE[nINDEX] = vSF[i][nINDEX] * m_vTableGRP9[dwDATA][0];
					}

					break;

				case 23	:
				case 24	:
				case 25	:
				case 26	:
				case 27	:
				case 28	:
				case 29	:
				case 30	:
				case 31	:
				case 32	:
				case 33	:
				case 34	:
				case 35	:
					{
						BYTE bBIT = BYTE(vDISPATCH[nINDEX] - 19);
						DWORD dwDATA = vSTREAM.PopBITs(bBIT) - ((1 << (bBIT - 1)) - 1);

						pSAMPLE[nINDEX + 1] = vSF[i][nINDEX + 1] * dwDATA;
						pSAMPLE[nINDEX] = vSF[i][nINDEX] * dwDATA;

						dwDATA = vSTREAM.PopBITs(bBIT) - ((1 << (bBIT - 1)) - 1);
						pSAMPLE[nINDEX + 64 + 1] = vSF[i][nINDEX + 1] * dwDATA;
						pSAMPLE[nINDEX + 64] = vSF[i][nINDEX] * dwDATA;

						dwDATA = vSTREAM.PopBITs(bBIT) - ((1 << (bBIT - 1)) - 1);
						pSAMPLE[nINDEX + 128 + 1] = vSF[i][nINDEX + 1] * dwDATA;
						pSAMPLE[nINDEX + 128] = vSF[i][nINDEX] * dwDATA;
						nINDEX++;
					}

					break;

				case 37	: vSTREAM.SeekBITs(nSKIP);
				case 36	:
					{
						bDISPATCH = FALSE;
						pSAMPLE += 3 * 64;
					}

					break;
				}
			}
		}

	pMUSIC->m_pSBTProc( pMUSIC, (FLOAT *) pMUSIC->m_pSAMPLE->m_vB, pMUSIC->m_vPARAM[pMUSIC->m_bINDEX].m_pOutput, 36);
}

void CT3DMusic::DecodeL3( CT3DMusic *pMUSIC,
						  int nFRAME)
{
	LPBYTE pMPEG = (LPBYTE) pMUSIC->m_vPARAM[pMUSIC->m_bINDEX].m_pInput;

	int nChannels = pMUSIC->GetChannels();
	int nMSMode = 0;
	int nISMode = 0;
	int nSIDE = 0;
	int nCOPY = 0;
	int nCRC = pMUSIC->GetCRCSize();

	CTBitStream vSTREAM( pMPEG + 4 + nCRC, pMUSIC->m_vPARAM[pMUSIC->m_bINDEX].m_nInput);
	SIDE_INFO vSIDE;

	if( pMUSIC->m_vPARAM[pMUSIC->m_bINDEX].m_vHeader.m_nMode == 1 )
	{
		nMSMode = pMUSIC->m_vPARAM[pMUSIC->m_bINDEX].m_vHeader.m_nModeEx >> 1;
		nISMode = pMUSIC->m_vPARAM[pMUSIC->m_bINDEX].m_vHeader.m_nModeEx & 1;
	}

	if( pMUSIC->m_vPARAM[pMUSIC->m_bINDEX].m_vHeader.m_nVersion == 1 )
	{
		vSIDE.m_dwSTART = vSTREAM.PopBITs(9);

		if( nChannels == 1 )
		{
			vSIDE.m_dwPRIVATE = vSTREAM.PopBITs(5);
			nSIDE = 17;
		}
		else
		{
			vSIDE.m_dwPRIVATE = vSTREAM.PopBITs(3);
			nSIDE = 32;
		}

		for( int i=0; i<nChannels; i++)
			vSIDE.m_dwSCFSI[i] = vSTREAM.PopBITs(4);

		for( i=0; i<2; i++)
			for( int j=0; j<nChannels; j++)
			{
				vSIDE.m_vINFO[i][j].m_dwPart2_3_Length = vSTREAM.PopBITs(12);
				vSIDE.m_vINFO[i][j].m_dwBigValues = vSTREAM.PopBITs(9);
				vSIDE.m_vINFO[i][j].m_dwGlobalGain = vSTREAM.PopBITs(8);

				if(nMSMode)
					vSIDE.m_vINFO[i][j].m_dwGlobalGain -= 2;

				vSIDE.m_vINFO[i][j].m_dwSFCompress = vSTREAM.PopBITs(4);
				vSIDE.m_vINFO[i][j].m_dwWindowSwitchingFlag = vSTREAM.PopBITs(1);

				if(vSIDE.m_vINFO[i][j].m_dwWindowSwitchingFlag)
				{
					vSIDE.m_vINFO[i][j].m_dwBlockType = vSTREAM.PopBITs(2);
					vSIDE.m_vINFO[i][j].m_dwMixedBlockFlag = vSTREAM.PopBITs(1);
					vSIDE.m_vINFO[i][j].m_dwTableSelect[0] = vSTREAM.PopBITs(5);
					vSIDE.m_vINFO[i][j].m_dwTableSelect[1] = vSTREAM.PopBITs(5);
					vSIDE.m_vINFO[i][j].m_dwSubblockGain[0] = vSTREAM.PopBITs(3);
					vSIDE.m_vINFO[i][j].m_dwSubblockGain[1] = vSTREAM.PopBITs(3);
					vSIDE.m_vINFO[i][j].m_dwSubblockGain[2] = vSTREAM.PopBITs(3);
					vSIDE.m_vINFO[i][j].m_dwRegion0Count = 7;
					vSIDE.m_vINFO[i][j].m_dwRegion1Count = 13;
				}
				else
				{
					vSIDE.m_vINFO[i][j].m_dwBlockType = 0;
					vSIDE.m_vINFO[i][j].m_dwMixedBlockFlag = 0;
					vSIDE.m_vINFO[i][j].m_dwTableSelect[0] = vSTREAM.PopBITs(5);
					vSIDE.m_vINFO[i][j].m_dwTableSelect[1] = vSTREAM.PopBITs(5);
					vSIDE.m_vINFO[i][j].m_dwTableSelect[2] = vSTREAM.PopBITs(5);
					vSIDE.m_vINFO[i][j].m_dwRegion0Count = vSTREAM.PopBITs(4);
					vSIDE.m_vINFO[i][j].m_dwRegion1Count = vSTREAM.PopBITs(3);
				}

				vSIDE.m_vINFO[i][j].m_dwPreFlag = vSTREAM.PopBITs(1);
				vSIDE.m_vINFO[i][j].m_dwSFScale = vSTREAM.PopBITs(1);
				vSIDE.m_vINFO[i][j].m_dwCount1TableSelect = vSTREAM.PopBITs(1);
			}
	}
	else
	{
		vSIDE.m_dwSTART = vSTREAM.PopBITs(8);

		if( nChannels == 1 )
		{
			vSIDE.m_dwPRIVATE = vSTREAM.PopBITs(1);
			nSIDE = 9;
		}
		else
		{
			vSIDE.m_dwPRIVATE = vSTREAM.PopBITs(2);
			nSIDE = 17;
		}

		vSIDE.m_dwSCFSI[0] = 0;
		vSIDE.m_dwSCFSI[1] = 0;

		for( int i=0; i<nChannels; i++)
		{
			vSIDE.m_vINFO[pMUSIC->m_nGroup][i].m_dwPart2_3_Length = vSTREAM.PopBITs(12);
			vSIDE.m_vINFO[pMUSIC->m_nGroup][i].m_dwBigValues = vSTREAM.PopBITs(9);
			vSIDE.m_vINFO[pMUSIC->m_nGroup][i].m_dwGlobalGain = vSTREAM.PopBITs(8);

			if(nMSMode)
				vSIDE.m_vINFO[pMUSIC->m_nGroup][i].m_dwGlobalGain -= 2;

			vSIDE.m_vINFO[pMUSIC->m_nGroup][i].m_dwSFCompress = vSTREAM.PopBITs(9);
			vSIDE.m_vINFO[pMUSIC->m_nGroup][i].m_dwWindowSwitchingFlag = vSTREAM.PopBITs(1);

			if(vSIDE.m_vINFO[pMUSIC->m_nGroup][i].m_dwWindowSwitchingFlag)
			{
				vSIDE.m_vINFO[pMUSIC->m_nGroup][i].m_dwBlockType = vSTREAM.PopBITs(2);
				vSIDE.m_vINFO[pMUSIC->m_nGroup][i].m_dwMixedBlockFlag = vSTREAM.PopBITs(1);
				vSIDE.m_vINFO[pMUSIC->m_nGroup][i].m_dwTableSelect[0] = vSTREAM.PopBITs(5);
				vSIDE.m_vINFO[pMUSIC->m_nGroup][i].m_dwTableSelect[1] = vSTREAM.PopBITs(5);
				vSIDE.m_vINFO[pMUSIC->m_nGroup][i].m_dwSubblockGain[0] = vSTREAM.PopBITs(3);
				vSIDE.m_vINFO[pMUSIC->m_nGroup][i].m_dwSubblockGain[1] = vSTREAM.PopBITs(3);
				vSIDE.m_vINFO[pMUSIC->m_nGroup][i].m_dwSubblockGain[2] = vSTREAM.PopBITs(3);

				if( vSIDE.m_vINFO[pMUSIC->m_nGroup][i].m_dwBlockType == 2 )
				{
					vSIDE.m_vINFO[pMUSIC->m_nGroup][i].m_dwRegion0Count = 5;
					vSIDE.m_vINFO[pMUSIC->m_nGroup][i].m_dwRegion1Count = 15;
				}
				else
				{
					vSIDE.m_vINFO[pMUSIC->m_nGroup][i].m_dwRegion0Count = 7;
					vSIDE.m_vINFO[pMUSIC->m_nGroup][i].m_dwRegion1Count = 13;
				}
			}
			else
			{
				vSIDE.m_vINFO[pMUSIC->m_nGroup][i].m_dwBlockType = 0;
				vSIDE.m_vINFO[pMUSIC->m_nGroup][i].m_dwMixedBlockFlag = 0;
				vSIDE.m_vINFO[pMUSIC->m_nGroup][i].m_dwTableSelect[0] = vSTREAM.PopBITs(5);
				vSIDE.m_vINFO[pMUSIC->m_nGroup][i].m_dwTableSelect[1] = vSTREAM.PopBITs(5);
				vSIDE.m_vINFO[pMUSIC->m_nGroup][i].m_dwTableSelect[2] = vSTREAM.PopBITs(5);
				vSIDE.m_vINFO[pMUSIC->m_nGroup][i].m_dwRegion0Count = vSTREAM.PopBITs(4);
				vSIDE.m_vINFO[pMUSIC->m_nGroup][i].m_dwRegion1Count = vSTREAM.PopBITs(3);
			}

			vSIDE.m_vINFO[pMUSIC->m_nGroup][i].m_dwPreFlag = 0;
			vSIDE.m_vINFO[pMUSIC->m_nGroup][i].m_dwSFScale = vSTREAM.PopBITs(1);
			vSIDE.m_vINFO[pMUSIC->m_nGroup][i].m_dwCount1TableSelect = vSTREAM.PopBITs(1);
		}
	}

	pMUSIC->m_vL3POS[0] = pMUSIC->m_vL3POS[1] - vSIDE.m_dwSTART;
	if( pMUSIC->m_vL3POS[1] > DEF_BUF_TRIGGER )
	{
		memmove( pMUSIC->m_pL3BUF, pMUSIC->m_pL3BUF + pMUSIC->m_vL3POS[0], vSIDE.m_dwSTART);
		pMUSIC->m_vL3POS[0] = 0;
		pMUSIC->m_vL3POS[1] = vSIDE.m_dwSTART;
	}

	nCOPY = nFRAME - (4 + nCRC + nSIDE);
	memmove( pMUSIC->m_pL3BUF + pMUSIC->m_vL3POS[1], pMPEG + (4 + nCRC + nSIDE), nCOPY);
	pMUSIC->m_vL3POS[1] += nCOPY;

	if( pMUSIC->m_vL3POS[0] >= 0 )
	{
		int nBIT = pMUSIC->m_vL3POS[0] << 3;

		if( pMUSIC->m_vPARAM[pMUSIC->m_bINDEX].m_vHeader.m_nVersion == 1 )
		{
			LPBYTE pOUTPUT = (LPBYTE) pMUSIC->m_vPARAM[pMUSIC->m_bINDEX].m_pOutput;

			DecodeMainL3( pMUSIC, &vSIDE, pOUTPUT, nMSMode, nISMode, nChannels, 0, nBIT);
			pOUTPUT = pOUTPUT + (pMUSIC->m_vPARAM[pMUSIC->m_bINDEX].m_nOutput / 2);
			DecodeMainL3( pMUSIC, &vSIDE, pOUTPUT, nMSMode, nISMode, nChannels, 1, nBIT);
		}
		else
		{
			DecodeMainL3( pMUSIC, &vSIDE, (LPBYTE) pMUSIC->m_vPARAM[pMUSIC->m_bINDEX].m_pOutput, nMSMode, nISMode, nChannels, pMUSIC->m_nGroup, nBIT);
			pMUSIC->m_nGroup = pMUSIC->m_nGroup ^ 1;
		}
	}
}

void CT3DMusic::DecodeMainL3( CT3DMusic *pMUSIC,
							  LPSIDE_INFO pSIDE,
							  LPBYTE pOUTPUT,
							  int nMSMode,
							  int nISMode,
							  int nChannels,
							  int nGROUP,
							  int& nBIT)
{
	static int vTAB[2][22] = {
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 3, 3, 3, 2, 0}};

	static int vRT[6][3][4] = {{
		{  6,  5, 5, 5},
		{  3,  3, 3, 3},
		{  6,  3, 3, 3}}, {
		{  6,  5, 7, 3},
		{  3,  3, 4, 2},
		{  6,  3, 4, 2}}, {
		{ 11, 10, 0, 0},
		{  6,  6, 0, 0},
		{  6,  3, 6, 0}}, {
		{  7,  7, 7, 0},
		{  4,  4, 4, 0},
		{  6,  5, 4, 0}}, {
		{  6,  6, 6, 3},
		{  4,  3, 3, 2},
		{  6,  4, 3, 2}}, {
		{  8,  8, 5, 0},
		{  5,  4, 3, 0},
		{  6,  6, 3, 0}}};

	static int vSLT[16][2] = {
		{ 0, 0},
		{ 0, 1},
		{ 0, 2},
		{ 0, 3},
		{ 3, 0},
		{ 1, 1},
		{ 1, 2},
		{ 1, 3},
		{ 2, 1},
		{ 2, 2},
		{ 2, 3},
		{ 3, 1},
		{ 3, 2},
		{ 3, 3},
		{ 4, 2},
		{ 4, 3}};

	int nVersion = pMUSIC->m_vPARAM[pMUSIC->m_bINDEX].m_vHeader.m_nVersion - 1;
	int nFREQ = pMUSIC->m_vPARAM[pMUSIC->m_bINDEX].m_vHeader.m_nFreqIndex;

	SCALE_FACTOR vSF[2][2];
	CB_INFO vCB[2][2];
	IS_SF_INFO vIS;

	FLOAT vRevBUF[192][3];
	FLOAT vSUB[3];

	ZeroMemory( vSF, 2 * 2 * sizeof(SCALE_FACTOR));
	for( int i=0; i<nChannels; i++)
	{
		CTBitStream vSTREAM( pMUSIC->m_pL3BUF + (nBIT >> 3), ((nBIT + pSIDE->m_vINFO[nGROUP][i].m_dwPart2_3_Length + 39) >> 3) + 1);
		int nSKIP = nBIT & 7;

		if(nSKIP)
			vSTREAM.SeekBITs(nSKIP);

		nBIT += pSIDE->m_vINFO[nGROUP][i].m_dwPart2_3_Length;
		if( pMUSIC->m_vPARAM[pMUSIC->m_bINDEX].m_vHeader.m_nVersion != 1 )
		{
			int nSFCompress = pSIDE->m_vINFO[nGROUP][i].m_dwSFCompress;
			int nISModeCH = nISMode & i;
			int nIndex = 0;
			int nRIndex;

			int vSL[4];
			int vR[4];

			pSIDE->m_vINFO[nGROUP][i].m_dwPreFlag = 0;
			if(nISModeCH)
			{
				vIS.m_nIScale = nSFCompress & 1;
				nSFCompress >>= 1;

				if( nSFCompress < 180 )
				{
					vSL[0] = nSFCompress / 36;
					vSL[1] = nSFCompress % 36;
					vSL[2] = vSL[1] % 6;
					vSL[1] = vSL[1] / 6;
					vSL[3] = 0;

					nRIndex = 3;
				}
				else if( nSFCompress < 244 )
				{
					nSFCompress -= 180;
					vSL[2] = nSFCompress & 3;
					nSFCompress >>= 2;
					vSL[1] = nSFCompress & 3;
					vSL[0] = nSFCompress >> 2;
					vSL[3] = 0;

					nRIndex = 4;
				}
				else
				{
					nSFCompress -= 244;
					vSL[0] = nSFCompress / 3;
					vSL[1] = nSFCompress % 3;
					vSL[2] = 0;
					vSL[3] = 0;

					nRIndex = 5;
				}
			}
			else if( nSFCompress < 400 )
			{
				vSL[1] = nSFCompress >> 4;
				vSL[0] = vSL[1] / 5;
				vSL[1] = vSL[1] % 5;
				vSL[3] = nSFCompress & 15;
				vSL[2] = vSL[3] >> 2;
				vSL[3] = vSL[3] & 3;

				nRIndex = 0;
			}
			else if( nSFCompress < 500 )
			{
				nSFCompress -= 400;
				vSL[1] = nSFCompress >> 2;
				vSL[0] = vSL[1] / 5;
				vSL[1] = vSL[1] % 5;
				vSL[2] = nSFCompress & 3;
				vSL[3] = 0;

				nRIndex = 1;
			}
			else
			{
				nSFCompress -= 500;
				vSL[0] = nSFCompress / 3;
				vSL[1] = nSFCompress % 3;
				vSL[2] = 0;
				vSL[3] = 0;

				if(pSIDE->m_vINFO[nGROUP][i].m_dwMixedBlockFlag)
				{
					vSL[2] = vSL[1];
					vSL[1] = vSL[0];
				}

				pSIDE->m_vINFO[nGROUP][i].m_dwPreFlag = 1;
				nRIndex = 2;
			}

			if( pSIDE->m_vINFO[nGROUP][i].m_dwBlockType == 2 )
				nIndex = (pSIDE->m_vINFO[nGROUP][i].m_dwMixedBlockFlag & 1) + 1;

			for( int j=0; j<4; j++)
				vR[j] = vRT[nRIndex][nIndex][j];

			if(nISModeCH)
				for( j=0; j<3; j++)
				{
					vIS.m_vSL[j] = vSL[j];
					vIS.m_vR[j] = vR[j];
				}

			if( pSIDE->m_vINFO[nGROUP][i].m_dwBlockType == 2 )
			{
				int nBLOCK = 0;

				if(pSIDE->m_vINFO[nGROUP][i].m_dwMixedBlockFlag)
				{
					if(vSL[0])
					{
						for( j=0; j<6; j++)
							vSF[nGROUP][i].m_nL[j] = vSTREAM.PopBITs(vSL[0]);
					}
					else
					{
						for( j=0; j<6; j++)
							vSF[nGROUP][i].m_nL[j] = 0;
					}

					nBLOCK = 3;
				}
				else if(vSL[0])
				{
					for( j=0; j<vR[0]; j++)
					{
						vSF[nGROUP][i].m_nS[0][nBLOCK] = vSTREAM.PopBITs(vSL[0]);
						vSF[nGROUP][i].m_nS[1][nBLOCK] = vSTREAM.PopBITs(vSL[0]);
						vSF[nGROUP][i].m_nS[2][nBLOCK] = vSTREAM.PopBITs(vSL[0]);

						nBLOCK++;
					}
				}
				else
				{
					for( j=0; j<vR[0]; j++)
					{
						vSF[nGROUP][i].m_nS[0][nBLOCK] = 0;
						vSF[nGROUP][i].m_nS[1][nBLOCK] = 0;
						vSF[nGROUP][i].m_nS[2][nBLOCK] = 0;

						nBLOCK++;
					}
				}

				for( j=1; j<4; j++)
					if(vSL[j])
					{
						for( int k=0; k<vR[j]; k++)
						{
							vSF[nGROUP][i].m_nS[0][nBLOCK] = vSTREAM.PopBITs(vSL[j]);
							vSF[nGROUP][i].m_nS[1][nBLOCK] = vSTREAM.PopBITs(vSL[j]);
							vSF[nGROUP][i].m_nS[2][nBLOCK] = vSTREAM.PopBITs(vSL[j]);

							nBLOCK++;
						}
					}
					else
					{
						for( int k=0; k<vR[j]; k++)
						{
							vSF[nGROUP][i].m_nS[0][nBLOCK] = 0;
							vSF[nGROUP][i].m_nS[1][nBLOCK] = 0;
							vSF[nGROUP][i].m_nS[2][nBLOCK] = 0;

							nBLOCK++;
						}
					}
			}
			else
			{
				int nBLOCK = 0;

				for( j=0; j<4; j++)
					if(vSL[j])
					{
						for( int k=0; k<vR[j]; k++)
						{
							vSF[nGROUP][i].m_nL[nBLOCK] = vSTREAM.PopBITs(vSL[j]);
							nBLOCK++;
						}
					}
					else
					{
						for( int k=0; k<vR[j]; k++)
						{
							vSF[nGROUP][i].m_nL[nBLOCK] = 0;
							nBLOCK++;
						}
					}
			}
		}
		else if( pSIDE->m_vINFO[nGROUP][i].m_dwBlockType == 2 )
			if(pSIDE->m_vINFO[nGROUP][i].m_dwMixedBlockFlag)
			{
				for( int j=0; j<8; j++)
					vSF[nGROUP][i].m_nL[j] = vSTREAM.PopBITs(vSLT[pSIDE->m_vINFO[nGROUP][i].m_dwSFCompress][0]);

				for( j=3; j<6; j++)
				{
					vSF[nGROUP][i].m_nS[0][j] = vSTREAM.PopBITs(vSLT[pSIDE->m_vINFO[nGROUP][i].m_dwSFCompress][0]);
					vSF[nGROUP][i].m_nS[1][j] = vSTREAM.PopBITs(vSLT[pSIDE->m_vINFO[nGROUP][i].m_dwSFCompress][0]);
					vSF[nGROUP][i].m_nS[2][j] = vSTREAM.PopBITs(vSLT[pSIDE->m_vINFO[nGROUP][i].m_dwSFCompress][0]);
				}

				for( j=6; j<12; j++)
				{
					vSF[nGROUP][i].m_nS[0][j] = vSTREAM.PopBITs(vSLT[pSIDE->m_vINFO[nGROUP][i].m_dwSFCompress][1]);
					vSF[nGROUP][i].m_nS[1][j] = vSTREAM.PopBITs(vSLT[pSIDE->m_vINFO[nGROUP][i].m_dwSFCompress][1]);
					vSF[nGROUP][i].m_nS[2][j] = vSTREAM.PopBITs(vSLT[pSIDE->m_vINFO[nGROUP][i].m_dwSFCompress][1]);
				}
			}
			else
			{
				for( int j=0; j<6; j++)
				{
					vSF[nGROUP][i].m_nS[0][j] = vSTREAM.PopBITs(vSLT[pSIDE->m_vINFO[nGROUP][i].m_dwSFCompress][0]);
					vSF[nGROUP][i].m_nS[1][j] = vSTREAM.PopBITs(vSLT[pSIDE->m_vINFO[nGROUP][i].m_dwSFCompress][0]);
					vSF[nGROUP][i].m_nS[2][j] = vSTREAM.PopBITs(vSLT[pSIDE->m_vINFO[nGROUP][i].m_dwSFCompress][0]);
				}

				for( j=6; j<12; j++)
				{
					vSF[nGROUP][i].m_nS[0][j] = vSTREAM.PopBITs(vSLT[pSIDE->m_vINFO[nGROUP][i].m_dwSFCompress][1]);
					vSF[nGROUP][i].m_nS[1][j] = vSTREAM.PopBITs(vSLT[pSIDE->m_vINFO[nGROUP][i].m_dwSFCompress][1]);
					vSF[nGROUP][i].m_nS[2][j] = vSTREAM.PopBITs(vSLT[pSIDE->m_vINFO[nGROUP][i].m_dwSFCompress][1]);
				}
			}
		else if(!nGROUP)
		{
			for( int j=0; j<11; j++)
				vSF[nGROUP][i].m_nL[j] = vSTREAM.PopBITs(vSLT[pSIDE->m_vINFO[nGROUP][i].m_dwSFCompress][0]);

			for( j=11; j<21; j++)
				vSF[nGROUP][i].m_nL[j] = vSTREAM.PopBITs(vSLT[pSIDE->m_vINFO[nGROUP][i].m_dwSFCompress][1]);
		}
		else
		{
			int nBLOCK = 0;

			if( pSIDE->m_dwSCFSI[i] & 8 )
			{
				while( nBLOCK < 6 )
				{
					vSF[nGROUP][i].m_nL[nBLOCK] = vSF[nGROUP - 1][i].m_nL[nBLOCK];
					nBLOCK++;
				}
			}
			else
			{
				while( nBLOCK < 6 )
				{
					vSF[nGROUP][i].m_nL[nBLOCK] = vSTREAM.PopBITs(vSLT[pSIDE->m_vINFO[nGROUP][i].m_dwSFCompress][0]);
					nBLOCK++;
				}
			}

			if( pSIDE->m_dwSCFSI[i] & 4 )
			{
				while( nBLOCK < 11 )
				{
					vSF[nGROUP][i].m_nL[nBLOCK] = vSF[nGROUP - 1][i].m_nL[nBLOCK];
					nBLOCK++;
				}
			}
			else
			{
				while( nBLOCK < 11 )
				{
					vSF[nGROUP][i].m_nL[nBLOCK] = vSTREAM.PopBITs(vSLT[pSIDE->m_vINFO[nGROUP][i].m_dwSFCompress][0]);
					nBLOCK++;
				}
			}

			if( pSIDE->m_dwSCFSI[i] & 2 )
			{
				while( nBLOCK < 16 )
				{
					vSF[nGROUP][i].m_nL[nBLOCK] = vSF[nGROUP - 1][i].m_nL[nBLOCK];
					nBLOCK++;
				}
			}
			else
			{
				while( nBLOCK < 16 )
				{
					vSF[nGROUP][i].m_nL[nBLOCK] = vSTREAM.PopBITs(vSLT[pSIDE->m_vINFO[nGROUP][i].m_dwSFCompress][1]);
					nBLOCK++;
				}
			}

			if( pSIDE->m_dwSCFSI[i] & 1 )
			{
				while( nBLOCK < 21 )
				{
					vSF[nGROUP][i].m_nL[nBLOCK] = vSF[nGROUP - 1][i].m_nL[nBLOCK];
					nBLOCK++;
				}
			}
			else
			{
				while( nBLOCK < 21 )
				{
					vSF[nGROUP][i].m_nL[nBLOCK] = vSTREAM.PopBITs(vSLT[pSIDE->m_vINFO[nGROUP][i].m_dwSFCompress][1]);
					nBLOCK++;
				}
			}
		}
		int vHUFF[7];

		vHUFF[1] = m_vSFBT[nVersion][nFREQ].m_nL[pSIDE->m_vINFO[nGROUP][i].m_dwRegion0Count + pSIDE->m_vINFO[nGROUP][i].m_dwRegion1Count + 2];
		vHUFF[0] = m_vSFBT[nVersion][nFREQ].m_nL[pSIDE->m_vINFO[nGROUP][i].m_dwRegion0Count + 1];
		vHUFF[2] = 2 * pSIDE->m_vINFO[nGROUP][i].m_dwBigValues;

		if( vHUFF[2] > pMUSIC->m_nBandLimit )
			vHUFF[2] = pMUSIC->m_nBandLimit;

		if( vHUFF[1] > vHUFF[2] )
			vHUFF[1] = vHUFF[2];

		if( vHUFF[0] > vHUFF[2] )
			vHUFF[0] = vHUFF[2];

		vHUFF[5] = vHUFF[2] - vHUFF[1];
		vHUFF[4] = vHUFF[1] - vHUFF[0];

		Huffman( &vSTREAM, (int *) pMUSIC->m_pSAMPLE->m_vA[i][nGROUP], vHUFF[0], pSIDE->m_vINFO[nGROUP][i].m_dwTableSelect[0]);
		Huffman( &vSTREAM, (int *) pMUSIC->m_pSAMPLE->m_vA[i][nGROUP] + vHUFF[0], vHUFF[4], pSIDE->m_vINFO[nGROUP][i].m_dwTableSelect[1]);
		Huffman( &vSTREAM, (int *) pMUSIC->m_pSAMPLE->m_vA[i][nGROUP] + vHUFF[1], vHUFF[5], pSIDE->m_vINFO[nGROUP][i].m_dwTableSelect[2]);
		int nQUAD = pSIDE->m_vINFO[nGROUP][i].m_dwPart2_3_Length - (vSTREAM.GetPOS() - nSKIP);

		vHUFF[6] = HuffmanQ(
			&vSTREAM,
			(int *) pMUSIC->m_pSAMPLE->m_vA[i][nGROUP] + vHUFF[2],
			pMUSIC->m_nBandLimit - vHUFF[2],
			nQUAD,
			pSIDE->m_vINFO[nGROUP][i].m_dwCount1TableSelect);

		vHUFF[3] = vHUFF[2] + vHUFF[6];
		pMUSIC->m_vCOUNT[nGROUP][i] = vHUFF[3];

		if( pSIDE->m_vINFO[nGROUP][i].m_dwBlockType == 2 )
			vHUFF[3] = min( vHUFF[3], pMUSIC->m_nBandLimit12);
		else
			vHUFF[3] = min( vHUFF[3], pMUSIC->m_nBandLimit21);

		if( vHUFF[3] < 576 )
			memset( pMUSIC->m_pSAMPLE->m_vA[i][nGROUP] + vHUFF[3], 0, sizeof(SAMPLE) * (576 - vHUFF[3]));

		if(vSTREAM.IsOverRun())
			memset( pMUSIC->m_pSAMPLE->m_vA[i][nGROUP], 0, sizeof(SAMPLE) * 576);
	}

	for( i=0; i<nChannels; i++)
	{
		vCB[nGROUP][i].m_nStart = 12;
		vCB[nGROUP][i].m_nEnd = 22;

		if( pSIDE->m_vINFO[nGROUP][i].m_dwBlockType == 2 )
			if(pSIDE->m_vINFO[nGROUP][i].m_dwMixedBlockFlag)
			{
				vCB[nGROUP][i].m_nEnd = pMUSIC->m_nMixedCBEnd;
				vCB[nGROUP][i].m_nStart = 3;
			}
			else
			{
				vCB[nGROUP][i].m_nStart = 0;
				vCB[nGROUP][i].m_nEnd = 0;
			}

		vCB[nGROUP][i].m_nLBType = pSIDE->m_vINFO[nGROUP][i].m_dwBlockType;
		vCB[nGROUP][i].m_nType = 0;
		vCB[nGROUP][i].m_nMax = 0;

		for( int j=0; j<3; j++)
			vCB[nGROUP][i].m_nMaxS[j] = 0;

		FLOAT fInit = m_vValueL3[pSIDE->m_vINFO[nGROUP][i].m_dwGlobalGain + 6];
		int nINDEX = 0;

		for( j=0; j<vCB[nGROUP][i].m_nEnd; j++)
		{
			FLOAT fSCALE = fInit * m_vScaleL3[pSIDE->m_vINFO[nGROUP][i].m_dwSFScale][vTAB[pSIDE->m_vINFO[nGROUP][i].m_dwPreFlag][j]][vSF[nGROUP][i].m_nL[j]];
			int nBAND = m_vSFBT[nVersion][nFREQ].m_nL[j + 1] - m_vSFBT[nVersion][nFREQ].m_nL[j];

			for( int k=0; k<nBAND; k++)
			{
				if(pMUSIC->m_pSAMPLE->m_vA[i][nGROUP][nINDEX].m_nI)
				{
					vCB[nGROUP][i].m_nMax = j;

					if( pMUSIC->m_pSAMPLE->m_vA[i][nGROUP][nINDEX].m_nI < -IS_MAX || pMUSIC->m_pSAMPLE->m_vA[i][nGROUP][nINDEX].m_nI >= IS_MAX )
					{
						FLOAT fVALUE = (FLOAT) pMUSIC->m_pSAMPLE->m_vA[i][nGROUP][nINDEX].m_nI;
						pMUSIC->m_pSAMPLE->m_vA[i][nGROUP][nINDEX].m_fF = fSCALE * fVALUE * powf( fabs(fVALUE), 1.0f / 3.0f);
					}
					else
						pMUSIC->m_pSAMPLE->m_vA[i][nGROUP][nINDEX].m_fF = fSCALE * m_vPowL3[IS_MAX + pMUSIC->m_pSAMPLE->m_vA[i][nGROUP][nINDEX].m_nI];
				}
				else
					pMUSIC->m_pSAMPLE->m_vA[i][nGROUP][nINDEX].m_fF = 0.0f;

				nINDEX++;
			}

			if( nINDEX >= pMUSIC->m_vCOUNT[nGROUP][i] )
				break;
		}

		if( vCB[nGROUP][i].m_nStart < 12 )
		{
			int nISTART = nINDEX;
			int nREV = 0;

			for( j=0; j<3; j++)
			{
				vSUB[j] = fInit * m_vSubBlockL3[pSIDE->m_vINFO[nGROUP][i].m_dwSubblockGain[j]];
				vCB[nGROUP][i].m_nMaxS[j] = vCB[nGROUP][i].m_nStart;
			}

			for( j=vCB[nGROUP][i].m_nStart; j<13; j++)
			{
				int nBAND = m_vSFBT[nVersion][nFREQ].m_nS[j + 1] - m_vSFBT[nVersion][nFREQ].m_nS[j];

				for( int k=0; k<3; k++)
				{
					FLOAT fSCALE = vSUB[k] * m_vScaleL3[pSIDE->m_vINFO[nGROUP][i].m_dwSFScale][0][vSF[nGROUP][i].m_nS[k][j]];

					for( int l=0; l<nBAND; l++)
					{
						int nRINDEX = nREV + l;

						if(pMUSIC->m_pSAMPLE->m_vA[i][nGROUP][nINDEX].m_nI)
						{
							vCB[nGROUP][i].m_nMaxS[k] = j;

							if( pMUSIC->m_pSAMPLE->m_vA[i][nGROUP][nINDEX].m_nI < -IS_MAX ||
								pMUSIC->m_pSAMPLE->m_vA[i][nGROUP][nINDEX].m_nI >= IS_MAX )
							{
								FLOAT fVALUE = (FLOAT) pMUSIC->m_pSAMPLE->m_vA[i][nGROUP][nINDEX].m_nI;
								vRevBUF[nRINDEX][k] = fSCALE * fVALUE * powf( fabs(fVALUE), 1.0f / 3.0f);
							}
							else
								vRevBUF[nRINDEX][k] = fSCALE * m_vPowL3[IS_MAX + pMUSIC->m_pSAMPLE->m_vA[i][nGROUP][nINDEX].m_nI];
						}
						else
							vRevBUF[nRINDEX][k] = 0.0f;

						nINDEX++;
					}
				}

				if( nINDEX >= pMUSIC->m_vCOUNT[nGROUP][i] )
					break;
				nREV += nBAND;
			}

			memmove( &pMUSIC->m_pSAMPLE->m_vA[i][nGROUP][nISTART].m_fF, vRevBUF, (nINDEX - nISTART) * sizeof(FLOAT));
			pMUSIC->m_vCOUNT[nGROUP][i] = nINDEX;

			vCB[nGROUP][i].m_nMax = max( vCB[nGROUP][i].m_nMaxS[1], vCB[nGROUP][i].m_nMaxS[2]);
			vCB[nGROUP][i].m_nMax = max( vCB[nGROUP][i].m_nMaxS[0], vCB[nGROUP][i].m_nMax);
			vCB[nGROUP][i].m_nType = 1;
		}
	}

	if(nMSMode)
	{
		int nCount;

		if(nISMode)
			nCount = GetBandIndex( nVersion, nFREQ, vCB[nGROUP][1].m_nType, vCB[nGROUP][1].m_nMax);
		else
			nCount = max( pMUSIC->m_vCOUNT[nGROUP][0], pMUSIC->m_vCOUNT[nGROUP][1]);

		ProcessMS( (FLOAT *) pMUSIC->m_pSAMPLE->m_vA[0][nGROUP], nCount);
	}

	if(nISMode)
		if(nVersion)
			ProcessIS2( (FLOAT *) pMUSIC->m_pSAMPLE->m_vA[0][nGROUP], &vSF[nGROUP][1], &vIS, vCB[nGROUP], nVersion, nFREQ, nMSMode, pMUSIC->m_vCOUNT[nGROUP][0]);
		else
			ProcessIS1( (FLOAT *) pMUSIC->m_pSAMPLE->m_vA[0][nGROUP], &vSF[nGROUP][1], vCB[nGROUP], nVersion, nFREQ, nMSMode, pMUSIC->m_vCOUNT[nGROUP][0]);

	if( nMSMode || nISMode )
		if( pMUSIC->m_vCOUNT[nGROUP][0] < pMUSIC->m_vCOUNT[nGROUP][1] )
			pMUSIC->m_vCOUNT[nGROUP][0] = pMUSIC->m_vCOUNT[nGROUP][1];
		else
			pMUSIC->m_vCOUNT[nGROUP][1] = pMUSIC->m_vCOUNT[nGROUP][0];

	for( i=0; i<nChannels; i++)
		if(vCB[nGROUP][i].m_nEnd)
		{
			int nCount = pSIDE->m_vINFO[nGROUP][i].m_dwMixedBlockFlag ? 1 : (pMUSIC->m_vCOUNT[nGROUP][i] + 7) / 18;

			if( nCount > 31 )
				nCount = 31;

			Antialias( (FLOAT *) pMUSIC->m_pSAMPLE->m_vA[i][nGROUP], nCount);
			nCount = 18 * nCount + 8;

			if( pMUSIC->m_vCOUNT[nGROUP][i] < nCount )
				pMUSIC->m_vCOUNT[nGROUP][i] = nCount;
		}

	pMUSIC->m_pXFormProc( pMUSIC, pSIDE, pOUTPUT, nVersion, nFREQ, nGROUP);
}

int CT3DMusic::HuffmanQ( CTBitStream *pSTREAM,
						 int *pVALUE,
						 int nCount,
						 int nBits,
						 int nTable)
{
	static BYTE vQT[64][2] = {
		{ 6, 11}, { 6, 15}, { 6, 13}, { 6, 14}, { 6,  7},
		{ 6,  5}, { 5,  9}, { 5,  9}, { 5,  6}, { 5,  6},
		{ 5,  3}, { 5,  3}, { 5, 10}, { 5, 10}, { 5, 12},
		{ 5, 12}, { 4,  2}, { 4,  2}, { 4,  2}, { 4,  2},
		{ 4,  1}, { 4,  1}, { 4,  1}, { 4,  1}, { 4,  4},
		{ 4,  4}, { 4,  4}, { 4,  4}, { 4,  8}, { 4,  8},
		{ 4,  8}, { 4,  8}, { 1,  0}, { 1,  0}, { 1,  0},
		{ 1,  0}, { 1,  0}, { 1,  0}, { 1,  0}, { 1,  0},
		{ 1,  0}, { 1,  0}, { 1,  0}, { 1,  0}, { 1,  0},
		{ 1,  0}, { 1,  0}, { 1,  0}, { 1,  0}, { 1,  0},
		{ 1,  0}, { 1,  0}, { 1,  0}, { 1,  0}, { 1,  0},
		{ 1,  0}, { 1,  0}, { 1,  0}, { 1,  0}, { 1,  0},
		{ 1,  0}, { 1,  0}, { 1,  0}, { 1,  0}};

	int nResult = -1;
	int nBackup = 15;
	int nIndex;

	nCount >>= 2;
	if(nTable)
	{
		for( nIndex = 0; nIndex < nCount; nIndex++)
		{
			if( nBits < 4 )
				break;
			int nValue = pSTREAM->PopBITs(4) ^ 15;

			nBits -= 4;
			if(nValue)
			{
				nResult = nIndex;
				nBackup = nValue;
			}

			for( int i=0; i<4; i++)
			{
				int nDATA = (nValue >> (3 - i)) & 0x01;

				if(nDATA)
				{
					if(pSTREAM->PopBITs(1))
						nDATA = -nDATA;
					nBits--;
				}

				pVALUE[nIndex * 4 + i] = nDATA;
			}

			if(pSTREAM->IsOverRun())
				break;
		}
	}
	else
	{
		for( nIndex = 0; nIndex < nCount; nIndex++)
		{
			if( nBits <= 0 )
				break;
			int nValue = pSTREAM->ReadBITs(6);

			pSTREAM->SeekBITs(vQT[nValue][0]);
			nBits -= vQT[nValue][0];
			nValue = vQT[nValue][1];

			if(nValue)
			{
				nResult = nIndex;
				nBackup = nValue;
			}

			for( int i=0; i<4; i++)
			{
				int nDATA = (nValue >> (3 - i)) & 0x01;

				if(nDATA)
				{
					if(pSTREAM->PopBITs(1))
						nDATA = -nDATA;
					nBits--;
				}

				pVALUE[nIndex * 4 + i] = nDATA;
			}

			if(pSTREAM->IsOverRun())
				break;
		}
	}

	if( nBits < 0 )
	{
		nIndex--;
		for( int i=0; i<4; i++)
			pVALUE[nIndex * 4 + i] = 0;
	}

	nResult = (nResult + 1) << 2;
	if(!(nBackup & 3))
		nResult -= 2;

	return nResult;
}

void CT3DMusic::Huffman( CTBitStream *pSTREAM,
						 int *pVALUE,
						 int nCount,
						 int nTable)
{
	static HUFF_ELEMENT vHT0[] = { 0, 0, 0, 64};
	static HUFF_ELEMENT vHT1[] = {
		0xFF000003, 0x03010102, 0x03010001, 0x02000101, 0x02000101,
		0x01000000, 0x01000000, 0x01000000, 0x01000000};

	static HUFF_ELEMENT vHT2[] = {
		0xFF000006, 0x06020202, 0x06020001, 0x05020102, 0x05020102,
		0x05010202, 0x05010202, 0x05000201, 0x05000201, 0x03010102,
		0x03010102, 0x03010102, 0x03010102, 0x03010102, 0x03010102,
		0x03010102, 0x03010102, 0x03010001, 0x03010001, 0x03010001,
		0x03010001, 0x03010001, 0x03010001, 0x03010001, 0x03010001,
		0x03000101, 0x03000101, 0x03000101, 0x03000101, 0x03000101,
		0x03000101, 0x03000101, 0x03000101, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000};

	static HUFF_ELEMENT vHT3[] = {
		0xFF000006, 0x06020202, 0x06020001, 0x05020102, 0x05020102,
		0x05010202, 0x05010202, 0x05000201, 0x05000201, 0x03000101,
		0x03000101, 0x03000101, 0x03000101, 0x03000101, 0x03000101,
		0x03000101, 0x03000101, 0x02010102, 0x02010102, 0x02010102,
		0x02010102, 0x02010102, 0x02010102, 0x02010102, 0x02010102,
		0x02010102, 0x02010102, 0x02010102, 0x02010102, 0x02010102,
		0x02010102, 0x02010102, 0x02010102, 0x02010001, 0x02010001,
		0x02010001, 0x02010001, 0x02010001, 0x02010001, 0x02010001,
		0x02010001, 0x02010001, 0x02010001, 0x02010001, 0x02010001,
		0x02010001, 0x02010001, 0x02010001, 0x02010001, 0x02000000,
		0x02000000, 0x02000000, 0x02000000, 0x02000000, 0x02000000,
		0x02000000, 0x02000000, 0x02000000, 0x02000000, 0x02000000,
		0x02000000, 0x02000000, 0x02000000, 0x02000000, 0x02000000};

	static HUFF_ELEMENT vHT5[] = {
		0xFF000008, 0x08030302, 0x08030202, 0x07020302, 0x07020302,
		0x06010302, 0x06010302, 0x06010302, 0x06010302, 0x07030102,
		0x07030102, 0x07030001, 0x07030001, 0x07000301, 0x07000301,
		0x07020202, 0x07020202, 0x06020102, 0x06020102, 0x06020102,
		0x06020102, 0x06010202, 0x06010202, 0x06010202, 0x06010202,
		0x06020001, 0x06020001, 0x06020001, 0x06020001, 0x06000201,
		0x06000201, 0x06000201, 0x06000201, 0x03010102, 0x03010102,
		0x03010102, 0x03010102, 0x03010102, 0x03010102, 0x03010102,
		0x03010102, 0x03010102, 0x03010102, 0x03010102, 0x03010102,
		0x03010102, 0x03010102, 0x03010102, 0x03010102, 0x03010102,
		0x03010102, 0x03010102, 0x03010102, 0x03010102, 0x03010102,
		0x03010102, 0x03010102, 0x03010102, 0x03010102, 0x03010102,
		0x03010102, 0x03010102, 0x03010102, 0x03010102, 0x03010102,
		0x03010001, 0x03010001, 0x03010001, 0x03010001, 0x03010001,
		0x03010001, 0x03010001, 0x03010001, 0x03010001, 0x03010001,
		0x03010001, 0x03010001, 0x03010001, 0x03010001, 0x03010001,
		0x03010001, 0x03010001, 0x03010001, 0x03010001, 0x03010001,
		0x03010001, 0x03010001, 0x03010001, 0x03010001, 0x03010001,
		0x03010001, 0x03010001, 0x03010001, 0x03010001, 0x03010001,
		0x03010001, 0x03010001, 0x03000101, 0x03000101, 0x03000101,
		0x03000101, 0x03000101, 0x03000101, 0x03000101, 0x03000101,
		0x03000101, 0x03000101, 0x03000101, 0x03000101, 0x03000101,
		0x03000101, 0x03000101, 0x03000101, 0x03000101, 0x03000101,
		0x03000101, 0x03000101, 0x03000101, 0x03000101, 0x03000101,
		0x03000101, 0x03000101, 0x03000101, 0x03000101, 0x03000101,
		0x03000101, 0x03000101, 0x03000101, 0x03000101, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000};

	static HUFF_ELEMENT vHT6[] = {
		0xFF000007, 0x07030302, 0x07030001, 0x06030202, 0x06030202,
		0x06020302, 0x06020302, 0x06000301, 0x06000301, 0x05030102,
		0x05030102, 0x05030102, 0x05030102, 0x05010302, 0x05010302,
		0x05010302, 0x05010302, 0x05020202, 0x05020202, 0x05020202,
		0x05020202, 0x05020001, 0x05020001, 0x05020001, 0x05020001,
		0x04020102, 0x04020102, 0x04020102, 0x04020102, 0x04020102,
		0x04020102, 0x04020102, 0x04020102, 0x04010202, 0x04010202,
		0x04010202, 0x04010202, 0x04010202, 0x04010202, 0x04010202,
		0x04010202, 0x04000201, 0x04000201, 0x04000201, 0x04000201,
		0x04000201, 0x04000201, 0x04000201, 0x04000201, 0x03010001,
		0x03010001, 0x03010001, 0x03010001, 0x03010001, 0x03010001,
		0x03010001, 0x03010001, 0x03010001, 0x03010001, 0x03010001,
		0x03010001, 0x03010001, 0x03010001, 0x03010001, 0x03010001,
		0x02010102, 0x02010102, 0x02010102, 0x02010102, 0x02010102,
		0x02010102, 0x02010102, 0x02010102, 0x02010102, 0x02010102,
		0x02010102, 0x02010102, 0x02010102, 0x02010102, 0x02010102,
		0x02010102, 0x02010102, 0x02010102, 0x02010102, 0x02010102,
		0x02010102, 0x02010102, 0x02010102, 0x02010102, 0x02010102,
		0x02010102, 0x02010102, 0x02010102, 0x02010102, 0x02010102,
		0x02010102, 0x02010102, 0x03000101, 0x03000101, 0x03000101,
		0x03000101, 0x03000101, 0x03000101, 0x03000101, 0x03000101,
		0x03000101, 0x03000101, 0x03000101, 0x03000101, 0x03000101,
		0x03000101, 0x03000101, 0x03000101, 0x03000000, 0x03000000,
		0x03000000, 0x03000000, 0x03000000, 0x03000000, 0x03000000,
		0x03000000, 0x03000000, 0x03000000, 0x03000000, 0x03000000,
		0x03000000, 0x03000000, 0x03000000, 0x03000000};

	static HUFF_ELEMENT vHT7[] = {
		0xFF000006, 0x00000041, 0x00000052, 0x0000005B, 0x00000060,
		0x00000063, 0x00000068, 0x0000006B, 0x06020102, 0x05010202,
		0x05010202, 0x06020001, 0x06000201, 0x04010102, 0x04010102,
		0x04010102, 0x04010102, 0x03010001, 0x03010001, 0x03010001,
		0x03010001, 0x03010001, 0x03010001, 0x03010001, 0x03010001,
		0x03000101, 0x03000101, 0x03000101, 0x03000101, 0x03000101,
		0x03000101, 0x03000101, 0x03000101, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0xFF000004, 0x04050502, 0x04050402, 0x04040502, 0x04030502,
		0x03050302, 0x03050302, 0x03040402, 0x03040402, 0x03050202,
		0x03050202, 0x03020502, 0x03020502, 0x02050102, 0x02050102,
		0x02050102, 0x02050102, 0xFF000003, 0x02010502, 0x02010502,
		0x03050001, 0x03040302, 0x02000501, 0x02000501, 0x03030402,
		0x03030302, 0xFF000002, 0x02040202, 0x02020402, 0x01040102,
		0x01040102, 0xFF000001, 0x01010402, 0x01000401, 0xFF000002,
		0x02040001, 0x02030202, 0x02020302, 0x02030001, 0xFF000001,
		0x01030102, 0x01010302, 0xFF000001, 0x01000301, 0x01020202};

	static HUFF_ELEMENT vHT8[] = {
		0xFF000008, 0x00000101, 0x0000010A, 0x0000010F, 0x08050102,
		0x08010502, 0x00000112, 0x00000115, 0x08040202, 0x08020402,
		0x08040102, 0x07010402, 0x07010402, 0x08040001, 0x08000401,
		0x08030202, 0x08020302, 0x08030102, 0x08010302, 0x08030001,
		0x08000301, 0x06020202, 0x06020202, 0x06020202, 0x06020202,
		0x06020001, 0x06020001, 0x06020001, 0x06020001, 0x06000201,
		0x06000201, 0x06000201, 0x06000201, 0x04020102, 0x04020102,
		0x04020102, 0x04020102, 0x04020102, 0x04020102, 0x04020102,
		0x04020102, 0x04020102, 0x04020102, 0x04020102, 0x04020102,
		0x04020102, 0x04020102, 0x04020102, 0x04020102, 0x04010202,
		0x04010202, 0x04010202, 0x04010202, 0x04010202, 0x04010202,
		0x04010202, 0x04010202, 0x04010202, 0x04010202, 0x04010202,
		0x04010202, 0x04010202, 0x04010202, 0x04010202, 0x04010202,
		0x02010102, 0x02010102, 0x02010102, 0x02010102, 0x02010102,
		0x02010102, 0x02010102, 0x02010102, 0x02010102, 0x02010102,
		0x02010102, 0x02010102, 0x02010102, 0x02010102, 0x02010102,
		0x02010102, 0x02010102, 0x02010102, 0x02010102, 0x02010102,
		0x02010102, 0x02010102, 0x02010102, 0x02010102, 0x02010102,
		0x02010102, 0x02010102, 0x02010102, 0x02010102, 0x02010102,
		0x02010102, 0x02010102, 0x02010102, 0x02010102, 0x02010102,
		0x02010102, 0x02010102, 0x02010102, 0x02010102, 0x02010102,
		0x02010102, 0x02010102, 0x02010102, 0x02010102, 0x02010102,
		0x02010102, 0x02010102, 0x02010102, 0x02010102, 0x02010102,
		0x02010102, 0x02010102, 0x02010102, 0x02010102, 0x02010102,
		0x02010102, 0x02010102, 0x02010102, 0x02010102, 0x02010102,
		0x02010102, 0x02010102, 0x02010102, 0x02010102, 0x03010001,
		0x03010001, 0x03010001, 0x03010001, 0x03010001, 0x03010001,
		0x03010001, 0x03010001, 0x03010001, 0x03010001, 0x03010001,
		0x03010001, 0x03010001, 0x03010001, 0x03010001, 0x03010001,
		0x03010001, 0x03010001, 0x03010001, 0x03010001, 0x03010001,
		0x03010001, 0x03010001, 0x03010001, 0x03010001, 0x03010001,
		0x03010001, 0x03010001, 0x03010001, 0x03010001, 0x03010001,
		0x03010001, 0x03000101, 0x03000101, 0x03000101, 0x03000101,
		0x03000101, 0x03000101, 0x03000101, 0x03000101, 0x03000101,
		0x03000101, 0x03000101, 0x03000101, 0x03000101, 0x03000101,
		0x03000101, 0x03000101, 0x03000101, 0x03000101, 0x03000101,
		0x03000101, 0x03000101, 0x03000101, 0x03000101, 0x03000101,
		0x03000101, 0x03000101, 0x03000101, 0x03000101, 0x03000101,
		0x03000101, 0x03000101, 0x03000101, 0x02000000, 0x02000000,
		0x02000000, 0x02000000, 0x02000000, 0x02000000, 0x02000000,
		0x02000000, 0x02000000, 0x02000000, 0x02000000, 0x02000000,
		0x02000000, 0x02000000, 0x02000000, 0x02000000, 0x02000000,
		0x02000000, 0x02000000, 0x02000000, 0x02000000, 0x02000000,
		0x02000000, 0x02000000, 0x02000000, 0x02000000, 0x02000000,
		0x02000000, 0x02000000, 0x02000000, 0x02000000, 0x02000000,
		0x02000000, 0x02000000, 0x02000000, 0x02000000, 0x02000000,
		0x02000000, 0x02000000, 0x02000000, 0x02000000, 0x02000000,
		0x02000000, 0x02000000, 0x02000000, 0x02000000, 0x02000000,
		0x02000000, 0x02000000, 0x02000000, 0x02000000, 0x02000000,
		0x02000000, 0x02000000, 0x02000000, 0x02000000, 0x02000000,
		0x02000000, 0x02000000, 0x02000000, 0x02000000, 0x02000000,
		0x02000000, 0x02000000, 0xFF000003, 0x03050502, 0x03040502,
		0x02050402, 0x02050402, 0x01030502, 0x01030502, 0x01030502,
		0x01030502, 0xFF000002, 0x02050302, 0x02040402, 0x01050202,
		0x01050202, 0xFF000001, 0x01020502, 0x01050001, 0xFF000001,
		0x01040302, 0x01030402, 0xFF000001, 0x01000501, 0x01030302};

	static HUFF_ELEMENT vHT9[] = {
		0xFF000006, 0x00000041, 0x0000004A, 0x0000004F, 0x00000052,
		0x00000057, 0x0000005A, 0x06040102, 0x06010402, 0x06030202,
		0x06020302, 0x05030102, 0x05030102, 0x05010302, 0x05010302,
		0x06030001, 0x06000301, 0x05020202, 0x05020202, 0x05020001,
		0x05020001, 0x04020102, 0x04020102, 0x04020102, 0x04020102,
		0x04010202, 0x04010202, 0x04010202, 0x04010202, 0x04000201,
		0x04000201, 0x04000201, 0x04000201, 0x03010102, 0x03010102,
		0x03010102, 0x03010102, 0x03010102, 0x03010102, 0x03010102,
		0x03010102, 0x03010001, 0x03010001, 0x03010001, 0x03010001,
		0x03010001, 0x03010001, 0x03010001, 0x03010001, 0x03000101,
		0x03000101, 0x03000101, 0x03000101, 0x03000101, 0x03000101,
		0x03000101, 0x03000101, 0x03000000, 0x03000000, 0x03000000,
		0x03000000, 0x03000000, 0x03000000, 0x03000000, 0x03000000,
		0xFF000003, 0x03050502, 0x03050402, 0x02050302, 0x02050302,
		0x02030502, 0x02030502, 0x03040502, 0x03050001, 0xFF000002,
		0x02040402, 0x02050202, 0x02020502, 0x02050102, 0xFF000001,
		0x01010502, 0x01040302, 0xFF000002, 0x01030402, 0x01030402,
		0x02000501, 0x02040001, 0xFF000001, 0x01040202, 0x01020402,
		0xFF000001, 0x01030302, 0x01000401};

	static HUFF_ELEMENT vHT10[] = {
		0xFF000008, 0x00000101, 0x0000010A, 0x0000010F, 0x00000118,
		0x0000011B, 0x00000120, 0x00000125, 0x08070102, 0x08010702,
		0x0000012A, 0x0000012D, 0x00000132, 0x08060102, 0x08010602,
		0x08000601, 0x00000137, 0x0000013A, 0x0000013D, 0x08040102,
		0x08010402, 0x08000401, 0x08030202, 0x08020302, 0x08030001,
		0x07030102, 0x07030102, 0x07010302, 0x07010302, 0x07000301,
		0x07000301, 0x07020202, 0x07020202, 0x06020102, 0x06020102,
		0x06020102, 0x06020102, 0x06010202, 0x06010202, 0x06010202,
		0x06010202, 0x06020001, 0x06020001, 0x06020001, 0x06020001,
		0x06000201, 0x06000201, 0x06000201, 0x06000201, 0x04010102,
		0x04010102, 0x04010102, 0x04010102, 0x04010102, 0x04010102,
		0x04010102, 0x04010102, 0x04010102, 0x04010102, 0x04010102,
		0x04010102, 0x04010102, 0x04010102, 0x04010102, 0x04010102,
		0x03010001, 0x03010001, 0x03010001, 0x03010001, 0x03010001,
		0x03010001, 0x03010001, 0x03010001, 0x03010001, 0x03010001,
		0x03010001, 0x03010001, 0x03010001, 0x03010001, 0x03010001,
		0x03010001, 0x03010001, 0x03010001, 0x03010001, 0x03010001,
		0x03010001, 0x03010001, 0x03010001, 0x03010001, 0x03010001,
		0x03010001, 0x03010001, 0x03010001, 0x03010001, 0x03010001,
		0x03010001, 0x03010001, 0x03000101, 0x03000101, 0x03000101,
		0x03000101, 0x03000101, 0x03000101, 0x03000101, 0x03000101,
		0x03000101, 0x03000101, 0x03000101, 0x03000101, 0x03000101,
		0x03000101, 0x03000101, 0x03000101, 0x03000101, 0x03000101,
		0x03000101, 0x03000101, 0x03000101, 0x03000101, 0x03000101,
		0x03000101, 0x03000101, 0x03000101, 0x03000101, 0x03000101,
		0x03000101, 0x03000101, 0x03000101, 0x03000101, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0xFF000003, 0x03070702, 0x03070602,
		0x03060702, 0x03070502, 0x03050702, 0x03060602, 0x02070402,
		0x02070402, 0xFF000002, 0x02040702, 0x02060502, 0x02050602,
		0x02070302, 0xFF000003, 0x02030702, 0x02030702, 0x02060402,
		0x02060402, 0x03050502, 0x03040502, 0x02030602, 0x02030602,
		0xFF000001, 0x01070202, 0x01020702, 0xFF000002, 0x02040602,
		0x02070001, 0x01000701, 0x01000701, 0xFF000002, 0x01020602,
		0x01020602, 0x02050402, 0x02050302, 0xFF000002, 0x01060001,
		0x01060001, 0x02030502, 0x02040402, 0xFF000001, 0x01060302,
		0x01060202, 0xFF000002, 0x02050202, 0x02020502, 0x01050102,
		0x01050102, 0xFF000002, 0x01010502, 0x01010502, 0x02040302,
		0x02030402, 0xFF000001, 0x01050001, 0x01000501, 0xFF000001,
		0x01040202, 0x01020402, 0xFF000001, 0x01030302, 0x01040001};

	static HUFF_ELEMENT vHT11[] = {
		0xFF000008, 0x00000101, 0x00000106, 0x0000010F, 0x00000114,
		0x00000117, 0x08070202, 0x08020702, 0x0000011C, 0x07010702,
		0x07010702, 0x08070102, 0x08000701, 0x08060302, 0x08030602,
		0x08000601, 0x0000011F, 0x00000122, 0x08050102, 0x07020602,
		0x07020602, 0x08060202, 0x08060001, 0x07060102, 0x07060102,
		0x07010602, 0x07010602, 0x08010502, 0x08040302, 0x08000501,
		0x00000125, 0x08040202, 0x08020402, 0x08040102, 0x08010402,
		0x08040001, 0x08000401, 0x07030202, 0x07030202, 0x07020302,
		0x07020302, 0x06030102, 0x06030102, 0x06030102, 0x06030102,
		0x06010302, 0x06010302, 0x06010302, 0x06010302, 0x07030001,
		0x07030001, 0x07000301, 0x07000301, 0x06020202, 0x06020202,
		0x06020202, 0x06020202, 0x05010202, 0x05010202, 0x05010202,
		0x05010202, 0x05010202, 0x05010202, 0x05010202, 0x05010202,
		0x04020102, 0x04020102, 0x04020102, 0x04020102, 0x04020102,
		0x04020102, 0x04020102, 0x04020102, 0x04020102, 0x04020102,
		0x04020102, 0x04020102, 0x04020102, 0x04020102, 0x04020102,
		0x04020102, 0x05020001, 0x05020001, 0x05020001, 0x05020001,
		0x05020001, 0x05020001, 0x05020001, 0x05020001, 0x05000201,
		0x05000201, 0x05000201, 0x05000201, 0x05000201, 0x05000201,
		0x05000201, 0x05000201, 0x03010102, 0x03010102, 0x03010102,
		0x03010102, 0x03010102, 0x03010102, 0x03010102, 0x03010102,
		0x03010102, 0x03010102, 0x03010102, 0x03010102, 0x03010102,
		0x03010102, 0x03010102, 0x03010102, 0x03010102, 0x03010102,
		0x03010102, 0x03010102, 0x03010102, 0x03010102, 0x03010102,
		0x03010102, 0x03010102, 0x03010102, 0x03010102, 0x03010102,
		0x03010102, 0x03010102, 0x03010102, 0x03010102, 0x03010001,
		0x03010001, 0x03010001, 0x03010001, 0x03010001, 0x03010001,
		0x03010001, 0x03010001, 0x03010001, 0x03010001, 0x03010001,
		0x03010001, 0x03010001, 0x03010001, 0x03010001, 0x03010001,
		0x03010001, 0x03010001, 0x03010001, 0x03010001, 0x03010001,
		0x03010001, 0x03010001, 0x03010001, 0x03010001, 0x03010001,
		0x03010001, 0x03010001, 0x03010001, 0x03010001, 0x03010001,
		0x03010001, 0x03000101, 0x03000101, 0x03000101, 0x03000101,
		0x03000101, 0x03000101, 0x03000101, 0x03000101, 0x03000101,
		0x03000101, 0x03000101, 0x03000101, 0x03000101, 0x03000101,
		0x03000101, 0x03000101, 0x03000101, 0x03000101, 0x03000101,
		0x03000101, 0x03000101, 0x03000101, 0x03000101, 0x03000101,
		0x03000101, 0x03000101, 0x03000101, 0x03000101, 0x03000101,
		0x03000101, 0x03000101, 0x03000101, 0x02000000, 0x02000000,
		0x02000000, 0x02000000, 0x02000000, 0x02000000, 0x02000000,
		0x02000000, 0x02000000, 0x02000000, 0x02000000, 0x02000000,
		0x02000000, 0x02000000, 0x02000000, 0x02000000, 0x02000000,
		0x02000000, 0x02000000, 0x02000000, 0x02000000, 0x02000000,
		0x02000000, 0x02000000, 0x02000000, 0x02000000, 0x02000000,
		0x02000000, 0x02000000, 0x02000000, 0x02000000, 0x02000000,
		0x02000000, 0x02000000, 0x02000000, 0x02000000, 0x02000000,
		0x02000000, 0x02000000, 0x02000000, 0x02000000, 0x02000000,
		0x02000000, 0x02000000, 0x02000000, 0x02000000, 0x02000000,
		0x02000000, 0x02000000, 0x02000000, 0x02000000, 0x02000000,
		0x02000000, 0x02000000, 0x02000000, 0x02000000, 0x02000000,
		0x02000000, 0x02000000, 0x02000000, 0x02000000, 0x02000000,
		0x02000000, 0x02000000, 0xFF000002, 0x02070702, 0x02070602,
		0x02060702, 0x02050702, 0xFF000003, 0x02060602, 0x02060602,
		0x02070402, 0x02070402, 0x02040702, 0x02040702, 0x03070502,
		0x03050502, 0xFF000002, 0x02060502, 0x02050602, 0x01070302,
		0x01070302, 0xFF000001, 0x01030702, 0x01060402, 0xFF000002,
		0x02050402, 0x02040502, 0x02050302, 0x02030502, 0xFF000001,
		0x01040602, 0x01070001, 0xFF000001, 0x01040402, 0x01050202,
		0xFF000001, 0x01020502, 0x01050001, 0xFF000001, 0x01030402,
		0x01030302};

	static HUFF_ELEMENT vHT12[] = {
		0xFF000007, 0x00000081, 0x0000008A, 0x0000008F, 0x00000092,
		0x00000097, 0x0000009A, 0x0000009D, 0x000000A2, 0x000000A5,
		0x000000A8, 0x07060202, 0x07020602, 0x07010602, 0x000000AD,
		0x000000B0, 0x000000B3, 0x07050102, 0x07010502, 0x07040302,
		0x07030402, 0x000000B6, 0x07040202, 0x07020402, 0x07040102,
		0x06030302, 0x06030302, 0x06010402, 0x06010402, 0x06030202,
		0x06030202, 0x06020302, 0x06020302, 0x07000401, 0x07030001,
		0x06000301, 0x06000301, 0x05030102, 0x05030102, 0x05030102,
		0x05030102, 0x05010302, 0x05010302, 0x05010302, 0x05010302,
		0x05020202, 0x05020202, 0x05020202, 0x05020202, 0x04020102,
		0x04020102, 0x04020102, 0x04020102, 0x04020102, 0x04020102,
		0x04020102, 0x04020102, 0x04010202, 0x04010202, 0x04010202,
		0x04010202, 0x04010202, 0x04010202, 0x04010202, 0x04010202,
		0x05020001, 0x05020001, 0x05020001, 0x05020001, 0x05000201,
		0x05000201, 0x05000201, 0x05000201, 0x04000000, 0x04000000,
		0x04000000, 0x04000000, 0x04000000, 0x04000000, 0x04000000,
		0x04000000, 0x03010102, 0x03010102, 0x03010102, 0x03010102,
		0x03010102, 0x03010102, 0x03010102, 0x03010102, 0x03010102,
		0x03010102, 0x03010102, 0x03010102, 0x03010102, 0x03010102,
		0x03010102, 0x03010102, 0x03010001, 0x03010001, 0x03010001,
		0x03010001, 0x03010001, 0x03010001, 0x03010001, 0x03010001,
		0x03010001, 0x03010001, 0x03010001, 0x03010001, 0x03010001,
		0x03010001, 0x03010001, 0x03010001, 0x03000101, 0x03000101,
		0x03000101, 0x03000101, 0x03000101, 0x03000101, 0x03000101,
		0x03000101, 0x03000101, 0x03000101, 0x03000101, 0x03000101,
		0x03000101, 0x03000101, 0x03000101, 0x03000101, 0xFF000003,
		0x03070702, 0x03070602, 0x02060702, 0x02060702, 0x02070502,
		0x02070502, 0x02050702, 0x02050702, 0xFF000002, 0x02060602,
		0x02070402, 0x02040702, 0x02050602, 0xFF000001, 0x01060502,
		0x01070302, 0xFF000002, 0x02030702, 0x02050502, 0x01070202,
		0x01070202, 0xFF000001, 0x01020702, 0x01060402, 0xFF000001,
		0x01040602, 0x01070102, 0xFF000002, 0x01010702, 0x01010702,
		0x02070001, 0x02000701, 0xFF000001, 0x01060302, 0x01030602,
		0xFF000001, 0x01050402, 0x01040502, 0xFF000002, 0x01040402,
		0x01040402, 0x02060001, 0x02050001, 0xFF000001, 0x01060102,
		0x01000601, 0xFF000001, 0x01050302, 0x01030502, 0xFF000001,
		0x01050202, 0x01020502, 0xFF000001, 0x01000501, 0x01040001};

	static HUFF_ELEMENT vHT13[] = {
		0xFF000006, 0x00000041, 0x00000082, 0x000000C3, 0x000000E4,
		0x00000105, 0x00000116, 0x0000011F, 0x00000130, 0x00000139,
		0x0000013E, 0x00000143, 0x00000146, 0x06020102, 0x06010202,
		0x06020001, 0x06000201, 0x04010102, 0x04010102, 0x04010102,
		0x04010102, 0x04010001, 0x04010001, 0x04010001, 0x04010001,
		0x03000101, 0x03000101, 0x03000101, 0x03000101, 0x03000101,
		0x03000101, 0x03000101, 0x03000101, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0xFF000006, 0x00000108, 0x00000111, 0x0000011A, 0x00000123,
		0x0000012C, 0x00000131, 0x00000136, 0x0000013F, 0x00000144,
		0x00000147, 0x0000014C, 0x00000151, 0x00000156, 0x0000015B,
		0x060F0102, 0x06010F02, 0x06000F01, 0x00000160, 0x00000163,
		0x00000166, 0x06020E02, 0x00000169, 0x060E0102, 0x06010E02,
		0x0000016C, 0x0000016F, 0x00000172, 0x00000175, 0x00000178,
		0x0000017B, 0x06060C02, 0x060D0302, 0x0000017E, 0x060D0202,
		0x06020D02, 0x060D0102, 0x06070B02, 0x00000181, 0x00000184,
		0x06030C02, 0x00000187, 0x060B0402, 0x05010D02, 0x05010D02,
		0x060D0001, 0x06000D01, 0x060A0802, 0x06080A02, 0x060C0402,
		0x06040C02, 0x060B0602, 0x06060B02, 0x050C0302, 0x050C0302,
		0x050C0202, 0x050C0202, 0x05020C02, 0x05020C02, 0x050B0502,
		0x050B0502, 0x06050B02, 0x06090802, 0x050C0102, 0x050C0102,
		0xFF000006, 0x05010C02, 0x05010C02, 0x06080902, 0x060C0001,
		0x05000C01, 0x05000C01, 0x06040B02, 0x060A0602, 0x06060A02,
		0x06090702, 0x050B0302, 0x050B0302, 0x05030B02, 0x05030B02,
		0x06080802, 0x060A0502, 0x050B0202, 0x050B0202, 0x06050A02,
		0x06090602, 0x05040A02, 0x05040A02, 0x06080702, 0x06070802,
		0x05040902, 0x05040902, 0x06070702, 0x06060702, 0x04020B02,
		0x04020B02, 0x04020B02, 0x04020B02, 0x040B0102, 0x040B0102,
		0x040B0102, 0x040B0102, 0x04010B02, 0x04010B02, 0x04010B02,
		0x04010B02, 0x050B0001, 0x050B0001, 0x05000B01, 0x05000B01,
		0x05060902, 0x05060902, 0x050A0402, 0x050A0402, 0x050A0302,
		0x050A0302, 0x05030A02, 0x05030A02, 0x05090502, 0x05090502,
		0x05050902, 0x05050902, 0x040A0202, 0x040A0202, 0x040A0202,
		0x040A0202, 0x04020A02, 0x04020A02, 0x04020A02, 0x04020A02,
		0xFF000005, 0x040A0102, 0x040A0102, 0x04010A02, 0x04010A02,
		0x050A0001, 0x05080602, 0x04000A01, 0x04000A01, 0x05060802,
		0x05090402, 0x04030902, 0x04030902, 0x05090302, 0x05080502,
		0x05050802, 0x05070602, 0x04090202, 0x04090202, 0x04020902,
		0x04020902, 0x05070502, 0x05050702, 0x04080302, 0x04080302,
		0x04030802, 0x04030802, 0x05060602, 0x05070402, 0x05040702,
		0x05060502, 0x05050602, 0x05030702, 0xFF000005, 0x03090102,
		0x03090102, 0x03090102, 0x03090102, 0x03010902, 0x03010902,
		0x03010902, 0x03010902, 0x04090001, 0x04090001, 0x04000901,
		0x04000901, 0x04080402, 0x04080402, 0x04040802, 0x04040802,
		0x04020702, 0x04020702, 0x05060402, 0x05040602, 0x03080202,
		0x03080202, 0x03080202, 0x03080202, 0x03020802, 0x03020802,
		0x03020802, 0x03020802, 0x03080102, 0x03080102, 0x03080102,
		0x03080102, 0xFF000004, 0x04070302, 0x04070202, 0x03070102,
		0x03070102, 0x03010702, 0x03010702, 0x04050502, 0x04070001,
		0x04000701, 0x04060302, 0x04030602, 0x04050402, 0x04040502,
		0x04060202, 0x04020602, 0x04050302, 0xFF000003, 0x02010802,
		0x02010802, 0x03080001, 0x03000801, 0x03060102, 0x03010602,
		0x03060001, 0x03000601, 0xFF000004, 0x04030502, 0x04040402,
		0x03050202, 0x03050202, 0x03020502, 0x03020502, 0x03050001,
		0x03050001, 0x02050102, 0x02050102, 0x02050102, 0x02050102,
		0x02010502, 0x02010502, 0x02010502, 0x02010502, 0xFF000003,
		0x03040302, 0x03030402, 0x03000501, 0x03040202, 0x03020402,
		0x03030302, 0x02040102, 0x02040102, 0xFF000002, 0x01010402,
		0x01010402, 0x02040001, 0x02000401, 0xFF000002, 0x02030202,
		0x02020302, 0x01030102, 0x01030102, 0xFF000001, 0x01010302,
		0x01030001, 0xFF000001, 0x01000301, 0x01020202, 0xFF000003,
		0x00000082, 0x0000008B, 0x0000008E, 0x00000091, 0x00000094,
		0x00000097, 0x030C0E02, 0x030D0D02, 0xFF000003, 0x00000093,
		0x030E0B02, 0x030B0E02, 0x030F0902, 0x03090F02, 0x030A0E02,
		0x030D0B02, 0x030B0D02, 0xFF000003, 0x030F0802, 0x03080F02,
		0x030C0C02, 0x0000008D, 0x030E0802, 0x00000090, 0x02070F02,
		0x02070F02, 0xFF000003, 0x020A0D02, 0x020A0D02, 0x030D0A02,
		0x030C0B02, 0x030B0C02, 0x03060F02, 0x020F0602, 0x020F0602,
		0xFF000002, 0x02080E02, 0x020F0502, 0x020D0902, 0x02090D02,
		0xFF000002, 0x02050F02, 0x02070E02, 0x020C0A02, 0x020B0B02,
		0xFF000003, 0x020F0402, 0x020F0402, 0x02040F02, 0x02040F02,
		0x030A0C02, 0x03060E02, 0x02030F02, 0x02030F02, 0xFF000002,
		0x010F0302, 0x010F0302, 0x020D0802, 0x02080D02, 0xFF000001,
		0x010F0202, 0x01020F02, 0xFF000002, 0x020E0602, 0x020C0902,
		0x010F0001, 0x010F0001, 0xFF000002, 0x02090C02, 0x020E0502,
		0x010B0A02, 0x010B0A02, 0xFF000002, 0x020D0702, 0x02070D02,
		0x010E0402, 0x010E0402, 0xFF000002, 0x02080C02, 0x02060D02,
		0x010E0302, 0x010E0302, 0xFF000002, 0x01090B02, 0x01090B02,
		0x020B0902, 0x020A0A02, 0xFF000001, 0x010A0B02, 0x01050E02,
		0xFF000001, 0x01040E02, 0x010C0802, 0xFF000001, 0x010D0602,
		0x01030E02, 0xFF000001, 0x010E0202, 0x010E0001, 0xFF000001,
		0x01000E01, 0x010D0502, 0xFF000001, 0x01050D02, 0x010C0702,
		0xFF000001, 0x01070C02, 0x010D0402, 0xFF000001, 0x010B0802,
		0x01080B02, 0xFF000001, 0x01040D02, 0x010A0902, 0xFF000001,
		0x01090A02, 0x010C0602, 0xFF000001, 0x01030D02, 0x010B0702,
		0xFF000001, 0x010C0502, 0x01050C02, 0xFF000001, 0x01090902,
		0x010A0702, 0xFF000001, 0x01070A02, 0x01070902, 0xFF000003,
		0x00000023, 0x030D0F02, 0x020D0E02, 0x020D0E02, 0x010F0F02,
		0x010F0F02, 0x010F0F02, 0x010F0F02, 0xFF000001, 0x010F0E02,
		0x010F0D02, 0xFF000001, 0x010E0E02, 0x010F0C02, 0xFF000001,
		0x010E0D02, 0x010F0B02, 0xFF000001, 0x010B0F02, 0x010E0C02,
		0xFF000002, 0x010C0D02, 0x010C0D02, 0x020F0A02, 0x02090E02,
		0xFF000001, 0x010A0F02, 0x010D0C02, 0xFF000001, 0x010E0A02,
		0x010E0902, 0xFF000001, 0x010F0702, 0x010E0702, 0xFF000001,
		0x010E0F02, 0x010C0F02};

	static HUFF_ELEMENT vHT15[] = {
		0xFF000008, 0x00000101, 0x00000122, 0x00000143, 0x00000154,
		0x00000165, 0x00000176, 0x0000017F, 0x00000188, 0x00000199,
		0x000001A2, 0x000001AB, 0x000001B4, 0x000001BD, 0x000001C2,
		0x000001CB, 0x000001D4, 0x000001D9, 0x000001DE, 0x000001E3,
		0x000001E8, 0x000001ED, 0x000001F2, 0x000001F7, 0x000001FC,
		0x00000201, 0x00000204, 0x00000207, 0x0000020A, 0x0000020F,
		0x00000212, 0x00000215, 0x0000021A, 0x0000021D, 0x00000220,
		0x08010902, 0x00000223, 0x00000226, 0x00000229, 0x0000022C,
		0x0000022F, 0x08080202, 0x08020802, 0x08080102, 0x08010802,
		0x00000232, 0x00000235, 0x00000238, 0x0000023B, 0x08070202,
		0x08020702, 0x08040602, 0x08070102, 0x08050502, 0x08010702,
		0x0000023E, 0x08060302, 0x08030602, 0x08050402, 0x08040502,
		0x08060202, 0x08020602, 0x08060102, 0x00000241, 0x08050302,
		0x07010602, 0x07010602, 0x08030502, 0x08040402, 0x07050202,
		0x07050202, 0x07020502, 0x07020502, 0x07050102, 0x07050102,
		0x07010502, 0x07010502, 0x08050001, 0x08000501, 0x07040302,
		0x07040302, 0x07030402, 0x07030402, 0x07040202, 0x07040202,
		0x07020402, 0x07020402, 0x07030302, 0x07030302, 0x06010402,
		0x06010402, 0x06010402, 0x06010402, 0x07040102, 0x07040102,
		0x07040001, 0x07040001, 0x06030202, 0x06030202, 0x06030202,
		0x06030202, 0x06020302, 0x06020302, 0x06020302, 0x06020302,
		0x07000401, 0x07000401, 0x07030001, 0x07030001, 0x06030102,
		0x06030102, 0x06030102, 0x06030102, 0x06010302, 0x06010302,
		0x06010302, 0x06010302, 0x06000301, 0x06000301, 0x06000301,
		0x06000301, 0x05020202, 0x05020202, 0x05020202, 0x05020202,
		0x05020202, 0x05020202, 0x05020202, 0x05020202, 0x05020102,
		0x05020102, 0x05020102, 0x05020102, 0x05020102, 0x05020102,
		0x05020102, 0x05020102, 0x05010202, 0x05010202, 0x05010202,
		0x05010202, 0x05010202, 0x05010202, 0x05010202, 0x05010202,
		0x05020001, 0x05020001, 0x05020001, 0x05020001, 0x05020001,
		0x05020001, 0x05020001, 0x05020001, 0x05000201, 0x05000201,
		0x05000201, 0x05000201, 0x05000201, 0x05000201, 0x05000201,
		0x05000201, 0x03010102, 0x03010102, 0x03010102, 0x03010102,
		0x03010102, 0x03010102, 0x03010102, 0x03010102, 0x03010102,
		0x03010102, 0x03010102, 0x03010102, 0x03010102, 0x03010102,
		0x03010102, 0x03010102, 0x03010102, 0x03010102, 0x03010102,
		0x03010102, 0x03010102, 0x03010102, 0x03010102, 0x03010102,
		0x03010102, 0x03010102, 0x03010102, 0x03010102, 0x03010102,
		0x03010102, 0x03010102, 0x03010102, 0x04010001, 0x04010001,
		0x04010001, 0x04010001, 0x04010001, 0x04010001, 0x04010001,
		0x04010001, 0x04010001, 0x04010001, 0x04010001, 0x04010001,
		0x04010001, 0x04010001, 0x04010001, 0x04010001, 0x04000101,
		0x04000101, 0x04000101, 0x04000101, 0x04000101, 0x04000101,
		0x04000101, 0x04000101, 0x04000101, 0x04000101, 0x04000101,
		0x04000101, 0x04000101, 0x04000101, 0x04000101, 0x04000101,
		0x03000000, 0x03000000, 0x03000000, 0x03000000, 0x03000000,
		0x03000000, 0x03000000, 0x03000000, 0x03000000, 0x03000000,
		0x03000000, 0x03000000, 0x03000000, 0x03000000, 0x03000000,
		0x03000000, 0x03000000, 0x03000000, 0x03000000, 0x03000000,
		0x03000000, 0x03000000, 0x03000000, 0x03000000, 0x03000000,
		0x03000000, 0x03000000, 0x03000000, 0x03000000, 0x03000000,
		0x03000000, 0x03000000, 0xFF000005, 0x050F0F02, 0x050F0E02,
		0x050E0F02, 0x050F0D02, 0x040E0E02, 0x040E0E02, 0x050D0F02,
		0x050F0C02, 0x050C0F02, 0x050E0D02, 0x050D0E02, 0x050F0B02,
		0x040B0F02, 0x040B0F02, 0x050E0C02, 0x050C0E02, 0x040D0D02,
		0x040D0D02, 0x040F0A02, 0x040F0A02, 0x040A0F02, 0x040A0F02,
		0x040E0B02, 0x040E0B02, 0x040B0E02, 0x040B0E02, 0x040D0C02,
		0x040D0C02, 0x040C0D02, 0x040C0D02, 0x040F0902, 0x040F0902,
		0xFF000005, 0x04090F02, 0x04090F02, 0x040A0E02, 0x040A0E02,
		0x040D0B02, 0x040D0B02, 0x040B0D02, 0x040B0D02, 0x040F0802,
		0x040F0802, 0x04080F02, 0x04080F02, 0x040C0C02, 0x040C0C02,
		0x040E0902, 0x040E0902, 0x04090E02, 0x04090E02, 0x040F0702,
		0x040F0702, 0x04070F02, 0x04070F02, 0x040D0A02, 0x040D0A02,
		0x040A0D02, 0x040A0D02, 0x040C0B02, 0x040C0B02, 0x040F0602,
		0x040F0602, 0x050E0A02, 0x050F0001, 0xFF000004, 0x030B0C02,
		0x030B0C02, 0x03060F02, 0x03060F02, 0x040E0802, 0x04080E02,
		0x040F0502, 0x040D0902, 0x03050F02, 0x03050F02, 0x030E0702,
		0x030E0702, 0x03070E02, 0x03070E02, 0x030C0A02, 0x030C0A02,
		0xFF000004, 0x030A0C02, 0x030A0C02, 0x030B0B02, 0x030B0B02,
		0x04090D02, 0x040D0802, 0x030F0402, 0x030F0402, 0x03040F02,
		0x03040F02, 0x030F0302, 0x030F0302, 0x03030F02, 0x03030F02,
		0x03080D02, 0x03080D02, 0xFF000004, 0x03060E02, 0x03060E02,
		0x030F0202, 0x030F0202, 0x03020F02, 0x03020F02, 0x040E0602,
		0x04000F01, 0x030F0102, 0x030F0102, 0x03010F02, 0x03010F02,
		0x030C0902, 0x030C0902, 0x03090C02, 0x03090C02, 0xFF000003,
		0x030E0502, 0x030B0A02, 0x030A0B02, 0x03050E02, 0x030D0702,
		0x03070D02, 0x030E0402, 0x03040E02, 0xFF000003, 0x030C0802,
		0x03080C02, 0x030E0302, 0x030D0602, 0x03060D02, 0x03030E02,
		0x030B0902, 0x03090B02, 0xFF000004, 0x030E0202, 0x030E0202,
		0x030A0A02, 0x030A0A02, 0x03020E02, 0x03020E02, 0x030E0102,
		0x030E0102, 0x03010E02, 0x03010E02, 0x040E0001, 0x04000E01,
		0x030D0502, 0x030D0502, 0x03050D02, 0x03050D02, 0xFF000003,
		0x030C0702, 0x03070C02, 0x030D0402, 0x030B0802, 0x02040D02,
		0x02040D02, 0x03080B02, 0x030A0902, 0xFF000003, 0x03090A02,
		0x030C0602, 0x03060C02, 0x030D0302, 0x02030D02, 0x02030D02,
		0x02020D02, 0x02020D02, 0xFF000003, 0x030D0202, 0x030D0001,
		0x020D0102, 0x020D0102, 0x020B0702, 0x020B0702, 0x02070B02,
		0x02070B02, 0xFF000003, 0x02010D02, 0x02010D02, 0x030C0502,
		0x03000D01, 0x02050C02, 0x02050C02, 0x020A0802, 0x020A0802,
		0xFF000002, 0x02080A02, 0x020C0402, 0x02040C02, 0x020B0602,
		0xFF000003, 0x02060B02, 0x02060B02, 0x03090902, 0x030C0001,
		0x020C0302, 0x020C0302, 0x02030C02, 0x02030C02, 0xFF000003,
		0x020A0702, 0x020A0702, 0x02070A02, 0x02070A02, 0x02060A02,
		0x02060A02, 0x03000C01, 0x030B0001, 0xFF000002, 0x01020C02,
		0x01020C02, 0x020C0202, 0x020B0502, 0xFF000002, 0x02050B02,
		0x020C0102, 0x02090802, 0x02080902, 0xFF000002, 0x02010C02,
		0x020B0402, 0x02040B02, 0x020A0602, 0xFF000002, 0x020B0302,
		0x02090702, 0x01030B02, 0x01030B02, 0xFF000002, 0x02070902,
		0x02080802, 0x020B0202, 0x020A0502, 0xFF000002, 0x01020B02,
		0x01020B02, 0x02050A02, 0x020B0102, 0xFF000002, 0x01010B02,
		0x01010B02, 0x02000B01, 0x02090602, 0xFF000002, 0x02060902,
		0x020A0402, 0x02040A02, 0x02080702, 0xFF000002, 0x02070802,
		0x020A0302, 0x01030A02, 0x01030A02, 0xFF000001, 0x01090502,
		0x01050902, 0xFF000001, 0x010A0202, 0x01020A02, 0xFF000001,
		0x010A0102, 0x01010A02, 0xFF000002, 0x020A0001, 0x02000A01,
		0x01080602, 0x01080602, 0xFF000001, 0x01060802, 0x01090402,
		0xFF000001, 0x01040902, 0x01090302, 0xFF000002, 0x01030902,
		0x01030902, 0x02070702, 0x02090001, 0xFF000001, 0x01080502,
		0x01050802, 0xFF000001, 0x01090202, 0x01070602, 0xFF000001,
		0x01060702, 0x01020902, 0xFF000001, 0x01090102, 0x01000901,
		0xFF000001, 0x01080402, 0x01040802, 0xFF000001, 0x01070502,
		0x01050702, 0xFF000001, 0x01080302, 0x01030802, 0xFF000001,
		0x01060602, 0x01070402, 0xFF000001, 0x01040702, 0x01080001,
		0xFF000001, 0x01000801, 0x01060502, 0xFF000001, 0x01050602,
		0x01070302, 0xFF000001, 0x01030702, 0x01060402, 0xFF000001,
		0x01070001, 0x01000701, 0xFF000001, 0x01060001, 0x01000601};

	static HUFF_ELEMENT vHT16[] = {
		0xFF000008, 0x00000101, 0x0000010A, 0x00000113, 0x080F0F02,
		0x00000118, 0x0000011D, 0x00000120, 0x08020F02, 0x00000131,
		0x080F0102, 0x08010F02, 0x00000134, 0x00000145, 0x00000156,
		0x00000167, 0x00000178, 0x00000189, 0x0000019A, 0x000001A3,
		0x000001AC, 0x000001B5, 0x000001BE, 0x000001C7, 0x000001D0,
		0x000001D9, 0x000001DE, 0x000001E3, 0x000001E6, 0x000001EB,
		0x000001F0, 0x08010502, 0x000001F3, 0x000001F6, 0x000001F9,
		0x000001FC, 0x08040102, 0x08010402, 0x000001FF, 0x08030202,
		0x08020302, 0x07030102, 0x07030102, 0x07010302, 0x07010302,
		0x08030001, 0x08000301, 0x07020202, 0x07020202, 0x06020102,
		0x06020102, 0x06020102, 0x06020102, 0x06010202, 0x06010202,
		0x06010202, 0x06010202, 0x06020001, 0x06020001, 0x06020001,
		0x06020001, 0x06000201, 0x06000201, 0x06000201, 0x06000201,
		0x04010102, 0x04010102, 0x04010102, 0x04010102, 0x04010102,
		0x04010102, 0x04010102, 0x04010102, 0x04010102, 0x04010102,
		0x04010102, 0x04010102, 0x04010102, 0x04010102, 0x04010102,
		0x04010102, 0x04010001, 0x04010001, 0x04010001, 0x04010001,
		0x04010001, 0x04010001, 0x04010001, 0x04010001, 0x04010001,
		0x04010001, 0x04010001, 0x04010001, 0x04010001, 0x04010001,
		0x04010001, 0x04010001, 0x03000101, 0x03000101, 0x03000101,
		0x03000101, 0x03000101, 0x03000101, 0x03000101, 0x03000101,
		0x03000101, 0x03000101, 0x03000101, 0x03000101, 0x03000101,
		0x03000101, 0x03000101, 0x03000101, 0x03000101, 0x03000101,
		0x03000101, 0x03000101, 0x03000101, 0x03000101, 0x03000101,
		0x03000101, 0x03000101, 0x03000101, 0x03000101, 0x03000101,
		0x03000101, 0x03000101, 0x03000101, 0x03000101, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000,
		0x01000000, 0x01000000, 0xFF000003, 0x030F0E02, 0x030E0F02,
		0x030F0D02, 0x030D0F02, 0x030F0C02, 0x030C0F02, 0x030F0B02,
		0x030B0F02, 0xFF000003, 0x020F0A02, 0x020F0A02, 0x030A0F02,
		0x030F0902, 0x03090F02, 0x03080F02, 0x020F0802, 0x020F0802,
		0xFF000002, 0x020F0702, 0x02070F02, 0x020F0602, 0x02060F02,
		0xFF000002, 0x020F0502, 0x02050F02, 0x010F0402, 0x010F0402,
		0xFF000001, 0x01040F02, 0x01030F02, 0xFF000004, 0x01000F01,
		0x01000F01, 0x01000F01, 0x01000F01, 0x01000F01, 0x01000F01,
		0x01000F01, 0x01000F01, 0x020F0302, 0x020F0302, 0x020F0302,
		0x020F0302, 0x000000E2, 0x000000F3, 0x000000FC, 0x00000105,
		0xFF000001, 0x010F0202, 0x010F0001, 0xFF000004, 0x000000FA,
		0x000000FF, 0x00000104, 0x00000109, 0x0000010C, 0x00000111,
		0x00000116, 0x00000119, 0x0000011E, 0x00000123, 0x00000128,
		0x04030E02, 0x0000012D, 0x00000130, 0x00000133, 0x00000136,
		0xFF000004, 0x00000128, 0x0000012B, 0x0000012E, 0x040D0001,
		0x00000131, 0x00000134, 0x00000137, 0x040C0302, 0x0000013A,
		0x040C0102, 0x04000C01, 0x0000013D, 0x03020E02, 0x03020E02,
		0x040E0202, 0x040E0102, 0xFF000004, 0x04030D02, 0x040D0202,
		0x04020D02, 0x04010D02, 0x040B0302, 0x0000012F, 0x030D0102,
		0x030D0102, 0x04040C02, 0x040B0602, 0x04030C02, 0x04070A02,
		0x030C0202, 0x030C0202, 0x04020C02, 0x04050B02, 0xFF000004,
		0x04010C02, 0x040C0001, 0x040B0402, 0x04040B02, 0x040A0602,
		0x04060A02, 0x03030B02, 0x03030B02, 0x040A0502, 0x04050A02,
		0x030B0202, 0x030B0202, 0x03020B02, 0x03020B02, 0x030B0102,
		0x030B0102, 0xFF000004, 0x03010B02, 0x03010B02, 0x040B0001,
		0x04000B01, 0x04090602, 0x04060902, 0x040A0402, 0x04040A02,
		0x04080702, 0x04070802, 0x03030A02, 0x03030A02, 0x040A0302,
		0x04090502, 0x030A0202, 0x030A0202, 0xFF000004, 0x04050902,
		0x04080602, 0x03010A02, 0x03010A02, 0x04060802, 0x04070702,
		0x03040902, 0x03040902, 0x04090402, 0x04070502, 0x03070602,
		0x03070602, 0x02020A02, 0x02020A02, 0x02020A02, 0x02020A02,
		0xFF000003, 0x020A0102, 0x020A0102, 0x030A0001, 0x03000A01,
		0x03090302, 0x03030902, 0x03080502, 0x03050802, 0xFF000003,
		0x02090202, 0x02090202, 0x02020902, 0x02020902, 0x03060702,
		0x03090001, 0x02090102, 0x02090102, 0xFF000003, 0x02010902,
		0x02010902, 0x03000901, 0x03080402, 0x03040802, 0x03050702,
		0x03080302, 0x03030802, 0xFF000003, 0x03060602, 0x03080202,
		0x02020802, 0x02020802, 0x03070402, 0x03040702, 0x02080102,
		0x02080102, 0xFF000003, 0x02010802, 0x02010802, 0x02000801,
		0x02000801, 0x03080001, 0x03060502, 0x02070302, 0x02070302,
		0xFF000003, 0x02030702, 0x02030702, 0x03050602, 0x03060402,
		0x02070202, 0x02070202, 0x02020702, 0x02020702, 0xFF000003,
		0x03040602, 0x03050502, 0x02070001, 0x02070001, 0x01070102,
		0x01070102, 0x01070102, 0x01070102, 0xFF000002, 0x01010702,
		0x01010702, 0x02000701, 0x02060302, 0xFF000002, 0x02030602,
		0x02050402, 0x02040502, 0x02060202, 0xFF000001, 0x01020602,
		0x01060102, 0xFF000002, 0x01010602, 0x01010602, 0x02060001,
		0x02000601, 0xFF000002, 0x01030502, 0x01030502, 0x02050302,
		0x02040402, 0xFF000001, 0x01050202, 0x01020502, 0xFF000001,
		0x01050102, 0x01050001, 0xFF000001, 0x01040302, 0x01030402,
		0xFF000001, 0x01000501, 0x01040202, 0xFF000001, 0x01020402,
		0x01030302, 0xFF000001, 0x01040001, 0x01000401, 0xFF000004,
		0x040E0C02, 0x00000086, 0x030E0D02, 0x030E0D02, 0x03090E02,
		0x03090E02, 0x040A0E02, 0x04090D02, 0x020E0E02, 0x020E0E02,
		0x020E0E02, 0x020E0E02, 0x030D0E02, 0x030D0E02, 0x030B0E02,
		0x030B0E02, 0xFF000003, 0x020E0B02, 0x020E0B02, 0x020D0C02,
		0x020D0C02, 0x030C0D02, 0x030B0D02, 0x020E0A02, 0x020E0A02,
		0xFF000003, 0x020C0C02, 0x020C0C02, 0x030D0A02, 0x030A0D02,
		0x030E0702, 0x030C0A02, 0x020A0C02, 0x020A0C02, 0xFF000003,
		0x03090C02, 0x030D0702, 0x020E0502, 0x020E0502, 0x010D0B02,
		0x010D0B02, 0x010D0B02, 0x010D0B02, 0xFF000002, 0x010E0902,
		0x010E0902, 0x020C0B02, 0x020B0C02, 0xFF000002, 0x020E0802,
		0x02080E02, 0x020D0902, 0x02070E02, 0xFF000002, 0x020B0B02,
		0x020D0802, 0x02080D02, 0x020E0602, 0xFF000001, 0x01060E02,
		0x010C0902, 0xFF000002, 0x020B0A02, 0x020A0B02, 0x02050E02,
		0x02070D02, 0xFF000002, 0x010E0402, 0x010E0402, 0x02040E02,
		0x020C0802, 0xFF000001, 0x01080C02, 0x010E0302, 0xFF000002,
		0x010D0602, 0x010D0602, 0x02060D02, 0x020B0902, 0xFF000002,
		0x02090B02, 0x020A0A02, 0x01010E02, 0x01010E02, 0xFF000002,
		0x01040D02, 0x01040D02, 0x02080B02, 0x02090A02, 0xFF000002,
		0x010B0702, 0x010B0702, 0x02070B02, 0x02000D01, 0xFF000001,
		0x010E0001, 0x01000E01, 0xFF000001, 0x010D0502, 0x01050D02,
		0xFF000001, 0x010C0702, 0x01070C02, 0xFF000001, 0x010D0402,
		0x010B0802, 0xFF000001, 0x010A0902, 0x010C0602, 0xFF000001,
		0x01060C02, 0x010D0302, 0xFF000001, 0x010C0502, 0x01050C02,
		0xFF000001, 0x010A0802, 0x01080A02, 0xFF000001, 0x01090902,
		0x010C0402, 0xFF000001, 0x01060B02, 0x010A0702, 0xFF000001,
		0x010B0502, 0x01090802, 0xFF000001, 0x01080902, 0x01090702,
		0xFF000001, 0x01070902, 0x01080802, 0xFF000001, 0x010C0E02,
		0x010D0D02};

	static HUFF_ELEMENT vHT24[] = {
		0xFF000009, 0x080F0E02, 0x080F0E02, 0x080E0F02, 0x080E0F02,
		0x080F0D02, 0x080F0D02, 0x080D0F02, 0x080D0F02, 0x080F0C02,
		0x080F0C02, 0x080C0F02, 0x080C0F02, 0x080F0B02, 0x080F0B02,
		0x080B0F02, 0x080B0F02, 0x070A0F02, 0x070A0F02, 0x070A0F02,
		0x070A0F02, 0x080F0A02, 0x080F0A02, 0x080F0902, 0x080F0902,
		0x07090F02, 0x07090F02, 0x07090F02, 0x07090F02, 0x07080F02,
		0x07080F02, 0x07080F02, 0x07080F02, 0x080F0802, 0x080F0802,
		0x080F0702, 0x080F0702, 0x07070F02, 0x07070F02, 0x07070F02,
		0x07070F02, 0x070F0602, 0x070F0602, 0x070F0602, 0x070F0602,
		0x07060F02, 0x07060F02, 0x07060F02, 0x07060F02, 0x070F0502,
		0x070F0502, 0x070F0502, 0x070F0502, 0x07050F02, 0x07050F02,
		0x07050F02, 0x07050F02, 0x070F0402, 0x070F0402, 0x070F0402,
		0x070F0402, 0x07040F02, 0x07040F02, 0x07040F02, 0x07040F02,
		0x070F0302, 0x070F0302, 0x070F0302, 0x070F0302, 0x07030F02,
		0x07030F02, 0x07030F02, 0x07030F02, 0x070F0202, 0x070F0202,
		0x070F0202, 0x070F0202, 0x07020F02, 0x07020F02, 0x07020F02,
		0x07020F02, 0x07010F02, 0x07010F02, 0x07010F02, 0x07010F02,
		0x080F0102, 0x080F0102, 0x08000F01, 0x08000F01, 0x090F0001,
		0x00000201, 0x00000206, 0x0000020B, 0x00000210, 0x00000215,
		0x0000021A, 0x0000021F, 0x040F0F02, 0x040F0F02, 0x040F0F02,
		0x040F0F02, 0x040F0F02, 0x040F0F02, 0x040F0F02, 0x040F0F02,
		0x040F0F02, 0x040F0F02, 0x040F0F02, 0x040F0F02, 0x040F0F02,
		0x040F0F02, 0x040F0F02, 0x040F0F02, 0x040F0F02, 0x040F0F02,
		0x040F0F02, 0x040F0F02, 0x040F0F02, 0x040F0F02, 0x040F0F02,
		0x040F0F02, 0x040F0F02, 0x040F0F02, 0x040F0F02, 0x040F0F02,
		0x040F0F02, 0x040F0F02, 0x040F0F02, 0x040F0F02, 0x00000224,
		0x00000229, 0x00000232, 0x00000237, 0x0000023A, 0x0000023F,
		0x00000242, 0x00000245, 0x0000024A, 0x0000024D, 0x00000250,
		0x00000253, 0x00000256, 0x00000259, 0x0000025C, 0x0000025F,
		0x00000262, 0x00000265, 0x00000268, 0x0000026B, 0x0000026E,
		0x00000271, 0x00000274, 0x00000277, 0x0000027A, 0x0000027D,
		0x00000280, 0x00000283, 0x00000288, 0x0000028B, 0x0000028E,
		0x00000291, 0x00000294, 0x00000297, 0x0000029A, 0x0000029F,
		0x09040B02, 0x000002A4, 0x000002A7, 0x000002AA, 0x09030B02,
		0x09080802, 0x000002AF, 0x09020B02, 0x000002B2, 0x000002B5,
		0x09060902, 0x09040A02, 0x000002B8, 0x09070802, 0x090A0302,
		0x09030A02, 0x09090502, 0x09050902, 0x090A0202, 0x09020A02,
		0x09010A02, 0x09080602, 0x09060802, 0x09070702, 0x09090402,
		0x09040902, 0x09090302, 0x09030902, 0x09080502, 0x09050802,
		0x09090202, 0x09070602, 0x09060702, 0x09020902, 0x09090102,
		0x09010902, 0x09080402, 0x09040802, 0x09070502, 0x09050702,
		0x09080302, 0x09030802, 0x09060602, 0x09080202, 0x09020802,
		0x09080102, 0x09070402, 0x09040702, 0x09010802, 0x000002BB,
		0x09060502, 0x09050602, 0x09070102, 0x000002BE, 0x08030702,
		0x08030702, 0x09070302, 0x09070202, 0x08020702, 0x08020702,
		0x08060402, 0x08060402, 0x08040602, 0x08040602, 0x08050502,
		0x08050502, 0x08010702, 0x08010702, 0x08060302, 0x08060302,
		0x08030602, 0x08030602, 0x08050402, 0x08050402, 0x08040502,
		0x08040502, 0x08060202, 0x08060202, 0x08020602, 0x08020602,
		0x08060102, 0x08060102, 0x08010602, 0x08010602, 0x09060001,
		0x09000601, 0x08050302, 0x08050302, 0x08030502, 0x08030502,
		0x08040402, 0x08040402, 0x08050202, 0x08050202, 0x08020502,
		0x08020502, 0x08050102, 0x08050102, 0x09050001, 0x09000501,
		0x07010502, 0x07010502, 0x07010502, 0x07010502, 0x08040302,
		0x08040302, 0x08030402, 0x08030402, 0x07040202, 0x07040202,
		0x07040202, 0x07040202, 0x07020402, 0x07020402, 0x07020402,
		0x07020402, 0x07030302, 0x07030302, 0x07030302, 0x07030302,
		0x07040102, 0x07040102, 0x07040102, 0x07040102, 0x07010402,
		0x07010402, 0x07010402, 0x07010402, 0x08040001, 0x08040001,
		0x08000401, 0x08000401, 0x07030202, 0x07030202, 0x07030202,
		0x07030202, 0x07020302, 0x07020302, 0x07020302, 0x07020302,
		0x06030102, 0x06030102, 0x06030102, 0x06030102, 0x06030102,
		0x06030102, 0x06030102, 0x06030102, 0x06010302, 0x06010302,
		0x06010302, 0x06010302, 0x06010302, 0x06010302, 0x06010302,
		0x06010302, 0x07030001, 0x07030001, 0x07030001, 0x07030001,
		0x07000301, 0x07000301, 0x07000301, 0x07000301, 0x06020202,
		0x06020202, 0x06020202, 0x06020202, 0x06020202, 0x06020202,
		0x06020202, 0x06020202, 0x05020102, 0x05020102, 0x05020102,
		0x05020102, 0x05020102, 0x05020102, 0x05020102, 0x05020102,
		0x05020102, 0x05020102, 0x05020102, 0x05020102, 0x05020102,
		0x05020102, 0x05020102, 0x05020102, 0x05010202, 0x05010202,
		0x05010202, 0x05010202, 0x05010202, 0x05010202, 0x05010202,
		0x05010202, 0x05010202, 0x05010202, 0x05010202, 0x05010202,
		0x05010202, 0x05010202, 0x05010202, 0x05010202, 0x06020001,
		0x06020001, 0x06020001, 0x06020001, 0x06020001, 0x06020001,
		0x06020001, 0x06020001, 0x06000201, 0x06000201, 0x06000201,
		0x06000201, 0x06000201, 0x06000201, 0x06000201, 0x06000201,
		0x04010102, 0x04010102, 0x04010102, 0x04010102, 0x04010102,
		0x04010102, 0x04010102, 0x04010102, 0x04010102, 0x04010102,
		0x04010102, 0x04010102, 0x04010102, 0x04010102, 0x04010102,
		0x04010102, 0x04010102, 0x04010102, 0x04010102, 0x04010102,
		0x04010102, 0x04010102, 0x04010102, 0x04010102, 0x04010102,
		0x04010102, 0x04010102, 0x04010102, 0x04010102, 0x04010102,
		0x04010102, 0x04010102, 0x04010001, 0x04010001, 0x04010001,
		0x04010001, 0x04010001, 0x04010001, 0x04010001, 0x04010001,
		0x04010001, 0x04010001, 0x04010001, 0x04010001, 0x04010001,
		0x04010001, 0x04010001, 0x04010001, 0x04010001, 0x04010001,
		0x04010001, 0x04010001, 0x04010001, 0x04010001, 0x04010001,
		0x04010001, 0x04010001, 0x04010001, 0x04010001, 0x04010001,
		0x04010001, 0x04010001, 0x04010001, 0x04010001, 0x04000101,
		0x04000101, 0x04000101, 0x04000101, 0x04000101, 0x04000101,
		0x04000101, 0x04000101, 0x04000101, 0x04000101, 0x04000101,
		0x04000101, 0x04000101, 0x04000101, 0x04000101, 0x04000101,
		0x04000101, 0x04000101, 0x04000101, 0x04000101, 0x04000101,
		0x04000101, 0x04000101, 0x04000101, 0x04000101, 0x04000101,
		0x04000101, 0x04000101, 0x04000101, 0x04000101, 0x04000101,
		0x04000101, 0x04000000, 0x04000000, 0x04000000, 0x04000000,
		0x04000000, 0x04000000, 0x04000000, 0x04000000, 0x04000000,
		0x04000000, 0x04000000, 0x04000000, 0x04000000, 0x04000000,
		0x04000000, 0x04000000, 0x04000000, 0x04000000, 0x04000000,
		0x04000000, 0x04000000, 0x04000000, 0x04000000, 0x04000000,
		0x04000000, 0x04000000, 0x04000000, 0x04000000, 0x04000000,
		0x04000000, 0x04000000, 0x04000000, 0xFF000002, 0x020E0E02,
		0x020E0D02, 0x020D0E02, 0x020E0C02, 0xFF000002, 0x020C0E02,
		0x020D0D02, 0x020E0B02, 0x020B0E02, 0xFF000002, 0x020D0C02,
		0x020C0D02, 0x020E0A02, 0x020A0E02, 0xFF000002, 0x020D0B02,
		0x020B0D02, 0x020C0C02, 0x020E0902, 0xFF000002, 0x02090E02,
		0x020D0A02, 0x020A0D02, 0x020C0B02, 0xFF000002, 0x020B0C02,
		0x020E0802, 0x02080E02, 0x020D0902, 0xFF000002, 0x02090D02,
		0x020E0702, 0x02070E02, 0x020C0A02, 0xFF000002, 0x020A0C02,
		0x020B0B02, 0x020D0802, 0x02080D02, 0xFF000003, 0x030E0001,
		0x03000E01, 0x020D0001, 0x020D0001, 0x01060E02, 0x01060E02,
		0x01060E02, 0x01060E02, 0xFF000002, 0x020E0602, 0x020C0902,
		0x01090C02, 0x01090C02, 0xFF000001, 0x010E0502, 0x010A0B02,
		0xFF000002, 0x01050E02, 0x01050E02, 0x020B0A02, 0x020D0702,
		0xFF000001, 0x01070D02, 0x01040E02, 0xFF000001, 0x010C0802,
		0x01080C02, 0xFF000002, 0x020E0402, 0x020E0202, 0x010E0302,
		0x010E0302, 0xFF000001, 0x010D0602, 0x01060D02, 0xFF000001,
		0x01030E02, 0x010B0902, 0xFF000001, 0x01090B02, 0x010A0A02,
		0xFF000001, 0x01020E02, 0x010E0102, 0xFF000001, 0x01010E02,
		0x010D0502, 0xFF000001, 0x01050D02, 0x010C0702, 0xFF000001,
		0x01070C02, 0x010D0402, 0xFF000001, 0x010B0802, 0x01080B02,
		0xFF000001, 0x01040D02, 0x010A0902, 0xFF000001, 0x01090A02,
		0x010C0602, 0xFF000001, 0x01060C02, 0x010D0302, 0xFF000001,
		0x01030D02, 0x010D0202, 0xFF000001, 0x01020D02, 0x010D0102,
		0xFF000001, 0x010B0702, 0x01070B02, 0xFF000001, 0x01010D02,
		0x010C0502, 0xFF000001, 0x01050C02, 0x010A0802, 0xFF000001,
		0x01080A02, 0x01090902, 0xFF000001, 0x010C0402, 0x01040C02,
		0xFF000001, 0x010B0602, 0x01060B02, 0xFF000002, 0x02000D01,
		0x020C0001, 0x010C0302, 0x010C0302, 0xFF000001, 0x01030C02,
		0x010A0702, 0xFF000001, 0x01070A02, 0x010C0202, 0xFF000001,
		0x01020C02, 0x010B0502, 0xFF000001, 0x01050B02, 0x010C0102,
		0xFF000001, 0x01090802, 0x01080902, 0xFF000001, 0x01010C02,
		0x010B0402, 0xFF000002, 0x02000C01, 0x020B0001, 0x010B0302,
		0x010B0302, 0xFF000002, 0x02000B01, 0x020A0001, 0x010A0102,
		0x010A0102, 0xFF000001, 0x010A0602, 0x01060A02, 0xFF000001,
		0x01090702, 0x01070902, 0xFF000002, 0x02000A01, 0x02090001,
		0x01000901, 0x01000901, 0xFF000001, 0x010B0202, 0x010A0502,
		0xFF000001, 0x01050A02, 0x010B0102, 0xFF000001, 0x01010B02,
		0x01090602, 0xFF000001, 0x010A0402, 0x01080702, 0xFF000001,
		0x01080001, 0x01000801, 0xFF000001, 0x01070001, 0x01000701};

	static HUFF_SETUP vSETUP[] = {
		{ vHT0, 0, HUFFCASE_NOBIT},
		{ vHT1, 0, HUFFCASE_ONE},
		{ vHT2, 0, HUFFCASE_ONE},
		{ vHT3, 0, HUFFCASE_ONE},
		{ vHT0, 0, HUFFCASE_NOBIT},
		{ vHT5, 0, HUFFCASE_ONE},
		{ vHT6, 0, HUFFCASE_ONE},
		{ vHT7, 0, HUFFCASE_NOLIN},
		{ vHT8, 0, HUFFCASE_NOLIN},
		{ vHT9, 0, HUFFCASE_NOLIN},
		{ vHT10, 0, HUFFCASE_NOLIN},
		{ vHT11, 0, HUFFCASE_NOLIN},
		{ vHT12, 0, HUFFCASE_NOLIN},
		{ vHT13, 0, HUFFCASE_NOLIN},
		{ vHT0, 0, HUFFCASE_NOBIT},
		{ vHT15, 0, HUFFCASE_NOLIN},
		{ vHT16, 1, HUFFCASE_LIN},
		{ vHT16, 2, HUFFCASE_LIN},
		{ vHT16, 3, HUFFCASE_LIN},
		{ vHT16, 4, HUFFCASE_LIN},
		{ vHT16, 6, HUFFCASE_LIN},
		{ vHT16, 8, HUFFCASE_LIN},
		{ vHT16, 10, HUFFCASE_LIN},
		{ vHT16, 13, HUFFCASE_LIN},
		{ vHT24, 4, HUFFCASE_LIN},
		{ vHT24, 5, HUFFCASE_LIN},
		{ vHT24, 6, HUFFCASE_LIN},
		{ vHT24, 7, HUFFCASE_LIN},
		{ vHT24, 8, HUFFCASE_LIN},
		{ vHT24, 9, HUFFCASE_LIN},
		{ vHT24, 11, HUFFCASE_LIN},
		{ vHT24, 13, HUFFCASE_LIN},
		{ vHT0, 0, HUFFCASE_QUADA},
		{ vHT0, 0, HUFFCASE_QUADB}};

	if( nCount <= 0 )
		return;
	nCount >>= 1;

	switch(vSETUP[nTable].m_nCASE)
	{
	case HUFFCASE_NOLIN	:
		for( int i=0; i<nCount; i++)
		{
			LPHUFF_ELEMENT pHUFF = vSETUP[nTable].m_pTABLE;
			int nVALUE = pSTREAM->ReadBITs(pHUFF[0].m_vElement.m_bSIGN);

			while(!pHUFF[nVALUE + 1].m_vElement.m_bPURGE)
			{
				pSTREAM->SeekBITs(pHUFF[0].m_vElement.m_bSIGN);
				pHUFF += pHUFF[nVALUE + 1].m_nDATA;
				nVALUE = pSTREAM->ReadBITs(pHUFF[0].m_vElement.m_bSIGN);
			}

			pSTREAM->SeekBITs(pHUFF[nVALUE + 1].m_vElement.m_bPURGE);
			int nX = pHUFF[nVALUE + 1].m_vElement.m_bX;
			int nY = pHUFF[nVALUE + 1].m_vElement.m_bY;

			if( nX && pSTREAM->PopBITs(1) )
				nX = -nX;

			if( nY && pSTREAM->PopBITs(1) )
				nY = -nY;

			pVALUE[i * 2 + 1] = nY;
			pVALUE[i * 2] = nX;

			if(pSTREAM->IsOverRun())
				break;
		}

		break;

	case HUFFCASE_ONE	:
		for( int i=0; i<nCount; i++)
		{
			int nVALUE = pSTREAM->ReadBITs(vSETUP[nTable].m_pTABLE[0].m_vElement.m_bSIGN);

			pSTREAM->SeekBITs(vSETUP[nTable].m_pTABLE[nVALUE + 1].m_vElement.m_bPURGE);
			int nX = vSETUP[nTable].m_pTABLE[nVALUE + 1].m_vElement.m_bX;
			int nY = vSETUP[nTable].m_pTABLE[nVALUE + 1].m_vElement.m_bY;

			if( nX && pSTREAM->PopBITs(1) )
				nX = -nX;

			if( nY && pSTREAM->PopBITs(1) )
				nY = -nY;

			pVALUE[i * 2 + 1] = nY;
			pVALUE[i * 2] = nX;

			if(pSTREAM->IsOverRun())
				break;
		}

		break;

	case HUFFCASE_LIN	:
		for( int i=0; i<nCount; i++)
		{
			LPHUFF_ELEMENT pHUFF = vSETUP[nTable].m_pTABLE;
			int nVALUE = pSTREAM->ReadBITs(pHUFF[0].m_vElement.m_bSIGN);

			while(!pHUFF[nVALUE + 1].m_vElement.m_bPURGE)
			{
				pSTREAM->SeekBITs(pHUFF[0].m_vElement.m_bSIGN);
				pHUFF += pHUFF[nVALUE + 1].m_nDATA;
				nVALUE = pSTREAM->ReadBITs(pHUFF[0].m_vElement.m_bSIGN);
			}

			pSTREAM->SeekBITs(pHUFF[nVALUE + 1].m_vElement.m_bPURGE);
			int nX = pHUFF[nVALUE + 1].m_vElement.m_bX;
			int nY = pHUFF[nVALUE + 1].m_vElement.m_bY;

			if( nX == 15 )
				nX += INT(pSTREAM->PopBITs(BYTE(vSETUP[nTable].m_nBITS)));

			if( nX && pSTREAM->PopBITs(1) )
				nX = -nX;

			if( nY == 15 )
				nY += INT(pSTREAM->PopBITs(BYTE(vSETUP[nTable].m_nBITS)));

			if( nY && pSTREAM->PopBITs(1) )
				nY = -nY;

			pVALUE[i * 2 + 1] = nY;
			pVALUE[i * 2] = nX;

			if(pSTREAM->IsOverRun())
				break;
		}

		break;

	default				:
		for( int i=0; i<nCount; i++)
			for( int j=0; j<2; j++)
				pVALUE[i * 2 + j] = 0;

		break;
	}
}

void CT3DMusic::Antialias( FLOAT *pVALUE,
						   int nCount)
{
	for( int i=0; i<nCount; i++)
	{
		for( int j=0; j<8; j++)
		{
			FLOAT fA = pVALUE[17 - j];
			FLOAT fB = pVALUE[18 + j];

			pVALUE[17 - j] = fA * m_vCSA[j][0] - fB * m_vCSA[j][1];
			pVALUE[18 + j] = fB * m_vCSA[j][0] + fA * m_vCSA[j][1];
		}

		pVALUE += 18;
	}
}

void CT3DMusic::ProcessMS( FLOAT *pVALUE,
						   int nCount)
{
	for( int i=0; i<nCount; i++)
	{
		FLOAT fR = pVALUE[i] - pVALUE[i + 1152];
		FLOAT fL = pVALUE[i] + pVALUE[i + 1152];

		pVALUE[i + 1152] = fR;
		pVALUE[i] = fL;
	}
}

void CT3DMusic::ProcessIS1( FLOAT *pVALUE,
						    LPSCALE_FACTOR pSF,
							LPCB_INFO pCB,
							int nVersion,
							int nFREQ,
							int nMSMode,
							int nCount)
{
	int nIndex = GetBandIndex( nVersion, nFREQ, pCB[1].m_nType, pCB[1].m_nMax);
	nCount -= nIndex;

	if(pCB[1].m_nType)
	{
		for( int i = pCB[1].m_nMax + 1; i < 12; i++)
		{
			int nBand = m_vSFBT[nVersion][nFREQ].m_nS[i + 1] - m_vSFBT[nVersion][nFREQ].m_nS[i];

			FLOAT vL[3];
			FLOAT vR[3];

			for( int j=0; j<3; j++)
			{
				int nSF = pSF->m_nS[j][i];

				vL[j] = m_vISData1[nMSMode][nSF][0];
				vR[j] = m_vISData1[nMSMode][nSF][1];
			}

			for( j=0; j<nBand; j++)
			{
				nCount -= 3;

				if( nCount < 0 )
					return;

				for( int k=0; k<3; k++)
				{
					pVALUE[nIndex + 1152] = vR[k] * pVALUE[nIndex];
					pVALUE[nIndex] = vL[k] * pVALUE[nIndex];
					nIndex++;
				}
			}
		}
	}
	else
	{
		for( int i = pCB[1].m_nMax + 1; i < 21; i++)
		{
			int nBand = m_vSFBT[nVersion][nFREQ].m_nL[i + 1] - m_vSFBT[nVersion][nFREQ].m_nL[i];
			int nSF = pSF->m_nL[i];

			FLOAT fL = m_vISData1[nMSMode][nSF][0];
			FLOAT fR = m_vISData1[nMSMode][nSF][1];

			for( int j=0; j<nBand; j++)
			{
				nCount--;

				if( nCount < 0 )
					return;

				pVALUE[nIndex + 1152] = fR * pVALUE[nIndex];
				pVALUE[nIndex] = fL * pVALUE[nIndex];
				nIndex++;
			}
		}
	}
}

void CT3DMusic::ProcessIS2( FLOAT *pVALUE,
						    LPSCALE_FACTOR pSF,
							LPIS_SF_INFO pISSF,
							LPCB_INFO pCB,
							int nVersion,
							int nFREQ,
							int nMSMode,
							int nCount)
{
	FLOAT *pISDATA = m_vISData2[pISSF->m_nIScale][nMSMode][0];

	int nIndex = 0;
	int vL[21];

	for( int i=0; i<3; i++)
	{
		int nVALUE = (1 << pISSF->m_vSL[i]) - 1;

		for( int j=0; j<pISSF->m_vR[i]; j++)
		{
			vL[nIndex] = nVALUE;
			nIndex++;
		}
	}

	if(pCB[1].m_nType)
	{
		for( i=0; i<3; i++)
		{
			nIndex = GetBandIndex( nVersion, nFREQ, 1, pCB[1].m_nMaxS[i]) + i;

			for( int j = pCB[1].m_nMaxS[i] + 1; j <= pCB[0].m_nMaxS[i]; j++)
			{
				int nBand = m_vSFBT[nVersion][nFREQ].m_nS[j + 1] - m_vSFBT[nVersion][nFREQ].m_nS[j];
				int nSF = vL[j] + pSF->m_nS[i][j];

				FLOAT fR = pISDATA[nSF * 2 + 1];
				FLOAT fL = pISDATA[nSF * 2];

				for( int k=0; k<nBand; k++)
				{
					pVALUE[nIndex + 1152] = fR * pVALUE[nIndex];
					pVALUE[nIndex] = fL * pVALUE[nIndex];
					nIndex += 3;
				}
			}
		}
	}
	else
	{
		nIndex = GetBandIndex( nVersion, nFREQ, 0, pCB[1].m_nMax);
		nCount -= nIndex;

		for( i = pCB[1].m_nMax + 1; i < 21; i++)
		{
			int nBand = m_vSFBT[nVersion][nFREQ].m_nL[i + 1] - m_vSFBT[nVersion][nFREQ].m_nL[i];
			int nSF = vL[i] + pSF->m_nL[i];

			FLOAT fR = pISDATA[nSF * 2 + 1];
			FLOAT fL = pISDATA[nSF * 2];

			for( int j=0; j<nBand; j++)
			{
				nCount--;

				if( nCount < 0 )
					return;

				pVALUE[nIndex + 1152] = fR * pVALUE[nIndex];
				pVALUE[nIndex] = fL * pVALUE[nIndex];
				nIndex++;
			}
		}
	}
}

void CT3DMusic::XFormDualL( CT3DMusic *pMUSIC,
						    LPSIDE_INFO pSIDE,
							LPVOID pPCM,
							int nVersion,
							int nFREQ,
							int nGROUP)
{
	XFormMono(
		pMUSIC,
		pSIDE,
		pPCM,
		nVersion,
		nFREQ,
		nGROUP,
		nGROUP ^ 1,
		0, 0);
}

void CT3DMusic::XFormDualR( CT3DMusic *pMUSIC,
						    LPSIDE_INFO pSIDE,
							LPVOID pPCM,
							int nVersion,
							int nFREQ,
							int nGROUP)
{
	XFormMono(
		pMUSIC,
		pSIDE,
		pPCM,
		nVersion,
		nFREQ,
		nGROUP,
		nGROUP ^ 1,
		1, 0);
}

void CT3DMusic::XFormDualMono( CT3DMusic *pMUSIC,
							   LPSIDE_INFO pSIDE,
							   LPVOID pPCM,
							   int nVersion,
							   int nFREQ,
							   int nGROUP)
{
	int nPREV = nGROUP ^ 1;
	int vHYBRID[3];

	FLOAT vOUTBUF[576];

	if( pSIDE->m_vINFO[nGROUP][0].m_dwBlockType == pSIDE->m_vINFO[nGROUP][1].m_dwBlockType &&
		!pSIDE->m_vINFO[nGROUP][0].m_dwMixedBlockFlag &&
		!pSIDE->m_vINFO[nGROUP][1].m_dwMixedBlockFlag )
	{
		vHYBRID[1] = pMUSIC->m_vCOUNT[nGROUP][0];

		if( vHYBRID[1] < pMUSIC->m_vCOUNT[nGROUP][1] )
			vHYBRID[1] = pMUSIC->m_vCOUNT[nGROUP][1];

		if( vHYBRID[1] > pMUSIC->m_nBandLimit )
			vHYBRID[1] = pMUSIC->m_nBandLimit;

		if( pSIDE->m_vINFO[nGROUP][0].m_dwBlockType == 2 )
			vHYBRID[0] = 0;
		else
			vHYBRID[0] = vHYBRID[1];

		for( int i=0; i<vHYBRID[1]; i++)
			pMUSIC->m_pSAMPLE->m_vA[0][nGROUP][i].m_fF += pMUSIC->m_pSAMPLE->m_vA[1][nGROUP][i].m_fF;

		vHYBRID[2] = Hybrid(
			(FLOAT *) pMUSIC->m_pSAMPLE->m_vA[0][nGROUP],
			(FLOAT *) pMUSIC->m_pSAMPLE->m_vA[0][nPREV],
			vOUTBUF,
			pSIDE->m_vINFO[nGROUP][0].m_dwBlockType,
			vHYBRID[0],
			vHYBRID[1],
			pMUSIC->m_vCOUNT[nPREV][0],
			pMUSIC->m_nBandLimit);
		pMUSIC->m_vCOUNT[nGROUP][0] = vHYBRID[2];
	}
	else
	{
		vHYBRID[0] = pMUSIC->m_vCOUNT[nGROUP][0];
		vHYBRID[1] = vHYBRID[0];

		if( pSIDE->m_vINFO[nGROUP][0].m_dwBlockType == 2 )
			if(pSIDE->m_vINFO[nGROUP][0].m_dwMixedBlockFlag)
				vHYBRID[0] = GetBandIndex( nVersion, nFREQ, 0, pMUSIC->m_nMixedCBEnd - 1);
			else
				vHYBRID[0] = 0;

		vHYBRID[2] = Hybrid(
			(FLOAT *) pMUSIC->m_pSAMPLE->m_vA[0][nGROUP],
			(FLOAT *) pMUSIC->m_pSAMPLE->m_vA[0][nPREV],
			vOUTBUF,
			pSIDE->m_vINFO[nGROUP][0].m_dwBlockType,
			vHYBRID[0],
			vHYBRID[1],
			pMUSIC->m_vCOUNT[nPREV][0],
			pMUSIC->m_nBandLimit);
		pMUSIC->m_vCOUNT[nGROUP][0] = vHYBRID[2];

		vHYBRID[0] = pMUSIC->m_vCOUNT[nGROUP][1];
		vHYBRID[1] = vHYBRID[0];

		if( pSIDE->m_vINFO[nGROUP][1].m_dwBlockType == 2 )
			if(pSIDE->m_vINFO[nGROUP][1].m_dwMixedBlockFlag)
				vHYBRID[0] = GetBandIndex( nVersion, nFREQ, 0, pMUSIC->m_nMixedCBEnd - 1);
			else
				vHYBRID[0] = 0;

		pMUSIC->m_vCOUNT[nGROUP][1] = AddHybrid(
			(FLOAT *) pMUSIC->m_pSAMPLE->m_vA[1][nGROUP],
			(FLOAT *) pMUSIC->m_pSAMPLE->m_vA[0][nGROUP],
			vOUTBUF,
			pSIDE->m_vINFO[nGROUP][1].m_dwBlockType,
			vHYBRID[0],
			vHYBRID[1]);

		if( vHYBRID[2] < pMUSIC->m_vCOUNT[nGROUP][1] )
			vHYBRID[2] = pMUSIC->m_vCOUNT[nGROUP][1];
	}
	InvFREQ( vOUTBUF, vHYBRID[2]);

	pMUSIC->m_pSBTProc( pMUSIC, vOUTBUF, pPCM, 0);
}

void CT3DMusic::XFormDual( CT3DMusic *pMUSIC,
						   LPSIDE_INFO pSIDE,
						   LPVOID pPCM,
						   int nVersion,
						   int nFREQ,
						   int nGROUP)
{
	int nCount = pMUSIC->GetChannels();
	int nPREV = nGROUP ^ 1;

	for( int i=0; i<nCount; i++)
	{
		XFormMono(
			pMUSIC,
			pSIDE,
			pPCM,
			nVersion,
			nFREQ,
			nGROUP,
			nPREV,
			i, i);
	}
}

void CT3DMusic::XFormMono( CT3DMusic *pMUSIC,
						   LPSIDE_INFO pSIDE,
						   LPVOID pPCM,
						   int nVersion,
						   int nFREQ,
						   int nGROUP,
						   int nPREV,
						   int nSIDE,
						   int nCH)
{
	FLOAT vOUTBUF[576];
	int vHYBRID[2];

	for( int i=0; i<2; i++)
		vHYBRID[i] = pMUSIC->m_vCOUNT[nGROUP][nSIDE];

	if( pSIDE->m_vINFO[nGROUP][nSIDE].m_dwBlockType == 2 )
		if(pSIDE->m_vINFO[nGROUP][nSIDE].m_dwMixedBlockFlag)
			vHYBRID[0] = GetBandIndex( nVersion, nFREQ, 0, pMUSIC->m_nMixedCBEnd - 1);
		else
			vHYBRID[0] = 0;

	for( i=0; i<2; i++)
		if( vHYBRID[i] > pMUSIC->m_nBandLimit )
			vHYBRID[i] = pMUSIC->m_nBandLimit;
	ZeroMemory( vOUTBUF, 576 * sizeof(FLOAT));

	pMUSIC->m_vCOUNT[nGROUP][nSIDE] = Hybrid(
		(FLOAT *) pMUSIC->m_pSAMPLE->m_vA[nSIDE][nGROUP],
		(FLOAT *) pMUSIC->m_pSAMPLE->m_vA[nSIDE][nPREV],
		vOUTBUF,
		pSIDE->m_vINFO[nGROUP][nSIDE].m_dwBlockType,
		vHYBRID[0],
		vHYBRID[1],
		pMUSIC->m_vCOUNT[nPREV][nSIDE],
		pMUSIC->m_nBandLimit);
	InvFREQ( vOUTBUF, pMUSIC->m_vCOUNT[nGROUP][nSIDE]);

	pMUSIC->m_pSBTProc( pMUSIC, vOUTBUF, pPCM, nCH);
}

void CT3DMusic::L3SBTMono32S( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH)
{
	short *pDATA = (short *) pPCM;

	for( int i=0; i<18; i++)
	{
		FdctMono32( pSAMPLE, pMUSIC->m_vSBTBUF[0] + pMUSIC->m_vSBT[0]);
		WinMono32S( pMUSIC->m_vSBTBUF[0], pMUSIC->m_vSBT[0], pDATA);

		pMUSIC->m_vSBT[0] = (pMUSIC->m_vSBT[0] - 32) & 511;
		pSAMPLE += 32;
		pDATA += 32;
	}
}

void CT3DMusic::L3SBTDual32S( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH)
{
	short *pDATA = (short *) pPCM;
	BYTE bINDEX = nCH ? 1 : 0;

	if(nCH)
		pDATA++;

	for( int i=0; i<18; i++)
	{
		FdctMono32( pSAMPLE, pMUSIC->m_vSBTBUF[bINDEX] + pMUSIC->m_vSBT[bINDEX]);
		WinDual32S( pMUSIC->m_vSBTBUF[bINDEX], pMUSIC->m_vSBT[bINDEX], pDATA);

		pMUSIC->m_vSBT[bINDEX] = (pMUSIC->m_vSBT[bINDEX] - 32) & 511;
		pSAMPLE += 32;
		pDATA += 64;
	}
}

void CT3DMusic::L3SBTMono32B( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH)
{
	LPBYTE pDATA = (LPBYTE) pPCM;

	for( int i=0; i<18; i++)
	{
		FdctMono32( pSAMPLE, pMUSIC->m_vSBTBUF[0] + pMUSIC->m_vSBT[0]);
		WinMono32B( pMUSIC->m_vSBTBUF[0], pMUSIC->m_vSBT[0], pDATA);

		pMUSIC->m_vSBT[0] = (pMUSIC->m_vSBT[0] - 32) & 511;
		pSAMPLE += 32;
		pDATA += 32;
	}
}

void CT3DMusic::L3SBTDual32B( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH)
{
	LPBYTE pDATA = (LPBYTE) pPCM;
	BYTE bINDEX = nCH ? 1 : 0;

	if(nCH)
		pDATA++;

	for( int i=0; i<18; i++)
	{
		FdctMono32( pSAMPLE, pMUSIC->m_vSBTBUF[bINDEX] + pMUSIC->m_vSBT[bINDEX]);
		WinDual32B( pMUSIC->m_vSBTBUF[bINDEX], pMUSIC->m_vSBT[bINDEX], pDATA);

		pMUSIC->m_vSBT[bINDEX] = (pMUSIC->m_vSBT[bINDEX] - 32) & 511;
		pSAMPLE += 32;
		pDATA += 64;
	}
}

void CT3DMusic::L3SBTMono16S( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH)
{
	short *pDATA = (short *) pPCM;

	for( int i=0; i<18; i++)
	{
		FdctMono16( pSAMPLE, pMUSIC->m_vSBTBUF[0] + pMUSIC->m_vSBT[0]);
		WinMono16S( pMUSIC->m_vSBTBUF[0], pMUSIC->m_vSBT[0], pDATA);

		pMUSIC->m_vSBT[0] = (pMUSIC->m_vSBT[0] - 16) & 255;
		pSAMPLE += 32;
		pDATA += 16;
	}
}

void CT3DMusic::L3SBTDual16S( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH)
{
	short *pDATA = (short *) pPCM;
	BYTE bINDEX = nCH ? 1 : 0;

	if(nCH)
		pDATA++;

	for( int i=0; i<18; i++)
	{
		FdctMono16( pSAMPLE, pMUSIC->m_vSBTBUF[bINDEX] + pMUSIC->m_vSBT[bINDEX]);
		WinDual16S( pMUSIC->m_vSBTBUF[bINDEX], pMUSIC->m_vSBT[bINDEX], pDATA);

		pMUSIC->m_vSBT[bINDEX] = (pMUSIC->m_vSBT[bINDEX] - 16) & 255;
		pSAMPLE += 32;
		pDATA += 32;
	}
}

void CT3DMusic::L3SBTMono16B( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH)
{
	LPBYTE pDATA = (LPBYTE) pPCM;

	for( int i=0; i<18; i++)
	{
		FdctMono16( pSAMPLE, pMUSIC->m_vSBTBUF[0] + pMUSIC->m_vSBT[0]);
		WinMono16B( pMUSIC->m_vSBTBUF[0], pMUSIC->m_vSBT[0], pDATA);

		pMUSIC->m_vSBT[0] = (pMUSIC->m_vSBT[0] - 16) & 255;
		pSAMPLE += 32;
		pDATA += 16;
	}
}

void CT3DMusic::L3SBTDual16B( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH)
{
	LPBYTE pDATA = (LPBYTE) pPCM;
	BYTE bINDEX = nCH ? 1 : 0;

	if(nCH)
		pDATA++;

	for( int i=0; i<18; i++)
	{
		FdctMono16( pSAMPLE, pMUSIC->m_vSBTBUF[bINDEX] + pMUSIC->m_vSBT[bINDEX]);
		WinDual16B( pMUSIC->m_vSBTBUF[bINDEX], pMUSIC->m_vSBT[bINDEX], pDATA);

		pMUSIC->m_vSBT[bINDEX] = (pMUSIC->m_vSBT[bINDEX] - 16) & 255;
		pSAMPLE += 32;
		pDATA += 32;
	}
}

void CT3DMusic::L3SBTMono8S( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH)
{
	short *pDATA = (short *) pPCM;

	for( int i=0; i<18; i++)
	{
		FdctMono8( pSAMPLE, pMUSIC->m_vSBTBUF[0] + pMUSIC->m_vSBT[0]);
		WinMono8S( pMUSIC->m_vSBTBUF[0], pMUSIC->m_vSBT[0], pDATA);

		pMUSIC->m_vSBT[0] = (pMUSIC->m_vSBT[0] - 8) & 127;
		pSAMPLE += 32;
		pDATA += 8;
	}
}

void CT3DMusic::L3SBTDual8S( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH)
{
	short *pDATA = (short *) pPCM;
	BYTE bINDEX = nCH ? 1 : 0;

	if(nCH)
		pDATA++;

	for( int i=0; i<18; i++)
	{
		FdctMono8( pSAMPLE, pMUSIC->m_vSBTBUF[bINDEX] + pMUSIC->m_vSBT[bINDEX]);
		WinDual8S( pMUSIC->m_vSBTBUF[bINDEX], pMUSIC->m_vSBT[bINDEX], pDATA);

		pMUSIC->m_vSBT[bINDEX] = (pMUSIC->m_vSBT[bINDEX] - 8) & 127;
		pSAMPLE += 32;
		pDATA += 16;
	}
}

void CT3DMusic::L3SBTMono8B( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH)
{
	LPBYTE pDATA = (LPBYTE) pPCM;

	for( int i=0; i<18; i++)
	{
		FdctMono8( pSAMPLE, pMUSIC->m_vSBTBUF[0] + pMUSIC->m_vSBT[0]);
		WinMono8B( pMUSIC->m_vSBTBUF[0], pMUSIC->m_vSBT[0], pDATA);

		pMUSIC->m_vSBT[0] = (pMUSIC->m_vSBT[0] - 8) & 127;
		pSAMPLE += 32;
		pDATA += 8;
	}
}

void CT3DMusic::L3SBTDual8B( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH)
{
	LPBYTE pDATA = (LPBYTE) pPCM;
	BYTE bINDEX = nCH ? 1 : 0;

	if(nCH)
		pDATA++;

	for( int i=0; i<18; i++)
	{
		FdctMono8( pSAMPLE, pMUSIC->m_vSBTBUF[bINDEX] + pMUSIC->m_vSBT[bINDEX]);
		WinDual8B( pMUSIC->m_vSBTBUF[bINDEX], pMUSIC->m_vSBT[bINDEX], pDATA);

		pMUSIC->m_vSBT[bINDEX] = (pMUSIC->m_vSBT[bINDEX] - 8) & 127;
		pSAMPLE += 32;
		pDATA += 16;
	}
}

void CT3DMusic::SBTMono32S( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH)
{
	short *pDATA = (short *) pPCM;

	for( int i=0; i<nCH; i++)
	{
		FdctMono32( pSAMPLE, pMUSIC->m_vSBTBUF[0] + pMUSIC->m_vSBT[0]);
		WinMono32S( pMUSIC->m_vSBTBUF[0], pMUSIC->m_vSBT[0], pDATA);

		pMUSIC->m_vSBT[0] = (pMUSIC->m_vSBT[0] - 32) & 511;
		pSAMPLE += 64;
		pDATA += 32;
	}
}

void CT3DMusic::SBTDual32S( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH)
{
	short *pDATA = (short *) pPCM;

	for( int i=0; i<nCH; i++)
	{
		FdctDual32( pSAMPLE, pMUSIC->m_vSBTBUF[0] + pMUSIC->m_vSBT[0]);
		FdctDual32( pSAMPLE + 1, pMUSIC->m_vSBTBUF[1] + pMUSIC->m_vSBT[0]);

		WinDual32S( pMUSIC->m_vSBTBUF[0], pMUSIC->m_vSBT[0], pDATA);
		WinDual32S( pMUSIC->m_vSBTBUF[1], pMUSIC->m_vSBT[0], pDATA + 1);

		pMUSIC->m_vSBT[0] = (pMUSIC->m_vSBT[0] - 32) & 511;
		pSAMPLE += 64;
		pDATA += 64;
	}
}

void CT3DMusic::SBTMono32B( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH)
{
	LPBYTE pDATA = (LPBYTE) pPCM;

	for( int i=0; i<nCH; i++)
	{
		FdctMono32( pSAMPLE, pMUSIC->m_vSBTBUF[0] + pMUSIC->m_vSBT[0]);
		WinMono32B( pMUSIC->m_vSBTBUF[0], pMUSIC->m_vSBT[0], pDATA);

		pMUSIC->m_vSBT[0] = (pMUSIC->m_vSBT[0] - 32) & 511;
		pSAMPLE += 64;
		pDATA += 32;
	}
}

void CT3DMusic::SBTDual32B( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH)
{
	LPBYTE pDATA = (LPBYTE) pPCM;

	for( int i=0; i<nCH; i++)
	{
		FdctDual32( pSAMPLE, pMUSIC->m_vSBTBUF[0] + pMUSIC->m_vSBT[0]);
		FdctDual32( pSAMPLE + 1, pMUSIC->m_vSBTBUF[1] + pMUSIC->m_vSBT[0]);

		WinDual32B( pMUSIC->m_vSBTBUF[0], pMUSIC->m_vSBT[0], pDATA);
		WinDual32B( pMUSIC->m_vSBTBUF[1], pMUSIC->m_vSBT[0], pDATA + 1);

		pMUSIC->m_vSBT[0] = (pMUSIC->m_vSBT[0] - 32) & 511;
		pSAMPLE += 64;
		pDATA += 64;
	}
}

void CT3DMusic::SBTMono16S( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH)
{
	short *pDATA = (short *) pPCM;

	for( int i=0; i<nCH; i++)
	{
		FdctMono16( pSAMPLE, pMUSIC->m_vSBTBUF[0] + pMUSIC->m_vSBT[0]);
		WinMono16S( pMUSIC->m_vSBTBUF[0], pMUSIC->m_vSBT[0], pDATA);

		pMUSIC->m_vSBT[0] = (pMUSIC->m_vSBT[0] - 16) & 255;
		pSAMPLE += 64;
		pDATA += 16;
	}
}

void CT3DMusic::SBTDual16S( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH)
{
	short *pDATA = (short *) pPCM;

	for( int i=0; i<nCH; i++)
	{
		FdctDual16( pSAMPLE, pMUSIC->m_vSBTBUF[0] + pMUSIC->m_vSBT[0]);
		FdctDual16( pSAMPLE + 1, pMUSIC->m_vSBTBUF[1] + pMUSIC->m_vSBT[0]);

		WinDual16S( pMUSIC->m_vSBTBUF[0], pMUSIC->m_vSBT[0], pDATA);
		WinDual16S( pMUSIC->m_vSBTBUF[1], pMUSIC->m_vSBT[0], pDATA + 1);

		pMUSIC->m_vSBT[0] = (pMUSIC->m_vSBT[0] - 16) & 255;
		pSAMPLE += 64;
		pDATA += 32;
	}
}

void CT3DMusic::SBTMono16B( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH)
{
	LPBYTE pDATA = (LPBYTE) pPCM;

	for( int i=0; i<nCH; i++)
	{
		FdctMono16( pSAMPLE, pMUSIC->m_vSBTBUF[0] + pMUSIC->m_vSBT[0]);
		WinMono16B( pMUSIC->m_vSBTBUF[0], pMUSIC->m_vSBT[0], pDATA);

		pMUSIC->m_vSBT[0] = (pMUSIC->m_vSBT[0] - 16) & 255;
		pSAMPLE += 64;
		pDATA += 16;
	}
}

void CT3DMusic::SBTDual16B( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH)
{
	LPBYTE pDATA = (LPBYTE) pPCM;

	for( int i=0; i<nCH; i++)
	{
		FdctDual16( pSAMPLE, pMUSIC->m_vSBTBUF[0] + pMUSIC->m_vSBT[0]);
		FdctDual16( pSAMPLE + 1, pMUSIC->m_vSBTBUF[1] + pMUSIC->m_vSBT[0]);

		WinDual16B( pMUSIC->m_vSBTBUF[0], pMUSIC->m_vSBT[0], pDATA);
		WinDual16B( pMUSIC->m_vSBTBUF[1], pMUSIC->m_vSBT[0], pDATA + 1);

		pMUSIC->m_vSBT[0] = (pMUSIC->m_vSBT[0] - 16) & 255;
		pSAMPLE += 64;
		pDATA += 32;
	}
}

void CT3DMusic::SBTMono8S( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH)
{
	short *pDATA = (short *) pPCM;

	for( int i=0; i<nCH; i++)
	{
		FdctMono8( pSAMPLE, pMUSIC->m_vSBTBUF[0] + pMUSIC->m_vSBT[0]);
		WinMono8S( pMUSIC->m_vSBTBUF[0], pMUSIC->m_vSBT[0], pDATA);

		pMUSIC->m_vSBT[0] = (pMUSIC->m_vSBT[0] - 8) & 127;
		pSAMPLE += 64;
		pDATA += 8;
	}
}

void CT3DMusic::SBTDual8S( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH)
{
	short *pDATA = (short *) pPCM;

	for( int i=0; i<nCH; i++)
	{
		FdctDual8( pSAMPLE, pMUSIC->m_vSBTBUF[0] + pMUSIC->m_vSBT[0]);
		FdctDual8( pSAMPLE + 1, pMUSIC->m_vSBTBUF[1] + pMUSIC->m_vSBT[0]);

		WinDual8S( pMUSIC->m_vSBTBUF[0], pMUSIC->m_vSBT[0], pDATA);
		WinDual8S( pMUSIC->m_vSBTBUF[1], pMUSIC->m_vSBT[0], pDATA + 1);

		pMUSIC->m_vSBT[0] = (pMUSIC->m_vSBT[0] - 8) & 127;
		pSAMPLE += 64;
		pDATA += 16;
	}
}

void CT3DMusic::SBTMono8B( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH)
{
	LPBYTE pDATA = (LPBYTE) pPCM;

	for( int i=0; i<nCH; i++)
	{
		FdctMono8( pSAMPLE, pMUSIC->m_vSBTBUF[0] + pMUSIC->m_vSBT[0]);
		WinMono8B( pMUSIC->m_vSBTBUF[0], pMUSIC->m_vSBT[0], pDATA);

		pMUSIC->m_vSBT[0] = (pMUSIC->m_vSBT[0] - 8) & 127;
		pSAMPLE += 64;
		pDATA += 8;
	}
}

void CT3DMusic::SBTDual8B( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH)
{
	LPBYTE pDATA = (LPBYTE) pPCM;

	for( int i=0; i<nCH; i++)
	{
		FdctDual8( pSAMPLE, pMUSIC->m_vSBTBUF[0] + pMUSIC->m_vSBT[0]);
		FdctDual8( pSAMPLE + 1, pMUSIC->m_vSBTBUF[1] + pMUSIC->m_vSBT[0]);

		WinDual8B( pMUSIC->m_vSBTBUF[0], pMUSIC->m_vSBT[0], pDATA);
		WinDual8B( pMUSIC->m_vSBTBUF[1], pMUSIC->m_vSBT[0], pDATA + 1);

		pMUSIC->m_vSBT[0] = (pMUSIC->m_vSBT[0] - 8) & 127;
		pSAMPLE += 64;
		pDATA += 16;
	}
}

void CT3DMusic::FdctDual32( FLOAT *pSAMPLE, FLOAT *pBUF)
{
	FLOAT vBUF[2][32];

	int nSTART = 0;
	int nEND = 62;

	if(m_bEQ)
		for( int i=0; i<32; i++)
			pSAMPLE[i] *= m_vEQ[i];

	for( int i=0; i<16; i++)
	{
		vBUF[0][i + 16] = m_vCoef32[i] * (pSAMPLE[nSTART] - pSAMPLE[nEND]);
		vBUF[0][i] = pSAMPLE[nSTART] + pSAMPLE[nEND];

		nSTART += 2;
		nEND -= 2;
	}

	ForwardBF( m_vCoef32 + 16, vBUF[0], vBUF[1], 2, 16);
	ForwardBF( m_vCoef32 + 16 + 8, vBUF[1], vBUF[0], 4, 8);
	ForwardBF( m_vCoef32 + 16 + 8 + 4, vBUF[0], vBUF[1], 8, 4);
	ForwardBF( m_vCoef32 + 16 + 8 + 4 + 2, vBUF[1], vBUF[0], 16, 2);
	BackBF( vBUF[0], vBUF[1], 8, 4);
	BackBF( vBUF[1], vBUF[0], 4, 8);
	BackBF( vBUF[0], vBUF[1], 2, 16);
	BackBF( vBUF[1], pBUF, 1, 32);
}

void CT3DMusic::FdctMono32( FLOAT *pSAMPLE, FLOAT *pBUF)
{
	FLOAT vBUF[2][32];

	int nSTART = 0;
	int nEND = 31;

	if(m_bEQ)
		for( int i=0; i<32; i++)
			pSAMPLE[i] *= m_vEQ[i];

	for( int i=0; i<16; i++)
	{
		vBUF[0][i + 16] = m_vCoef32[i] * (pSAMPLE[nSTART] - pSAMPLE[nEND]);
		vBUF[0][i] = pSAMPLE[nSTART] + pSAMPLE[nEND];

		nSTART++;
		nEND--;
	}

	ForwardBF( m_vCoef32 + 16, vBUF[0], vBUF[1], 2, 16);
	ForwardBF( m_vCoef32 + 16 + 8, vBUF[1], vBUF[0], 4, 8);
	ForwardBF( m_vCoef32 + 16 + 8 + 4, vBUF[0], vBUF[1], 8, 4);
	ForwardBF( m_vCoef32 + 16 + 8 + 4 + 2, vBUF[1], vBUF[0], 16, 2);
	BackBF( vBUF[0], vBUF[1], 8, 4);
	BackBF( vBUF[1], vBUF[0], 4, 8);
	BackBF( vBUF[0], vBUF[1], 2, 16);
	BackBF( vBUF[1], pBUF, 1, 32);
}

void CT3DMusic::FdctDual16( FLOAT *pSAMPLE, FLOAT *pBUF)
{
	FLOAT vBUF[2][16];

	int nSTART = 2;
	int nEND = 28;

	vBUF[0][8] = m_vCoef32[16] * pSAMPLE[0];
	vBUF[0][0] = pSAMPLE[0];

	for( int i=1; i<8; i++)
	{
		vBUF[0][i + 8] = m_vCoef32[i + 16] * (pSAMPLE[nSTART] - pSAMPLE[nEND]);
		vBUF[0][i] = pSAMPLE[nSTART] + pSAMPLE[nEND];

		nSTART += 2;
		nEND -= 2;
	}

	ForwardBF( m_vCoef32 + 16 + 8, vBUF[0], vBUF[1], 2, 8);
	ForwardBF( m_vCoef32 + 16 + 8 + 4, vBUF[1], vBUF[0], 4, 4);
	ForwardBF( m_vCoef32 + 16 + 8 + 4 + 2, vBUF[0], vBUF[1], 8, 2);
	BackBF( vBUF[1], vBUF[0], 4, 4);
	BackBF( vBUF[0], vBUF[1], 2, 8);
	BackBF( vBUF[1], pBUF, 1, 16);
}

void CT3DMusic::FdctMono16( FLOAT *pSAMPLE, FLOAT *pBUF)
{
	FLOAT vBUF[2][16];

	int nSTART = 1;
	int nEND = 14;

	vBUF[0][8] = m_vCoef32[16] * pSAMPLE[0];
	vBUF[0][0] = pSAMPLE[0];

	for( int i=1; i<8; i++)
	{
		vBUF[0][i + 8] = m_vCoef32[i + 16] * (pSAMPLE[nSTART] - pSAMPLE[nEND]);
		vBUF[0][i] = pSAMPLE[nSTART] + pSAMPLE[nEND];

		nSTART++;
		nEND--;
	}

	ForwardBF( m_vCoef32 + 16 + 8, vBUF[0], vBUF[1], 2, 8);
	ForwardBF( m_vCoef32 + 16 + 8 + 4, vBUF[1], vBUF[0], 4, 4);
	ForwardBF( m_vCoef32 + 16 + 8 + 4 + 2, vBUF[0], vBUF[1], 8, 2);
	BackBF( vBUF[1], vBUF[0], 4, 4);
	BackBF( vBUF[0], vBUF[1], 2, 8);
	BackBF( vBUF[1], pBUF, 1, 16);
}

void CT3DMusic::FdctDual8( FLOAT *pSAMPLE, FLOAT *pBUF)
{
	FLOAT vBUF[2][8];

	int nSTART = 0;
	int nEND = 14;

	for( int i=0; i<4; i++)
	{
		vBUF[0][i + 4] = m_vCoef32[i + 16 + 8] * (pSAMPLE[nSTART] - pSAMPLE[nEND]);
		vBUF[0][i] = pSAMPLE[nSTART] + pSAMPLE[nEND];

		nSTART += 2;
		nEND -= 2;
	}

	ForwardBF( m_vCoef32 + 16 + 8 + 4, vBUF[0], vBUF[1], 2, 4);
	ForwardBF( m_vCoef32 + 16 + 8 + 4 + 2, vBUF[1], vBUF[0], 4, 2);
	BackBF( vBUF[0], vBUF[1], 2, 4);
	BackBF( vBUF[1], pBUF, 1, 8);
}

void CT3DMusic::FdctMono8( FLOAT *pSAMPLE, FLOAT *pBUF)
{
	FLOAT vBUF[2][8];

	int nSTART = 0;
	int nEND = 7;

	for( int i=0; i<4; i++)
	{
		vBUF[0][i + 4] = m_vCoef32[i + 16 + 8] * (pSAMPLE[nSTART] - pSAMPLE[nEND]);
		vBUF[0][i] = pSAMPLE[nSTART] + pSAMPLE[nEND];

		nSTART++;
		nEND--;
	}

	ForwardBF( m_vCoef32 + 16 + 8 + 4, vBUF[0], vBUF[1], 2, 4);
	ForwardBF( m_vCoef32 + 16 + 8 + 4 + 2, vBUF[1], vBUF[0], 4, 2);
	BackBF( vBUF[0], vBUF[1], 2, 4);
	BackBF( vBUF[1], pBUF, 1, 8);
}

void CT3DMusic::WinDual32S( FLOAT *pBUF, int nPOS, LPVOID pPCM)
{
	short *pDATA = (short *) pPCM;

	int nSI = nPOS + 16;
	int nBX = (nSI + 32) & 511;

	FLOAT *pCOEF = m_vWinCoef;
	FLOAT fTOTAL = 0.0f;

	for( int i=0; i<16; i++)
	{
		fTOTAL = 0.0f;

		for( int j=0; j<8; j++)
		{
			fTOTAL += (*pCOEF) * pBUF[nSI];
			pCOEF++;
			nSI = (nSI + 64) & 511;

			fTOTAL -= (*pCOEF) * pBUF[nBX];
			pCOEF++;
			nBX = (nBX + 64) & 511;
		}

		nSI++;
		nBX--;

		long nTOTAL = (long) fTOTAL;
		if( nTOTAL > 32767 )
			nTOTAL = 32767;
		else if( nTOTAL < -32768 )
			nTOTAL = -32768;

		*pDATA = (short) nTOTAL;
		pDATA += 2;
	}
	fTOTAL = 0.0f;

	for( i=0; i<8; i++)
	{
		fTOTAL += (*pCOEF) * pBUF[nBX];
		pCOEF++;
		nBX = (nBX + 64) & 511;
	}

	long nTOTAL = (long) fTOTAL;
	if( nTOTAL > 32767 )
		nTOTAL = 32767;
	else if( nTOTAL < -32768 )
		nTOTAL = -32768;

	*pDATA = (short) nTOTAL;
	pDATA += 2;
	pCOEF = m_vWinCoef + 255;

	for( i=0; i<15; i++)
	{
		fTOTAL = 0.0f;
		nSI--;
		nBX++;

		for( int j=0; j<8; j++)
		{
			fTOTAL += (*pCOEF) * pBUF[nSI];
			pCOEF--;
			nSI = (nSI + 64) & 511;

			fTOTAL += (*pCOEF) * pBUF[nBX];
			pCOEF--;
			nBX = (nBX + 64) & 511;
		}

		nTOTAL = (long) fTOTAL;
		if( nTOTAL > 32767 )
			nTOTAL = 32767;
		else if( nTOTAL < -32768 )
			nTOTAL = -32768;

		*pDATA = (short) nTOTAL;
		pDATA += 2;
	}
}

void CT3DMusic::WinDual32B( FLOAT *pBUF, int nPOS, LPVOID pPCM)
{
	LPBYTE pDATA = (LPBYTE) pPCM;

	int nSI = nPOS + 16;
	int nBX = (nSI + 32) & 511;

	FLOAT *pCOEF = m_vWinCoef;
	FLOAT fTOTAL = 0.0f;

	for( int i=0; i<16; i++)
	{
		fTOTAL = 0.0f;

		for( int j=0; j<8; j++)
		{
			fTOTAL += (*pCOEF) * pBUF[nSI];
			pCOEF++;
			nSI = (nSI + 64) & 511;

			fTOTAL -= (*pCOEF) * pBUF[nBX];
			pCOEF++;
			nBX = (nBX + 64) & 511;
		}

		nSI++;
		nBX--;

		long nTOTAL = (long) fTOTAL;
		if( nTOTAL > 32767 )
			nTOTAL = 32767;
		else if( nTOTAL < -32768 )
			nTOTAL = -32768;

		*pDATA = BYTE(nTOTAL >> 8) ^ 0x80;
		pDATA += 2;
	}
	fTOTAL = 0.0f;

	for( i=0; i<8; i++)
	{
		fTOTAL += (*pCOEF) * pBUF[nBX];
		pCOEF++;
		nBX = (nBX + 64) & 511;
	}

	long nTOTAL = (long) fTOTAL;
	if( nTOTAL > 32767 )
		nTOTAL = 32767;
	else if( nTOTAL < -32768 )
		nTOTAL = -32768;

	*pDATA = BYTE(nTOTAL >> 8) ^ 0x80;
	pDATA += 2;
	pCOEF = m_vWinCoef + 255;

	for( i=0; i<15; i++)
	{
		fTOTAL = 0.0f;
		nSI--;
		nBX++;

		for( int j=0; j<8; j++)
		{
			fTOTAL += (*pCOEF) * pBUF[nSI];
			pCOEF--;
			nSI = (nSI + 64) & 511;

			fTOTAL += (*pCOEF) * pBUF[nBX];
			pCOEF--;
			nBX = (nBX + 64) & 511;
		}

		nTOTAL = (long) fTOTAL;
		if( nTOTAL > 32767 )
			nTOTAL = 32767;
		else if( nTOTAL < -32768 )
			nTOTAL = -32768;

		*pDATA = BYTE(nTOTAL >> 8) ^ 0x80;
		pDATA += 2;
	}
}

void CT3DMusic::WinMono32S( FLOAT *pBUF, int nPOS, LPVOID pPCM)
{
	short *pDATA = (short *) pPCM;

	int nSI = nPOS + 16;
	int nBX = (nSI + 32) & 511;

	FLOAT *pCOEF = m_vWinCoef;
	FLOAT fTOTAL = 0.0f;

	for( int i=0; i<16; i++)
	{
		fTOTAL = 0.0f;

		for( int j=0; j<8; j++)
		{
			fTOTAL += (*pCOEF) * pBUF[nSI];
			pCOEF++;
			nSI = (nSI + 64) & 511;

			fTOTAL -= (*pCOEF) * pBUF[nBX];
			pCOEF++;
			nBX = (nBX + 64) & 511;
		}

		nSI++;
		nBX--;

		long nTOTAL = (long) fTOTAL;
		if( nTOTAL > 32767 )
			nTOTAL = 32767;
		else if( nTOTAL < -32768 )
			nTOTAL = -32768;

		*pDATA = (short) nTOTAL;
		pDATA++;
	}
	fTOTAL = 0.0f;

	for( i=0; i<8; i++)
	{
		fTOTAL += (*pCOEF) * pBUF[nBX];
		pCOEF++;
		nBX = (nBX + 64) & 511;
	}

	long nTOTAL = (long) fTOTAL;
	if( nTOTAL > 32767 )
		nTOTAL = 32767;
	else if( nTOTAL < -32768 )
		nTOTAL = -32768;

	*pDATA = (short) nTOTAL;
	pDATA++;
	pCOEF = m_vWinCoef + 255;

	for( i=0; i<15; i++)
	{
		fTOTAL = 0.0f;
		nSI--;
		nBX++;

		for( int j=0; j<8; j++)
		{
			fTOTAL += (*pCOEF) * pBUF[nSI];
			pCOEF--;
			nSI = (nSI + 64) & 511;

			fTOTAL += (*pCOEF) * pBUF[nBX];
			pCOEF--;
			nBX = (nBX + 64) & 511;
		}

		nTOTAL = (long) fTOTAL;
		if( nTOTAL > 32767 )
			nTOTAL = 32767;
		else if( nTOTAL < -32768 )
			nTOTAL = -32768;

		*pDATA = (short) nTOTAL;
		pDATA++;
	}
}

void CT3DMusic::WinMono32B( FLOAT *pBUF, int nPOS, LPVOID pPCM)
{
	LPBYTE pDATA = (LPBYTE) pPCM;

	int nSI = nPOS + 16;
	int nBX = (nSI + 32) & 511;

	FLOAT *pCOEF = m_vWinCoef;
	FLOAT fTOTAL = 0.0f;

	for( int i=0; i<16; i++)
	{
		fTOTAL = 0.0f;

		for( int j=0; j<8; j++)
		{
			fTOTAL += (*pCOEF) * pBUF[nSI];
			pCOEF++;
			nSI = (nSI + 64) & 511;

			fTOTAL -= (*pCOEF) * pBUF[nBX];
			pCOEF++;
			nBX = (nBX + 64) & 511;
		}

		nSI++;
		nBX--;

		long nTOTAL = (long) fTOTAL;
		if( nTOTAL > 32767 )
			nTOTAL = 32767;
		else if( nTOTAL < -32768 )
			nTOTAL = -32768;

		*pDATA = BYTE(nTOTAL >> 8) ^ 0x80;
		pDATA++;
	}
	fTOTAL = 0.0f;

	for( i=0; i<8; i++)
	{
		fTOTAL += (*pCOEF) * pBUF[nBX];
		pCOEF++;
		nBX = (nBX + 64) & 511;
	}

	long nTOTAL = (long) fTOTAL;
	if( nTOTAL > 32767 )
		nTOTAL = 32767;
	else if( nTOTAL < -32768 )
		nTOTAL = -32768;

	*pDATA = BYTE(nTOTAL >> 8) ^ 0x80;
	pDATA++;
	pCOEF = m_vWinCoef + 255;

	for( i=0; i<15; i++)
	{
		fTOTAL = 0.0f;
		nSI--;
		nBX++;

		for( int j=0; j<8; j++)
		{
			fTOTAL += (*pCOEF) * pBUF[nSI];
			pCOEF--;
			nSI = (nSI + 64) & 511;

			fTOTAL += (*pCOEF) * pBUF[nBX];
			pCOEF--;
			nBX = (nBX + 64) & 511;
		}

		nTOTAL = (long) fTOTAL;
		if( nTOTAL > 32767 )
			nTOTAL = 32767;
		else if( nTOTAL < -32768 )
			nTOTAL = -32768;

		*pDATA = BYTE(nTOTAL >> 8) ^ 0x80;
		pDATA++;
	}
}

void CT3DMusic::WinDual16S( FLOAT *pBUF, int nPOS, LPVOID pPCM)
{
	short *pDATA = (short *) pPCM;

	BYTE bSI = nPOS + 8;
	BYTE bBX = bSI + 16;

	FLOAT *pCOEF = m_vWinCoef;
	FLOAT fTOTAL = 0.0f;

	for( int i=0; i<8; i++)
	{
		fTOTAL = 0.0f;

		for( int j=0; j<8; j++)
		{
			fTOTAL += (*pCOEF) * pBUF[bSI];
			pCOEF++;
			bSI += 32;

			fTOTAL -= (*pCOEF) * pBUF[bBX];
			pCOEF++;
			bBX += 32;
		}

		pCOEF += 16;
		bSI++;
		bBX--;

		long nTOTAL = (long) fTOTAL;
		if( nTOTAL > 32767 )
			nTOTAL = 32767;
		else if( nTOTAL < -32768 )
			nTOTAL = -32768;

		*pDATA = (short) nTOTAL;
		pDATA += 2;
	}
	fTOTAL = 0.0f;

	for( i=0; i<8; i++)
	{
		fTOTAL += (*pCOEF) * pBUF[bBX];
		pCOEF++;
		bBX += 32;
	}

	long nTOTAL = (long) fTOTAL;
	if( nTOTAL > 32767 )
		nTOTAL = 32767;
	else if( nTOTAL < -32768 )
		nTOTAL = -32768;

	*pDATA = (short) nTOTAL;
	pDATA += 2;
	pCOEF = m_vWinCoef + 255;

	for( i=0; i<7; i++)
	{
		fTOTAL = 0.0f;
		pCOEF -= 16;
		bSI--;
		bBX++;

		for( int j=0; j<8; j++)
		{
			fTOTAL += (*pCOEF) * pBUF[bSI];
			pCOEF--;
			bSI += 32;

			fTOTAL += (*pCOEF) * pBUF[bBX];
			pCOEF--;
			bBX += 32;
		}

		nTOTAL = (long) fTOTAL;
		if( nTOTAL > 32767 )
			nTOTAL = 32767;
		else if( nTOTAL < -32768 )
			nTOTAL = -32768;

		*pDATA = (short) nTOTAL;
		pDATA += 2;
	}
}

void CT3DMusic::WinDual16B( FLOAT *pBUF, int nPOS, LPVOID pPCM)
{
	LPBYTE pDATA = (LPBYTE) pPCM;

	BYTE bSI = nPOS + 8;
	BYTE bBX = bSI + 16;

	FLOAT *pCOEF = m_vWinCoef;
	FLOAT fTOTAL = 0.0f;

	for( int i=0; i<8; i++)
	{
		fTOTAL = 0.0f;

		for( int j=0; j<8; j++)
		{
			fTOTAL += (*pCOEF) * pBUF[bSI];
			pCOEF++;
			bSI += 32;

			fTOTAL -= (*pCOEF) * pBUF[bBX];
			pCOEF++;
			bBX += 32;
		}

		pCOEF += 16;
		bSI++;
		bBX--;

		long nTOTAL = (long) fTOTAL;
		if( nTOTAL > 32767 )
			nTOTAL = 32767;
		else if( nTOTAL < -32768 )
			nTOTAL = -32768;

		*pDATA = BYTE(nTOTAL >> 8) ^ 0x80;
		pDATA += 2;
	}
	fTOTAL = 0.0f;

	for( i=0; i<8; i++)
	{
		fTOTAL += (*pCOEF) * pBUF[bBX];
		pCOEF++;
		bBX += 32;
	}

	long nTOTAL = (long) fTOTAL;
	if( nTOTAL > 32767 )
		nTOTAL = 32767;
	else if( nTOTAL < -32768 )
		nTOTAL = -32768;

	*pDATA = BYTE(nTOTAL >> 8) ^ 0x80;
	pDATA += 2;
	pCOEF = m_vWinCoef + 255;

	for( i=0; i<7; i++)
	{
		fTOTAL = 0.0f;
		pCOEF -= 16;
		bSI--;
		bBX++;

		for( int j=0; j<8; j++)
		{
			fTOTAL += (*pCOEF) * pBUF[bSI];
			pCOEF--;
			bSI += 32;

			fTOTAL += (*pCOEF) * pBUF[bBX];
			pCOEF--;
			bBX += 32;
		}

		nTOTAL = (long) fTOTAL;
		if( nTOTAL > 32767 )
			nTOTAL = 32767;
		else if( nTOTAL < -32768 )
			nTOTAL = -32768;

		*pDATA = BYTE(nTOTAL >> 8) ^ 0x80;
		pDATA += 2;
	}
}

void CT3DMusic::WinMono16S( FLOAT *pBUF, int nPOS, LPVOID pPCM)
{
	short *pDATA = (short *) pPCM;

	BYTE bSI = nPOS + 8;
	BYTE bBX = bSI + 16;

	FLOAT *pCOEF = m_vWinCoef;
	FLOAT fTOTAL = 0.0f;

	for( int i=0; i<8; i++)
	{
		fTOTAL = 0.0f;

		for( int j=0; j<8; j++)
		{
			fTOTAL += (*pCOEF) * pBUF[bSI];
			pCOEF++;
			bSI += 32;

			fTOTAL -= (*pCOEF) * pBUF[bBX];
			pCOEF++;
			bBX += 32;
		}

		pCOEF += 16;
		bSI++;
		bBX--;

		long nTOTAL = (long) fTOTAL;
		if( nTOTAL > 32767 )
			nTOTAL = 32767;
		else if( nTOTAL < -32768 )
			nTOTAL = -32768;

		*pDATA = (short) nTOTAL;
		pDATA++;
	}
	fTOTAL = 0.0f;

	for( i=0; i<8; i++)
	{
		fTOTAL += (*pCOEF) * pBUF[bBX];
		pCOEF++;
		bBX += 32;
	}

	long nTOTAL = (long) fTOTAL;
	if( nTOTAL > 32767 )
		nTOTAL = 32767;
	else if( nTOTAL < -32768 )
		nTOTAL = -32768;

	*pDATA = (short) nTOTAL;
	pDATA++;
	pCOEF = m_vWinCoef + 255;

	for( i=0; i<7; i++)
	{
		fTOTAL = 0.0f;
		pCOEF -= 16;
		bSI--;
		bBX++;

		for( int j=0; j<8; j++)
		{
			fTOTAL += (*pCOEF) * pBUF[bSI];
			pCOEF--;
			bSI += 32;

			fTOTAL += (*pCOEF) * pBUF[bBX];
			pCOEF--;
			bBX += 32;
		}

		nTOTAL = (long) fTOTAL;
		if( nTOTAL > 32767 )
			nTOTAL = 32767;
		else if( nTOTAL < -32768 )
			nTOTAL = -32768;

		*pDATA = (short) nTOTAL;
		pDATA++;
	}
}

void CT3DMusic::WinMono16B( FLOAT *pBUF, int nPOS, LPVOID pPCM)
{
	LPBYTE pDATA = (LPBYTE) pPCM;

	BYTE bSI = nPOS + 8;
	BYTE bBX = bSI + 16;

	FLOAT *pCOEF = m_vWinCoef;
	FLOAT fTOTAL = 0.0f;

	for( int i=0; i<8; i++)
	{
		fTOTAL = 0.0f;

		for( int j=0; j<8; j++)
		{
			fTOTAL += (*pCOEF) * pBUF[bSI];
			pCOEF++;
			bSI += 32;

			fTOTAL -= (*pCOEF) * pBUF[bBX];
			pCOEF++;
			bBX += 32;
		}

		pCOEF += 16;
		bSI++;
		bBX--;

		long nTOTAL = (long) fTOTAL;
		if( nTOTAL > 32767 )
			nTOTAL = 32767;
		else if( nTOTAL < -32768 )
			nTOTAL = -32768;

		*pDATA = BYTE(nTOTAL >> 8) ^ 0x80;
		pDATA++;
	}
	fTOTAL = 0.0f;

	for( i=0; i<8; i++)
	{
		fTOTAL += (*pCOEF) * pBUF[bBX];
		pCOEF++;
		bBX += 32;
	}

	long nTOTAL = (long) fTOTAL;
	if( nTOTAL > 32767 )
		nTOTAL = 32767;
	else if( nTOTAL < -32768 )
		nTOTAL = -32768;

	*pDATA = BYTE(nTOTAL >> 8) ^ 0x80;
	pDATA++;
	pCOEF = m_vWinCoef + 255;

	for( i=0; i<7; i++)
	{
		fTOTAL = 0.0f;
		pCOEF -= 16;
		bSI--;
		bBX++;

		for( int j=0; j<8; j++)
		{
			fTOTAL += (*pCOEF) * pBUF[bSI];
			pCOEF--;
			bSI += 32;

			fTOTAL += (*pCOEF) * pBUF[bBX];
			pCOEF--;
			bBX += 32;
		}

		nTOTAL = (long) fTOTAL;
		if( nTOTAL > 32767 )
			nTOTAL = 32767;
		else if( nTOTAL < -32768 )
			nTOTAL = -32768;

		*pDATA = BYTE(nTOTAL >> 8) ^ 0x80;
		pDATA++;
	}
}

void CT3DMusic::WinDual8S( FLOAT *pBUF, int nPOS, LPVOID pPCM)
{
	short *pDATA = (short *) pPCM;

	int nSI = nPOS + 4;
	int nBX = (nSI + 8) & 127;

	FLOAT *pCOEF = m_vWinCoef;
	FLOAT fTOTAL = 0.0f;

	for( int i=0; i<4; i++)
	{
		fTOTAL = 0.0f;

		for( int j=0; j<8; j++)
		{
			fTOTAL += (*pCOEF) * pBUF[nSI];
			pCOEF++;
			nSI = (nSI + 16) & 127;

			fTOTAL -= (*pCOEF) * pBUF[nBX];
			pCOEF++;
			nBX = (nBX + 16) & 127;
		}

		pCOEF += 48;
		nSI++;
		nBX--;

		long nTOTAL = (long) fTOTAL;
		if( nTOTAL > 32767 )
			nTOTAL = 32767;
		else if( nTOTAL < -32768 )
			nTOTAL = -32768;

		*pDATA = (short) nTOTAL;
		pDATA += 2;
	}
	fTOTAL = 0.0f;

	for( i=0; i<8; i++)
	{
		fTOTAL += (*pCOEF) * pBUF[nBX];
		pCOEF++;
		nBX = (nBX + 16) & 127;
	}

	long nTOTAL = (long) fTOTAL;
	if( nTOTAL > 32767 )
		nTOTAL = 32767;
	else if( nTOTAL < -32768 )
		nTOTAL = -32768;

	*pDATA = (short) nTOTAL;
	pDATA += 2;
	pCOEF = m_vWinCoef + 255;

	for( i=0; i<3; i++)
	{
		fTOTAL = 0.0f;
		pCOEF -= 48;
		nSI--;
		nBX++;

		for( int j=0; j<8; j++)
		{
			fTOTAL += (*pCOEF) * pBUF[nSI];
			pCOEF--;
			nSI = (nSI + 16) & 127;

			fTOTAL += (*pCOEF) * pBUF[nBX];
			pCOEF--;
			nBX = (nBX + 16) & 127;
		}

		nTOTAL = (long) fTOTAL;
		if( nTOTAL > 32767 )
			nTOTAL = 32767;
		else if( nTOTAL < -32768 )
			nTOTAL = -32768;

		*pDATA = (short) nTOTAL;
		pDATA += 2;
	}
}

void CT3DMusic::WinDual8B( FLOAT *pBUF, int nPOS, LPVOID pPCM)
{
	LPBYTE pDATA = (LPBYTE) pPCM;

	int nSI = nPOS + 4;
	int nBX = (nSI + 8) & 127;

	FLOAT *pCOEF = m_vWinCoef;
	FLOAT fTOTAL = 0.0f;

	for( int i=0; i<4; i++)
	{
		fTOTAL = 0.0f;

		for( int j=0; j<8; j++)
		{
			fTOTAL += (*pCOEF) * pBUF[nSI];
			pCOEF++;
			nSI = (nSI + 16) & 127;

			fTOTAL -= (*pCOEF) * pBUF[nBX];
			pCOEF++;
			nBX = (nBX + 16) & 127;
		}

		pCOEF += 48;
		nSI++;
		nBX--;

		long nTOTAL = (long) fTOTAL;
		if( nTOTAL > 32767 )
			nTOTAL = 32767;
		else if( nTOTAL < -32768 )
			nTOTAL = -32768;

		*pDATA = BYTE(nTOTAL >> 8) ^ 0x80;
		pDATA += 2;
	}
	fTOTAL = 0.0f;

	for( i=0; i<8; i++)
	{
		fTOTAL += (*pCOEF) * pBUF[nBX];
		pCOEF++;
		nBX = (nBX + 16) & 127;
	}

	long nTOTAL = (long) fTOTAL;
	if( nTOTAL > 32767 )
		nTOTAL = 32767;
	else if( nTOTAL < -32768 )
		nTOTAL = -32768;

	*pDATA = BYTE(nTOTAL >> 8) ^ 0x80;
	pDATA += 2;
	pCOEF = m_vWinCoef + 255;

	for( i=0; i<3; i++)
	{
		fTOTAL = 0.0f;
		pCOEF -= 48;
		nSI--;
		nBX++;

		for( int j=0; j<8; j++)
		{
			fTOTAL += (*pCOEF) * pBUF[nSI];
			pCOEF--;
			nSI = (nSI + 16) & 127;

			fTOTAL += (*pCOEF) * pBUF[nBX];
			pCOEF--;
			nBX = (nBX + 16) & 127;
		}

		nTOTAL = (long) fTOTAL;
		if( nTOTAL > 32767 )
			nTOTAL = 32767;
		else if( nTOTAL < -32768 )
			nTOTAL = -32768;

		*pDATA = BYTE(nTOTAL >> 8) ^ 0x80;
		pDATA += 2;
	}
}

void CT3DMusic::WinMono8S( FLOAT *pBUF, int nPOS, LPVOID pPCM)
{
	short *pDATA = (short *) pPCM;

	int nSI = nPOS + 4;
	int nBX = (nSI + 8) & 127;

	FLOAT *pCOEF = m_vWinCoef;
	FLOAT fTOTAL = 0.0f;

	for( int i=0; i<4; i++)
	{
		fTOTAL = 0.0f;

		for( int j=0; j<8; j++)
		{
			fTOTAL += (*pCOEF) * pBUF[nSI];
			pCOEF++;
			nSI = (nSI + 16) & 127;

			fTOTAL -= (*pCOEF) * pBUF[nBX];
			pCOEF++;
			nBX = (nBX + 16) & 127;
		}

		pCOEF += 48;
		nSI++;
		nBX--;

		long nTOTAL = (long) fTOTAL;
		if( nTOTAL > 32767 )
			nTOTAL = 32767;
		else if( nTOTAL < -32768 )
			nTOTAL = -32768;

		*pDATA = (short) nTOTAL;
		pDATA++;
	}
	fTOTAL = 0.0f;

	for( i=0; i<8; i++)
	{
		fTOTAL += (*pCOEF) * pBUF[nBX];
		pCOEF++;
		nBX = (nBX + 16) & 127;
	}

	long nTOTAL = (long) fTOTAL;
	if( nTOTAL > 32767 )
		nTOTAL = 32767;
	else if( nTOTAL < -32768 )
		nTOTAL = -32768;

	*pDATA = (short) nTOTAL;
	pDATA++;
	pCOEF = m_vWinCoef + 255;

	for( i=0; i<3; i++)
	{
		fTOTAL = 0.0f;
		pCOEF -= 48;
		nSI--;
		nBX++;

		for( int j=0; j<8; j++)
		{
			fTOTAL += (*pCOEF) * pBUF[nSI];
			pCOEF--;
			nSI = (nSI + 16) & 127;

			fTOTAL += (*pCOEF) * pBUF[nBX];
			pCOEF--;
			nBX = (nBX + 16) & 127;
		}

		nTOTAL = (long) fTOTAL;
		if( nTOTAL > 32767 )
			nTOTAL = 32767;
		else if( nTOTAL < -32768 )
			nTOTAL = -32768;

		*pDATA = (short) nTOTAL;
		pDATA++;
	}
}

void CT3DMusic::WinMono8B( FLOAT *pBUF, int nPOS, LPVOID pPCM)
{
	LPBYTE pDATA = (LPBYTE) pPCM;

	int nSI = nPOS + 4;
	int nBX = (nSI + 8) & 127;

	FLOAT *pCOEF = m_vWinCoef;
	FLOAT fTOTAL = 0.0f;

	for( int i=0; i<4; i++)
	{
		fTOTAL = 0.0f;

		for( int j=0; j<8; j++)
		{
			fTOTAL += (*pCOEF) * pBUF[nSI];
			pCOEF++;
			nSI = (nSI + 16) & 127;

			fTOTAL -= (*pCOEF) * pBUF[nBX];
			pCOEF++;
			nBX = (nBX + 16) & 127;
		}

		pCOEF += 48;
		nSI++;
		nBX--;

		long nTOTAL = (long) fTOTAL;
		if( nTOTAL > 32767 )
			nTOTAL = 32767;
		else if( nTOTAL < -32768 )
			nTOTAL = -32768;

		*pDATA = BYTE(nTOTAL >> 8) ^ 0x80;
		pDATA++;
	}
	fTOTAL = 0.0f;

	for( i=0; i<8; i++)
	{
		fTOTAL += (*pCOEF) * pBUF[nBX];
		pCOEF++;
		nBX = (nBX + 16) & 127;
	}

	long nTOTAL = (long) fTOTAL;
	if( nTOTAL > 32767 )
		nTOTAL = 32767;
	else if( nTOTAL < -32768 )
		nTOTAL = -32768;

	*pDATA = BYTE(nTOTAL >> 8) ^ 0x80;
	pDATA++;
	pCOEF = m_vWinCoef + 255;

	for( i=0; i<3; i++)
	{
		fTOTAL = 0.0f;
		pCOEF -= 48;
		nSI--;
		nBX++;

		for( int j=0; j<8; j++)
		{
			fTOTAL += (*pCOEF) * pBUF[nSI];
			pCOEF--;
			nSI = (nSI + 16) & 127;

			fTOTAL += (*pCOEF) * pBUF[nBX];
			pCOEF--;
			nBX = (nBX + 16) & 127;
		}

		nTOTAL = (long) fTOTAL;
		if( nTOTAL > 32767 )
			nTOTAL = 32767;
		else if( nTOTAL < -32768 )
			nTOTAL = -32768;

		*pDATA = BYTE(nTOTAL >> 8) ^ 0x80;
		pDATA++;
	}
}

void CT3DMusic::ForwardBF( FLOAT *pCOEF,
						   FLOAT *pDATA,
						   FLOAT *pBUF,
						   int nCount,
						   int nSIZE)
{
	int nCOUNT = nSIZE >> 1;
	int nInit = 0;

	for( int i=0; i<nCount; i++)
	{
		int nIndex = 0;
		int nSTART = nInit;
		int nEND = nSTART + nSIZE - 1;

		for( int j=0; j<nCOUNT; j++)
		{
			pBUF[nSTART + nCOUNT] = pCOEF[nIndex] * (pDATA[nSTART] - pDATA[nEND]);
			pBUF[nSTART] = pDATA[nSTART] + pDATA[nEND];

			nSTART++;
			nEND--;
			nIndex++;
		}

		nInit += nSIZE;
	}
}

void CT3DMusic::BackBF( FLOAT *pDATA,
					    FLOAT *pBUF,
						int nCount,
						int nSIZE)
{
	int nSize1 = nSIZE >> 1;
	int nSize2 = nSize1 - 1;
	int nInit = 0;

	for( int i=0; i<nCount; i++)
	{
		int nEVEN = nInit;
		int nODD = nInit;

		for( int j=0; j<nSize1; j++)
		{
			pBUF[nEVEN] = pDATA[nODD];
			nEVEN += 2;
			nODD++;
		}
		nEVEN = nInit + 1;

		for( j=0; j<nSize2; j++)
		{
			pBUF[nEVEN] = pDATA[nODD] + pDATA[nODD + 1];
			nEVEN += 2;
			nODD++;
		}
		pBUF[nEVEN] = pDATA[nODD];

		nInit += nSIZE;
	}
}

int CT3DMusic::AddHybrid( FLOAT *pIN,
						  FLOAT *pLEFT,
						  FLOAT *pOUT,
						  int nTYPE,
						  int nLONG,
						  int nTOT)
{
	if( nTYPE == 2 )
		nTYPE = 0;
	int nCount = (nLONG + 17) / 18;

	for( int i=0; i<nCount; i++)
	{
		Imdct18(pIN);

		for( int j=0; j<9; j++)
		{
			pOUT[(j + 9) * 32 + i] += m_vWIN[nTYPE][j + 9] * pIN[17 - j];
			pOUT[j * 32 + i] += m_vWIN[nTYPE][j] * pIN[j + 9];
		}

		for( j=0; j<4; j++)
		{
			FLOAT fB = pIN[8 - j];
			FLOAT fA = pIN[j];

			pLEFT[j] += m_vWIN[nTYPE][18 + j] * fB;
			pLEFT[8 - j] += m_vWIN[nTYPE][(18 + 8) - j] * fA;
			pLEFT[9 + j] += m_vWIN[nTYPE][(18 + 9) + j] * fA;
			pLEFT[17 - j] += m_vWIN[nTYPE][(18 + 17) - j] * fB;
		}
		FLOAT fA = pIN[j];

		pLEFT[j] += m_vWIN[nTYPE][18 + j] * fA;
		pLEFT[9 + j] += m_vWIN[nTYPE][(18 + 9) + j] * fA;

		pLEFT += 18;
		pIN += 18;
	}

	nCount = (nTOT + 17) / 18;
	while(i < nCount)
	{
		Imdct6_3(pIN);

		for( int j=0; j<3; j++)
		{
			pOUT[(6 + j) * 32 + i] += m_vWIN[2][j] * pIN[3 + j];
			pOUT[(9 + j) * 32 + i] += m_vWIN[2][3 + j] * pIN[5 - j];

			pOUT[(12 + j) * 32 + i] += m_vWIN[2][6 + j] * pIN[2 - j] + m_vWIN[2][j] * pIN[(6 + 3) + j];
			pOUT[(15 + j) * 32 + i] += m_vWIN[2][9 + j] * pIN[j] + m_vWIN[2][3 + j] * pIN[(6 + 5) - j];
		}

		for( j=0; j<3; j++)
		{
			pLEFT[j] += m_vWIN[2][6 + j] * pIN[(6 + 2) - j] + m_vWIN[2][j] * pIN[(12 + 3) + j];
			pLEFT[3 + j] += m_vWIN[2][9 + j] * pIN[6 + j] + m_vWIN[2][3 + j] * pIN[(12 + 5) - j];
		}

		for( j=0; j<3; j++)
		{
			pLEFT[6 + j] += m_vWIN[2][6 + j] * pIN[(12 + 2) - j];
			pLEFT[9 + j] += m_vWIN[2][9 + j] * pIN[12 + j];
		}

		pLEFT += 18;
		pIN += 18;
		i++;
	}

	return 18 * i;
}

int CT3DMusic::Hybrid( FLOAT *pIN,
					   FLOAT *pPREV,
					   FLOAT *pOUT,
					   int nTYPE,
					   int nLONG,
					   int nTOT,
					   int nPREV,
					   int nBand)
{
	if( nTYPE == 2 )
		nTYPE = 0;
	int nCount = (nLONG + 17) / 18;

	for( int i=0; i<nCount; i++)
	{
		Imdct18(pIN);

		for( int j=0; j<9; j++)
		{
			pOUT[(9 + j) * 32 + i] = pPREV[9 + j] + m_vWIN[nTYPE][9 + j] * pIN[17 - j];
			pOUT[j * 32 + i] = pPREV[j] + m_vWIN[nTYPE][j] * pIN[9 + j];
		}

		for( j=0; j<4; j++)
		{
			FLOAT fB = pIN[8 - j];
			FLOAT fA = pIN[j];

			pIN[j] = m_vWIN[nTYPE][18 + j] * fB;
			pIN[8 - j] = m_vWIN[nTYPE][(18 + 8) - j] * fA;
			pIN[9 + j] = m_vWIN[nTYPE][(18 + 9) + j] * fA;
			pIN[17 - j] = m_vWIN[nTYPE][(18 + 17) - j] * fB;
		}
		FLOAT fA = pIN[j];

		pIN[j] = m_vWIN[nTYPE][18 + j] * fA;
		pIN[9 + j] = m_vWIN[nTYPE][(18 + 9) + j] * fA;

		pPREV += 18;
		pIN += 18;
	}

	nCount = (nTOT + 17) / 18;
	while(i < nCount)
	{
		Imdct6_3(pIN);

		for( int j=0; j<3; j++)
		{
			pOUT[j * 32 + i] = pPREV[j];
			pOUT[(3 + j) * 32 + i] = pPREV[3 + j];

			pOUT[(6 + j) * 32 + i] = pPREV[6 + j] + m_vWIN[2][j] * pIN[3 + j];
			pOUT[(9 + j) * 32 + i] = pPREV[9 + j] + m_vWIN[2][3 + j] * pIN[5 - j];

			pOUT[(12 + j) * 32 + i] = pPREV[12 + j] + m_vWIN[2][6 + j] * pIN[2 - j] + m_vWIN[2][j] * pIN[(6 + 3) + j];
			pOUT[(15 + j) * 32 + i] = pPREV[15 + j] + m_vWIN[2][9 + j] * pIN[j] + m_vWIN[2][3 + j] * pIN[(6 + 5) - j];
		}

		for( j=0; j<3; j++)
		{
			pIN[j] = m_vWIN[2][6 + j] * pIN[(6 + 2) - j] + m_vWIN[2][j] * pIN[(12 + 3) + j];
			pIN[3 + j] = m_vWIN[2][9 + j] * pIN[6 + j] + m_vWIN[2][3 + j] * pIN[(12 + 5) - j];
		}

		for( j=0; j<3; j++)
		{
			pIN[6 + j] = m_vWIN[2][6 + j] * pIN[(12 + 2) - j];
			pIN[9 + j] = m_vWIN[2][9 + j] * pIN[12 + j];
		}

		for( j=0; j<3; j++)
		{
			pIN[12 + j] = 0.0f;
			pIN[15 + j] = 0.0f;
		}

		pPREV += 18;
		pIN += 18;
		i++;
	}

	nCount = (nPREV + 17) / 18;
	while(i < nCount)
	{
		for( int j=0; j<18; j++)
			pOUT[j * 32 + i] = pPREV[j];

		pPREV += 18;
		i++;
	}
	int nResult = 18 * i;

	nCount = (nBand + 17) / 18;
	while(i < nCount)
	{
		for( int j=0; j<18; j++)
			pOUT[j * 32 + i] = 0.0f;

		i++;
	}

	return nResult;
}

void CT3DMusic::Imdct6_3( FLOAT *pDATA)
{
	FLOAT vBUF[18];

	FLOAT *pBUF[2] = {
		vBUF,
		pDATA};

	for( int i=0; i<3; i++)
	{
		FLOAT vG[2] = {
			m_vV1[0] * pDATA[3 * 0],
			m_vV1[5] * pDATA[3 * 5]};

		pBUF[0][0] = vG[0] + vG[1];
		pBUF[0][3 + 0] = m_vV2[0] * (vG[0] - vG[1]);

		vG[0] = m_vV1[1] * pDATA[3 * 1];
		vG[1] = m_vV1[4] * pDATA[3 * 4];
		pBUF[0][1] = vG[0] + vG[1];
		pBUF[0][3 + 1] = m_vV2[1] * (vG[0] - vG[1]);

		vG[0] = m_vV1[2] * pDATA[3 * 2];
		vG[1] = m_vV1[3] * pDATA[3 * 3];
		pBUF[0][2] = vG[0] + vG[1];
		pBUF[0][3 + 2] = m_vV2[2] * (vG[0] - vG[1]);

		pBUF[0] += 6;
		pDATA++;
	}
	pBUF[0] = vBUF;

	for( i=0; i<3; i++)
	{
		FLOAT vA[2] = {
			pBUF[0][0] + pBUF[0][2],
			pBUF[0][3] + pBUF[0][5]};

		pBUF[1][0] = vA[0] + pBUF[0][1];
		pBUF[1][1] = vA[1] + pBUF[0][4];
		pBUF[1][2] = m_fCoef87 * (pBUF[0][0] - pBUF[0][2]);
		pBUF[1][3] = m_fCoef87 * (pBUF[0][3] - pBUF[0][5]) - pBUF[1][1];
		pBUF[1][1] = pBUF[1][1] - pBUF[1][0];
		pBUF[1][2] = pBUF[1][2] - pBUF[1][1];
		pBUF[1][4] = vA[0] - pBUF[0][1] - pBUF[0][1];
		pBUF[1][5] = vA[1] - pBUF[0][4] - pBUF[0][4] - pBUF[1][3];
		pBUF[1][3] = pBUF[1][3] - pBUF[1][2];
		pBUF[1][4] = pBUF[1][4] - pBUF[1][3];
		pBUF[1][5] = pBUF[1][5] - pBUF[1][4];

		pBUF[0] += 6;
		pBUF[1] += 6;
	}
}

void CT3DMusic::Imdct18( FLOAT *pDATA)
{
	FLOAT vA[9];
	FLOAT vB[9];

	for( int i=0; i<4; i++)
	{
		FLOAT fG2 = m_vW1[17 - i] * pDATA[17 - i];
		FLOAT fG1 = m_vW1[i] * pDATA[i];

		FLOAT fB1 = m_vW2[i] * (fG1 - fG2);
		FLOAT fA1 = fG1 + fG2;

		fG1 = m_vW1[8 - i] * pDATA[8 - i];
		fG2 = m_vW1[9 + i] * pDATA[9 + i];

		FLOAT fB2 = m_vW2[8 - i] * (fG1 - fG2);
		FLOAT fA2 = fG1 + fG2;

		vA[5 + i] = fA1 - fA2;
		vA[i] = fA1 + fA2;

		vB[5 + i] = fB1 - fB2;
		vB[i] = fB1 + fB2;
	}

	FLOAT fG2 = m_vW1[17 - i] * pDATA[17 - i];
	FLOAT fG1 = m_vW1[i] * pDATA[i];

	vB[i] = m_vW2[i] * (fG1 - fG2);
	vA[i] = fG1 + fG2;

	pDATA[0] = 0.5f * (vA[0] + vA[1] + vA[2] + vA[3] + vA[4]);
	pDATA[1] = 0.5f * (vB[0] + vB[1] + vB[2] + vB[3] + vB[4]);

	pDATA[2] = m_vCoef[1][0] * vA[5] + m_vCoef[1][1] * vA[6] + m_vCoef[1][2] * vA[7] + m_vCoef[1][3] * vA[8];
	pDATA[3] = m_vCoef[1][0] * vB[5] + m_vCoef[1][1] * vB[6] + m_vCoef[1][2] * vB[7] + m_vCoef[1][3] * vB[8] - pDATA[1];
	pDATA[1] = pDATA[1] - pDATA[0];
	pDATA[2] = pDATA[2] - pDATA[1];

	pDATA[4] = m_vCoef[2][0] * vA[0] + m_vCoef[2][1] * vA[1] + m_vCoef[2][2] * vA[2] + m_vCoef[2][3] * vA[3] - vA[4];
	pDATA[5] = m_vCoef[2][0] * vB[0] + m_vCoef[2][1] * vB[1] + m_vCoef[2][2] * vB[2] + m_vCoef[2][3] * vB[3] - vB[4] - pDATA[3];
	pDATA[3] = pDATA[3] - pDATA[2];
	pDATA[4] = pDATA[4] - pDATA[3];

	pDATA[6] = m_vCoef[3][0] * (vA[5] - vA[7] - vA[8]);
	pDATA[7] = m_vCoef[3][0] * (vB[5] - vB[7] - vB[8]) - pDATA[5];
	pDATA[5] = pDATA[5] - pDATA[4];
	pDATA[6] = pDATA[6] - pDATA[5];

	pDATA[8] = m_vCoef[4][0] * vA[0] + m_vCoef[4][1] * vA[1] + m_vCoef[4][2] * vA[2] + m_vCoef[4][3] * vA[3] + vA[4];
	pDATA[9] = m_vCoef[4][0] * vB[0] + m_vCoef[4][1] * vB[1] + m_vCoef[4][2] * vB[2] + m_vCoef[4][3] * vB[3] + vB[4] - pDATA[7];
	pDATA[7] = pDATA[7] - pDATA[6];
	pDATA[8] = pDATA[8] - pDATA[7];

	pDATA[10] = m_vCoef[5][0] * vA[5] + m_vCoef[5][1] * vA[6] + m_vCoef[5][2] * vA[7] + m_vCoef[5][3] * vA[8];
	pDATA[11] = m_vCoef[5][0] * vB[5] + m_vCoef[5][1] * vB[6] + m_vCoef[5][2] * vB[7] + m_vCoef[5][3] * vB[8] - pDATA[9];
	pDATA[9] = pDATA[9] - pDATA[8];
	pDATA[10] = pDATA[10] - pDATA[9];

	pDATA[12] = 0.5f * (vA[0] + vA[2] + vA[3]) - vA[1] - vA[4];
	pDATA[13] = 0.5f * (vB[0] + vB[2] + vB[3]) - vB[1] - vB[4] - pDATA[11];
	pDATA[11] = pDATA[11] - pDATA[10];
	pDATA[12] = pDATA[12] - pDATA[11];

	pDATA[14] = m_vCoef[7][0] * vA[5] + m_vCoef[7][1] * vA[6] + m_vCoef[7][2] * vA[7] + m_vCoef[7][3] * vA[8];
	pDATA[15] = m_vCoef[7][0] * vB[5] + m_vCoef[7][1] * vB[6] + m_vCoef[7][2] * vB[7] + m_vCoef[7][3] * vB[8] - pDATA[13];
	pDATA[13] = pDATA[13] - pDATA[12];
	pDATA[14] = pDATA[14] - pDATA[13];

	pDATA[16] = m_vCoef[8][0] * vA[0] + m_vCoef[8][1] * vA[1] + m_vCoef[8][2] * vA[2] + m_vCoef[8][3] * vA[3] + vA[4];
	pDATA[17] = m_vCoef[8][0] * vB[0] + m_vCoef[8][1] * vB[1] + m_vCoef[8][2] * vB[2] + m_vCoef[8][3] * vB[3] + vB[4] - pDATA[15];
	pDATA[15] = pDATA[15] - pDATA[14];
	pDATA[16] = pDATA[16] - pDATA[15];
	pDATA[17] = pDATA[17] - pDATA[16];
}

void CT3DMusic::InvFREQ( FLOAT *pDATA,
						 int nCount)
{
	nCount = (nCount + 17) / 18;

	for( int i=0; i<18; i+=2)
		for( int j=0; j<nCount; j+=2)
			pDATA[(i + 1) * 32 + j + 1] = -pDATA[(i + 1) * 32 + j + 1];
}
