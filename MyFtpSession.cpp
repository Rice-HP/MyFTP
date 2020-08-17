#include "pch.h"
#include "MyFtpSession.h"
#include <algorithm>    // std::sort

MyFtpSession::MyFtpSession(void)
{
	m_pFtpConn = NULL;
	m_strServerAddr.Empty();
	m_wServerPort = FTP_DEFAULT_PORT;
	m_strUserName.Empty();
	m_strPassWord.Empty();
	m_bInitiativeMode = FALSE;
	m_strLastErrorMsg.Empty();

}

MyFtpSession::MyFtpSession(CString strUserName, CString strPassWord,CString strServerAddr,
	INTERNET_PORT wServerPort, BOOL bInitiativeMode) : MyFtpSession()
{
	SetUserInfo(strUserName, strPassWord, bInitiativeMode);
	SetServerParam(strServerAddr, wServerPort);
}



MyFtpSession::~MyFtpSession(void)
{
	CloseConnection();
}



// ����FTP��������ַ���˿�

void MyFtpSession::SetServerParam(CString strServerAddr, INTERNET_PORT wServerPort)
{
	m_strServerAddr = strServerAddr;
	m_wServerPort = wServerPort;

}



// ���õ�¼��������

void MyFtpSession::SetUserInfo(CString strUserName, CString strPassWord, BOOL bInitiativeMode)
{
	m_strUserName = strUserName;
	m_strPassWord = strPassWord;
	m_bInitiativeMode = bInitiativeMode;
}



// ���ӵ�ָ��FTP������

BOOL MyFtpSession::ConnectToServer()
{
	try
	{
		CloseConnection();

		// m_bInitiativeMode = TRUE Ϊ����ģʽ,һ����false
		m_pFtpConn = m_cInetSession.GetFtpConnection(
			m_strServerAddr,
			m_strUserName, 
			m_strPassWord, 
			m_wServerPort, 
			m_bInitiativeMode );

		if (NULL != m_pFtpConn)
		{
			m_pFtpConn->GetCurrentDirectory(m_rootPath);
			return TRUE;
		}	

	}
	catch (CInternetException * e)
	{

		TCHAR szError[1024];

		if (e->GetErrorMessage(szError, 1024))
			AfxMessageBox(szError);
		else
			AfxMessageBox(L"There was an exception");

		e->Delete();

	}

	return FALSE;

}


void MyFtpSession::GetFileList(CString strDirPath, std::vector<FileProperty>& vctFileList)
{
	// �õ�ָ��Ŀ¼�µ������ļ�

	if (NULL == m_pFtpConn
		&& !ConnectToServer())

		return;


	vctFileList.clear();
	CFtpFileFind cFtpFileFind(m_pFtpConn);

	// ����ָ��Ŀ¼
	BOOL bWorking = cFtpFileFind.FindFile(strDirPath);
	while (bWorking)
	{

		bWorking = cFtpFileFind.FindNextFile();

		// .����..
		if (cFtpFileFind.IsDots())
			continue;

		FileProperty curFile;
		// �õ��ļ���
		curFile.strFileName = cFtpFileFind.GetFileName();

		curFile.dwFileType = 0;

		//ʱ��
		FILETIME ftime;
		cFtpFileFind.GetLastWriteTime(&ftime);

		SYSTEMTIME systime;
		FileTimeToSystemTime(&ftime, &systime);
		CString timeStr;
		timeStr.Format(L"%d-%0.2d-%0.2d %0.2d:%0.2d", systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute);
		curFile.strFileLastTime = timeStr;

		// �ж��Ƿ�δ�ļ���		
		if (cFtpFileFind.IsDirectory())
		{
			curFile.strFileBit = "-";
			curFile.dwFileType = 0;	// �ļ���	

		}
		else
		{
			//��С
			char szBuf[1024];
			LONG lsize = cFtpFileFind.GetLength();
			if (lsize >= 1024)
			{
				_ltoa_s(lsize / 1024, szBuf, 10);
				curFile.strFileBit = CString(szBuf);
				curFile.strFileBit += "K";
			}
			else
			{
				_ltoa_s(lsize, szBuf, 10);
				curFile.strFileBit = CString(szBuf);
				curFile.strFileBit += "B";
			}	

			if(curFile.strFileName.Find(L".txt", curFile.strFileName.GetLength() - 4) != -1)
				curFile.dwFileType = 1;	// txt�ļ�	
			else if (curFile.strFileName.Find(L".exe", curFile.strFileName.GetLength() - 4) != -1)
				curFile.dwFileType = 3;	// 	Ӧ���ļ�	
			else if (curFile.strFileName.Find(L".c", curFile.strFileName.GetLength() - 2) != -1)
				curFile.dwFileType = 2;	// 	c�ļ�
			else
				curFile.dwFileType = 4;	// 	�����ļ�	

		}

		// ��ӽ��ļ��б�
		vctFileList.push_back(curFile);

		//// �ļ���
		//if (cFtpFileFind.IsDirectory())
		//	curFile.dwFileType |= FILE_ATTRIBUTE_DIRECTORY;
		//else
		//	curFile.dwFileType |= FILE_ATTRIBUTE_NORMAL;	// ��ͨ�ļ�

	}

	auto myfunction = [](FileProperty a, FileProperty b)->bool { return (a.dwFileType < b.dwFileType);  };
	std::sort(vctFileList.begin(), vctFileList.end(), myfunction);

}




