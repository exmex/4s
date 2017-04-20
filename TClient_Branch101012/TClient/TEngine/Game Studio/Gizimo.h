#pragma once

#include <d3dx9.h>
#include <vector>


class CGizimoState;
class CD3DCamera;


//!	Gizimo Class.
/*!
*	@brief	이 클래스의 인스턴스를 생성해서 사용.
*
*/
class CGizimo
{
private :

	CGizimoState*				m_pGizimo;

public :

	static IDirect3DDevice9*	m_pDevice;
	static HWND					m_hWnd;
	static CD3DCamera*			m_pCamera;

	CGizimo( HWND hwnd, CD3DCamera* pCamera );
	~CGizimo();

	CGizimoState*				GetCurrentState();
	void						ChangeGizimo( CGizimoState* pGizimo );
	void						Update( D3DXVECTOR3* vecObject );
};


struct GPLAG
{
	union
	{
		struct
		{
			int x : 1;
			int y : 1;
			int z : 1;
		};

		int val;
	};

	GPLAG( int _val = 0 )
		:	val( _val )
	{}
};


struct GLINE
{
	std::vector< D3DXVECTOR3 >	VtxList;
	std::vector< D3DXVECTOR3 >	BBoxMin;
	std::vector< D3DXVECTOR3 >	BBoxMax;
	D3DCOLOR					Color;

	GLINE()
		:	Color( D3DCOLOR_ARGB(255,255,255,255) )
	{
	}

	void Release()
	{
		VtxList.clear();
		BBoxMin.clear();
		BBoxMax.clear();
	}
};

//!	라인매니져.
/*!
*	@brief	기지모를 구성할 라인들을 관리하는 클래스.
*
*/
class CGizimoLine
{
private :

	ID3DXLine*					m_pLine;

public :

	CGizimoLine();
	~CGizimoLine();

	static CGizimoLine*			GetInstance();
	void						InitInstance();
	void						Draw( D3DXVECTOR3* vecObject, GLINE& line );
	void						Release();
};


//!	마우스컨트롤.
class CGizimoMouseController
{
private :

	bool						m_bDrag;
	D3DXVECTOR3					m_PrevPos;

public :

	static CGizimoMouseController*		GetInstance();

	void						BeginDrag();
	D3DXVECTOR3					Drag( D3DXPLANE* plane, D3DXVECTOR3* start, D3DXVECTOR3* end );
	void						EndDrag();

	bool						GetDrag() const;
};


//!	gizimo 상태클래스.
class CGizimoState
{
protected :

	D3DXVECTOR3*				m_vecObject;
	std::vector< GLINE >		m_vLine;
	GPLAG						m_SelectedAxis;

protected :

	virtual void				UpdateX()	{}
	virtual void				UpdateY()	{}
	virtual void				UpdateXY()	{}
	virtual void				UpdateZ()	{}
	virtual void				UpdateXZ()	{}
	virtual void				UpdateYZ()	{}
	virtual void				UpdateXYZ()	{}

public :

	CGizimoState();
	virtual						~CGizimoState()	{}

	virtual void				Enter()		= 0;
	virtual void				Excute( D3DXVECTOR3* vecObject );
	virtual void				Exit()		= 0;
};


class CGizimoTranslate : public CGizimoState
{
private :

	D3DXPLANE					m_AxisPlane[3];
	POINT						m_mousePt;

protected :

	void						UpdateX();
	void						UpdateY();
	void						UpdateXY();
	void						UpdateZ();
	void						UpdateXZ();
	void						UpdateYZ();

public :

	CGizimoTranslate();
	~CGizimoTranslate();

	static CGizimoTranslate*	GetInstance();

	void						Enter();
	void						Excute( D3DXVECTOR3* vecObject );
	void						Exit();
};

//
//class CGizimoRotate : public CGizimoState
//{
//public :
//
//	static CGizimoRotate*		GetInstance();
//};
//
//class CGizimoScale : public CGizimoState
//{
//
//};

