#pragma once


class CTPetDlg : public CTClientUIBase
{
public:
	

protected:
	TComponent*		m_pImage3D;
	TComponent*		m_pKindNameTxt;
	
	BOOL			m_bNeedUpdate;

	CTClientPet*		m_pDisplayPet;
	CD3DDevice*			m_pCDevice;
	LPDIRECT3DDEVICE9	m_pDxDevice;
	CD3DCamera			m_vCamera;

	CD3DLight m_vLights[TLIGHT_COUNT];

public:
	//static CString KindToString(BYTE bKind);
	//static CString KindValueToString(BYTE bKind, WORD wKindValue);
	//static CString CondToString(BYTE bCond);
	//static CString CondValueToString(BYTE bCond, DWORD dwCondValue);

	static CString KindToString(BYTE bKind, WORD wKindValue);
	static CString KindToStringContract(BYTE bKind, WORD wKindValue);
	static CString CondToString(BYTE bCond, DWORD dwCondValue);
	static CString CondToStringContract(BYTE bCond, DWORD dwCondValue);

public:
	virtual void SetDisplayPet(const CString& strName, WORD wPetID, const CTime& tEndTime);
	virtual CTClientPet* GetDisplayPet() const { return m_pDisplayPet; }

	virtual void NotifyUpdate();
	virtual void Update(DWORD dwTickCount);

public:
	virtual HRESULT Render(DWORD dwTickCount);

protected:
	void EnableTLIGHT(CD3DCamera* pCamera, BYTE bENABLE);
	CTClientPet* NewDisplayPet(const CString& strName, WORD wPetID, const CTime& tEndTime);

public:
	CTPetDlg(TComponent* pParent, LP_FRAMEDESC pDesc, CD3DDevice* pDevice);
	virtual ~CTPetDlg();
};