BOOL MyFtpSession::PutFileToServer(CString strLocalFile, CString strRemoteFile)
{
	// �ϴ�һ���ļ���FTP������

	if (NULL == m_pFtpConn && !ConnectToServer())
		return FALSE;

	return m_pFtpConn->PutFile(strLocalFile, strRemoteFile);
}



BOOL MyFtpSession::DownLoadFileFromServer(CString strRemoteFile, CString strLocalFile, BOOL bFailIfExists)
{
	if (NULL == m_pFtpConn && !ConnectToServer())
		return FALSE;

	return m_pFtpConn->GetFile(strRemoteFile, strLocalFile, bFailIfExists);

}

BOOL MyFtpSession::CreateDirectoryInServer(CString strDirName)
{
	if (NULL == m_pFtpConn && !ConnectToServer())
		return FALSE;

	return m_pFtpConn->CreateDirectory(strDirName);
}

BOOL MyFtpSession::RemoveFileOrDirInServer(CString strFileName, BOOL isFile)
{
	if (NULL == m_pFtpConn && !ConnectToServer())
		return FALSE;

	if(isFile)
		return m_pFtpConn->Remove(strFileName);
	return m_pFtpConn->RemoveDirectoryW(strFileName);
}

BOOL MyFtpSession::RenameFileOrDirInServer(CString strExistName, CString strnewName)
{
	if (NULL == m_pFtpConn && !ConnectToServer())
		return FALSE;
	return m_pFtpConn->Rename(strExistName, strnewName);
}



void MyFtpSession::CloseConnection()
{
	// �ر�����

	if (NULL != m_pFtpConn)
	{
		m_pFtpConn->Close();
		delete m_pFtpConn;
	}

	m_pFtpConn = NULL;
}




BOOL MyFtpSession::IsConnectionActive()
{
	// �������Ƿ����

	if (NULL == m_pFtpConn)
		return FALSE;

	return TRUE;
}



// ��ȡ���һ��������Ϣ

CString MyFtpSession::GetLastErrorMessage()
{
	m_strLastErrorMsg.Format(_T("�����룺%ld"), ::GetLastError());

	return m_strLastErrorMsg;
}

CString MyFtpSession::GetHostName()
{
	CString strHost(m_strServerAddr);
	CString strPort;
	strPort.Format(L" : %d", m_wServerPort);

	return strHost + strPort;
}

CString MyFtpSession::GetCurPath()
{
	CString curPath;
	if (m_pFtpConn->GetCurrentDirectory(curPath))
		return curPath;

	return CString();
}

BOOL MyFtpSession::SetCurPath(CString strPath)
{
	CString curStr = GetCurPath();
	m_strPrePathStack.push(curStr);
	return m_pFtpConn->SetCurrentDirectory(strPath);
}

BOOL MyFtpSession::ReturnToParent()
{
	CString curStr = GetCurPath();
	m_strPrePathStack.push(curStr);

	CString curPath;
	m_pFtpConn->GetCurrentDirectory(curPath);
	if (curPath == m_rootPath)
		return FALSE;
	return m_pFtpConn->SetCurrentDirectory(L"../");
}

BOOL MyFtpSession::SetRootPath()
{
	CString curStr = GetCurPath();
	m_strPrePathStack.push(curStr);

	CString curPath;
	m_pFtpConn->GetCurrentDirectory(curPath);
	if (curPath == m_rootPath)
		return FALSE;
	return m_pFtpConn->SetCurrentDirectory(m_rootPath);
}

BOOL MyFtpSession::BackPrePath()
{
	if(m_strPrePathStack.empty())
		return FALSE;

	if (m_strPrePathStack.top().GetLength() == 0)
	{
		m_strPrePathStack.pop();
		return FALSE;
	}

	if (!m_pFtpConn->SetCurrentDirectory(m_strPrePathStack.top()))
	{
		m_strPrePathStack.pop();
		return FALSE;
	}

	m_strPrePathStack.pop();
	return TRUE;
}

BOOL MyFtpSession::IsFileExistence(CString FileName)
{

	if (NULL == m_pFtpConn && !ConnectToServer())
		return FALSE;


	CFtpFileFind cFtpFileFind(m_pFtpConn);

	// ����ָ��Ŀ¼
	BOOL bWorking = cFtpFileFind.FindFile(GetCurPath());
	while (bWorking)
	{

		bWorking = cFtpFileFind.FindNextFile();

		// .����..
		if (cFtpFileFind.IsDots())
			continue;

		if (cFtpFileFind.GetFileName() == FileName)
			return TRUE;
	}
	return FALSE;
}
