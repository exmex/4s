#include "stdafx.h"
#include "TMailSender.h"


// ====================================================================================================
#define MAIL_SND_PORT	25
// ====================================================================================================

// ====================================================================================================
CTMailSender::CTMailSender()
{
}
// ----------------------------------------------------------------------------------------------------
CTMailSender::~CTMailSender()
{
}
// ====================================================================================================

// ====================================================================================================
void CTMailSender::SetMailServer(const CString& strMailServer)
{
	m_strMailServer = strMailServer;
}
// ----------------------------------------------------------------------------------------------------
void CTMailSender::SetSender(const CString& strMail, const CString& strName)
{
	m_strSndMail = strMail;
	m_strSndName = strName;
}
// ----------------------------------------------------------------------------------------------------
void CTMailSender::SetReceiver(const CString& strMail, const CString& strName)
{
	m_strRcvMail = strMail;
	m_strRcvName = strName;
}
// ----------------------------------------------------------------------------------------------------
void CTMailSender::SetSubject(const CString& strSubject)
{
	m_strSubject = strSubject;
}
// ----------------------------------------------------------------------------------------------------
void CTMailSender::SetMessage(const CString& strMessage)
{
	m_strMessage = strMessage;
}
// ----------------------------------------------------------------------------------------------------
void CTMailSender::SetFile(const CString& strFile)
{
	m_strFile = strFile;
}
// ====================================================================================================
BOOL CTMailSender::SendMail()
{
	CString s1,s2;
	CHAR buf[512];
    	
	//::WSACleanup();

	WSADATA WSAData;
	if( ::WSAStartup(MAKEWORD(1,1),&WSAData) != 0 ) 
		return FALSE;

	if( LOBYTE( WSAData.wVersion ) != 1 || HIBYTE( WSAData.wVersion ) != 1 ) 
	{
		//::WSACleanup();
		return FALSE;
	}			   

	SOCKET WinSock = socket(AF_INET,SOCK_STREAM,0);
	if( WinSock == INVALID_SOCKET ) 
	{
		::closesocket(WinSock);
		return FALSE;
	}

	PHOSTENT phostent = gethostbyname(m_strMailServer);
	if( phostent == NULL ) 
	{
		::closesocket(WinSock);
		return FALSE;
	}

	struct sockaddr_in their_addr;
	their_addr.sin_port = htons(MAIL_SND_PORT);
	their_addr.sin_family = AF_INET;
	::memcpy(&their_addr.sin_addr, phostent->h_addr, phostent->h_length);
	
	if( ::connect(WinSock,(PSOCKADDR)&their_addr,sizeof(their_addr)) == SOCKET_ERROR ) 
	{
		::closesocket(WinSock);
		return FALSE;
	}

	INT iReturn = recv(WinSock, buf, 511, 0);
	if( iReturn == SOCKET_ERROR ) 
	{
		::closesocket(WinSock);
		return FALSE;
	}
	
	CString tmp = "helo zemi\r\n";
	::send(WinSock, tmp.GetBuffer(0), tmp.GetLength(), 0);
	iReturn= recv(WinSock, buf, 511, 0);
	if( iReturn == SOCKET_ERROR ) 
	{
		::closesocket(WinSock);
		return FALSE;
	}
	
	tmp = buf;
	if( tmp.Mid(0,3) != "250" )
	{
		::closesocket(WinSock);
		return FALSE;
	}
	
	tmp = "MAIL FROM: <" + m_strSndMail +">\r\n";
	::send(WinSock, tmp.GetBuffer(0), tmp.GetLength(), 0);

	iReturn = recv(WinSock ,buf, 511, 0);
	tmp = buf;
	if( tmp.Mid(0,3) !=  "250" )
	{
		::closesocket(WinSock);
		return FALSE;
	}
	
	tmp =  "RCPT TO:<" + m_strRcvMail +">\r\n";
	::send(WinSock, tmp.GetBuffer(0), tmp.GetLength(), 0);

	iReturn= recv(WinSock, buf, 511, 0);
	tmp = buf;
	if( tmp.Mid(0,3) !=  "250" )
	{
		::closesocket(WinSock);
		return FALSE;
	}

	::send(WinSock, "DATA\r\n", strlen("DATA\r\n"), 0);
	
	iReturn = recv(WinSock, buf, 511, 0);
	tmp = buf;
	if( tmp.Mid(0,3) !=  "354" )
	{
		::closesocket(WinSock);
		return FALSE;
	}

	tmp = GenerateMessageID( m_strSndMail.Mid(s1.Find('@',0)+1) ) + "\r\n";
	::send(WinSock, tmp.GetBuffer(0), tmp.GetLength(), 0);

	tmp = "Date: " + CTime::GetCurrentTime().Format("%A, %B %d, %Y") + "\r\n";
	::send(WinSock, tmp.GetBuffer(0), tmp.GetLength(), 0);
	
	if( m_strSndName.IsEmpty() )
		m_strSndName = m_strSndMail;

	tmp = "From: " + m_strSndName + " <" + m_strSndMail + ">" + "\r\n";
	send(WinSock, tmp.GetBuffer(0), tmp.GetLength(), 0);

	if( m_strRcvName.IsEmpty() )
		m_strRcvName = m_strRcvName ;

	tmp = "To: " + m_strRcvName + " <" + m_strRcvMail + ">" + "\r\n";
	::send(WinSock, tmp.GetBuffer(0), tmp.GetLength(), 0) ;

	tmp = CString("Reply-to: ") + CString(" <") + m_strSndMail + CString(">") + "\r\n" ;
	::send(WinSock, tmp.GetBuffer(0), tmp.GetLength(), 0);

	tmp = "Subject: " + m_strSubject +  "\r\n" ;
	::send(WinSock, tmp.GetBuffer(0), tmp.GetLength(), 0);

	tmp = "X-Mailer: zemi - Mail v1.0 \r\n";
	send(WinSock, tmp.GetBuffer(0), tmp.GetLength(),0);

	tmp = GetMIMEHeader("NextMimePart", "plain") + "\r\n";
	::send(WinSock, tmp.GetBuffer(0), tmp.GetLength(), 0);

	if( m_strMessage.IsEmpty() )
		m_strMessage = "NO MESSAGE";
		
	::send(WinSock, m_strMessage.GetBuffer(0), m_strMessage.GetLength(), 0);

	if( !m_strFile.IsEmpty() )
	{
		s1 = CreateTmpAttachFile((LPCSTR)m_strFile);

		CFile cfileinpt;
		CFileException einp;
		if( cfileinpt.Open(s1, CFile::modeRead, &einp ) )
		{
			::send(WinSock, "\r\n\r\n", strlen("\r\n\r\n"),0); 
			
			DWORD fiplen = (DWORD) cfileinpt.GetLength(); 
			HLOCAL hBuff = ::LocalAlloc(LHND, fiplen);

			char* pBuff = (char*) ::LocalLock(hBuff);
			cfileinpt.Read(pBuff,fiplen);

			CString outfl;
			outfl = pBuff;

			::LocalUnlock(hBuff);
			::LocalFree(hBuff);
		
			tmp = MakeAttachmentHeader("NextMimePart", m_strFile);
			
			::send(WinSock, tmp.GetBuffer(0), tmp.GetLength(), 0);
			::send(WinSock, outfl.GetBuffer(0), outfl.GetLength(), 0);
			::send(WinSock, "\r\n", strlen("\r\n"),0);
		}	
		cfileinpt.Close();
		cfileinpt.Remove(s1); 
	}
	
	::send(WinSock, "\r\n.\r\n", strlen("\r\n.\r\n"), 0);
	iReturn = recv(WinSock, buf, 511, 0);
	tmp = buf;

	if( tmp.Mid(0,3) != "250" )
	{
		closesocket(WinSock);
		return FALSE;
	}

	::send(WinSock, "QUIT\r\n", strlen("QUIT\r\n"), 0);

	//::WSACleanup( );
	
	return TRUE;
}
// ====================================================================================================


