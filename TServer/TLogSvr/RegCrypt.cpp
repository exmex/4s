/*
 *	
 */
#include "StdAfx.h"
#include "RegCrypt.h"
#include "XRegKey.h"

bool CRegCrypt::RegEncrypt(char* pRegKeyName, char* pRegValueName, void* pValue, int nLen, char* pHashKey)
{
	HCRYPTPROV	hCryptProv	= NULL;
	HCRYPTKEY	hKey		= NULL;
	HCRYPTHASH	hHash		= NULL;

	HANDLE		hSource		= NULL, 
				hDest		= NULL;

	CXRegKey keySetting;

    LONG lRes = keySetting.Open(HKEY_LOCAL_MACHINE, pRegKeyName, KEY_READ | KEY_WRITE);

    if (lRes != ERROR_SUCCESS)
	{
        return false;
	}

	// handle to default provider (we might change it later)
	if(!CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, 0))
	{
		if(GetLastError() == NTE_BAD_KEYSET)
		{
			//new keyset
			if(!CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET))
				return false;
		}
	}

	//create session key;
	if(!CryptCreateHash(hCryptProv, CALG_MD5, 0, 0, &hHash))
		return false;

	if(!CryptHashData(hHash, (BYTE *)pHashKey, strlen(pHashKey), 0))
		return false;

	if(!CryptDeriveKey(hCryptProv, CALG_RC2, hHash, 0, &hKey))
		return false;

	CryptDestroyHash(hHash);
	hHash = NULL;

	BYTE  Buffer[1024]	= {0,};	
	DWORD dwCount		= nLen;
	memcpy(Buffer, pValue, dwCount);

	if(!CryptEncrypt(hKey, 0, TRUE, 0, Buffer, &dwCount, sizeof(Buffer)))
		return false;
	
	lRes = keySetting.SetBinaryValue( Buffer, pRegValueName, dwCount );
	if (lRes != ERROR_SUCCESS)
		return false;				

	keySetting.Close();

	//clean up
	if(hSource)
		CloseHandle(hSource);
	if(hDest)
		CloseHandle(hDest);

	if(hKey)
		CryptDestroyKey(hKey);

	if(hHash)
		CryptDestroyHash(hHash);

	if(hCryptProv)
		CryptReleaseContext(hCryptProv, 0);
	
	return true;
}


bool CRegCrypt::RegDecrypt(char* pRegKeyName, char* pRegValueName, void* pOutValue, int* pInOutLen, char* pHashKey)
{
	HCRYPTPROV	hCryptProv	= NULL;
	HCRYPTKEY	hKey		= NULL;
	HCRYPTHASH	hHash		= NULL;

	HANDLE		hSource		= NULL, 
				hDest		= NULL;

	CXRegKey keySetting;

    LONG lRes = keySetting.Open(HKEY_LOCAL_MACHINE, pRegKeyName, KEY_READ);

    if (lRes != ERROR_SUCCESS)
	{
        return false;
	}

	// handle to default provider (we might change it later)
	if(!CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, 0))
	{
		if(GetLastError() == NTE_BAD_KEYSET)
		{
			//new keyset
			if(!CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET))
				return false;
		}
	}

	//create session key;
	if(!CryptCreateHash(hCryptProv, CALG_MD5, 0, 0, &hHash))
		return false;

	if(!CryptHashData(hHash, (BYTE *)pHashKey, strlen(pHashKey), 0))
		return false;

	if(!CryptDeriveKey(hCryptProv, CALG_RC2, hHash, 0, &hKey))
		return false;

	CryptDestroyHash(hHash);
	hHash = NULL;

	BYTE  Buffer[1024]	= {0,};	
	DWORD dwCount		= sizeof(Buffer);

	lRes = keySetting.QueryBinaryValue( Buffer, pRegValueName, &dwCount );
	if (lRes != ERROR_SUCCESS)		
		return false;

	if(!CryptDecrypt(hKey, 0, TRUE, 0, Buffer, &dwCount))
		return false;

	if(*pInOutLen == 0 || (DWORD)*pInOutLen < dwCount)
	{
		*pInOutLen = 0;
		return false;
	}

	memcpy(pOutValue, Buffer, dwCount);
	*pInOutLen = dwCount;

	//clean up
	if(hSource)
		CloseHandle(hSource);
	if(hDest)
		CloseHandle(hDest);

	if(hKey)
		CryptDestroyKey(hKey);

	if(hHash)
		CryptDestroyHash(hHash);

	if(hCryptProv)
		CryptReleaseContext(hCryptProv, 0);
	
	return true;
}


