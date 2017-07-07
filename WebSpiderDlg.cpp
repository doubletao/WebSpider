
// WebSpiderDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WebSpider.h"
#include "WebSpiderDlg.h"
#include "afxdialogex.h"
#include <sstream>
#include "GlobalFunction.h"
#include "TcpSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CWebSpiderDlg 对话框




CWebSpiderDlg::CWebSpiderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWebSpiderDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWebSpiderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWebSpiderDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START, &CWebSpiderDlg::OnBnClickedBtnStart)
END_MESSAGE_MAP()


// CWebSpiderDlg 消息处理程序

BOOL CWebSpiderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CWebSpiderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CWebSpiderDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CWebSpiderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CWebSpiderDlg::OnBnClickedBtnStart()
{
	CString strHost = _T("www.quora.com");
	CString strPath = _T("/hhp/9AD4");
	CTcpSocket tcpSocket;
	tcpSocket.Create();
	tcpSocket.Connect(strHost, 80);

	std::stringstream stream;
	stream << "GET " << CGlobalFunction::ConverCStringToStdString(strPath);// << "?" << get_content;
	stream << " HTTP/1.1\r\n";
	stream << "Host: " << CGlobalFunction::ConverCStringToStdString(strHost) << "\r\n";
	stream <<"User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9.2.3) Gecko/20100401 Firefox/3.6.3\r\n";
	stream <<"Connection:close\r\n\r\n";

	std::string strMsg = stream.str();

	tcpSocket.Send(strMsg.c_str(), strMsg.size());

	const int nBufSize = 1024;
	char cBuf[nBufSize];
	memset(cBuf, 0, nBufSize * sizeof(char));

	CFile file;
	if(file.Open(_T("a.txt"), CFile::modeCreate | CFile::modeWrite))
	{
		int nRet = 1;
		while(nRet > 0)
		{
			nRet = tcpSocket.Recv(cBuf, nBufSize - 1);
			if (nRet > 0 && nRet < nBufSize)
			{
				cBuf[nRet] = 0;
				file.Write(cBuf, nRet);
			}
			memset(cBuf, 0, nBufSize * sizeof(char));
		}
		file.Close();
	}

	tcpSocket.Close();
}
