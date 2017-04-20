#pragma once
#include <TachyonHUGEMAP.h>

class CTMonSpawnInspectorHUGEMAP : public CTachyonHUGEMAP
{
public:
	CTMonSpawnInspectorHUGEMAP(void);
	virtual ~CTMonSpawnInspectorHUGEMAP(void);

	int GetUNITID(
		D3DXVECTOR3 vSpawnPos);

	BOOL CheckAvailableLoadUNIT(
		INT nUnitID);

	void LoadUNIT(
		INT nUnitID );

	void UnloadUNIT();

	FLOAT GetHeight(
		FLOAT fPosX,
		FLOAT fPosY,
		FLOAT fPosZ);

public:
	void InitVB(
		CD3DDevice *pDevice);

private:
	void LoadUNIT(
		LPDIRECT3DDEVICE9 pDevice,
		CTachyonEXTMAP *pUNIT,
		int nUnitID);
};
