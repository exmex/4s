//----------------------------------------------------------------------------
// N O L D U S   I N F O R M A T I O N   T E C H N O L O G Y   B . V .
//----------------------------------------------------------------------------
// Filename:      BitmapPickerCombo.cpp
// Project:       EthoVision
// Module:        BitmapPicker
// Programmer:    Anneke Sicherer-Roetman
// Version:       1.00
// Revision Date: 06-10-1999
//----------------------------------------------------------------------------
// Description:   Definition of class CBitmapPickerCombo
//                See CBitmapPickerCombo.h
//----------------------------------------------------------------------------
// Acknowledgements: based on Joel Wahlberg's CIconComboBox
//                  (joel.wahlberg@enator.se)
//----------------------------------------------------------------------------
// Revision history:
// 06-10-1999 - First implementation
//----------------------------------------------------------------------------
// Bugs: ........
//----------------------------------------------------------------------------
// @doc
//----------------------------------------------------------------------------

#include "stdafx.h"
#include "BitmapPickerCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




//----------------------------------------------------------------------------
// Function CBitmapPickerCombo::CBitmapPickerCombo
// @mfunc   constructor
// @xref    <c CBitmapPickerCombo>
//----------------------------------------------------------------------------
// @prog 
// Anneke Sicherer-Roetman
// @revs 
// 06-10-1999 - First implementation
//----------------------------------------------------------------------------
// @todo 
//----------------------------------------------------------------------------
CBitmapPickerCombo::CBitmapPickerCombo():
  CComboBox(),
    m_nItemWidth(0),
    m_nItemHeight(0),
	m_crTextColor(RGB(0,0,0)),
	m_dwBlank(0)
{	
}

CBitmapPickerCombo::~CBitmapPickerCombo()
{
	ClearBitmap();
}
//----------------------------------------------------------------------------
// Function DrawBitmap
// @func    draws bitmap at specified point in specified device context
// @rdesc   nothing
// @parm    const CBitmap | *bitmap | bitmap to draw
// @parm    const CDC     | *pDC    | device context to draw in
// @parm    const CPoint  | &point  | top left point of bitmap
//----------------------------------------------------------------------------
// @prog 
// Anneke Sicherer-Roetman
// @revs 
// 06-10-1999 - First implementation
//----------------------------------------------------------------------------
// @todo 
//----------------------------------------------------------------------------
static void DrawBitmap(const CBitmap *bitmap, const CDC *pDC, const CPoint &point)
{
  BITMAP bm; ((CBitmap*)bitmap)->GetBitmap(&bm);
  int w = bm.bmWidth; 
  int h = bm.bmHeight;
  CDC memDC; memDC.CreateCompatibleDC((CDC*)pDC);
  CBitmap *pBmp = memDC.SelectObject((CBitmap*)bitmap);
  ((CDC*)pDC)->BitBlt(point.x, point.y, w, h, &memDC, 0, 0, SRCCOPY);
  memDC.SelectObject(pBmp);
}

//----------------------------------------------------------------------------
// Function DrawBitmap
// @func    draws bitmap centered in specified rectangle in specified device context
// @rdesc   nothing
// @parm    const CBitmap | *bitmap | bitmap to draw
// @parm    const CDC     | *pDC    | device context to draw in
// @parm    const CRect   | &rect   | rectangle to center in
//----------------------------------------------------------------------------
// @prog 
// Anneke Sicherer-Roetman
// @revs 
// 06-10-1999 - First implementation
//----------------------------------------------------------------------------
// @todo 
//----------------------------------------------------------------------------
static void DrawBitmap(const CBitmap *bitmap, const CDC *pDC, const CRect &rect)
{
  BITMAP bm; ((CBitmap*)bitmap)->GetBitmap(&bm);
  int w = bm.bmWidth; 
  int h = bm.bmHeight;
  CPoint point;
  point.x = rect.left + ((rect.right - rect.left) / 2) - (w / 2);
  point.y = rect.top + ((rect.bottom - rect.top) / 2) - (h / 2);
  DrawBitmap(bitmap, pDC, point);
}


//----------------------------------------------------------------------------
// Function CBitmapPickerCombo::AddBitmap
// @mfunc   adds bitmap (and string) item to combobox
// @rdesc   index of item (-1 on failure) (int)
// @parm    const CBitmap | *bitmap | bitmap to add
// @parm    const char    | *string | string to add (default NULL)
// @xref    <c CBitmapPickerCombo>
//----------------------------------------------------------------------------
// @prog 
// Anneke Sicherer-Roetman
// @revs 
// 06-10-1999 - First implementation
//----------------------------------------------------------------------------
// @todo 
//----------------------------------------------------------------------------
int CBitmapPickerCombo::AddBitmap(const CBitmap *bitmap, LPCTSTR string)
{
	m_vBitmap.push_back(bitmap);
    return InsertBitmap(GetCount(), bitmap, string);
}

