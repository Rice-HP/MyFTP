
// MyFTPDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MyFTP.h"
#include "MyFTPDlg.h"
#include "afxdialogex.h"
#include "MyFtpSession.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMyFTPDlg 对话框



CMyFTPDlg::CMyFTPDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_MYFTP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_Main);
}

CMyFTPDlg::CMyFTPDlg(MyFtpSession& ftp, CWnd* pParent)
	: CDialog(IDD_MYFTP_DIALOG, pParent), m_ptrFtp(&ftp)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_Main); 
	//this->SetWindowTextW(m_ptrFtp->GetHostName());
	//AfxGetMainWnd()->SetWindowText(L"??");
	//MessageBox(m_ptrFtp->GetHostName());
	m_imgList.Create(32, 32, ILC_COLOR32 | ILC_MASK, 0, 0);//图标,去黑背景

	m_imgList.Add(AfxGetApp()->LoadIconW(IDI_ICON_Dir));
	m_imgList.Add(AfxGetApp()->LoadIconW(IDI_ICON_TxtFile));
	m_imgList.Add(AfxGetApp()->LoadIconW(IDI_ICON_CFileIco));
	m_imgList.Add(AfxGetApp()->LoadIconW(IDI_ICON_EXE));
	m_imgList.Add(AfxGetApp()->LoadIconW(IDI_ICON_Other));

	TCHAR MyDir[512];
	SHGetSpecialFolderPath(0, MyDir, CSIDL_DESKTOPDIRECTORY, 0);
	m_strSavaPath = MyDir;
}

void CMyFTPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST1, m_listCtrl);
	DDX_Control(pDX, IDC_EDIT_RenameEdit, m_editRename);
}

BEGIN_MESSAGE_MAP(CMyFTPDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_Logout, &CMyFTPDlg::OnBnClickedBtnLogout)
	ON_BN_CLICKED(IDC_BTN_Exit, &CMyFTPDlg::OnBnClickedBtnExit)
	ON_COMMAND(ID_32794, &CMyFTPDlg::OnAboutShow)
	ON_COMMAND(ID_32789, &CMyFTPDlg::OnSmallICON)
	ON_COMMAND(ID_32790, &CMyFTPDlg::OnReportView)
	ON_COMMAND(ID_32787, &CMyFTPDlg::OnChangeFilePath)
	ON_EN_KILLFOCUS(IDC_EDIT_RenameEdit, &CMyFTPDlg::OnEnKillfocusEditRenameedit)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CMyFTPDlg::OnNMR_RighClickList)
	ON_COMMAND(ID_32796, &CMyFTPDlg::OnMenuSmall)
	ON_COMMAND(ID_32797, &CMyFTPDlg::OnMenuList)
	ON_COMMAND(ID_32798, &CMyFTPDlg::OnItemSmallIcon)
	ON_COMMAND(ID_32799, &CMyFTPDlg::OnItemList)
	ON_COMMAND(ID_ITEM_32771, &CMyFTPDlg::OnItemDownLoad)
	ON_COMMAND(ID_ITEM_32773, &CMyFTPDlg::OnItemSaveAs)
	ON_COMMAND(ID_ITEM_32774, &CMyFTPDlg::OnItemRenameFile)
	ON_COMMAND(ID_ITEM_32775, &CMyFTPDlg::OnUpLoad)
	ON_COMMAND(ID_ITEM_32776, &CMyFTPDlg::OnRemove)
	ON_COMMAND(ID_ITEM_32777, &CMyFTPDlg::OnCreateDirectory)
	ON_COMMAND(ID_ITEM_32781, &CMyFTPDlg::OnReturnToParentDir)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CMyFTPDlg::OnNMDblclkList_ItemDClick)
	ON_COMMAND(ID_NONE_32779, &CMyFTPDlg::OnNoneUpLoad)
	ON_COMMAND(ID_NONE_32780, &CMyFTPDlg::OnNoneCreateDirectory)
	ON_COMMAND(ID_NONE_32795, &CMyFTPDlg::OnNoneReturnToParentDir)
	ON_COMMAND(ID_NONE_32802, &CMyFTPDlg::OnNoneReturnToRoot)
	ON_COMMAND(ID_32801, &CMyFTPDlg::OnMenuSavePath)
	ON_COMMAND(ID_BUTTON_Home, &CMyFTPDlg::OnMainToorbarHome)
	ON_COMMAND(ID_BUTTON_Up, &CMyFTPDlg::OnMainToorbarUp)
	ON_COMMAND(ID_BUTTON_Back, &CMyFTPDlg::OnMainToorbarBack)
	ON_WM_SIZE()
	ON_COMMAND(ID_32786, &CMyFTPDlg::OnMainMenuUpLoad)
	ON_COMMAND(ID_32788, &CMyFTPDlg::OnMainMenuCreateDir)
	ON_COMMAND(ID_32803, &CMyFTPDlg::OnMainMenuOpenSaveDir)
	ON_COMMAND(ID_32791, &CMyFTPDlg::OnMainMenuUnlog)
	ON_COMMAND(ID_32804, &CMyFTPDlg::OnMainMenuExit)
	ON_COMMAND(ID_32793, &CMyFTPDlg::OnMainMenuInstructions)
