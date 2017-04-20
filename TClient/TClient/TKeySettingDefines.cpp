#include "Stdafx.h"
#include "TKeySettingDefines.h"
#include "TClient.h"
#include "Resource.h"

// ==================================================================================================
DWORD CTKeySetting::m_dwVersion = 1;
// ==================================================================================================
TKeySetInfo CTKeySetting::m_vDefInfo[TKEY_COUNT] = 
{
	{ FALSE,	MAKELONG( 7, TKEYMOD_NONE),				0,							CString("TKEY_NONE") },				
	{ TRUE,		MAKELONG( 'W', TKEYMOD_NONE),			TSTR_TKEY_FORWARD,			CString("TKEY_FORWARD") },			
	{ TRUE,		MAKELONG( 'S', TKEYMOD_NONE),			TSTR_TKEY_BACK,				CString("TKEY_BACK") },				
	{ TRUE,		MAKELONG( 'A', TKEYMOD_NONE),			TSTR_TKEY_LEFT,				CString("TKEY_LEFT") },				
	{ TRUE,		MAKELONG( 'D', TKEYMOD_NONE),			TSTR_TKEY_RIGHT,				CString("TKEY_RIGHT") },			
	{ TRUE,		MAKELONG( 'Q', TKEYMOD_NONE),			TSTR_TKEY_LSIDEE,			CString("TKEY_LSIDE") },			
	{ TRUE,		MAKELONG( 'E', TKEYMOD_NONE),			TSTR_TKEY_RSIDE,				CString("TKEY_RSIDE") },			
	{ TRUE,		MAKELONG( VK_SPACE, TKEYMOD_NONE),		TSTR_TKEY_JUMP,				CString("TKEY_JUMP") },				
	{ TRUE,		MAKELONG( 'R', TKEYMOD_NONE),			TSTR_TKEY_AUTORUN,			CString("TKEY_AUTORUN") },			
	{ TRUE,		MAKELONG( 'V', TKEYMOD_NONE),			TSTR_TKEY_RUN,				CString("TKEY_RUN") },				
	{ FALSE,	MAKELONG( VK_RBUTTON, TKEYMOD_NONE),	0,							CString("TKEY_ROT") },				
	{ TRUE,		MAKELONG( VK_LEFT, TKEYMOD_NONE),		TSTR_TKEY_CAM_LEFT,			CString("TKEY_CAM_LEFT") },			
	{ TRUE,		MAKELONG( VK_RIGHT, TKEYMOD_NONE),		TSTR_TKEY_CAM_RIGHT,			CString("TKEY_CAM_RIGHT") },		
	{ TRUE,		MAKELONG( VK_UP, TKEYMOD_NONE),			TSTR_TKEY_CAM_UP,			CString("TKEY_CAM_UP") },			
	{ TRUE,		MAKELONG( VK_DOWN, TKEYMOD_NONE),		TSTR_TKEY_CAM_DOWN,			CString("TKEY_CAM_DOWN") },			
	{ FALSE,	MAKELONG( VK_LBUTTON, TKEYMOD_NONE),	0,							CString("TKEY_CAM_ROT") },			
	{ TRUE,		MAKELONG( VK_DELETE, TKEYMOD_NONE),		TSTR_TKEY_ZOOM_IN,			CString("TKEY_ZOOM_IN") },			
	{ TRUE,		MAKELONG( VK_END, TKEYMOD_NONE),		TSTR_TKEY_ZOOM_OUT,			CString("TKEY_ZOOM_OUT") },			
	{ TRUE,		MAKELONG( VK_TAB, TKEYMOD_NONE),		TSTR_TKEY_PICK_NEXT,			CString("TKEY_PICK_NEXT") },		
	{ TRUE,		MAKELONG( 'X', TKEYMOD_NONE),			TSTR_TKEY_PICK_NEXT_DEAD,	CString("TKEY_PICK_NEXT_DEAD") },		
	
	{ TRUE,		MAKELONG( '1', TKEYMOD_NONE),			TSTR_TKEY_SKILL_1,			CString("TKEY_SKILL_1") },			
	{ TRUE,		MAKELONG( '2', TKEYMOD_NONE),			TSTR_TKEY_SKILL_2,			CString("TKEY_SKILL_2") },			
	{ TRUE,		MAKELONG( '3', TKEYMOD_NONE),			TSTR_TKEY_SKILL_3,			CString("TKEY_SKILL_3") },			
	{ TRUE,		MAKELONG( '4', TKEYMOD_NONE),			TSTR_TKEY_SKILL_4,			CString("TKEY_SKILL_4") },			
	{ TRUE,		MAKELONG( '5', TKEYMOD_NONE),			TSTR_TKEY_SKILL_5,			CString("TKEY_SKILL_5") },			
	{ TRUE,		MAKELONG( '6', TKEYMOD_NONE),			TSTR_TKEY_SKILL_6,			CString("TKEY_SKILL_6") },			
	{ TRUE,		MAKELONG( '7', TKEYMOD_NONE),			TSTR_TKEY_SKILL_7,			CString("TKEY_SKILL_7") },			
	{ TRUE,		MAKELONG( '8', TKEYMOD_NONE),			TSTR_TKEY_SKILL_8,			CString("TKEY_SKILL_8") },			
	{ TRUE,		MAKELONG( '9', TKEYMOD_NONE),			TSTR_TKEY_SKILL_9,			CString("TKEY_SKILL_9") },			
	{ TRUE,		MAKELONG( '0', TKEYMOD_NONE),			TSTR_TKEY_SKILL_10,			CString("TKEY_SKILL_10") },			
	{ TRUE,		MAKELONG( VK_OEM_MINUS, TKEYMOD_NONE),	TSTR_TKEY_SKILL_11,			CString("TKEY_SKILL_11") },			
	{ TRUE,		MAKELONG( VK_OEM_PLUS, TKEYMOD_NONE),	TSTR_TKEY_SKILL_12,			CString("TKEY_SKILL_12") },		
	{ TRUE,		MAKELONG( VK_F1, TKEYMOD_NONE),			TSTR_TKEY_SKILL2_1,			CString("TKEY_SKILL2_1") },			
	{ TRUE,		MAKELONG( VK_F2, TKEYMOD_NONE),			TSTR_TKEY_SKILL2_2,			CString("TKEY_SKILL2_2") },			
	{ TRUE,		MAKELONG( VK_F3, TKEYMOD_NONE),			TSTR_TKEY_SKILL2_3,			CString("TKEY_SKILL2_3") },			
	{ TRUE,		MAKELONG( VK_F4, TKEYMOD_NONE),			TSTR_TKEY_SKILL2_4,			CString("TKEY_SKILL2_4") },			
	{ TRUE,		MAKELONG( VK_F5, TKEYMOD_NONE),			TSTR_TKEY_SKILL2_5,			CString("TKEY_SKILL2_5") },			
	{ TRUE,		MAKELONG( VK_F6, TKEYMOD_NONE),			TSTR_TKEY_SKILL2_6,			CString("TKEY_SKILL2_6") },			
	{ TRUE,		MAKELONG( VK_F7, TKEYMOD_NONE),			TSTR_TKEY_SKILL2_7,			CString("TKEY_SKILL2_7") },			
	{ TRUE,		MAKELONG( VK_F8, TKEYMOD_NONE),			TSTR_TKEY_SKILL2_8,			CString("TKEY_SKILL2_8") },			
	{ TRUE,		MAKELONG( VK_F9, TKEYMOD_NONE),			TSTR_TKEY_SKILL2_9,			CString("TKEY_SKILL2_9") },			
	{ TRUE,		MAKELONG( VK_F10, TKEYMOD_NONE),		TSTR_TKEY_SKILL2_10,			CString("TKEY_SKILL2_10") },			
	{ TRUE,		MAKELONG( VK_F11, TKEYMOD_NONE),		TSTR_TKEY_SKILL2_11,			CString("TKEY_SKILL2_11") },			
	{ TRUE,		MAKELONG( VK_F12, TKEYMOD_NONE),		TSTR_TKEY_SKILL2_12,			CString("TKEY_SKILL2_12") },		

	{ FALSE,	MAKELONG( VK_RETURN, TKEYMOD_NONE),		TSTR_TKEY_CHAT,				CString("TKEY_CHAT") },	
	{ FALSE,	MAKELONG( VK_RETURN, TKEYMOD_CTRL),		TSTR_TKEY_WHISPER_REPLY,		CString("TKEY_WHISPER_REPLY") },
	{ TRUE,		MAKELONG( VK_ESCAPE, TKEYMOD_NONE),		TSTR_TKEY_CLOSE_UI,			CString("TKEY_CLOSE_UI") },			
	{ TRUE,		MAKELONG( 'M', TKEYMOD_NONE),			TSTR_TKEY_WORLDMAP,			CString("TKEY_WORLDMAP") },	
	{ TRUE,		MAKELONG( 'I', TKEYMOD_NONE),			TSTR_TKEY_INVEN,				CString("TKEY_INVEN") },			
	{ TRUE,		MAKELONG( 'C', TKEYMOD_NONE),			TSTR_TKEY_CHAR_INFO,			CString("TKEY_CHAR_INFO") },		
	{ TRUE,		MAKELONG( 'K', TKEYMOD_NONE),			TSTR_TKEY_SKILL_INFO,		CString("TKEY_SKILL_INFO") },		
	{ TRUE,		MAKELONG( 'J', TKEYMOD_NONE),			TSTR_TKEY_QUEST_INFO,		CString("TKEY_QUEST_INFO") },		
	{ TRUE,		MAKELONG( 'G', TKEYMOD_NONE),			TSTR_TKEY_TOGGLE_GUILD_UI,	CString("TKEY_TOGGLE_GUILD_UI") },
	{ TRUE,		MAKELONG( 'B', TKEYMOD_NONE),			TSTR_TKEY_MODE,				CString("TKEY_MODE") },				

	{ TRUE,		MAKELONG( '1', TKEYMOD_ALT),			TSTR_TKEY_RECALL_BACK,		CString("TKEY_RECALL_BACK") },		
	{ TRUE,		MAKELONG( '2', TKEYMOD_ALT),			TSTR_TKEY_RECALL_STAY,		CString("TKEY_RECALL_STAY") },		
	{ TRUE,		MAKELONG( '3', TKEYMOD_ALT),			TSTR_TKEY_RECALL_SKILL_1,	CString("TKEY_RECALL_SKILL_1") },	
	{ TRUE,		MAKELONG( '4', TKEYMOD_ALT),			TSTR_TKEY_RECALL_SKILL_2,	CString("TKEY_RECALL_SKILL_2") },	
	{ TRUE,		MAKELONG( '5', TKEYMOD_ALT),			TSTR_TKEY_RECALL_SKILL_3,	CString("TKEY_RECALL_SKILL_3") },	
	{ TRUE,		MAKELONG( '6', TKEYMOD_ALT),			TSTR_TKEY_RECALL_SKILL_4,	CString("TKEY_RECALL_SKILL_4") },	
	{ TRUE,		MAKELONG( '7', TKEYMOD_ALT),			TSTR_TKEY_RECALL_ACTIVE,		CString("TKEY_RECALL_ACTIVE") },	
	{ TRUE,		MAKELONG( '8', TKEYMOD_ALT),			TSTR_TKEY_RECALL_PASSIVE,	CString("TKEY_RECALL_PASSIVE") },	
	{ TRUE,		MAKELONG( '9', TKEYMOD_ALT),			TSTR_TKEY_RECALL_MANUAL,		CString("TKEY_RECALL_MANUAL") },	

	{ TRUE,		MAKELONG( VK_OEM_3, TKEYMOD_NONE),		TSTR_TKEY_TRSCS_ME,			CString("TKEY_TRSCS_ME") },			
	{ TRUE,		MAKELONG( VK_F1, TKEYMOD_CTRL),			TSTR_TKEY_TRSCS_F1,			CString("TKEY_TRSCS_F1") },			
	{ TRUE,		MAKELONG( VK_F2, TKEYMOD_CTRL),			TSTR_TKEY_TRSCS_F2,			CString("TKEY_TRSCS_F2") },			
	{ TRUE,		MAKELONG( VK_F3, TKEYMOD_CTRL),			TSTR_TKEY_TRSCS_F3,			CString("TKEY_TRSCS_F3") },			
	{ TRUE,		MAKELONG( VK_F4, TKEYMOD_CTRL),			TSTR_TKEY_TRSCS_F4,			CString("TKEY_TRSCS_F4") },			
	{ TRUE,		MAKELONG( VK_F5, TKEYMOD_CTRL),			TSTR_TKEY_TRSCS_F5,			CString("TKEY_TRSCS_F5") },			
	{ TRUE,		MAKELONG( VK_F6, TKEYMOD_CTRL),			TSTR_TKEY_TRSCS_F6,			CString("TKEY_TRSCS_F6") },			
	{ TRUE,		MAKELONG( VK_F7, TKEYMOD_CTRL),			TSTR_TKEY_TRSCS_F7,			CString("TKEY_TRSCS_F7") },			

	{ TRUE,		MAKELONG( '1', TKEYMOD_CTRL),			TSTR_TKEY_HOTKEY_F1,			CString("TKEY_HOTKEY_F1") },		
	{ TRUE,		MAKELONG( '2', TKEYMOD_CTRL),			TSTR_TKEY_HOTKEY_F2,			CString("TKEY_HOTKEY_F2") },		
	{ TRUE,		MAKELONG( '3', TKEYMOD_CTRL),			TSTR_TKEY_HOTKEY_F3,			CString("TKEY_HOTKEY_F3") },		
	{ TRUE,		MAKELONG( '4', TKEYMOD_CTRL),			TSTR_TKEY_HOTKEY_F4,			CString("TKEY_HOTKEY_F4") },		
	{ TRUE,		MAKELONG( '5', TKEYMOD_CTRL),			TSTR_TKEY_HOTKEY_F5,			CString("TKEY_HOTKEY_F5") },		
	{ TRUE,		MAKELONG( '6', TKEYMOD_CTRL),			TSTR_TKEY_HOTKEY_F6,			CString("TKEY_HOTKEY_F6") },		
	{ TRUE,		MAKELONG( '7', TKEYMOD_CTRL),			TSTR_TKEY_HOTKEY_F7,			CString("TKEY_HOTKEY_F7") },		
	{ TRUE,		MAKELONG( '8', TKEYMOD_CTRL),			TSTR_TKEY_HOTKEY_F8,			CString("TKEY_HOTKEY_F8") },		
	{ TRUE,		MAKELONG( '9', TKEYMOD_CTRL),			TSTR_TKEY_HOTKEY_F9,			CString("TKEY_HOTKEY_F9") },		
	{ TRUE,		MAKELONG( '0', TKEYMOD_CTRL),			TSTR_TKEY_HOTKEY_F10,		CString("TKEY_HOTKEY_F10") },	

	{ TRUE,		MAKELONG( 'Z', TKEYMOD_CTRL),			TSTR_TKEY_HOTKEY_PREV,		CString("TKEY_HOTKEY_PREV") },		
	{ TRUE,		MAKELONG( 'X', TKEYMOD_CTRL),			TSTR_TKEY_HOTKEY_NEXT,		CString("TKEY_HOTKEY_NEXT") },		
	{ TRUE,		MAKELONG( 'F', TKEYMOD_NONE),			TSTR_TKEY_HOTKEY_ACTION,		CString("TKEY_HOTKEY_ACTION") },	
	{ TRUE,		MAKELONG( 'Z', TKEYMOD_NONE),			TSTR_TKEY_GET_ALL,			CString("TKEY_GET_ALL") },			
	{ TRUE,		MAKELONG( 'U', TKEYMOD_CTRL),			TSTR_TKEY_SHOW_UI,			CString("TKEY_SHOW_UI") },			
	{ TRUE,		MAKELONG( 'U', TKEYMOD_NONE),			TSTR_TKEY_HUD,				CString("TKEY_HUD") },				
	{ TRUE,		MAKELONG( 'T', TKEYMOD_NONE),			TSTR_TKEY_FOLLOW,			CString("TKEY_FOLLOW") },		

	{ TRUE,		MAKELONG( 'N', TKEYMOD_NONE),			TSTR_TKEY_CHAR_ROT180,			CString("TKEY_CHAR_ROT180") },
	{ TRUE,		MAKELONG( 'Y', TKEYMOD_NONE),			TSTR_TKEY_CAM_ROT180,			CString("TKEY_CAM_ROT180") },				
	{ TRUE,		MAKELONG( 'P', TKEYMOD_NONE),			TSTR_TKEY_PET_INFO,			CString("TKEY_PET_INFO") },
	{ TRUE,		MAKELONG( 'H', TKEYMOD_NONE),			TSTR_TKEY_HELP,			CString("TKEY_HELP") },

	{ TRUE,		MAKELONG( 'O', TKEYMOD_NONE),			TSTR_TKEY_FAMERANK,			CString("TKEY_FAMERANK") },
	{ TRUE,		MAKELONG( VK_OEM_2, TKEYMOD_NONE),	TSTR_TKEY_CAUTION15,			CString("TKEY_CAUTION15") },
};
// ==================================================================================================


