// AppLauncher.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	CString strBUF[2];

	if( argc < 2 )
	{
		printf("Useage :\nAppLauncher [Exec] [Param1] [Param2]...");
		return 0;
	}

	strBUF[0].Format( "%s", argv[1]);
	strBUF[1] = strBUF[0];
	argc -= 2;

	for( int i=0; i<argc; i++)
	{
		strBUF[0].Format( "%s %s", strBUF[1], argv[i + 2]);
		strBUF[1] = strBUF[0];
	}

	printf(strBUF[0]);
	_spawnl( _P_WAIT, argv[1], strBUF[0], NULL);

	return 0;
}