END_MESSAGE_MAP()


// CMyFTPDlg 消息处理程序

BOOL CMyFTPDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	this->SetWindowTextW(m_ptrFtp->GetHostName());  //设置标题

	m_listCtrl.SetImageList(&m_imgList, LVSIL_SMALL); //设置图标
	m_listCtrl.SetImageList(&m_imgList, LVSIL_NORMAL);  //设置大图标

	UpdataList(); //更新列表

	m_editRename.ShowWindow(SW_HIDE);  //隐藏

	if (!m_toolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP | CBRS_GRIPPER |
		CBRS_TOOLTIPS) || !m_toolBar.LoadToolBar(IDR_TOOLBAR1))
	{
		AfxMessageBox(TEXT("Failed to create toolbar!"), NULL, NULL);
		return FALSE;
	}

	//添加工具栏
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);


	CRect rect;
	GetClientRect(rect);
	static UINT indicators[] = { IDS_STATSTR1,IDS_STATSTR2 };
	if (!m_Statusbar.Create(this) || !m_Statusbar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT)))
	{
		TRACE0("Can't create status bar\n");

		return FALSE;

	}

	//添加状态栏信息
	m_Statusbar.MoveWindow(1, rect.bottom - 20, rect.right, 20);// 调整状态栏的位置和大小
	m_Statusbar.SetPaneInfo(0, m_Statusbar.GetDlgCtrlID(), SBPS_NORMAL, 50);
	m_Statusbar.SetPaneInfo(1, m_Statusbar.GetDlgCtrlID(), SBPS_NORMAL, 45);

	//m_Statusbar.SetPaneText(1,L"下载中");
	//m_Statusbar.SetPaneInfo(2, IDS_PROGRESS, SBPS_NORMAL, 200); ,IDS_PROGRESS
	//m_Statusbar.SetPaneInfo(1, IDS_PROGRESS, SBPS_NORMAL, 60);
	//m_Statusbar.SetPaneText(1, L"进度条");

	//m_Progress.Create(WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDS_PROGRESS);
	//VERIFY(m_Progress.Create(WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDS_PROGRESS));

	m_Progress.Create(PBS_SMOOTH | WS_CHILD | WS_VISIBLE, CRect(rect.left+110, rect.top, rect.right, rect.bottom), &m_Statusbar, IDS_PROGRESS);     //创建进度条 

	m_Progress.SetRange(1, 100);
	m_Progress.SetPos(0);
	//m_Progress.ShowWindow(SW_HIDE);
	//m_Progress.ShowWindow(SW_NORMAL);
	//m_Progress.SetRange(1, 60);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMyFTPDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMyFTPDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMyFTPDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMyFTPDlg::OnBnClickedBtnLogout()
{
	// TODO: 注销
	m_isLogout = TRUE;
	firstUpdate = FALSE;
	CDialog::OnOK();

}


