#include "ProtocolBase.h"
#include <vector>
#include <queue>
#include <map>
#include <list>

using namespace std;

#define STR_TMACROMACHINE								_T("<SSLHYUN CREATE> TMACROMACHINE VERSION #1")

#define CHECK_SESSION									100
#define TIME_CHECK_SESSION								60000

#define TIME_WAIT										5000
#define TIME_ENDTHREAD									30000

#define WM_SESSION_MSG									(WM_USER + 0x3000)
#define WM_SESSION_CLOSE								(WM_USER + 0x4000)

#define CS_START_ACK									(CS_LOGIN + 0x000E)
#define CS_RELAYCONNECT_ACK								(CS_MAP + 0x01D4)
#define CS_CHAT_REQ										(CS_MAP + 0x0074)
#define CS_RELAYCHARDATA_REQ							(CS_MAP + 0x01D5)
#define CS_CORPSCMD_REQ									(CS_MAP + 0x00B2)
#define CS_CORPSENEMYLIST_REQ							(CS_MAP + 0x00B4)
#define CS_MOVECORPSENEMY_REQ							(CS_MAP + 0x00B6)
#define CS_MOVECORPSUNIT_REQ							(CS_MAP + 0x00B8)
#define CS_ADDCORPSENEMY_REQ							(CS_MAP + 0x00BA)
#define CS_DELCORPSENEMY_REQ							(CS_MAP + 0x00BC)
#define CS_CORPSHP_REQ									(CS_MAP + 0x00FD)
#define CS_RELAYPROTECTEDLIST_REQ						(CS_MAP + 0x01D7)
#define CS_RELAYPROTECTEDADD_REQ						(CS_MAP + 0x01D8)
#define CS_RELAYPROTECTEDDEL_REQ						(CS_MAP + 0x01D9)


#define STR_SM_BASE				_T("SM_BASE")			// System message base for server
#define STR_MW_BASE				_T("MW_BASE")			// Map server <-> World server message base
#define STR_DM_BASE				_T("DM_BASE")			// DB message base for server
#define STR_CS_LOGIN			_T("CS_LOGIN")			// Login server <-> Client message base
#define STR_CS_MAP				_T("CS_MAP")			// Map server <-> Client message base
#define STR_CT_CONTROL			_T("CT_CONTROL")		// Control Server Base
#define STR_CT_PATCH			_T("CT_PATCH")			// Patch Server <-> Client

#define TYPE_BYTE				0
#define TYPE_WORD				1
#define TYPE_DWORD				2
#define TYPE_FLOAT				3
#define TYPE_STRING				4
#define TYPE_INT64				5
#define TYPE_SOCKADDR			6
#define TYPE_LOOPSTART			7
#define TYPE_LOOPEND			8

#define STR_TYPE_BYTE			_T("BYTE")
#define STR_TYPE_WORD			_T("WORD")
#define STR_TYPE_DWORD			_T("DWORD")
#define STR_TYPE_FLOAT			_T("FLOAT")
#define STR_TYPE_STRING			_T("STRING")
#define STR_TYPE_INT64			_T("INT64")
#define STR_TYPE_SOCKADDR		_T("SOCKADDR")
#define STR_TYPE_LOOPSTART		_T("{")
#define STR_TYPE_LOOPEND		_T("}")

#define TYPE_INPUT				0
#define TYPE_OUTPUT				1
#define TYPE_SELF				2
#define TYPE_END				3

#define STR_TYPE_INPUT			_T("INPUT")
#define STR_TYPE_OUTPUT			_T("OUTPUT")
#define STR_TYPE_SELF			_T("SELF")
#define STR_TYPE_END			_T("END")

#define STR_SELF_NAME			_T("<SELF>")
#define STR_END_NAME			_T("<END>")

#define TYPE_START				0
#define TYPE_RECEIVE			1
#define TYPE_MAPCONNECT			2

#define OUTPUT_SUCCESS			0
#define OUTPUT_BREAK			1
#define OUTPUT_SELF				2
#define OUTPUT_END				3

#define STR_ERROR_MACRO			_T("매크로가 비어있습니다.")
#define STR_ERROR_MACROSEL		_T("매크로가 선택되지 않았습니다.")

typedef map<DWORD, CString>								MAPTPROTOCOL, *LPMAPTPROTOCOL;
typedef vector<BYTE>									VECTORTYPE, *LPVECTORTYPE;
typedef vector<DWORD>									VECTORDWORD, *LPVECTORDWORD;
typedef map<DWORD, VECTORTYPE>							MAPTPROTOCOLTYPE, *LPMAPTPROTOCOLTYPE;

typedef queue<CPacket *>								QPACKET, *LPQPACKET;
typedef map<SOCKET, CTachyonSession *>					MAPSESSION, *LPMAPSESSION;
typedef map<DWORD, CTachyonSession *>					MAPTSESSION, *LPMAPTSESSION;

typedef struct tagTYPE									TTYPE, *LPTTYPE;
typedef queue<TTYPE>									QTTYPE, *LPQTTYPE;

