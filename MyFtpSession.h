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


	// 设置FTP服务器地址、端口
	void SetServerParam(CString strServerAddr, INTERNET_PORT wServerPort);

	// 设置登录名和密码
	void SetUserInfo(CString strUserName, CString strPassWord, BOOL bInitiativeMode = FALSE);

	// 连接到指定FTP服务器
	BOOL ConnectToServer();

	// 检测对象是否存在
	BOOL IsConnectionActive();

	// 关闭连接
	void CloseConnection();

	// 上传一个文件到FTP服务器
	BOOL PutFileToServer(CString strLocalFile, CString strPutPath);

	// 从FTP服务器下载一个文件
	BOOL DownLoadFileFromServer(CString strRemoteFile, CString strLocalPath, BOOL bFailIfExists = FALSE);

	// 在FTP服务器创建一个文件
	BOOL CreateFileInServer(CString strFileName);

	// 在FTP服务器创建一个文件夹
	BOOL CreateDirectoryInServer(CString strDirName);

	// 删除FTP服务器上一个文件
	BOOL RemoveFileOrDirInServer(CString strFileName,BOOL isFile = TRUE);

	// 重命名FTP服务器上一个文件
	BOOL RenameFileOrDirInServer(CString strExistName, CString strnewName);

	// 得到指定目录下的所有文件
	void GetFileList(CString strDirPath, std::vector<FileProperty>& vctFileList);

	// 获取最后一个错误信息
	CString GetLastErrorMessage();

	// 获取主机名
	CString GetHostName();

	// 获取当前路径
	CString GetCurPath();

	// 设置当前路径
	BOOL SetCurPath(CString);

	// 获取根路径
	BOOL ReturnToParent();

	// 设置根路径
	BOOL SetRootPath();

	// 返回
	BOOL BackPrePath();

	// 文件是否存在
	BOOL IsFileExistence(CString FileName);

private:

	// FTP服务器地址
	CString m_strServerAddr;

	// 服务端口
	INTERNET_PORT m_wServerPort;

	// 登录用户名
	CString m_strUserName;

	// 登录密码
	CString m_strPassWord;

	// 根目录
	CString m_rootPath;

	// 被动模式-FALSE  主动模式-TRUE
	BOOL m_bInitiativeMode;

	// 记录最后一个错误信息
	CString m_strLastErrorMsg;



	// 创建并初始化一个或多个同时的Internet 会话
	CInternetSession m_cInetSession;

	// 管理与Internet服务器的FTP连接并允许直接操纵服务器中的目录和文件
	CFtpConnection* m_pFtpConn;

	std::stack<CString> m_strPrePathStack;

};