void CMyFTPDlg::OnBnClickedBtnExit()
{
	// TODO: 退出
	if (MessageBox(L"确定要退出吗？", L"提示", MB_YESNO | MB_DEFBUTTON2) == IDYES)
	{
		m_isLogout = false;
		CDialog::OnOK();
	}
}


void CMyFTPDlg::OnAboutShow()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}



void CMyFTPDlg::OnSmallICON()
{
	// TODO: 小图标

	//m_listCtrl.DeleteAllItems(); // 全部清空
	//while (m_listCtrl.DeleteColumn(0));  //删除所有列

	LONG lStyle;
	lStyle = GetWindowLong(m_listCtrl.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位
	lStyle |= LVS_ICON; //设置style 小图标

	SetWindowLong(m_listCtrl.m_hWnd, GWL_STYLE, lStyle);//设置style

	m_isList = FALSE;
	UpdataList();
}


void CMyFTPDlg::OnReportView()
{
	// TODO: 报表风格

	//m_listCtrl.DeleteAllItems(); // 全部清空
	//while (m_listCtrl.DeleteColumn(0));  //删除所有列
	//listCtrl.SetImageList(nullptr, LVSIL_SMALL);  //取消图片

	LONG lStyle;
	lStyle = GetWindowLong(m_listCtrl.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位
	lStyle |= LVS_REPORT; //设置style 报表

	SetWindowLong(m_listCtrl.m_hWnd, GWL_STYLE, lStyle);//设置style

	//设置格式,选中高亮+网格
	m_listCtrl.SetExtendedStyle(m_listCtrl.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);


	m_isList = TRUE;
	UpdataList();
}


void CMyFTPDlg::OnChangeFilePath()
{
	// TODO: 选择文件路径

	CFolderPickerDialog fd(NULL, 0, this, 0);
	if (fd.DoModal() == IDOK)
	{
		CString des("null");
		des = fd.GetPathName();

		//MessageBox(des);
		m_strSavaPath = des;

		CMenu* subMenu = AfxGetMainWnd()->GetMenu()->GetSubMenu(3); // 0：第一列菜单
		if(subMenu)
			subMenu->ModifyMenu(0, MF_BYPOSITION, IDR_MENU_Main, m_strSavaPath); // 0：第一列菜单下第1个子菜单
	}

	this->SetFocus();
}

BOOL CMyFTPDlg::IsFileExistence(CString FileName, BOOL IsRemote)
{
	if (IsRemote) //远程服务是否
	{
		//return m_ptrFtp->IsFileExistence(FileName);

		for (int i = 0; i < m_listCtrl.GetItemCount(); i++)
		{
			if (m_listCtrl.GetItemText(i, 0) == FileName)
			{
				return TRUE;
			}
		}

	}
	else //本地是否存在
	{

		CFileFind find;
		BOOL IsFind = find.FindFile(m_strSavaPath + _T("/*.*"));

		while (IsFind)
		{
			IsFind = find.FindNextFile();
			if (find.IsDots())
			{
				continue;
			}
			else
			{
				if (find.GetFileName() == FileName)
					return TRUE;								
			}
		}

	}

	return FALSE;
}

void CMyFTPDlg::UpdataList()
{
	m_listCtrl.DeleteAllItems(); // 全部清空
	while (m_listCtrl.DeleteColumn(0));  //删除所有列

	std::vector<FileProperty> fileList;

	m_ptrFtp->GetFileList(m_ptrFtp->GetCurPath(), fileList);

	if (fileList.empty())
	{
		//MessageBox(L"获取文件列表异常~",L"提示");
		return;
	}

	//小图标
	if (!m_isList)
	{
		int i = 0;
		for (auto it : fileList)
		{
			m_listCtrl.InsertItem(i, it.strFileName, it.dwFileType);
			i++;
		}
	}
	else
	{
		static CString strHead[4] = { TEXT("文件名称"), TEXT("文件类型"), TEXT("文件大小"), TEXT("修改时间") };

		//添加表头 InsertCol
		m_listCtrl.InsertColumn(0, strHead[0], LVCFMT_CENTER, 250);
		m_listCtrl.InsertColumn(1, strHead[1], LVCFMT_CENTER, 100);
		m_listCtrl.InsertColumn(2, strHead[2], LVCFMT_CENTER, 150);
		m_listCtrl.InsertColumn(3, strHead[3], LVCFMT_CENTER, 150);

		static CString fileType[5] = { TEXT("文件夹"), TEXT("文本文件"), TEXT("C 文件"), TEXT("应用程序"),TEXT("其他文件") };

		int i = 0;
		for (auto it : fileList)
		{
			m_listCtrl.InsertItem(i, it.strFileName, it.dwFileType);  //文件名与icon
			m_listCtrl.SetItemText(i, 1, fileType[it.dwFileType]);
			m_listCtrl.SetItemText(i, 2, it.strFileBit);
			m_listCtrl.SetItemText(i, 3, it.strFileLastTime);
			i++;
		}
	}

	//for (int i = 0; i < 30; i++)
	//{
	//	m_listCtrl.InsertItem(i, CString('a', i),i%3);
	//}

	//m_listCtrl.SetImageList(&m_imgList, LVSIL_NORMAL);//小图标
}


void CMyFTPDlg::OnEnKillfocusEditRenameedit()
{
	// TODO: 在此添加控件通知处理程序代码
	CString tem;
	m_editRename.GetWindowText(tem); //得到用户输入的新的内容

	BOOL IsChange = m_listCtrl.GetItemText(m_nRow, 0) != tem;
	if (IsChange && m_ptrFtp->IsFileExistence(tem))
	{
		MessageBox(tem + L"服务器已存在~", L"提示",  MB_DEFBUTTON2);
		SetFocus();
	}
	else if (IsChange && !m_ptrFtp->RenameFileOrDirInServer(m_listCtrl.GetItemText(m_nRow, 0), tem))//重命名
	{
		MessageBox(m_ptrFtp->GetLastErrorMessage(),L" 创建错误");  //删除文件夹
	}
	else
	{
		m_listCtrl.SetItemText(m_nRow, 0, tem); //设置编辑框的新内容
	}

	m_editRename.ShowWindow(SW_HIDE); //应藏编辑框

	m_nRow = -1;
}


void CMyFTPDlg::OnNMR_RighClickList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	// TODO: 右键菜单
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1)
	{
		CPoint pt;
		GetCursorPos(&pt);
		CMenu menu;
		menu.LoadMenu(IDR_MENU_ItemRight);
		CMenu* popmenu;
		popmenu = menu.GetSubMenu(0);
		popmenu->TrackPopupMenu(TPM_RIGHTBUTTON, pt.x, pt.y, this);
	}
	else  //未选中item
	{
		CPoint pt;
		GetCursorPos(&pt);
		CMenu menu;
		menu.LoadMenu(IDR_MENU_None);
		CMenu* popmenu;
		popmenu = menu.GetSubMenu(0);
		popmenu->TrackPopupMenu(TPM_RIGHTBUTTON, pt.x, pt.y, this);
	}

	*pResult = 0;
}


