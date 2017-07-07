
// WebSpiderDlg.h : ͷ�ļ�
//

#pragma once
#include <set>

// CWebSpiderDlg �Ի���
class CWebSpiderDlg : public CDialogEx
{
// ����
public:
	CWebSpiderDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_WEBSPIDER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CString m_strRetPath;

	// ���ɵ���Ϣӳ�亯��
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
