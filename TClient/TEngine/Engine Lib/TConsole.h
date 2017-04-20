#pragma once

class CTConsole
{
private:
	HANDLE m_hConsole;

public:
	bool Create(LPCSTR szTitle, bool bNoClose = false);
	
	void Color(WORD wColor = NULL);
	void Output(LPCSTR szOutput = NULL, ...);
	
	void SetTitle(LPCSTR szTitle);
	char* GetTitle();

	HWND GetHWND();
	HANDLE GetHandle();

	void Show(bool bShow = true);
	void DisableClose();
	void Clear();

	void Close();

	bool IsValid() const { return m_hConsole? true: false; }

public:
	CTConsole();
	~CTConsole();

	static CTConsole* GetInstance();
};

#ifdef TRACE
#undef TRACE
#endif

void TRACE(LPCSTR szOutput = NULL, ...);