void CMyFTPDlg::OnMenuSmall()
{
	// TODO: 小图标
	OnSmallICON();
}


void CMyFTPDlg::OnMenuList()
{
	// TODO: 详细列表
	OnReportView();
}


void CMyFTPDlg::OnItemSmallIcon()
{
	// TODO: 小图标
	OnSmallICON();
}


void CMyFTPDlg::OnItemList()
{
	// TODO: 详细列表
	OnReportView();
}


void CMyFTPDlg::OnItemDownLoad()
{
	// TODO: 下载

	int ncount = 0;
	POSITION pos = m_listCtrl.GetFirstSelectedItemPosition();
	if (pos == NULL)
		MessageBox(L"No items were selected!/n");
	else
	{
		m_Progress.SetRange(1, m_listCtrl.GetSelectedCount());
		m_Statusbar.SetPaneText(1, L"下载中");
		while (pos)
		{
			int nItem = m_listCtrl.GetNextSelectedItem(pos);
			CString str;
			//str.Format(L"Item %d was selected!/n", nItem);

			// 处理选中的item

			str = m_listCtrl.GetItemText(nItem, 0);  //文件名

			if (IsFileExistence(str))
			{
				if (MessageBox(str + L"本地已存在,是否覆盖？", L"提示", MB_YESNO | MB_DEFBUTTON2) == IDNO)
				{
					continue;
				}
			}

			if(!m_ptrFtp->DownLoadFileFromServer(m_ptrFtp->GetCurPath()+ "/" + str, m_strSavaPath + "/" + str,0))
			{
				if (str.Find(L".") == -1)
				{
					MessageBox(L"不可下载文件夹~", L"提示");
					return;
				}
				else
					MessageBox(m_ptrFtp->GetLastErrorMessage() + L" 下载发生错误~", L"错误");
				break;
			}
			m_Progress.SetPos(++ncount);
			//ncount++;
		}
	}

	CString str;
	str.Format(L"一共 %d 个文件下载完成!\n 路径:", ncount);

	m_Statusbar.SetPaneText(1, L"正常");
	MessageBox(str + m_strSavaPath);
	m_Progress.SetPos(0);
}