// ==================================================================================================
CTKeySetting::CTKeySetting()
{
	SetToDefault();

	m_mapVKeyToSid.clear();

	m_mapVKeyToSid.insert( make_pair(VK_LBUTTON,		TSTR_VK_LBUTTON) );
	m_mapVKeyToSid.insert( make_pair(VK_RBUTTON,		TSTR_VK_RBUTTON) );
	m_mapVKeyToSid.insert( make_pair(VK_CANCEL,			TSTR_VK_CANCEL) );
	m_mapVKeyToSid.insert( make_pair(VK_MBUTTON,		TSTR_VK_MBUTTON) );
	m_mapVKeyToSid.insert( make_pair(VK_BACK,			TSTR_VK_BACK) );
	m_mapVKeyToSid.insert( make_pair(VK_TAB,			TSTR_VK_TAB) );
	m_mapVKeyToSid.insert( make_pair(VK_CLEAR,			TSTR_VK_CLEAR) );
	m_mapVKeyToSid.insert( make_pair(VK_RETURN,			TSTR_VK_RETURN) );
	m_mapVKeyToSid.insert( make_pair(VK_SHIFT,			TSTR_VK_SHIFT) );
	m_mapVKeyToSid.insert( make_pair(VK_MENU,			TSTR_VK_MENU) );
	m_mapVKeyToSid.insert( make_pair(VK_PAUSE,			TSTR_VK_PAUSE) );
	m_mapVKeyToSid.insert( make_pair(VK_ESCAPE,			TSTR_VK_ESCAPE) );
	m_mapVKeyToSid.insert( make_pair(VK_SPACE,			TSTR_VK_SPACE) );
	m_mapVKeyToSid.insert( make_pair(VK_END,			TSTR_VK_END) );
	m_mapVKeyToSid.insert( make_pair(VK_HOME,			TSTR_VK_HOME) );
	m_mapVKeyToSid.insert( make_pair(VK_LEFT,			TSTR_VK_LEFT) );
	m_mapVKeyToSid.insert( make_pair(VK_UP,				TSTR_VK_UP) );
	m_mapVKeyToSid.insert( make_pair(VK_RIGHT,			TSTR_VK_RIGHT) );
	m_mapVKeyToSid.insert( make_pair(VK_DOWN,			TSTR_VK_DOWN) );
	m_mapVKeyToSid.insert( make_pair(VK_INSERT,			TSTR_VK_INSERT) );
	m_mapVKeyToSid.insert( make_pair(VK_DELETE,			TSTR_VK_DELETE) );
	m_mapVKeyToSid.insert( make_pair(VK_NUMPAD0, 		TSTR_VK_NUMPAD0) );
	m_mapVKeyToSid.insert( make_pair(VK_NUMPAD1, 		TSTR_VK_NUMPAD1) );
	m_mapVKeyToSid.insert( make_pair(VK_NUMPAD2, 		TSTR_VK_NUMPAD2) );
	m_mapVKeyToSid.insert( make_pair(VK_NUMPAD3, 		TSTR_VK_NUMPAD3) );
	m_mapVKeyToSid.insert( make_pair(VK_NUMPAD4, 		TSTR_VK_NUMPAD4) );
	m_mapVKeyToSid.insert( make_pair(VK_NUMPAD5, 		TSTR_VK_NUMPAD5) );
	m_mapVKeyToSid.insert( make_pair(VK_NUMPAD6, 		TSTR_VK_NUMPAD6) );
	m_mapVKeyToSid.insert( make_pair(VK_NUMPAD7, 		TSTR_VK_NUMPAD7) );
	m_mapVKeyToSid.insert( make_pair(VK_NUMPAD8, 		TSTR_VK_NUMPAD8) );
	m_mapVKeyToSid.insert( make_pair(VK_NUMPAD9, 		TSTR_VK_NUMPAD9) );
	m_mapVKeyToSid.insert( make_pair(VK_MULTIPLY,		TSTR_VK_MULTIPLY) );
	m_mapVKeyToSid.insert( make_pair(VK_ADD,			TSTR_VK_ADD) );
	m_mapVKeyToSid.insert( make_pair(VK_SEPARATOR,		TSTR_VK_SEPARATOR) );
	m_mapVKeyToSid.insert( make_pair(VK_SUBTRACT,		TSTR_VK_SUBTRACT) );
	m_mapVKeyToSid.insert( make_pair(VK_DECIMAL,		TSTR_VK_DECIMAL) );
	m_mapVKeyToSid.insert( make_pair(VK_DIVIDE,			TSTR_VK_DIVIDE) );
	m_mapVKeyToSid.insert( make_pair(VK_F1, 			TSTR_VK_F1) );
	m_mapVKeyToSid.insert( make_pair(VK_F2, 			TSTR_VK_F2) );
	m_mapVKeyToSid.insert( make_pair(VK_F3, 			TSTR_VK_F3) );
	m_mapVKeyToSid.insert( make_pair(VK_F4, 			TSTR_VK_F4) );
	m_mapVKeyToSid.insert( make_pair(VK_F5, 			TSTR_VK_F5) );
	m_mapVKeyToSid.insert( make_pair(VK_F6, 			TSTR_VK_F6) );
	m_mapVKeyToSid.insert( make_pair(VK_F7, 			TSTR_VK_F7) );
	m_mapVKeyToSid.insert( make_pair(VK_F8, 			TSTR_VK_F8) );
	m_mapVKeyToSid.insert( make_pair(VK_F9, 			TSTR_VK_F9) );
	m_mapVKeyToSid.insert( make_pair(VK_F10, 			TSTR_VK_F10) );
	m_mapVKeyToSid.insert( make_pair(VK_F11, 			TSTR_VK_F11) );
	m_mapVKeyToSid.insert( make_pair(VK_F12, 			TSTR_VK_F12) );
	m_mapVKeyToSid.insert( make_pair(VK_NUMLOCK,		TSTR_VK_NUMLOCK) );
	m_mapVKeyToSid.insert( make_pair(VK_SCROLL, 		TSTR_VK_SCROLL) );
	m_mapVKeyToSid.insert( make_pair(VK_LSHIFT, 		TSTR_VK_LSHIFT) );
	m_mapVKeyToSid.insert( make_pair(VK_RSHIFT, 		TSTR_VK_RSHIFT) );
	m_mapVKeyToSid.insert( make_pair('A',				TSTR_VK_A) );
	m_mapVKeyToSid.insert( make_pair('B',				TSTR_VK_B) );
	m_mapVKeyToSid.insert( make_pair('C',				TSTR_VK_C) );
	m_mapVKeyToSid.insert( make_pair('D',				TSTR_VK_D) );
	m_mapVKeyToSid.insert( make_pair('E',				TSTR_VK_E) );
	m_mapVKeyToSid.insert( make_pair('F',				TSTR_VK_F) );
	m_mapVKeyToSid.insert( make_pair('G',				TSTR_VK_G) );
	m_mapVKeyToSid.insert( make_pair('H',				TSTR_VK_H) );
	m_mapVKeyToSid.insert( make_pair('I',				TSTR_VK_I) );
	m_mapVKeyToSid.insert( make_pair('J',				TSTR_VK_J) );
	m_mapVKeyToSid.insert( make_pair('K',				TSTR_VK_K) );
	m_mapVKeyToSid.insert( make_pair('L',				TSTR_VK_L) );
	m_mapVKeyToSid.insert( make_pair('M',				TSTR_VK_M) );
	m_mapVKeyToSid.insert( make_pair('N',				TSTR_VK_N) );
	m_mapVKeyToSid.insert( make_pair('O',				TSTR_VK_O) );
	m_mapVKeyToSid.insert( make_pair('P',				TSTR_VK_P) );
	m_mapVKeyToSid.insert( make_pair('Q',				TSTR_VK_Q) );
	m_mapVKeyToSid.insert( make_pair('R',				TSTR_VK_R) );
	m_mapVKeyToSid.insert( make_pair('S',				TSTR_VK_S) );
	m_mapVKeyToSid.insert( make_pair('T',				TSTR_VK_T) );
	m_mapVKeyToSid.insert( make_pair('U',				TSTR_VK_U) );
	m_mapVKeyToSid.insert( make_pair('V',				TSTR_VK_V) );
	m_mapVKeyToSid.insert( make_pair('W',				TSTR_VK_W) );
	m_mapVKeyToSid.insert( make_pair('X',				TSTR_VK_X) );
	m_mapVKeyToSid.insert( make_pair('Y',				TSTR_VK_Y) );
	m_mapVKeyToSid.insert( make_pair('Z',				TSTR_VK_Z) );
	m_mapVKeyToSid.insert( make_pair('0',				TSTR_VK_0) );
	m_mapVKeyToSid.insert( make_pair('1',				TSTR_VK_1) );
	m_mapVKeyToSid.insert( make_pair('2',				TSTR_VK_2) );
	m_mapVKeyToSid.insert( make_pair('3',				TSTR_VK_3) );
	m_mapVKeyToSid.insert( make_pair('4',				TSTR_VK_4) );
	m_mapVKeyToSid.insert( make_pair('5',				TSTR_VK_5) );
	m_mapVKeyToSid.insert( make_pair('6',				TSTR_VK_6) );
	m_mapVKeyToSid.insert( make_pair('7',				TSTR_VK_7) );
	m_mapVKeyToSid.insert( make_pair('8',				TSTR_VK_8) );
	m_mapVKeyToSid.insert( make_pair('9',				TSTR_VK_9) );
	m_mapVKeyToSid.insert( make_pair(VK_OEM_1,	 		TSTR_VK_OEM_1) );
	m_mapVKeyToSid.insert( make_pair(VK_OEM_2,	 		TSTR_VK_OEM_2) );
	m_mapVKeyToSid.insert( make_pair(VK_OEM_3, 			TSTR_VK_OEM_3) );
	m_mapVKeyToSid.insert( make_pair(VK_OEM_4, 			TSTR_VK_OEM_4) );
	m_mapVKeyToSid.insert( make_pair(VK_OEM_5, 			TSTR_VK_OEM_5) );
	m_mapVKeyToSid.insert( make_pair(VK_OEM_6, 			TSTR_VK_OEM_6) );
	m_mapVKeyToSid.insert( make_pair(VK_OEM_PLUS,		TSTR_VK_OEM_PLUS) );
	m_mapVKeyToSid.insert( make_pair(VK_OEM_COMMA,		TSTR_VK_OEM_COMMA) );
	m_mapVKeyToSid.insert( make_pair(VK_OEM_MINUS,		TSTR_VK_OEM_MINUS) );
	m_mapVKeyToSid.insert( make_pair(VK_OEM_PERIOD,		TSTR_VK_OEM_PERIOD) );

	if( CTNationOption::JAPAN )
	{
		m_mapVKeyToSid.insert( make_pair(VK_OEM_7,		TSTR_VK_OEM_7) );
	}
}
// --------------------------------------------------------------------------------------------------
CTKeySetting::~CTKeySetting()
{
}
// --------------------------------------------------------------------------------------------------
CTKeySetting* CTKeySetting::GetInstance()
{
	static CTKeySetting inst;		
	return &inst;
}
// ==================================================================================================

