#pragma once

class CTachyonCompressor
{
protected:
	std::vector<BYTE>	m_vData;
	
	DWORD				m_dwCompressedSize;
	LPBYTE				m_pCompressedData;

public:
	void Compress();
	void Release();

	void Write(LPCVOID lpBuf, UINT nCount);

	void ToFile(CFile* pFile);

	LPCVOID GetData() const				{ return m_vData.empty()? NULL: (&m_vData[0]); }
	LPCVOID GetCompressedData() const	{ return m_pCompressedData; }

	DWORD GetLength() const				{ return (DWORD)m_vData.size(); }
	DWORD GetCompressedLength() const	{ return m_dwCompressedSize; }

public:
	CTachyonCompressor();
	~CTachyonCompressor();
};

class CTachyonUncompressor
{
protected:
	DWORD	m_dwSize;
	LPBYTE	m_pData;
	DWORD	m_dwPos;

public:
	void Uncompress(LPCVOID pCompressedData, DWORD dwCompressedSize, DWORD dwOriginalSize);
	void Release();
	
	UINT Read(LPVOID lpBuf, UINT nCount);
	DWORD Seek(DWORD lOff, UINT nFrom);
	
	DWORD FromFile(CFile* pFile);

	LPCVOID GetCurData() const			{ return m_pData + m_dwPos; }
	LPCVOID GetData() const				{ return m_pData; }
	DWORD GetLength() const				{ return m_dwSize; }
	DWORD GetPosition() const			{ return m_dwPos; }

public:
	CTachyonUncompressor();
	~CTachyonUncompressor();
};
