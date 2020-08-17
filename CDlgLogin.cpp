// CDlgLogin.cpp: 实现文件
//

#include "pch.h"
#include "MyFTP.h"
#include "CDlgLogin.h"
#include "afxdialogex.h"
#include "MyFTPDlg.h"
#include "MyFtpSession.h"
#include "CDlgLoading.h"

// CDlgLogin 对话框

IMPLEMENT_DYNAMIC(CDlgLogin, CDialogEx)

CDlgLogin::CDlgLogin(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_Login, pParent)
	, m_host(_T(""))
	, m_user(_T(""))
	, m_passwd(_T(""))
{

}

CDlgLogin::~CDlgLogin()
{
}

void CDlgLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_Host, m_host);
	DDX_Text(pDX, IDC_EDIT_User, m_user);
	DDX_Text(pDX, IDC_EDIT_Passwd, m_passwd);
}


BEGIN_MESSAGE_MAP(CDlgLogin, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_CNN, &CDlgLogin::OnBnClickedBtnCnn)
END_MESSAGE_MAP()


// CDlgLogin 消息处理程序


void CDlgLogin::OnBnClickedBtnCnn()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);
	//MessageBox(m_host);

	MyFtpSession ftpCnn;
	ftpCnn.SetServerParam(m_host,21);
	ftpCnn.SetUserInfo(m_user,m_passwd);

	CDlgLoading dlgLoading(ftpCnn);

	dlgLoading.DoModal();

	CMyFTPDlg* dlg = nullptr;
	if (ftpCnn.IsConnectionActive())
	{
		this->ShowWindow(HIDE_WINDOW);
		
		dlg = new CMyFTPDlg(ftpCnn);

		if(dlg)
			dlg->DoModal();
	}
	else
	{
		MessageBox(L"连接失败~", L"提示");

		return ;
	}
	

	if (dlg->m_isLogout)
	{
		this->ShowWindow(SW_NORMAL);

		delete dlg;
		dlg = nullptr;
	}
	else
	{
		delete dlg;
		CDialogEx::OnOK();
	}
	
}


void CDlgLogin::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	if (MessageBox(L"确定要退出吗？", L"提示", MB_YESNO | MB_DEFBUTTON2) == IDYES)
	{
		CDialogEx::OnCancel();
		//exit(0);
	}

}


BOOL CDlgLogin::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化



	m_host = "192.168.19.1";
	m_user = "2360780968@qq.com";
	m_passwd = "18894899674.";

	//m_host = "39.108.67.249";
	//m_user = "testftp";
	//m_passwd = "134679";

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDlgLogin::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnOK();
	OnBnClickedBtnCnn();
}
