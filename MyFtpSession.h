#pragma once
#include <afxinet.h>
#include <vector>
#include <stack>

#define FTP_DEFAULT_PORT 21

struct FileProperty
{
	CString strFileName;
	CString strFileLastTime;
	CString strFileBit;
	DWORD dwFileType;
};


class MyFtpSession
{

public:

	MyFtpSession(void);

	MyFtpSession(CString strUserName,
				CString strPassWord,
				CString strServerAddr,
				INTERNET_PORT wServerPort = FTP_DEFAULT_PORT,
				BOOL bInitiativeMode = FALSE );

	~MyFtpSession(void);


	// ����FTP��������ַ���˿�
	void SetServerParam(CString strServerAddr, INTERNET_PORT wServerPort);

	// ���õ�¼��������
	void SetUserInfo(CString strUserName, CString strPassWord, BOOL bInitiativeMode = FALSE);

	// ���ӵ�ָ��FTP������
	BOOL ConnectToServer();

	// �������Ƿ����
	BOOL IsConnectionActive();

	// �ر�����
	void CloseConnection();

	// �ϴ�һ���ļ���FTP������
	BOOL PutFileToServer(CString strLocalFile, CString strPutPath);

	// ��FTP����������һ���ļ�
	BOOL DownLoadFileFromServer(CString strRemoteFile, CString strLocalPath, BOOL bFailIfExists = FALSE);

	// ��FTP����������һ���ļ�
	BOOL CreateFileInServer(CString strFileName);

	// ��FTP����������һ���ļ���
	BOOL CreateDirectoryInServer(CString strDirName);

	// ɾ��FTP��������һ���ļ�
	BOOL RemoveFileOrDirInServer(CString strFileName,BOOL isFile = TRUE);

	// ������FTP��������һ���ļ�
	BOOL RenameFileOrDirInServer(CString strExistName, CString strnewName);

	// �õ�ָ��Ŀ¼�µ������ļ�
	void GetFileList(CString strDirPath, std::vector<FileProperty>& vctFileList);

	// ��ȡ���һ��������Ϣ
	CString GetLastErrorMessage();

	// ��ȡ������
	CString GetHostName();

	// ��ȡ��ǰ·��
	CString GetCurPath();

	// ���õ�ǰ·��
	BOOL SetCurPath(CString);

	// ��ȡ��·��
	BOOL ReturnToParent();

	// ���ø�·��
	BOOL SetRootPath();

	// ����
	BOOL BackPrePath();

	// �ļ��Ƿ����
	BOOL IsFileExistence(CString FileName);

private:

	// FTP��������ַ
	CString m_strServerAddr;

	// ����˿�
	INTERNET_PORT m_wServerPort;

	// ��¼�û���
	CString m_strUserName;

	// ��¼����
	CString m_strPassWord;

	// ��Ŀ¼
	CString m_rootPath;

	// ����ģʽ-FALSE  ����ģʽ-TRUE
	BOOL m_bInitiativeMode;

	// ��¼���һ��������Ϣ
	CString m_strLastErrorMsg;



	// ��������ʼ��һ������ͬʱ��Internet �Ự
	CInternetSession m_cInetSession;

	// ������Internet��������FTP���Ӳ�����ֱ�Ӳ��ݷ������е�Ŀ¼���ļ�
	CFtpConnection* m_pFtpConn;

	std::stack<CString> m_strPrePathStack;

};
