#pragma once

class CTMailSender
{
protected:
	CString	m_strMailServer;
	CString m_strSndName;
	CString m_strSndMail;
	CString m_strRcvName;
	CString m_strRcvMail;
	CString m_strSubject;
	CString m_strMessage;
	CString m_strFile;

public:
	void SetMailServer(const CString& strMailServer);
	void SetSender(const CString& strMail, const CString& strName="");
	void SetReceiver(const CString& strMail, const CString& strName="");
	void SetSubject(const CString& strSubject);
	void SetMessage(const CString& strMessage);
	void SetFile(const CString& strFile);

	BOOL SendMail();

protected:
	CString GenerateMessageID(const CString& sHost);                
	CString GetMIMEHeader(const CString& vsBoundaryID, const CString& msgType); 
    CString CreateTmpAttachFile(LPCSTR flnm);
	CString MakeAttachmentHeader(const CString& vsBoundaryID, const CString& FullpathfileName);
	BOOL EncodeFileBase64(const CString& inputFl, const CString& outputFl);
	void Encode(unsigned char input[3], unsigned char output[4], int len);
	UINT CharsNeededToEncode(UINT cb);

public:
	CTMailSender();
	~CTMailSender();

};