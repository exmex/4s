#pragma once

// =====================================================================
/**	@class		CTProfileNode
	@author		Greg Hjelstrom & Byon Garrabrant & COFT
	@version	0.1
	@date		2005-03-15
	@brief		이는 프로파일링 시스템 내부에서만 쓰이는 클래스이다.
				이 클래스는 코드 블럭 하나가 소비한 총 시간과 그 블럭의
				호출 횟수를 저장한다. CPU 가 한 노드의 범위안에 있으면,
				그 노드는 시작 시간도 저장한다.
	
*/// ===================================================================
class CTProfileNode {
	// #####################################################################
	// 멤버 변수
	// #####################################################################
public:
	const char*		Name;
	int				TotalCalls;
	float			TotalTime;
	__int64			StartTime;
	int				RecursionCounter;

	CTProfileNode*	Parent;
	CTProfileNode*	Child;
	CTProfileNode*	Sibling;

	// #####################################################################
	// 멤버 함수
	// #####################################################################
public:	
	/**
		생성자
		@param name		이 노드의 이름
		@param parent	부모 노드
	*/
	CTProfileNode( const char * name, CTProfileNode * parent );
	/// 소멸자
	~CTProfileNode( void );

	CTProfileNode * 	Get_Sub_Node( const char * name );

	CTProfileNode * 	Get_Parent( void )			{ return Parent; }
	CTProfileNode * 	Get_Sibling( void )			{ return Sibling; }
	CTProfileNode * 	Get_Child( void )			{ return Child; }

	void			Reset( void );
	void			Call( void );
	bool			Return( void );

	const char *	Get_Name( void )			{ return Name; }
	int				Get_Total_Calls( void )		{ return TotalCalls; }
	float			Get_Total_Time( void )		{ return TotalTime; }
	
};// end of CTProfileNode

// =====================================================================
/**	@ingroup	CTProfileIterator
	@class		Profile
	@author		Greg Hjelstrom & Byon Garrabrant & COFT
	@version	0.1
	@date		2005-03-15
	@brief		이 클래스는 프로파일 트리를 편하게 탐색하는데 쓰이는 반
				복자이다. 이 클래스는 트리탐색 및 특정 노드의 내용 표시에
				필요한 함수를 제공한다.  일반적으로 하나으 노드와 그에 직
				접 연결된 자식 노드들이 표시된다. 각 자식에 대해서는 총
				소비시간, 총 호출 횟수, 프레임 당 호출 횟수, 프레임 당 소
				비 시간 등이 표시된다.

*/// ===================================================================
class CTProfileIterator {

	friend	class CTProfileManager;

	// #####################################################################
	// 멤버 변수
	// #####################################################################
public:
	CTProfileNode*	CurrentParent;
	CTProfileNode*	CurrentChild;

	// #####################################################################
	// 멤버 함수
	// #####################################################################
protected:
	CTProfileIterator( CTProfileNode * start );
	
public:	
	// Access all the children of the current parent
	//
	void				First(void);
	void				Next(void);
	bool				Is_Done(void);

	void				Enter_Child( int index );		// Make the given child the new parent
	void				Enter_Largest_Child( void );	// Make the largest child the new parent
	void				Enter_Parent( void );			// Make the current parent's parent the new parent

	// Access the current child
	//
	const char *		Get_Current_Name( void )			{ return CurrentChild->Get_Name(); }
	int					Get_Current_Total_Calls( void )		{ return CurrentChild->Get_Total_Calls(); }
	float				Get_Current_Total_Time( void )		{ return CurrentChild->Get_Total_Time(); }

	// Access the current parent
	//
	const char *		Get_Current_Parent_Name( void )			{ return CurrentParent->Get_Name(); }
	int					Get_Current_Parent_Total_Calls( void )	{ return CurrentParent->Get_Total_Calls(); }
	float				Get_Current_Parent_Total_Time( void )	{ return CurrentParent->Get_Total_Time(); }

};// end of CTProfileIterator

// =====================================================================
/**	@ingroup	CTProfileManager
	@class		Profile
	@author		Greg Hjelstrom & Byon Garrabrant & COFT
	@version	0.1
	@date		2005-03-15
	@brief		이 클래스는 프로파일링 시스템에 대한 외부 인터페이스이다.
				이 클래스는 현재 실행된는 코드 범위에 해당하는 프로파일 트
				리를 가리키는 CurrentNode 포인터를 관리한다. 또한 표시 목
				적으로 프로파일 트리에 접근할 때 쓰일 함수들도 제공한다.

*/// ===================================================================
class CTProfileManager {
	// #####################################################################
	// 멤버 변수
	// #####################################################################
public:
	static	CTProfileNode		Root;
	static	CTProfileNode*		CurrentNode;
	static	int					FrameCounter;
	static	__int64				ResetTime;

