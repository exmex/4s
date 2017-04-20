// AddFarImageDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "AddFarImageDlg.h"
#include ".\addfarimagedlg.h"


// CAddFarImageDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAddFarImageDlg, CDialog)
CAddFarImageDlg::CAddFarImageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddFarImageDlg::IDD, pParent)
{
}

CAddFarImageDlg::~CAddFarImageDlg()
{
}

void CAddFarImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAddFarImageDlg, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CAddFarImageDlg 메시지 처리기입니다.

void CAddFarImageDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.
	CRect rect;
	CPen pen, *pOldPen;
	int nR = 255, nG = 255, nB = 255;
	int nCX = 100, nCY = 100;
	float fX, fY;

	GetClientRect(rect);

	CreatePen( PS_SOLID, 1, RGB(nR,nG,nB) );

	pOldPen = (CPen*)dc.SelectObject(&pen);

	for( DWORD i = 0 ; i < 33 ; i++ )
	{
		FLOAT fTheta = ( 2 * D3DX_PI * i ) / ( 33 - 1 );
		fX = 100.0f + sinf(fTheta) * 100;
		fY = 100.0f + cosf(fTheta) * 100;
		dc.MoveTo(120, 120);
		if (i != 32) 
		{
			dc.LineTo( INT(fX + 20), INT(fY + 20) );
			CString strCnt;
			strCnt.Format( "%d", i );
			dc.TextOut( INT(fX + 20)  - 5, INT(fY + 20) - 5, strCnt,strCnt.GetLength() );
		}
	}
}
