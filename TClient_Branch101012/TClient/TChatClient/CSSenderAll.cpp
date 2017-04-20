#include "stdafx.h"
#include "TChatClient.h"

#include "TChatClientDoc.h"
#include "TChatClientView.h"


void CTChatClientView::SendCS_CONREADY_REQ( BYTE bServerID)
{
	CPacket vMSG;

	vMSG.SetID(CS_CONREADY_REQ);
	SayToSvr( vMSG, bServerID);
}