//----------------------------------------------------------------------------
// Function CBitmapPickerCombo::InsertBitmap
// @mfunc   adds bitmap (and string) item to combobox at specified index
// @rdesc   index of item (-1 on failure) (int)
// @parm    int           | nIndex  | index at which to insert
// @parm    const CBitmap | *bitmap | bitmap to add
// @parm    const char    | *string | string to add (default NULL)
// @xref    <c CBitmapPickerCombo>
//----------------------------------------------------------------------------
// @prog 
// Anneke Sicherer-Roetman <nl>
// after: Icon Picker Combo Box by Joel Wahlberg <nl>
// http://www.codeguru.com/combobox/icon_combobox.shtml
// @revs 
// 06-10-1999 - First implementation
//----------------------------------------------------------------------------
// @todo 
//----------------------------------------------------------------------------
int CBitmapPickerCombo::InsertBitmap(int nIndex, const CBitmap *bitmap, LPCTSTR string)
{
	CString strText = string;
	FixTextAlignLeft(strText);
	
	TCHAR text[256];
	lstrcpy(text,strText);
	int n = CComboBox::InsertString(nIndex, text ? text : "");
	
	if (n != CB_ERR && n != CB_ERRSPACE) 
	{
		SetItemData(n, (DWORD)bitmap);
		BITMAP bm; 
		((CBitmap*)bitmap)->GetBitmap(&bm);
		SetSize(bm.bmWidth, bm.bmHeight);
	}

	return n;
}

//----------------------------------------------------------------------------
// Function CBitmapPickerCombo::MeasureItem
// @mfunc   Called by MFC when combo box is created
// @rdesc   nothing
// @parm    LPMEASUREITEMSTRUCT | lpMIS | standard parameter
// @xref    <c CBitmapPickerCombo>
//----------------------------------------------------------------------------
// @prog 
// Anneke Sicherer-Roetman
// @revs 
// 06-10-1999 - First implementation
//----------------------------------------------------------------------------
// @todo 
//----------------------------------------------------------------------------
void CBitmapPickerCombo::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{ 
  lpMIS->itemWidth = (m_nItemWidth );
  lpMIS->itemHeight = (m_nItemHeight );
}

//----------------------------------------------------------------------------
// Function CBitmapPickerCombo::DrawItem
// @mfunc   Called by MFC when visual aspect of combo box changes 
// @rdesc   nothing
// @parm    LPDRAWITEMSTRUCT | lpDIS | standard parameter
// @xref    <c CBitmapPickerCombo>
//----------------------------------------------------------------------------
// @prog 
// Anneke Sicherer-Roetman <nl>
// after: Icon Picker Combo Box by Joel Wahlberg <nl>
// http://www.codeguru.com/combobox/icon_combobox.shtml
// @revs 
// 06-10-1999 - First implementation
//----------------------------------------------------------------------------
// @todo 
//----------------------------------------------------------------------------
void CBitmapPickerCombo::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
  CDC* pDC = CDC::FromHandle(lpDIS->hDC);

  if (!IsWindowEnabled()) {
    CBrush brDisabled(RGB(192,192,192)); // light gray
    CBrush* pOldBrush = pDC->SelectObject(&brDisabled);
    CPen penDisabled(PS_SOLID, 1, RGB(192,192,192));
    CPen* pOldPen = pDC->SelectObject(&penDisabled);
    OutputBitmap(lpDIS, false);
    pDC->SelectObject(pOldBrush);
    pDC->SelectObject(pOldPen);
    return;
  }

  // Selected
  if ((lpDIS->itemState & ODS_SELECTED) 
    && (lpDIS->itemAction & (ODA_SELECT | ODA_DRAWENTIRE))) {
    CBrush brHighlight(::GetSysColor(COLOR_HIGHLIGHT)); 
    CBrush* pOldBrush = pDC->SelectObject(&brHighlight);
    CPen penHighlight(PS_SOLID, 1, ::GetSysColor(COLOR_HIGHLIGHT));
    CPen* pOldPen = pDC->SelectObject(&penHighlight);
    pDC->Rectangle(&lpDIS->rcItem);
    pDC->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
    pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
    OutputBitmap(lpDIS, true);
    pDC->SelectObject(pOldBrush);
    pDC->SelectObject(pOldPen);
  }

  // De-Selected
  if (!(lpDIS->itemState & ODS_SELECTED) 
    && (lpDIS->itemAction & (ODA_SELECT | ODA_DRAWENTIRE))) {
    CBrush brWindow(::GetSysColor(COLOR_WINDOW)); 
    CBrush* pOldBrush = pDC->SelectObject(&brWindow);
    CPen penHighlight(PS_SOLID, 1, ::GetSysColor(COLOR_WINDOW));
    CPen* pOldPen = pDC->SelectObject(&penHighlight);
    pDC->Rectangle(&lpDIS->rcItem);
    pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
    pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
    OutputBitmap(lpDIS, false);
    pDC->SelectObject(pOldBrush);
    pDC->SelectObject(pOldPen);
  }

  // Focus
  if (lpDIS->itemAction & ODA_FOCUS) 
    pDC->DrawFocusRect(&lpDIS->rcItem);
}

