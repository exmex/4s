#pragma once

#include <vector>
#include <d3dx9.h>

#define  GIZMO_NONE			((BYTE) 0x00)
#define  GIZMO_ROTATE		((BYTE) 0x01)
#define  GIZMO_TRANSLATE	((BYTE) 0x02)
#define  GIZMO_SCALE		((BYTE) 0x03)

class CGizmoState;
class CD3DCamera;

class CGizmo
{
public :

	CGizmo( HWND hwnd, CD3DCamera* pCamera );
	~CGizmo(void);
	
private :

	BYTE				m_bGizmoState;			// Gizmo State
	CGizmoState*		m_pGizmo;				// Gizmo
	BOOL				m_bMouse;				// Mouse Move Flag


public :

	static LPDIRECT3DDEVICE9	m_pDevice;
	static HWND					m_hWnd;
	static CD3DCamera*			m_pCamera;
	
	


public :
	
	//////////////////////////////////////////////////////////////////////////
	// Completed
	
	
	//////////////////////////////////////////////////////////////////////////
	// Not Completed 

	void				SetGizemo( CGizmoState* pGizmo );			// Gizmo Setting
	void				BeginMouseMove();							// Mouse Move Start
	void				MouseMove();								// Mouse Move
	void				EndMouseMove();								// Mouse Move End

	void				Draw();										// Gizmo Draw




};

//////////////////////////////////////////////////////////////////////////

class CGizmoState
{
public :

	CGizmoState(void);
	virtual			~CGizmoState(void)	{ }

public :

	virtual	void		Update() = 0;
	virtual	void		Draw() = 0;
	virtual BOOL		CheckIntersect() = 0;


public : 




};



class CGizmoRotate : public CGizmoState
{
public :

	CGizmoRotate(void);
	~CGizmoRotate(void);

public :

	void			Update();
	void			Draw();
	BOOL			CheckIntersect();

};


class CGizmoTranslate : public CGizmoState
{
public :

	CGizmoTranslate(void);
	~CGizmoTranslate(void);

public :

	void			Update();
	void			Draw();
	BOOL			CheckIntersect();
};

//class CGizmoScale : public CGizmoState
//{
//
//};




