////////////////////////////////////////////////////////////////////////////////
//	File Name	:	NameingList.h
//
//
//	Copyright	:	(c) Zemi Interactive, Inc. 2002-2007
////////////////////////////////////////////////////////////////////////////////
#pragma once


#include "../../../TProtocol/LogPacket.h"

LPCTSTR ACTIONLIST[] =
{
	"Log In"				,		//	0x0000		
	"Create Charater"		,		//	0x0004		1
	"Delete Charater"		,		//	0x0005

	"Start Game"			,		//	0x0008

	"Enter Map"				,		//	0x1010
	"Level Up"				,		//	0x1013
	"User Die"				,		//	0x1015

	"Mon Spwon"				,		//  0x1020
	"Mon Kill"				,		//	0x1021		
	"Mon MoneySolo"			,		//	0x1022		
	"Mon MoneyParty"		,		//	0x1023		10
	"Mon ExpSolo"			,		//	0x1024
	"Mon ExpParty"			,		//	0x1025

	"Item ByMonSolo"		,		//	0x1030
	"Item ByMonParty"		,		//	0x1031
	"Item ByWorld"			,		//	0x1032

	"Item Delete"			,		//	0x1034
	"Item Buy"				,		//	0x1035
	"Item Sell"				,		//	0x1036
	"Item Use"				,		//	0x1037
	
	"Item Upgrade Success"	,		// 0x1040		20
	"Item Upgrade FaliDel"	,		// 0x1041
	
	"Item MakeSuccess"		,		// 0x1042
	"Item MakeZero"			,		// 0x1043
	"Item MakeFailDel"		,		// 0x1044

	"Item DownGradeSuccess"	,		// 0x1045
	"Item DownGradeFail"	,		// 0x1046

	"Item Cabinet In"		,		//	0x1050
	"Item Cabinet Out"		,		//	0x1051
	"Item Cabinet Make"		,		//	0x1052

	"Item Trade Rec"		,		//	0x1055		30
	"Item Trade Send"		,		//	0x1056		
	"Money Trade Rec"		,		//	0x1057
	"Money Trade Send"		,		//	0x1058

	"Post Item Send"		,		//	0x1060
	"Post Item Get"			,		//	0x1061

	"Post Money Send"		,		//	0x1062
	"Post Money Get"		,		//	0x1063

	"Post Item Del"			,		//	0x0064
	"Post Money Del"		,		//	0x0065
	
	"Store Item Sell"		,		//	0x1070		40
	"Store Item Buy"		,		//	0x1071
	"Store Item Del"		,		//	0x1072

	"Item Duration End"		,		//	0x1075

	"Item Repair Success"	,		//	0x1076
	"Item Refine Del"		,		//	0x1077
	"Item Refine Success"	,		//	0x0078
	"Item Refine Fail"		,		//	0x1079
	
	"Quest Start"			,		//	0x1080		
	"Quest Drop"			,		//	0x1081
	"Quest Failed"			,		//	0x1082		50
	"Quest Success"			,		//	0x1083
	"Quest ExpGet"			,		//	0x1084
	"Quest MoneyGet"		,		//	0x1085
	"Quest ItemGet"			,		//	0x1086
	"Quest ItemDel"			,		//	0x1087

	"Gamble Success"		,		//	0x0090
	"Gamble Fail"			,		//	0x0091
	"Gamble RepFail"		,		//	0x0092

	"Seal Success"			,		//	0x0095
	"Seal Fail"				,		//	0x0096		60
	"Seal RepFail"			,		//	0x0097

	"Skill Buy"				,		//	0x1100		
	"Skill Up"				,		//	0x1101

	"Pet Make"				,		//	0x1110			
	"Pet Delete"			,		//	0x1111

	"Chat Wisper"			,		//	LOG_BASEMAP		+ 0x0120
	"Chat Near"				,		//	LOG_BASEMAP		+ 0x0121
	"Chat Map"				,		//	LOG_BASEMAP		+ 0x0122
	"Chat Wold"				,		//	LOG_BASEMAP		+ 0x0123
	"Chat Party"			,		//	LOG_BASEMAP		+ 0x0124	70
	"Chat Guild"			,		//	LOG_BASEMAP		+ 0x0125
	"Chat Force"			,		//	LOG_BASEMAP		+ 0x0126
	"Chat Operator"			,		//	LOG_BASEMAP		+ 0x0127
	"Teleport"				,		//  LOG_BASEMAP		+ 0x0130

	"LocalOccupy"			,		//	LOG_BASEMAP		+ 0x1140
	"LocalEnable"			,		//	LOG_BASEMAP		+ 0x1141

	"PvP Point Gain Char"	,		//	LOG_BASEMAP		+ 0x0150
	"PvP Point Use Char"	,		//	LOG_BASEMAP		+ 0x0151
	"PvP Point Gain Guild"	,		//	LOG_BASEMAP		+ 0x0152
	"PvP Point Use Guild"	,		//	LOG_BASEMAP		+ 0x0153	80

	"Auction Reg"			,		//	LOG_BASEMAP		+ 0x0154
	"Auction Reg Cancel"	,		//	LOG_BASEMAP		+ 0x0155
	"Auction Bid"			,		//	LOG_BASEMAP		+ 0x0156
	"Auction Bid Success"	,		//	LOG_BASEMAP		+ 0x0157
	"Auction Bid Fail"		,		//	LOG_BASEMAP		+ 0x0158
	"Auction Bid Drop"		,		//	LOG_BASEMAP		+ 0x0159
	"Auction Buy Direct"	,		//	LOG_BASEMAP		+ 0x015A
	"Auction Success Money"	,		//	LOG_BASEMAP		+ 0x015B

	"Cash ItemBuy"			,		//	LOG_BASEMAP		+ 0x01A0
	"Cash ItemPresent"		,		//	LOG_BASEMAP		+ 0x01A1	90	

	"Cash BankIn"			,		//	LOG_BASEMAP		+ 0x01A4
	"Cash BankOut"			,		//	LOG_BASEMAP		+ 0x01A5

	"Cash CabinetBuy"		,		//	LOG_BASEMAP		+ 0x01A8
	"Cash ChangeChar"		,		//	LOG_BASEMAP		+ 0x01AA
	"Cash RandMoney"		,		//	LOG_BASEMAP		+ 0x01AB

	"Guild Create"			,		//	0x1200
	"Guild Disorganization"	,		//	0x1201
	"Guild Invite"			,		//	0x1202	
	"Guild InviteAnswer"	,		//	0x1203		
	"Guild Join"			,		//	0x1204						100
	
	"Guild Leave"			,		//	0x1205						
	"Guild Kickout"			,		//	0x1206		
	
	"Guild Duty"			,		//	0x1207
	"Guild Peer"			,		//	0x1208
	
	"Guild Contribution"	,		//	0x1209
	
	"Guild LevelUP"			,		//	0x0210

	"Guild CabinetPutin"	,		//	0x0211
	"Guild CabinetTakeout"	,		//	0x0212

	"Tournament Apply"		,		//LOG_BASEMAP		+ 0x0250
	"Tournament Evenet"		,		//LOG_BASEMAP		+ 0x0251	110
	"Tournament Win"		,		//LOG_BASEMAP		+ 0x0252
	
	"RPS Reword"			,		//LOG_BASEMAP		+ 0x0253

	"Country Change"		,		//LOG_BASEMAP		+ 0x0254
	"AIDCountry Change"		,		//LOG_BASEMAP		+ 0x0255
	
	"Kick Out"						//LOG_BASEMAP		+ 0x0256
};

