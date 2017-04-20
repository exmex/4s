#include "Stdafx.h"
#include "TProfile.h"

// =====================================================================
inline void Profile_Get_Ticks(_int64 * ticks)
{
	::QueryPerformanceCounter((LARGE_INTEGER *)ticks);
}
inline float Profile_Get_Tick_Rate(void)
{
	static float _CPUFrequency = -1.0f;

	if (_CPUFrequency == -1.0f)
	{
		__int64 curr_rate = 0;
		::QueryPerformanceFrequency ((LARGE_INTEGER *)&curr_rate);
		_CPUFrequency = (float)curr_rate;
	} 

	return _CPUFrequency;
}
// =====================================================================

// CTProfileNode
// =====================================================================
CTProfileNode::CTProfileNode( const char * name, CTProfileNode * parent ) :
	Name( name ),
	TotalCalls( 0 ),
	TotalTime( 0 ),
	StartTime( 0 ),
	RecursionCounter( 0 ),
	Parent( parent ),
	Child( NULL ),
	Sibling( NULL )
{
	Reset();
}
CTProfileNode::~CTProfileNode( void )
{
	delete Child;
	delete Sibling;
}
// =====================================================================
CTProfileNode * CTProfileNode::Get_Sub_Node( const char * name )
{
	// Try to find this sub node
	CTProfileNode * child = Child;
	while ( child ) 
	{
		if ( child->Name == name ) 
			return child;
		
		child = child->Sibling;
	}

	// We didn't find it, so add it
	CTProfileNode * node = new CTProfileNode( name, this );
	node->Sibling = Child;
	Child = node;

	return node;
}
void	CTProfileNode::Reset( void )
{
	TotalCalls = 0;
	TotalTime = 0.0f;

	if ( Child ) 
		Child->Reset();
	
	if ( Sibling ) 
		Sibling->Reset();
}
void	CTProfileNode::Call( void )
{
	TotalCalls++;

	if (RecursionCounter++ == 0) 
		Profile_Get_Ticks(&StartTime);
}
bool	CTProfileNode::Return( void )
{
	if ( --RecursionCounter == 0 && TotalCalls != 0 )
	{ 
		__int64 time;
		Profile_Get_Ticks(&time);
		time -= StartTime;
		TotalTime += (float)time / Profile_Get_Tick_Rate();
	}

	return ( RecursionCounter == 0 );
}
// =====================================================================

// CTProfileIterator
// =====================================================================
CTProfileIterator::CTProfileIterator( CTProfileNode * start )
{
	CurrentParent = start;
	CurrentChild = CurrentParent->Get_Child();
}
void	CTProfileIterator::First(void)
{
	CurrentChild = CurrentParent->Get_Child();
}
void	CTProfileIterator::Next(void)
{
	CurrentChild = CurrentChild->Get_Sibling();
}
bool	CTProfileIterator::Is_Done(void)
{
	return CurrentChild == NULL;
}
void	CTProfileIterator::Enter_Child( int index )
{
	CurrentChild = CurrentParent->Get_Child();
	while ( (CurrentChild != NULL) && (index != 0) ) 
	{
		--index;
		CurrentChild = CurrentChild->Get_Sibling();
	}

	if ( CurrentChild != NULL )
	{
		CurrentParent = CurrentChild;
		CurrentChild = CurrentParent->Get_Child();
	}
}
void	CTProfileIterator::Enter_Parent( void )
{
	if ( CurrentParent->Get_Parent() != NULL ) 
		CurrentParent = CurrentParent->Get_Parent();
	
	CurrentChild = CurrentParent->Get_Child();
}
// =====================================================================

// CTProfileManager
// =====================================================================
CTProfileNode	CTProfileManager::Root( "Root", NULL );
CTProfileNode *	CTProfileManager::CurrentNode = &CTProfileManager::Root;
int				CTProfileManager::FrameCounter = 0;
__int64			CTProfileManager::ResetTime = 0;
// =====================================================================
void	CTProfileManager::Start_Profile( const char * name )
{
	if (name != CurrentNode->Get_Name())
		CurrentNode = CurrentNode->Get_Sub_Node( name );

	CurrentNode->Call();
}
// =====================================================================
void	CTProfileManager::Stop_Profile( void )
{
	// Return will indicate whether we should back up to our parent (we may
	// be profiling a recursive function)
	if (CurrentNode->Return())
		CurrentNode = CurrentNode->Get_Parent();
}
// =====================================================================
void	CTProfileManager::Reset( void )
{ 
	Root.Reset(); 
	FrameCounter = 0;
	Profile_Get_Ticks(&ResetTime);
}
// =====================================================================
void CTProfileManager::Increment_Frame_Counter( void )
{
	FrameCounter++;
}
// =====================================================================
float CTProfileManager::Get_Time_Since_Reset( void )
{
	__int64 time;
	Profile_Get_Ticks(&time);
	time -= ResetTime;
	return (float)time / Profile_Get_Tick_Rate();
}
// =====================================================================
void CTProfileManager::Log(CTProfileNode* node, int depth)
{
	static float totalTime;

	CTProfileNode* curNode = node;
	if( !curNode )
	{
		curNode = &Root;
		totalTime = Get_Time_Since_Reset();

		TRACE("-------------------------------------------------------------------\n");
		TRACE("Result of Profile (%.3fsec)\n", totalTime);
		TRACE("[operation] [Total calls] [average time] [ROC(parent)] [ROC(total)]\n");
		TRACE("-------------------------------------------------------------------\n");
	}

	char bufBlank[50];
	for(int i=0; i<50 && i<depth; i++)
		bufBlank[i] = '+';
	
	bufBlank[depth] = NULL;

	while(curNode)
	{
		if( curNode != &Root )
		{
			float avgTime = curNode->TotalTime / ((float)curNode->TotalCalls?(float)curNode->TotalCalls:1);

			float rocp = 100.0f;
			if( curNode->Parent && curNode->Parent->TotalTime )
				rocp = curNode->TotalTime * 100.0f / curNode->Parent->TotalTime;

			TRACE("%d%s->[%s] [%d] [%.3fms] [%.1f%%] [%.1f%%]\n"
				,depth, bufBlank, curNode->Name, curNode->TotalCalls, avgTime, rocp
				,curNode->TotalTime * 100.0f / totalTime);
		}

		if( curNode->Child )
			CTProfileManager::Log(curNode->Child, depth+1);

		curNode = curNode->Sibling;
	}
}
// =====================================================================

LARGE_INTEGER CProfileSample::m_liFrequency;
BOOL CProfileSample::m_bStartProfiling = FALSE;

