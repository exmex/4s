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
