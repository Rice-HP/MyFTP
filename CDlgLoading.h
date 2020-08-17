#pragma once
#include<thread>
#include "CPictureEx.h"
#include "MyFtpSession.h"
// CDlgLoading 对话框

class CDlgLoading : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLoading)

public:
	CDlgLoading(CWnd* pParent = nullptr);   // 标准构造函数

	CDlgLoading(MyFtpSession& ftp,CWnd* pParent = nullptr);   // 自定义
	virtual ~CDlgLoading();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIG_Loading };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	//void ConnectToFtp();
	MyFtpSession* m_ptrFtp; //ftp自定义类
private:
	CPictureEx m_gifLoading; //gif

	BOOL m_isCnn;
	//std::thread* m_pCnnThread;

	// 子线程句柄
	HANDLE m_cnnThread;

};

DWORD WINAPI ThreadCnnToServer(LPVOID lpParam);