#pragma once

#include <d3dx9.h>


struct SPickRay
{
	D3DXVECTOR3		m_Position;
	D3DXVECTOR3		m_Direction;
};


class CPicking
{
private :

	SPickRay		m_PickRay;

public :

	CPicking();
	~CPicking();

	static SPickRay		GetRay();
};

