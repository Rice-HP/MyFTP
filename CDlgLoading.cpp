// CDlgLoading.cpp: 实现文件
//

#include "pch.h"
#include "MyFTP.h"
#include "CDlgLoading.h"
#include "afxdialogex.h"
#include <chrono>


// CDlgLoading 对话框

IMPLEMENT_DYNAMIC(CDlgLoading, CDialogEx)

CDlgLoading::CDlgLoading(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIG_Loading, pParent)
{

}

CDlgLoading::CDlgLoading(MyFtpSession& ftp, CWnd* pParent)
	: CDialogEx(IDD_DIG_Loading, pParent), m_ptrFtp(&ftp)
{

}

CDlgLoading::~CDlgLoading()
{

}

void CDlgLoading::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_Gif, m_gifLoading);
}


BEGIN_MESSAGE_MAP(CDlgLoading, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgLoading::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgLoading 消息处理程序


void CDlgLoading::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	CDlgLoading::OnOK();
}

// 获取执行文件所有的路径
CString GetAbsolutePath()
{
	CString strTemp;
	TCHAR* pBuf = strTemp.GetBuffer(MAX_PATH);

	::GetModuleFileName(NULL, pBuf, MAX_PATH);
	strTemp.ReleaseBuffer();
	strTemp = strTemp.Left(strTemp.ReverseFind(_T('\\'))) + _T('\\');

	return strTemp;
}

BOOL CDlgLoading::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	CString strFile = GetAbsolutePath() + _T("loading.gif");
	if (m_gifLoading.Load(strFile))
	{
		m_gifLoading.Draw();
	}

	GetDlgItem(IDC_STATIC)->SetWindowTextW(m_ptrFtp->GetHostName() + L"正在连接...");
	//Sleep(1000);
	//m_ptrFtp->ConnectToServer();
	//m_pCnnThread = new std::thread(&CDlgLoading::ConnectToFtp,this);
	//
	//m_pCnnThread->detach();

	m_cnnThread = CreateThread(NULL, 0, ThreadCnnToServer, this, NULL, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


BOOL CDlgLoading::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类

	//MessageBox(L"!");
	//if (m_pCnnThread != nullptr)
	//{
	//	delete m_pCnnThread;
	//	m_pCnnThread = nullptr;
	//}
	//GetDlgItem(IDOK)->EnableWindow(TRUE);
	return CDialogEx::DestroyWindow();
}
//
//void CDlgLoading::ConnectToFtp()
//{
//	//std::this_thread::sleep_for(std::chrono::milliseconds(500));
//	if (m_ptrFtp->ConnectToServer())
//	{
//		m_isCnn = TRUE;
//		GetDlgItem(IDOK)->EnableWindow(FALSE);
//		CDialogEx::OnOK();
//
//	}
//	else
//	{
//		//CDialogEx::OnOK();
//	}
//
//}

DWORD __stdcall ThreadCnnToServer(LPVOID lpParam)
{
	CDlgLoading* pDlg = (CDlgLoading*)lpParam;

	//pDlg->OnBnClickedOk();
	DWORD res;
	if (pDlg->m_ptrFtp->ConnectToServer())
		res = 0;
	else
		res = -1;


	pDlg->PostMessage(WM_CLOSE);
	
	return res;
}