	// #####################################################################
	// 멤버 함수
	// #####################################################################
public:	
	/**
		프로파일 표본을 시작한다. 이 함수는 요청된 프로파일의 이름을 현재 노드의
		이름과 비교하여 재귀적인 순환을 감지한다.
		
		@param name		현재 이름
	*/
	static	void				Start_Profile( const char* name );
	
	/**
		프로파일 표본을 끝낸다. 이 함수는 우선 현재 노드에 대해 Return() 함수를
		호출해서 시간 측정을 종료하고 결과를 기록한다. 프로파일 관리자는 표본 측
		정중인 현재 노드를 계속 유지하므로, 이 과정에서는 어떠한 검색도 일어나지
		않는다. 재귀 도중이 아니라면 부모 노드를 현재 노드로 만든다.
	*/
	static	void				Stop_Profile( void );

	static	void				Reset( void );
	static	void				Increment_Frame_Counter( void );
	static	int					Get_Frame_Count_Since_Reset( void )		{ return FrameCounter; }
	static	float				Get_Time_Since_Reset( void );

	static	CTProfileIterator *	Get_Iterator( void )	{ return new CTProfileIterator( &Root ); }
	static	void				Release_Iterator( CTProfileIterator * iterator ) { delete iterator; }

	//* 현재 프로파일 상태를 로그파일에 기록한다.
	static	void				Log(CTProfileNode* node = NULL, int depth = 0);

};// end of CTProfileManager

// =====================================================================
/*!	@ingroup	CTProfileSample
	@class		Profile
	@author		Greg Hjelstrom & Byon Garrabrant & COFT
	@version	0.1
	@date		2005-03-15
	@brief		이 작은 클래스는 생성자에서 Start_Profile 을 호출하고 소멸자
				에서 Stop_Profile 을 호출하는 것만이 유일한 임무이다.

*/// ===================================================================
class CTProfileSample {
	// #####################################################################
	// 멤버 함수
	// #####################################################################
public:
	CTProfileSample( const char * name )
	{ 
		CTProfileManager::Start_Profile( name ); 
	}

	~CTProfileSample( void )					
	{ 
		CTProfileManager::Stop_Profile(); 
	}

};// end of CTProfileSample

// =====================================================================
// Macro
// =====================================================================
#ifdef TEST_MODE
	#define	TPROFILE_AUTO( name )		CTProfileSample __profile( name )
	#define	TPROFILE_START( name )		CTProfileManager::Start_Profile( name )
	#define	TPROFILE_END( name )		CTProfileManager::Stop_Profile()
	#define TPROFILE_RESET				CTProfileManager::Reset();
	#define TPROFILE_LOG(node)			CTProfileManager::Log(node);
#else
	#define	TPROFILE_AUTO( name )		
	#define	TPROFILE_START( name )		
	#define	TPROFILE_END( name )		
	#define TPROFILE_RESET			
	#define TPROFILE_LOG(node)		
#endif

class CProfiler
{
public:
	double m_dAccumulatedSec;
	UINT m_uNumberOfTimes;

public:
	CProfiler()
	{
		m_dAccumulatedSec = 0;
		m_uNumberOfTimes = 0;
	}

	~CProfiler()
	{
	}
};

class CProfileSample
{
public:
	inline CProfileSample( CProfiler& p, BOOL bProfile=TRUE )
		: m_Profiler(p), m_bProfile(bProfile)
	{
		if( m_bStartProfiling && m_bProfile )
			QueryPerformanceCounter( &m_liStartTime );
	}

	inline ~CProfileSample()
	{
		if( m_bStartProfiling && m_bProfile )
		{
			LARGE_INTEGER liEnd;
			QueryPerformanceCounter(&liEnd);

			++m_Profiler.m_uNumberOfTimes;
			m_Profiler.m_dAccumulatedSec +=
				( ((double)liEnd.QuadPart) / m_liFrequency.QuadPart) - ( ((double)m_liStartTime.QuadPart) / m_liFrequency.QuadPart);
		}
	}

public:
	CProfiler& m_Profiler;
	LARGE_INTEGER m_liStartTime;
	BOOL m_bProfile;

public:
	static LARGE_INTEGER m_liFrequency;
	static BOOL m_bStartProfiling;

	static void InitFrequency()
	{
		QueryPerformanceFrequency(&m_liFrequency);
	}
	
	static void StartProfiling()
	{
		m_bStartProfiling = TRUE;
	}
};