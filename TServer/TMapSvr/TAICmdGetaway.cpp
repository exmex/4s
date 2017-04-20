#include "StdAfx.h"


CTAICmdGetaway::CTAICmdGetaway()
{
}

CTAICmdGetaway::~CTAICmdGetaway()
{
}

DWORD CTAICmdGetaway::GetDelay( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType)
{
	return m_dwDelay;
}

BYTE CTAICmdGetaway::CanRun( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType)
{
	if( pMON->m_bStatus != OS_WAKEUP)
		return FALSE;

	return CTAICommand::CanRun( pMON, dwEventHost, dwRHId, bRHType);
}

BYTE CTAICmdGetaway::ExecAI( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType)
{
	CTPlayer *pHOST = pMON->FindHost(pMON->m_dwHostID);
	BYTE bRet = TRUE;

	if(pHOST)
	{
		pMON->m_dwHostKEY++;
		CTMonster * pHelper = NULL;

		FLOAT fRad = FLOAT(rand() % 360) * FLOAT(M_PI) / 180.0f;
		FLOAT fPosX = pMON->m_fStartX + pMON->m_pSPAWN->m_pSPAWN->m_bArea * cosf(fRad);
		FLOAT fPosY = pMON->m_fStartY;
		FLOAT fPosZ = pMON->m_fStartZ + pMON->m_pSPAWN->m_pSPAWN->m_bArea * sinf(fRad);

		if(pMON->m_dwHelper)
		{
			pHelper = pMON->m_pMAP->FindMonster(pMON->m_dwHelper);
			if(pHelper && pHelper->m_bMode == MT_NORMAL)
			{
				FLOAT fDifX = abs(pHelper->m_fPosX-pMON->m_fPosX);
				FLOAT fDifZ = abs(pHelper->m_fPosZ-pMON->m_fPosZ);
				if(fDifX <= 2.5 && fDifZ <= 2.5)
				{
					pMON->m_dwHelper = 0;
					pMON->m_bGetaway = FALSE;
					pMON->OnEvent(AT_DEFEND, 0, pMON->m_dwHostID, pMON->m_dwTargetID, pMON->m_bTargetType);
					pHelper->OnEvent(AT_DEFEND, 0, pMON->m_dwHostID, pMON->m_dwTargetID, pMON->m_bTargetType);
					return FALSE;
				}
				else
				{
					fPosX = pHelper->m_fPosX;
					fPosY = pHelper->m_fPosY;
					fPosZ = pHelper->m_fPosZ;
				}
			}
			else
			{
				pMON->m_dwHelper = 0;
				pHelper = NULL;
				bRet = FALSE;
			}
		}
		else
		{
			if(pMON->m_pMON->m_bCall)
			{
				VTMONSTER vHelper;
				pMON->m_pMAP->GetNeighbor(&vHelper, pMON->m_fPosX, pMON->m_fPosZ);

				CTMonster * pNew;
				while(vHelper.size())
				{
					pNew = vHelper.back();
					vHelper.pop_back();
					if(pNew->m_pMON->m_wKind != pMON->m_pMON->m_wKind ||
						pNew->m_bMode != MT_NORMAL ||
						GetDistance(pMON->m_fPosX, pMON->m_fPosZ, pNew->m_fPosX, pNew->m_fPosZ) > 30)
						continue;

					if(!pHelper)
						pHelper = pNew;
					else
					{
						FLOAT fOldDiff = abs(pHelper->m_fPosX-pMON->m_fPosX)+abs(pHelper->m_fPosZ-pMON->m_fPosZ);
						FLOAT fNewDiff = abs(pNew->m_fPosX-pMON->m_fPosX)+abs(pNew->m_fPosZ-pMON->m_fPosZ);
						if(fOldDiff > fNewDiff)
							pHelper = pNew;
					}
				}

				if(!pHelper)
					bRet = FALSE;

				vHelper.clear();
			}
			else
			{
				fPosX = pMON->m_fPosX + pMON->m_pSPAWN->m_pSPAWN->m_bArea * cosf(fRad);
				fPosY = pMON->m_fPosY;
				fPosZ = pMON->m_fPosZ + pMON->m_pSPAWN->m_pSPAWN->m_bArea * sinf(fRad);

				if(pMON->CheckAttack())
					bRet = FALSE;
			}

			if(pHelper)
			{
				pMON->m_dwHelper = pHelper->m_dwID;
				pMON->m_bHelpCount++;
				fPosX = pHelper->m_fPosX;
				fPosY = pHelper->m_fPosY;
				fPosZ = pHelper->m_fPosZ;
			}
		}

		if(bRet)
		{
			pHOST->SendCS_MONACTION_ACK(
				pMON->m_dwID,
				TA_RUN,
				fPosX,
				fPosY,
				fPosZ);
		}
		else
		{
			pMON->m_bGetaway = FALSE;
			pMON->OnEvent(AT_DEFEND, 0, pMON->m_dwHostID, pMON->m_dwTargetID, pMON->m_bTargetType);
			return FALSE;
		}
	}
	else
		return FALSE;

	return CTAICommand::ExecAI( pMON, dwEventHost, dwRHId, bRHType);
}
