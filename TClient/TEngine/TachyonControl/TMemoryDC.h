#if (!defined __TMEMORYDC_H)
#define __TMEMORYDC_H

#if (!defined __TMFC_DEF_H)
	#include <TMfcDef.h>
#endif // __TMFC_DEF_H

class __TUI_API CTMemoryDC : public CDC
{
protected:
	CBitmap *m_pOldBmp;
	CBitmap m_bmp;

	CRect m_rect;
	CDC *m_pDC;

	COLORREF *m_pHelperDibSurface;
	BOOL m_bMemDC;

public:
	enum eOptFlags
	{
		MDCOPT_TO_MEMORY	= 0x001L,
		MDCOPT_FILL_SURFACE	= 0x002L,
		MDCOPT_FORCE_BLACK	= 0x004L,
		MDCOPT_FORCE_DIB	= 0x008L,
		MDCOPT_NO_COPY_OPT	= 0x010L,
		MDCOPT_FILL_BITS	= 0x020L,
		MDCOPT_DEFAULT = MDCOPT_TO_MEMORY|MDCOPT_FILL_SURFACE
	};

	void __InitMemoryDC(
		CDC *pDC,
		const CRect *pRect = NULL,
		DWORD dwOptFlags = MDCOPT_DEFAULT)
	{
		m_pOldBmp = NULL;
		m_pDC = pDC;

		if(dwOptFlags&MDCOPT_TO_MEMORY)
			m_bMemDC = !pDC->IsPrinting();
		else
			m_bMemDC = FALSE;

		if(!pRect)
			pDC->GetClipBox(&m_rect);
		else 
			m_rect = *pRect;

		if(m_bMemDC)
		{
			CreateCompatibleDC(pDC);

			if(!(dwOptFlags&MDCOPT_NO_COPY_OPT))
				pDC->LPtoDP(&m_rect);

			if(dwOptFlags&MDCOPT_FORCE_BLACK)
			{
				m_bmp.CreateBitmap(
					m_rect.Width(),
					m_rect.Height(),
					1, 1,
					NULL);
			}
			else if(dwOptFlags&MDCOPT_FORCE_DIB)
			{
				BITMAPINFOHEADER bih;

				bih.biSize = sizeof(BITMAPINFOHEADER);
				bih.biWidth = m_rect.Width();
				bih.biHeight = m_rect.Height();
				bih.biPlanes = 1;
				bih.biBitCount = 32;
				bih.biCompression = BI_RGB;
				bih.biSizeImage = bih.biWidth*bih.biHeight;
				bih.biXPelsPerMeter = 0;
				bih.biYPelsPerMeter = 0;
				bih.biClrUsed = 0;
				bih.biClrImportant = 0;

				HBITMAP hDIB = ::CreateDIBSection(
					pDC->GetSafeHdc(),
					(LPBITMAPINFO) &bih,
					DIB_RGB_COLORS,
					(void **) &m_pHelperDibSurface,
					NULL,
					NULL);

				if( !hDIB || !m_pHelperDibSurface )
					return;

				m_bmp.Attach(hDIB);
			}
			else
			{
				m_bmp.CreateCompatibleBitmap(
					pDC,
					m_rect.Width(),
					m_rect.Height());
			}

			m_pOldBmp = SelectObject(&m_bmp);
			if(!(dwOptFlags&MDCOPT_NO_COPY_OPT))
			{
				SetMapMode(pDC->GetMapMode());
				pDC->DPtoLP(&m_rect);

				SetWindowOrg( m_rect.left, m_rect.top);
			}
		}
		else
		{
			if(pDC->IsPrinting())
				m_bPrinting = pDC->m_bPrinting;

			m_hDC = pDC->m_hDC;
			m_hAttribDC = pDC->m_hAttribDC;
		}

		if(dwOptFlags&MDCOPT_FILL_BITS)
		{
			BitBlt(
				0, 0,
				m_rect.Width(),
				m_rect.Height(),
				pDC,
				m_rect.left,
				m_rect.top,
				SRCCOPY);
		}
		else if(dwOptFlags&MDCOPT_FILL_SURFACE)
			FillSolidRect( m_rect, pDC->GetBkColor());
	};

	void __Flush( BOOL bMoveImage = TRUE)
	{
		if(!m_pDC)
			return;

		if(m_bMemDC)
		{
			if(bMoveImage)
			{
				m_pDC->BitBlt(
					m_rect.left,
					m_rect.top,
					m_rect.Width(),
					m_rect.Height(),
					this,
					m_rect.left,
					m_rect.top,
					SRCCOPY);
			}

			SelectObject(m_pOldBmp);
		}
		else
			m_hDC = m_hAttribDC = NULL;

		m_pHelperDibSurface = NULL;
		m_rect.SetRectEmpty();
		m_pOldBmp = NULL;
		m_bMemDC = FALSE;
		m_pDC = NULL;
	};

	CTMemoryDC( CDC *pDC = NULL, const CRect *pRect = NULL, DWORD dwOptFlags = MDCOPT_DEFAULT) : CDC()
	{
		m_pHelperDibSurface = NULL;
		m_pOldBmp = NULL;
		m_pDC = NULL;

		m_rect.SetRectEmpty();
		if(pDC)
		{
			__InitMemoryDC(
				pDC,
				pRect,
				dwOptFlags);
		}
	};

	~CTMemoryDC()
	{
		__Flush();
	};

	CRect &__GetDrawRect()
	{
		return m_rect;
	};

	const CRect &__GetDrawRect() const
	{
		return m_rect;
	};

	BOOL __IsMemDC() const
	{
		return m_bMemDC;
	}

	CTMemoryDC* operator->()
	{
		return this;
	};

	operator CTMemoryDC*()
	{
		return this;
	};

	COLORREF *GetDibSurface()
	{
		return m_pHelperDibSurface;
	};

	const COLORREF *GetDibSurface() const
	{
		return m_pHelperDibSurface;
	};

#if (defined __TPAINTMANAGER_H)
	void __TransparentBltFrom(
		CDC &dcFrom,
		int nXOriginDest,
		int nYOriginDest,
		int nWidthDest,
		int hHeightDest,
		int nXOriginSrc,
		int nYOriginSrc,
		int nWidthSrc,
		int nHeightSrc,
		COLORREF crTransparent)
	{
		CTPaintManager::TransparentBlt(
			GetSafeHdc(),
			nXOriginDest,
			nYOriginDest,
			nWidthDest,
			hHeightDest,
			dcFrom.GetSafeHdc(),
			nXOriginSrc,
			nYOriginSrc,
			nWidthSrc,
			nHeightSrc,
			crTransparent);
	};

	void __TransparentBltTo(
		CDC &dcTo,
		int nXOriginDest,
		int nYOriginDest,
		int nWidthDest,
		int hHeightDest,
		int nXOriginSrc,
		int nYOriginSrc,
		int nWidthSrc,
		int nHeightSrc,
		COLORREF crTransparent)
	{
		CTPaintManager::TransparentBlt(
			dcTo.GetSafeHdc(),
			nXOriginDest,
			nYOriginDest,
			nWidthDest,
			hHeightDest,
			GetSafeHdc(),
			nXOriginSrc,
			nYOriginSrc,
			nWidthSrc,
			nHeightSrc,
			crTransparent);
	};
#endif // #if (defined __TPAINTMANAGER_H)
};


#endif // #if (!defined __TMEMORYDC_H)