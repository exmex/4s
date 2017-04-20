// TImageList.h: interface for the TImageList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMAGELIST_H__32028F19_B866_4E3C_8120_CB3D8EE18F87__INCLUDED_)
#define AFX_TIMAGELIST_H__32028F19_B866_4E3C_8120_CB3D8EE18F87__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// =====================================================================
/**	@class		TImageList
	@brief		복수의 이미지를 유지하며 한번에 하나의 이미지를 선택 할 
				수 있는 컴포넌트이다.
	
*/// ===================================================================
class TImageList : public TComponent  
{
public:
	/// 이미지리스트를 대표하는 클래스이다.
	class TDelegate : public TComponent
	{
	public:
		/// 주어진 이미지리스트의 CurIdx를 참조하여 렌더한다.
		TComponent* RenderImgList( TImageList* pImgList, DWORD dwCurTick, DWORD dwDeltaTick);
		/// 주어진 이미지(pImgComp)를 참조하여 렌더한다.
		void RenderImgComp( TImageList* pImgList, TComponent* pImgComp, DWORD dwCurTick, DWORD dwDeltaTick);
		/// 주어진 이미지리스트를 참조하여 영역검사를 수행한다.
		BOOL HitRectDelegate( TImageList* pImgList, CPoint pt);
		/// 주어진 이미지리스트를 참조하여 충돌검사를 수행한다.
		BOOL HitTestDelegate( TImageList* pImgList, CPoint pt);
		/// 이미지의 전체 갯수를 얻는다.
		int GetImageCount();
		/// 이미지 얻기
		TComponent* GetImage( int nIndex );

	public:
		TDelegate(LP_FRAMEDESC pDesc);
		~TDelegate();
	};

	/// 대표자 인스턴스를 관리하기위한 클래스이다.
	class TDelegateManager
	{
	public:
		struct InstKey
		{
			LP_FRAMEDESC	m_pDesc;

			InstKey(LP_FRAMEDESC pDesc)
			:	 m_pDesc(pDesc)
			{}

			bool operator < (const InstKey& r) const;
		};

		struct InstData
		{
			int				m_nRefCnt;	///< 참조 카운트
			TDelegate*		m_pInst;	///< 대표자

			InstData(TDelegate* pInst)
			:	 m_nRefCnt(1),
				 m_pInst(pInst)
			{}
		};
		
	private:
		typedef std::map<InstKey, InstData> TDELEGATEMAP;

	protected:
		/// LP_FRAMEDESC 로 대표자를 관리하기위한 맵.
		TDELEGATEMAP m_mapDelegates;

	public:
		TDelegate* NewInstance(LP_FRAMEDESC pDesc);
		void ReleaseInstance(TDelegate* pInst);
	};

protected:
	/// 대표 인스턴스의 관리자
	static TDelegateManager m_Manager;

protected:
	int			m_nCurIdx;		///< 현재 선택된 이미지의 인덱스
	int			m_nLastIdx;		///< 가장 최근에 그려진 이미지의 인덱스
	TComponent*	m_pLastImg;		///< 가장 최근에 그려진 이미지
	TComponent*	m_pSkinImg;		///< 스킨
	
	BOOL		m_bUserColor;	///< 유저가 정한 색을 사용할지 여부
	DWORD		m_dwUserColor;	///< 유저가 정한 색
	BOOL		m_bUsePixelHitTest; ///< 픽셀단위로 HitTest 검사 여부

	TDelegate*	m_pDelegate;	///< 대표 인스턴스

protected:
	virtual void BuildKid( LP_FRAMEDESC pDesc );
	
public:
	virtual BOOL HitRect( CPoint pt);
	virtual BOOL HitTest( CPoint pt);
	virtual HRESULT Render( DWORD dwTickCount);
	
public:
	BOOL EndOfImgs( TCOMP_LIST::iterator it );
	TCOMP_LIST::iterator GetFirstImgsFinder();
	TComponent* GetNextImg( TCOMP_LIST::iterator &it );

protected:
	void InitRect();

public:
	void SetCurImage(int nIndex)		{ m_nCurIdx = nIndex; }
	int GetCurImage()					{ return m_nCurIdx; }

	void EnableUserColor(BOOL bEnable)	{ m_bUserColor = bEnable; }
	BOOL IsUserColorEnabled()			{ return m_bUserColor; }

	void SetUserColor(DWORD dwColor)	{ m_dwUserColor = dwColor; }
	DWORD GetUserColor()				{ return m_dwUserColor; }

	int GetImageCount()					{ return m_pDelegate->GetImageCount(); }
	TDelegate* GetDelegate()			{ return m_pDelegate; }

	void SetPixelHitTest(BOOL bUse)	{ m_bUsePixelHitTest = bUse; }

	void SetSkinImage( int nIndex );
	void SetSkinImageEmpty();

	void UpdateLastImg() { m_nLastIdx = -1; }

public:
	TImageList(TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~TImageList();
};

#endif // !defined(AFX_TIMAGELIST_H__32028F19_B866_4E3C_8120_CB3D8EE18F87__INCLUDED_)



















