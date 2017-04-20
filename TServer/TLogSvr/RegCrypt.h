#ifndef __REGCRYPT_H__
#define __REGCRYPT_H__

#include <windows.h>
#include <wincrypt.h>

/**
 * @ingroup		Libraries
 * @author		Jason
 * @version		1.0
 * @date		08/05/2004
 * @brief		Encrypt/Decrypt the registry
 */

class CRegCrypt  
{
public:
	CRegCrypt()  {};
	~CRegCrypt() {};

	bool RegEncrypt(char* pRegKeyName, char* pRegValueName, void* pValue, int nLen, char* pHashKey = "!yookyoungsahng0111$");
	bool RegDecrypt(char* pRegKeyName, char* pRegValueName, void* pOutValue, int* pInOutLen, char* pHashKey = "!yookyoungsahng0111$");
	
	bool Encrypt(void* pValue, int & nLen, char* pHashKey = "!%$¹®ÆÀÀådfÂ¯ÇÏÇ×*^%&k¸Þ·Õ$");
	bool Decrypt(void* pValue, int & nLen, char* pHashKey = "!%$¹®ÆÀÀådfÂ¯ÇÏÇ×*^%&k¸Þ·Õ$");
	
	USHORT PacketEncrypt(char * pSouce, int iLen, int sKey);
	USHORT PacketDecrypt(char * pSouce, int iLen, int sKey);

	USHORT Checksum(char *buffer, int iLen);

};

#endif //__REGCRYPT_H__
