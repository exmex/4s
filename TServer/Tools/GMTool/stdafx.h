// stdafx.h : 잘 변경되지 않고 자주 사용하는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Windows 헤더에서 거의 사용되지 않는 내용을 제외시킵니다.
#endif

// 아래 지정된 플랫폼보다 우선하는 플랫폼을 대상으로 하는 경우 다음 정의를 수정하십시오.
// 다른 플랫폼에 사용되는 해당 값의 최신 정보는 MSDN을 참조하십시오.
#ifndef WINVER				// Windows 95 및 Windows NT 4 이후 버전에서만 기능을 사용할 수 있습니다.
#define WINVER 0x0400		// Windows 98과 Windows 2000 이후 버전에 맞도록 적합한 값으로 변경해 주십시오.
#endif

#ifndef _WIN32_WINNT		// Windows NT 4 이후 버전에서만 기능을 사용할 수 있습니다.
#define _WIN32_WINNT 0x0400		// Windows 98과 Windows 2000 이후 버전에 맞도록 적합한 값으로 변경해 주십시오.
#endif						

#ifndef _WIN32_WINDOWS		// Windows 98 이후 버전에서만 기능을 사용할 수 있습니다.
#define _WIN32_WINDOWS 0x0410 // Windows Me 이후 버전에 맞도록 적합한 값으로 변경해 주십시오.
#endif

#ifndef _WIN32_IE			// IE 4.0 이후 버전에서만 기능을 사용할 수 있습니다.
#define _WIN32_IE 0x0400	// IE 5.0 이후 버전에 맞도록 적합한 값으로 변경해 주십시오.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 일부 CString 생성자는 명시적으로 선언됩니다.

// MFC의 공통 부분과 무시 가능한 경고 메시지에 대한 숨기기를 해제합니다.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소
#include <afxext.h>         // MFC 익스텐션

#include <afxdtctl.h>		// Internet Explorer 4 공용 컨트롤에 대한 MFC 지원
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows 공용 컨트롤에 대한 MFC 지원
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>		// MFC 소켓 익스텐션

//////////////////////////////////////////////////////////////////////

#include <queue>
#include <map>
#include <vector>
#include <list>
using namespace std;

#include "Packet.h"
#include "CTProtocol.h"


//

#define DEF_PORT						3615
#define DEF_IP							_T("83.141.22.175")
#define CONFIG_FILE						_T("GMTool.txt")
#define POSLIST_FILE					_T("UserPosList.txt")
#define MAX_POSLIST_COLUMN				6
#define MSG_CONNECT_FAIL				_T("Failed to connect\nCheck the connection information out")
#define MSG_CONNECT_CLOSE				_T("the connection is Closed")
#define MSG_SVR_NOTSELECT				_T("Server is not selected")
#define MSG_CHARNAME_NULL				_T("You should enter the name")
#define MSG_AUTHORITY_NO				_T("You don't have the authority")
#define MSG_NOT_FOUND					_T("Not Fount")
#define MSG_NOT_FOUNTSERVER				_T("Not Fount Server")
#define MSG_DELETE_Q					_T("Are you sure you want to delete?")
#define MSG_ENTER_AGAIN					_T("Enter again")

#define ACTION_AT_DEAD					9
#define MAX_MONSPAWN_STAT_COLUMN		6
//
typedef struct tagUSERPOS				USERPOS, *LPUSERPOS;
typedef struct tagHAPPYGROUP			HAPPYGROUP, *LPHAPPYGROUP;

typedef list< LPUSERPOS >				LISTUSERPOS, *LPLISTUSERPOS;
typedef map<BYTE, LPHAPPYGROUP>			MAPGROUP, *LPMAPGROUP;



//

typedef enum MANAGER_CLASS
{	
	MANAGER_ALL = 1, // 모두 사용
	MANAGER_CONTROL , // 패치/업로드
	MANAGER_USER , // 공지사항/강제퇴장/위치이동
	MANAGER_SERVICE , // 서비스 ON/OFF	
	MANAGER_GMLEVEL1 , // GMTool 의 모든 기능(캐릭터 메세지,이동,채팅금지,강퇴)
	MANAGER_GMLEVEL2 ,  // GMTool 의 캐릭터 메세지, 이동 기능만.
	MANAGER_COUNT

} *LPMANAGER_CLASS;


struct tagUSERPOS
{
	char POSNAME[50];	
	DWORD ChannelNum;
	DWORD MapID;
	float PosX;
	float PosY;
	float PosZ;

	tagUSERPOS::tagUSERPOS()
	{
		strcpy(POSNAME,"");
	}
};

struct tagHAPPYGROUP
{
	BYTE m_bID;
	CString m_strName;
};

class CPosEqual
{
public:
	CPosEqual(char* _str)
	{
		strcpy(m_str, _str);
	};

	bool operator()(LPUSERPOS _USERPOS) const {
		if( !strcmp(m_str,_USERPOS->POSNAME) )
			return TRUE;
		else
			return FALSE;
	};
public:
	char m_str[100];
};