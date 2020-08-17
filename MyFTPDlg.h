
// MyFTPDlg.h: 头文件
//

#pragma once

class MyFtpSession;

// CMyFTPDlg 对话框
class CMyFTPDlg : public CDialog
{
// 构造
public:
	CMyFTPDlg(CWnd* pParent = nullptr);	// 标准构造函数

	CMyFTPDlg(MyFtpSession&, CWnd* pParent = nullptr);	// 自定义构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYFTP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnLogout();
	afx_msg void OnBnClickedBtnExit();

public:
	bool m_isLogout = false;

	
private:
	MyFtpSession* m_ptrFtp; //ftp自定义类
	CImageList m_imgList;  //图标列表
	CString m_strSavaPath;

public:
	afx_msg void OnAboutShow();
	afx_msg void OnSmallICON();
	afx_msg void OnReportView();
	afx_msg void OnChangeFilePath();

	BOOL IsFileExistence(CString FileName,BOOL IsRemote = FALSE);
	void UpdataList();

private:
	CListCtrl m_listCtrl; //listCtrl控件
	CEdit m_editRename; //隐藏输入框

	CToolBar m_toolBar;
	CStatusBar  m_Statusbar;  //状态栏类

	CProgressCtrl m_Progress;

	int m_nRow = -1;
	BOOL m_isList = FALSE;
	BOOL firstUpdate = FALSE;
public:
	afx_msg void OnEnKillfocusEditRenameedit();
	afx_msg void OnNMR_RighClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuSmall();
	afx_msg void OnMenuList();
	afx_msg void OnItemSmallIcon();
	afx_msg void OnItemList();
	afx_msg void OnItemDownLoad();
	afx_msg void OnItemSaveAs();
	afx_msg void OnItemRenameFile();
	afx_msg void OnUpLoad();
	afx_msg void OnRemove();
	afx_msg void OnCreateDirectory();
	afx_msg void OnReturnToParentDir();
	afx_msg void OnNMDblclkList_ItemDClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNoneUpLoad();
	afx_msg void OnNoneCreateDirectory();
	afx_msg void OnNoneReturnToParentDir();
	afx_msg void OnNoneReturnToRoot();
	afx_msg void OnMenuSavePath();
	afx_msg void OnMainToorbarHome();
	afx_msg void OnMainToorbarUp();
	afx_msg void OnMainToorbarBack();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnOK();
	afx_msg void OnMainMenuUpLoad();
	afx_msg void OnMainMenuCreateDir();
	afx_msg void OnMainMenuOpenSaveDir();
	afx_msg void OnMainMenuUnlog();
	afx_msg void OnMainMenuExit();
	afx_msg void OnMainMenuInstructions();
	afx_msg void OnRefresh();
	virtual void OnCancel();
};