typedef struct tagPTVALUE								TPTVALUE, *LPTPTVALUE;
typedef vector<TPTVALUE>								VTPTVALUE, *LPVTPTVALUE;

typedef struct tagVALUE									TVALUE, *LPTVALUE;
typedef struct tagPROTOCOL								TPROTOCOL, *LPTPROTOCOL;
typedef struct tagMACRO									TMACRO, *LPTMACRO;
typedef struct tagMACROTEMP								TMACROTEMP, *LPTMACROTEMP;

typedef map<DWORD, LPTPROTOCOL>							MAPTPROTOCOLTEMP, *LPTMAPTPROTOCOLTEMP;
typedef map<DWORD, LPTMACROTEMP>						MAPTMACROTEMP, *LPMAPTMACROTEMP;
typedef map<DWORD, LPTVALUE>							MAPTVALUETEMP, *LPMAPTVALUETEMP;

typedef struct tagCHILDPROTOCOL							TCHILD, *LPTCHILD;
typedef struct tagQPROTOCOL								TQPROTOCOL, *LPTQPROTOCOL;

typedef map<DWORD, TVALUE>								MAPTVALUE, *LPMAPTVALUE;
typedef map<DWORD, TQPROTOCOL>							MAPTQPROTOCOL, *LPMAPTQPROTOCOL;
typedef queue<TQPROTOCOL>								QTPROTOCOL, *LPQTPROTOCOL;
typedef queue<TMACRO>									QTMACRO, *LPQTMACRO;
typedef vector<TQPROTOCOL>								VTQPROTOCOL, *LPVTPROTOCOL;
typedef vector<TCHILD>									VTCHILD, *LPVTCHILD;
typedef vector<DWORD>									VDWORD, *LPVDWORD;
typedef list< LPTPROTOCOL >								LISTTPROTOCOLTEMP, *LPLISTTPROTOCOLTEMP;


struct tagTYPE
{
	int m_nErrorCode;
	BYTE m_bType;
};

struct tagVALUE
{
	DWORD m_dwID;
	CString m_strName;
	BYTE m_bType;
	BYTE m_bValueType;
	CString m_strValue;
};

struct tagPTVALUE
{
	BYTE m_bValueType;
	DWORD m_dwValueID;
};

struct tagPROTOCOL
{
	DWORD m_dwIndex;
	DWORD m_dwID;
	DWORD m_dwParent;
	BYTE m_bLoop;
	BYTE m_bSelf;
	BYTE m_bEndChild;
    WORD m_wSleep;
	VTPTVALUE m_vType;
	
	tagPROTOCOL()
	{
		m_vType.clear();
	}
};

struct tagCHILDPROTOCOL
{
	DWORD m_dwID;
	BYTE m_bSelf;
	BYTE m_bRecvAck;
	BYTE m_bEndChild;
	VTPTVALUE m_vType;

	tagCHILDPROTOCOL()
	{
		m_dwID = 0;
		m_bRecvAck = FALSE;
		m_bEndChild = FALSE;
		m_vType.clear();
	};
};

struct tagQPROTOCOL
{
	DWORD m_dwID;
	BYTE m_bLoop;
    WORD m_wSleep;
	VTPTVALUE m_vType;
	TCHILD m_tChild;
	
	BYTE m_bEndChild;
	VTCHILD m_vTChild;

	tagQPROTOCOL()
	{
		m_vType.clear();
		m_vTChild.clear();
	};

	void ResetAck()
	{
		for(int i = 0; i < (int)m_vTChild.size(); i++)
			m_vTChild[i].m_bRecvAck = FALSE;
	}
};

struct tagMACRO
{
	DWORD m_dwID;
	CString m_strName;
	BYTE m_bLoop;
	QTPROTOCOL m_qProtocol;
};

struct tagMACROTEMP
{
	DWORD m_dwID;
	CString m_strName;
	BYTE m_bLoop;
	MAPTPROTOCOLTEMP m_mapProtocol;	
	LISTTPROTOCOLTEMP m_listChildProtocol;

	~tagMACROTEMP()
	{
		MAPTPROTOCOLTEMP::iterator it;
		for(it=m_mapProtocol.begin(); it!=m_mapProtocol.end(); it++)
			delete (*it).second;
		m_mapProtocol.clear();

		LISTTPROTOCOLTEMP::iterator itC;
		for( itC = m_listChildProtocol.begin(); itC != m_listChildProtocol.end(); itC++ )
			delete (*itC);
		m_listChildProtocol.end();
	};
};

typedef enum CONNECT_TYPE
{
	CON_LOGIN = 0,
	CON_MAP,
	CON_RELAY
};

class CTProtocol_Equal
{
public:
	CTProtocol_Equal(DWORD _dwID)
	{
		m_dwID = _dwID;
	};

	BOOL operator()(LPTPROTOCOL pProtocol) const 
	{
		if( m_dwID == pProtocol->m_dwID )
			return TRUE;
		else
			return FALSE;		
	};
public:
	DWORD m_dwID;
};