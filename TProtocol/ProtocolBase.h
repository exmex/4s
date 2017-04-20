#pragma once


#define TVERSION							((WORD) 0x1028)	// Protocol version

#define SM_BASE								(0x1000)		// System message base for server
#define MW_BASE								(0x2000)		// Map server <-> World server message base
#define DM_BASE								(0x9000)		// DB message base for server
#define CS_LOGIN							(0x4000)		// Login server <-> Client message base
#define CS_MAP								(0x5000)		// Map server <-> Client message base
#define CT_CONTROL							(0x6000)		// Control Server Base
#define CT_PATCH							(0x7000)		// Patch Server <-> Client
#define RW_RELAY							(0x8000)		// Relay Server <-> Client
