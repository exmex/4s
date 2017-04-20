#pragma once


class CTMiniDump
{
public:
	CTMiniDump();
	~CTMiniDump();

	static LONG WINAPI TopLevelExceptionFilter(_EXCEPTION_POINTERS *pException);
};

extern CTMiniDump theTMiniDump;