//----------------------------------------------------------------------------
// Function CBitmapPickerCombo::OutputBitmap
// @mfunc   draws bitmap (and string) in item
// @rdesc   nothing
// @parm    LPDRAWITEMSTRUCT | lpDIS    | item data
// @parm    bool             | selected | is the item selected?
// @xref    <c CBitmapPickerCombo>
//----------------------------------------------------------------------------
// @prog 
// Anneke Sicherer-Roetman <nl>
// after: Icon Picker Combo Box by Joel Wahlberg <nl>
// http://www.codeguru.com/combobox/icon_combobox.shtml
// @revs 
// 06-10-1999 - First implementation
//----------------------------------------------------------------------------
// @todo 
//----------------------------------------------------------------------------
void CBitmapPickerCombo::OutputBitmap(LPDRAWITEMSTRUCT lpDIS, bool selected)
{
  const CBitmap *bitmap = (const CBitmap*)(lpDIS->itemData);
  if (bitmap && bitmap != (const CBitmap *)(0xffffffff)) {
    CDC* pDC = CDC::FromHandle(lpDIS->hDC);
    CString string; 
    if (lpDIS->itemID != -1) 
      GetLBText(lpDIS->itemID, string); 
    if (string.IsEmpty()) 
      DrawBitmap(bitmap, pDC, lpDIS->rcItem);
    else {
      CPoint point;
      point.x = lpDIS->rcItem.left ;
      point.y = lpDIS->rcItem.top + ((lpDIS->rcItem.bottom - lpDIS->rcItem.top) / 2) - (m_nItemHeight / 2) ; 
      DrawBitmap(bitmap, pDC, point);
      CRect rcText(lpDIS->rcItem); 
      //rcText.DeflateRect(m_nItemWidth + 4, 0, 0, 0);
	  pDC->SetTextColor(m_crTextColor);
	  pDC->SetBkMode(TRANSPARENT);
      pDC->DrawText(string, rcText, DT_SINGLELINE |DT_VCENTER ); 
    }
  }
}               

//----------------------------------------------------------------------------
// Function CBitmapPickerCombo::SetSize
// @mfunc   sets overall item size
// @rdesc   nothing
// @parm    int | width  | width of item
// @parm    int | height | height of item
// @xref    <c CBitmapPickerCombo>
//----------------------------------------------------------------------------
// @prog 
// Anneke Sicherer-Roetman
// @revs 
// 06-10-1999 - First implementation
//----------------------------------------------------------------------------
// @todo 
//----------------------------------------------------------------------------
void CBitmapPickerCombo::SetSize(int width, int height)
{
  if (width > m_nItemWidth)
    m_nItemWidth = width;
  if (height > m_nItemHeight)
    m_nItemHeight = height;
  for (int i = -1; i < GetCount(); i++) 
    SetItemHeight(i, m_nItemHeight); // 줄간격 설정
}

//----------------------------------------------------------------------------

#ifdef _DEBUG
void CBitmapPickerCombo::PreSubclassWindow() 
{
  CComboBox::PreSubclassWindow();

  // ensure some styles are set
  // modifying style here has NO effect!?!
  ASSERT(GetStyle() & CBS_DROPDOWNLIST);
  ASSERT(GetStyle() & CBS_OWNERDRAWVARIABLE);
  ASSERT(GetStyle() & CBS_HASSTRINGS);
}
#endif

//----------------------------------------------------------------------------

void CBitmapPickerCombo::SetTextColor_(COLORREF crColor)
{
	m_crTextColor = crColor;
}

void CBitmapPickerCombo::SetFont_(CString strFont, DWORD dwSize)
{
	//1.
	//CFont Font;
	//Font.CreateFont( dwSize,0,0,0,0, 0, 0, FALSE,DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, OUT_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH, strFont);
	//SetFont(&Font);

	//2.
	LOGFONT lf;
	ZeroMemory(&lf, sizeof(lf));
	lf.lfHeight = dwSize;
	strcpy(lf.lfFaceName, strFont);
	HWND hFont = (HWND)CreateFontIndirect(&lf);
	SendMessage(WM_SETFONT, (WPARAM)hFont, (LPARAM)TRUE);
	
}

