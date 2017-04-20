#pragma once

#include "ProtocolBase.h"

// Common system message for server
#define SM_DELSESSION_REQ					(SM_BASE + 0x0001)	// Close session request (see remark)
// =======================================
// Remark (Very important)
// =======================================
// This command will delete PACKETBUF->m_pSESSION
// Session delete step :
// 1. Complete I/O operation (Clear IOCP queue associated this session)
// 2. Send SM_DELSESSION_REQ to batch thread (Clear batch queue associated this session)

#define SM_QUITSERVICE_REQ					(SM_BASE + 0x0002) // Stop server

#define SM_AICMD_REQ						(SM_BASE + 0x0003)
// DWORD dwAICmdHandle
// DWORD dwHostKEY
// DWORD dwEventHost
// DWORD dwDelay
// DWORD dwMonID
// BYTE bChannel
// WORD wMapID
// WORD wPartyID

#define SM_AICMD_ACK						(SM_BASE + 0x0004)
// DWORD dwAICmdHandle
// DWORD dwHostKEY
// DWORD dwEventHost
// DWORD dwMonID
// BYTE bChannel
// WORD wMapID
// WORD wPartyID

#define SM_LOCALENABLE_REQ					(SM_BASE + 0x0005)
#define SM_CASTLEENABLE_REQ					(SM_BASE + 0x0006)

#define SM_SWITCHCHANGE_REQ					(SM_BASE + 0x0007)
#define SM_ADDSELFOBJ_REQ					(SM_BASE + 0x0008)
#define SM_DELSELFOBJ_REQ					(SM_BASE + 0x0009)
#define SM_DELSELFOBJ_ACK					(SM_BASE + 0x000A)
#define SM_SWITCHSTART_REQ					(SM_BASE + 0x000B)

#define SM_GUILDDISORGANIZATION_REQ			(SM_BASE + 0x000C)
// DWORD dwGuildID
// DWORD dwTime
// BYTE bDisorg
#define SM_POSTBILL_REQ						(SM_BASE + 0x000D)
#define SM_POSTBILL_ACK						(SM_BASE + 0x000E)
#define SM_POSTBILLERASE_REQ				(SM_BASE + 0x000F)

// ∞·≈ı
#define SM_DUELSTART_REQ					(SM_BASE + 0x0010)
#define SM_DUELSTART_ACK					(SM_BASE + 0x0011)
#define SM_DUELEND_REQ						(SM_BASE + 0x0012)
#define SM_DUELEND_ACK						(SM_BASE + 0x0013)
#define SM_DUELCOMPLETE_ACK					(SM_BASE + 0x0014)

#define SM_RESETHOST_REQ					(SM_BASE + 0x0015)
#define SM_RESETHOST_ACK					(SM_BASE + 0x0016)

#define SM_EVENTQUARTER_REQ					(SM_BASE + 0x0017)
#define SM_EVENTQUARTERNOTIFY_REQ			(SM_BASE + 0x0018)
#define SM_GODBALLCMD_REQ					(SM_BASE + 0x0019)
#define SM_GODBALLCMD_ACK					(SM_BASE + 0x001A)
#define SM_TIMER_REQ						(SM_BASE + 0x001B)
#define SM_BATTLESTATUS_REQ					(SM_BASE + 0x001C)

#define SM_APEXDATA_REQ						(SM_BASE + 0x001D)
#define SM_APEXDATA_ACK						(SM_BASE + 0x001E)
#define SM_APEXKILLUSER_REQ					(SM_BASE + 0x001F)
#define SM_APEXKILLUSER_ACK					(SM_BASE + 0x0020)

#define SM_AUCTIONCMD_REQ					(SM_BASE + 0x0021)
#define SM_AUCTIONCMDDEL_REQ				(SM_BASE + 0x0022)

#define SM_EVENTEXPIRED_REQ					(SM_BASE + 0x0023)
#define SM_EVENTEXPIRED_ACK					(SM_BASE + 0x0024)

#define SM_MONTHRANKSAVE_REQ				(SM_BASE + 0x0025)
#define SM_TOURNAMENT_REQ					(SM_BASE + 0x0026)
#define SM_TOURNAMENTUPDATE_REQ				(SM_BASE + 0x0027)
#define SM_TOURNAMENTEVENT_REQ				(SM_BASE + 0x0028)
#define SM_TOURNAMENTEVENT_ACK				(SM_BASE + 0x0029)
#define SM_MEETINGROOM_REQ					(SM_BASE + 0x002A)
#define SM_MEETINGROOM_ACK					(SM_BASE + 0x002B)
#define SM_CHANGEDAY_REQ					(SM_BASE + 0x002C)
