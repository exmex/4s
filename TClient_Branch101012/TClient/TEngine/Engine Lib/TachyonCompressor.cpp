#include "Stdafx.h"
#include "TachyonCompressor.h"

// CTachyonCompressor
// ==============================================================
CTachyonCompressor::CTachyonCompressor()
	: m_dwCompressedSize(0), m_pCompressedData(NULL)
{
}
// --------------------------------------------------------------
CTachyonCompressor::~CTachyonCompressor()
{
	Release();
}
// ==============================================================
void CTachyonCompressor::Compress()
{
	if( m_pCompressedData )
	{
		delete [] m_pCompressedData;
		m_pCompressedData = NULL;
	}
	
	m_dwCompressedSize = 0;

	UINT nSize = (UINT)m_vData.size();
	m_dwCompressedSize = nSize + nSize/10 + 12;
	m_pCompressedData = new BYTE[m_dwCompressedSize];

	compress2(m_pCompressedData,&m_dwCompressedSize, &m_vData[0],nSize, 9);
}
// --------------------------------------------------------------
void CTachyonCompressor::Release()
{
	m_vData.clear();

	if( m_pCompressedData )
	{
		delete [] m_pCompressedData;
		m_pCompressedData = NULL;
	}
	
	m_dwCompressedSize = 0;
}
// ==============================================================
void CTachyonCompressor::Write(LPCVOID lpBuf, UINT nCount)
{
	size_t nPos = m_vData.size();
	m_vData.resize( nPos + nCount );
	memcpy(&m_vData[nPos], lpBuf, nCount);
}
// ==============================================================
void CTachyonCompressor::ToFile(CFile* pFile)
{
	Compress();
		
	DWORD dwOriginSize = GetLength();
	pFile->Write(&dwOriginSize, sizeof(dwOriginSize));
	pFile->Write(&m_dwCompressedSize, sizeof(m_dwCompressedSize));
	pFile->Write(m_pCompressedData, m_dwCompressedSize);
}
// ==============================================================


// CTachyonUncompressor
// ==============================================================
CTachyonUncompressor::CTachyonUncompressor()
	: m_dwSize(0), m_pData(NULL)
{
}
// --------------------------------------------------------------
CTachyonUncompressor::~CTachyonUncompressor()
{
	Release();
}
// ==============================================================
void CTachyonUncompressor::Uncompress(LPCVOID pCompressedData, DWORD dwCompressedSize, DWORD dwOriginalSize)
{
	Release();

	m_dwSize = dwOriginalSize;
	m_pData = new BYTE[dwOriginalSize];
	uncompress(m_pData, &m_dwSize, (const Bytef*)pCompressedData,dwCompressedSize);
}
// --------------------------------------------------------------
void CTachyonUncompressor::Release()
{
	if( m_pData )
	{
		delete [] m_pData;
		m_pData = NULL;
	}

	m_dwSize = 0;
	m_dwPos = 0;
}
// ==============================================================
UINT CTachyonUncompressor::Read(LPVOID lpBuf, UINT nCount)
{
	if( m_dwPos > m_dwSize )
		return 0;

	if( m_dwPos+nCount >= m_dwSize )
		nCount = m_dwSize - m_dwPos;

	if( nCount )
	{
		memcpy(lpBuf, m_pData+m_dwPos, nCount);
		m_dwPos += nCount;
	}

	return m_dwPos;
}
// --------------------------------------------------------------
DWORD CTachyonUncompressor::Seek(DWORD lOff, UINT nFrom)
{
	m_dwPos = nFrom + lOff;
	return m_dwPos;
}
// ==============================================================
DWORD CTachyonUncompressor::FromFile(CFile* pFile)
{
	DWORD dwORIGIN, dwLENGTH;
	pFile->Read(&dwORIGIN, sizeof(DWORD));
	pFile->Read(&dwLENGTH, sizeof(DWORD));

	BYTE* pDATA = new BYTE[dwLENGTH];
	pFile->Read(pDATA, dwLENGTH);
	
	Uncompress(pDATA, dwLENGTH, dwORIGIN);
	delete [] pDATA;

	return (DWORD)pFile->GetPosition();
}
// ==============================================================