void CMyFTPDlg::OnItemSaveAs()
{
	// TODO: 另存为

	CFolderPickerDialog fd(NULL, 0, this, 0);
	if (fd.DoModal() == IDOK)
	{
		CString des("null");
		des = fd.GetPathName();

		int ncount = 0;
		POSITION pos = m_listCtrl.GetFirstSelectedItemPosition();
		if (pos == NULL)
			MessageBox(L"No items were selected!/n");
		else
		{
			m_Progress.SetRange(1, m_listCtrl.GetSelectedCount());
			m_Statusbar.SetPaneText(1, L"下载中");
			while (pos)
			{
				int nItem = m_listCtrl.GetNextSelectedItem(pos);
				CString str;
				//str.Format(L"Item %d was selected!/n", nItem);

				// 处理选中的item

				str = m_listCtrl.GetItemText(nItem, 0);  //文件名

				//MessageBox(str);

				if (IsFileExistence(str))
				{
					if (MessageBox(str + L"本地已存在,是否覆盖？", L"提示", MB_YESNO | MB_DEFBUTTON2) == IDNO)
					{
						continue;
					}
				}

				if (!m_ptrFtp->DownLoadFileFromServer(m_ptrFtp->GetCurPath() + "/" + str, des + "/" + str))
				{
					MessageBox(m_ptrFtp->GetLastErrorMessage() + L" 下载发生错误~",L"错误");
					break;
				}
				//ncount++;
				m_Progress.SetPos(++ncount);
			}
		}

		CString str;
		str.Format(L"一共 %d 个文件下载完成!\n 路径:", ncount);
		m_Statusbar.SetPaneText(1, L"正常");
		MessageBox(str + des);
		m_Progress.SetPos(0);
	}
}


void CMyFTPDlg::OnItemRenameFile()
{
	// TODO: 重命名
	POSITION pos = m_listCtrl.GetFirstSelectedItemPosition();
	CRect rc;
	m_nRow = m_listCtrl.GetNextSelectedItem(pos);//获得选中的行

	if (m_nRow >= 0) //如果选择的是子项;
	{
		m_listCtrl.GetSubItemRect(m_nRow, 0, LVIR_LABEL, rc);//获得子项的RECT；  
		m_editRename.SetParent(&m_listCtrl);//转换坐标为列表框中的坐标  
		m_editRename.MoveWindow(rc);//移动Edit到RECT坐在的位置;  
		m_editRename.SetWindowText(m_listCtrl.GetItemText(m_nRow, 0));//将该子项中的值放在Edit控件中；  
		m_editRename.ShowWindow(SW_SHOW);//显示Edit控件；  
		m_editRename.SetFocus();//设置Edit焦点  
		m_editRename.ShowCaret();//显示光标  
		m_editRename.SetSel(-1);//将光标移动到最后  

	}
}


