#pragma once

// =====================================================================
/**	@class		SharedPtr
	@brief		참조 카운트 기능이 있는 공유 포인터 클래스
	
*/// ===================================================================
template<class T> class SharedPtr
{
public:
	/// if(ptr) 과 같은 검사를 가능하게 하기위함
	class TestProxy{};

	// ===================================================================
	// 멤버 변수
	// ===================================================================
protected:
	T*				pRep;
	unsigned int*	pUseCount;

	// ===================================================================
	// 멤버 함수
	// ===================================================================
public:
	// ===================================================================
	inline T& operator*() const		{ return *get(); }
	inline T* operator->() const	{ return  get(); }
	inline bool operator!() const	{ return !get(); }
	// -------------------------------------------------------------------
	/// if(ptr) 과 같은 검사를 가능하게 하기위함
	operator TestProxy*() const
	{
		if(!(*this) || !get() ) 
			return(0);

		static TestProxy testProxy;
		return(&testProxy);
	}
	// ===================================================================
	/// 공유포인터가 가지고 있는 덤 포인터를 얻는다.
	virtual T* get() const					
	{
		return pRep; 
	}
	/// 공유포인터가 가지고 있는 덤 포인터를 static_cast 후 얻는다.
	template <typename RET>	inline RET* staticGet() const					
	{
		return static_cast<RET*>(get()); 
	}
	/// 공유포인터가 가지고 있는 덤 포인터를 const_cast 후 얻는다.
	template <typename RET>	inline RET* constGet() const					
	{
		return const_cast<RET*>(get()); 
	}
	/// 공유포인터가 가지고 있는 덤 포인터를 reinterpret_cast 후 얻는다.
	template <typename RET>	inline RET* reinterpretGet() const					
	{
		return reinterpret_cast<RET*>(get()); 
	}
	// ===================================================================
	/// 이 공유 포인터를 static_cast 한 공유 포인터를 얻는다.
	template <typename RET>	inline SharedPtr<RET> staticCast() const
	{
		SharedPtr<RET> sptr(static_cast<RET*>(pRep), pUseCount);
		return sptr;
	}
	/// 이 공유 포인터를 const_cast 한 공유 포인터를 얻는다.
	template <typename RET>	inline SharedPtr<RET> constCast() const
	{
		SharedPtr<RET> sptr(const_cast<RET*>(pRep), pUseCount);
		return sptr;
	}
	/// 이 공유 포인터를 reinterpret_cast 한 공유 포인터를 얻는다.
	template <typename RET>	inline SharedPtr<RET> reinterpretCast() const
	{
		SharedPtr<RET> sptr(reinterpret_cast<RET*>(pRep), pUseCount);
		return sptr;
	}
	// ===================================================================
	/// 이 공유 포인터의 갯수가 단 하나인지 알아온다.
	inline bool unique() const				
	{ 
		bool res = false;
		if(pUseCount)
			res = static_cast<bool>(*pUseCount==1); 
			
		return res;
	}
	/// 이 포인터가 공유중인 갯수를 얻는다.
	inline unsigned int useCount() const	
	{ 
		unsigned int res = 0;
		if(pUseCount)
			res = *pUseCount; 

		return res;
	}
	// ===================================================================
	/// 널 포인터인가?
	inline bool isNull(void) const			
	{ return static_cast<bool>(get()==NULL); }
	/// 이 포인터를 널로 만든다.
	inline void setNull(void) 
	{ 
		release();

		pRep = NULL;
		pUseCount = NULL;
	}
	// ===================================================================
	// 비교 연산자들..
	//
	friend inline bool operator==(SharedPtr<T>& r,SharedPtr<T>& l)
	{ return (r.get() == l.get()); }

	friend inline bool operator!=(SharedPtr<T>& r,SharedPtr<T>& l)
	{ return (r.get() != l.get()); }

	friend inline bool operator>(SharedPtr<T>& r,SharedPtr<T>& l)
	{ return (r.get() > l.get()); }

	friend inline bool operator<(SharedPtr<T>& r,SharedPtr<T>& l)
	{ return (r.get() < l.get()); }

	friend inline bool operator<=(SharedPtr<T>& r,SharedPtr<T>& l)
	{ return (r.get() <= l.get()); }

	friend inline bool operator>=(SharedPtr<T>& r,SharedPtr<T>& l)
	{ return (r.get() >= l.get()); }

	// ===================================================================
	// 생성/소멸
	// ===================================================================
protected:
	virtual void release(void) 
	{
		bool del = false;

		if (pUseCount)
		{
			if (--(*pUseCount) == 0) 
				del = true;
		}

		if(del)
			destroy();
	}

	virtual void destroy(void)
	{
        delete pRep;
		delete pUseCount;
	}

public:
	// -------------------------------------------------------------------
	SharedPtr() : pRep(NULL), pUseCount(NULL) 
	{
	}
	// -------------------------------------------------------------------
	explicit SharedPtr(T* rep) : pRep(rep)
	{
		if(pRep)
			pUseCount = new unsigned int(1);
		else
			pUseCount = NULL;
	}
	// -------------------------------------------------------------------
	SharedPtr(const SharedPtr& r) 
	{ 
		pRep = r.pRep;
		pUseCount = r.pUseCount;
		if(pUseCount)
			++(*pUseCount); 
	}
	// -------------------------------------------------------------------
	SharedPtr(T* rep, unsigned int* usecnt)
	{
		pRep = rep;
		pUseCount = usecnt;
		if (pUseCount)
			++(*pUseCount);
	}
	// -------------------------------------------------------------------
	SharedPtr& operator=(const SharedPtr& r) 
	{
		if (pRep == r.pRep)
			return *this;
		release();

		pRep = r.pRep;
		pUseCount = r.pUseCount;
		if (pUseCount)
			++(*pUseCount);
	
		return *this;
	}
	// -------------------------------------------------------------------
	virtual ~SharedPtr() 
	{
		release();
	}
	// -------------------------------------------------------------------
};










