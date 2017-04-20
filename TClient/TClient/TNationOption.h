#pragma once

class CTNationOption
{
public:
	static BOOL GERMANY;
	static BOOL FRANCE;
	static BOOL ENGLISH;
	static BOOL JAPAN;
	static BOOL POLAND;
	static BOOL TAIWAN;
	static BOOL CZECH;
	static BOOL ITALY;
	static BOOL SPAIN;
	static BOOL UNITEDKINGDOM;
	static BOOL GREECE;
	static BOOL RUSSIA;
	static BOOL ROMANIA;
	static BOOL UNITEDSTATE;
	static BOOL HUNGARY;
	static BOOL TURKEY;
	static BOOL PORTUGAL;
	static BOOL KOR;

	static BOOL MODIFY_WORD_BREAK;
	static BOOL CAMERA_QUAKE;
	static BOOL ADD_SLIDE;
	static BOOL USE_STATE_FRAME_WIDE;
	static BOOL JAPAN_IME;
	static BOOL TAIWAN_IME;
	static BOOL MODIFY_DIRECTLOGIN;
	static BOOL PASSWORD_MD5;
	static BOOL INSTALL_XTRAP;
	static BOOL INSTALL_APEX;
	static BOOL INSTALL_HACKSHIELD;
	static BOOL USE_WEB_CASHSHOP;
	static BOOL INSTALL_GAMEGUARD;
	static BOOL WAITING_REVIVAL_ITEM;

	static CString m_strAppTitle;

public:
	static void Clear();
	static void SetNation( INT nNation );
	static BOOL SetNation( CString strNation );
};



