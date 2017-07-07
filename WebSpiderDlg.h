
// WebSpiderDlg.h : 头文件
//

#pragma once
#include <set>

// CWebSpiderDlg 对话框
class CWebSpiderDlg : public CDialogEx
{
// 构造
public:
	CWebSpiderDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_WEBSPIDER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CString m_strRetPath;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();

	CWnd * GetDlgSafeItem(int nID);
	void InitialLayout();
	void DealOneURL(CString strURL, std::set<CString> & setKeyWord, CString strPath);
	std::set<CString> FindKeyWordURL(CString & strHtml, std::set<CString> & setKeyWord);

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnClearRetPath();
	afx_msg void OnBnClickedBtnOpenRetPath();
};