void CMyFTPDlg::OnUpLoad()
{
	 //TODO: 上传

	//CString filename;//保存路径
	//CFileDialog opendlg(TRUE, _T("*"), _T("*.*"), OFN_OVERWRITEPROMPT| OFN_ALLOWMULTISELECT | OFN_EXPLORER, _T("所有文件(*.*;)|*.*||"), NULL);
	//if (opendlg.DoModal() == IDOK)
	//{
	//	filename = opendlg.GetPathName();

	//	//MessageBox(filename);

	//	if (!m_ptrFtp->PutFileToServer(filename, m_ptrFtp->GetCurPath() + "/" + opendlg.GetFileName()))
	//	{
	//		MessageBox(L"上传发生错误~",L"错误");
	//	}
	//}
	//UpdataList();

	CFileDialog Filedlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT | OFN_EXPLORER, _T("所有文件(*.*)|*.*||"));
	// Create buffer for file names.
	const DWORD numberOfFileNames = 32;//最多允许32个文件
	const DWORD fileNameMaxLength = MAX_PATH + 1;
	const DWORD bufferSize = (numberOfFileNames * fileNameMaxLength) + 1;
	TCHAR* filenamesBuffer = new TCHAR[bufferSize];
	// Initialize beginning and end of buffer.
	filenamesBuffer[0] = NULL;//必须的
	filenamesBuffer[bufferSize - 1] = NULL;

	// Attach buffer to OPENFILENAME member.
	Filedlg.m_ofn.lpstrFile = filenamesBuffer;
	Filedlg.m_ofn.nMaxFile = bufferSize;

	int iCtr = 0;
	if (IDOK == Filedlg.DoModal())
	{
		CString filePath;
		POSITION pos = Filedlg.GetStartPosition();

		m_Progress.SetRange(1, 32);
		m_Statusbar.SetPaneText(1, L"上传中");

		while (pos != NULL)
		{
			filePath = Filedlg.GetNextPathName(pos);//返回选定文件文件名// Retrieve file name(s).
			//MessageBox(aviName);
			//MessageBox(Filedlg.GetPathName());

			//MessageBox(fileName.Mid(Filedlg.GetPathName().GetLength()));

			int firstPos = Filedlg.GetPathName().GetLength() + 1;
			CString fileName;
			if (firstPos >= filePath.GetLength())  //单个文件
			{
				fileName = Filedlg.GetFileName();   //单个文件可直接获取,多个不行
			}
			else
				fileName = filePath.Mid(firstPos);
			//MessageBox(fileName);

			if (IsFileExistence(fileName, TRUE))
			{
				if (MessageBox(fileName + L"服务器已存在,是否覆盖？", L"提示", MB_YESNO | MB_DEFBUTTON2) == IDNO)
				{
					continue;
				}
			}

			if (!m_ptrFtp->PutFileToServer(filePath, m_ptrFtp->GetCurPath() + "/" + fileName))
			{
				MessageBox(m_ptrFtp->GetLastErrorMessage() + L" 上传发生错误~",L"错误");
				UpdataList();

				CString str;
				str.Format(L"一共 %d 个文件上传完成!\n ", iCtr);
				MessageBox(str, L"提示");

				return;
			}

			//iCtr++;
			m_Progress.SetPos(++iCtr);
		}

		UpdataList();

		CString str;

		m_Statusbar.SetPaneText(1, L"正常");
		str.Format(L"一共 %d 个文件上传完成!\n ", iCtr);
		MessageBox(str, L"提示");
		m_Progress.SetPos(0);
	}


	delete[] filenamesBuffer;//Release file names buffer.

}


