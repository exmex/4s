#if !defined __TNETDEF_H
#define __TNETDEF_H


//////////////////////////////////////////////////////////////////////////////
// TNetLib constant define

#define ONE_KBYTE					(1024)
#define MAX_THREAD					(256)


//////////////////////////////////////////////////////////////////////////////
// Default port

#define DEF_CONTROLPORT				(3615)
#define DEF_PATCHPORT				(3715)
#define DEF_WORLDPORT				(3815)
#define DEF_LOGINPORT				(4815)
#define DEF_MAPPORT					(5815)

//////////////////////////////////////////////////////////////////////////////
// Completion key
#define COMP_NULL					((BYTE) 0)
#define COMP_ACCEPT					((BYTE) 1)
#define COMP_SESSION				((BYTE) 2)
#define COMP_CLOSE					((BYTE) 3)
#define COMP_SEND					((BYTE) 4)
#define COMP_EXIT					((BYTE) 255)

//////////////////////////////////////////////////////////////////////////////
// TNetLib macro

#define SMART_LOCKCS(x)				CCSLock lock_object(x);

inline __time64_t __DBTOTIME(TIMESTAMP_STRUCT timestamp)
{
	if(timestamp.year < 2000)
		return 0;

	CTime time(
		timestamp.year,
		timestamp.month,
		timestamp.day,
		timestamp.hour,
		timestamp.minute,
		timestamp.second);

	return time.GetTime();
};

inline void __TIMETODB(__time64_t time_t, TIMESTAMP_STRUCT & timestamp)
{
	if(!time_t)
	{
		timestamp.year = 1900;
		timestamp.month = 1;
		timestamp.day = 1;
		timestamp.hour = 0;
		timestamp.minute = 0;
		timestamp.second = 0;
		timestamp.fraction = 0;
	}
	else
	{
		CTime time(time_t);

		timestamp.year = time.GetYear();
		timestamp.month = time.GetMonth();
		timestamp.day = time.GetDay();
		timestamp.hour = time.GetHour();
		timestamp.minute = time.GetMinute();
		timestamp.second = time.GetSecond();
		timestamp.fraction = 0;
	}
};

inline DWORD TRand(DWORD dwMaxNumber)
{
	if(dwMaxNumber <= 1)
		return 0;

	DWORD dwRand = 0;
	WORD wCM = 0;

	if(dwMaxNumber > 0xFFFFFF)
	{
		dwRand = (rand() % HIBYTE(HIWORD(dwMaxNumber))) << 24;
		wCM = WORD((dwMaxNumber - (dwMaxNumber & 0xFF000000)) >> 16) + 0x100;
	}

	if(dwMaxNumber > 0xFFFF)
	{
		dwRand = dwRand + (rand() % max(wCM, LOBYTE(HIWORD(dwMaxNumber))) << 16);
		wCM = WORD((dwMaxNumber - (dwMaxNumber & 0xFFFF0000)) >> 8) + 0x100;
	}

	if(dwMaxNumber > 0xFF)
	{
		dwRand = dwRand + (rand() % max(wCM, HIBYTE(LOWORD(dwMaxNumber))) << 8);
		wCM = WORD(dwMaxNumber - (dwMaxNumber & 0xFFFFFF00) + 0x100);
	}

	dwRand = dwRand + (rand() % max(wCM, LOBYTE(LOWORD(dwMaxNumber))));

	return dwRand;
}

//////////////////////////////////////////////////////////////////////////////
// TNetLib type define

typedef struct tagPACKETBUF			PACKETBUF, *LPPACKETBUF;
typedef queue<LPPACKETBUF>			QPACKETBUF, *LPQPACKETBUF;

//////////////////////////////////////////////////////////////////////////////
// TNetLib structure

struct tagPACKETBUF
{
	CSession *m_pSESSION;
	CPacket m_packet;
	tagPACKETBUF()
	{
		m_pSESSION = NULL;
	};
};

//////////////////////////////////////////////////////////////////////////////
// Smart sync class for critical section

class CCSLock
{
public:
	CCSLock( CRITICAL_SECTION *pCS)
	{
		EnterCriticalSection(pCS);
		m_pCS = pCS;
	};

	virtual ~CCSLock()
	{
		LeaveCriticalSection(m_pCS);
	};

protected:
	CRITICAL_SECTION *m_pCS;
};


#endif // !defined __TNETDEF_H
