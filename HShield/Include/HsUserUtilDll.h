#ifndef __HSUSERUTILDLL_H__
#define __HSUSERUTILDLL_H__

#ifdef HSUSERUTILDLL_EXPORTS
#define HSUSERUTILDLL_API __declspec(dllexport)
#else
#define HSUSERUTILDLL_API __declspec(dllimport)
#endif


// error codes
#define HSUSERUTIL_ERR_CREATE_USER_FAILED				0x01
#define HSUSERUTIL_ERR_SET_FOLDER_PERMISSION_FAILED		0x02
#define HSUSERUTIL_ERR_DELETE_USER_FAILED				0x03
#define HSUSERUTIL_ERR_ENABLE_HSADMIN_RIGHTS			0x04
#define HSUSERUTIL_ERR_HSUSER_ACCOUNT_NOT_EXIST			0x05

// exported functions

#ifdef __cplusplus
extern "C" {
#endif



	
/*
	@param
		void

	@retval
		DWORD : ERROR_SUCCESS = 성공
				HSUSERUTIL_ERR_CREATE_USER_FAILED = 오류

	@remarks
		해당 함수 호출이 성공적으로 완료되어야 일반 사용자 계정에서도 게임을 
		실행하고 게임 해킹 방어 기능을 수행할 수 있습니다.
		이 때, 해당 함수는 관리자 계정으로 게임 프로그램, Install-Shield, 게임 런처등의 프로그램에서 
		호출합니다.
*/
HSUSERUTILDLL_API  
DWORD __stdcall HsUserUtil_ExecUserCreate();

/*
	@param
		LPTSTR : 권한을 부여할 경로

	@retval
		DWORD : ERROR_SUCCESS = 성공
				HSUSERUTIL_ERR_SET_FOLDER_PERMISSION_FAILED = 오류
	@remarks
		게임 프로그램이 NTFS 볼륨에 설치된 경우에는 해당 폴더에 대해서 일반 사용자 권한으로는 
		파일 쓰기 권한이 없어 게임이 정상적으로 동작하지 않을 수 있습니다. 
		예를 들어 게임 모듈에 대한 업데이트 프로그램이 실행되어도 게임이 설치된 폴더에 
		최신 파일을 쓰지 못하는 상황이 발생한다든지, 게임 실행 중에 게임 데이터를 
		저장하는데 실패할 수도 있습니다. 

		일반 사용자 계정에 대하여 게임 설치 폴더에 파일 쓰기 권한을 부여하려면 
		해당 함수를 사용함으로써 NTFS 쓰기 권한을 부여할 수 있습니다
*/
HSUSERUTILDLL_API 
DWORD __stdcall HsUserUtil_ExecSetFolderPermission(LPTSTR szInstallDir);

/*
	@param
		void

	@retval
		DWORD : ERROR_SUCCESS = 성공
				HSUSERUTIL_ERR_DELETE_USER_FAILED = 오류
	@remarks
		일반 사용자 계정에서 정상적으로 게임및 핵쉴드 보안기능을 구동하기 위해 등록한
		계정 정보를 삭제하며, 게임 제거시에 해당함수를 호출해야 합니다.
*/
HSUSERUTILDLL_API 
DWORD __stdcall HsUserUtil_ExecUserDelete();

/*
	@param
		void

	@retval
		DWORD : ERROR_SUCCESS = ADMIN 권한이거나, 쉐도우 계정이 존재하는 경우.
				HSUSERUTIL_ERR_ENABLE_HSADMIN_RIGHTS = 오류
	@remarks
		현재 계정이 핵쉴드를 실행할 수 있는 권한을 가지고 있는지 확인하는 함수입니다. 
*/
HSUSERUTILDLL_API 
DWORD __stdcall HsUserUtil_IsEnableHSAdminRights();


/*
	@param
		void

	@retval
		DWORD : ERROR_SUCCESS = 핵쉴드 쉐도우 계정이 존재하는 경우.
				HSUSERUTIL_ERR_HSUSER_ACCOUNT_NOT_EXIST = 핵쉴드 쉐도우 계정이 존재하지 않는 경우.
				그외의 리턴값 = 오류.
	@remarks
		현재 OS 에 핵쉴드 쉐도우 계정이 존재하는지 확인하는 함수입니다.
*/
HSUSERUTILDLL_API 
DWORD	__stdcall HsUserUtil_CheckHSShadowAccount( );


/*
	@param
		void

	@retval
		BOOL : TRUE  = ADMIN 권한을 가지고  있는 경우.
			   FALSE = 어드민 권한을 가지고 있지 않은 경우.
	@remarks
	   현재 로그인된 계정이 어드민 권한을 가지고 있는지 확인하는 함수입니다.
		
*/
HSUSERUTILDLL_API 
BOOL __stdcall HsUserUtil_IsAdmin();



#ifdef __cplusplus
}
#endif


#endif