void CMyFTPDlg::OnRemove()
{
	// TODO: 删除文件
	POSITION pos = m_listCtrl.GetFirstSelectedItemPosition();

	m_Statusbar.SetPaneText(1, L"删除中");

	while (pos)
	{
		//int nItem = m_listCtrl.GetNextSelectedItem(pos);
		int nRow = m_listCtrl.GetNextSelectedItem(pos);//获得选中的行

		CString fileName = m_listCtrl.GetItemText(nRow, 0);

		if (fileName.Find(L".", 0) == -1)
		{
			if (MessageBox(L"确定删除 " + fileName + " 文件夹?", L"提示", MB_YESNO | MB_DEFBUTTON2) == IDYES)
			{
				if (!m_ptrFtp->RemoveFileOrDirInServer(m_ptrFtp->GetCurPath() + "/" + fileName, 0))
				{
					MessageBox(m_ptrFtp->GetLastErrorMessage()+L" 删除失败~目录不为空~", L"错误");  //删除文件夹

				}
			}
		}
		else
		{
			if (MessageBox(L"确定删除 " + fileName + " 文件?", L"提示", MB_YESNO | MB_DEFBUTTON2) == IDYES)
			{
				if (!m_ptrFtp->RemoveFileOrDirInServer(m_ptrFtp->GetCurPath() + "/" + fileName))
					MessageBox(m_ptrFtp->GetLastErrorMessage() + L" 删除失败~", L"错误");  //删除文件夹
			}
		}
	}
	m_Statusbar.SetPaneText(1, L"正常");

	UpdataList();
}

void CMyFTPDlg::OnCreateDirectory()
{
	// TODO: 创建文件夹
	CString dirName;
	int index = 1;


	while (1)
	{
		dirName.Format(L"新键文件夹(%d)", index++);

		bool isExistence = false;

		for (int i = 0; i < m_listCtrl.GetItemCount(); i++)
		{
			if (m_listCtrl.GetItemText(i, 0) == dirName)
			{	
				isExistence = true;
				break;
			}
		}

		if (!isExistence)
			break;
	}

	//MessageBox(dirName);

	if (!m_ptrFtp->CreateDirectoryInServer(m_ptrFtp->GetCurPath() + "/" + dirName))
	{
		MessageBox(m_ptrFtp->GetLastErrorMessage() + L" 创建发生错误~", L"错误");
	}

	UpdataList();

	for (int i = 1; i < m_listCtrl.GetItemCount(); i++)
	{
		if (m_listCtrl.GetItemText(i, 0) == dirName)
		{
			index = i;
			break;
		}
	}
	//m_listCtrl.f
	m_listCtrl.SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	OnItemRenameFile();

	
}


void CMyFTPDlg::OnReturnToParentDir()
{
	// TODO: 返回上级目录
	//if (m_ptrFtp->GetCurPath() == "/")
	//{
	//	MessageBox(L"已在根目录~", L"提示");
	//	return;
	//}

	if (!m_ptrFtp->ReturnToParent())
	{
		//MessageBox(L"返回发生错误~", L"错误");
		MessageBox(L"已在根目录~", L"提示");
		return;
	}
	UpdataList();
}


void CMyFTPDlg::OnNMDblclkList_ItemDClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int nRow = pNMListView->iItem;

	CString fileName = m_listCtrl.GetItemText(nRow, 0);

	if (fileName.Find(L".", 0) == -1)
	{
		if (m_ptrFtp->SetCurPath(m_ptrFtp->GetCurPath() + "/" + fileName))
		{	
			UpdataList();
		}
	}
	else
	{
		if (MessageBox(L"确定下载 " + fileName + " 文件?", L"提示", MB_YESNO | MB_DEFBUTTON2) == IDYES)
		{
			OnItemDownLoad();
		}
	}
	

	*pResult = 0;
}


void CMyFTPDlg::OnNoneUpLoad()
{
	// TODO: 在此添加命令处理程序代码
	OnUpLoad();
}


void CMyFTPDlg::OnNoneCreateDirectory()
{
	// TODO: 在此添加命令处理程序代码
	OnCreateDirectory();
}


void CMyFTPDlg::OnNoneReturnToParentDir()
{
	// TODO: 在此添加命令处理程序代码
	OnReturnToParentDir();
}


