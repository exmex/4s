#pragma once

class CTClientGuildMarkTexture
{
public:
	LPDIRECT3DTEXTURE9 m_pSYMBOL;
	LPDIRECT3DTEXTURE9 m_pMANTLE;

	CTClientGuildMarkTexture()
		: m_pSYMBOL(NULL), m_pMANTLE(NULL) {}
};

class CTClientGuildMark;

class CTMantleTextureKey
{
public:
	BYTE m_bFront;
	BYTE m_bBack;
	BYTE m_bFrontColor;
	BYTE m_bBackColor1;
	BYTE m_bBackColor2;
	BYTE m_bRaceID;
	BYTE m_bSexID;
	DWORD m_dwMESH;

	bool operator < ( const CTMantleTextureKey& k ) const
	{
		if( m_bFront != k.m_bFront )
			return m_bFront < k.m_bFront;

		if( m_bBack != k.m_bBack )
			return m_bBack < k.m_bBack;

		if( m_bFrontColor != k.m_bFrontColor )
			return m_bFrontColor < k.m_bFrontColor;

		if( m_bBackColor1 != k.m_bBackColor1 )
			return m_bBackColor1 < k.m_bBackColor1;

		if( m_bBackColor2 != k.m_bBackColor2 )
			return m_bBackColor2 < k.m_bBackColor2;
		
		if( m_bRaceID != k.m_bRaceID )
			return m_bRaceID < k.m_bRaceID;

		if( m_bSexID != k.m_bSexID )
			return m_bSexID < k.m_bSexID;

		return m_dwMESH < k.m_dwMESH;
	}
};

typedef std::map< CTMantleTextureKey, LPTEXTURESET> MAPTMANTLETEXTURE, *LPMAPTMANTLETEXTURE;

class CTClientGuildMark
{
public:
	typedef std::map<CTClientGuildMark, LPDIRECT3DTEXTURE9>	MAPGMARKTEXTURE;

protected:
	static VECTORIMAGE m_vFRONTIMG;
	static VECTORIMAGE m_vBACKIMG;
	static DWORD m_dwLEFTTICK;
	static MAPGMARKTEXTURE m_mapTEXTURE;
	static FLOAT m_vFrontVertices[20];
	static FLOAT m_vBackVertices[20];

public:
	static DWORD m_dwCOLOR[TMAXGUILD_MARK_COLOR];

	static VECTORTEXTURESET m_vFRONTTEX;
	static VECTORTEXTURESET m_vBACKTEX;

protected:
	static LPDIRECT3DTEXTURE9 MCreateTexture(const CTClientGuildMark& r);

public:
	static BOOL OnDeviceReset( BOOL bRelease );
	static void MAddFrontImage(CD3DImage* pImage);
	static void MAddBackImage(CD3DImage* pImage);

	static void MAddBakingFrontTexture( LPTEXTURESET p);
	static void MAddBakingBackTexture( LPTEXTURESET p);
	static void LoadBakingTexture();

	static void MReset();
	static void MRelease();

	static LPDIRECT3DTEXTURE9 MGetTexture(const CTClientGuildMark& r);
	static void SetMantleTexture( CTClientChar*, CTClientGuildMark* pMark );
	static void SetMantleColor( CTClientChar*, TCOLOR color );

	static MAPTMANTLETEXTURE m_mapTMANTLEBAKINGTEXTURE;
	static LPTEXTURESET GetMantleTexture(
		const CTMantleTextureKey& k,
		BOOL bDeviceReset=FALSE );
	static void ReleaseTMantleBakingTexture();

protected:
	BYTE m_bFront;
	BYTE m_bBack;
	BYTE m_bFrontColor;
	BYTE m_bBackColor1;
	BYTE m_bBackColor2;

public:
	CTClientGuildMark(BYTE bFront, 
					  BYTE bBack, 
					  BYTE bFrontColor,
					  BYTE bBackColor1,
					  BYTE bBackColor2);

	CTClientGuildMark(const CTClientGuildMark& r);

	bool operator < ( const CTClientGuildMark& r) const;
	CTClientGuildMark& operator = ( const CTClientGuildMark& r);

	LPDIRECT3DTEXTURE9 GetTexture();

	void SetFront(BYTE bFront, BYTE bFrontColor)	
	{
		m_bFront = bFront; 
		m_bFrontColor = bFrontColor;
	}
	
	void SetBack(BYTE bBack, BYTE bBackColor1, BYTE bBackColor2)	
	{
		m_bBack = bBack; 
		m_bBackColor1 = bBackColor1;
		m_bBackColor2 = bBackColor2;
	}

	void SetFront(BYTE bID)				{ m_bFront = bID; }
	void SetBack(BYTE bID)				{ m_bBack = bID; }

	void SetFrontColorID(BYTE eColor)	{ m_bFrontColor = eColor; }
	void SetBackColorID1(BYTE eColor)	{ m_bBackColor1 = eColor; }
	void SetBackColorID2(BYTE eColor)	{ m_bBackColor2 = eColor; }

	BYTE GetFront() const				{ return m_bFront; }
	BYTE GetBack() const				{ return m_bBack; }

	BYTE GetFrontColorID() const		{ return m_bFrontColor; }
	BYTE GetBackColorID1() const		{ return m_bBackColor1; }
	BYTE GetBackColorID2() const		{ return m_bBackColor2; }

	DWORD GetFrontColor() const			{ return m_dwCOLOR[m_bFrontColor]; }
	DWORD GetBackColor1() const			{ return m_dwCOLOR[m_bBackColor1]; }
	DWORD GetBackColor2() const			{ return m_dwCOLOR[m_bBackColor2]; }
};