// ====================================================================================================
CString CTMailSender::GenerateMessageID(const CString& sHost)
{
	CString sMessageID = "Message-ID: ";

	int idnum = (int)::time(NULL);
	char tmp[10];
	itoa(idnum, tmp, 10);
	
	CString s1 = tmp;
	s1.TrimLeft();
	sMessageID = sMessageID + s1 + ".zemimail@" + sHost;

	return sMessageID;
}
// ----------------------------------------------------------------------------------------------------
CString CTMailSender::GetMIMEHeader(const CString& vsBoundaryID, const CString& msgType)
{
	CString ch((char)34);

	CString MIMEHeader = CString("MIME-Version: 1.0\r\n") +
					     CString("Content-Type: multipart/mixed; boundary=") + ch +
						 vsBoundaryID + ch + CString("; type=") + ch + CString("text/plain") + ch + CString("\r\n") +
						 CString("Text displayed only to non-MIME-compliant mailers\r\n") +
						 CString("--") + vsBoundaryID + CString("\r\n");

	
	if (msgType == "plain")
	 	 MIMEHeader = MIMEHeader + 
					  CString("Content-Type: text/plain; charset=iso-8859-1\r\n") +
					  CString("Content-Transfer-Encoding: 7bit\r\n");
	else
	 	 MIMEHeader = MIMEHeader + 
					  CString("Content-Type: text/html; charset=iso-8859-1\r\n") +
					  CString("Content-Transfer-Encoding: quoted printable\r\n");

	return MIMEHeader;
}
// ----------------------------------------------------------------------------------------------------
CString CTMailSender::CreateTmpAttachFile(LPCSTR flnm)
{
	CString s1;

	CFile cfileinpt;
	CFileException einp;
	if( !cfileinpt.Open(flnm, CFile::modeRead, &einp) )
	{
		cfileinpt.Close(); 
		return s1;
	}

	CString s2, s3;
	s1 = flnm;
	s2 = cfileinpt.GetFileTitle();
	cfileinpt.Close();
	s3 = s2.Mid(0,s2.Find('.')) + ".Y64";
	
	char cpath[100];
	_getcwd(cpath, 100);
	
	s2 = cpath;
	s2 = s2 + "\\" + s3;

	EncodeFileBase64(s1, s2);
	
	return s2;
}
// ----------------------------------------------------------------------------------------------------
CString CTMailSender::MakeAttachmentHeader(const CString& vsBoundaryID, const CString& FullpathfileName)
{
	CString s1, fileName;
	CString ch((char)34);
    CString strtab((char)9);
	 
	int pos = FullpathfileName.ReverseFind('\\') ;
	fileName = FullpathfileName.Mid(pos+1);
	
	s1 = CString("\r\n--NextMimePart\r\n");
	s1 = s1 + CString("Content-Type: text/plain;\r\n") +
		strtab + CString("name=") + ch + fileName + ch + CString("\r\n");
	
	s1 = s1 + CString("Content-Transfer-Encoding: base64") +  CString("\r\n");
	s1 = s1 + CString("Content-Disposition: attachment;\r\n") +  strtab + 
		CString("filename=") + ch + fileName + ch + CString("\r\n\r\n");

	return s1;
}
// ----------------------------------------------------------------------------------------------------
BOOL CTMailSender::EncodeFileBase64(const CString& inputFl, const CString& outputFl)
{
	CFile cfileinpt, cfileout;
	CFileException einp, eout;

	if( !cfileinpt.Open( inputFl, CFile::modeRead, &einp ) )
		return FALSE;
	
	if (!cfileout.Open(outputFl, CFile::modeCreate | CFile::modeWrite, &eout))
		return FALSE;

	CString InputFileName  = inputFl;
	CString OutputFileName = outputFl;
	
	DWORD fiplen = (DWORD)cfileinpt.GetLength(); 
	HLOCAL hBuff = LocalAlloc(LHND, fiplen);
	char* pBuff = (char*) LocalLock(hBuff);
	cfileinpt.Read(pBuff,fiplen);

	DWORD foutlen = CharsNeededToEncode(fiplen);
	HLOCAL hBuffout = LocalAlloc(LHND, foutlen);
	char* pBuffout = (char*) LocalLock(hBuffout);

	unsigned char input[3], output[4];
	DWORD i=0, j=0;
	
	while( (i + 2) < fiplen)
	{
		input[0]=pBuff[i];
		input[1]=pBuff[i+1];
		input[2]=pBuff[i+2];
		
		Encode(input,output, 3);

		pBuffout[j]   = output[0];
		pBuffout[j+1] = output[1];
		pBuffout[j+2] = output[2];
		pBuffout[j+3] = output[3];
		
		i=i+3;
		j=j+4;

	}
	
	switch(fiplen % 3) {
	case 1:
		input[0]=pBuff[i];

		Encode(input,output, 1);

		pBuffout[j]   = output[0];
		pBuffout[j+1] = output[1];
		pBuffout[j+2] = output[2];
		pBuffout[j+3] = output[3];
		
		break;

	case 2:
		input[0]=pBuff[i];
		input[1]=pBuff[i+1];

		Encode(input,output, 1);

		pBuffout[j]   = output[0];
		pBuffout[j+1] = output[1];
		pBuffout[j+2] = output[2];
		pBuffout[j+3] = output[3];
		
		break;
	}
	
	cfileout.Write(pBuffout,foutlen);

	LocalUnlock(hBuff);
	LocalFree(hBuff);

	LocalUnlock(hBuffout);
	LocalFree(hBuffout);

	cfileinpt.Close();
	cfileout.Close();
	
	return TRUE;
}
// ----------------------------------------------------------------------------------------------------
void CTMailSender::Encode(unsigned char input[3], unsigned char output[4], int len)
{
	static const CHAR BASE64CHARS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	output[0] = BASE64CHARS[ input[0] >> 2 ];
	output[1] = BASE64CHARS[ ((input[0] & 0x03) << 4) | ((input[1] & 0xf0) >> 4) ];
	output[2] = (unsigned char) (len>1?BASE64CHARS[ ((input[1] & 0x0f) << 2) | ((input[2] & 0xc0) >> 6) ] : '=');
	output[3] = (unsigned char) (len>2?BASE64CHARS[ input[2] & 0x3f ] : '=');
};
// ----------------------------------------------------------------------------------------------------
UINT CTMailSender::CharsNeededToEncode(UINT cb)
{
	if(!cb)
		return 0;

	int tRet = (cb/3)*4; 
	if ((cb % 3) !=0 )
		tRet = tRet + 4 ;

	return (tRet);
};
// ====================================================================================================


