void CMyFTPDlg::OnNoneReturnToRoot()
{
	// TODO: 返回根目录
	//if (m_ptrFtp->GetCurPath() == "/")
	//{
	//	MessageBox(L"已在根目录~", L"提示");
	//	return;
	//}

	if (!m_ptrFtp->SetRootPath())
	{
		//MessageBox(L"返回发生错误~", L"错误");
		MessageBox(L"已在根目录~", L"提示");
		return;
	}
	UpdataList();
}


void CMyFTPDlg::OnMenuSavePath()
{
	// TODO: 在此添加命令处理程序代码
	OnChangeFilePath();
}

void CMyFTPDlg::OnMainToorbarHome()
{
	OnNoneReturnToRoot();
}

void CMyFTPDlg::OnMainToorbarUp()
{
	OnReturnToParentDir();
}

void CMyFTPDlg::OnMainToorbarBack()
{
	//MessageBox(L"此功能开发中~");
	m_ptrFtp->BackPrePath();
	UpdataList();
}


void CMyFTPDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码

	CRect l_ct;
	GetClientRect(&l_ct);
	if (m_listCtrl.m_hWnd)
	{
		m_listCtrl.SetWindowPos(NULL, l_ct.left+14, l_ct.top+22, l_ct.Width() - 30, l_ct.Height() - 80, SWP_DRAWFRAME);
	}

	////添加状态栏信息
	m_Statusbar.MoveWindow(1, l_ct.bottom - 20, l_ct.right, 20);// 调整状态栏的位置和大小
	//m_Progress.MoveWindow(100, l_ct.bottom - 20, 300, 20);// 调整状态栏的位置和大小
	//m_Progress.SetPos(50);

	//初始化调用会报错
	if (firstUpdate)
	{
		CButton* pBtnLogout = (CButton*)GetDlgItem(IDC_BTN_Logout);//必须进行强制类型转换
		//HWND hBtnLogout = pBtnLogout->GetSafeHwnd();

		CButton* pBtnExit = (CButton*)GetDlgItem(IDC_BTN_Exit);//必须进行强制类型转换
		//HWND hBtnExit = pBtnLogout->GetSafeHwnd();

		CRect rc;
		m_listCtrl.GetWindowRect(&rc);

		int Wnd_W = rc.Width();//当然，也可以专直接用rc
		int Wnd_H = rc.Height();

		CRect bt;
		pBtnLogout->GetWindowRect(bt);
		pBtnLogout->MoveWindow(50, Wnd_H + 25, bt.Width(), bt.Height());

		pBtnExit->MoveWindow(Wnd_W - 130, Wnd_H + 25, bt.Width(), bt.Height());

		UpdataList();
	}
	firstUpdate = true;
}


void CMyFTPDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialog::OnOK();

	SetFocus();
}

void CMyFTPDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (MessageBox(L"确定要退出吗？", L"提示", MB_YESNO | MB_DEFBUTTON2) == IDYES)
	{
		m_isLogout = false;
		CDialog::OnCancel();
	}

}

void CMyFTPDlg::OnMainMenuUpLoad()
{
	// TODO: 在此添加命令处理程序代码
	OnUpLoad();
}


void CMyFTPDlg::OnMainMenuCreateDir()
{
	// TODO: 在此添加命令处理程序代码
	OnCreateDirectory();
}


void CMyFTPDlg::OnMainMenuOpenSaveDir()
{
	// TODO: 在此添加命令处理程序代码
	ShellExecute(NULL, NULL, _T("explorer"), m_strSavaPath, NULL, SW_SHOW);
}


void CMyFTPDlg::OnMainMenuUnlog()
{
	// TODO: 在此添加命令处理程序代码
	OnBnClickedBtnLogout();
}


void CMyFTPDlg::OnMainMenuExit()
{
	// TODO: 在此添加命令处理程序代码
	OnBnClickedBtnExit();
	
}


void CMyFTPDlg::OnMainMenuInstructions()
{
	// TODO: 在此添加命令处理程序代码
	MessageBox(TEXT("1. 只能下载文件,不可直接下载文件夹\n2. 只能删除空文件夹,若删除需清空\n3. 撤回功能尚未开放,敬请期待"),L"介绍");
}



