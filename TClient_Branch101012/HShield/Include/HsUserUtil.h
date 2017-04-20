/*********************************************************************************
		Administartors그룹 권한을 가진 계정생성 헤더파일 - HsUserUtil.h
					HsUserUtil V1.0 for Windows 2000, XP, 2003
							(주) 안철수 연구소

	버  전 :
	주  석 : Administartors그룹 권한을 가진 계정생성 및 폴더권한 설정
*********************************************************************************/

#ifndef _HSUSERUTIL_H
#define _HSUSERUTIL_H

//ERROR CODE 
#define	HSUSERUTIL_ERR_OK						0x00000000		//성공
#define	HSUSERUTIL_ERR_UNKNOWN					0x0005A001		//알수없는 오류
#define	HSUSERUTIL_ERR_NOT_ADMIN				0x0005A002		//Administrator 권한이 아님
#define	HSUSERUTIL_ERR_NOT_NT					0x0005A003		//NT계열이 아님
#define	HSUSERUTIL_ERR_LOADDLL_FAIL				0x0005A004		//DLL동적 로딩 실패
#define	HSUSERUTIL_ERR_DELSHADOWACNT_FAIL		0x0005A005		//쉐도우계정 패턴삭제 실패
#define	HSUSERUTIL_ERR_DELHIDEIDINFO_FAIL		0x0005A006		//로그온시 유저계정 감추기기 위한 정보삭제 실패
#define	HSUSERUTIL_ERR_DELSHADOWACNTINFO_FAIL	0x0005A007		//쉐도우계정정보 삭제실패
#define	HSUSERUTIL_ERR_ADDSHADOWACNT_FAIL		0x0005A008		//쉐도우계정 생성실패
#define	HSUSERUTIL_ERR_SHADOWACNT_NOT_EXIST		0x0005A009		//쉐도우계정 존재하지 않음


#define	HSUSERUTIL_ERR_SETFLDRPERMISSION_FAIL	0x0005A10A		//폴더권한주기 실패
#define	HSUSERUTIL_ERR_GETGROUPSID_FAIL			0x0005A10B		//그룹 SID 얻기 실패
#define	HSUSERUTIL_ERR_GETSECINFO_FAIL			0x0005A10C		//SD와 DACL정보 얻기실패
#define	HSUSERUTIL_ERR_ADDNEWACE_FAIL			0x0005A10D		//새 ACE 생성 실패
#define	HSUSERUTIL_ERR_ADDNEWDACL_FAIL			0x0005A10E		//새 DACL 생성 실패
#define	HSUSERUTIL_ERR_COPYOLDDACL_FAIL			0x0005A10F		//새 DACL에 기존DACL 복사 실패
#define	HSUSERUTIL_ERR_ADDNEWACETONEWDACL_FAIL	0x0005A110		//새 DACL에 새 ACE 추가 실패
#define	HSUSERUTIL_ERR_GETVOLUMEINFO_FAIL		0x0005A111		//드라이브 VOLUME INFO얻기 실패.



/*
	@param
		void

	@retval
		DWORD : ERROR_SUCCESS = 성공
				그외의 리턴값 = 오류

	@remarks
		일반 사용자 계정에서 정상적으로 게임및 핵쉴드 보안기능을 구동하기 위해 핵쉴드 계정을
		생성합니다.

		해당 함수 호출이 성공적으로 완료되어야 일반 사용자 계정에서도 게임을 
		실행하고 게임 해킹 방어 기능을 수행할 수 있습니다.
		이 때, 해당 함수는 관리자 계정으로 게임 프로그램, Install-Shield, 게임 런처등의 프로그램에서 
		호출합니다.
*/
DWORD   __stdcall _AhnHsUserUtil_CreateUser ( );


/*
	@param
		LPTSTR : 권한을 부여할 경로

	@retval
		DWORD : ERROR_SUCCESS = 성공
				그외의 리턴값 = 오류
	@remarks
		게임 프로그램이 NTFS 볼륨에 설치된 경우에는 해당 폴더에 대해서 일반 사용자 권한으로는 
		파일 쓰기 권한이 없어 게임이 정상적으로 동작하지 않을 수 있습니다. 
		예를 들어 게임 모듈에 대한 업데이트 프로그램이 실행되어도 게임이 설치된 폴더에 
		최신 파일을 쓰지 못하는 상황이 발생한다든지, 게임 실행 중에 게임 데이터를 
		저장하는데 실패할 수도 있습니다. 

		일반 사용자 계정에 대하여 게임 설치 폴더에 파일 쓰기 권한을 부여하려면 
		해당 함수를 사용함으로써 NTFS 쓰기 권한을 부여할 수 있습니다
*/
DWORD	__stdcall _AhnHsUserUtil_SetFolderPermission(LPTSTR szPath);



/*
	@param
		void

	@retval
		DWORD : ERROR_SUCCESS = 성공
				그외의 리턴값 = 오류
	@remarks
		일반 사용자 계정에서 정상적으로 게임및 핵쉴드 보안기능을 구동하기 위해 등록한
		계정 정보를 삭제하며, 게임 제거시에 해당함수를 호출해야 합니다.
*/
DWORD	__stdcall _AhnHsUserUtil_DeleteUser ( );


/*
	@param
		void

	@retval
		DWORD : ERROR_SUCCESS = ADMIN 권한이거나, 쉐도우 계정이 존재하는 경우.
				그외의 리턴값 = 오류
	@remarks
		현재 로그인된 계정이 핵쉴드를 실행할 수 있는 권한을 가지고 있는지 확인하는 함수입니다. 
*/
DWORD	__stdcall _AhnHsUserUtil_IsEnableHSAdminRights ( );


/*
	@param
		void

	@retval
		DWORD : ERROR_SUCCESS = 핵쉴드 쉐도우 계정이 존재하는 경우.
				HSUSERUTIL_ERR_SHADOWACNT_NOT_EXIST = 핵쉴드 쉐도우 계정이 존재하지 않는 경우.
				그외의 리턴값 = 오류.
	@remarks
		현재 OS 에 핵쉴드 쉐도우 계정이 존재하는지 확인하는 함수입니다.
*/
DWORD	__stdcall _AhnHsUserUtil_CheckHSShadowAccount( );


/*
	@param
		void

	@retval
		BOOL : TRUE  = ADMIN 권한을 가지고  있는 경우.
			   FALSE = 어드민 권한을 가지고 있지 않은 경우.
	@remarks
	   현재 로그인된 계정이 어드민 권한을 가지고 있는지 확인하는 함수입니다.
		
*/
BOOL	__stdcall _AhnHSUserUtil_IsAdmin ();




#endif //_HSUSERUTIL_H