// ==================================================================================================
const CString& CTKeySetting::VKeyToStr(WORD wVKey, WORD wModKey) const
{
	static CString strRESULT;
	
	strRESULT.Empty();
	
	MAPDWORD::const_iterator itr = m_mapVKeyToSid.find(wVKey);
	if( itr != m_mapVKeyToSid.end() )
	{
		if( wModKey & TKEYMOD_CTRL )
			strRESULT = "C";
		
		if( wModKey & TKEYMOD_ALT )
		{
			if( !strRESULT.IsEmpty() )
				strRESULT += ",";

			strRESULT += "A";
		}
		
		if( wModKey & TKEYMOD_SHIFT )
		{
			if( !strRESULT.IsEmpty() )
				strRESULT += ",";

			strRESULT += "S";
		}

		if( !strRESULT.IsEmpty() )
			strRESULT += "-";

		CString strKEY;
		strKEY = CTChart::LoadString( (TSTRING) itr->second);
		strRESULT += strKEY;
	}

	return strRESULT;
}
// ==================================================================================================
const TKeySetInfo& CTKeySetting::GetDefaultInfo(TKEY_SET eKey) const
{
	return m_vDefInfo[eKey];
}
// --------------------------------------------------------------------------------------------------
const BOOL CTKeySetting::NeedCustomize(TKEY_SET eKey) const
{
	return m_vDefInfo[eKey].m_bCustomize;
}
// --------------------------------------------------------------------------------------------------
const CString& CTKeySetting::GetKeyDesc(TKEY_SET eKey) const
{
	static CString strRESULT;

	if( !m_vDefInfo[eKey].m_dwDescStrID )
		strRESULT.Empty();
	else
		strRESULT = CTChart::LoadString( (TSTRING) m_vDefInfo[eKey].m_dwDescStrID);

	return strRESULT;
}
// ==================================================================================================
void CTKeySetting::SetToDefault()
{
	m_mapWKeyToVKey.clear();

	if( CTNationOption::JAPAN )
	{
		for(INT i=0; i<TKEY_COUNT; ++i)
		{
			if( m_vDefInfo[ i ].m_dwDefKey == MAKELONG( VK_OEM_PLUS, TKEYMOD_NONE) )
					m_vDefInfo[ i ].m_dwDefKey = MAKELONG( VK_OEM_7, TKEYMOD_NONE); // 일본 키보드 키배열에 의해 이 키가 더 어울린다.

			m_vKeySet[i] = m_vDefInfo[i].m_dwDefKey;
			m_mapWKeyToVKey.insert( make_pair(m_vKeySet[i], i) );
		}
	}
	if ( CTNationOption::FRANCE )
	{
		for(INT i=0; i<TKEY_COUNT; ++i)
		{
			
			switch(i)
			{
				case	TKEY_FORWARD		: m_vDefInfo[ i ].m_dwDefKey = MAKELONG( 'Z', TKEYMOD_NONE);		break;
				case	TKEY_LEFT			: m_vDefInfo[ i ].m_dwDefKey = MAKELONG( 'Q', TKEYMOD_NONE);		break;
				case	TKEY_LSIDE			: m_vDefInfo[ i ].m_dwDefKey = MAKELONG( 'A', TKEYMOD_NONE);		break;
				case	TKEY_GET_ALL		: m_vDefInfo[ i ].m_dwDefKey = MAKELONG( 'W', TKEYMOD_NONE);		break;
				case	TKEY_HOTKEY_PREV	: m_vDefInfo[ i ].m_dwDefKey = MAKELONG( 'W', TKEYMOD_CTRL);		break;
			}

			m_vKeySet[i] = m_vDefInfo[i].m_dwDefKey;
			m_mapWKeyToVKey.insert( make_pair(m_vKeySet[i], i) );
		}
	}
	else
	{
		for(INT i=0; i<TKEY_COUNT; ++i)
		{
			m_vKeySet[i] = m_vDefInfo[i].m_dwDefKey;
			m_mapWKeyToVKey.insert( make_pair(m_vKeySet[i], i) );
		}
	}
}
// --------------------------------------------------------------------------------------------------
DWORD CTKeySetting::SetKey(TKEY_SET eKey, WORD wVKey, WORD wModKey)
{
	return SetKey( eKey, MAKELONG(wVKey, wModKey) );
}
// --------------------------------------------------------------------------------------------------
DWORD CTKeySetting::SetKey(TKEY_SET eKey, DWORD dwKey)
{
	if( eKey == TKEY_NONE || m_vKeySet[eKey] == dwKey )
		return TKEY_RESULT_NOCHANGE;

	if( !m_vDefInfo[eKey].m_bCustomize )
		return TKEY_RESULT_NOCUSTOM;

	if( m_mapVKeyToSid.find(LOWORD(dwKey)) == m_mapVKeyToSid.end() )
		return TKEY_RESULT_NOCUSTOM;

	TKEY_SET eReplace = GetCurKeySet(dwKey);
	if( eReplace != TKEY_NONE )
	{
		SetCurKeySet(m_vKeySet[eKey], eReplace);
		m_vKeySet[eReplace] = m_vKeySet[eKey];
	}
	else
		SetCurKeySet(m_vKeySet[eKey], TKEY_NONE);

	SetCurKeySet(dwKey, eKey);
	m_vKeySet[eKey] = dwKey;
	
	return eReplace;
}
// ==================================================================================================
DWORD CTKeySetting::GetCurKey(TKEY_SET eKey) const
{
	return m_vKeySet[eKey];
}
// --------------------------------------------------------------------------------------------------
WORD CTKeySetting::GetCurVKey(TKEY_SET eKey) const
{
	return LOWORD(m_vKeySet[eKey]);
}
// --------------------------------------------------------------------------------------------------
WORD CTKeySetting::GetCurModKey(TKEY_SET eKey) const
{
	return HIWORD(m_vKeySet[eKey]);
}
// ==================================================================================================
TKEY_SET CTKeySetting::GetCurKeySet(WORD wVKey, WORD wModKey) const
{
	return GetCurKeySet(MAKELONG(wVKey, wModKey));
}
// --------------------------------------------------------------------------------------------------
TKEY_SET CTKeySetting::GetCurKeySet(DWORD dwKey) const
{
	MAPDWORD::const_iterator itr = m_mapWKeyToVKey.find(dwKey);
	if( itr == m_mapWKeyToVKey.end() )
		return TKEY_NONE;

	return (TKEY_SET)itr->second;
}
// ==================================================================================================
void CTKeySetting::SetCurKeySet(WORD wVKey, WORD wModKey, TKEY_SET eKey)
{
	SetCurKeySet( MAKELONG(wVKey, wModKey), eKey );
}
// --------------------------------------------------------------------------------------------------
void CTKeySetting::SetCurKeySet(DWORD dwKey, TKEY_SET eKey)
{
	MAPDWORD::iterator itr = m_mapWKeyToVKey.find(dwKey);
	if( itr != m_mapWKeyToVKey.end() )
	{
		if( eKey != TKEY_NONE )
			itr->second = eKey;
		else
			m_mapWKeyToVKey.erase(itr);
	}
	else
		m_mapWKeyToVKey.insert( make_pair(dwKey,eKey) );
}
// ==================================================================================================
void CTKeySetting::LoadRegistry(const CString& strKey)
{
	CString strVERSION;
	strVERSION = CTChart::LoadString( TSTR_TKEY_VERSION);

	DWORD dwVersion = (WORD)theApp.GetProfileInt(strKey, strVERSION, 0);
	if( dwVersion && dwVersion == m_dwVersion )
	{
		for(INT i=0; i<TKEY_COUNT; ++i)
		{
			if( !m_vDefInfo[i].m_bCustomize )
				continue;

			DWORD dwKey = (DWORD)(theApp.GetProfileInt(strKey, m_vDefInfo[i].m_strRegField, m_vKeySet[i]));
			SetKey((TKEY_SET)i, dwKey);
		}
	}
}
// --------------------------------------------------------------------------------------------------
void CTKeySetting::SaveRegistry(const CString& strKey)
{
	CString strVERSION;
	strVERSION = CTChart::LoadString( TSTR_TKEY_VERSION);

	theApp.WriteProfileInt(strKey, strVERSION, m_dwVersion);

	for(INT i=0; i<TKEY_COUNT; ++i)
		theApp.WriteProfileInt(strKey, m_vDefInfo[i].m_strRegField, m_vKeySet[i]);
}
// ==================================================================================================