bool CRegCrypt::Encrypt(void* pValue, int & nLen, char* pHashKey)
{
	HCRYPTPROV	hCryptProv	= NULL;
	HCRYPTKEY	hKey		= NULL;
	HCRYPTHASH	hHash		= NULL;

	HANDLE		hSource		= NULL, 
				hDest		= NULL;

	
	// handle to default provider (we might change it later)
	if(!CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, 0))
	{
		if(GetLastError() == NTE_BAD_KEYSET)
		{
			//new keyset
			if(!CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET))
				return false;
		}
	}

	//create session key;
	if(!CryptCreateHash(hCryptProv, CALG_MD5, 0, 0, &hHash))
		return false;

	if(!CryptHashData(hHash, (BYTE *)pHashKey, strlen(pHashKey), 0))
		return false;

	if(!CryptDeriveKey(hCryptProv, CALG_RC2, hHash, 0, &hKey))
		return false;

	CryptDestroyHash(hHash);
	hHash = NULL;

	BYTE  Buffer[1024]	= {0,};	
	DWORD dwCount		= nLen;
	memcpy(Buffer, pValue, dwCount);

	if(!CryptEncrypt(hKey, 0, TRUE, 0, Buffer, &dwCount, sizeof(Buffer)))
		return false;
	
	memcpy(pValue ,Buffer , dwCount);

	nLen = dwCount ;


	//clean up
	if(hSource)
		CloseHandle(hSource);
	if(hDest)
		CloseHandle(hDest);

	if(hKey)
		CryptDestroyKey(hKey);

	if(hHash)
		CryptDestroyHash(hHash);

	if(hCryptProv)
		CryptReleaseContext(hCryptProv, 0);
	
	return true;
}


bool CRegCrypt::Decrypt( void* pValue, int & nLen, char* pHashKey)
{
	HCRYPTPROV	hCryptProv	= NULL;
	HCRYPTKEY	hKey		= NULL;
	HCRYPTHASH	hHash		= NULL;

	HANDLE		hSource		= NULL, 
				hDest		= NULL;


	// handle to default provider (we might change it later)
	if(!CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, 0))
	{
		if(GetLastError() == NTE_BAD_KEYSET)
		{
			//new keyset
			if(!CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET))
				return false;
		}
	}

	//create session key;
	if(!CryptCreateHash(hCryptProv, CALG_MD5, 0, 0, &hHash))
		return false;

	if(!CryptHashData(hHash, (BYTE *)pHashKey, strlen(pHashKey), 0))
		return false;

	if(!CryptDeriveKey(hCryptProv, CALG_RC2, hHash, 0, &hKey))
		return false;

	CryptDestroyHash(hHash);
	hHash = NULL;

	BYTE  Buffer[1024]	= {0,};	
	DWORD dwCount		= nLen;

	memcpy(Buffer , pValue, nLen);

	
	if(!CryptDecrypt(hKey, 0, TRUE, 0, Buffer, &dwCount))
		return false;


	if(nLen == 0 || (DWORD)nLen < dwCount)
	{
		nLen = 0;
		return false;
	}

	memcpy(pValue, Buffer, dwCount);
	nLen = dwCount;

	//clean up
	if(hSource)
		CloseHandle(hSource);
	if(hDest)
		CloseHandle(hDest);

	if(hKey)
		CryptDestroyKey(hKey);

	if(hHash)
		CryptDestroyHash(hHash);

	if(hCryptProv)
		CryptReleaseContext(hCryptProv, 0);
	
	return true;
}
