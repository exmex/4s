#pragma once

class CTMonSpawnInspectorRes : public CTachyonRes
{
public:
	CTMonSpawnInspectorRes(void);
	virtual ~CTMonSpawnInspectorRes(void);

	CTachyonMAP* LockMAP(
		DWORD dwID);

	void LockOBJ(
		LPOBJECT pOBJ);

	void LoadMESH(
		CString strGroupID);

	void LoadMESH(
		DWORD dwBASE,
		DWORD dwFileID,
		LPMAPRES pTRES,
		int& nIndex,
		int nTotal);

	void LoadMAP(
		CString strGroupID);

	void LoadOBJ(
		CString strGroupID);

	void LoadOBJ(
		CString strFILE,
		LPMAPRES pTRES,
		int& nIndex,
		int nTotal);
};