void CBitmapPickerCombo::SetTextLeftBlank(DWORD dwBlank)
{
	m_dwBlank = dwBlank;
}

void CBitmapPickerCombo::FixTextAlignLeft(CString& str)
{	
	if( str == _T("") )
		return;

	CString strText=_T("");
	CString strBlank =_T(" ");
	for(DWORD i = 0; i < m_dwBlank; i++)	
		strText = strText + strBlank;
	
	str = strText + str;
}

void CBitmapPickerCombo::ClearBitmap()
{
	while( (DWORD)m_vBitmap.size() > 0 )
	{
		const CBitmap* pBitmap = m_vBitmap.back();
        delete pBitmap;
		pBitmap = NULL;

		m_vBitmap.pop_back();
	}
	m_vBitmap.clear();
}

BOOL CBitmapPickerCombo::LoadBMPImage( LPCTSTR sBMPFile, CBitmap* bitmap, CPalette *pPal )
{
	CFile file;
	if( !file.Open( sBMPFile, CFile::modeRead) )
		return FALSE;

	BITMAPFILEHEADER bmfHeader;

	// Read file header
	if (file.Read((LPSTR)&bmfHeader, sizeof(bmfHeader)) != sizeof(bmfHeader))
		return FALSE;

	// File type should be 'BM'
	if (bmfHeader.bfType != ((WORD) ('M' << 8) | 'B'))
		return FALSE;

	// Get length of the remainder of the file and allocate memory
	DWORD nPackedDIBLen = file.GetLength() - sizeof(BITMAPFILEHEADER);
	HGLOBAL hDIB = ::GlobalAlloc(GMEM_FIXED, nPackedDIBLen);
	if (hDIB == 0)
		return FALSE;

	// Read the remainder of the bitmap file.
	//if (file.ReadHuge((LPSTR)hDIB, nPackedDIBLen) != nPackedDIBLen )
	if (file.Read((LPSTR)hDIB, nPackedDIBLen) != nPackedDIBLen )
	{
		::GlobalFree(hDIB);
		return FALSE;
	}


	BITMAPINFOHEADER &bmiHeader = *(LPBITMAPINFOHEADER)hDIB ;
	BITMAPINFO &bmInfo = *(LPBITMAPINFO)hDIB ;

	// If bmiHeader.biClrUsed is zero we have to infer the number
	// of colors from the number of bits used to specify it.
	int nColors = bmiHeader.biClrUsed ? bmiHeader.biClrUsed : 
	1 << bmiHeader.biBitCount;

	LPVOID lpDIBBits;
	if( bmInfo.bmiHeader.biBitCount > 8 )
		lpDIBBits = (LPVOID)((LPDWORD)(bmInfo.bmiColors + bmInfo.bmiHeader.biClrUsed) + 
		((bmInfo.bmiHeader.biCompression == BI_BITFIELDS) ? 3 : 0));
	else
		lpDIBBits = (LPVOID)(bmInfo.bmiColors + nColors);

	// Create the logical palette
	if( pPal != NULL )
	{
		// Create the palette
		if( nColors <= 256 )
		{
			UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * nColors);
			LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];

			pLP->palVersion = 0x300;
			pLP->palNumEntries = nColors;

			for( int i=0; i < nColors; i++)
			{
				pLP->palPalEntry[i].peRed = bmInfo.bmiColors[i].rgbRed;
				pLP->palPalEntry[i].peGreen = bmInfo.bmiColors[i].rgbGreen;
				pLP->palPalEntry[i].peBlue = bmInfo.bmiColors[i].rgbBlue;
				pLP->palPalEntry[i].peFlags = 0;
			}

			pPal->CreatePalette( pLP );

			delete[] pLP;
		}
	}

	CClientDC dc(NULL);
	CPalette* pOldPalette = NULL;
	if( pPal )
	{
		pOldPalette = dc.SelectPalette( pPal, FALSE );
		dc.RealizePalette();
	}

	HBITMAP hBmp = CreateDIBitmap( dc.m_hDC,                // handle to device context 
		&bmiHeader,     // pointer to bitmap size and format data 
		CBM_INIT,       // initialization flag 
		lpDIBBits,      // pointer to initialization data 
		&bmInfo,        // pointer to bitmap color-format data 
		DIB_RGB_COLORS);                // color-data usage 
	bitmap->Attach( hBmp );

	if( pOldPalette )
		dc.SelectPalette( pOldPalette, FALSE );

	::GlobalFree(hDIB);
	//                 bitmap->Attach(hBmp); // bug fix by Jeremy Blake
	return TRUE;
}