LPCSTR DBFIELDSTR[] =
{
	"LT_SEQ",	
	"LT_LOGDATE",
	"LT_ACTION",
	"LT_CLIENTIP",
	"LT_MAPID",
	"LT_DWKEY1",
	"LT_DWKEY2",
	"LT_DWKEY3",
	"LT_DWKEY4",
	"LT_DWKEY5",
	"LT_DWKEY6",
	"LT_DWKEY7",
	"LT_DWKEY8",
	"LT_DWKEY9",
	"LT_DWKEY10",	
	"LT_DWKEY11",
	"LT_KEY1",
	"LT_KEY2",
	"LT_KEY3",
	"LT_KEY4",
	"LT_KEY5",
	"LT_KEY6",
	"LT_KEY7",
	"LT_LOG"
};


int ActionNum[] =
{
	LOG_BASELOGIN	+	0x0000,		//	"Log In"				
	LOG_BASELOGIN	+	0x0004,		//	"Create Charater"		
	LOG_BASELOGIN	+	0x0005,		//	"Delete Charater"		
										
	LOG_BASELOGIN	+	0x0008,		//	"Start Game"			
										
	LOG_BASEMAP		+	0x0010,		//	"Enter Map"				
	LOG_BASEMAP		+	0x0013,		//	"Level Up"				
	LOG_BASEMAP		+	0x0015,		//	"User Die"				
										
	LOG_BASEMAP		+	0x0020,		//	"Mon Spwon"				
	LOG_BASEMAP		+	0x0021,		//	"Mon Kill"				
	LOG_BASEMAP		+	0x0022,		//	"Mon MoneySolo"			
	LOG_BASEMAP		+	0x0023,		//	"Mon MoneyParty"		
	LOG_BASEMAP		+	0x0024,		//	"Mon ExpSolo"			
	LOG_BASEMAP		+	0x0025,		//	"Mon ExpParty"			
										
	LOG_BASEMAP		+	0x0030,		//	"Item ByMonSolo"		
	LOG_BASEMAP		+	0x0031,		//	"Item ByMonParty"		
	LOG_BASEMAP		+	0x0032,		//	"Item ByWorld"			
										
	LOG_BASEMAP		+	0x0034,		//	"Item Delete"			
	LOG_BASEMAP		+	0x0035,		//	"Item Buy"				
	LOG_BASEMAP		+	0x0036,		//	"Item Sell"				
	LOG_BASEMAP		+	0x0037,		//	"Item Use"				
										
	LOG_BASEMAP		+	0x0040,		//	"Item Upgrade Success"	
	LOG_BASEMAP		+	0x0041,		//	"Item Upgrade FaliDel"	
										
	LOG_BASEMAP		+	0x0042,		//	"Item MakeSuccess"		
	LOG_BASEMAP		+	0x0043,		//	"Item MakeZero"			
	LOG_BASEMAP		+	0x0044,		//	"Item MakeFailDel"		
										
	LOG_BASEMAP		+	0x0045,		//	"Item DownGradeSuccess"	
	LOG_BASEMAP		+	0x0046,		//	"Item DownGradeFail"	
										
	LOG_BASEMAP		+	0x0050,		//	"Item Cabinet In"		
	LOG_BASEMAP		+	0x0051,		//	"Item Cabinet Out"		
	LOG_BASEMAP		+	0x0052,		//	"Item Cabinet Make"		
										
	LOG_BASEMAP		+	0x0055,		//	"Item Trade Rec"		
	LOG_BASEMAP		+	0x0056,		//	"Item Trade Send"		
	LOG_BASEMAP		+	0x0057,		//	"Money Trade Rec"		
	LOG_BASEMAP		+	0x0058,		//	"Money Trade Send"		
										
	LOG_BASEMAP		+	0x0060,		//	"Post Item Send"		
	LOG_BASEMAP		+	0x0061,		//	"Post Item Get"			
	LOG_BASEMAP		+	0x0062,		//	"Post Money Send"		
	LOG_BASEMAP		+	0x0063,		//	"Post Money Get"		
										
	LOG_BASEMAP		+	0x0064,		//	"Post Item Del"			
	LOG_BASEMAP		+	0x0065,		//	"Post Money Del"		
										
	LOG_BASEMAP		+	0x0070,		//	"Store Item Sell"		
	LOG_BASEMAP		+	0x0071,		//	"Store Item Buy"		
	LOG_BASEMAP		+	0x0072,		//	"Store Item Del"		
										
	LOG_BASEMAP		+	0x0075,		//	"Item Duration End"		
										
	LOG_BASEMAP		+	0x0076,		//	"Item Repair Success"	
	LOG_BASEMAP		+	0x0077,		//	"Item Refine Del"		
	LOG_BASEMAP		+	0x0078,		//	"Item Refine Success"	
	LOG_BASEMAP		+	0x0079,		//	"Item Refine Fail"		
										
	LOG_BASEMAP		+	0x0080,		//	"Quest Start"			
	LOG_BASEMAP		+	0x0081,		//	"Quest Drop"			
	LOG_BASEMAP		+	0x0082,		//	"Quest Failed"			
	LOG_BASEMAP		+	0x0083,		//	"Quest Success"			
	LOG_BASEMAP		+	0x0084,		//	"Quest ExpGet"			
	LOG_BASEMAP		+	0x0085,		//	"Quest MoneyGet"		
	LOG_BASEMAP		+	0x0086,		//	"Quest ItemGet"			
	LOG_BASEMAP		+	0x0087,		//	"Quest ItemDel"			
										
	LOG_BASEMAP		+	0x0090,		//	"Gamble Success"		
	LOG_BASEMAP		+	0x0091,		//	"Gamble Fail"			
	LOG_BASEMAP		+	0x0092,		//	"Gamble RepFail"		
										
	LOG_BASEMAP		+	0x0095,		//	"Seal Success"			
	LOG_BASEMAP		+	0x0096,		//	"Seal Fail"				
	LOG_BASEMAP		+	0x0097,		//	"Seal RepFail"			
										
	LOG_BASEMAP		+	0x0100,		//	"Skill Buy"				
	LOG_BASEMAP		+	0x0101,		//	"Skill Up"				
										
	LOG_BASEMAP		+	0x0110,		//	"Pet Make"				
	LOG_BASEMAP		+	0x0111,		//	"Pet Delete"			
										
	LOG_BASEMAP		+	0x0120,		//	"Chat Wisper"			
	LOG_BASEMAP		+	0x0121,		//	"Chat Near"				
	LOG_BASEMAP		+	0x0122,		//	"Chat Map"				
	LOG_BASEMAP		+	0x0123,		//	"Chat Wold"				
	LOG_BASEMAP		+	0x0124,		//	"Chat Party"			
	LOG_BASEMAP		+	0x0125,		//	"Chat Guild"			
	LOG_BASEMAP		+	0x0126,		//	"Chat Force"			
	LOG_BASEMAP		+	0x0127,		//	"Chat Operator"			
										
	LOG_BASEMAP		+	0x0130,		//  "Teleport"
	LOG_BASEMAP		+	0x0140,		//	"LocalOccupy"			
	LOG_BASEMAP		+	0x0141,		//	"LocalEnable"			
										
	LOG_BASEMAP		+	0x0150,		//	"PvP Point Gain Char"	
	LOG_BASEMAP		+	0x0151,		//	"PvP Point Use Char"	
	LOG_BASEMAP		+	0x0152,		//	"PvP Point Gain Guild"	
	LOG_BASEMAP		+	0x0153,		//	"PvP Point Use Guild"	
										
	LOG_BASEMAP		+	0x0154,		//	"Auction Reg"			
	LOG_BASEMAP		+	0x0155,		//	"Auction Reg Cancel"	
	LOG_BASEMAP		+	0x0156,		//	"Auction Bid"			
	LOG_BASEMAP		+	0x0157,		//	"Auction Bid Success"	
	LOG_BASEMAP		+	0x0158,		//	"Auction Bid Fail"		
	LOG_BASEMAP		+	0x0159,		//	"Auction Bid Drop"		
	LOG_BASEMAP		+	0x015A,		//	"Auction Buy Direct"	
	LOG_BASEMAP		+	0x015B,		//	"Auction Success Money"	

	LOG_BASEMAP		+	0x01A0,		//	"Cash ItemBuy"			
	LOG_BASEMAP		+	0x01A1,		//	"Cash ItemPresent"		
										
	LOG_BASEMAP		+	0x01A4,		//	"Cash BankIn"			
	LOG_BASEMAP		+	0x01A5,		//	"Cash BankOut"			
										
	LOG_BASEMAP		+	0x01A8,		//	"Cash CabinetBuy"
	LOG_BASEMAP		+	0x01AA,		//	"Cash ChangeChar"		
	LOG_BASEMAP		+	0x01AB,		//	"Cash RandMoney"		
										
	LOG_BASEMAP		+	0x0200,		//	"Guild Create"			
	LOG_BASEMAP		+	0x0201,		//	"Guild Disorganization"	
	LOG_BASEMAP		+	0x0202,		//	"Guild Invite"			
	LOG_BASEMAP		+	0x0203,		//	"Guild InviteAnswer"	
	LOG_BASEMAP		+	0x0204,		//	"Guild Join"			
										
	LOG_BASEMAP		+	0x0205,		//	"Guild Leave"			
	LOG_BASEMAP		+	0x0206,		//	"Guild Kickout"			
										
	LOG_BASEMAP		+	0x0207,		//	"Guild Duty"			
	LOG_BASEMAP		+	0x0208,		//	"Guild Peer"			
										
	LOG_BASEMAP		+	0x0209,		//	"Guild Contribution"	
										
	LOG_BASEMAP		+	0x0210,		//	"Guild LevelUP"			
	LOG_BASEMAP		+	0x0211,		//	"Guild CabinetPutin"	
	LOG_BASEMAP		+	0x0212,		//	"Guild CabinetTakeout"	
										
	LOG_BASEMAP		+	0x0250,		//	"Tournament Apply"		
	LOG_BASEMAP		+	0x0251,		//	"Tournament Evenet"		
	LOG_BASEMAP		+	0x0252,		//	"Tournament Win"

	LOG_BASEMAP		+	0x0253,		//	"RPS Reword"

	LOG_BASEMAP		+	0x0254,		//	"Country Change"		
	LOG_BASEMAP		+	0x0255,		//	"AIDCountry Change"		
										
	LOG_BASEMAP		+	0x0256		//	"Kick Out"				
};																	
																	
int nActionCount = sizeof(ActionNum) / sizeof(ActionNum[0]);		